// This addon requires both ZellnoComboLock and RaG_BaseBuilding. Its explicit
// CfgPatches dependencies guarantee that this layer is compiled after the RaG
// 4_World action overrides while the Zellno core remains usable without RaG.

modded class ActionDialCombinationLockOnTarget
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (ZellnoFenceCompatibility.HasZellnoLock(target))
            return false;

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionNextCombinationLockDialOnTarget
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (ZellnoFenceCompatibility.HasZellnoLock(target))
            return false;

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionCloseFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (ZellnoFenceCompatibility.HasZellnoLock(target))
            return false;

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionOpenFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (ZellnoFenceCompatibility.HasZellnoLock(target))
            return false;

        return super.ActionCondition(player, target, item);
    }
}
