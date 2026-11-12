#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/ui/Popup.hpp>
#include <array>
#include <vector>

using namespace geode::prelude;

struct DecoStyle { const char* name; const char* desc; int d, o, g; ccColor3B bg, blk, out, acc, glow; };

static const std::array<DecoStyle, 20> g_styles = {{
    {"Nightmare","Dark walls lit blood red & chains.",   44,211,503,ccc3(20,2,5),   ccc3(10,10,10), ccc3(255,30,56),  ccc3(153,0,26),   ccc3(255,0,34)},
    {"Lagoon",   "Cool water tones & floating neon.",    18,211,503,ccc3(5,62,184), ccc3(6,30,100), ccc3(100,237,255),ccc3(142,205,223),ccc3(60,160,209)},
    {"Reactor",  "Industrial core with harsh lights.",   18,211,503,ccc3(18,24,16), ccc3(34,42,32), ccc3(154,255,62), ccc3(88,184,40),  ccc3(120,255,32)},
    {"Solar",    "Warm sunlight & bright orange glow.",  18,211,503,ccc3(59,22,4),  ccc3(36,13,2),  ccc3(255,168,40), ccc3(255,110,20), ccc3(255,196,77)},
    {"Circuit",  "Clean tech shapes & electric lines.",  44,211,503,ccc3(4,30,36),  ccc3(8,18,24),  ccc3(0,245,212),  ccc3(0,187,249),  ccc3(56,255,229)},
    {"Crimson",  "Dark red plating & danger geometry.",  44,211,503,ccc3(38,4,8),   ccc3(24,4,6),   ccc3(255,59,92),  ccc3(201,24,74),  ccc3(255,77,109)},
    {"Arcade",   "Retro playful blocks & bright neon.",  18,211,503,ccc3(26,8,54),  ccc3(16,5,36),  ccc3(255,0,229),  ccc3(0,240,255),  ccc3(255,230,0)},
    {"Nova",     "Deep space blue with soft star glow.", 18,211,503,ccc3(9,12,46),  ccc3(5,7,28),   ccc3(123,140,255),ccc3(184,192,255),ccc3(94,114,235)},
    {"Glass",    "Translucent blocks & sharp shine.",    18,211,503,ccc3(22,40,54), ccc3(36,59,77), ccc3(224,251,252),ccc3(152,193,217),ccc3(255,255,255)},
    {"Obsidian", "Dark stone & high-contrast violet.",   44,211,503,ccc3(14,14,18), ccc3(24,24,32), ccc3(160,108,213),ccc3(98,71,170),  ccc3(193,158,224)},
    {"Echo",     "Mechanical pulses & emerald glow.",    18,211,503,ccc3(13,31,34), ccc3(8,20,22),  ccc3(82,183,136), ccc3(116,198,157),ccc3(149,213,178)},
    {"Prism",    "Color-split outlines & layered art.",  18,211,503,ccc3(27,17,44), ccc3(18,11,30), ccc3(255,112,166),ccc3(112,214,255),ccc3(255,151,112)},
    {"Pulse",    "Fast rhythmic patterns & neon pink.",  44,211,503,ccc3(36,7,46),  ccc3(22,4,28),  ccc3(247,37,133), ccc3(181,23,158), ccc3(76,201,240)},
    {"Bloom",    "Gentle pastel glow & smooth shapes.",  18,211,503,ccc3(43,18,36), ccc3(28,11,23), ccc3(255,175,204),ccc3(255,200,221),ccc3(189,224,254)},
    {"Vanta",    "Pure black contrast & crisp white.",   44,211,503,ccc3(3,3,3),    ccc3(8,8,8),    ccc3(255,255,255),ccc3(176,176,176),ccc3(232,232,232)},
    {"Volt",     "Electric cyan details & bold edges.",  44,211,503,ccc3(3,20,48),  ccc3(2,11,28),  ccc3(0,180,216),  ccc3(72,202,228), ccc3(144,224,239)},
    {"Lava",     "Hot magma layers & fiery outlines.",   44,211,503,ccc3(41,8,0),   ccc3(26,5,0),   ccc3(255,84,0),   ccc3(255,133,0),  ccc3(255,189,0)},
    {"Abyss",    "Deep ocean darks & cobalt glow.",      18,211,503,ccc3(2,10,24),  ccc3(4,18,38),  ccc3(29,78,216),  ccc3(59,130,246), ccc3(96,165,250)},
    {"Dawn",     "Warm sunrise gradients & gold glow.",  18,211,503,ccc3(51,21,38), ccc3(33,13,24), ccc3(255,158,125),ccc3(255,193,140),ccc3(255,229,168)},
    {"Toxic",    "Acid green energy & hostile neon.",    44,211,503,ccc3(10,31,8),  ccc3(6,18,5),   ccc3(56,176,0),   ccc3(112,224,0),  ccc3(158,240,26)}
}};

static int g_idx = 0, g_mode = 0;
static std::array<ccColor3B, 5> g_pal = {g_styles[0].bg, g_styles[0].blk, g_styles[0].out, g_styles[0].acc, g_styles[0].glow};

static CCLayerColor* box(ccColor3B c, float w, float h, CCPoint p, GLubyte a = 255) {
    auto b = CCLayerColor::create(ccc4(c.r, c.g, c.b, a), w, h);
    b->ignoreAnchorPointForPosition(false); b->setAnchorPoint({0.5f, 0.5f}); b->setPosition(p); return b;
}

static void setCh(LevelEditorLayer* ed, int ch, ccColor3B c, bool bl = false) {
    if (ed && ed->m_levelSettings && ed->m_levelSettings->m_effectManager)
        if (auto a = ed->m_levelSettings->m_effectManager->getColorAction(ch)) { a->m_fromColor = a->m_toColor = a->m_color = c; a->m_blending = bl; }
}

static void decorateAll(EditorUI* ui) {
    if (!ui || !ui->m_editorLayer || !ui->m_editorLayer->m_objects) return;
    auto ed = ui->m_editorLayer; auto const& st = g_styles[g_idx];
    setCh(ed,1000,g_pal[0]); setCh(ed,1001,g_pal[1]); setCh(ed,1,g_pal[1]); setCh(ed,2,g_pal[2],true); setCh(ed,3,g_pal[3]); setCh(ed,4,g_pal[4],true);
    std::vector<GameObject*> lay;
    for (auto* o : CCArrayExt<GameObject*>(ed->m_objects))
        if (o && o->m_objectType != GameObjectType::Decoration && o->m_objectType != GameObjectType::Modifier && o->m_objectType != GameObjectType::Special) lay.push_back(o);
    if (lay.empty()) return Notification::create("No layout found!", NotificationIcon::Warning)->show();
    auto add = CCArray::create(); int k = 0;
    for (auto* o : lay) {
        auto p = o->getPosition(); float r = o->getRotation(), s = o->getScale();
        bool haz = (o->m_objectType == GameObjectType::Hazard || o->m_objectType == GameObjectType::AnimatedHazard);
        if (o->m_baseColor) o->m_baseColor->m_colorID = haz ? 2 : 1;
        if (o->m_detailColor) o->m_detailColor->m_colorID = haz ? 4 : 2;
        if (!haz) {
            if (auto d = ed->createObject(st.d, p, false)) { d->setRotation(r); d->setScale(s); d->m_zLayer = ZLayer::B1; d->m_zOrder = 2; if (d->m_baseColor) d->m_baseColor->m_colorID = 3; add->addObject(d); }
            if (auto out = ed->createObject(st.o, p, false)) { out->setRotation(r); out->setScale(s); out->m_zLayer = ZLayer::T1; out->m_zOrder = 5; if (out->m_baseColor) out->m_baseColor->m_colorID = 2; add->addObject(out); }
            if (g_mode == 0 && (k % 3 == 0))
                for (int j = 1; j <= 3; ++j)
                    if (auto pil = ed->createObject(st.d, p - ccp(0, 30.f * j * s), false)) { pil->setScale(s); pil->m_zLayer = ZLayer::B3; pil->m_zOrder = -5; if (pil->m_baseColor) pil->m_baseColor->m_colorID = 1; add->addObject(pil); }
        }
        if (g_mode != 1)
            if (auto gl = ed->createObject(st.g, p, false)) { gl->setRotation(r); gl->m_zLayer = ZLayer::T2; gl->m_zOrder = 10; gl->setScale(s * (g_mode == 2 ? 1.4f : 1.15f)); if (gl->m_baseColor) gl->m_baseColor->m_colorID = 4; add->addObject(gl); }
        ++k;
    }
    if (add->count() > 0) { ui->selectObjects(add, true); ui->updateButtons(); }
    ed->updateEditorMode();
    Notification::create(fmt::format("Decorated {} blocks ({})!", lay.size(), st.name), NotificationIcon::Success)->show();
}

class ColorsPopup : public geode::Popup {
    std::array<CCLayerColor*, 5> m_b{}; std::array<CCLabelBMFont*, 5> m_h{};
    bool init() override {
        if (!Popup::init(420.f, 285.f)) return false;
        m_mainLayer->addChild(box(ccc3(24,25,44), 420.f, 285.f, {210.f, 142.5f}));
        this->setTitle("Colours", "goldFont.fnt", 0.7f, 18.f);
        static const char* nm[5] = {"Background","Blocks","Outline","Accent","Glow"};
        auto m = CCMenu::create(); m->setPosition({0,0}); m_mainLayer->addChild(m);
        for (int i = 0; i < 5; ++i) {
            float y = 215.f - i * 32.f; auto c = g_pal[i];
            auto l = CCLabelBMFont::create(nm[i], "bigFont.fnt"); l->setScale(0.34f); l->setAnchorPoint({0,0.5f}); l->setPosition({35.f, y}); m_mainLayer->addChild(l);
            m_b[i] = CCLayerColor::create(ccc4(c.r,c.g,c.b,255), 105.f, 20.f);
            auto bt = CCMenuItemSpriteExtra::create(m_b[i], this, menu_selector(ColorsPopup::onTap)); bt->setTag(i); bt->setPosition({235.f, y}); m->addChild(bt);
            m_h[i] = CCLabelBMFont::create(fmt::format("{:02X}{:02X}{:02X}", c.r,c.g,c.b).c_str(), "chatFont.fnt"); m_h[i]->setScale(0.75f); m_h[i]->setPosition({345.f, y}); m_mainLayer->addChild(m_h[i]);
        }
        auto dn = CCMenuItemSpriteExtra::create(box(ccc3(110,108,245), 130.f, 26.f, {0,0}), this, menu_selector(ColorsPopup::onDn));
        auto dl = CCLabelBMFont::create("Done", "bigFont.fnt"); dl->setScale(0.34f); dl->setPosition({65.f, 13.f}); dn->addChild(dl); dn->setPosition({210.f, 26.f}); m->addChild(dn);
        return true;
    }
    void onTap(CCObject* s) { int i = s->getTag(); auto& c = g_pal[i]; c = ccc3((c.r+32)%255,(c.g+22)%255,(c.b+42)%255); m_b[i]->setColor(c); m_h[i]->setString(fmt::format("{:02X}{:02X}{:02X}",c.r,c.g,c.b).c_str()); }
    void onDn(CCObject*) { this->onClose(nullptr); }
public:
    static ColorsPopup* create() { auto r = new ColorsPopup(); if (r->init()) { r->autorelease(); return r; } delete r; return nullptr; }
};

class AutoDecoPopup : public geode::Popup {
    EditorUI* m_ui = nullptr; CCLabelBMFont *m_t = nullptr, *m_d = nullptr, *m_s = nullptr, *m_i = nullptr;
    CCLayerColor *pBg = nullptr, *pGl = nullptr, *pB1 = nullptr, *pB2 = nullptr, *pO1 = nullptr, *pO2 = nullptr, *pAc = nullptr;
    void sync() {
        auto const& s = g_styles[g_idx]; g_pal = {s.bg, s.blk, s.out, s.acc, s.glow};
        m_t->setString(s.name); m_d->setString(s.desc); m_i->setString(fmt::format("{} / 20", g_idx + 1).c_str());
        pBg->setColor(g_pal[0]); pB1->setColor(g_pal[1]); pB2->setColor(g_pal[1]); pO1->setColor(g_pal[2]); pO2->setColor(g_pal[2]); pAc->setColor(g_pal[3]); pGl->setColor(g_pal[4]);
    }
    bool init(EditorUI* ui) {
        if (!Popup::init(490.f, 300.f)) return false;
        m_ui = ui; m_mainLayer->addChild(box(ccc3(24,25,44), 490.f, 300.f, {245.f, 150.f}));
        auto hd = CCLabelBMFont::create("Auto Deco", "bigFont.fnt"); hd->setScale(0.55f); hd->setAnchorPoint({0,0.5f}); hd->setPosition({22.f, 276.f}); m_mainLayer->addChild(hd);
        pBg = box(g_pal[0],205.f,125.f,{126.f,180.f}); pGl = box(g_pal[4],185.f,42.f,{126.f,150.f},90);
        pB1 = box(g_pal[1],205.f,24.f,{126.f,130.f}); pO1 = box(g_pal[2],205.f,3.f,{126.f,142.f});
pB2 = box(g_pal[1],54.f,32.f,{174.f,178.f});  pO2 = box(g_pal[2],54.f,3.f,{174.f,194.f}); pAc = box(g_pal[3],40.f,14.f,{174.f,178.f});
m_mainLayer->addChild(pBg); m_mainLayer->addChild(pGl); m_mainLayer->addChild(pB1); m_mainLayer->addChild(pO1); m_mainLayer->addChild(pB2); m_mainLayer->addChild(pO2); m_mainLayer->addChild(pAc);
m_t = CCLabelBMFont::create("", "bigFont.fnt"); m_t->setScale(0.5f); m_t->setAnchorPoint({0,0.5f}); m_t->setPosition({246.f, 226.f}); m_mainLayer->addChild(m_t);
m_d = CCLabelBMFont::create("", "chatFont.fnt"); m_d->setScale(0.55f); m_d->setColor(ccc3(175,185,210)); m_d->setAnchorPoint({0,1.f}); m_d->setPosition({246.f, 210.f}); m_d->setWidth(220.f); m_mainLayer->addChild(m_d);
auto m = CCMenu::create(); m->setPosition({0,0}); m_mainLayer->addChild(m);
auto pv = CCMenuItemSpriteExtra::create(ButtonSprite::create("<",24,0,0.45f,true,"bigFont.fnt","GJ_button_04.png",26.f),this,menu_selector(AutoDecoPopup::onPv)); pv->setPosition({268.f,142.f}); m->addChild(pv);
auto nx = CCMenuItemSpriteExtra::create(ButtonSprite::create(">",24,0,0.45f,true,"bigFont.fnt","GJ_button_04.png",26.f),this,menu_selector(AutoDecoPopup::onNx)); nx->setPosition({448.f,142.f}); m->addChild(nx);
m_i = CCLabelBMFont::create("", "bigFont.fnt"); m_i->setScale(0.34f); m_i->setPosition({358.f, 142.f}); m_mainLayer->addChild(m_i);
auto cb = CCMenuItemSpriteExtra::create(box(ccc3(38,41,68),140.f,40.f,{0,0}),this,menu_selector(AutoDecoPopup::onCl)); auto ct = CCLabelBMFont::create("Colours","bigFont.fnt"); ct->setScale(0.34f); ct->setPosition({70.f,20.f}); cb->addChild(ct); cb->setPosition({92.f,80.f}); m->addChild(cb);
auto sb = CCMenuItemSpriteExtra::create(box(ccc3(38,41,68),140.f,40.f,{0,0}),this,menu_selector(AutoDecoPopup::onSt)); auto st = CCLabelBMFont::create("Structure","bigFont.fnt"); st->setScale(0.32f); st->setPosition({70.f,25.f}); sb->addChild(st); m_s = CCLabelBMFont::create("pillars","chatFont.fnt"); m_s->setScale(0.55f); m_s->setPosition({70.f,12.f}); sb->addChild(m_s); sb->setPosition({245.f,80.f}); m->addChild(sb);
auto db = CCMenuItemSpriteExtra::create(box(ccc3(110,108,245),140.f,40.f,{0,0}),this,menu_selector(AutoDecoPopup::onGo)); auto dt = CCLabelBMFont::create("Decorate level","bigFont.fnt"); dt->setScale(0.31f); dt->setPosition({70.f,20.f}); db->addChild(dt); db->setPosition({398.f,80.f}); m->addChild(db);
sync(); return true;
}
void onPv(CCObject*) { g_idx = (g_idx + 19) % 20; sync(); }
void onNx(CCObject*) { g_idx = (g_idx + 1) % 20; sync(); }
void onCl(CCObject*) { if (auto p = ColorsPopup::create()) p->show(); }
void onSt(CCObject*) { g_mode = (g_mode + 1) % 3; static const char* md[3] = {"pillars","blocks only","heavy glow"}; m_s->setString(md[g_mode]); }
void onGo(CCObject*) { auto u = m_ui; this->onClose(nullptr); decorateAll(u); }
public:
static AutoDecoPopup* create(EditorUI* u) { auto r = new AutoDecoPopup(); if (r->init(u)) { r->autorelease(); return r; } delete r; return nullptr; }
};
class FloatingZBtn : public CCLayer {
EditorUI* m_ui = nullptr; CCSprite* m_s = nullptr; CCPoint m_ts, m_ns; bool m_dr = false;
bool init(EditorUI* ui) {
if (!CCLayer::init()) return false;
m_ui = ui; setTouchEnabled(true); setTouchMode(kCCTouchesOneByOne); setTouchPriority(-500);
m_s = CCSprite::createWithSpriteFrameName("GJ_button_04.png"); m_s->setScale(0.85f);
auto z = CCLabelBMFont::create("Z", "goldFont.fnt"); z->setScale(0.95f); z->setPosition(m_s->getContentSize()/2.f + ccp(0,1.f)); m_s->addChild(z);
setContentSize(m_s->getScaledContentSize()); m_s->setPosition(getContentSize()/2.f); addChild(m_s);
auto ws = CCDirector::sharedDirector()->getWinSize(); setPosition({ws.width - 145.f, ws.height - 55.f}); return true;
}
bool ccTouchBegan(CCTouch* t, CCEvent*) override { if (!isVisible() || !CCRect(0,0,getContentSize().width,getContentSize().height).containsPoint(convertToNodeSpace(t->getLocation()))) return false; m_ts = t->getLocation(); m_ns = getPosition(); m_dr = false; m_s->setScale(0.78f); return true; }
void ccTouchMoved(CCTouch* t, CCEvent*) override { auto d = t->getLocation() - m_ts; if (d.getLength() > 6.f) m_dr = true; if (m_dr) setPosition(m_ns + d); }
void ccTouchEnded(CCTouch*, CCEvent*) override { m_s->setScale(0.85f); if (!m_dr) if (auto p = AutoDecoPopup::create(m_ui)) p->show(); }
public:
static FloatingZBtn* create(EditorUI* u) { auto r = new FloatingZBtn(); if (r->init(u)) { r->autorelease(); return r; } delete r; return nullptr; }
};
class $modify(AutoDecoEditorUI, EditorUI) {
bool init(LevelEditorLayer* ed) {
if (!EditorUI::init(ed)) return false;
if (auto b = FloatingZBtn::create(this)) { b->setID("auto-deco-z-btn"_spr); addChild(b, 100); }
return true;
}
};

<FollowUp>
Tap the **Copy** icon on this single compact block, paste it into **`src/main.cpp`**, commit your changes, and let me know when **Actions** turns **green**!
</FollowUp>
