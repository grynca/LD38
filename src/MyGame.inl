#include "MyGame.h"

inline void MyGame::init() {
#ifndef WEB
    getModule<Window>().setVSync(false);
#endif
    initAssets();

    initEntityTypes<EntityTypes>();

    // update systems
    addUpdateSystem<WorldSystem>();
    addUpdateSystem<MovementSystem>();
    //addUpdateSystem<DeltaTransformSystem>();
    //addUpdateSystem<CollisionSystem>();
    // render systems
    //addRenderSystem<CollisionsDebugSystem>();
    addRenderSystem<RenderSystem>();
    player_id = createEntity<Player>(Player::init, asGameBase()).getIndex();
}

inline void MyGame::update() {
    callOnEntitySafe(player_id, Player::update, asGameBase());

//    udpateEntitySafe(hud_id, HUD::update);
//    udpateEntitySafe(gui_root_id, [](Entity& e, GameBase& g) {
//        e.get<CTransform>().move(Vec2(10, 0)*g.getTargetTicklen());
//    });
//
//    Events& events = getModule<Window>().getEvents();
//    bool alt_down = events.isKeyDown(SDL_SCANCODE_LALT)|events.isKeyDown(SDL_SCANCODE_RALT);
//    if (events.wasKeyPressed(SDL_SCANCODE_F4) && alt_down)
//        quit();
//    else if (events.wasKeyPressed(SDL_SCANCODE_RETURN) && alt_down)
//        getModule<Window>().toggleFullscreen();
//
//    static f32 camera_l_speed = (f32)100/60;
//    Vec2 camera_motion;
//    camera_motion.accX() -= (int)events.isKeyDown(SDL_SCANCODE_LEFT);
//    camera_motion.accX() += (int)events.isKeyDown(SDL_SCANCODE_RIGHT);
//    camera_motion.accY() -= (int)events.isKeyDown(SDL_SCANCODE_UP);
//    camera_motion.accY() += (int)events.isKeyDown(SDL_SCANCODE_DOWN);
//    if (camera_motion.getX() && camera_motion.getY()) {
//        static f32 norm = 1.f/(f32)sqrt(2);
//        camera_motion *= norm;
//    }
//    camera_motion *= camera_l_speed;
//    getModule<Window>().getViewPort().moveRelative(camera_motion);
}

inline void MyGame::render(f32 dt) {
    callOnEntitySafe(player_id, Player::render, asGameBase(), dt);

    // attach camera on player
    const Transform& player_tr = getEntity(player_id).get<CTransform>().get();

    ViewPort& vp = getWindow().getViewPort();

    vp.setRotation(physics::motAngleWithTime(vp.getRotation(), player_tr.getRotation(), 0.5f, dt));
    vp.setPosition(physics::motPosWithTime(vp.getPosition(), player_tr.getPosition(), 0.5f, dt));
}

inline void MyGame::tick() {

}

inline void MyGame::initAssets() {
    AssetsManager& am = getWindow().getAssets();
    assets.images_pack_id = am.loadSpritesDirToTexture("data/sprites", GL_RGBA, 2048, tidSprites);
    if (!assets.images_pack_id.isValid()) {
        throw Exception("Error Loading sprites.");
    }
    assets.fonts_pack_id = am.loadFontToTexture("data/fonts/arial.ttf", {{10, 50}}, 2048, tidFonts);
    if (!assets.fonts_pack_id.isValid()) {
        throw Exception("Error Loading fonts.");
    }

//    am.getImagesPacks().getItem(assets.images_pack_id).getPackImage()->saveToPNG("data/sprites_pack.png");
//    am.getFontsPacks().getItem(assets.fonts_pack_id).getPackImage()->saveToPNG("data/fonts_pack.png");
}