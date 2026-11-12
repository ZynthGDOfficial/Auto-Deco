#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(AutoDecoEditorUI, EditorUI) {
    void onAutoDecoButton(CCObject*) {
        FLAlertLayer::create(
            "Auto Deco",
            "The Auto Deco button is working.",
            "OK"
        )->show();
    }

    CCArray* createCustomItems() {
        auto items = EditorUI::createCustomItems();
        if (!items) {
            return nullptr;
        }

        auto sprite = ButtonSprite::create(
            "Z", 30, 0, 0.5f, true,
            "bigFont.fnt", "GJ_button_04.png", 30.0f
        );

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(AutoDecoEditorUI::onAutoDecoButton)
        );

        items->addObject(button);
        return items;
    }
};
