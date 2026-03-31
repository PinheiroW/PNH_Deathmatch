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

class DMEventMessage
{
    string title;
    string details;
};

class DMEventConfig
{
    string name;
    string displayName = "Round";
    ref DMEventMessage greeting = new DMEventMessage();

    ref TDMOutfitArray outfits = new TDMOutfitArray();

    ref array<ref DMWeaponConfig> primaryWeapons = new array<ref DMWeaponConfig>();
    ref array<ref DMWeaponConfig> secondaryWeapons = new array<ref DMWeaponConfig>();
    ref TStringArray knives = new TStringArray();

    ref array<ref DMWeaponConfig> crateWeapons = new array<ref DMWeaponConfig>();
    ref array<ref TStringArray> crateArmor = new array<ref TStringArray>();
    ref array<ref TStringArray> crateOther = new array<ref TStringArray>();

    ref TStringArray infectedTypes = new TStringArray();
};

typedef map<string, ref DMEventConfig> TDMEventConfigMap;
typedef array<ref DMEventConfig> TDMEventConfigArray;

class DMEventsConfigFile
{
    ref TDMEventConfigArray custom = new TDMEventConfigArray();
}

class DMEventsConfig
{
    private static const string VALID_NAME_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

    private ref TDMEventConfigMap m_events = new TDMEventConfigMap();

    void DMEventsConfig()
    {
        this.Load();
    }

    void Load()
    {
        if (FileExist(DMFilenames.EVENTS_JSON))
        {
            DMEventsConfigFile file = new DMEventsConfigFile();

            DMJSONLoader<DMEventsConfigFile>.LoadFile(DMFilenames.EVENTS_JSON, file);
            foreach (DMEventConfig cfg : file.custom)
            {
                cfg.name.ToLower();

                if (this.ValidName(cfg.name))
                {
                    if (cfg.displayName == "") cfg.displayName = "Round";

                    m_events.Insert(cfg.name, cfg);
                }
            }
        }
        else
        {
            PrintFormat(
                "Deathmatch events file (%1) does not exist; creating...", DMFilenames.EVENTS_JSON);
            this.Generate();
        }
    }

    void Generate()
    {
        MakeDirectory(DMFilenames.DIRECTORY);

        FileHandle fh = OpenFile(DMFilenames.EVENTS_JSON, FileMode.WRITE);
        FPrintln(fh, "{");
        FPrintln(
            fh, "  \"$schema\": \"https://crimsonzamboni.com/deathmatch/schemas/v1/events.json\",");
        FPrintln(fh, "  \"custom\": [");
        FPrintln(fh, "  ]");
        FPrintln(fh, "}");
        CloseFile(fh);
    }

    int CustomEventCount()
    {
        return m_events.Count();
    }

    TStringArray GetEventNames()
    {
        return m_events.GetKeyArray();
    }

    DMEventConfig GetEventConfig(string name)
    {
        name.ToLower();

        return m_events.Get(name);
    }

    private bool ValidName(string name)
    {
        if (name.Length() == 0)
        {
            Error("Invalid custom event name; must be at least one character!");
            return false;
        }

        for (int i = 0; i < name.Length(); i++)
        {
            string chr = name.Get(i);

            if (!VALID_NAME_CHARS.Contains(chr))
            {
                Error(
                    string.Format(
                        "Invalid custom event name \"%1\"; must only contain letters, numbers, and underscores",
                        name));
                return false;
            }
        }

        return true;
    }
};

// vim:ft=enforce
