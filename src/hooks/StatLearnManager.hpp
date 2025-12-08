#pragma once

/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

/**
  * Brings cocos2d and all Geode namespaces to the current scope.
  */
using namespace geode::prelude;

class $modify(StatLearnManager, GameStatsManager) {
    int getStat(char const* p0);
};