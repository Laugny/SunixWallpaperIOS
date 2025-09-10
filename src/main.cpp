#include <Geode/Bindings.hpp>
#include <Geode/Modify/CCDirector.hpp>
#include <Geode/Modify/PlayLayer.hpp>
#include <Geode/Modify/LevelEditorLayer.hpp>
#include <Geode/Modify/CCLayer.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Flag, ob wir gerade im Level oder Editor sind
static bool gInLevelOrEditor = false;

// Hintergrund-Node & Partikel-Emitter
static CCParallaxNodeExtra* gBG = nullptr;
static CCParticleSystemQuad* gEmitter = nullptr;

// Hilfsfunktionen zur Verwaltung von Hintergrund & Emitter
static void ensureBG(CCLayer* layer, int zOrder) {
    if (!gBG) {
        gBG = CCParallaxNodeExtra::create();
        layer->addChild(gBG, zOrder);
    }
}

static void replaceBG(CCLayer* layer) {
    layer->removeChild(gBG);
    gBG = nullptr;

    auto tree = CCSprite::create(Mod::get()->getResource("res/tree.png")->c_str());
    auto sun  = CCSprite::create(Mod::get()->getResource("res/sun.png")->c_str());

    auto win = CCDirector::sharedDirector()->getWinSize();
    tree->setScaleY(win.height / tree->getContentSize().height);
    tree->setPosition({ tree->getContentSize().width * 0.5f, 0.0f });
    sun->setPosition({ win.width / 2, win.height * 0.85f });

    ensureBG(layer, -10);
    gBG->addChild(sun, 1, {0.7f, 0.75f}, sun->getPosition());
    gBG->addChild(tree, 1, {1.0f, 0.25f}, tree->getPosition());
}

static void attachEmitterToMenuBG(CCLayer* layer) {
    if (gEmitter && gEmitter->getParent() != layer) {
        layer->addChild(gEmitter, 0);
    }
}

static void cleanupEmitter() {
    if (gEmitter) {
        gEmitter->removeFromParentAndCleanup(true);
        gEmitter = nullptr;
    }
}

class $modify(CCDirector) {
    void willSwitchToScene(CCScene* scene) {
        CCDirector::willSwitchToScene(scene);

        bool isPlay = getChildOfType<PlayLayer>(scene, 0) != nullptr;
        bool isEdit = getChildOfType<LevelEditorLayer>(scene, 0) != nullptr;
        gInLevelOrEditor = (isPlay || isEdit);

        log::info("[Sunix Wallpaper] gameplay particles {}",
                  gInLevelOrEditor ? "SUPPRESSED" : "ENABLED");

        if (auto layer = getChildOfType<CCLayer>(scene, 0)) {
            bool isSecret = dynamic_cast<SecretLayer*>(layer)
                         || dynamic_cast<SecretLayer2*>(layer)
                         || dynamic_cast<SecretLayer3*>(layer)
                         || dynamic_cast<SecretLayer4*>(layer)
                         || dynamic_cast<SecretRewardsLayer*>(layer)
                         || dynamic_cast<GauntletSelectLayer*>(layer);

            if (!isSecret) {
                if (!gInLevelOrEditor) {
                    replaceBG(layer);
                } else {
                    cleanupEmitter();
                }
            }
        }
    }

    void reshapeProjection(const CCSize& newSize) {
        CCDirector::reshapeProjection(newSize);
        if (!gEmitter) return;

        const auto win = CCDirector::sharedDirector()->getWinSize();
        gEmitter->setPosition(ccp(win.width * 0.5f, 0.f));
        gEmitter->setPosVar(ccp(win.width * 0.5f, 0.f));

        if (!gInLevelOrEditor) {
            if (auto scene = getRunningScene()) {
                if (auto layer = getChildOfType<CCLayer>(scene, 0)) {
                    ensureBG(layer, -10);
                    attachEmitterToMenuBG(layer);
                }
            }
        }

        gEmitter->resumeSchedulerAndActions();
        gEmitter->setVisible(!gInLevelOrEditor);
    }
};
