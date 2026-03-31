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

modded class MissionGameplay
{
    override void OnMissionStart()
    {
        super.OnMissionStart();

        if (g_Game.IsClient())
        {
            g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.MisconfiguredServer, 30000, true);
        }
    }

    override void OnMissionFinish()
    {
        g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.MisconfiguredServer);

        super.OnMissionFinish();
    }

    void MisconfiguredServer()
    {
        ref ChatMessageEventParams tmp = new ChatMessageEventParams(
                CCBattlEye,
                "Deathmatch",
                "This server is misconfigured! CrimsonZamboniDeathmatch should be loaded as a -servermod only!",
                "");
        m_Chat.Add(tmp);
    }
};

// vim:ft=enforce
