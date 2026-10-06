class ActionZellnoRemoveLockCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(3.0);
    }
}

class ActionZellnoRemoveLock : ActionContinuousBase
{
    protected Zellno_CombinationLock4 m_ZellnoLock;

    void ActionZellnoRemoveLock()
    {
        m_CallbackClass = ActionZellnoRemoveLockCB;
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

        Fence fence = ZellnoFenceCompatibility.ResolveFence(target);
        if (!ZellnoFenceCompatibility.IsClosedUsableGate(fence))
            return false;

        m_ZellnoLock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
        if (!m_ZellnoLock || !m_ZellnoLock.IsLocked() || m_ZellnoLock.GetHierarchyParent() != fence)
            return false;

        if (!ZellnoFenceCompatibility.HasInteractionSelection(fence, target))
            return false;

        // The authorized-player list exists only on the server.
        // The client may display the action, but completion is always
        // authoritatively validated again before releasing the lock.
        if (g_Game && g_Game.IsServer())
            return m_ZellnoLock.ZellnoCanPlayerRemove(player, fence);

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target || !action_data.m_MainItem)
            return;

        if (!action_data.m_MainItem.IsKindOf("Pliers") || action_data.m_MainItem.IsRuined())
            return;

        Fence fence = ZellnoFenceCompatibility.ResolveFence(action_data.m_Target);
        if (!fence)
            return;

        Zellno_CombinationLock4 lock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
        if (!lock)
            return;

        if (lock.ZellnoRemoveServer(action_data.m_Player, fence))
            action_data.m_MainItem.DecreaseHealth(UADamageApplied.DISMANTLE, false);
    }

    override string GetAdminLogMessage(ActionData action_data)
    {
        return " removed Zellno Combo Lock with " + action_data.m_MainItem.GetDisplayName();
    }
}

// Actions using a main item are discovered through the action list of the
// item in hand. Registering it only on Fence does not expose the action.
modded class Pliers
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionZellnoRemoveLock);
#ifdef RAG_HUNTING_CABIN
        AddAction(ActionZellnoRemoveHuntingCabinLock);
#endif
    }
}
