class CfgPatches
{
    class ZellnoComboLock_RaGCompat
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "ZellnoComboLock",
            "RaG_BaseBuilding"
        };
    };
};

class CfgMods
{
    class ZellnoComboLock_RaGCompat
    {
        dir = "ZellnoComboLock_RaGCompat";
        name = "Zellno Combo Lock - RaG Compatibility";
        author = "Zellno";
        version = "1.0.0";
        type = "mod";

        dependencies[] =
        {
            "World"
        };

        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "ZellnoComboLock_RaGCompat/scripts/4_World"
                };
            };
        };
    };
};
