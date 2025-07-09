#pragma once
/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

#include "Stat.cpp"
#include "hooks/StatDisplayBaseGameLayer.cpp"
#include "StatNode.cpp"
 /**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;

// set up filter for dispatch event
using EventAddStat = geode::DispatchEvent<std::string, std::string, std::string, CCSprite*, int*>;
template <class>
struct ToFilterImpl;

template <class... Args>
struct ToFilterImpl<geode::DispatchEvent<Args...>> {
    using type = geode::DispatchFilter <Args...>;
};

template <class T>
using ToFilter = typename ToFilterImpl<T>::type;



// The strings used in text mode
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



/**
* Used to learn how getStat works
*/
class $modify(StatLearnManager, GameStatsManager) {
    int getStat(char const* p0) {
        int value = GameStatsManager::getStat(p0);
        bool logStatChanges = Mod::get()->template getSettingValue<bool>("enableLogs");
        if (logStatChanges) {
            geode::log::debug("p0 = {}, return value {}", p0, value);
        }
        return value;
    }
};

// declare dispatchEvents
$execute{
    // add a stat
    // contains start text, end text, id, and an image
    new EventListener(+[](std::string start, std::string end, std::string id, CCSprite* img, int* value) {
        StatDisplayBaseGameLayer* layer = static_cast<StatDisplayBaseGameLayer*>(GJBaseGameLayer::get());
        layer->createStatsNode(layer->m_fields->m_statsContainer, Stat::OTHER, id, value, start, end);
        return ListenerResult::Stop;
    }, ToFilter<EventAddStat>("legowiifun.stat_display/addStat"));
};