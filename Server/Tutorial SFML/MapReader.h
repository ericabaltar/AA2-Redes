#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

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

class MapReader {
public:
    MapReader() = default;

    bool Init();
    bool CheckIfInformationIsCorrect(int width, int height, std::vector<Tile> tiles);

    const std::vector<std::vector<Tile*>>& GetGrid() const { return m_grid; }
    const std::vector<Tile*>& GetTiles() const { return m_tiles; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    std::vector<Tile*> GetTilesByType(TileType type) const;

    Tile* GetPlayerSpawn() const;
    Tile* GetEnemySpawns() const;

    void Clear();

    ~MapReader() { Clear(); }

private:
    std::vector<std::vector<Tile*>> m_grid;
    std::vector<Tile*> m_tiles;
    int m_width = 0;
    int m_height = 0;
    bool informationIsCorrect;
    const std::string& filename = "gameConfig.txt";

    void ProcessTile(char tileChar, int col, int row);
};