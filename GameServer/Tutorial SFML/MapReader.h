#pragma once
#include <vector>
#include <string>

#define MAP MapReader::Instance()

enum class TileType { FLOOR = '#', PLAYER = 'P', OPONENT = 'O' };

struct Tile {
    TileType type;
    int x, y;
    char originalChar;

    Tile(TileType t, int col, int row, char c) : type(t), x(col), y(row), originalChar(c) {}
    Tile() { type = TileType::FLOOR; x = 0; y = 0; originalChar = ' '; }
};

class MapReader {
public:
    static MapReader* Instance() { static MapReader map; return &map; }

    bool Init();
    std::vector<Tile*> GetTilesByType(TileType type) const;
    float GetDefaultTileSize() const { return 80.f; }
    void Clear();

private:
    MapReader() = default;
    ~MapReader() { Clear(); }

    std::vector<Tile*> m_tiles;
    int m_width = 0;
    int m_height = 0;
    std::string filename = "gameConfig.txt";
};

