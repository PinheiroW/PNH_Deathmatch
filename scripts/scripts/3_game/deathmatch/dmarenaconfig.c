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

typedef array<ref TFloatArray> TDMSpawnPointArray;

class DMPropConfig
{
    string type;
    ref TFloatArray position = new TFloatArray();
    ref TFloatArray orientation = new TFloatArray();
};

typedef array<ref DMPropConfig> TDMPropConfigArray;

class DMArenaConfig
{
    // Required
    string name;
    ref TFloatArray center = new TFloatArray();
    int radius;
    bool rectangular;
    int xSize;
    int zSize;

    // Optional
    int minimumPlayers;
    int maximumPlayers;
    ref TDMSpawnPointArray playerSpawns = new TDMSpawnPointArray();
    ref TDMSpawnPointArray crateSpawns = new TDMSpawnPointArray();
    ref TDMSpawnPointArray infectedSpawns = new TDMSpawnPointArray();
    ref TDMSpawnPointArray christmasSpawns = new TDMSpawnPointArray();
    ref TDMPropConfigArray props = new TDMPropConfigArray();

    // Special options
    bool dark;
    bool disableWalls;

    bool IsValid(string filename, int index)
    {
        if (name.Length() < 1 || name.Contains(" "))
        {
            Error(string.Format("%1: arenas[%2] has invalid name ('%3')", filename, index, name));
            return false;
        }

        bool valid = true;

        if (center.Count() != 3)
        {
            Error(string.Format("%1: Arena '%2' has invalid center!", filename, name));
            valid = false;
        }

        if (rectangular)
        {
            if (xSize < 1)
            {
                Error(
                    string.Format("%1: Arena '%2' has invalid xSize (%3)", filename, name, xSize));
                valid = false;
            }
            if (zSize < 1)
            {
                Error(
                    string.Format("%1: Arena '%2' has invalid xSize (%3)", filename, name, zSize));
                valid = false;
            }
        }
        else
        {
            if (radius < 1)
            {
                Error(
                    string.Format(
                        "%1: Arena '%2' has invalid radius (%3)", filename, name, radius));
                valid = false;
            }
        }

        minimumPlayers = Math.Max(minimumPlayers, 0);
        if (maximumPlayers == 0) maximumPlayers = int.MAX;

        if (maximumPlayers < minimumPlayers)
        {
            Error(
                string.Format(
                    "%1: Arena '%2' maximumPlayers (%3) cannot be less than minimumPlayers (%4)",
                    filename, name, maximumPlayers, minimumPlayers));
            valid = false;
        }

        if (!IsValidSpawnsArray(filename, name, playerSpawns, "playerSpawns"))
        {
            valid = false;
        }

        if (!IsValidSpawnsArray(filename, name, crateSpawns, "crateSpawns"))
        {
            valid = false;
        }

        if (!IsValidSpawnsArray(filename, name, infectedSpawns, "infectedSpawns"))
        {
            valid = false;
        }

        if (!IsValidSpawnsArray(filename, name, christmasSpawns, "christmasSpawns"))
        {
            valid = false;
        }

        for (int i = 0; i < props.Count(); i++)
        {
            if (props[i].type.Length() < 1)
            {
                Error(
                    string.Format(
                        "%1: Arena '%2' props[%3] has invalid type ('%4')",
                        filename, name, i, props[i].type));
                valid = false;
            }

            if (props[i].position.Count() != 3)
            {
                Error(
                    string.Format(
                        "%1: Arena '%2' props[%3] has invalid position", filename, name, i));
                valid = false;
            }

            if (props[i].orientation.Count() != 3)
            {
                Error(
                    string.Format(
                        "%1: Arena '%2' props[%3] has invalid orientation", filename, name, i));
                valid = false;
            }
        }

        return valid;
    }

    private static bool IsValidSpawnsArray(
        string filename, string arenaName, TDMSpawnPointArray spawns, string arrayName)
    {
        bool valid = true;

        for (int i = 0; i < spawns.Count(); i++)
        {
            if (spawns[i].Count() != 3)
            {
                Error(
                    string.Format(
                        "%1: Arena '%2' %3[%4] is invalid!", filename, arenaName, arrayName, i));
                valid = false;
            }
        }

        return valid;
    }
};

// vim:ft=enforce
