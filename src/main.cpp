/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

 /**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;
/**
* Various stats tracked by getStat
1: total jumps
2: total attempts
3: completed main levels
4: completed online levels
5: demons
6: stars
7: map packs
8: secret coins
9: players destroyed
10: liked/disliked levels
11: rated levels
12: user coins
13: diamonds
14: mana orbs
15: daily levels
22: total orbs
28: moons
29: diamond shards
40: gauntlets
41: list rewards
42: insane levels
Other values are unknown
*/

enum class Stat {
    JUMPS = 1, ATTEMPTS, MAIN_LEVELS, ONLINE_LEVELS, DEMONS, STARS, MAP_PACKS, SECRET_COINS, DESTROYED_PLAYERS,
    LIKED_LEVELS, RATED_LEVELS, USER_COINS, DIAMONDS, MANA_ORBS, DAILY_LEVELS, TOTAL_ORBS = 22,
    MOONS = 28, DIAMOND_SHARDS, GAUNTLETS = 40, LIST_REWARDS, INSANES
};

std::unordered_map<Stat, std::pair<std::string, std::string>> g_strings = {
    {Stat::JUMPS, {"You currently have jumped ", " times"}},
    {Stat::ATTEMPTS, {"You currently have done ", " attempts"}},
    {Stat::STARS, {"You currently have ", " stars"}},
    {Stat::ONLINE_LEVELS, {"You currently completed ", " online levels"}},
    {Stat::DEMONS, {"You currently have completed ", " demons"}},
    {Stat::LIKED_LEVELS, {"You currently have liked ", " levels"}},
    {Stat::RATED_LEVELS, {"You currently have rated ", " levels"}},
    {Stat::USER_COINS, {"You currently have ", " user coins"}},
    {Stat::MANA_ORBS, {"You currently have ", " mana orbs"}},
    {Stat::MOONS, {"You currently have ", " moons"}},
    {Stat::LIST_REWARDS, {"You currently have claimed ", " list rewards"}},
    {Stat::INSANES, {"You currently have completed ", " insane levels"}},
};

class StatNode : public CCNodeRGBA {

    Stat m_stat;
    CCLabelBMFont* m_label;
    CCSprite* m_sprite;
    std::string m_setting;
    bool m_usesImage;

public:
    static StatNode* create(Stat stat, std::string id, GJGameLevel* level) {
        auto* ret = new (std::nothrow) StatNode;
        if (ret && ret->init(stat, id, level)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    bool init(Stat stat, std::string id, GJGameLevel* level) {
        if (!CCNodeRGBA::init()) return false;
        m_stat = stat;
        setContentHeight(40);

        m_sprite = CCSprite::create(fmt::format("{}.png"_spr, id).c_str());
        m_label = CCLabelBMFont::create("", "bigFont.fnt");
        m_label->setOpacity(64);
        setScale(0.3f);

        m_setting = id;
        m_setting[0] = std::toupper(m_setting[0]);

        std::string display = Mod::get()->getSettingValue<std::string>(fmt::format("display{}", m_setting));
        m_usesImage = Mod::get()->getSettingValue<bool>("useImg");

        if (display != "always") {
            setVisible(false);
        }

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

        if (m_usesImage) {
            addChild(m_sprite);
        }
        addChild(m_label);

        AxisLayout* layout = AxisLayout::create(Axis::Row)
            ->setAutoScale(true)
            ->setGap(5.f)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setAxisAlignment(AxisAlignment::Start);

        setLayout(layout);
        setID(fmt::format("{}"_spr, id).c_str());

        updateLabel();
        return true;
    }

    void updateLabel() {

        std::string statID = utils::numToString((int)m_stat);
        std::string statString = utils::numToString(GameStatsManager::get()->getStat(statID.c_str()));
        
        std::pair<std::string, std::string> parts = g_strings[m_stat];

        if (m_usesImage) {
            m_label->setString(statString.c_str());
        }
        else {
            m_label->setString((parts.first + statString + parts.second).c_str());
        }

        updateLayout();
    }
};

class $modify(GJBaseGameLayer) {

    struct Fields {
        CCNode* m_statsContainer;
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
    
    void createTextLayers() {
        GJBaseGameLayer::createTextLayers();

        std::string position = Mod::get()->getSettingValue<std::string>("position");

        auto fields = m_fields.self();
        fields->m_statsContainer = CCNode::create();
        fields->m_statsContainer->setZOrder(999);

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
    }

    void createStatsNode(CCNode* parent, Stat stat, std::string id) {
        StatNode* node = StatNode::create(stat, id, m_level);
        parent->addChild(node);
    }

    // modify the update method
    void update(float dt) {
        GJBaseGameLayer::update(dt);
        updateText();
    }
};


/**
* Used to learn how getStat works
*/
class $modify(GameStatsManager) {
    int getStat(char const* p0) {
        int value = GameStatsManager::getStat(p0);
        bool logStatChanges = Mod::get()->template getSettingValue<bool>("enableLogs");
        if (logStatChanges) {
            geode::log::debug("p0 = {}, return value {}", p0, value);
        }
        return value;
    }
};