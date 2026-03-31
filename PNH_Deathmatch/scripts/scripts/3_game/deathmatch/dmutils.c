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

class DMUtils
{
    private void DMUtils() {}
    private void ~DMUtils() {}

    static string BoolToOnOff(bool value)
    {
        if (value)
        {
            return "ON";
        }
        else
        {
            return "OFF";
        }
    }

    static string s(int count, string pluralSuffix = "s")
    {
        if (count == 1)
        {
            return "";
        }

        return pluralSuffix;
    }

    static string PadLeft(string text, int size, string paddingChar = " ")
    {
        while (text.Length() < size)
        {
            text = paddingChar + text;
        }
        return text;
    }

    static string Join(TStringArray items, string sep)
    {
        string result;
        foreach (string item : items)
        {
            if (result.Length() > 0)
            {
                result += sep;
            }
            result += item;
        }
        return result;
    }

    static vector VectorFromArray(TFloatArray a)
    {
        return Vector(a[0], a[1], a[2]);
    }

    static TVectorArray VectorsFromArray(array<ref TFloatArray> positions)
    {
        TVectorArray result = new TVectorArray();

        foreach (TFloatArray pos : positions)
        {
            result.Insert(DMUtils.VectorFromArray(pos));
        }

        return result;
    }

    static vector RandomYaw()
    {
        return Vector(Math.RandomFloat(0, 360), 0, 0);
    }

    static bool ObjectSpawnable(string type)
    {
        return g_Game.ConfigGetInt(string.Format("CfgVehicles %1 scope", type)) == 2;
    }

    static bool WeaponSpawnable(string type)
    {
        return g_Game.ConfigGetInt(string.Format("CfgWeapons %1 scope", type)) == 2;
    }

    static const int DM_PROP = ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY;

    static Object SpawnObject(string type, vector position, vector orientation, int flags)
    {
        Object obj = g_Game.CreateObjectEx(type, position, flags, RF_IGNORE);

        if (!obj)
        {
            Error(
                string.Format("Failed to spawn object '%1' at %2", type, position.ToString(true)));
            return null;
        }

        obj.SetOrientation(orientation);

        return obj;
    }

    static Object SpawnProp(string type, vector position, vector orientation)
    {
        return SpawnObject(type, position, orientation, DM_PROP);
    }

    static int GetPlayerCount()
    {
        array<PlayerIdentity> ids = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(ids);

        return ids.Count();
    }

    static string DateStamp()
    {
        int year;
        int month;
        int day;

        GetYearMonthDayUTC(year, month, day);

        return string.Format("%1-%2-%3", year, month, day);
    }
}

// vim:ft=enforce
