/* Copyright (c) 2024-2025 The Crimson Zamboni
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

class DMAutoRespawn
{
    private static ref DMAutoRespawn s_instance;

    private ref map<string, bool> m_enabled = new map<string, bool>();
    private bool m_enabledByDefault;

    void DMAutoRespawn(DeathmatchSettings settings)
    {
        s_instance = this;
        m_enabledByDefault = settings.autoRespawn;
    }

    static DMAutoRespawn Instance()
    {
        return s_instance;
    }

    void InitializeID(string id)
    {
        if (!m_enabled.Contains(id))
        {
            m_enabled[id] = m_enabledByDefault;
        }
    }

    bool IsEnabledForID(string id)
    {
        return m_enabled[id];
    }

    void ToggleForID(string id)
    {
        m_enabled[id] = !m_enabled[id];
    }
};

// vim:ft=enforce
