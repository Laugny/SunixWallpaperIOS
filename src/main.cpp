#include <Geode/Bindings.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode;
using namespace cocos2d;

// Globale Zustände
static CCParallaxNode*       gBG      = nullptr;
static CCParticleSystemQuad* gEmitter = nullptr;
static bool                  gInLevelOrEditor = false;

// Setting-Abfrage für Partikel (default: true)
static inline bool particlesEnabled() {
    bool enabled = true;
    try { enabled = Mod::get()->getSettingValue<bool>("particles"); } catch (...) {}
    return enabled;
}

// Hintergrund + Partikel in Szene einfügen
static void buildOrAttachBackground(CCLayer* parent) {
    if (!parent) return;

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Parallax-Node einmalig erstellen
    if (!gBG) {
        gBG = CCParallaxNode::create();
        gBG->setAnchorPoint({0, 0});
        gBG->setPosition({0, 0});

        // Sprites laden (über _spr Suffix); Fallbacks verhindern Absturz
        auto bg   = CCSprite::create("background.png"_spr);
        auto tree = CCSprite::create("tree.png"_spr);
        auto sun  = CCSprite::create("sun.png"_spr);
        if (!bg)   bg   = CCSprite::create();
        if (!tree) tree = CCSprite::create();
        if (!sun)  sun  = CCSprite::create();

        // Hintergrund skalieren
        if (bg->getContentSize().width > 0 && bg->getContentSize().height > 0) {
            const float sx = winSize.width  / bg->getContentSize().width;
            const float sy = winSize.height / bg->getContentSize().height;
            bg->setScaleX(sx);
            bg->setScaleY(sy);
        }
        bg->setAnchorPoint({0,0});
        tree->setAnchorPoint({0.5f, 0.f});
        sun->setAnchorPoint({0.5f, 0.5f});

        // Parallax‑Kinder: Z‑Order, Ratio und Offset
        gBG->addChild(bg,   -3, ccp(0.f,    0.f),    ccp(0.f, 0.f));
        gBG->addChild(tree, -2, ccp(0.15f,  0.f),    ccp(winSize.width * 0.15f, 0.f));
        gBG->addChild(sun,  -1, ccp(0.05f,  0.05f),  ccp(winSize.width * 0.85f, winSize.height * 0.8f));
    }

    // Parallax‑Node bei Bedarf an den Layer anhängen
    if (!gBG->getParent()) {
        parent->addChild(gBG, -50);
    }

    // Partikel nur in Menüs anzeigen
    const bool showParticles = particlesEnabled() && !gInLevelOrEditor;

    if (showParticles) {
        if (!gEmitter) {
            // Partikelsystem aus PLIST laden; Datei muss in mod.json unter "resources.files" stehen
            gEmitter = CCParticleSystemQuad::create("particleEffect.plist");
            if (gEmitter) {
                gEmitter->setPosition(ccp(winSize.width * 0.5f, 0.f));
                gEmitter->setPositionType(kCCPositionTypeGrouped);
                gEmitter->setAutoRemoveOnFinish(false);
                // Als Kind von gBG mit Null‑Parallax an der gewünschten Position anhängen
                gBG->addChild(gEmitter, 1, ccp(0.f, 0.f), gEmitter->getPosition());
            }
        }
        if (gEmitter) {
            gEmitter->resumeSchedulerAndActions();
            gEmitter->setVisible(true);
        }
    } else {
        if (gEmitter) {
            gEmitter->pauseSchedulerAndActions();
            gEmitter->setVisible(false);
        }
    }
}

// Szenenwechsel und Menü initialisieren
class $modify(hookDirector, CCDirector) {
    void willSwitchToScene(CCScene* scene) {
        CCDirector::willSwitchToScene(scene);
        if (!scene) return;
        // Gameplay/Editor erkennen
        gInLevelOrEditor =
            scene->getChildByType<PlayLayer>(0) != nullptr ||
            scene->getChildByType<LevelEditorLayer>(0) != nullptr;
        // Menühintergrund anlegen
        if (auto layer = scene->getChildByType<CCLayer>(0)) {
            buildOrAttachBackground(layer);
        }
    }
    void startAnimation() {
        CCDirector::startAnimation();
        if (auto running = this->getRunningScene()) {
            if (auto layer = running->getChildByType<CCLayer>(0)) {
                buildOrAttachBackground(layer);
            }
        }
    }
};

class $modify(hookMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        if (auto scene = CCDirector::sharedDirector()->getRunningScene()) {
            if (auto layer = scene->getChildByType<CCLayer>(0)) {
                buildOrAttachBackground(layer);
            }
        }
        return true;
    }
};
