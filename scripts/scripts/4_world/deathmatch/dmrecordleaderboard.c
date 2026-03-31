/* Copyright (c) 2022-2025 The Crimson Zamboni
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

class DMLeaderboardScore
{
    string id;
    string name;
    int kills;
    int deaths;
};

class DMLeaderboardFile
{
    string arenaName;
    string eventName;
    string timestamp;

    ref array<ref DMLeaderboardScore> players = new array<ref DMLeaderboardScore>;
};

class DMRecordLeaderboard
{
    static void Save(DMArena arena, DMEvent dmEvent, TDMScoreArray scores)
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);

        auto output = new DMLeaderboardFile();
        output.arenaName = arena.GetName();
        if (dmEvent)
        {
            output.eventName = dmEvent.GetName();
        }
        output.timestamp = string.Format(
            "%1-%2-%3T%4:%5:%6Z",
            year, month.ToStringLen(2), day.ToStringLen(2),
            hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2));
        foreach (auto score : scores)
        {
            auto player = new DMLeaderboardScore();
            player.id = score.m_id;
            player.name = score.m_name;
            player.kills = score.m_kills;
            player.deaths = score.m_deaths;
            output.players.Insert(player);
        }

        MakeDirectory("$profile:deathmatch/scores");

        string filename = string.Format(
            "$profile:deathmatch/scores/%1%2%3%4%5%6_scores.json",
            year, month.ToStringLen(2), day.ToStringLen(2),
            hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2));

        JsonFileLoader<DMLeaderboardFile>.JsonSaveFile(filename, output);
    }
};

// vim:ft=enforce
