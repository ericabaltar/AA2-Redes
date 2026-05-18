#pragma once
#include "dep/inc/xml/rapidxml.hpp"
#include "dep/inc/xml/rapidxml_print.hpp"
#include "dep/inc/xml/rapidxml_utils.hpp"
#include "dep/inc/xml/rapidxml_iterators.hpp"

#include <sstream>
#include <iostream>

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
	XMLReader(const std::string& filename);
	~XMLReader();

    const Background& getBackground() const { return m_background; }
    const std::vector<Platform>& getPlatforms() const { return m_platforms; }
    const std::vector<SpawnPoint>& getSpawns() const { return m_spawns; }

private:
    Background m_background;
    std::vector<Platform> m_platforms;
    std::vector<SpawnPoint> m_spawns;

    //void ReadBackground();
};

