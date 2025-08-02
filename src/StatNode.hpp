#pragma once

/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>

#include "main.hpp"
#include "Stat.cpp"
 /**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;
class StatNode : public CCNodeRGBA {
    Stat m_stat;
    CCLabelBMFont* m_label;
    CCSprite* m_sprite;
    std::string m_setting;
    bool m_usesImage;
    int val = 0;
    int otherStatID;
public:
    static StatNode* create(Stat stat, std::string id, GJGameLevel* level, int value = 0);
    bool init(Stat stat, std::string id, GJGameLevel* level, int val);
    void updateLabel();
};