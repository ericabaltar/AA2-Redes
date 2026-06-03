#include "MapReader.h"

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

    if (lines.empty()) {
        std::cerr << "Error: El archivo está vacío" << std::endl;
        return false;
    }

    m_height = lines.size();
    m_width = 0;
    for (const auto& l : lines) {
        m_width = std::max(m_width, (int)l.length());
    }

    m_grid.assign(m_height, std::vector<Tile*>(m_width, nullptr));

    for (int row = 0; row < m_height; ++row) {
        const std::string& currentLine = lines[row];
        for (int col = 0; col < m_width; ++col) {
            char tileChar = (col < currentLine.length()) ? currentLine[col] : ' ';
            ProcessTile(tileChar, col, row);
        }
    }

    std::cout << "Mapa cargado correctamente: " << m_width << "x" << m_height
        << ", " << m_tiles.size() << " tiles activos" << std::endl;

    return true;
}

bool MapReader::CheckIfInformationIsCorrect(int width, int height, std::vector<Tile> tiles)
{
    if(width = m_width || height != m_height) {
        return false;
	}

    for(int i = 0; i < tiles.size(); i++) {
        if (tiles[i].x != m_tiles[i]->x || tiles[i].y != m_tiles[i]->y || tiles[i].type != m_tiles[i]->type || 
            tiles[i].originalChar != m_tiles[i]->originalChar) {
            return false;
        }
    }

    return true;
}

void MapReader::ProcessTile(char tileChar, int col, int row) {
    TileType type;

    switch (tileChar) {
    case '#':
        type = TileType::FLOOR;
        break;
    case 'P':
        type = TileType::PLAYER;
        break;
    case 'E':
        type = TileType::OPONENT;
        break;
    case ' ':
        return;
    default:
        std::cerr << "Advertencia: Carácter desconocido '" << tileChar
            << "' en posición (" << col << "," << row << ")" << std::endl;
        return;
    }

    Tile* tile = new Tile(type, col, row, tileChar);
    m_tiles.push_back(tile);
    m_grid[row][col] = tile;
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

Tile* MapReader::GetPlayerSpawn() const {
    for (Tile* tile : m_tiles) {
        if (tile->type == TileType::PLAYER) {
            return tile;
        }
    }
    return nullptr;
}

Tile* MapReader::GetEnemySpawns() const {
    for (Tile* tile : m_tiles) {
        if (tile->type == TileType::OPONENT) {
            return tile;
        }
    }
    return nullptr;
}

void MapReader::Clear() {
    for (Tile* tile : m_tiles) {
        delete tile;
    }
    m_tiles.clear();
    m_grid.clear();
    m_width = 0;
    m_height = 0;
}