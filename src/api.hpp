#pragma once
/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

/**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;

#ifdef MY_MOD_ID
    #undef MY_MOD_ID
#endif
#define MY_MOD_ID "legowiifun.stat_display"

namespace stat_display_api {
    inline void addStatEvent(std::string start, std::string end, std::string id, int value) GEODE_EVENT_EXPORT_NORES(&addStatEvent, (start, end, id, value));
}