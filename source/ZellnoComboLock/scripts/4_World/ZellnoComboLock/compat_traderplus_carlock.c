#ifdef CLDefine
// O TraderPlus CarLock oferece ActionDetachCarLock a administradores mesmo
// quando CarLockTargetHelper nao encontra um veiculo. Bloqueamos somente o
// falso positivo sobre uma Fence protegida pelo Zellno Combo Lock.
class ZellnoTraderPlusCarLockCompatibility
{
    static bool HasZellnoLock(ActionTarget target)
    {
        if (!target)
            return false;

        Object targetObject = target.GetObject();
        if (Zellno_CombinationLock4.Cast(targetObject))
            return true;

        Object targetParent = target.GetParent();
        if (Zellno_CombinationLock4.Cast(targetParent))
            return true;

        Fence fence = Fence.Cast(targetObject);
        if (!fence)
            fence = Fence.Cast(targetParent);

        if (fence && Zellno_CombinationLock4.Cast(fence.GetCombinationLock()))
            return true;

#ifdef RAG_HUNTING_CABIN
        rag_hunting_cabin cabin = rag_hunting_cabin.Cast(targetObject);
        if (!cabin)
            cabin = rag_hunting_cabin.Cast(targetParent);

        EntityAI targetEntity = EntityAI.Cast(targetObject);
        if (!cabin && targetEntity)
            cabin = rag_hunting_cabin.Cast(targetEntity.GetHierarchyParent());

        EntityAI parentEntity = EntityAI.Cast(targetParent);
        if (!cabin && parentEntity)
            cabin = rag_hunting_cabin.Cast(parentEntity.GetHierarchyParent());

        if (cabin && ZellnoHuntingCabinCompatibility.GetZellnoLock(cabin))
            return true;
#endif

        return false;
    }
}

modded class ActionDetachCarLock
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (ZellnoTraderPlusCarLockCompatibility.HasZellnoLock(target))
            return false;

        return super.ActionCondition(player, target, item);
    }
}
#endif
