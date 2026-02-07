#pragma once
/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

#include "Stat.cpp"
#include "hooks/StatDisplayBaseGameLayer.hpp"

/**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;

inline int otherStats = 0;

// The strings used in text mode
inline std::unordered_map<int, std::pair<std::string, std::string>> g_strings = {
    {static_cast<int>(Stat::JUMPS), {"You currently have jumped ", " times"}},
    {static_cast<int>(Stat::ATTEMPTS), {"You currently have done ", " attempts"}},
    {static_cast<int>(Stat::STARS), {"You currently have ", " stars"}},
    {static_cast<int>(Stat::ONLINE_LEVELS), {"You currently completed ", " online levels"}},
    {static_cast<int>(Stat::DEMONS), {"You currently have completed ", " demons"}},
    {static_cast<int>(Stat::LIKED_LEVELS), {"You currently have liked ", " levels"}},
    {static_cast<int>(Stat::RATED_LEVELS), {"You currently have rated ", " levels"}},
    {static_cast<int>(Stat::USER_COINS), {"You currently have ", " user coins"}},
    {static_cast<int>(Stat::MANA_ORBS), {"You currently have ", " mana orbs"}},
    {static_cast<int>(Stat::MOONS), {"You currently have ", " moons"}},
    {static_cast<int>(Stat::LIST_REWARDS), {"You currently have claimed ", " list rewards"}},
    {static_cast<int>(Stat::INSANES), {"You currently have completed ", " insane levels"}},
};
