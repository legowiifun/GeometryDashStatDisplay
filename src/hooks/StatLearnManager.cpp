#include "StatLearnManager.hpp"

/**
* Used to learn how getStat works
*/
int StatLearnManager::getStat(char const* p0) {
    int value = GameStatsManager::getStat(p0);
    bool logStatChanges = Mod::get()->template getSettingValue<bool>("enableLogs");
    if (logStatChanges) {
        geode::log::debug("p0 = {}, return value {}", p0, value);
    }
    return value;
}