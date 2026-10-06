class Zellno_CombinationLock4 : CombinationLock4
{
    static const int ZELLNO_MAX_AUTHORIZED_PLAYERS = 64;
    static const int ZELLNO_STORAGE_VERSION = 2;
    static const int ZELLNO_RPC_SUBMIT_CODE = 734201;
    static const int ZELLNO_RPC_CODE_RESULT = 734202;
    static const int ZELLNO_RESULT_ACCEPTED = 2;
    static const int ZELLNO_RESULT_REJECTED = 3;
    static const int ZELLNO_RESULT_WAIT = 4;
    static const int ZELLNO_MAX_DISTANCE_METERS = 3;
#ifdef RAG_HUNTING_CABIN
    static const int ZELLNO_HUNTING_CABIN_MAX_DISTANCE_METERS = 5;
#endif
    static const int ZELLNO_ATTEMPT_INTERVAL_MS = 1000;

    protected bool m_ZellnoConfigured;
    protected string m_ZellnoCode;
    protected ref array<string> m_ZellnoAuthorizedPlayerIds;
    protected ref map<string, int> m_ZellnoLastAttemptByPlayerId;

    void Zellno_CombinationLock4()
    {
        m_ZellnoAuthorizedPlayerIds = new array<string>;
        m_ZellnoLastAttemptByPlayerId = new map<string, int>;
        RegisterNetSyncVariableBool("m_ZellnoConfigured");
    }

    override void SetActions()
    {
        super.SetActions();
        RemoveAction(ActionNextCombinationLockDial);
        RemoveAction(ActionDialCombinationLock);
        RemoveAction(ActionNextCombinationLockDialOnTarget);
        RemoveAction(ActionDialCombinationLockOnTarget);
    }

    bool ZellnoHasCode()
    {
        return m_ZellnoConfigured;
    }

    bool ZellnoIsPlayerAuthorized(PlayerBase player)
    {
        return player && player.GetIdentity() && m_ZellnoAuthorizedPlayerIds.Find(player.GetIdentity().GetId()) != -1;
    }

    bool ZellnoCanPlayerRemove(PlayerBase player, Fence fence)
    {
        if (!ZellnoContextIsValid(player, fence))
            return false;

        return !m_ZellnoConfigured || ZellnoIsPlayerAuthorized(player);
    }

    bool ZellnoRemoveServer(PlayerBase player, Fence fence)
    {
        if (!ZellnoCanPlayerRemove(player, fence))
            return false;

        // Chamada explicita ao comportamento vanilla: destrava o slot e
        // deposita o cadeado intacto junto ao portao.
        super.UnlockServer(player, fence);
        Print(string.Format("[ZellnoComboLock] Cadeado removido com alicate: player=%1", player.GetIdentity().GetId()));
        return true;
    }

#ifdef RAG_HUNTING_CABIN
    bool ZellnoCanPlayerRemoveHuntingCabin(PlayerBase player, rag_hunting_cabin cabin)
    {
        if (!ZellnoHuntingCabinContextIsValid(player, cabin))
            return false;

        return !m_ZellnoConfigured || ZellnoIsPlayerAuthorized(player);
    }

    bool ZellnoRemoveHuntingCabinServer(PlayerBase player, rag_hunting_cabin cabin)
    {
        if (!ZellnoCanPlayerRemoveHuntingCabin(player, cabin))
            return false;

        super.UnlockServer(player, cabin);
        Print(string.Format("[ZellnoComboLock] Cadeado removido da RaG Hunting Cabin com alicate: player=%1", player.GetIdentity().GetId()));
        return true;
    }
#endif

    protected bool ZellnoAuthorizePlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return false;

        string playerId = player.GetIdentity().GetId();
        if (playerId == "")
            return false;

        if (m_ZellnoAuthorizedPlayerIds.Find(playerId) != -1)
            return true;

        if (m_ZellnoAuthorizedPlayerIds.Count() >= ZELLNO_MAX_AUTHORIZED_PLAYERS)
            return false;

        m_ZellnoAuthorizedPlayerIds.Insert(playerId);
        return true;
    }

    protected bool ZellnoContextIsValid(PlayerBase player, Fence fence)
    {
        if (!g_Game.IsServer() || !player || !player.GetIdentity() || !player.IsAlive())
            return false;

        if (!fence || !fence.CanUseConstruction() || !fence.HasHinges() || fence.IsOpened())
            return false;

        if (!IsLockAttached() || !IsLocked() || GetHierarchyParent() != fence || fence.GetCombinationLock() != this)
            return false;

        return vector.Distance(player.GetPosition(), fence.GetPosition()) <= ZELLNO_MAX_DISTANCE_METERS;
    }

#ifdef RAG_HUNTING_CABIN
    protected bool ZellnoHuntingCabinContextIsValid(PlayerBase player, rag_hunting_cabin cabin)
    {
        if (!g_Game.IsServer() || !player || !player.GetIdentity() || !player.IsAlive())
            return false;

        if (!cabin)
            return false;

        if (cabin.IsOpened())
            return false;

        // IsLockAttached() follows the vanilla Fence ownership path and is not
        // reliable for the dedicated RaG Hunting Cabin attachment.  Parent,
        // slot identity and locked state provide the authoritative context.
        if (!IsLocked() || GetHierarchyParent() != cabin || cabin.GetCombinationLock() != this)
            return false;

        // Attached items inherit the Hunting Cabin world origin, so the lock
        // and cabin report the same position.  The dedicated five-metre limit
        // covers the real door interaction position while remaining local to
        // this structure; vanilla Fence keeps the stricter three-metre limit.
        float lockDistance = vector.Distance(player.GetPosition(), GetPosition());
        if (lockDistance > ZELLNO_HUNTING_CABIN_MAX_DISTANCE_METERS)
            return false;

        return true;
    }
#endif

    protected bool ZellnoCodeIsValid(string code)
    {
        int codeLength = code.Length();
        if (codeLength < 4 || codeLength > 6)
            return false;

        for (int i = 0; i < codeLength; ++i)
        {
            if ("0123456789".IndexOf(code.Get(i)) == -1)
                return false;
        }

        return true;
    }

    protected void ZellnoSendResult(PlayerIdentity identity, int result)
    {
        if (!identity)
            return;

        PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
        if (player)
            g_Game.RPCSingleParam(player, ZELLNO_RPC_CODE_RESULT, new Param2<Object, int>(this, result), true, identity);
    }

    void ZellnoOpenIfAuthorized(PlayerBase player, Fence fence)
    {
        if (!m_ZellnoConfigured || !ZellnoContextIsValid(player, fence) || !ZellnoIsPlayerAuthorized(player))
            return;

        fence.OpenFence();
        Print(string.Format("[ZellnoComboLock] Abertura autorizada de Fence: player=%1", player.GetIdentity().GetId()));
        ZellnoSendResult(player.GetIdentity(), ZELLNO_RESULT_ACCEPTED);
    }

#ifdef RAG_HUNTING_CABIN
    void ZellnoOpenHuntingCabinIfAuthorized(PlayerBase player, rag_hunting_cabin cabin)
    {
        if (!m_ZellnoConfigured || !ZellnoHuntingCabinContextIsValid(player, cabin) || !ZellnoIsPlayerAuthorized(player))
            return;

        cabin.OpenHuntCabDoor();
        Print(string.Format("[ZellnoComboLock] Abertura autorizada de RaG Hunting Cabin: player=%1", player.GetIdentity().GetId()));
        ZellnoSendResult(player.GetIdentity(), ZELLNO_RESULT_ACCEPTED);
    }
#endif

    void ZellnoSubmitCodeServer(PlayerIdentity sender, string code)
    {
        if (!sender)
            return;

        PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
        Fence fence = Fence.Cast(GetHierarchyParent());
#ifdef RAG_HUNTING_CABIN
        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(GetHierarchyParent());
        if (!ZellnoContextIsValid(player, fence) && !ZellnoHuntingCabinContextIsValid(player, cabin))
        {
            ZellnoSendResult(sender, ZELLNO_RESULT_REJECTED);
            return;
        }
#else
        if (!ZellnoContextIsValid(player, fence))
            return;
#endif

        string playerId = sender.GetId();
        int now = g_Game.GetTime();
        int lastAttempt;
        if (m_ZellnoLastAttemptByPlayerId.Find(playerId, lastAttempt) && now - lastAttempt < ZELLNO_ATTEMPT_INTERVAL_MS)
        {
            ZellnoSendResult(sender, ZELLNO_RESULT_WAIT);
            return;
        }

        m_ZellnoLastAttemptByPlayerId.Set(playerId, now);

        if (!ZellnoCodeIsValid(code))
        {
            ZellnoSendResult(sender, ZELLNO_RESULT_REJECTED);
            return;
        }

        if (!m_ZellnoConfigured)
        {
            m_ZellnoCode = code;
            m_ZellnoConfigured = true;
            m_ZellnoAuthorizedPlayerIds.Clear();

            if (!ZellnoAuthorizePlayer(player))
            {
                m_ZellnoCode = "";
                m_ZellnoConfigured = false;
                SetSynchDirty();
                ZellnoSendResult(sender, ZELLNO_RESULT_REJECTED);
                return;
            }

            SetSynchDirty();
            Print(string.Format("[ZellnoComboLock] Senha configurada: player=%1", playerId));
            ZellnoSendResult(sender, ZELLNO_RESULT_ACCEPTED);
            return;
        }

        if (m_ZellnoCode != code || !ZellnoAuthorizePlayer(player))
        {
            ZellnoSendResult(sender, ZELLNO_RESULT_REJECTED);
            return;
        }

        ZellnoSendResult(sender, ZELLNO_RESULT_ACCEPTED);
        if (fence)
        {
            fence.OpenFence();
            Print(string.Format("[ZellnoComboLock] Senha aceita e Fence aberta: player=%1", playerId));
        }
#ifdef RAG_HUNTING_CABIN
        else if (cabin)
        {
            cabin.OpenHuntCabDoor();
            Print(string.Format("[ZellnoComboLock] Senha aceita e RaG Hunting Cabin aberta: player=%1", playerId));
        }
#endif
    }

    override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
    {
        super.OnItemLocationChanged(old_owner, new_owner);

        if (g_Game.IsServer() && IsInitialized() && new_owner && new_owner.IsInherited(BaseBuildingBase) && old_owner != new_owner)
        {
            m_ZellnoConfigured = false;
            m_ZellnoCode = "";
            m_ZellnoAuthorizedPlayerIds.Clear();
            SetSynchDirty();
        }
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(ZELLNO_STORAGE_VERSION);
        ctx.Write(m_ZellnoConfigured);
        ctx.Write(m_ZellnoCode);

        int count = m_ZellnoAuthorizedPlayerIds.Count();
        ctx.Write(count);
        for (int i = 0; i < count; ++i)
            ctx.Write(m_ZellnoAuthorizedPlayerIds[i]);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;

        int storageVersion;
        if (!ctx.Read(storageVersion))
            return false;

        int count;
        if (storageVersion == 1)
        {
            if (!ctx.Read(count) || count < 0 || count > ZELLNO_MAX_AUTHORIZED_PLAYERS)
                return false;

            for (int oldIndex = 0; oldIndex < count; ++oldIndex)
            {
                string discardedPlayerId;
                if (!ctx.Read(discardedPlayerId))
                    return false;
            }

            m_ZellnoConfigured = false;
            m_ZellnoCode = "";
            m_ZellnoAuthorizedPlayerIds.Clear();
            return true;
        }

        if (storageVersion != ZELLNO_STORAGE_VERSION)
            return false;

        if (!ctx.Read(m_ZellnoConfigured) || !ctx.Read(m_ZellnoCode) || !ctx.Read(count))
            return false;

        if (count < 0 || count > ZELLNO_MAX_AUTHORIZED_PLAYERS)
            return false;

        if (!m_ZellnoConfigured)
            m_ZellnoCode = "";

        m_ZellnoAuthorizedPlayerIds.Clear();
        for (int i = 0; i < count; ++i)
        {
            string playerId;
            if (!ctx.Read(playerId))
                return false;

            if (playerId != "" && m_ZellnoAuthorizedPlayerIds.Find(playerId) == -1)
                m_ZellnoAuthorizedPlayerIds.Insert(playerId);
        }

        return true;
    }

    override void UnlockServer(EntityAI player, EntityAI parent)
    {
        // The inherited analogue dial must not authorize or unlock a Zellno lock.
    }
}

modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == Zellno_CombinationLock4.ZELLNO_RPC_SUBMIT_CODE && g_Game.IsServer())
        {
            if (!sender || sender.GetPlayer() != this)
                return;

            Param2<Object, string> submitParams = new Param2<Object, string>(null, "");
            if (!ctx.Read(submitParams))
                return;

            Zellno_CombinationLock4 submitLock = Zellno_CombinationLock4.Cast(submitParams.param1);
            if (submitLock)
                submitLock.ZellnoSubmitCodeServer(sender, submitParams.param2);
            return;
        }

        if (rpc_type == Zellno_CombinationLock4.ZELLNO_RPC_CODE_RESULT && g_Game.IsClient())
        {
            Param2<Object, int> resultParams = new Param2<Object, int>(null, 0);
            if (!ctx.Read(resultParams))
                return;

            Zellno_CombinationLock4 resultLock = Zellno_CombinationLock4.Cast(resultParams.param1);
            if (resultLock)
                ZellnoComboLockMenu.HandleServerResult(resultLock, resultParams.param2);
        }
    }
}
