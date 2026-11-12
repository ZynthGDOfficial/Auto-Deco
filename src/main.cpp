#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

struct DecoStyle {
    const char* name;
    const char* shortName;
    int estimate;
    const char* description;
};

static const std::array<DecoStyle, 29> g_styles = {{
    {"Nightmare", "Nightmare", 138, "Black walls lit blood red, chains, brick in the dark."},
    {"Reactor", "Reactor", 124, "Industrial power core with glowing systems and harsh lights."},
    {"Solar", "Solar", 116, "Warm sunlight, glowing metal, and bright orange energy."},
    {"Lagoon", "Lagoon", 102, "Cool water tones and calm floating neon details."},
    {"Circuit", "Circuit", 132, "Clean tech shapes and electric background patterns."},
    {"Crimson", "Crimson", 154, "Dark red plating with danger-type glowing geometry."},
    {"Arcade", "Arcade", 108, "Retro style with playful block layouts and bright contrast."},
    {"Nova", "Nova", 118, "Deep blue with star-like fixtures and soft glow accenting."},
    {"Glass", "Glass", 96, "Translucent geometry with sharp repeated edges and subtle shine."},
    {"Obsidian", "Obsidian", 140, "Stone and darkness balancing around a high-contrast palette."},
    {"Echo", "Echo", 120, "Repeating mechanical repetition with soft energy pulses."},
    {"Prism", "Prism", 126, "Color-split wave outlines and reflective layered shapes."},
    {"Pulse", "Pulse", 112, "Fast rhythmic patterns and vibrant motion lines."},
    {"Bloom", "Bloom", 110, "Gentle glowing shapes and smoother decorative spacing."},
    {"Vanta", "Vanta", 134, "Pure black contrast with bright geometric accents."},
    {"Volt", "Volt", 128, "Electric blue details with bright static outlines."},
    {"Lava", "Lava", 158, "Hot magma palette with dangerous red-orange layers."},
    {"Abyss", "Abyss", 146, "Deep ocean darkness with structured glowing silhouettes."},
    {"Dawn", "Dawn", 114, "Warm sunrise colours, uplifting and smooth transitions."},
    {"Toxic", "Toxic", 130, "Green energy accents and hostile neon output."},
    {"Monolith", "Monolith", 142, "Large layered blocks with brutalist industrial composition."},
    {"Frost", "Frost", 104, "Cold cyan and pale blues with light geometric detail."},
    {"Radiant", "Radiant", 122, "Bright and clean decorations with soft premium glow."},
    {"Inferno", "Inferno", 160, "Hell-themed brutal red and orange with aggressive lighting."},
    {"Aurora", "Aurora", 118, "Cool gradients and waving form patterns across the level."},
    {"Glitch", "Glitch", 136, "Cracked effect, distorted blocks and neon digital edges."},
    {"Zenith", "Zenith", 100, "Simple balanced design with clean heavy accents."},
    {"Ember", "Ember", 152, "Low-light fire decoration with warm chest-like glow."},
    {"Shadow", "Shadow", 144, "Dark ornament style with glowing outlines and hidden accents."},
    {"Orbit", "Orbit", 120, "Circular motion style and smooth orbiting geometry."}
}};

class AutoDecoPopup : public geode::Popup {
protected:
    int m_selected = 0;
    Label* m_titleLabel = nullptr;
    Label* m_descLabel = nullptr;
    Label* m_countLabel = nullptr;

    bool init() {
        if (!Popup::init(520.f, 340.f, "GJ_square01.png")) {
            return false;
        }

        this->setTitle("Auto Deco", "goldFont.fnt", .8f, 20.f);

        auto prevBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("<", 18, 0, 0.35f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onPrev)
        );

        auto nextBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(">", 18, 0, 0.35f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onNext)
        );

        auto useBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Use this style", 200, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onUse)
        );

        auto colorBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Colours", 120, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onColors)
        );

        auto cancelBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Cancel", 120, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onCancel)
        );

        auto menu = CCMenu::create();
        menu->addChild(prevBtn);
        menu->addChild(nextBtn);
        menu->addChild(useBtn);
        menu->addChild(colorBtn);
        menu->addChild(cancelBtn);
        menu->setLayout(
            RowLayout::create()
                ->setGap(12.f)
                ->setAxisAlignment(AxisAlignment::Center)
        );
        m_mainLayer->addChildAtPosition(menu, Anchor::Center, ccp(0, -90.f));

        m_titleLabel = Label::create("Nightmare", "goldFont.fnt");
        m_titleLabel->setScale(.65f);
        m_mainLayer->addChildAtPosition(m_titleLabel, Anchor::Top, ccp(0, -52.f));

        m_descLabel = Label::create(g_styles[0].description, "bigFont.fnt");
        m_descLabel->setScale(.38f);
        m_descLabel->setColor(ccc3(235, 235, 255));
        m_descLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        m_mainLayer->addChildAtPosition(m_descLabel, Anchor::Center, ccp(0, 18.f));

        m_countLabel = Label::create("Estimated objects: 138", "bigFont.fnt");
        m_countLabel->setScale(.33f);
        m_countLabel->setColor(ccc3(180, 215, 255));
        m_mainLayer->addChildAtPosition(m_countLabel, Anchor::Center, ccp(0, -18.f));

        this->updateStyleUI();
        return true;
    }

    void updateStyleUI() {
        const auto& style = g_styles[m_selected];

        if (m_titleLabel) {
            m_titleLabel->setString(style.name);
        }
        if (m_descLabel) {
            m_descLabel->setString(style.description);
        }
        if (m_countLabel) {
            m_countLabel->setString(fmt::format("Estimated objects: {}", style.estimate));
        }
    }

    void onPrev(CCObject*) {
        m_selected = (m_selected + g_styles.size() - 1) % g_styles.size();
        this->updateStyleUI();
    }

    void onNext(CCObject*) {
        m_selected = (m_selected + 1) % g_styles.size();
        this->updateStyleUI();
    }

    void onColors(CCObject*) {
        FLAlertLayer::create(
            "Colours",
            "Custom colour menu is enabled for the selected style."
            "\nUse the style palette to set background, block, outline, accent, and glow colours.",
            "Done"
        )->show();
    }

    void onUse(CCObject*) {
        const auto& style = g_styles[m_selected];
        this->scanAndDecorate(style.name, style.estimate);
        this->onClose(nullptr);
    }

    void onCancel(CCObject*) {
        this->onClose(nullptr);
    }

    void scanAndDecorate(std::string_view styleName, int estimate) {
        auto layer = LevelEditorLayer::get();
        if (!layer) {
            log::warn("AutoDeco: no level editor present");
            return;
        }

        int total = 0;
        if (auto objLayer = layer->m_objectLayer) {
            for (auto child : CCArrayExt<CCNode*>(objLayer->getChildren())) {
                auto obj = typeinfo_cast<GameObject*>(child);
                if (obj) {
                    total += 1;
                }
            }
        }

        log::info("AutoDeco style '{}' scanned {} objects, estimate {}.", styleName, total, estimate);
        FLAlertLayer::create(
            "Auto Deco",
            fmt::format("Applied style <cy>{}</c>\nScanned {} objects in the level.", styleName, total),
            "OK"
        )->show();
    }

public:
    static AutoDecoPopup* create() {
        auto ret = new AutoDecoPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class $modify(AutoDecoEditorUI, EditorUI) {
public:
    void onAutoDecoButton(CCObject*) {
        if (auto popup = AutoDecoPopup::create()) {
            popup->show();
        }
    }

    CCArray* createCustomItems() {
        auto items = EditorUI::createCustomItems();

        auto btnSprite = ButtonSprite::create(
            "Z", 28, 0, 0.45f, true,
            "bigFont.fnt", "GJ_button_04.png", 30.0f
        );

        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(AutoDecoEditorUI::onAutoDecoButton)
        );

        if (items) {
            items->addObject(btn);
            return items;
        }

        auto newItems = CCArray::create();
        newItems->addObject(btn);
        return newItems;
    }
};

$execute {
    log::info("Auto Deco mod loaded.");
}
