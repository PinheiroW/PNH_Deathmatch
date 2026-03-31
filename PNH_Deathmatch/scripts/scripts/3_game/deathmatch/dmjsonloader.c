/* Copyright (c) 2023-2025 The Crimson Zamboni
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

class DMJSONLoader<Class T>
{
    private static ref JsonSerializer m_serializer = new JsonSerializer();

    static bool LoadFile(string filename, out T data)
    {
        auto handle = OpenFile(filename, FileMode.READ);
        if (handle == 0)
        {
            Error(string.Format("Unable to open %1 for reading!", filename));
            return false;
        }

        string content;
        string buffer;

        while (ReadFile(handle, buffer, 1024) != 0)
        {
            content += buffer;
        }

        CloseFile(handle);

        string error;

        if (!m_serializer.ReadFromString(data, content, error))
        {
            Error(error);
            return false;
        }

        return true;
    }
}

// vim:ft=enforce
