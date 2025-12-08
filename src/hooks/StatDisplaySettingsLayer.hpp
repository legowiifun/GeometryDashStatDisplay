#include <Geode/modify/PauseLayer.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;
class $modify(StatDisplaySettingsLayer, PauseLayer) {
    void customSetup();
    void onSetting(CCObject*);
};