class CfgPatches
{
    class ZellnoComboLock
    {
        units[] =
        {
            "Zellno_CombinationLock4"
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Gear_Camping",
            "DZ_Scripts"
        };
    };
};

class CfgVehicles
{
    class CombinationLock4;

    class Zellno_CombinationLock4 : CombinationLock4
    {
        scope = 2;
        displayName = "Zellno Combination Lock";
        descriptionShort = "A four-dial combination lock prepared for Zellno access control.";
    };
};

class CfgMods
{
    class ZellnoComboLock
    {
        dir = "ZellnoComboLock";
        name = "Zellno Combo Lock";
        author = "Zellno";
        version = "1.0.0";
        type = "mod";

        dependencies[] =
        {
            "Game",
            "World"
        };

        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "ZellnoComboLock/scripts/4_World"
                };
            };
        };
    };
};
