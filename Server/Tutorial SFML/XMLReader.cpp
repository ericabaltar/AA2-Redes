#include "XMLReader.h"

//Codigo generado por IA
XMLReader::XMLReader(const std::string& filename) {
    rapidxml::xml_document<> doc;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string content(buffer.str());

    try {
        doc.parse<0>(&content[0]);
    }
    catch (const rapidxml::parse_error& e) {
        std::cerr << "Error de parseo XML: " << e.what() << std::endl;
        return;
    }

    rapidxml::xml_node<>* rootNode = doc.first_node(NODE_MAP);
    if (!rootNode) {
        std::cerr << "Error: Nodo raíz 'map' no encontrado." << std::endl;
        return;
    }

    rapidxml::xml_node<>* bgNode = rootNode->first_node(NODE_BACKGROUND);
    if (bgNode) {
        rapidxml::xml_node<>* posNode = bgNode->first_node(NODE_POSITION);
        if (posNode) {
            m_background.position.x = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_X)->value());
            m_background.position.y = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_Y)->value());
        }

        rapidxml::xml_node<>* spriteNode = bgNode->first_node(NODE_SPRITE);
        if (spriteNode) {
            m_background.sprite = spriteNode->value();
        }

        rapidxml::xml_node<>* sizeNode = bgNode->first_node(NODE_SIZE);
        if (sizeNode) {
            m_background.size.width = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_WIDTH)->value());
            m_background.size.height = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_HEIGHT)->value());
        }
    }

    for (rapidxml::xml_node<>* platformNode = rootNode->first_node(NODE_PLATFORM);
        platformNode;
        platformNode = platformNode->next_sibling(NODE_PLATFORM)) {

        Platform platform;

        rapidxml::xml_node<>* posNode = platformNode->first_node(NODE_POSITION);
        if (posNode) {
            platform.position.x = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_X)->value());
            platform.position.y = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_Y)->value());
        }

        rapidxml::xml_node<>* spriteNode = platformNode->first_node(NODE_SPRITE);
        if (spriteNode) {
            platform.sprite = spriteNode->value();
        }

        rapidxml::xml_node<>* sizeNode = platformNode->first_node(NODE_SIZE);
        if (sizeNode) {
            platform.size.width = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_WIDTH)->value());
            platform.size.height = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_HEIGHT)->value());
        }

        m_platforms.push_back(platform);
    }

    rapidxml::xml_node<>* spawnsNode = rootNode->first_node(NODE_SPAWNS);
    if (spawnsNode) {
        for (rapidxml::xml_node<>* playerNode = spawnsNode->first_node(NODE_PLATFORM);
            playerNode;
            playerNode = playerNode->next_sibling(NODE_PLATFORM)) {

            SpawnPoint spawn;
            spawn.playerId = std::stoi(playerNode->first_attribute(NODE_PLAYER_ID)->value());

            rapidxml::xml_node<>* posNode = playerNode->first_node(NODE_POSITION);
            if (posNode) {
                spawn.position.x = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_X)->value());
                spawn.position.y = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_Y)->value());
            }

            m_spawns.push_back(spawn);
        }
    }
}