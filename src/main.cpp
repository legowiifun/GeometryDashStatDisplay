/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

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
class $modify(customUILayer, UILayer) {
    struct Fields {
        CCNode* node = nullptr;
        CCLabelBMFont* text = nullptr;
        CCSprite* img = nullptr;
        int currentPos = 2;
        bool activateStats[43];
    };
    // add a label with the given ID
    void addLabel(std::string ID) {
        std::string position = Mod::get()->template getSettingValue<std::string>("position");
        bool img = Mod::get()->template getSettingValue<bool>("useImg");

        auto director = CCDirector::sharedDirector();
        float screenTop = director->getScreenTop();
        float screenBottom = director->getScreenBottom();
        float screenLeft = director->getScreenLeft();
        float screenRight = director->getScreenRight();

        // if you want image mode
        if (img) {
            m_fields->node = CCNode::create();
            m_fields->node->setID(ID);

            // img
            m_fields->img = CCSprite::create((ID + ".png").c_str());
            m_fields->img->setID("image");
            m_fields->img->setScale(0.3f);
            m_fields->img->setAnchorPoint({ 1, 1 });
            m_fields->node->addChild(m_fields->img);
            // text
            m_fields->text = CCLabelBMFont::create("", "bigFont.fnt");
            m_fields->text->setID("text");
            m_fields->text->setColor({ 255, 255, 255 });
            m_fields->text->setOpacity(64);
            m_fields->text->setZOrder(999);
            m_fields->text->setScale(0.3f);
            m_fields->text->setAnchorPoint({ 0, 1 });
            m_fields->node->addChild(m_fields->text);

            // set position
            if (position == "top-left") {
                m_fields->node->setPosition(13, (int)(screenTop - m_fields->currentPos));
                m_fields->node->setAnchorPoint({ 0, 1 });
            }
            else if (position == "bottom-left") {
                m_fields->node->setPosition(13, (int)(screenBottom + m_fields->currentPos + 8));
                m_fields->node->setAnchorPoint({ 0, 1 });
            }
            else if (position == "top-right") {
                m_fields->node->setPosition(screenRight - 13, (int)(screenTop - m_fields->currentPos));
                m_fields->node->setAnchorPoint({ 1, 1 });
            }
            else {
                m_fields->node->setPosition(screenRight - 13, (int)(screenBottom + m_fields->currentPos + 8));
                m_fields->node->setAnchorPoint({ 1, 1 });
            }

            this->addChild(m_fields->node);
        }
        else {
            // text mode
            m_fields->text = CCLabelBMFont::create("", "bigFont.fnt");
            m_fields->text->setID(ID);
            m_fields->text->setColor({ 255, 255, 255 });
            m_fields->text->setOpacity(64);
            m_fields->text->setZOrder(999);
            m_fields->text->setScale(0.3f);
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
            this->addChild(m_fields->text);
        }

        //m_fields->node->addChild(m_fields->text);


        m_fields->currentPos = m_fields->currentPos + 11;
    }
    bool init(GJBaseGameLayer * layer) {
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
/*
        if (jumps == "always") {
            m_fields->activateStats[(int)stats::JUMPS] = true;
        }
        if (attempts == "always") {
            m_fields->activateStats[(int)stats::ATTEMPTS] = true;
        }
        if (stars == "always" || (layer->m_level->m_stars.value() != 0 && !m_level->isPlatformer() && stars == "when playing rated classic levels")) {
            m_fields->activateStats[(int)stats::STARS] = true;
        }
        if (online == "always") {
            ui->m_fields->activateStats[(int)stats::ONLINE_LEVELS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::DEMONS] = (demons == "always");
        }
        else if (demons == "always" || (m_level->m_demon.value() == 1 && demons == "when playing demon levels")) {
            ui->m_fields->activateStats[(int)stats::DEMONS] = true;
        }
        if (liked == "always") {
            ui->m_fields->activateStats[(int)stats::LIKED_LEVELS] = true;
        }
        if (rated == "always") {
            ui->m_fields->activateStats[(int)stats::RATED_LEVELS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::USER_COINS] = (coins == "always");
        }
        else if (coins == "always" || (m_level->m_coinsVerified.value() >= 1 && coins == "when playing levels with rated coins")) {
            ui->m_fields->activateStats[(int)stats::USER_COINS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::MANA_ORBS] = (orbs == "always");
        }
        else if (orbs == "always" || (m_level->m_stars != 0 && orbs == "when playing rated levels")) {
            ui->m_fields->activateStats[(int)stats::MANA_ORBS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::MOONS] = (moons == "always");
        }
        else if (moons == "always" || (m_level->m_stars.value() != 0 && m_level->isPlatformer() && moons == "when playing rated platformer levels")) {
            ui->m_fields->activateStats[(int)stats::MOONS] = true;
        }
        if (lists == "always") {
            ui->m_fields->activateStats[(int)stats::LIST_REWARDS] = true;
        }
        // Add extra checks for main insane levels
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::INSANES] = (insanes == "always");
        }
        else if (insanes == "always" || ((((m_level->m_stars.value() == 8 || m_level->m_stars.value() == 9) && m_level->m_creatorName != "")
            || (m_level->m_creatorName == "" && m_level->m_stars.value() >= 10 && m_level->m_stars.value() < 14 && m_level->m_levelName != "Geometrical Dominator" && m_level->m_levelName != "Blast Processing"))
            && insanes == "when playing insane difficulty levels")) {
            ui->m_fields->activateStats[(int)stats::INSANES] = true;
        }*/


        if (!UILayer::init(layer)) return false;
        geode::log::debug("m_gameLayer->m_level={}", m_gameLayer->m_level);



        if (m_fields->activateStats[(int)stats::JUMPS]) {
            addLabel("jumps"_spr);
        }
        geode::log::debug("UI jump activation (in UI) = {}", m_fields->activateStats[(int)stats::JUMPS]);

        if (m_fields->activateStats[(int)stats::ATTEMPTS]) {
            addLabel("attempts"_spr);
        }
        if (m_fields->activateStats[(int)stats::STARS]) {
            addLabel("stars"_spr);
        }
        if (m_fields->activateStats[(int)stats::ONLINE_LEVELS]) {
            addLabel("onlineLevels"_spr);
        }
        if (m_fields->activateStats[(int)stats::DEMONS]) {
            addLabel("demons"_spr);
        }
        if (m_fields->activateStats[(int)stats::LIKED_LEVELS]) {
            addLabel("liked"_spr);
        }
        if (m_fields->activateStats[(int)stats::RATED_LEVELS]) {
            addLabel("rated"_spr);
        }
        if (m_fields->activateStats[(int)stats::USER_COINS]) {
            addLabel("coins"_spr);
        }
        if (m_fields->activateStats[(int)stats::MANA_ORBS]) {
            addLabel("orbs"_spr);
        }
        if (m_fields->activateStats[(int)stats::MOONS]) {
            addLabel("moons"_spr);
        }
        if (m_fields->activateStats[(int)stats::LIST_REWARDS]) {
            addLabel("lists"_spr);
        }
        if (m_fields->activateStats[(int)stats::INSANES]) {
            addLabel("insanes"_spr);
        }

        this->updateLayout();

        this->scheduleUpdate();

        return true;
    }
};
#include<Geode/modify/PlayLayer.hpp>
class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        geode::log::debug("level={}", level);

        if (PlayLayer::init(level, useReplay, dontCreateObjects) == false) {
            return false;
        }

    customUILayer* ui = (customUILayer*) m_uiLayer;
    for (int i = 0; i < 43; i++) {
        ui->m_fields->activateStats[i] = false;
    }

    // get settings
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
    if (jumps == "always") {
        ui->m_fields->activateStats[(int)stats::JUMPS] = true;
    }
    geode::log::debug("UI jump activation = {}", ui->m_fields->activateStats[(int)stats::JUMPS]);

    if (attempts == "always") {
        ui->m_fields->activateStats[(int)stats::ATTEMPTS] = true;
    }
    if (level == nullptr) {
        ui->m_fields->activateStats[(int)stats::STARS] = (stars == "always");
    }
    else if (stars == "always" || (level->m_stars.value() != 0 && !level->isPlatformer() && stars == "when playing rated classic levels")) {
        ui->m_fields->activateStats[(int)stats::STARS] = true;
    }
    if (online == "always") {
        ui->m_fields->activateStats[(int)stats::ONLINE_LEVELS] = true;
    }
    if (level == nullptr) {
        ui->m_fields->activateStats[(int)stats::DEMONS] = (demons == "always");
    }
    else if (demons == "always" || (level->m_demon.value() == 1 && demons == "when playing demon levels")) {
        ui->m_fields->activateStats[(int)stats::DEMONS] = true;
    }
    if (liked == "always") {
        ui->m_fields->activateStats[(int)stats::LIKED_LEVELS] = true;
    }
    if (rated == "always") {
        ui->m_fields->activateStats[(int)stats::RATED_LEVELS] = true;
    }
    if (level == nullptr) {
        ui->m_fields->activateStats[(int)stats::USER_COINS] = (coins == "always");
    }
    else if (coins == "always" || (level->m_coinsVerified.value() >= 1 && coins == "when playing levels with rated coins")) {
        ui->m_fields->activateStats[(int)stats::USER_COINS] = true;
    }
    if (level == nullptr) {
        ui->m_fields->activateStats[(int)stats::MANA_ORBS] = (orbs == "always");
    }
    else if (orbs == "always" || (level->m_stars != 0 && orbs == "when playing rated levels")) {
        ui->m_fields->activateStats[(int)stats::MANA_ORBS] = true;
    }
    if (level == nullptr) {
        ui->m_fields->activateStats[(int)stats::MOONS] = (moons == "always");
    }
    else if (moons == "always" || (level->m_stars.value() != 0 && level->isPlatformer() && moons == "when playing rated platformer levels")) {
        ui->m_fields->activateStats[(int)stats::MOONS] = true;
    }
    if (lists == "always") {
        ui->m_fields->activateStats[(int)stats::LIST_REWARDS] = true;
    }
    // Add extra checks for main insane levels
    if (level == nullptr) {
        ui->m_fields->activateStats[(int)stats::INSANES] = (insanes == "always");
    }
    else if (insanes == "always" || ((((level->m_stars.value() == 8 || level->m_stars.value() == 9) && level->m_creatorName != "")
        || (level->m_creatorName == "" && level->m_stars.value() >= 10 && level->m_stars.value() < 14 && level->m_levelName != "Geometrical Dominator" && level->m_levelName != "Blast Processing"))
        && insanes == "when playing insane difficulty levels")) {
        ui->m_fields->activateStats[(int)stats::INSANES] = true;
    }

    return true;
    }
};
class $modify(GJBaseGameLayer) {

    /*bool init() {
        if (GJBaseGameLayer::init() == false) {
            return false;
        }
 
        customUILayer* ui = (customUILayer*) m_uiLayer;
        for (int i = 0; i < 43; i++) {
            ui->m_fields->activateStats[i] = false;
        }

        // get settings
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
        
        if (jumps == "always") {
            ui->m_fields->activateStats[(int)stats::JUMPS] = true;
        }
        if (attempts == "always") {
            ui->m_fields->activateStats[(int)stats::ATTEMPTS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::STARS] = (stars == "always");
        }
        else if (stars == "always" || (m_level->m_stars.value() != 0 && !m_level->isPlatformer() && stars == "when playing rated classic levels")) {
            ui->m_fields->activateStats[(int)stats::STARS] = true;
        }
        if (online == "always") {
            ui->m_fields->activateStats[(int)stats::ONLINE_LEVELS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::DEMONS] = (demons == "always");
        }
        else if (demons == "always" || (m_level->m_demon.value() == 1 && demons == "when playing demon levels")) {
            ui->m_fields->activateStats[(int)stats::DEMONS] = true;
        }
        if (liked == "always") {
            ui->m_fields->activateStats[(int)stats::LIKED_LEVELS] = true;
        }
        if (rated == "always") {
            ui->m_fields->activateStats[(int)stats::RATED_LEVELS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::USER_COINS] = (coins == "always");
        }
        else if (coins == "always" || (m_level->m_coinsVerified.value() >= 1 && coins == "when playing levels with rated coins")) {
            ui->m_fields->activateStats[(int)stats::USER_COINS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::MANA_ORBS] = (orbs == "always");
        }
        else if (orbs == "always" || (m_level->m_stars != 0 && orbs == "when playing rated levels")) {
            ui->m_fields->activateStats[(int)stats::MANA_ORBS] = true;
        }
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::MOONS] = (moons == "always");
        }
        else if (moons == "always" || (m_level->m_stars.value() != 0 && m_level->isPlatformer() && moons == "when playing rated platformer levels")) {
            ui->m_fields->activateStats[(int)stats::MOONS] = true;
        }
        if (lists == "always") {
            ui->m_fields->activateStats[(int)stats::LIST_REWARDS] = true;
        }
        // Add extra checks for main insane levels
        if (m_level == nullptr) {
            ui->m_fields->activateStats[(int)stats::INSANES] = (insanes == "always");
        }
        else if (insanes == "always" || ((((m_level->m_stars.value() == 8 || m_level->m_stars.value() == 9) && m_level->m_creatorName != "")
            || (m_level->m_creatorName == "" && m_level->m_stars.value() >= 10 && m_level->m_stars.value() < 14 && m_level->m_levelName != "Geometrical Dominator" && m_level->m_levelName != "Blast Processing"))
            && insanes == "when playing insane difficulty levels")) {
            ui->m_fields->activateStats[(int)stats::INSANES] = true;
        }

        return true;
    }*/

    // to determine whether to display the stats: use GJGameLevel: m_level
    // m_level->m_stars.value() for stars/moons
    // m_level->isPlatformer() for if level is platformer
    // m_level->m_coinsVerified.value() for if coins are verified

    // Set the visibility status of the image for a given id
    void updateImg(CCSprite* img) {
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
        

        // check status
        if (jumps == "always" && (img->getParent()->getID() == "jumps"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying jumps");
        }
        else if (attempts == "always" && (img->getParent()->getID() == "attempts"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying attempts");
        }
        else if ((stars == "always" || (m_level->m_stars.value() != 0 && !m_level->isPlatformer() && stars == "when playing rated classic levels"))
            && (img->getParent()->getID() == "stars"_spr)) {
            img->setVisible(true);
        }
        else if (online == "always" && (img->getParent()->getID() == "onlineLevels"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying online levels");
        }
        else if ((demons == "always" || (m_level->m_demon.value() == 1 && demons == "when playing demon levels")) && (img->getParent()->getID() == "demons"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying demons");
        }
        else if (liked == "always" && (img->getParent()->getID() == "liked"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying liked levels");
        }
        else if (rated == "always" && (img->getParent()->getID() == "rated"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying rated levels");
        }
        else if ((coins == "always" || (m_level->m_coinsVerified.value() >= 1 && coins == "when playing levels with rated coins"))
            && (img->getParent()->getID() == "coins"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying user coins");
        }
        else if ((orbs == "always" || (m_level->m_stars != 0 && orbs == "when playing rated levels")) && (img->getParent()->getID() == "orbs"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying mana orbs");
        }
        else if ((moons == "always" || (m_level->m_stars.value() != 0 && m_level->isPlatformer() && moons == "when playing rated platformer levels"))
            && (img->getParent()->getID() == "moons"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying moons");
        }
        else if (lists == "always" && (img->getParent()->getID() == "lists"_spr)) {
            img->setVisible(true);
            geode::log::debug("Displaying lists");
        }
        // Add extra checks for main insane levels
        else if ((insanes == "always" || ((((m_level->m_stars.value() == 8 || m_level->m_stars.value() == 9) && m_level->m_creatorName != "")
            || (m_level->m_creatorName == "" && m_level->m_stars.value() >= 10 && m_level->m_stars.value() < 14 && m_level->m_levelName != "Geometrical Dominator" && m_level->m_levelName != "Blast Processing"))
            && insanes == "when playing insane difficulty levels")) && (img->getParent()->getID() == "insanes"_spr)) {
            img->setVisible(true);
        }
        else {
            img->setVisible(false);
        }
    }
    
    // update the text for a given id
    void updateText(std::string id, std::string before, stats stat, std::string after, int posOffset) {
        bool img = Mod::get()->template getSettingValue<bool>("useImg");
        CCLabelBMFont* text = nullptr;
        if (!img) {
            text = (CCLabelBMFont*)this->m_uiLayer->getChildByID(id);
        }
        // if you wish to use the image mode, initialize the text and the image as children of the node
        else {
            CCNode* n = this->m_uiLayer->getChildByID(id);
            if (n) {
                text = (CCLabelBMFont*)n->getChildByID("text");
                CCSprite* img = (CCSprite*)n->getChildByID("image");
               // if (img) {
                //    updateImg(img);
              //  }
            }
        }
        std::string statID = std::to_string(static_cast<int>(stat));

        if (text!=nullptr) {
            if (!img) {
                text->setString((before + std::to_string(GameStatsManager::get()->getStat(statID.c_str())) + after).c_str());
            }
            else {
                text->setString(std::to_string(GameStatsManager::get()->getStat(statID.c_str())).c_str());
            }
        }
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
            updateText("jumps"_spr, "You currently have jumped ", stats::JUMPS, " times", -13);
        }
        if (attempts == "always") {
            updateText("attempts"_spr, "You currently have done ", stats::ATTEMPTS, " attempts", -12);
        }
        if (stars == "always" || (m_level->m_stars.value() != 0 && !m_level->isPlatformer() && stars == "when playing rated classic levels")) {
            updateText("stars"_spr, "You currently have ", stats::STARS, " stars", 11);
        }
        if (online == "always") { 
            updateText("onlineLevels"_spr, "You currently have completed ", stats::ONLINE_LEVELS, " online levels", -34);
        }
        if (demons == "always" || (m_level->m_demon.value() == 1 && demons == "when playing demon levels")) { 
            updateText("demons"_spr, "You currently have completed ", stats::DEMONS, " demons", -17);
        }
        if (liked == "always") {
            updateText("liked"_spr, "You currently have liked ", stats::LIKED_LEVELS, " levels", -4);
        }
        if (rated == "always") {
            updateText("rated"_spr, "You currently have rated ", stats::RATED_LEVELS, " levels", -6);
        }
        if (coins == "always" || (m_level->m_coinsVerified.value()>=1 && coins=="when playing levels with rated coins")) {
            updateText("coins"_spr, "You currently have ", stats::USER_COINS, " user coins", 0);
        }
        if (orbs == "always" || (m_level->m_stars!=0 && orbs=="when playing rated levels")) {
            updateText("orbs"_spr, "You currently have ", stats::MANA_ORBS, " mana orbs",3);
        }
        if (moons == "always" || (m_level->m_stars.value() != 0 && m_level->isPlatformer() && moons == "when playing rated platformer levels")) {
            updateText("moons"_spr, "You currently have ", stats::MOONS, " moons",10);
        }
        if (lists == "always") {
            updateText("lists"_spr, "You currently have claimed ", stats::LIST_REWARDS, " list rewards",-23);
        }
        // Add extra checks for main insane levels
        if (insanes == "always" || ((((m_level->m_stars.value()==8||m_level->m_stars.value()==9) && m_level->m_creatorName!="")
            || (m_level->m_creatorName == ""&&m_level->m_stars.value()>=10&&m_level->m_stars.value()<14&&m_level->m_levelName!="Geometrical Dominator" && m_level->m_levelName != "Blast Processing"))
            && insanes == "when playing insane difficulty levels")) {
            updateText("insanes"_spr, "You currently have completed ", stats::INSANES, " insane levels",-34);
        }
    }
};




/**
* Used to learn how getStat works
*/
class $modify(GameStatsManager) {
    int getStat(char const* p0) {
        int value=GameStatsManager::getStat(p0);
        bool logStatChanges = Mod::get()->template getSettingValue<bool>("enableLogs");
        if (logStatChanges) {
            geode::log::debug("p0 = {}, return value {}", p0, value);
        }
        return value;
    }
};