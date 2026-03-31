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
class DMVote
{
    protected Deathmatch m_deathmatch;
    private ref map<string, bool> m_votes = new map<string, bool>();
    private ref Timer m_timer = new Timer();
    private bool m_ended = false;

    void DMVote(Deathmatch deathmatch)
    {
        m_deathmatch = deathmatch;
    }

    void ~DMVote()
    {
        if (m_timer)
        {
            m_timer.Stop();
        }
    }

    protected void Start(int durationMinutes)
    {
        m_timer.Run(durationMinutes * 60, this, "OnEnd");
    }

    bool HasVoted(DMPlayer player)
    {
        return m_votes.Contains(player.GetId());
    }

    void TallyFor(DMPlayer player)
    {
        m_votes.Set(player.GetId(), true);

        if (this.CanEndEarly())
        {
            this.End();
        }
    }

    void TallyAgainst(DMPlayer player)
    {
        m_votes.Set(player.GetId(), false);

        if (this.CanEndEarly())
        {
            this.End();
        }
    }

    bool CanEndEarly()
    {
        array<PlayerIdentity> players = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(players);

        int votesFor = 0;
        int votesAgainst = 0;

        foreach (PlayerIdentity player : players)
        {
            string id = player.GetId();

            if (m_votes.Contains(id))
            {
                if (m_votes.Get(id))
                {
                    votesFor++;
                }
                else
                {
                    votesAgainst++;
                }
            }
        }

        int minimumVotes = players.Count() / 2;

        return (votesFor > minimumVotes) || (votesAgainst > minimumVotes);
    }

    bool Succeeded()
    {
        array<PlayerIdentity> players = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(players);

        int totalVotes = 0;
        int votesFor = 0;

        foreach (PlayerIdentity player : players)
        {
            string id = player.GetId();

            if (m_votes.Contains(id))
            {
                totalVotes++;

                if (m_votes.Get(id))
                {
                    votesFor++;
                }
            }
        }

        int minimumVotes = players.Count() / 2;

        return (totalVotes > minimumVotes) && (votesFor > (totalVotes / 2));
    }

    void End()
    {
        if (m_timer)
        {
            m_timer.Stop();
        }

        this.OnEnd();
    }

    void OnEnd()
    {
        if (m_ended)
        {
            return;
        }

        m_ended = true;

        if (this.Succeeded())
        {
            this.OnSucceeded();
        }
        else
        {
            this.OnFailed();
        }
        m_deathmatch.ResetVote();
    }

    void OnSucceeded()
    {
        DMChat.Announce("Vote succeeded!");
    }

    void OnFailed()
    {
        DMChat.Announce("Vote failed!");
    }

    void OnRoundEnding()
    {
    }
};

// vim:ft=enforce
