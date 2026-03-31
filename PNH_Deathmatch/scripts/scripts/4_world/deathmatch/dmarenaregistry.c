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

class DMArenaRegistry
{
    static private const string CUSTOM_ARENAS_FILENAME = "$mission:custom-arenas.json";

    static private ref DMArenaRegistry s_instance;
    private ref map<string, ref DMArena> m_arenas = new map<string, ref DMArena>();
    private bool m_frozen = false;

    private void DMArenaRegistry()
    {
        this.Load("$mission:arenas.json");

        if (FileExist(DMArenaRegistry.CUSTOM_ARENAS_FILENAME))
        {
            this.Load(DMArenaRegistry.CUSTOM_ARENAS_FILENAME);
        }
        else
        {
            this.Generate(DMArenaRegistry.CUSTOM_ARENAS_FILENAME);
        }
    }

    static DMArenaRegistry Instance()
    {
        if (!DMArenaRegistry.s_instance)
        {
            DMArenaRegistry.s_instance = new DMArenaRegistry();
        }

        return DMArenaRegistry.s_instance;
    }

    static void AddArena(DMArena arena)
    {
        DMArenaRegistry.Instance().InsertArena(arena);
    }

    void InsertArena(DMArena arena)
    {
        if (m_frozen)
        {
            Error("Cannot add arena; DMArenaRegistry is frozen!");
        }
        else
        {
            m_arenas.Set(arena.GetName(), arena);
        }
    }

    void RemoveArena(string name)
    {
        if (m_frozen)
        {
            Error("Cannot remove arena; DMArenaRegistry is frozen!");
        }
        else
        {
            m_arenas.Remove(name);
        }
    }

    void Freeze()
    {
        if (!m_frozen)
        {
            PrintFormat("%1 arenas loaded", m_arenas.Count());
        }

        m_frozen = true;
    }

    int IsEmpty()
    {
        return m_arenas.Count() == 0;
    }

    DMArena GetArena(string name)
    {
        return m_arenas.Get(name);
    }

    TStringArray GetAllArenaNames()
    {
        return m_arenas.GetKeyArray();
    }

    TStringArray GetSupportedArenaNames(int playerCount, TStringArray arenas)
    {
        TStringArray names = new TStringArray();

        foreach (string name : arenas)
        {
            DMArena arena = this.GetArena(name);
            if (arena && arena.SupportsPlayers(playerCount))
            {
                names.Insert(name);
            }
        }

        return names;
    }

    TStringArray GetSupportedArenaNames(int playerCount)
    {
        TStringArray names = new TStringArray();

        array<DMArena> arenas = m_arenas.GetValueArray();
        foreach (DMArena arena : arenas)
        {
            if (arena.SupportsPlayers(playerCount))
            {
                names.Insert(arena.GetName());
            }
        }

        return names;
    }

    private void Load(string filename)
    {
        DMArenasConfig config = DMArenasConfig.Load(filename);

        for (int i = 0; i < config.arenas.Count(); i++)
        {
            if (config.arenas[i].IsValid(filename, i))
            {
                DMArena arena = DMArena.FromConfig(config.arenas[i]);

                this.InsertArena(arena);
            }
        }
    }

    private void Generate(string filename)
    {
        FileHandle fh = OpenFile(filename, FileMode.WRITE);
        FPrintln(fh, "{");
        FPrintln(
            fh, "  \"$schema\": \"https://crimsonzamboni.com/deathmatch/schemas/v1/arenas.json\",");
        FPrintln(fh, "  \"arenas\": [");
        FPrintln(fh, "  ]");
        FPrintln(fh, "}");
        CloseFile(fh);
    }
}

// vim:ft=enforce
