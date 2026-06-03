#pragma once
#include "dep/inc/xml/rapidxml.hpp"
#include "dep/inc/xml/rapidxml_iterators.hpp"
#include "dep/inc/xml/rapidxml_print.hpp"
#include "dep/inc/xml/rapidxml_utils.hpp"

#include <sstream>
#include <iostream>

#define XML XMLReader::Instance()

#define NODE_MAP "map"
#define NODE_BACKGROUND "background"
#define NODE_POSITION "position"
#define ATTRIBUTE_POSITION_X "x"
#define ATTRIBUTE_POSITION_Y "y"
#define NODE_SPRITE "sprite"
#define NODE_SIZE "size"
#define ATTRIBUTE_SIZE_WIDTH "width"
#define ATTRIBUTE_SIZE_HEIGHT "height"
#define NODE_SPAWNS "spawns"
#define NODE_PLATFORM "platform"
#define NODE_PLAYER "player"
#define NODE_PLAYER_ID "id"

struct Position {
    float x, y;
};

struct Size {
    float width, height;
};

struct Background {
    Position position;
    std::string sprite;
    Size size;
};

struct Platform {
    Position position;
    std::string sprite;
    Size size;
};

struct SpawnPoint {
    int playerId;
    Position position;
};

class XMLReader
{
public:
    static XMLReader* Instance() {
        static XMLReader xml;

        return &xml;
    }
private:
    Background m_background;
    std::vector<Platform> m_platforms;
    std::vector<SpawnPoint> m_spawns;

private:
    XMLReader() = default;
    XMLReader(const XMLReader& xml) = default;
    XMLReader& operator=(const XMLReader& xml) = delete;
    ~XMLReader() = default;

    //void ReadBackground();
public:
    void ReadFile(const std::string& filename);
    bool SaveToFile(const std::string& filename);

    inline Background GetBackground() { return m_background; }
    inline void SetBackground(Background background) { m_background = background; }

    inline std::vector<Platform> GetPlatforms() { return m_platforms; }
    inline void SetPlatforms(std::vector<Platform> platforms) { m_platforms = platforms; }

    inline std::vector<SpawnPoint> GetSpawnPoints() { return m_spawns; }
    inline void SetSpawnPoints(std::vector<SpawnPoint> spawnPoints) { m_spawns = spawnPoints; }


};

