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
enum class stats {
    JUMPS = 1, ATTEMPTS, MAIN_LEVELS, ONLINE_LEVELS, DEMONS, STARS, MAP_PACKS, SECRET_COINS, DESTROYED_PLAYERS,
    LIKED_LEVELS, RATED_LEVELS, USER_COINS, DIAMONDS, MANA_ORBS, DAILY_LEVELS, TOTAL_ORBS = 22,
    MOONS = 28, DIAMOND_SHARDS, GAUNTLETS = 40, LIST_REWARDS, INSANES
};
class statText:public CCNodeRGBA {
public:
    CCLabelBMFont* text = nullptr;
    CCSprite* img = nullptr;
    int currentPos;
    static statText* create(std::string imgName = "") {
        auto* ret = new (std::nothrow) statText;
        if (ret && ret->init(imgName)) {
            ret->autorelease();
            return ret;
        }
        else {
            delete ret;
            return nullptr;
        }
    }
    bool init(std::string imgName="") {
        if (!CCNodeRGBA::init()) return false;
        auto director = CCDirector::sharedDirector();
        float screenTop = director->getScreenTop();
        float screenBottom = director->getScreenBottom();
        float screenLeft = director->getScreenLeft();
        float screenRight = director->getScreenRight();
        text = CCLabelBMFont::create("", "bigFont.fnt");
        if (imgName != "") {
            img = CCSprite::create(imgName.c_str());
            img->setID("img");
            img->setScale(0.3f);
            this->addChild(img);
        }
        text->setID("text");
        text->setScale(0.3f);
        this->addChild(text);
        text->setColor({ 255, 255, 255 });
        text->setOpacity(64);
        std::string position = Mod::get()->template getSettingValue<std::string>("position");
        if (position == "top-left") {
            text->setPosition(5, (int)(screenTop - currentPos));
            text->setAnchorPoint({ 0, 1 });
        }
        else if (position == "bottom-left") {
            text->setPosition(5, (int)(screenBottom + currentPos + 8));
            text->setAnchorPoint({ 0, 1 });
        }
        else if (position == "top-right") {
            text->setPosition(screenRight - 5, (int)(screenTop - currentPos));
            text->setAnchorPoint({ 1, 1 });
        }
        else {
            text->setPosition(screenRight - 5, (int)(screenBottom + currentPos + 8));
            text->setAnchorPoint({ 1, 1 });
        }
        currentPos = currentPos + 11;
        
        this->setLayout(
            AxisLayout::create(Axis::Row)
            ->setAutoScale(false)
            ->setGap(2.f)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAxisReverse(true)
        );
        this->getLayout()->ignoreInvisibleChildren(true);
        return true;
    }
    void createImg(std::string name) {
        img = CCSprite::create(name.c_str());
        img->setID("img");
    }
};

class allStats :public CCNodeRGBA {
public:
    statText* text = nullptr;
    int currentPos = 2;
    static allStats* create(GJGameLevel* level) {
        auto* ret = new (std::nothrow) allStats;
        if (ret && ret->init(level)) {
            ret->autorelease();
            return ret;
        }
        else {
            delete ret;
            return nullptr;
        }
    }
    bool init(GJGameLevel* level) {
        if (!CCNodeRGBA::init()) return false;
        this->setID("stats"_spr);
        //get settings
        std::string jumps = Mod::get()->template getSettingValue<std::string>("displayJumps");
        std::string attempts = Mod::get()->template getSettingValue<std::string>("displayAttempts");
        std::string online = Mod::get()->template getSettingValue<std::string>("displayOnlineLevels");
        std::string demons = Mod::get()->template getSettingValue<std::string>("displayDemons");
        std::string stars = Mod::get()->template getSettingValue<std::string>("displayStars");
        std::string liked = Mod::get()->template getSettingValue<std::string>("displayLiked");
        std::string rated = Mod::get()->template getSettingValue<std::string>("displayRated");
        std::string coins = Mod::get()->template getSettingValue<std::string>("displayCoins");
        std::string orbs = Mod::get()->template getSettingValue<std::string>("displayManaOrbs");
        std::string moons = Mod::get()->template getSettingValue<std::string>("displayMoons");
        std::string lists = Mod::get()->template getSettingValue<std::string>("displayLists");
        std::string insanes = Mod::get()->template getSettingValue<std::string>("displayInsanes");

        bool img = Mod::get()->template getSettingValue<bool>("useImg");

        if (jumps == "always") {
            if (img) {
                addLabel("jumps"_spr, "jumps.png"_spr);
            }
            else {
                addLabel("jumps"_spr);
            }
        }
        if (attempts == "always") {
            if (img) {
                addLabel("attempts"_spr, "attempts.png"_spr);
            }
            else {
                addLabel("attempts"_spr);
            }
        }
        if (stars == "always" || (level->m_stars.value() != 0 && !level->isPlatformer() && stars == "when playing rated classic levels")) {
            if (img) {
                addLabel("stars"_spr, "stars.png"_spr);
            }
            else {
                addLabel("stars"_spr);
            }
        }
        if (online == "always") {
            if (img) {
                addLabel("onlineLevels"_spr, "onlineLevels.png"_spr);
            }
            else {
                addLabel("onlineLevels"_spr);
            }
        }
        if (demons == "always" || (level->m_demon.value() == 1 && demons == "when playing demon levels")) {
            if (img) {
                addLabel("demons"_spr, "demons.png"_spr);
            }
            else {
                addLabel("demons"_spr);
            }
        }
        if (liked == "always") {
            if (img) {
                addLabel("liked"_spr, "liked.png"_spr);
            }
            else {
                addLabel("liked"_spr);
            }
        }
        if (rated == "always") {
            if (img) {
                addLabel("rated"_spr, "rated.png"_spr);
            }
            else {
                addLabel("rated"_spr);
            }
        }
        if (coins == "always" || (level->m_coinsVerified.value() >= 1 && coins == "when playing levels with rated coins")) {
            if (img) {
                addLabel("coins"_spr, "coins.png"_spr);
            }
            else {
                addLabel("coins"_spr);
            }
        }
        if (orbs == "always" || (level->m_stars != 0 && orbs == "when playing rated levels")) {
            if (img) {
                addLabel("orbs"_spr, "orbs.png"_spr);
            }
            else {
                addLabel("orbs"_spr);
            }
        }
        if (moons == "always" || (level->m_stars.value() != 0 && level->isPlatformer() && moons == "when playing rated platformer levels")) {
            if (img) {
                addLabel("moons"_spr, "moons.png"_spr);
            }
            else {
                addLabel("moons"_spr);
            }
        }
        if (lists == "always") {
            if (img) {
                addLabel("lists"_spr, "lists.png"_spr);
            }
            else {
                addLabel("lists"_spr);
            }
        }
        // Add extra checks for main insane levels
        if (insanes == "always" || ((((level->m_stars.value() == 8 || level->m_stars.value() == 9) && level->m_creatorName != "")
            || (level->m_creatorName == "" && level->m_stars.value() >= 10 && level->m_stars.value() < 14 && level->m_levelName != "Geometrical Dominator" && level->m_levelName != "Blast Processing"))
            && insanes == "when playing insane difficulty levels")) {
            if (img) {
                addLabel("insanes"_spr, "insanes.png"_spr);
            }
            else {
                addLabel("insanes"_spr);
            }
        }
        this->setLayout(
            AxisLayout::create(Axis::Column)
            ->setAutoScale(false)
            ->setGap(2.f)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAxisReverse(true)
        );
        this->updateLayout();

        this->scheduleUpdate();
        return true;
    }
    // add a label with the given ID
    void addLabel(std::string ID, std::string img = "") {
        std::string position = Mod::get()->template getSettingValue<std::string>("position");

        auto director = CCDirector::sharedDirector();
        float screenTop = director->getScreenTop();
        float screenBottom = director->getScreenBottom();
        float screenLeft = director->getScreenLeft();
        float screenRight = director->getScreenRight();

        text = statText::create(img);

        text->setID(ID);

        if (position == "top-left") {
            text->setPosition(5, (int)(screenTop - currentPos));
            text->setAnchorPoint({ 0, 1 });
        }
        else if (position == "bottom-left") {
            text->setPosition(5, (int)(screenBottom + currentPos + 8));
            text->setAnchorPoint({ 0, 1 });
        }
        else if (position == "top-right") {
            text->setPosition(screenRight - 5, (int)(screenTop - currentPos));
            text->setAnchorPoint({ 1, 1 });
        }
        else {
            text->setPosition(screenRight - 5, (int)(screenBottom + currentPos + 8));
            text->setAnchorPoint({ 1, 1 });
        }

        text->setZOrder(999);

        this->addChild(text);
        currentPos = currentPos + 11;
    }
};

/*class $modify(StatDisplayUI, UILayer) {
    struct Fields {
        statText* text = nullptr;
        int currentPos = 2;
    };
    // add a label with the given ID
    void addLabel(std::string ID, std::string img="") {
        std::string position = Mod::get()->template getSettingValue<std::string>("position");

        auto director = CCDirector::sharedDirector();
        float screenTop = director->getScreenTop();
        float screenBottom = director->getScreenBottom();
        float screenLeft = director->getScreenLeft();
        float screenRight = director->getScreenRight();

        m_fields->text = statText::create(img);

        m_fields->text->setID(ID);

        if (position == "top-left") {
            m_fields->text->setPosition(5, (int)(screenTop - m_fields->currentPos));
            m_fields->text->setAnchorPoint({ 0, 1 });
        }
        else if (position == "bottom-left") {
            m_fields->text->setPosition(5, (int)(screenBottom + m_fields->currentPos + 8));
            m_fields->text->setAnchorPoint({ 0, 1 });
        }
        else if (position == "top-right") {
            m_fields->text->setPosition(screenRight - 5, (int)(screenTop - m_fields->currentPos));
            m_fields->text->setAnchorPoint({ 1, 1 });
        }
        else {
            m_fields->text->setPosition(screenRight - 5, (int)(screenBottom + m_fields->currentPos + 8));
            m_fields->text->setAnchorPoint({ 1, 1 });
        }

        m_fields->text->setZOrder(999);

        this->addChild(m_fields->text);
        m_fields->currentPos = m_fields->currentPos + 11;
    }
};*/
class $modify(GJBaseGameLayer) {
    // to determine whether to display the stats: use GJGameLevel: m_level
    // m_level->m_stars.value() for stars/moons
    // m_level->isPlatformer() for if level is platformer
    // m_level->m_coinsVerified.value() for if coins are verified

    // update the text for a given id
    void updateText(std::string id, std::string before, stats stat, std::string after) {
        allStats* s = (allStats*)this->m_uiLayer->getChildByID("stats"_spr);
        if (s != nullptr) {
            statText* text = (statText*)s->getChildByID(id);
            if (text != nullptr) {
                CCLabelBMFont* t = text->text;
                std::string statID = std::to_string(static_cast<int>(stat));
                bool img = Mod::get()->template getSettingValue<bool>("useImg");
                if (t != nullptr) {
                    if (img) {
                        t->setString(std::to_string(GameStatsManager::get()->getStat(statID.c_str())).c_str());
                    }
                    else {
                        t->setString((before + std::to_string(GameStatsManager::get()->getStat(statID.c_str())) + after).c_str());
                    }
                }
            }
        }
    }
    void createTextLayers() {
        GJBaseGameLayer::createTextLayers();
        allStats* stat = allStats::create(m_level);
        m_uiLayer->addChild(stat);
        std::string position = Mod::get()->template getSettingValue<std::string>("position");
        auto director = CCDirector::sharedDirector();
        float screenTop = director->getScreenTop();
        float screenBottom = director->getScreenBottom();
        float screenLeft = director->getScreenLeft();
        float screenRight = director->getScreenRight();
        if (position == "top-left") {
            stat->setPosition(5, (int)(screenTop - 2));
            stat->setAnchorPoint({ 0, 1 });
        }
        else if (position == "bottom-left") {
            stat->setPosition(5, (int)(screenBottom + 10));
            stat->setAnchorPoint({ 0, 1 });
        }
        else if (position == "top-right") {
            stat->setPosition(screenRight - 5, (int)(screenTop - 2));
            stat->setAnchorPoint({ 1, 1 });
        }
        else {
            stat->setPosition(screenRight - 5, (int)(screenBottom + 10 ));
            stat->setAnchorPoint({ 1, 1 });
        }
        stat->setZOrder(999);
    }
    // modify the update method
    void update(float dt) {
        GJBaseGameLayer::update(dt);

        //get settings
        std::string jumps = Mod::get()->template getSettingValue<std::string>("displayJumps");
        std::string attempts = Mod::get()->template getSettingValue<std::string>("displayAttempts");
        std::string online = Mod::get()->template getSettingValue<std::string>("displayOnlineLevels");
        std::string demons = Mod::get()->template getSettingValue<std::string>("displayDemons");
        std::string stars = Mod::get()->template getSettingValue<std::string>("displayStars");
        std::string liked = Mod::get()->template getSettingValue<std::string>("displayLiked");
        std::string rated = Mod::get()->template getSettingValue<std::string>("displayRated");
        std::string coins = Mod::get()->template getSettingValue<std::string>("displayCoins");
        std::string orbs = Mod::get()->template getSettingValue<std::string>("displayManaOrbs");
        std::string moons = Mod::get()->template getSettingValue<std::string>("displayMoons");
        std::string lists = Mod::get()->template getSettingValue<std::string>("displayLists");
        std::string insanes = Mod::get()->template getSettingValue<std::string>("displayInsanes");

        // update stats
        if (jumps == "always") {
            updateText("jumps"_spr, "You currently have jumped ", stats::JUMPS, " times");
        }
        if (attempts == "always") {
            updateText("attempts"_spr, "You currently have done ", stats::ATTEMPTS, " attempts");
        }
        if (stars == "always" || (m_level->m_stars.value() != 0 && !m_level->isPlatformer() && stars == "when playing rated classic levels")) {
            updateText("stars"_spr, "You currently have ", stats::STARS, " stars");
        }
        if (online == "always") {
            updateText("onlineLevels"_spr, "You currently have completed ", stats::ONLINE_LEVELS, " online levels");
        }
        if (demons == "always" || (m_level->m_demon.value() == 1 && demons == "when playing demon levels")) {
            updateText("demons"_spr, "You currently have completed ", stats::DEMONS, " demons");
        }
        if (liked == "always") {
            updateText("liked"_spr, "You currently have liked ", stats::LIKED_LEVELS, " levels");
        }
        if (rated == "always") {
            updateText("rated"_spr, "You currently have rated ", stats::RATED_LEVELS, " levels");
        }
        if (coins == "always" || (m_level->m_coinsVerified.value() >= 1 && coins == "when playing levels with rated coins")) {
            updateText("coins"_spr, "You currently have ", stats::USER_COINS, " user coins");
        }
        if (orbs == "always" || (m_level->m_stars != 0 && orbs == "when playing rated levels")) {
            updateText("orbs"_spr, "You currently have ", stats::MANA_ORBS, " mana orbs");
        }
        if (moons == "always" || (m_level->m_stars.value() != 0 && m_level->isPlatformer() && moons == "when playing rated platformer levels")) {
            updateText("moons"_spr, "You currently have ", stats::MOONS, " moons");
        }
        if (lists == "always") {
            updateText("lists"_spr, "You currently have claimed ", stats::LIST_REWARDS, " list rewards");
        }
        // Add extra checks for main insane levels
        if (insanes == "always" || ((((m_level->m_stars.value() == 8 || m_level->m_stars.value() == 9) && m_level->m_creatorName != "")
            || (m_level->m_creatorName == "" && m_level->m_stars.value() >= 10 && m_level->m_stars.value() < 14 && m_level->m_levelName != "Geometrical Dominator" && m_level->m_levelName != "Blast Processing"))
            && insanes == "when playing insane difficulty levels")) {
            updateText("insanes"_spr, "You currently have completed ", stats::INSANES, " insane levels");
        }
    }
    void addLabel(std::string ID, std::string img="") {
        allStats* s = (allStats*)this->m_uiLayer->getChildByID("stats"_spr);
        s->addLabel(ID,img);

    }
   /* bool init() {
        if (GJBaseGameLayer::init() == false) {
            return false;
        }
        //get settings
        std::string jumps = Mod::get()->template getSettingValue<std::string>("displayJumps");
        std::string attempts = Mod::get()->template getSettingValue<std::string>("displayAttempts");
        std::string online = Mod::get()->template getSettingValue<std::string>("displayOnlineLevels");
        std::string demons = Mod::get()->template getSettingValue<std::string>("displayDemons");
        std::string stars = Mod::get()->template getSettingValue<std::string>("displayStars");
        std::string liked = Mod::get()->template getSettingValue<std::string>("displayLiked");
        std::string rated = Mod::get()->template getSettingValue<std::string>("displayRated");
        std::string coins = Mod::get()->template getSettingValue<std::string>("displayCoins");
        std::string orbs = Mod::get()->template getSettingValue<std::string>("displayManaOrbs");
        std::string moons = Mod::get()->template getSettingValue<std::string>("displayMoons");
        std::string lists = Mod::get()->template getSettingValue<std::string>("displayLists");
        std::string insanes = Mod::get()->template getSettingValue<std::string>("displayInsanes");
        if (jumps != "never") {
            addLabel("jumps"_spr);
        }
        if (attempts != "never") {
            addLabel("attempts"_spr);
        }
        if (stars != "never") {
            addLabel("stars"_spr);
        }
        if (online != "never") {
            addLabel("onlineLevels"_spr);
        }
        if (demons != "never") {
            addLabel("demons"_spr);
        }
        if (liked != "never") {
            addLabel("liked"_spr);
        }
        if (rated != "never") {
            addLabel("rated"_spr);
        }
        if (coins != "never") {
            addLabel("coins"_spr);
        }
        if (orbs != "never") {
            addLabel("orbs"_spr);
        }
        if (moons != "never") {
            addLabel("moons"_spr);
        }
        if (lists != "never") {
            addLabel("lists"_spr);
        }
        if (insanes != "never") {
            addLabel("insanes"_spr);
        }

        this->updateLayout();

        this->scheduleUpdate();

        return true;
    }*/
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