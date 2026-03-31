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
class DMEventVote : DMVote
{
    private DMEventRegistry m_registry;
    private string m_eventName;

    static DMEventVote Start(
        Deathmatch deathmatch, int durationMinutes, DMEventRegistry registry, string eventName)
    {
        DMEventVote vote = new DMEventVote(deathmatch);

        vote.m_registry = registry;
        vote.m_eventName = eventName;

        vote.Start(durationMinutes);

        return vote;
    }

    override void OnSucceeded()
    {
        DMChat.Announce(string.Format("Vote succeeded! Next event will be %1", m_eventName));

        m_registry.SetNextEventName(m_eventName);
    }
};

// vim:ft=enforce
