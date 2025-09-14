#include <Geode/Bindings.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode;
using namespace cocos2d;

// -------------------- Globale Zustände --------------------
static CCParallaxNode*       gBG      = nullptr;
static CCParticleSystemQuad* gEmitter = nullptr;
static bool                  gInLevelOrEditor = false;

// Prüft das Setting „particles“ aus mod.json (default: true)
static inline bool particlesEnabled() {
    bool enabled = true;
    try { enabled = Mod::get()->getSettingValue<bool>("particles"); } catch (...) {}
    return enabled;
}

// -------------------- Hintergrund aufbauen --------------------
static void buildOrAttachBackground(CCLayer* parent) {
    if (!parent) return;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (!gBG) {
        // Parallax-Knoten erstellen
        gBG = CCParallaxNode::create();
        gBG->setAnchorPoint({0, 0});
        gBG->setPosition({0, 0});

        // Sprites laden; Dateien müssen in mod.json unter "resources.sprites" gelistet sein
        auto bg   = CCSprite::create("background.png"_spr);
        auto tree = CCSprite::create("tree.png"_spr);
        auto sun  = CCSprite::create("sun.png"_spr);

        // Fallbacks, falls ein Sprite fehlt
        if (!bg)   bg   = CCSprite::create();
        if (!tree) tree = CCSprite::create();
        if (!sun)  sun  = CCSprite::create();

        // Hintergrund skaliert auf volle Fenstergröße
        if (bg->getContentSize().width > 0 && bg->getContentSize().height > 0) {
            const float sx = winSize.width  / bg->getContentSize().width;
            const float sy = winSize.height / bg->getContentSize().height;
            bg->setScaleX(sx);
            bg->setScaleY(sy);
        }
        bg->setAnchorPoint({0, 0});
        tree->setAnchorPoint({0.5f, 0.f});
        sun->setAnchorPoint({0.5f, 0.5f});

        // Startpositionen und Parallax-Faktoren
        gBG->addChild(bg,   -3, {0.f,   0.f  }, {0, 0});
        gBG->addChild(tree, -2, {0.15f, 0.f  }, {winSize.width * 0.15f, 0.f});
        gBG->addChild(sun,  -1, {0.05f, 0.05f}, {winSize.width * 0.85f, winSize.height * 0.8f});
    }

    // Nur einmal anhängen
    if (!gBG->getParent()) {
        parent->addChild(gBG, -50);
    }

    // Partikel nur im Menü anzeigen, niemals im Level/Editor
    const bool showParticles = particlesEnabled() && !gInLevelOrEditor;

    if (showParticles) {
        if (!gEmitter) {
            // PLIST muss in mod.json unter "resources.files" eingetragen sein
            gEmitter = CCParticleSystemQuad::create("particleEffect.plist");
            if (gEmitter) {
                gEmitter->setPosition({winSize.width * 0.5f, 0.f});
                gEmitter->setPositionType(kCCPositionTypeGrouped);
                gEmitter->setAutoRemoveOnFinish(false);
                gBG->addChild(gEmitter, 1);
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

// -------------------- Hooks --------------------

// Erkennen von Szenewechseln (Levels/Editor vs. Menüs)
class $modify(hookDirector, CCDirector) {
    void willSwitchToScene(CCScene* scene) {
        CCDirector::willSwitchToScene(scene);

        if (!scene) return;
        // Erkennen, ob es sich um Gameplay (PlayLayer) oder Editor handelt
        gInLevelOrEditor =
            scene->getChildByType<PlayLayer>(0) != nullptr ||
            scene->getChildByType<LevelEditorLayer>(0) != nullptr;

        // In Menüs: Hintergrund (und Partikel) anlegen
        if (auto layer = scene->getChildByType<CCLayer>(0)) {
            buildOrAttachBackground(layer);
        }
    }

    void startAnimation() {
        CCDirector::startAnimation();
        // Auch beim App‑Resume sicherstellen, dass der BG existiert
        if (auto* running = this->getRunningScene()) {
            if (auto layer = running->getChildByType<CCLayer>(0)) {
                buildOrAttachBackground(layer);
            }
        }
    }
};

// Beim Betreten des Hauptmenüs BG und Partikel initialisieren
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
