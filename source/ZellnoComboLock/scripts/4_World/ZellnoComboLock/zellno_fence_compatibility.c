class ZellnoFenceCompatibility
{
    static Fence ResolveFence(ActionTarget target)
    {
        if (!target)
            return null;

        Object targetObject = target.GetObject();
        Fence fence = Fence.Cast(targetObject);
        if (fence)
            return fence;

        Object targetParent = target.GetParent();
        fence = Fence.Cast(targetParent);
        if (fence)
            return fence;

        EntityAI targetEntity = EntityAI.Cast(targetObject);
        if (targetEntity)
        {
            fence = Fence.Cast(targetEntity.GetHierarchyParent());
            if (fence)
                return fence;
        }

        EntityAI parentEntity = EntityAI.Cast(targetParent);
        if (parentEntity)
            return Fence.Cast(parentEntity.GetHierarchyParent());

        return null;
    }

    static bool HasZellnoLock(ActionTarget target)
    {
        Fence fence = ResolveFence(target);
        if (!fence)
            return false;

        return Zellno_CombinationLock4.Cast(fence.GetCombinationLock()) != null;
    }

    static bool IsRaGBaseBuildingFence(Fence fence)
    {
        if (!fence)
            return false;

        // Runtime classname detection keeps RaG_BaseBuilding optional.  No RaG
        // class is referenced at compile time, so ZellnoComboLock still loads
        // normally on servers where RaG is absent.
        return fence.GetType().IndexOf("RaG_BB_") == 0;
    }

    static bool HasInteractionSelection(Fence fence, ActionTarget target)
    {
        if (!fence || !target)
            return false;

        array<string> selections = new array<string>;
        fence.GetActionComponentNameList(target.GetComponentIndex(), selections);

        if (selections.Find("gate_interact") != -1)
            return true;

        // RaG doors use their own model selections (for example the rotating
        // wall interaction) instead of vanilla gate_interact.
        return IsRaGBaseBuildingFence(fence);
    }

    static bool IsClosedUsableGate(Fence fence)
    {
        if (!fence || !fence.CanUseConstruction() || fence.IsOpened())
            return false;

        if (IsRaGBaseBuildingFence(fence))
            return true;

        return fence.HasHinges();
    }

}
