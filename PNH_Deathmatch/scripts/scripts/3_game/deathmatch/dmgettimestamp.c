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

string DMGetTimestamp()
{
    float time = g_Game.GetTickTime();
    int ms = (time - Math.Floor(time)) * 1000;
    int seconds = time;
    int minutes = seconds / 60;
    int hours = minutes / 60;

    seconds = seconds % 60;
    minutes = minutes % 60;

    return string.Format(
            "%1:%2:%3.%4",
            hours, minutes.ToStringLen(2), seconds.ToStringLen(2), ms.ToStringLen(3));
}

// vim:ft=enforce
