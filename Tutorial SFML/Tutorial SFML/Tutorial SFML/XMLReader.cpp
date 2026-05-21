#include "XMLReader.h"

//Codigo generado por IA
//XMLReader::XMLReader(const std::string& filename) {
//    rapidxml::xml_document<> doc;
//
//    std::ifstream file(filename);
//    if (!file.is_open()) {
//        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
//        return;
//    }
//
//    std::stringstream buffer;
//    buffer << file.rdbuf();
//    file.close();
//
//    std::string content(buffer.str());
//
//    try {
//        doc.parse<0>(&content[0]);
//    }
//    catch (const rapidxml::parse_error& e) {
//        std::cerr << "Error de parseo XML: " << e.what() << std::endl;
//        return;
//    }
//
//    rapidxml::xml_node<>* rootNode = doc.first_node(NODE_MAP);
//    if (!rootNode) {
//        std::cerr << "Error: Nodo raíz 'map' no encontrado." << std::endl;
//        return;
//    }
//
//    rapidxml::xml_node<>* bgNode = rootNode->first_node(NODE_BACKGROUND);
//    if (bgNode) {
//        rapidxml::xml_node<>* posNode = bgNode->first_node(NODE_POSITION);
//        if (posNode) {
//            m_background.position.x = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_X)->value());
//            m_background.position.y = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_Y)->value());
//        }
//
//        rapidxml::xml_node<>* spriteNode = bgNode->first_node(NODE_SPRITE);
//        if (spriteNode) {
//            m_background.sprite = spriteNode->value();
//        }
//
//        rapidxml::xml_node<>* sizeNode = bgNode->first_node(NODE_SIZE);
//        if (sizeNode) {
//            m_background.size.width = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_WIDTH)->value());
//            m_background.size.height = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_HEIGHT)->value());
//        }
//    }
//
//    for (rapidxml::xml_node<>* platformNode = rootNode->first_node(NODE_PLATFORM);
//        platformNode;
//        platformNode = platformNode->next_sibling(NODE_PLATFORM)) {
//
//        Platform platform;
//
//        rapidxml::xml_node<>* posNode = platformNode->first_node(NODE_POSITION);
//        if (posNode) {
//            platform.position.x = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_X)->value());
//            platform.position.y = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_Y)->value());
//        }
//
//        rapidxml::xml_node<>* spriteNode = platformNode->first_node(NODE_SPRITE);
//        if (spriteNode) {
//            platform.sprite = spriteNode->value();
//        }
//
//        rapidxml::xml_node<>* sizeNode = platformNode->first_node(NODE_SIZE);
//        if (sizeNode) {
//            platform.size.width = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_WIDTH)->value());
//            platform.size.height = std::stof(sizeNode->first_attribute(ATTRIBUTE_SIZE_HEIGHT)->value());
//        }
//
//        m_platforms.push_back(platform);
//    }
//
//    rapidxml::xml_node<>* spawnsNode = rootNode->first_node(NODE_SPAWNS);
//    if (spawnsNode) {
//        for (rapidxml::xml_node<>* playerNode = spawnsNode->first_node(NODE_PLAYER);
//            playerNode;
//            playerNode = playerNode->next_sibling(NODE_PLAYER)) {
//
//            SpawnPoint spawn;
//            spawn.playerId = std::stoi(playerNode->first_attribute(NODE_PLAYER_ID)->value());
//
//            rapidxml::xml_node<>* posNode = playerNode->first_node(NODE_POSITION);
//            if (posNode) {
//                spawn.position.x = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_X)->value());
//                spawn.position.y = std::stof(posNode->first_attribute(ATTRIBUTE_POSITION_Y)->value());
//            }
//
//            m_spawns.push_back(spawn);
//        }
//    }
//}

bool XMLReader::SaveToFile(const std::string& filename)
{
    rapidxml::xml_document<> doc;

    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);

    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, NODE_MAP);
    doc.append_node(root);

    rapidxml::xml_node<>* bgNode = doc.allocate_node(rapidxml::node_element, NODE_BACKGROUND);
    root->append_node(bgNode);

    rapidxml::xml_node<>* bgPosNode = doc.allocate_node(rapidxml::node_element, NODE_POSITION);
    bgNode->append_node(bgPosNode);
    bgPosNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_POSITION_X, std::to_string(m_background.position.x).c_str()));
    bgPosNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_POSITION_Y, std::to_string(m_background.position.y).c_str()));

    rapidxml::xml_node<>* bgSpriteNode = doc.allocate_node(rapidxml::node_element, NODE_SPRITE);
    bgSpriteNode->value(doc.allocate_string(m_background.sprite.c_str()));
    bgNode->append_node(bgSpriteNode);

    rapidxml::xml_node<>* bgSizeNode = doc.allocate_node(rapidxml::node_element, NODE_SIZE);
    bgNode->append_node(bgSizeNode);
    bgSizeNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_SIZE_WIDTH, std::to_string(m_background.size.width).c_str()));
    bgSizeNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_SIZE_HEIGHT, std::to_string(m_background.size.height).c_str()));

    for (const auto& platform : m_platforms) {
        rapidxml::xml_node<>* platNode = doc.allocate_node(rapidxml::node_element, NODE_PLATFORM);
        root->append_node(platNode);

        rapidxml::xml_node<>* platPosNode = doc.allocate_node(rapidxml::node_element, NODE_POSITION);
        platNode->append_node(platPosNode);
        platPosNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_POSITION_X, std::to_string(platform.position.x).c_str()));
        platPosNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_POSITION_Y, std::to_string(platform.position.y).c_str()));

        rapidxml::xml_node<>* platSpriteNode = doc.allocate_node(rapidxml::node_element, NODE_SPRITE);
        platSpriteNode->value(doc.allocate_string(platform.sprite.c_str()));
        platNode->append_node(platSpriteNode);

        rapidxml::xml_node<>* platSizeNode = doc.allocate_node(rapidxml::node_element, NODE_SIZE);
        platNode->append_node(platSizeNode);
        platSizeNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_SIZE_WIDTH, std::to_string(platform.size.width).c_str()));
        platSizeNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_SIZE_HEIGHT, std::to_string(platform.size.height).c_str()));
    }

    if (!m_spawns.empty()) {
        rapidxml::xml_node<>* spawnsNode = doc.allocate_node(rapidxml::node_element, NODE_SPAWNS);
        root->append_node(spawnsNode);

        for (const auto& spawn : m_spawns) {
            rapidxml::xml_node<>* playerNode = doc.allocate_node(rapidxml::node_element, NODE_PLAYER);
            spawnsNode->append_node(playerNode);

            playerNode->append_attribute(doc.allocate_attribute(NODE_PLAYER_ID, std::to_string(spawn.playerId).c_str()));

            rapidxml::xml_node<>* spawnPosNode = doc.allocate_node(rapidxml::node_element, NODE_POSITION);
            playerNode->append_node(spawnPosNode);
            spawnPosNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_POSITION_X, std::to_string(spawn.position.x).c_str()));
            spawnPosNode->append_attribute(doc.allocate_attribute(ATTRIBUTE_POSITION_Y, std::to_string(spawn.position.y).c_str()));
        }
    }

    std::string xmlString;
    rapidxml::print(std::back_inserter(xmlString), doc, 0);

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << " para escritura" << std::endl;
        return false;
    }

    file << xmlString;
    file.close();

    doc.clear();

    return true;
}