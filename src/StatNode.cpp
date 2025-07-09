#pragma once
/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>


#include "main.cpp"
#include "Stat.cpp"

 /**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;

// Contains the text and image for one stat
class StatNode : public CCNodeRGBA {

    Stat m_stat;
    CCLabelBMFont* m_label;
    CCSprite* m_sprite;
    std::string m_setting;
    bool m_usesImage;
    int* val = nullptr;
    std::string startStr;
    std::string endStr;

public:
    static StatNode* create(Stat stat, std::string id, GJGameLevel* level, int* value = nullptr, std::string start = "", std::string end = "") {
        auto* ret = new (std::nothrow) StatNode;
        if (ret && ret->init(stat, id, level, value)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    bool init(Stat stat, std::string id, GJGameLevel* level, int* value = nullptr, std::string start = "", std::string end = "") {
        if (!CCNodeRGBA::init()) return false;
        m_stat = stat;
        setContentHeight(40);

        //create the sprite and label
        if (stat != Stat::OTHER) {
            m_sprite = CCSprite::create(fmt::format("{}.png"_spr, id).c_str());
        }
        else {
            m_sprite = CCSprite::create(id.c_str());
        }
        m_label = CCLabelBMFont::create("", "bigFont.fnt");
        m_label->setOpacity(64);
        setScale(0.3f);


        m_usesImage = Mod::get()->getSettingValue<bool>("useImg");

        // determine visibility
        if (stat != Stat::OTHER) {
            m_setting = id;
            m_setting[0] = std::toupper(m_setting[0]);
            std::string display = Mod::get()->getSettingValue<std::string>(fmt::format("display{}", m_setting));

            if (display != "always") {
                setVisible(false);
            }


            // go through each stat
            switch (stat) {
            case Stat::STARS: {
                if (level->m_stars != 0 && !level->isPlatformer() && display == "when playing rated classic levels") {
                    setVisible(true);
                }
                break;
            }
            case Stat::DEMONS: {
                if (level->m_demon == 1 && display == "when playing demon levels") {
                    setVisible(true);
                }
                break;
            }
            case Stat::USER_COINS: {
                if (level->m_coinsVerified >= 1 && display == "when playing levels with rated coins") {
                    setVisible(true);
                }
                break;
            }
            case Stat::MANA_ORBS: {
                if (level->m_stars != 0 && display == "when playing rated levels") {
                    setVisible(true);
                }
                break;
            }
            case Stat::MOONS: {
                if (level->m_stars != 0 && level->isPlatformer() && display == "when playing rated platformer levels") {
                    setVisible(true);
                }
                break;
            }
            case Stat::INSANES: {
                bool isStar = level->m_stars == 8 || level->m_stars == 9;
                // account for robtop insanes
                bool hasCreator = level->m_creatorName != "";
                bool isRob = !hasCreator && level->m_stars >= 10 && level->m_stars < 14;
                bool isntExcluded = level->m_levelName != "Geometrical Dominator" && level->m_levelName != "Blast Processing";

                if ((isStar && hasCreator) || (isRob && isntExcluded) && display == "when playing insane difficulty levels") {
                    setVisible(true);
                }
                break;
            }
            default: {
                break;
            }
            }
            std::pair<std::string, std::string> parts = g_strings[m_stat];
            startStr = parts.first;
            endStr = parts.second;
        }
        else {
            startStr = start;
            endStr = end;
        }


        if (m_usesImage) {
            addChild(m_sprite);
        }
        addChild(m_label);
        val = value;
        // set the layout
        AxisLayout* layout = AxisLayout::create(Axis::Row)
            ->setAutoScale(true)
            ->setGap(5.f)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setAxisAlignment(AxisAlignment::Start);

        setLayout(layout);
        if (stat != Stat::OTHER) {
            setID(fmt::format("{}"_spr, id).c_str());
        }
        else {
            setID(id);
        }

        updateLabel();
        return true;
    }
    // update the label
    void updateLabel() {

        if (m_stat != Stat::OTHER) {
            std::string statID = utils::numToString((int)m_stat);
            std::string statString = utils::numToString(GameStatsManager::get()->getStat(statID.c_str()));

            if (m_usesImage) {
                m_label->setString(statString.c_str());
            }
            else {
                m_label->setString((startStr + statString + endStr).c_str());
            }
        }
        else {
            std::string statString = utils::numToString(*val);
            if (m_usesImage) {
                m_label->setString(statString.c_str());
            }
            else {
                m_label->setString((startStr + statString + endStr).c_str());
            }
        }

        updateLayout();
    }
};