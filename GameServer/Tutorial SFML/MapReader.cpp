#include "MapReader.h"
#include <iostream>
#include <fstream>
#include <algorithm>

bool MapReader::Init() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return false;
    }

    Clear();

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    if (lines.empty()) return false;

    m_height = lines.size();
    m_width = 0;
    for (const auto& l : lines) {
        m_width = std::max(m_width, (int)l.length());
    }

    for (int row = 0; row < m_height; ++row) {
        const std::string& currentLine = lines[row];
        for (int col = 0; col < m_width; ++col) {
            char tileChar = (col < currentLine.length()) ? currentLine[col] : ' ';
            if (tileChar == '#') m_tiles.push_back(new Tile(TileType::FLOOR, col, row, '#'));
            else if (tileChar == 'P') m_tiles.push_back(new Tile(TileType::PLAYER, col, row, 'P'));
            else if (tileChar == 'O') m_tiles.push_back(new Tile(TileType::OPONENT, col, row, 'O'));
        }
    }

    std::cout << "Mapa cargado en el servidor: " << m_width << "x" << m_height << std::endl;
    return true;
}

std::vector<Tile*> MapReader::GetTilesByType(TileType type) const {
    std::vector<Tile*> result;
    for (Tile* tile : m_tiles) {
        if (tile->type == type) {
            result.push_back(tile);
        }
    }
    return result;
}

void MapReader::Clear() {
    for (Tile* tile : m_tiles) {
        delete tile;
    }
    m_tiles.clear();
    m_width = 0;
    m_height = 0;
}