#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/ui/Popup.hpp>
#include <array>
#include <vector>

using namespace geode::prelude;

struct DecoStyle {
    const char* name;
    const char* desc;
    int detID, outID, glowID;
    ccColor3B bg, blk, out, acc, glow;
};

static const std::array<DecoStyle, 20> g_styles = {{
    {"Nightmare", "Black walls lit blood red, chains, brick in the dark.",    44, 211, 503, ccc3(20,2,5),     ccc3(10,10,10),   ccc3(255,30,56),   ccc3(153,0,26),    ccc3(255,0,34)},
    {"Lagoon",    "Cool water tones and calm floating neon details.",         18, 211, 503, ccc3(5,62,184),   ccc3(6,30,100),   ccc3(100,237,255), ccc3(142,205,223), ccc3(60,160,209)},
    {"Reactor",   "Industrial power core with glowing systems and lights.",   18, 211, 503, ccc3(18,24,16),   ccc3(34,42,32),   ccc3(154,255,62),  ccc3(88,184,40),   ccc3(120,255,32)},
    {"Solar",     "Warm sunlight, glowing metal, and bright orange energy.",  18, 211, 503, ccc3(59,22,4),    ccc3(36,13,2),    ccc3(255,168,40),  ccc3(255,110,20),  ccc3(255,196,77)},
    {"Circuit",   "Clean tech shapes and electric background patterns.",      44, 211, 503, ccc3(4,30,36),    ccc3(8,18,24),    ccc3(0,245,212),   ccc3(0,187,249),   ccc3(56,255,229)},
    {"Crimson",   "Dark red plating with danger-type glowing geometry.",      44, 211, 503, ccc3(38,4,8),     ccc3(24,4,6),     ccc3(255,59,92),   ccc3(201,24,74),   ccc3(255,77,109)},
    {"Arcade",    "Retro style with playful block layouts and bright neon.",  18, 211, 503, ccc3(26,8,54),    ccc3(16,5,36),    ccc3(255,0,229),   ccc3(0,240,255),   ccc3(255,230,0)},
    {"Nova",      "Deep blue with star-like fixtures and soft glow accents.", 18, 211, 503, ccc3(9,12,46),    ccc3(5,7,28),     ccc3(123,140,255), ccc3(184,192,255), ccc3(94,114,235)},
    {"Glass",     "Translucent geometry with sharp edges and subtle shine.",  18, 211, 503, ccc3(22,40,54),   ccc3(36,59,77),   ccc3(224,251,252), ccc3(152,193,217), ccc3(255,255,255)},
    {"Obsidian",  "Stone and darkness balancing around high-contrast violet.", 44, 211, 503, ccc3(14,14,18),   ccc3(24,24,32),   ccc3(160,108,213), ccc3(98,71,170),   ccc3(193,158,224)},
    {"Echo",      "Repeating mechanical patterns with soft energy pulses.",   18, 211, 503, ccc3(13,31,34),   ccc3(8,20,22),    ccc3(82,183,136),  ccc3(116,198,157), ccc3(149,213,178)},
    {"Prism",     "Color-split wave outlines and reflective layered shapes.", 18, 211, 503, ccc3(27,17,44),   ccc3(18,11,30),   ccc3(255,112,166), ccc3(112,214,255), ccc3(255,151,112)},
    {"Pulse",     "Fast rhythmic patterns and vibrant neon motion lines.",    44, 211, 503, ccc3(36,7,46),    ccc3(22,4,28),    ccc3(247,37,133),  ccc3(181,23,158),  ccc3(76,201,240)},
    {"Bloom",     "Gentle glowing shapes and smoother decorative spacing.",   18, 211, 503, ccc3(43,18,36),   ccc3(28,11,23),   ccc3(255,175,204), ccc3(255,200,221), ccc3(189,224,254)},
    {"Vanta",     "Pure black contrast with sharp white geometric accents.",  44, 211, 503, ccc3(3,3,3),      ccc3(8,8,8),      ccc3(255,255,255), ccc3(176,176,176), ccc3(232,232,232)},
    {"Volt",      "Electric blue details with bright static outlines.",       44, 211, 503, ccc3(3,20,48),    ccc3(2,11,28),    ccc3(0,180,216),   ccc3(72,202,228),  ccc3(144,224,239)},
    {"Lava",      "Hot magma palette with dangerous red-orange layers.",      44, 211, 503, ccc3(41,8,0),     ccc3(26,5,0),     ccc3(255,84,0),    ccc3(255,133,0),   ccc3(255,189,0)},
    {"Abyss",     "Deep ocean darkness with structured glowing silhouettes.", 18, 211, 503, ccc3(2,10,24),    ccc3(4,18,38),    ccc3(29,78,216),   ccc3(59,130,246),  ccc3(96,165,250)},
    {"Dawn",      "Warm sunrise colours with smooth atmospheric transitions.", 18, 211, 503, ccc3(51,21,38),   ccc3(33,13,24),   ccc3(255,158,125), ccc3(255,193,140), ccc3(255,229,168)},
    {"Toxic",     "Acid green energy accents and hostile neon output.",       44, 211, 503, ccc3(10,31,8),    ccc3(6,18,5),     ccc3(56,176,0),    ccc3(112,224,0),   ccc3(158,240,26)}
}};

static int g_idx = 0;
static int g_structMode = 0;
static std::array<ccColor3B, 5> g_pal = {g_styles[0].bg, g_styles[0].blk, g_styles[0].out, g_styles[0].acc, g_styles[0].glow};
static CCLayerColor* makeBox(ccColor3B c, float w, float h, CCPoint pos, GLubyte a = 255) {
    auto b = CCLayerColor::create(ccc4(c.r, c.g, c.b, a), w, h);
    b->ignoreAnchorPointForPosition(false);
    b->setAnchorPoint({0.5f, 0.5f});
    b->setPosition(pos);
    return b;
}

static void setCh(LevelEditorLayer* ed, int ch, ccColor3B c, bool blend = false) {
    if (!ed || !ed->m_levelSettings || !ed->m_levelSettings->m_effectManager) return;
    if (auto a = ed->m_levelSettings->m_effectManager->getColorAction(ch)) {
        a->m_fromColor = a->m_toColor = a->m_color = c;
        a->m_blending = blend;
    }
}

static void decorateAll(EditorUI* ui) {
    if (!ui || !ui->m_editorLayer || !ui->m_editorLayer->m_objects) return;
    auto ed = ui->m_editorLayer;
    auto const& st = g_styles[g_idx];
    setCh(ed, 1000, g_pal[0]); setCh(ed, 1001, g_pal[1]);
    setCh(ed, 1, g_pal[1]); setCh(ed, 2, g_pal[2], true);
    setCh(ed, 3, g_pal[3]); setCh(ed, 4, g_pal[4], true);

    std::vector<GameObject*> layout;
    for (auto* o : CCArrayExt<GameObject*>(ed->m_objects)) {
        if (o && o->m_objectType != GameObjectType::Decoration && o->m_objectType != GameObjectType::Modifier)
            layout.push_back(o);
    }
    if (layout.empty()) return Notification::create("No layout found!", NotificationIcon::Warning)->show();

    auto added = CCArray::create();
    for (auto* o : layout) {
        auto p = o->getPosition();
        if (o->m_baseColor) o->m_baseColor->m_colorID = 1;
        if (o->m_detailColor) o->m_detailColor->m_colorID = 2;

        if (auto d = ed->createObject(st.detID, p, false)) {
            d->m_zLayer = ZLayer::B1; d->m_zOrder = 2;
            if (d->m_baseColor) d->m_baseColor->m_colorID = 3;
            added->addObject(d);
        }
        if (auto out = ed->createObject(st.outID, p, false)) {
            out->m_zLayer = ZLayer::T1; out->m_zOrder = 5;
            if (out->m_baseColor) out->m_baseColor->m_colorID = 2;
            added->addObject(out);
        }
        if (g_structMode != 1) {
            if (auto gl = ed->createObject(st.glowID, p, false)) {
                gl->m_zLayer = ZLayer::T2; gl->m_zOrder = 10; gl->setScale(g_structMode == 2 ? 1.35f : 1.15f);
                if (gl->m_baseColor) gl->m_baseColor->m_colorID = 4;
                added->addObject(gl);
            }
        }
    }
    if (added->count() > 0) { ui->selectObjects(added, true); ui->updateButtons(); }
    ed->updateEditorMode();
    Notification::create(fmt::format("Decorated {} blocks ({})!", layout.size(), st.name), NotificationIcon::Success)->show();
}

class ColorsPopup : public geode::Popup<> {
protected:
    std::array<CCLayerColor*, 5> m_box{};
    std::array<CCLabelBMFont*, 5> m_hex{};
    bool setup() override {
        m_mainLayer->addChild(makeBox(ccc3(24,25,44), 430.f, 295.f, {215.f, 147.5f}));
        this->setTitle("Colours", "goldFont.fnt", 0.7f, 18.f);
        static const char* names[5] = {"Background", "Blocks", "Outline", "Accent", "Glow"};
        auto menu = CCMenu::create(); menu->setPosition({0,0}); m_mainLayer->addChild(menu);
        for (int i = 0; i < 5; ++i) {
            float y = 225.f - i * 33.f;
            m_mainLayer->addChild(makeBox(ccc3(36,38,64), 380.f, 26.f, {215.f, y}));
            auto lbl = CCLabelBMFont::create(names[i], "bigFont.fnt");
            lbl->setScale(0.35f); lbl->setAnchorPoint({0,0.5f}); lbl->setPosition({38.f, y});
            m_mainLayer->addChild(lbl);

            auto c = g_pal[i];
            m_box[i] = CCLayerColor::create(ccc4(c.r, c.g, c.b, 255), 110.f, 20.f);
            auto btn = CCMenuItemSpriteExtra::create(m_box[i], this, menu_selector(ColorsPopup::onTap));
            btn->setTag(i); btn->setPosition({240.f, y}); menu->addChild(btn);

            m_hex[i] = CCLabelBMFont::create(fmt::format("{:02X}{:02X}{:02X}", c.r, c.g, c.b).c_str(), "chatFont.fnt");
            m_hex[i]->setScale(0.75f); m_hex[i]->setPosition({350.f, y}); m_mainLayer->addChild(m_hex[i]);
        }
        auto cancel = CCMenuItemSpriteExtra::create(makeBox(ccc3(48,50,82), 130.f, 28.f, {0,0}), this, menu_selector(ColorsPopup::onCancel));
        auto cLbl = CCLabelBMFont::create("Cancel", "bigFont.fnt"); cLbl->setScale(0.35f); cLbl->setPosition({65.f, 14.f}); cancel->addChild(cLbl);
        cancel->setPosition({130.f, 26.f}); menu->addChild(cancel);

        auto done = CCMenuItemSpriteExtra::create(makeBox(ccc3(110,108,245), 130.f, 28.f, {0,0}), this, menu_selector(ColorsPopup::onDone));
        auto dLbl = CCLabelBMFont::create("Done", "bigFont.fnt"); dLbl->setScale(0.35f); dLbl->setPosition({65.f, 14.f}); done->addChild(dLbl);
        done->setPosition({300.f, 26.f}); menu->addChild(done);
        return true;
    }
    void onTap(CCObject* s) {
        int i = s->getTag(); auto& c = g_pal[i];
        c = ccc3((c.r + 32) % 255, (c.g + 22) % 255, (c.b + 42) % 255);
        m_box[i]->setColor(c);
        m_hex[i]->setString(fmt::format("{:02X}{:02X}{:02X}", c.r, c.g, c.b).c_str());
    }
    void onCancel(CCObject*) {
        auto const& s = g_styles[g_idx];
        g_pal = {s.bg, s.blk, s.out, s.acc, s.glow};
        this->onClose(nullptr);
    }
    void onDone(CCObject*) { this->onClose(nullptr); }
public:
    static ColorsPopup* create() {
        auto r = new ColorsPopup();
        if (r->initAnchored(430.f, 295.f, "GJ_square01.png")) { r->autorelease(); return r; }
        delete r; return nullptr;
    }
};
class AutoDecoPopup : public geode::Popup<EditorUI*> {
protected:
    EditorUI* m_ui = nullptr;
    CCLabelBMFont *m_title = nullptr, *m_desc = nullptr, *m_structSub = nullptr, *m_idxLbl = nullptr;
    CCLayerColor *m_pBg = nullptr, *m_pGlow = nullptr, *m_pBlk1 = nullptr, *m_pBlk2 = nullptr, *m_pOut1 = nullptr, *m_pOut2 = nullptr, *m_pAcc = nullptr;

    void sync() {
        auto const& s = g_styles[g_idx];
        g_pal = {s.bg, s.blk, s.out, s.acc, s.glow};
        m_title->setString(s.name);
        m_desc->setString(s.desc);
        m_idxLbl->setString(fmt::format("{} / 20", g_idx + 1).c_str());

        m_pBg->setColor(g_pal[0]);
        m_pBlk1->setColor(g_pal[1]);
        m_pBlk2->setColor(g_pal[1]);
        m_pOut1->setColor(g_pal[2]);
        m_pOut2->setColor(g_pal[2]);
        m_pAcc->setColor(g_pal[3]);
        m_pGlow->setColor(g_pal[4]);
    }

    bool setup(EditorUI* ui) override {
        m_ui = ui;
        m_mainLayer->addChild(makeBox(ccc3(24, 25, 44), 500.f, 310.f, {250.f, 155.f}));

        auto header = CCLabelBMFont::create("Auto Deco", "bigFont.fnt");
        header->setScale(0.55f); header->setAnchorPoint({0, 0.5f}); header->setPosition({22.f, 286.f});
        m_mainLayer->addChild(header);

        auto sub = CCLabelBMFont::create("Preview a style on the section in view", "chatFont.fnt");
        sub->setScale(0.6f); sub->setColor(ccc3(160, 170, 195)); sub->setAnchorPoint({0, 0.5f}); sub->setPosition({22.f, 268.f});
        m_mainLayer->addChild(sub);

        m_mainLayer->addChild(makeBox(ccc3(60, 64, 98), 214.f, 134.f, {129.f, 185.f}));
        m_pBg   = makeBox(g_pal[0], 210.f, 130.f, {129.f, 185.f});
        m_pGlow = makeBox(g_pal[4], 190.f, 45.f,  {129.f, 155.f}, 90);
        m_pBlk1 = makeBox(g_pal[1], 210.f, 26.f,  {129.f, 133.f});
        m_pOut1 = makeBox(g_pal[2], 210.f, 3.f,   {129.f, 146.f});
        m_pBlk2 = makeBox(g_pal[1], 56.f,  34.f,  {178.f, 182.f});
        m_pOut2 = makeBox(g_pal[2], 56.f,  3.f,   {178.f, 199.f});
        m_pAcc  = makeBox(g_pal[3], 42.f,  16.f,  {178.f, 182.f});
        m_mainLayer->addChild(m_pBg);
        m_mainLayer->addChild(m_pGlow);
        m_mainLayer->addChild(m_pBlk1);
        m_mainLayer->addChild(m_pOut1);
        m_mainLayer->addChild(m_pBlk2);
        m_mainLayer->addChild(m_pOut2);
        m_mainLayer->addChild(m_pAcc);

        if (auto spk = CCSprite::createWithSpriteFrameName("spike_01_001.png")) {
            spk->setScale(0.55f); spk->setPosition({82.f, 155.f}); spk->setColor(g_pal[2]);
            m_mainLayer->addChild(spk);
        }
        if (auto orb = CCSprite::createWithSpriteFrameName("ring_01_001.png")) {
            orb->setScale(0.55f); orb->setPosition({126.f, 195.f});
            m_mainLayer->addChild(orb);
        }

        m_title = CCLabelBMFont::create("", "bigFont.fnt");
        m_title->setScale(0.52f); m_title->setAnchorPoint({0, 0.5f}); m_title->setPosition({252.f, 232.f});
        m_mainLayer->addChild(m_title);

        m_desc = CCLabelBMFont::create("", "chatFont.fnt");
        m_desc->setScale(0.56f); m_desc->setColor(ccc3(175, 185, 210));
        m_desc->setAnchorPoint({0, 1.f}); m_desc->setPosition({252.f, 216.f});
        m_desc->setWidth(225.f);
        m_mainLayer->addChild(m_desc);

        auto m = CCMenu::create(); m->setPosition({0,0}); m_mainLayer->addChild(m);

        m_mainLayer->addChild(makeBox(ccc3(38, 41, 68), 226.f, 34.f, {365.f, 145.f}));
        auto prevBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("<", 24, 0, 0.45f, true, "bigFont.fnt", "GJ_button_04.png", 26.f), this, menu_selector(AutoDecoPopup::onPrev));
        auto nextBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create(">", 24, 0, 0.45f, true, "bigFont.fnt", "GJ_button_04.png", 26.f), this, menu_selector(AutoDecoPopup::onNext));
        prevBtn->setPosition({272.f, 145.f});
        nextBtn->setPosition({458.f, 145.f});
        m->addChild(prevBtn); m->addChild(nextBtn);

        m_idxLbl = CCLabelBMFont::create("", "bigFont.fnt");
        m_idxLbl->setScale(0.34f); m_idxLbl->setPosition({365.f, 145.f});
        m_mainLayer->addChild(m_idxLbl);

        auto colBtn = CCMenuItemSpriteExtra::create(makeBox(ccc3(38, 41, 68), 144.f, 42.f, {0,0}), this, menu_selector(AutoDecoPopup::onCol));
        auto cTxt = CCLabelBMFont::create("Colours", "bigFont.fnt"); cTxt->setScale(0.35f); cTxt->setPosition({72.f, 21.f}); colBtn->addChild(cTxt);
        colBtn->setPosition({94.f, 82.f}); m->addChild(colBtn);

        auto strBtn = CCMenuItemSpriteExtra::create(makeBox(ccc3(38, 41, 68), 144.f, 42.f, {0,0}), this, menu_selector(AutoDecoPopup::onStruct));
        auto sTxt = CCLabelBMFont::create("Structure layout", "bigFont.fnt"); sTxt->setScale(0.3f); sTxt->setPosition({72.f, 26.f}); strBtn->addChild(sTxt);
        m_structSub = CCLabelBMFont::create("pillars and walls", "chatFont.fnt"); m_structSub->setScale(0.55f); m_structSub->setColor(ccc3(160,170,195)); m_structSub->setPosition({72.f, 13.f}); strBtn->addChild(m_structSub);
        strBtn->setPosition({250.f, 82.f}); m->addChild(strBtn);

        auto decBtn = CCMenuItemSpriteExtra::create(makeBox(ccc3(110, 108, 245), 144.f, 42.f, {0,0}), this, menu_selector(AutoDecoPopup::onUse));
        auto dTxt = CCLabelBMFont::create("Decorate level", "bigFont.fnt"); dTxt->setScale(0.33f); dTxt->setPosition({72.f, 21.f}); decBtn->addChild(dTxt);
        decBtn->setPosition({406.f, 82.f}); m->addChild(decBtn);

        auto ver = CCLabelBMFont::create("Z Auto Deco 1.0", "chatFont.fnt");
        ver->setScale(0.55f); ver->setColor(ccc3(120, 130, 155)); ver->setPosition({435.f, 26.f});
        m_mainLayer->addChild(ver);

        sync();
        return true;
    }

    void onPrev(CCObject*) { g_idx = (g_idx + 19) % 20; sync(); }
    void onNext(CCObject*) { g_idx = (g_idx + 1) % 20; sync(); }
    void onCol(CCObject*)  { if (auto p = ColorsPopup::create()) p->show(); }
    void onStruct(CCObject*) {
        g_structMode = (g_structMode + 1) % 3;
        static const char* modes[3] = {"pillars and walls", "blocks only", "heavy glow"};
        m_structSub->setString(modes[g_structMode]);
    }
    void onUse(CCObject*) { auto u = m_ui; this->onClose(nullptr); decorateAll(u); }

public:
    static AutoDecoPopup* create(EditorUI* ui) {
        auto r = new AutoDecoPopup();
        if (r->initAnchored(500.f, 310.f, ui, "GJ_square01.png")) { r->autorelease(); return r; }
        delete r; return nullptr;
    }
};

class FloatingZBtn : public CCLayer {
protected:
    EditorUI* m_ui = nullptr;
    CCSprite* m_spr = nullptr;
    CCPoint m_tStart, m_nStart;
    bool m_drag = false;

    bool init(EditorUI* ui) {
        if (!CCLayer::init()) return false;
        m_ui = ui; setTouchEnabled(true); setTouchMode(kCCTouchesOneByOne); setTouchPriority(-500);
        m_spr = CCSprite::createWithSpriteFrameName("GJ_button_04.png");
        m_spr->setScale(0.85f);
        auto z = CCLabelBMFont::create("Z", "goldFont.fnt");
        z->setScale(0.95f); z->setPosition(m_spr->getContentSize() / 2.f + ccp(0, 1.f));
        m_spr->addChild(z);
        setContentSize(m_spr->getScaledContentSize()); m_spr->setPosition(getContentSize() / 2.f); addChild(m_spr);
        auto ws = CCDirector::sharedDirector()->getWinSize();
        setPosition({ws.width - 145.f, ws.height - 55.f});
        return true;
    }
    bool ccTouchBegan(CCTouch* t, CCEvent*) override {
        if (!isVisible() || !CCRect(0,0,getContentSize().width,getContentSize().height).containsPoint(convertToNodeSpace(t->getLocation()))) return false;
        m_tStart = t->getLocation(); m_nStart = getPosition(); m_drag = false; m_spr->setScale(0.78f); return true;
    }
    void ccTouchMoved(CCTouch* t, CCEvent*) override {
        auto d = t->getLocation() - m_tStart;
        if (d.getLength() > 6.f) m_drag = true;
        if (m_drag) setPosition(m_nStart + d);
    }
    void ccTouchEnded(CCTouch*, CCEvent*) override {
        m_spr->setScale(0.85f);
        if (!m_drag) if (auto p = AutoDecoPopup::create(m_ui)) p->show();
    }
public:
    static FloatingZBtn* create(EditorUI* ui) {
        auto r = new FloatingZBtn();
        if (r->init(ui)) { r->autorelease(); return r; }
        delete r; return nullptr;
    }
};

class $modify(AutoDecoEditorUI, EditorUI) {
    bool init(LevelEditorLayer* ed) {
        if (!EditorUI::init(ed)) return false;
        if (auto b = FloatingZBtn::create(this)) { b->setID("auto-deco-z-btn"_spr); addChild(b, 100); }
        return true;
    }
};
class AutoDecoPopup : public geode::Popup {
protected:
    EditorUI* m_ui = nullptr;
    CCLabelBMFont *m_title = nullptr, *m_desc = nullptr, *m_structSub = nullptr, *m_idxLbl = nullptr;
    CCLayerColor *m_pBg = nullptr, *m_pGlow = nullptr, *m_pBlk1 = nullptr, *m_pBlk2 = nullptr, *m_pOut1 = nullptr, *m_pOut2 = nullptr, *m_pAcc = nullptr;

    void sync() {
        auto const& s = g_styles[g_idx];
        g_pal = {s.bg, s.blk, s.out, s.acc, s.glow};
        m_title->setString(s.name);
        m_desc->setString(s.desc);
        m_idxLbl->setString(fmt::format("{} / 20", g_idx + 1).c_str());

        m_pBg->setColor(g_pal[0]);
        m_pBlk1->setColor(g_pal[1]);
        m_pBlk2->setColor(g_pal[1]);
        m_pOut1->setColor(g_pal[2]);
        m_pOut2->setColor(g_pal[2]);
        m_pAcc->setColor(g_pal[3]);
        m_pGlow->setColor(g_pal[4]);
    }

    bool init(EditorUI* ui) {
        if (!Popup::init(500.f, 310.f, "GJ_square01.png")) return false;
        m_ui = ui;
        m_mainLayer->addChild(makeBox(ccc3(24, 25, 44), 500.f, 310.f, {250.f, 155.f}));

        auto header = CCLabelBMFont::create("Auto Deco", "bigFont.fnt");
        header->setScale(0.55f); header->setAnchorPoint({0, 0.5f}); header->setPosition({22.f, 286.f});
        m_mainLayer->addChild(header);

        auto sub = CCLabelBMFont::create("Preview a style on the section in view", "chatFont.fnt");
        sub->setScale(0.6f); sub->setColor(ccc3(160, 170, 195)); sub->setAnchorPoint({0, 0.5f}); sub->setPosition({22.f, 268.f});
        m_mainLayer->addChild(sub);

        m_mainLayer->addChild(makeBox(ccc3(60, 64, 98), 214.f, 134.f, {129.f, 185.f}));
        m_pBg   = makeBox(g_pal[0], 210.f, 130.f, {129.f, 185.f});
        m_pGlow = makeBox(g_pal[4], 190.f, 45.f,  {129.f, 155.f}, 90);
        m_pBlk1 = makeBox(g_pal[1], 210.f, 26.f,  {129.f, 133.f});
        m_pOut1 = makeBox(g_pal[2], 210.f, 3.f,   {129.f, 146.f});
        m_pBlk2 = makeBox(g_pal[1], 56.f,  34.f,  {178.f, 182.f});
        m_pOut2 = makeBox(g_pal[2], 56.f,  3.f,   {178.f, 199.f});
        m_pAcc  = makeBox(g_pal[3], 42.f,  16.f,  {178.f, 182.f});
        m_mainLayer->addChild(m_pBg);
        m_mainLayer->addChild(m_pGlow);
        m_mainLayer->addChild(m_pBlk1);
        m_mainLayer->addChild(m_pOut1);
        m_mainLayer->addChild(m_pBlk2);
        m_mainLayer->addChild(m_pOut2);
        m_mainLayer->addChild(m_pAcc);

        if (auto spk = CCSprite::createWithSpriteFrameName("spike_01_001.png")) {
            spk->setScale(0.55f); spk->setPosition({82.f, 155.f}); spk->setColor(g_pal[2]);
            m_mainLayer->addChild(spk);
        }
        if (auto orb = CCSprite::createWithSpriteFrameName("ring_01_001.png")) {
            orb->setScale(0.55f); orb->setPosition({126.f, 195.f});
            m_mainLayer->addChild(orb);
        }

        m_title = CCLabelBMFont::create("", "bigFont.fnt");
        m_title->setScale(0.52f); m_title->setAnchorPoint({0, 0.5f}); m_title->setPosition({252.f, 232.f});
        m_mainLayer->addChild(m_title);

        m_desc = CCLabelBMFont::create("", "chatFont.fnt");
        m_desc->setScale(0.56f); m_desc->setColor(ccc3(175, 185, 210));
        m_desc->setAnchorPoint({0, 1.f}); m_desc->setPosition({252.f, 216.f});
        m_desc->setWidth(225.f);
        m_mainLayer->addChild(m_desc);

        auto m = CCMenu::create(); m->setPosition({0,0}); m_mainLayer->addChild(m);

        m_mainLayer->addChild(makeBox(ccc3(38, 41, 68), 226.f, 34.f, {365.f, 145.f}));
        auto prevBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("<", 24, 0, 0.45f, true, "bigFont.fnt", "GJ_button_04.png", 26.f), this, menu_selector(AutoDecoPopup::onPrev));
        auto nextBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create(">", 24, 0, 0.45f, true, "bigFont.fnt", "GJ_button_04.png", 26.f), this, menu_selector(AutoDecoPopup::onNext));
        prevBtn->setPosition({272.f, 145.f});
        nextBtn->setPosition({458.f, 145.f});
        m->addChild(prevBtn); m->addChild(nextBtn);

        m_idxLbl = CCLabelBMFont::create("", "bigFont.fnt");
        m_idxLbl->setScale(0.34f); m_idxLbl->setPosition({365.f, 145.f});
        m_mainLayer->addChild(m_idxLbl);

        auto colBtn = CCMenuItemSpriteExtra::create(makeBox(ccc3(38, 41, 68), 144.f, 42.f, {0,0}), this, menu_selector(AutoDecoPopup::onCol));
        auto cTxt = CCLabelBMFont::create("Colours", "bigFont.fnt"); cTxt->setScale(0.35f); cTxt->setPosition({72.f, 21.f}); colBtn->addChild(cTxt);
        colBtn->setPosition({94.f, 82.f}); m->addChild(colBtn);

        auto strBtn = CCMenuItemSpriteExtra::create(makeBox(ccc3(38, 41, 68), 144.f, 42.f, {0,0}), this, menu_selector(AutoDecoPopup::onStruct));
        auto sTxt = CCLabelBMFont::create("Structure layout", "bigFont.fnt"); sTxt->setScale(0.3f); sTxt->setPosition({72.f, 26.f}); strBtn->addChild(sTxt);
        m_structSub = CCLabelBMFont::create("pillars and walls", "chatFont.fnt"); m_structSub->setScale(0.55f); m_structSub->setColor(ccc3(160,170,195)); m_structSub->setPosition({72.f, 13.f}); strBtn->addChild(m_structSub);
        strBtn->setPosition({250.f, 82.f}); m->addChild(strBtn);

        auto decBtn = CCMenuItemSpriteExtra::create(makeBox(ccc3(110, 108, 245), 144.f, 42.f, {0,0}), this, menu_selector(AutoDecoPopup::onUse));
        auto dTxt = CCLabelBMFont::create("Decorate level", "bigFont.fnt"); dTxt->setScale(0.33f); dTxt->setPosition({72.f, 21.f}); decBtn->addChild(dTxt);
        decBtn->setPosition({406.f, 82.f}); m->addChild(decBtn);

        auto ver = CCLabelBMFont::create("Z Auto Deco 1.0", "chatFont.fnt");
        ver->setScale(0.55f); ver->setColor(ccc3(120, 130, 155)); ver->setPosition({435.f, 26.f});
        m_mainLayer->addChild(ver);

        sync();
        return true;
    }

    void onPrev(CCObject*) { g_idx = (g_idx + 19) % 20; sync(); }
    void onNext(CCObject*) { g_idx = (g_idx + 1) % 20; sync(); }
    void onCol(CCObject*)  { if (auto p = ColorsPopup::create()) p->show(); }
    void onStruct(CCObject*) {
        g_structMode = (g_structMode + 1) % 3;
        static const char* modes[3] = {"pillars and walls", "blocks only", "heavy glow"};
        m_structSub->setString(modes[g_structMode]);
    }
    void onUse(CCObject*) { auto u = m_ui; this->onClose(nullptr); decorateAll(u); }

public:
    static AutoDecoPopup* create(EditorUI* ui) {
        auto r = new AutoDecoPopup();
        if (r->init(ui)) { r->autorelease(); return r; }
        delete r; return nullptr;
    }
};

class FloatingZBtn : public CCLayer {
protected:
    EditorUI* m_ui = nullptr;
    CCSprite* m_spr = nullptr;
    CCPoint m_tStart, m_nStart;
    bool m_drag = false;

    bool init(EditorUI* ui) {
        if (!CCLayer::init()) return false;
        m_ui = ui; setTouchEnabled(true); setTouchMode(kCCTouchesOneByOne); setTouchPriority(-500);
        m_spr = CCSprite::createWithSpriteFrameName("GJ_button_04.png");
        m_spr->setScale(0.85f);
        auto z = CCLabelBMFont::create("Z", "goldFont.fnt");
        z->setScale(0.95f); z->setPosition(m_spr->getContentSize() / 2.f + ccp(0, 1.f));
        m_spr->addChild(z);
        setContentSize(m_spr->getScaledContentSize()); m_spr->setPosition(getContentSize() / 2.f); addChild(m_spr);
        auto ws = CCDirector::sharedDirector()->getWinSize();
        setPosition({ws.width - 145.f, ws.height - 55.f});
        return true;
    }
    bool ccTouchBegan(CCTouch* t, CCEvent*) override {
        if (!isVisible() || !CCRect(0,0,getContentSize().width,getContentSize().height).containsPoint(convertToNodeSpace(t->getLocation()))) return false;
        m_tStart = t->getLocation(); m_nStart = getPosition(); m_drag = false; m_spr->setScale(0.78f); return true;
    }
    void ccTouchMoved(CCTouch* t, CCEvent*) override {
        auto d = t->getLocation() - m_tStart;
        if (d.getLength() > 6.f) m_drag = true;
        if (m_drag) setPosition(m_nStart + d);
    }
    void ccTouchEnded(CCTouch*, CCEvent*) override {
        m_spr->setScale(0.85f);
        if (!m_drag) if (auto p = AutoDecoPopup::create(m_ui)) p->show();
    }
public:
    static FloatingZBtn* create(EditorUI* ui) {
        auto r = new FloatingZBtn();
        if (r->init(ui)) { r->autorelease(); return r; }
        delete r; return nullptr;
    }
};

class $modify(AutoDecoEditorUI, EditorUI) {
    bool init(LevelEditorLayer* ed) {
        if (!EditorUI::init(ed)) return false;
        if (auto b = FloatingZBtn::create(this)) { b->setID("auto-deco-z-btn"_spr); addChild(b, 100); }
        return true;
    }
};
