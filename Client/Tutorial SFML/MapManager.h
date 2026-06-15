#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include "MapReader.h"

#define MapM MapManager::Instance()

class MapManager
{
private:
    bool isDirty = true;
public:
    static MapManager* Instance() {
        static MapManager mpm;

        return &mpm;
    }

	void SendMap() {}
	void ReceiveMap(sf::Packet data) {
        int width, height;
        data >> width >> height;

        int tileCount = 0;
        data >> tileCount;

        Tileset tempTileVector;

        for (int i = 0; i < tileCount; i++) {
            Tile tile;
            //data >> tile;
            tempTileVector.push_back(new Tile(tile));
        }

        MAP->SetHeight(height);
        MAP->SetWidth(width);
        MAP->SetTiles(tempTileVector);
        MAP->Init();

        isDirty = false;
    }

    void SetDirtyState(bool state) { isDirty = state; }
    const bool GetDirtyState() const { return isDirty; }
};

