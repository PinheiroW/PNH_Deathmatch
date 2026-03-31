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

class DMWebhookSettings
{
    string type;
    string url;
    bool playerTracking;
};

class DMAdminSettings
{
    string id;
};

class DMEventSettings
{
    string name;
    float chance;
    int roundMinutes;
    ref TStringArray arenas = new TStringArray();
};

class DMEventsSettings
{
    float chance;
    ref array<ref DMEventSettings> types = new array<ref DMEventSettings>();
};

class DeathmatchSettings
{
    private static ref DeathmatchSettings s_settings;

    int maxRounds = 0;
    int maxUptimeHours = 0;
    int roundMinutes = 0;

    ref TStringArray arenaRotation = new TStringArray();
    ref TStringArray excludeArenas = new TStringArray();

    int infectedChance = 0;
    int infectedPlayerFactor = 0;
    int forceInfectedPlayerLimit = 0;
    int minimumInfected = 0;
    int maximumInfected = 0;

    bool autoRespawn = false;

    bool saveLeaderboards = false;

    bool disableWalls = 0;

    ref DMEventsSettings events = new DMEventsSettings();

    int christmas = 0;

    int voteMinimumPlayers = 0;
    int voteMinutes = 0;

    bool ignoreNight = false;

    bool disableSprintLeaning = false;

    int maxIdleSeconds = 0;

    int killFeed = 1;

    int _debugArena = 0;
    int _debugSettings = 0;

    string commandCharacter;

    ref array<ref DMAdminSettings> admins = new array<ref DMAdminSettings>();

    ref DMWebhookSettings killFeedWebhook = new DMWebhookSettings();

    ref array<ref DMWeaponConfig> primaryWeapons = new array<ref DMWeaponConfig>();
    ref array<ref DMWeaponConfig> secondaryWeapons = new array<ref DMWeaponConfig>();
    ref array<ref DMWeaponConfig> crateWeapons = new array<ref DMWeaponConfig>();
    ref array<ref TStringArray> crateArmor = new array<ref TStringArray>();
    ref array<ref TStringArray> crateOther = new array<ref TStringArray>();
    ref TStringArray knives = new TStringArray();

    ref TDMOutfitArray outfits = new TDMOutfitArray();

    ref TStringArray infectedTypes = new TStringArray();

    static DeathmatchSettings Instance()
    {
        return s_settings;
    }

    void DeathmatchSettings()
    {
        s_settings = this;
    }

    void Load()
    {
        if (FileExist(DMFilenames.SETTINGS_JSON))
        {
            DMJSONLoader<ref DeathmatchSettings>.LoadFile(DMFilenames.SETTINGS_JSON, this);
        }
        else
        {
            PrintFormat(
                "Deathmatch settings file (%1) does not exist; creating...",
                DMFilenames.SETTINGS_JSON);
            this.Generate();
        }

        PrintFormat("Max rounds: %1", this.maxRounds);
        PrintFormat("Max uptime hours: %1", this.maxUptimeHours);

        if (this.roundMinutes < 1) this.roundMinutes = 20;
        PrintFormat("Round duration: %1", this.roundMinutes);

        if (this.arenaRotation.Count() > 0)
        {
            PrintFormat("Arena Rotation: %1", DMUtils.Join(this.arenaRotation, ", "));
        }
        else
        {
            Print("Arena Rotation: (random)");
        }
        PrintFormat("Exclude Arenas: %1", DMUtils.Join(this.excludeArenas, ", "));

        if (this.infectedChance < 0) this.infectedChance = 0;
        if (this.infectedChance > 100) this.infectedChance = 100;
        PrintFormat("Infected chance: %1", this.infectedChance);

        if (this.infectedPlayerFactor < 1) this.infectedPlayerFactor = 5;
        PrintFormat("Infected player factor: %1", this.infectedPlayerFactor);

        PrintFormat("Force infected player limit: %1", this.forceInfectedPlayerLimit);

        if (this.minimumInfected < 1) this.minimumInfected = 25;
        PrintFormat("Minimum infected: %1", this.minimumInfected);

        if (this.maximumInfected < 1) this.maximumInfected = 50;
        PrintFormat("Maximum infected: %1", this.maximumInfected);

        PrintFormat("Save leaderboards: %1", this.saveLeaderboards.ToString());

        PrintFormat("Disable walls: %1", this.disableWalls.ToString());

        this.events.chance = Math.Clamp(this.events.chance, 0, 1);
        PrintFormat("Events (chance: %1):", this.events.chance);
        foreach (DMEventSettings eventSettings : this.events.types)
        {
            eventSettings.name.ToLower();
            eventSettings.chance = Math.Clamp(eventSettings.chance, 0, 1);
            if (eventSettings.roundMinutes < 1) eventSettings.roundMinutes = this.roundMinutes;
            PrintFormat(
                    "  %1 :: chance=%2 :: roundMinutes=%3",
                    eventSettings.name, eventSettings.chance, eventSettings.roundMinutes);
        }

        this.christmas = ValidateChristmas(this.christmas);
        PrintFormat("Christmas: %1", this.christmas.ToString());

        if (this.voteMinimumPlayers < 1) this.voteMinimumPlayers = 2;
        PrintFormat("Vote Minimum Players: %1", this.voteMinimumPlayers);

        if (this.voteMinutes < 1) this.voteMinutes = 1;
        PrintFormat("Vote Minutes: %1", this.voteMinutes);

        PrintFormat("Ignore night: %1", this.ignoreNight.ToString());

        PrintFormat("Disable sprint leaning: %1", this.disableSprintLeaning.ToString());

        if (this.maxIdleSeconds < 0) this.maxIdleSeconds = 0;
        PrintFormat("Max idle seconds: %1", this.maxIdleSeconds);

        PrintFormat("Kill feed: %1", this.killFeed);

        if (this.commandCharacter.Length() == 0) this.commandCharacter = "/";
        this.commandCharacter = this.commandCharacter.Substring(0, 1);
        PrintFormat("Command character: %1", this.commandCharacter);

        PrintFormat("Admins: %1", this.admins.Count());

        if (this.killFeedWebhook.type == "")
        {
            Print("Kill feed webhook: disabled");
        }
        else
        {
            PrintFormat(
                "Kill feed webhook: %1 (%2...)",
                this.killFeedWebhook.type,
                this.killFeedWebhook.url.Substring(0, this.killFeedWebhook.url.Length() / 4));
            PrintFormat(
                "Send player join/leave messages to webhook: %1",
                this.killFeedWebhook.playerTracking.ToString());
        }

        if (this.primaryWeapons.Count() == 0)
        {
            DMDefaultWeaponConfigs.SetPrimaryWeapons(this.primaryWeapons);
        }
        PrintFormat("Configured %1 primary weapons", this.primaryWeapons.Count());

        if (this.secondaryWeapons.Count() == 0)
        {
            DMDefaultWeaponConfigs.SetSecondaryWeapons(this.secondaryWeapons);
        }
        PrintFormat("Configured %1 secondary weapons", this.secondaryWeapons.Count());

        if (this.crateWeapons.Count() == 0)
        {
            DMDefaultWeaponConfigs.SetCrateWeapons(this.crateWeapons);
        }
        PrintFormat("Configured %1 crate weapons", this.crateWeapons.Count());

        if (this.crateArmor.Count() == 0)
        {
            DMDefaultWeaponConfigs.SetCrateArmor(this.crateArmor);
        }
        PrintFormat("Configured %1 armor crate items", this.crateArmor.Count());

        if (this.crateOther.Count() == 0)
        {
            DMDefaultWeaponConfigs.SetCrateOther(this.crateOther);
        }
        PrintFormat("Configured %1 other crate items", this.crateOther.Count());

        if (this.knives.Count() == 0)
        {
            DMDefaultWeaponConfigs.SetKnives(this.knives);
        }
        PrintFormat("Configured %1 knives", this.knives.Count());

        if (this.outfits.Count() == 0)
        {
            DMDefaultClothesConfigs.SetOutfits(this.outfits);
        }
        PrintFormat("Configured %1 outfits", this.outfits.Count());

        if (this.infectedTypes.Count() == 0)
        {
            DMDefaultInfectedConfig.SetInfectedTypes(this.infectedTypes);
        }
        PrintFormat("Configured %1 infected types", this.infectedTypes.Count());

        if (this._debugSettings)
        {
            const string filename = "$saves:settings-debug.json";
            PrintFormat("Writing effective settings to %1", filename);
            JsonFileLoader<ref DeathmatchSettings>.JsonSaveFile(filename, this);
        }
    }

    void Generate()
    {
        MakeDirectory(DMFilenames.DIRECTORY);

        FileHandle fh = OpenFile(DMFilenames.SETTINGS_JSON, FileMode.WRITE);
        FPrintln(fh, "{");
        FPrintln(
            fh,
            "  \"$schema\": \"https://crimsonzamboni.com/deathmatch/schemas/v1/settings.json\",");
        FPrintln(fh, "");
        FPrintln(
            fh,
            "  \"$comment\": \"Please refer to https://crimsonzamboni.com/deathmatch/setup.html#optional-server-settings\"");
        FPrintln(fh, "}");
        CloseFile(fh);
    }

    bool IsAdmin(PlayerIdentity identity)
    {
        foreach (DMAdminSettings admin : admins)
        {
            if (identity && identity.GetId() == admin.id)
            {
                return true;
            }
        }

        return false;
    }

    TStringArray GetEventNames()
    {
        TStringArray names = new TStringArray();
        foreach (DMEventSettings eventSettings : this.events.types)
        {
            names.Insert(eventSettings.name);
        }
        return names;
    }

    DMEventSettings GetEventSettings(string name)
    {
        name.ToLower();

        foreach (DMEventSettings eventSettings : this.events.types)
        {
            if (name == eventSettings.name)
            {
                return eventSettings;
            }
        }

        return null;
    }

    DMEventSettings RollForRandomEvent()
    {
        if (this.events.chance <= 0) return null;

        float roll = Math.RandomFloatInclusive(0, 1);
        PrintFormat("Event roll: %1", roll);
        if (roll > this.events.chance) return null;

        float pick = Math.RandomFloatInclusive(0, 1);
        foreach (DMEventSettings eventSettings : this.events.types)
        {
            if (pick < eventSettings.chance) return eventSettings;
            pick = pick - eventSettings.chance;
        }

        return null;
    }

    private static int ValidateChristmas(int input)
    {
        if (input == 2)
        {
            int year, month, day;
            GetYearMonthDayUTC(year, month, day);

            input = month == 12;
        }

        return input;
    }
};

// vim:ft=enforce
