#include "StatDisplaySettingsLayer.hpp"
	// add the settings button to the pause menu
	void StatDisplaySettingsLayer::customSetup() {
		PauseLayer::customSetup();
		auto menu = getChildByID("right-button-menu");
		if ((menu!=nullptr) && Mod::get()->template getSettingValue<bool>("showPause")) {
			auto settingBtn = CCMenuItemSpriteExtra::create(
				CircleButtonSprite::createWithSprite("SettingsIcon.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Tiny),
				this, menu_selector(StatDisplaySettingsLayer::onSetting)
			);
			menu->addChild(settingBtn);
			menu->updateLayout();
		}
	}
	void StatDisplaySettingsLayer::onSetting(CCObject*) {
		geode::openSettingsPopup(Mod::get());
	}