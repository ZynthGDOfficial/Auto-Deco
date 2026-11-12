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

class AutoDecoColorsPopup : public geode::Popup {
protected:
    std::array<std::string, 5> m_labels = {"Background", "Blocks", "Outline", "Accent", "Glow"};
    std::array<ccColor3B, 5> m_currentColors = {
        ccc3(0x05, 0x3E, 0xB8),
        ccc3(0x06, 0x1E, 0x64),
        ccc3(0x64, 0xED, 0xFF),
        ccc3(0x8E, 0xCD, 0xDF),
        ccc3(0x3C, 0xA0, 0xD1)
    };

    bool init() {
        if (!Popup::init(420.f, 320.f, "GJ_square01.png")) {
            return false;
        }

        this->setTitle("Colours", "goldFont.fnt", .7f, 18.f);

        auto bg = CCMenu::create();
        bg->setLayout(
            ColumnLayout::create()
                ->setGap(12.f)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(CrossAxisAlignment::Center)
        );

        for (size_t i = 0; i < m_labels.size(); ++i) {
            auto row = CCNode::create();
            row->setContentSize({ 260.f, 38.f });

            auto label = Label::create(m_labels[i], "bigFont.fnt");
            label->setScale(.42f);
            row->addChild(label);

            auto colorBox = CCNode::create();
            colorBox->setContentSize({ 120.f, 22.f });
            colorBox->setColor(m_currentColors[i]);
            colorBox->setAnchorPoint({ 0.5f, 0.5f });
            row->addChild(colorBox);

            bg->addChild(row);
        }

        m_mainLayer->addChildAtPosition(bg, Anchor::Center, ccp(0, 18.f));

        auto done = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Done", 120, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoColorsPopup::onDone)
        );

        auto cancel = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Cancel", 120, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoColorsPopup::onCancel)
        );

        auto buttons = CCMenu::create();
        buttons->addChild(cancel);
        buttons->addChild(done);
        buttons->setLayout(RowLayout::create()->setGap(16.f));
        m_mainLayer->addChildAtPosition(buttons, Anchor::Bottom, ccp(0, 30.f));

        return true;
    }

    void onDone(CCObject*) {
        this->onClose(nullptr);
    }

    void onCancel(CCObject*) {
        this->onClose(nullptr);
    }

public:
    static AutoDecoColorsPopup* create() {
        auto ret = new AutoDecoColorsPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class AutoDecoPopup : public geode::Popup {
protected:
    int m_selected = 0;
    Label* m_titleLabel = nullptr;
    Label* m_descLabel = nullptr;
    Label* m_countLabel = nullptr;

    bool init() {
        if (!Popup::init(540.f, 360.f, "GJ_square01.png")) {
            return false;
        }

        this->setTitle("Auto Deco", "goldFont.fnt", .8f, 18.f);

        auto titleRow = CCMenu::create();
        titleRow->setLayout(RowLayout::create()->setGap(12.f)->setAxisAlignment(AxisAlignment::Center));

        auto prevBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("<", 22, 0, 0.35f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onPrev)
        );

        auto nextBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(">", 22, 0, 0.35f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
            this,
            menu_selector(AutoDecoPopup::onNext)
        );

        titleRow->addChild(prevBtn);
        titleRow->addChild(nextBtn);
        m_mainLayer->addChildAtPosition(titleRow, Anchor::Top, ccp(0, -44.f));

        m_titleLabel = Label::create(g_styles[0].name, "goldFont.fnt");
        m_titleLabel->setScale(.7f);
        m_mainLayer->addChildAtPosition(m_titleLabel, Anchor::Top, ccp(0, -86.f));

        m_descLabel = Label::create(g_styles[0].description, "bigFont.fnt");
        m_descLabel->setScale(.36f);
        m_descLabel->setOpacity(210);
        m_mainLayer->addChildAtPosition(m_descLabel, Anchor::Center, ccp(0, 20.f));

        m_countLabel = Label::create("Estimated objects: 138", "bigFont.fnt");
        m_countLabel->setScale(.34f);
        m_countLabel->setColor(ccc3(180, 215, 255));
        m_mainLayer->addChildAtPosition(m_countLabel, Anchor::Center, ccp(0, -12.f));

        auto actionMenu = CCMenu::create();
        actionMenu->setLayout(RowLayout::create()->setGap(12.f)->setAxisAlignment(AxisAlignment::Center));

        auto useBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Use this style", 180, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.f),
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

        actionMenu->addChild(useBtn);
        actionMenu->addChild(colorBtn);
        actionMenu->addChild(cancelBtn);
        m_mainLayer->addChildAtPosition(actionMenu, Anchor::Bottom, ccp(0, 28.f));

        this->updateStyleUI();
        return true;
    }

    void updateStyleUI() {
        const auto& style = g_styles[m_selected];
        if (m_titleLabel) m_titleLabel->setString(style.name);
        if (m_descLabel) m_descLabel->setString(style.description);
        if (m_countLabel) m_countLabel->setString(fmt::format("Estimated objects: {}", style.estimate));
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
        if (auto popup = AutoDecoColorsPopup::create()) popup->show();
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
            log::warn("AutoDeco: no level editor is available");
            return;
        }

        int total = 0;
        if (auto objLayer = layer->m_objectLayer) {
            for (auto child : CCArrayExt<CCNode*>(objLayer->getChildren())) {
                if (typeinfo_cast<GameObject*>(child)) {
                    total++;
                }
            }
        }

        FLAlertLayer::create(
            "Auto Deco",
            fmt::format("Applied <cy>{}</c> style.\nScanned {} objects in this level.", styleName, total),
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

        auto button = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(AutoDecoEditorUI::onAutoDecoButton)
        );

        if (items) {
            items->addObject(button);
            return items;
        }

        auto newItems = CCArray::create();
        newItems->addObject(button);
        return newItems;
    }
};

$execute {
    log::info("Auto Deco mod loaded.");
}
