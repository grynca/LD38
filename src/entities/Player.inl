#include "Player.h"

inline void Player::Resources::init(GameBase &game) {
    parts[rBody].color = Colorf::Blue();
    parts[rBody].rest_pos = {0, 0};
    parts[rBody].size = {10, 50};
    parts[rBody].offset = {-0.5f, -0.5f};
    parts[rBody].layer = lPlayer;

    parts[rGun].color = Colorf::Brown();
    parts[rGun].rest_pos = {0, -parts[rBody].size.getY()*0.35f};
    parts[rGun].size = {35, 5};
    parts[rGun].offset = {0, -0.5f};
    parts[rGun].layer = lPlayerGun;

    parts[rHead].color = Colorf::Blue();
    parts[rHead].rest_pos = {0, -parts[rBody].size.getY()*0.5f};
    parts[rHead].size = {12, 12};
    parts[rHead].offset = {0, -0.5f};
    parts[rHead].layer = lPlayer;

    move_speed = 80;
    move_hop_height = parts[rBody].size.getY()*0.4f;
    move_hop_dt = 0.5f;

    jump_impulse = 50;

    machinegun_firerate = 30;
    machinegun_dispersion = 0.3f;
    machinegun_recoil = 10;
}


inline void Player::init(Entity &me, GameBase &game) {
//static
    static Resources &res = Resources::get();

    CWorldBodyData& cwb = me.getData<CWorldBodyData>();
    cwb.half_height = res.parts[rBody].size.getY()*0.5f;

    CRenderables rs = me.get<CRenderables>();
    rs.resize(rCount);

    for (u32 i=0; i<rCount; ++i) {
        rs.set<RectRenderable>(i)
                .setColor(res.parts[i].color)
                .setSize(res.parts[i].size)
                .setPosition(res.parts[i].rest_pos)
                .setGeomNormOffset(res.parts[i].offset)
                .accRenderTask()
                   ->setLayer(res.parts[i].layer);
    }

    CTransform ctr = me.get<CTransform>();
    ctr.setRotation(Angle::Pi_2);
    ctr.setPosition({0, -20});
}

inline void Player::update(Entity &me, GameBase &game) {
// static
    PROFILE_BLOCK("Player update");
    Resources &res = Resources::get();

    CWorldBodyData& cpd = me.getData<CWorldBodyData>();
    CTransform ctr = me.get<CTransform>();
    CRenderables crs = me.get<CRenderables>();
    CMovableData& cmd = me.getData<CMovableData>();

    Window& w = game.getModule<Window>();
    Events& e = w.getEvents();

    f32 accel = get1DMotionDir(e, SDL_SCANCODE_D, SDL_SCANCODE_A)*res.move_speed;
    if (accel) {
        auto head = crs.get<RectRenderable>(rHead);
        head.accSize().accX() = sgn(accel)*fabsf(head.accSize().accX());
    }

    if (cpd.state_flags[sfLanded]) {
        if (e.isKeyDown(SDL_SCANCODE_SPACE)) {
            cmd.speed.addLinearSpeed(-ctr.get().getBottomDir()*res.jump_impulse);
        }

        if (accel) {
            f32 curr_speed = cmd.speed.getLinearSpeed().calcDirProjection(ctr.get().getRightDir());
            if (sgn(curr_speed) != sgn(accel) || fabsf(curr_speed) < fabsf(accel))
                cmd.speed.addLinearSpeed(ctr.get().getRightDir()*accel);
        }
        else {
            cmd.speed.setLinearSpeed({0, cmd.speed.getLinearSpeed().getY()});
        }
    }

    if (e.isButtonDown(mbLeft)) {
        u32 shots_to_fire = 0;
        if (e.wasButtonClicked(mbLeft)) {
            shots_to_fire = 1;
        }
        else {
            f32 since_last_shot = game.getTimer().getElapsed() - cpd.last_shot_time;
            f32 time_between_shots = 1.0f/res.machinegun_firerate;
            while (since_last_shot > time_between_shots) {
                shots_to_fire++;
                since_last_shot -= time_between_shots;
            }
        }

        Dir2 gun_dir;
        Vec2 gun_tip_pos = getGunTip_(me, &gun_dir);
        Dir2 dispersion_vec = gun_dir.perpR();
        if (shots_to_fire > 0) {
            cpd.state_flags.set(sfShooting);
            cpd.last_shot_time = game.getTimer().getElapsed();

            for (u32 i=0; i<shots_to_fire; ++i) {
                Vec2 shot_dir = gun_dir + randFloat(-0.5f, 0.5f)*dispersion_vec*res.machinegun_dispersion;
                game.createEntity<Projectile>(Projectile::init, game, gun_tip_pos, shot_dir);
                cmd.speed.addLinearSpeed(-shot_dir*res.machinegun_recoil);
            }
        }
    }
    else if (e.wasButtonReleased(mbLeft)) {
        cpd.state_flags.reset(sfShooting);
    }
}

inline void Player::render(Entity &me, GameBase &game, f32 dt) {
// static
    PROFILE_BLOCK("Player render");
    CWorldBodyData& cwb = me.getData<CWorldBodyData>();

    Resources &res = Resources::get();
    renderGunAiming_(me, game);

    renderMotion_(me, game, dt);
}

inline void Player::onKilled(Entity &me, GameBase &game) {
// static
}

inline void Player::renderGunAiming_(Entity &me, GameBase &game) {
    CTransform ctr = me.get<CTransform>();
    CRenderables crs = me.get<CRenderables>();

    Resources &res = Resources::get();
    Window& window = game.getModule<Window>();
    Events& events = window.getEvents();

    RectRenderable gun = crs.get<RectRenderable>(rGun);
    Mat3 tr_m = ctr.get().calcMatrix();

    // calculate desired aim rotation
    Vec2 mouse_glob = window.getViewPort().viewToWorld(events.getMousePos());
    Vec2 gun_glob = tr_m*gun.getPosition();
    Dir2 aim_dir = mouse_glob - gun_glob;
    Angle aim_rot = (aim_dir.getAngle() - ctr.get().getRotation()).normalize();
    gun.setRotation(aim_rot);
}

inline void Player::renderMotion_(Entity &me, GameBase &game, f32 dt) {
    Resources &res = Resources::get();

    CTransform ctr = me.get<CTransform>();
    CRenderables crs = me.get<CRenderables>();
    CWorldBodyData& cwb = me.getData<CWorldBodyData>();
    CMovableData& cmd = me.getData<CMovableData>();

    if (cwb.state_flags[sfLanded]) {

        Window& w = game.getModule<Window>();
        Events& e = w.getEvents();
        bool moving = e.isKeyDown(SDL_SCANCODE_A) || e.isKeyDown(SDL_SCANCODE_D);
        if (moving) {
            // motion "hopping" animation
            f32 move_hop_stage = fmodf(game.getTimer().getElapsed(), res.move_hop_dt)/res.move_hop_dt;
            move_hop_stage = std::max(0.1f, move_hop_stage);
            Vec2 hop_offset{0, -move_hop_stage*res.move_hop_height};
            for (u32 i=0; i<rCount; ++i) {
                auto rend = crs.get<RectRenderable>(i);
                rend.setPosition(physics::motPosWithTime(rend.getPosition(), res.parts[i].rest_pos + hop_offset, 0.2f*move_hop_stage, dt));
            }
            return;
        }
    }

    // no animation
    for (u32 i=0; i<rCount; ++i) {
        auto rend = crs.get<RectRenderable>(i);
        rend.setPosition(physics::motPosWithTime(rend.getPosition(), res.parts[i].rest_pos, 0.1f, dt));
    }
}

inline Vec2 Player::getGunTip_(Entity &me, Dir2* gun_dir_out) {
    Resources &res = Resources::get();

    CTransform ctr = me.get<CTransform>();
    CRenderables crs = me.get<CRenderables>();
    Transform loc_tr = crs.get(rGun).accRenderTask()->getLocalTransform();
    Mat3 tr_m = ctr.get().calcMatrix() * loc_tr.calcMatrix();
    if (gun_dir_out) {
        *gun_dir_out = normalize(tr_m*Dir2{1.0f, 0.0f});
    }
    return tr_m*Vec2{1.0f, 0.0f};
}