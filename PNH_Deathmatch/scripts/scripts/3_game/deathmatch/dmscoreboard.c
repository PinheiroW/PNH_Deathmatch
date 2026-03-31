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

class DMScore
{
    int m_kills;
    int m_deaths;
    string m_name;
    string m_id;

    void DMScore(int kills, int deaths, string name, string id)
    {
        m_kills = kills;
        m_deaths = deaths;
        m_name = name;
        m_id = id;
    }

    int Compare(DMScore other)
    {
        if (this.m_kills > other.m_kills) return -1;
        if (this.m_kills == other.m_kills && this.m_deaths < other.m_deaths) return -1;
        if (this.m_kills == other.m_kills && this.m_deaths == other.m_deaths) return 0;
        return 1;
    }
};

typedef array<ref DMScore> TDMScoreArray;

class DMScoreBoard
{
    private autoptr TStringIntMap m_player_kills = new TStringIntMap();
    private autoptr TStringIntMap m_player_deaths = new TStringIntMap();

    void Reset()
    {
        m_player_kills.Clear();
        m_player_deaths.Clear();
    }

    void ResetPlayer(PlayerIdentity player)
    {
        string id = player.GetId();

        m_player_kills.Set(id, 0);
        m_player_deaths.Set(id, 0);
    }

    void AddKill(PlayerIdentity killer)
    {
        string id = killer.GetId();

        m_player_kills.Set(id, m_player_kills.Get(id) + 1);
    }

    void AddDeath(PlayerIdentity victim)
    {
        string id = victim.GetId();

        m_player_deaths.Set(id, m_player_deaths.Get(id) + 1);
    }

    DMScore GetPlayerScore(PlayerIdentity player)
    {
        string id = player.GetId();

        return new DMScore(m_player_kills.Get(id), m_player_deaths.Get(id), player.GetName(), id);
    }

    static private int SortPartition(TDMScoreArray arr, int low, int high)
    {
        DMScore pivot = arr.Get(high);

        int i = low - 1;

        for (int j = low; j <= high - 1; j++)
        {
            if (arr.Get(j).Compare(pivot) < 0)
            {
                i++;
                arr.SwapItems(i, j);
            }
        }

        arr.SwapItems(i + 1, high);
        return i + 1;
    }

    static private void Sort(TDMScoreArray arr, int low, int high)
    {
        if (low >= high) return;

        int pi = SortPartition(arr, low, high);

        Sort(arr, low, pi - 1);
        Sort(arr, pi + 1, high);
    }

    static private void Sort(TDMScoreArray arr)
    {
        Sort(arr, 0, arr.Count() - 1);
    }

    TDMScoreArray GetLeaderboard(array<PlayerIdentity> players)
    {
        TDMScoreArray leaderboard = new TDMScoreArray();

        foreach (PlayerIdentity player : players)
        {
            leaderboard.Insert(this.GetPlayerScore(player));
        }

        Sort(leaderboard);

        return leaderboard;
    }

    private static void DumpMap(TStringIntMap stats)
    {
        for (int i = 0; i < stats.Count(); i++)
        {
            PrintFormat("DEBUG ::  %1 = %2", stats.GetKey(i), stats.GetElement(i));
        }
    }

    void DumpKills()
    {
        Print("DEBUG :: Kills:");
        DumpMap(m_player_kills);
    }

    void DumpDeaths()
    {
        Print("DEBUG :: Deaths:");
        DumpMap(m_player_deaths);
    }
};

// vim:ft=enforce
