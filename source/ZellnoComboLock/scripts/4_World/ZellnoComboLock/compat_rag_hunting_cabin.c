#ifdef RAG_HUNTING_CABIN
class ZellnoHuntingCabinCompatibility
{
    static bool HasDoorSelection(rag_hunting_cabin cabin, ActionTarget target)
    {
        if (!cabin || !target)
            return false;

        return cabin.GetActionComponentName(target.GetComponentIndex()) == "cabin_door";
    }

    static Zellno_CombinationLock4 GetZellnoLock(rag_hunting_cabin cabin)
    {
        if (!cabin)
            return null;

        return Zellno_CombinationLock4.Cast(cabin.GetCombinationLock());
    }

}

class ActionZellnoOpenHuntingCabin : ActionInteractBase
{
    protected Zellno_CombinationLock4 m_ZellnoLock;

    void ActionZellnoOpenHuntingCabin()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Open (Zellno)";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
    }

    override string GetText()
    {
        if (m_ZellnoLock && !m_ZellnoLock.ZellnoHasCode())
            return "Set Password (Zellno)";

        return "Open (Zellno)";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        m_ZellnoLock = null;

        if (!player || !target)
            return false;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
        if (!cabin || cabin.IsOpened() || !ZellnoHuntingCabinCompatibility.HasDoorSelection(cabin, target))
            return false;

        m_ZellnoLock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        return m_ZellnoLock && m_ZellnoLock.IsLocked() && m_ZellnoLock.GetHierarchyParent() == cabin;
    }

    override void Start(ActionData action_data)
    {
        super.Start(action_data);

        if (!g_Game || !g_Game.IsClient() || !action_data || !action_data.m_Target)
            return;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(action_data.m_Target.GetObject());
        Zellno_CombinationLock4 lock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        if (!lock)
            return;

        if (lock.ZellnoHasCode())
            ZellnoComboLockMenu.ScheduleUnlockMenu(lock);
        else
            ZellnoComboLockMenu.OpenForLock(lock, true);
    }

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        if (!action_data || !action_data.m_Player || !action_data.m_Target)
            return;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(action_data.m_Target.GetObject());
        Zellno_CombinationLock4 lock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        if (lock)
            lock.ZellnoOpenHuntingCabinIfAuthorized(action_data.m_Player, cabin);
    }
}

class ActionZellnoCloseHuntingCabin : ActionInteractBase
{
    void ActionZellnoCloseHuntingCabin()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Close (Zellno)";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target)
            return false;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
        if (!cabin || !cabin.CanCloseDoor() || !ZellnoHuntingCabinCompatibility.HasDoorSelection(cabin, target))
            return false;

        Zellno_CombinationLock4 lock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        return lock && lock.GetHierarchyParent() == cabin;
    }

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        if (!action_data || !action_data.m_Target)
            return;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(action_data.m_Target.GetObject());
        Zellno_CombinationLock4 lock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        if (cabin && cabin.CanCloseDoor() && lock && lock.GetHierarchyParent() == cabin)
            cabin.CloseHuntCabDoor();
    }
}

class ActionZellnoRemoveHuntingCabinLockCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(3.0);
    }
}

class ActionZellnoRemoveHuntingCabinLock : ActionContinuousBase
{
    protected Zellno_CombinationLock4 m_ZellnoLock;

    void ActionZellnoRemoveHuntingCabinLock()
    {
        m_CallbackClass = ActionZellnoRemoveHuntingCabinLockCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Remove Lock (Zellno)";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        m_ZellnoLock = null;

        if (!player || !target || !item || !item.IsKindOf("Pliers"))
            return false;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
        if (!cabin || cabin.IsOpened() || !ZellnoHuntingCabinCompatibility.HasDoorSelection(cabin, target))
            return false;

        m_ZellnoLock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        if (!m_ZellnoLock || !m_ZellnoLock.IsLocked() || m_ZellnoLock.GetHierarchyParent() != cabin)
            return false;

        if (g_Game && g_Game.IsServer())
            return m_ZellnoLock.ZellnoCanPlayerRemoveHuntingCabin(player, cabin);

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target || !action_data.m_MainItem)
            return;

        if (!action_data.m_MainItem.IsKindOf("Pliers") || action_data.m_MainItem.IsRuined())
            return;

        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(action_data.m_Target.GetObject());
        Zellno_CombinationLock4 lock = ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin);
        if (lock && lock.ZellnoRemoveHuntingCabinServer(action_data.m_Player, cabin))
            action_data.m_MainItem.DecreaseHealth(UADamageApplied.DISMANTLE, false);
    }
}

modded class rag_hunting_cabin
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionZellnoOpenHuntingCabin);
        AddAction(ActionZellnoCloseHuntingCabin);
    }
}

modded class ActionDialCombinationLockOnHuntingCabin
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target)
        {
            rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
            if (ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionNextCombinationLockDialOnHuntingCabin
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target)
        {
            rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
            if (ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionOpenHuntingCabin
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target)
        {
            rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
            if (ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionCloseHuntingCabin
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target)
        {
            rag_hunting_cabin cabin = rag_hunting_cabin.Cast(target.GetObject());
            if (ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}
#endif
