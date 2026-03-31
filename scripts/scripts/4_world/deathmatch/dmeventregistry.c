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

class DMEventRegistry
{
    static private ref DMEventRegistry instance;

    private ref map<string, ref DMEvent> m_events = new map<string, ref DMEvent>();
    private string m_nextEventName;

    static DMEventRegistry Instance()
    {
        if (!instance)
        {
            instance = new DMEventRegistry();
        }

        return instance;
    }

    private void DMEventRegistry()
    {
        m_events.Insert("cowboy", new DMCowboyEvent());
        m_events.Insert("bambi", new DMBambiEvent());
        m_events.Insert("hotshot", new DMHotshotEvent());
        m_events.Insert("halloween", new DMHalloweenEvent());
        m_events.Insert("toxic", new DMToxicEvent());
        m_events.Insert("medieval", new DMMedievalEvent());

        DMEventsConfig config = new DMEventsConfig();

        int count = config.CustomEventCount();
        PrintFormat("Loaded %1 custom event definition%2", count, DMUtils.s(count));

        TStringArray eventNames = config.GetEventNames();
        foreach (string name : eventNames)
        {
            m_events.Insert(name, DMEvent.CreateFromConfig(config.GetEventConfig(name)));
        }
    }

    DMEvent GetEvent(string name)
    {
        name.ToLower();

        return m_events.Get(name);
    }

    void SetNextEventName(string name)
    {
        name.ToLower();

        m_nextEventName = name;
    }

    void ResetNextEvent()
    {
        m_nextEventName = "";
    }

    bool HaveNextEvent()
    {
        return m_nextEventName != "";
    }

    string GetNextEventName()
    {
        return m_nextEventName;
    }
};

// vim:ft=enforce
