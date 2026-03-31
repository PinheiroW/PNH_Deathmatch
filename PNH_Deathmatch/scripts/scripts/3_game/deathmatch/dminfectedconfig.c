/* Copyright (c) 2021-2025 The Crimson Zamboni
 *
 * This Software is licensed for non-commercial use only, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software may not be monetized. This means that you cannot use it on a
 * server with access limited only to paying players, or a server with extra
 * privileges, such as priority queue slots, granted to paying players.
 *
 * The Software is not authorized for posting on Steam, except under the Steam
 * account https://steamcommunity.com/id/DirtySanchez/
 */
class DMDefaultInfectedConfig
{
    static private ref TStringArray TYPES;

    static TStringArray AllInfectedTypes()
    {
        if (!TYPES)
        {
            TYPES = new TStringArray();

            int children_count = g_Game.ConfigGetChildrenCount("CfgVehicles");
            string child_name;

            for (int i = 0; i < children_count; i++)
            {
                g_Game.ConfigGetChildName("CfgVehicles", i, child_name);
                if (DMUtils.ObjectSpawnable(child_name) && g_Game.IsKindOf(child_name, "DayZInfected"))
                {
                    if (child_name != "ZmbM_Santa" && child_name != "ZmbM_Mummy")
                    {
                        TYPES.Insert(child_name);
                    }
                }
            }

            TYPES.ShuffleArray();
        }

        return TYPES;
    }

    static void SetInfectedTypes(TStringArray types)
    {
        types.InsertArray(DMDefaultInfectedConfig.AllInfectedTypes());
    }
}

// vim:ft=enforce
