#include "StatNode.hpp"

    StatNode* StatNode::create(Stat stat, std::string id, GJGameLevel* level, int value) {
        auto* ret = new (std::nothrow) StatNode;
        if (ret && ret->init(stat, id, level, value)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    bool StatNode::init(Stat stat, std::string id, GJGameLevel* level, int value) {
        if (!CCNodeRGBA::init()) return false;
        val = value;
        m_stat = stat;
        setContentHeight(40);

        //create the sprite and label
        if (m_stat == Stat::OTHER) {
            otherStatID = otherStats;
            m_sprite = CCSprite::create(fmt::format("{}.png", id).c_str());
        }
        else {
            m_sprite = CCSprite::create(fmt::format("{}.png"_spr, id).c_str());
        }
        m_label = CCLabelBMFont::create("", "bigFont.fnt");
        m_label->setOpacity(64);
        setScale(0.3f);

        m_setting = id;
        m_setting[0] = std::toupper(m_setting[0]);
        m_usesImage = Mod::get()->getSettingValue<bool>("useImg");

        if (m_stat != Stat::OTHER) {
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
        }

        if (m_usesImage) {
            addChild(m_sprite);
        }
        addChild(m_label);

        // set the layout
        AxisLayout* layout = AxisLayout::create(Axis::Row)
            ->setAutoScale(true)
            ->setGap(5.f)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setAxisAlignment(AxisAlignment::Start);

        setLayout(layout);
        if (m_stat == Stat::OTHER) {
            setID(fmt::format("{}", id).c_str());
        }
        else {
            setID(fmt::format("{}"_spr, id).c_str());
        }

        updateLabel();
        return true;
    }
    // update the label
    void StatNode::updateLabel() {

        std::string statID = utils::numToString((int)m_stat);
        std::string statString;

        std::pair<std::string, std::string> parts;

        if (m_stat == Stat::OTHER) {
            parts = g_strings[static_cast<int>(m_stat) + otherStatID];
            statString = std::to_string(val);
        }
        else {
            parts = g_strings[static_cast<int>(m_stat)];
            statString = utils::numToString(GameStatsManager::get()->getStat(statID.c_str()));
        }

        if (m_usesImage) {
            m_label->setString(statString.c_str());
        }
        else {
            m_label->setString((parts.first + statString + parts.second).c_str());
        }

        updateLayout();
    }