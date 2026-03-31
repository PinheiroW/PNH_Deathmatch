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
class DMArenaVote : DMVote
{
    private DMArenaRotation m_rotation;
    private string m_arenaName;

    static DMArenaVote Start(
        Deathmatch deathmatch, int durationMinutes, DMArenaRotation rotation, string arenaName)
    {
        DMArenaVote vote = new DMArenaVote(deathmatch);

        vote.m_rotation = rotation;
        vote.m_arenaName = arenaName;

        vote.Start(durationMinutes);

        return vote;
    }

    override void OnSucceeded()
    {
        DMChat.Announce(string.Format("Vote succeeded! Next arena will be %1", m_arenaName));

        m_rotation.SetNextArena(m_arenaName);
    }
};

// vim:ft=enforce
