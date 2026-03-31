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

enum DMArenaPositionStatus
{
    INSIDE,
    TAKING_DAMAGE,
    INSTANT_KILL
};

class DMProp
{
    string type;
    vector position;
    vector orientation;

    void DMProp(string typeName, vector pos, vector orient)
    {
        type = typeName;
        position = pos;
        orientation = orient;
    }

    Object Spawn()
    {
        return DMUtils.SpawnProp(this.type, this.position, this.orientation);
    }
}

typedef array<ref DMProp> TDMPropArray;

class DMArena
{
    private static ref const TStringArray TREES = {"ChristmasTree", "ChristmasTree_Green"};
    private const float KILL_DELTA = 70;
    private const float CLEANUP_DELTA = 100;

    private string m_name;
    private vector m_center;
    private float m_radius;
    private float m_killRadius;
    private float m_cleanupRadius;
    private int m_minimumPlayers = 0;
    private int m_maximumPlayers = int.MAX;
    private bool m_dark = false;
    private bool m_disableWalls = false;
    private bool m_rectangular = false;
    private float m_width;
    private float m_minX;
    private float m_maxX;
    private float m_breadth;
    private float m_minZ;
    private float m_maxZ;
    private ref TVectorArray m_playerSpawnPositions = new TVectorArray();
    private ref TVectorArray m_cratePositions = new TVectorArray();
    private ref TVectorArray m_infectedSpawnPositions = new TVectorArray();
    private ref TVectorArray m_christmasTreePositions = new TVectorArray();
    private ref TDMPropArray m_props = new TDMPropArray();

    void DMArena(string name, vector center, int radius = 500)
    {
        m_name = name;
        m_center = Vector(center[0], 0, center[2]);
        m_radius = radius;
        m_killRadius = radius + KILL_DELTA;
        m_cleanupRadius = radius + CLEANUP_DELTA;
    }

    string GetName()
    {
        return m_name;
    }

    vector GetCenter()
    {
        return m_center;
    }

    float DistanceFromCenter(vector position)
    {
        return vector.Distance(m_center, Vector(position[0], 0, position[2]));
    }

    DMArenaPositionStatus GetPositionStatus(Man player)
    {
        if (m_rectangular)
        {
            vector position = player.GetPosition();

            if (position[0] < (m_minX - KILL_DELTA)) return DMArenaPositionStatus.INSTANT_KILL;
            if (position[0] > (m_maxX + KILL_DELTA)) return DMArenaPositionStatus.INSTANT_KILL;
            if (position[2] < (m_minZ - KILL_DELTA)) return DMArenaPositionStatus.INSTANT_KILL;
            if (position[2] > (m_maxZ + KILL_DELTA)) return DMArenaPositionStatus.INSTANT_KILL;

            if (position[0] < m_minX) return DMArenaPositionStatus.TAKING_DAMAGE;
            if (position[0] > m_maxX) return DMArenaPositionStatus.TAKING_DAMAGE;
            if (position[2] < m_minZ) return DMArenaPositionStatus.TAKING_DAMAGE;
            if (position[2] > m_maxZ) return DMArenaPositionStatus.TAKING_DAMAGE;
        }
        else
        {
            float distance = this.DistanceFromCenter(player.GetPosition());

            if (distance > this.GetRadius())
            {
                Print("Player " + player.GetIdentity().GetName() + " is too far away (" + distance + ")");
                if (distance > this.GetKillRadius())
                {
                    return DMArenaPositionStatus.INSTANT_KILL;
                }
                else
                {
                    return DMArenaPositionStatus.TAKING_DAMAGE;
                }
            }
        }

        return DMArenaPositionStatus.INSIDE;
    }

    int GetRadius()
    {
        return m_radius;
    }

    int GetKillRadius()
    {
        return m_killRadius;
    }

    void FindObjectsToClean(array<Object> objects)
    {
        array<CargoBase> cargos = new array<CargoBase>();
        g_Game.GetObjectsAtPosition(m_center, m_cleanupRadius, objects, cargos);
    }

    void SetRectangle(int width, int breadth)
    {
        m_width = width;
        m_breadth = breadth;

        m_minX = m_center[0] - (m_width / 2);
        m_maxX = m_center[0] + (m_width / 2);
        m_minZ = m_center[2] - (m_breadth / 2);
        m_maxZ = m_center[2] + (m_breadth / 2);

        m_radius = vector.Distance(m_center, Vector(m_minX, 0, m_minZ));
        m_killRadius = m_radius + KILL_DELTA;
        m_cleanupRadius = m_radius + CLEANUP_DELTA;

        m_rectangular = true;
    }

    void SetMinimumPlayers(int minimum)
    {
        m_minimumPlayers = minimum;
    }

    void SetMaximumPlayers(int maximum)
    {
        m_maximumPlayers = maximum;
    }

    void SetDark()
    {
        m_dark = true;
    }

    bool IsDark()
    {
        return m_dark;
    }

    void DisableWalls()
    {
        m_disableWalls = true;
    }

    void SetPlayerSpawnPositions(TVectorArray positions)
    {
        m_playerSpawnPositions = positions;
    }

    TVectorArray GetPlayerSpawnPositions()
    {
        return m_playerSpawnPositions;
    }

    vector GetRandomPlayerSpawnPosition(bool avoidPlayers = true)
    {
        if (m_playerSpawnPositions.Count() == 0)
        {
            return Vector(m_center[0], g_Game.SurfaceY(m_center[0], m_center[2]), m_center[2]);
        }

        vector position = m_playerSpawnPositions.GetRandomElement();

        if (avoidPlayers)
        {
            CEApi ce = GetCEApi();

            if (!ce.AvoidPlayer(position, 20))
            {
                return this.GetRandomPlayerSpawnPosition(false);
            }
        }

        return position;
    }

    void SetDirectionToCenter(PlayerBase player)
    {
        player.SetDirection(vector.Direction(player.GetPosition(), m_center));

        // Force synchronization, see https://feedback.bistudio.com/T180802
        player.SetPosition(player.GetPosition() + "0 0.1 0");
    }

    void DebugPlayerSpawns()
    {
        Print("Checking player spawn positions");
        foreach (vector pos : m_playerSpawnPositions)
        {
            float distance = this.DistanceFromCenter(pos);

            if (distance >= m_radius)
            {
                PrintFormat(
                        "ERROR :: Player spawn %1 is outside arena (distance %2)", pos, distance);
            }
        }
    }

    void SetCratePositions(TVectorArray positions)
    {
        m_cratePositions = positions;
    }

    TVectorArray GetCratePositions()
    {
        return m_cratePositions;
    }

    void SetInfectedPositions(TVectorArray positions)
    {
        m_infectedSpawnPositions = positions;
    }

    bool HaveInfectedPositions()
    {
        return m_infectedSpawnPositions.Count() > 0;
    }

    int GetInfectedPositionCount()
    {
        return m_infectedSpawnPositions.Count();
    }

    TVectorArray GetInfectedPositions()
    {
        return m_infectedSpawnPositions;
    }

    vector GetRandomInfectedPosition()
    {
        return m_infectedSpawnPositions.GetRandomElement();
    }

    void SetChristmasTreePositions(TVectorArray positions)
    {
        m_christmasTreePositions = positions;
    }

    TVectorArray GetChristmasTreePositions()
    {
        return m_christmasTreePositions;
    }

    array<Object> SpawnChristmasTrees(array<Object> objects)
    {
        foreach (vector pos : m_christmasTreePositions)
        {
            if (Math.RandomFloat01() < 0.5)
            {
                objects.Insert(
                    DMUtils.SpawnProp(
                        TREES.GetRandomElement(), pos + "0 7.5 0", DMUtils.RandomYaw()));
            }
            else
            {
                objects.Insert(
                    DMUtils.SpawnProp(
                        "Wreck_SantasSleigh", pos + "0 1.7 1.5", DMUtils.RandomYaw()));
            }
        }

        return objects;
    }

    void AddProp(string type, vector position, vector orientation)
    {
        m_props.Insert(new DMProp(type, position, orientation));
    }

    bool SupportsPlayers(int playerCount)
    {
        if (playerCount >= m_minimumPlayers && playerCount <= m_maximumPlayers)
        {
            return true;
        }

        return false;
    }

    private Object PlaceWall(vector position, vector orientation)
    {
        position[1] = g_Game.SurfaceY(position[0], position[2]);

        int flags = ECE_PLACE_ON_SURFACE;
        float depth = g_Game.GetWaterDepth(position);
        if (depth > 1)
        {
            position[1] = position[1] + depth - 1;
            flags |= ECE_KEEPHEIGHT;
        }

        Object obj = g_Game.CreateObjectEx("Land_Castle_Wall1_20", position, flags);
        obj.SetOrientation(orientation);

        return obj;
    }

    private void EncloseCircle(array<Object> enclosure)
    {
        float step = 360 / ((2 * Math.PI * m_radius) / 20);
        float angle = 0;
        while (angle < 360)
        {
            vector vec = Vector(angle, 0, 0).AnglesToVector();
            vector position = m_center + (vec * m_radius);
            vector orientation = Vector(angle + 180, 0, 0);

            enclosure.Insert(this.PlaceWall(position, orientation));

            angle += step;
        }
    }

    private void EncloseRectangle(array<Object> enclosure)
    {
        float x = m_center[0] - (m_width / 2);

        while (x <= m_center[0] + (m_width / 2))
        {
            enclosure.Insert(this.PlaceWall(Vector(x, 0, m_center[2] - (m_breadth / 2)), "0 0 0"));
            enclosure.Insert(
                    this.PlaceWall(Vector(x, 0, m_center[2] + (m_breadth / 2)), "180 0 0"));

            x += 20;
        }

        float z = m_center[2] - (m_breadth / 2);
        while (z <= m_center[2] + (m_breadth / 2))
        {
            enclosure.Insert(this.PlaceWall(Vector(m_center[0] - (m_width / 2), 0, z), "90 0 0"));
            enclosure.Insert(
                    this.PlaceWall(Vector(m_center[0] + (m_width / 2), 0, z), "270 0 0"));

            z += 20;
        }
    }

    array<Object> Enclose(array<Object> enclosure)
    {
        if (m_disableWalls) return enclosure;

        if (m_rectangular)
        {
            this.EncloseRectangle(enclosure);
        }
        else
        {
            this.EncloseCircle(enclosure);
        }

        return enclosure;
    }

    array<Object> Prepare(array<Object> objects)
    {
        foreach (DMProp prop : m_props)
        {
            Object obj = prop.Spawn();
            if (obj)
            {
                objects.Insert(obj);
            }
        }

        return objects;
    }

    void WarningSounds()
    {
        float angle = Math.RandomFloatInclusive(0, 360);
        vector vec = Vector(angle, 0, 0).AnglesToVector();
        vector position = m_center + (vec * (m_radius + 500));
        position[1] = g_Game.SurfaceY(position[0], position[2]);

        array<ref Param> params = new array<ref Param>();
        params.Insert(new Param1<vector>(position));

        g_Game.RPC(null, ERPCs.RPC_SOUND_ARTILLERY, params, true);
    }

    static DMArena FromConfig(DMArenaConfig config)
    {
        DMArena arena = new DMArena(
            config.name, DMUtils.VectorFromArray(config.center), config.radius);

        if (config.rectangular)
        {
            arena.SetRectangle(config.xSize, config.zSize);
        }

        arena.SetMinimumPlayers(config.minimumPlayers);
        arena.SetMaximumPlayers(config.maximumPlayers);

        if (config.playerSpawns.Count() > 0)
        {
            arena.SetPlayerSpawnPositions(DMUtils.VectorsFromArray(config.playerSpawns));
        }
        arena.SetCratePositions(DMUtils.VectorsFromArray(config.crateSpawns));
        arena.SetInfectedPositions(DMUtils.VectorsFromArray(config.infectedSpawns));
        arena.SetChristmasTreePositions(DMUtils.VectorsFromArray(config.christmasSpawns));

        foreach (DMPropConfig prop : config.props)
        {
            arena.AddProp(
                prop.type, DMUtils.VectorFromArray(prop.position),
                DMUtils.VectorFromArray(prop.orientation));
        }

        if (config.dark)
        {
            arena.SetDark();
        }

        if (config.disableWalls)
        {
            arena.DisableWalls();
        }

        return arena;
    }
}

// vim:ft=enforce
