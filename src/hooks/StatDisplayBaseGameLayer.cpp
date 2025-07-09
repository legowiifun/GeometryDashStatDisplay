#pragma once
/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

#include "../main.cpp"
#include "../Stat.cpp"
#include "../StatNode.cpp"

 /**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;

class $modify(StatDisplayBaseGameLayer, GJBaseGameLayer) {

    struct Fields {
        CCNode* m_statsContainer;
        bool hasFinishedCreateTextLayers = false;
    };

    // to determine whether to display the stats: use GJGameLevel: m_level
    // m_level->m_stars.value() for stars/moons
    // m_level->isPlatformer() for if level is platformer
    // m_level->m_coinsVerified.value() for if coins are verified

    // update the text
    void updateText() {
        auto fields = m_fields.self();
        for (StatNode* node : CCArrayExt<StatNode*>(fields->m_statsContainer->getChildren())) {
            node->updateLabel();
        }
    }

    // replacement for init
    // so we can access m_level
    void createTextLayers() {
        GJBaseGameLayer::createTextLayers();

        std::string position = Mod::get()->getSettingValue<std::string>("position");

        auto fields = m_fields.self();
        fields->m_statsContainer = CCNode::create();
        fields->m_statsContainer->setZOrder(999);
        fields->m_statsContainer->setID("stats"_spr);

        m_uiLayer->addChild(fields->m_statsContainer);

        CCSize winSize = CCDirector::get()->getWinSize();

        AxisLayout* layout = AxisLayout::create(Axis::Column)
            ->setAutoScale(false)
            ->setGap(2.f)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAxisReverse(true);

        layout->ignoreInvisibleChildren(true);

        if (position == "top-left") {
            fields->m_statsContainer->setPosition(5, winSize.height - 5);
            fields->m_statsContainer->setAnchorPoint({ 0, 1 });
            layout->setCrossAxisLineAlignment(AxisAlignment::Start);
        }
        else if (position == "bottom-left") {
            fields->m_statsContainer->setPosition(5, 5);
            fields->m_statsContainer->setAnchorPoint({ 0, 0 });
            layout->setCrossAxisLineAlignment(AxisAlignment::Start);
        }
        else if (position == "top-right") {
            fields->m_statsContainer->setPosition(winSize.width - 5, winSize.height - 5);
            fields->m_statsContainer->setAnchorPoint({ 1, 1 });
            layout->setCrossAxisLineAlignment(AxisAlignment::End);
        }
        else {
            fields->m_statsContainer->setPosition(winSize.width - 5, 5);
            fields->m_statsContainer->setAnchorPoint({ 1, 0 });
            layout->setCrossAxisLineAlignment(AxisAlignment::End);
        }

        createStatsNode(fields->m_statsContainer, Stat::JUMPS, "jumps");
        createStatsNode(fields->m_statsContainer, Stat::ATTEMPTS, "attempts");
        createStatsNode(fields->m_statsContainer, Stat::ONLINE_LEVELS, "onlineLevels");
        createStatsNode(fields->m_statsContainer, Stat::DEMONS, "demons");
        createStatsNode(fields->m_statsContainer, Stat::STARS, "stars");
        createStatsNode(fields->m_statsContainer, Stat::LIKED_LEVELS, "liked");
        createStatsNode(fields->m_statsContainer, Stat::RATED_LEVELS, "rated");
        createStatsNode(fields->m_statsContainer, Stat::USER_COINS, "coins");
        createStatsNode(fields->m_statsContainer, Stat::MANA_ORBS, "manaOrbs");
        createStatsNode(fields->m_statsContainer, Stat::MOONS, "moons");
        createStatsNode(fields->m_statsContainer, Stat::LIST_REWARDS, "lists");
        createStatsNode(fields->m_statsContainer, Stat::INSANES, "insanes");

        fields->m_statsContainer->setLayout(layout);

        fields->m_statsContainer->updateLayout();
        fields->hasFinishedCreateTextLayers = true;
    }

    // create a stats node
    void createStatsNode(CCNode * parent, Stat stat, std::string id, int* value = nullptr, std::string start = "", std::string end = "") {
        StatNode* node = StatNode::create(stat, id, m_level, value, start, end);
        parent->addChild(node);
    }

    // modify the update method
    void update(float dt) {
        GJBaseGameLayer::update(dt);
        updateText();
    }
};