class ActionZellnoOpenLockedFence : ActionInteractBase
{
    protected Zellno_CombinationLock4 m_ZellnoLock;

    void ActionZellnoOpenLockedFence()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Usar Zellno Combo Lock";
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

        Fence fence = ZellnoFenceCompatibility.ResolveFence(target);
        if (!ZellnoFenceCompatibility.IsClosedUsableGate(fence))
            return false;

        m_ZellnoLock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
        if (!m_ZellnoLock || !m_ZellnoLock.IsLocked() || m_ZellnoLock.GetHierarchyParent() != fence)
            return false;

        return ZellnoFenceCompatibility.HasInteractionSelection(fence, target);
    }

    override void Start(ActionData action_data)
    {
        super.Start(action_data);

        if (!g_Game || !g_Game.IsClient())
            return;

        if (!action_data || !action_data.m_Target)
            return;

        Fence fence = ZellnoFenceCompatibility.ResolveFence(action_data.m_Target);
        if (!fence)
            return;

        Zellno_CombinationLock4 lock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
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

        Fence fence = ZellnoFenceCompatibility.ResolveFence(action_data.m_Target);
        if (!ZellnoFenceCompatibility.IsClosedUsableGate(fence))
            return;

        Zellno_CombinationLock4 lock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
        if (lock)
            lock.ZellnoOpenIfAuthorized(action_data.m_Player, fence);
    }
}

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionZellnoOpenLockedFence);
        actions.Insert(ActionZellnoCloseFence);
        actions.Insert(ActionZellnoRemoveLock);
#ifdef RAG_HUNTING_CABIN
        actions.Insert(ActionZellnoOpenHuntingCabin);
        actions.Insert(ActionZellnoCloseHuntingCabin);
        actions.Insert(ActionZellnoRemoveHuntingCabinLock);
#endif
    }
}

modded class Fence
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionZellnoOpenLockedFence);
        AddAction(ActionZellnoCloseFence);
        AddAction(ActionZellnoRemoveLock);
    }
}
