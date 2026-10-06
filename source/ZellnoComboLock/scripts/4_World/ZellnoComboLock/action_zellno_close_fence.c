class ActionZellnoCloseFence : ActionInteractBase
{
    protected ref NoiseParams m_ZellnoNoiseParams;

    void ActionZellnoCloseFence()
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

        Fence fence = ZellnoFenceCompatibility.ResolveFence(target);
        if (!fence || !fence.CanUseConstruction() || !fence.CanCloseFence())
            return false;

        Zellno_CombinationLock4 lock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
        if (!lock || lock.GetHierarchyParent() != fence)
            return false;

        return ZellnoFenceCompatibility.HasInteractionSelection(fence, target);
    }

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        if (!action_data || !action_data.m_Target)
            return;

        Fence fence = ZellnoFenceCompatibility.ResolveFence(action_data.m_Target);
        if (!fence || !fence.CanCloseFence())
            return;

        Zellno_CombinationLock4 lock = Zellno_CombinationLock4.Cast(fence.GetCombinationLock());
        if (!lock || lock.GetHierarchyParent() != fence)
            return;

        fence.CloseFence();
    }

    override void OnEndServer(ActionData action_data)
    {
        super.OnEndServer(action_data);

        if (!action_data || !action_data.m_Player || !action_data.m_Target)
            return;

        m_ZellnoNoiseParams = new NoiseParams();
        m_ZellnoNoiseParams.LoadFromPath("CfgVehicles SurvivorBase NoiseActionDefault");

        NoiseSystem noise = g_Game.GetNoiseSystem();
        if (noise)
            noise.AddNoisePos(action_data.m_Player, action_data.m_Target.GetObject().GetPosition(), m_ZellnoNoiseParams, NoiseAIEvaluate.GetNoiseReduction(g_Game.GetWeather()));
    }
}
