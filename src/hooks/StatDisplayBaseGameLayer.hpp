#pragma once
/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

#include "../main.hpp"
#include "../Stat.cpp"
#include "../StatNode.hpp"

 /**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;
class $modify(StatDisplayBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        CCNode* m_statsContainer = CCNode::create();
        bool hasFinishedCreateTextLayers = false;
    };

    // update the text
    void updateText();
    void createTextLayers();
    void createStatsNode(CCNode* parent, Stat stat, std::string id, int value = 0);
    void update(float dt);
};