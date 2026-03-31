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

class DMPlayer
{
    private string m_name;
    private Man m_player;
    private bool m_isAdmin;
    private string m_id;

    static DMPlayer GetByPlayerName(DeathmatchSettings settings, string name)
    {
        array<Man> players = new array<Man>();
        g_Game.GetPlayers(players);

        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();

            if (identity && identity.GetName() == name)
            {
                return new DMPlayer(settings, name, player, identity.GetId());
            }
        }

        return null;
    }

    static DMPlayer GetByPlayerId(DeathmatchSettings settings, string id)
    {
        array<Man> players = new array<Man>();
        g_Game.GetPlayers(players);

        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();

            if (identity && identity.GetId() == id)
            {
                return new DMPlayer(settings, identity.GetName(), player, identity.GetId());
            }
        }

        return null;
    }

    string GetName()
    {
        return m_name;
    }

    Man GetPlayer()
    {
        return m_player;
    }

    bool IsAdmin()
    {
        return m_isAdmin;
    }

    PlayerIdentity GetIdentity()
    {
        return m_player.GetIdentity();
    }

    string GetId()
    {
        return m_id;
    }

    private void DMPlayer(DeathmatchSettings settings, string name, Man player, string id)
    {
        m_name = name;
        m_player = player;
        m_isAdmin = settings.IsAdmin(this.GetIdentity());
        m_id = id;
    }
};

// vim:ft=enforce
