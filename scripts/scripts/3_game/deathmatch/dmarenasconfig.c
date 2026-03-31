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

typedef array<ref DMArenaConfig> TDMArenaConfigArray;

class DMArenasConfig
{
    ref TDMArenaConfigArray arenas = new TDMArenaConfigArray();

    static DMArenasConfig Load(string filename)
    {
        DMArenasConfig config = new DMArenasConfig();

        PrintFormat("Reading arena configurations from %1", filename);

        DMJSONLoader<DMArenasConfig>.LoadFile(filename, config);

        return config;
    }
};

// vim:ft=enforce
