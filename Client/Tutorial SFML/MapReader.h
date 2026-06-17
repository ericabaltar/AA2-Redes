#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#define MAP MapReader::Instance()

enum class TileType {
    FLOOR = '#',
    PLAYER = 'P',
    OPONENT = 'O',
};

struct Tile {
    TileType type;
    int x, y;
    char originalChar;

    Tile(TileType t, int col, int row, char c)
        : type(t), x(col), y(row), originalChar(c) {
    }

    Tile() {
        type = TileType::FLOOR;
        x = 0;
        y = 0;
        originalChar = ' ';
    }
};

typedef std::vector<Tile*> Tileset;
typedef std::vector<std::vector<Tile*>> Grid;

class MapReader {
public:
    static MapReader* Instance() {
        static MapReader map;

        return &map;
    }

    bool Init();

    const Grid& GetGrid() const { return m_grid; }
    const Tileset& GetTiles() const { return m_tiles; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    bool GetInformationHasBeenChecked() const { return informationHasBeenChecked; }
    float GetDefaultTileSize() const { return defaultTileSize; }

    void SetInformationHasBeenChecked(bool checked) { informationHasBeenChecked = checked; }
    void SetWidth(int correctWidth) { m_width = correctWidth;}
    void SetHeight(int correctHeight) { m_height = correctHeight;}
	void SetTiles(const Tileset& correctTiles) { m_tiles = correctTiles; }

    Tileset GetTilesByType(TileType type) const;

    Tile* GetPlayerSpawn() const;
    Tile* GetEnemySpawns() const;

    void Clear();

private:
    MapReader() = default;
    MapReader(const MapReader& map) = delete;
    MapReader& operator=(const MapReader& map) = delete;
    ~MapReader() = default;

    Grid m_grid;
    Tileset m_tiles;
    int m_width = 0;
    int m_height = 0;
    float defaultTileSize = 80.f;
	bool informationHasBeenChecked = false;

    const std::string& filename = "gameConfig.txt";

    void ProcessTile(char tileChar, int col, int row);
};