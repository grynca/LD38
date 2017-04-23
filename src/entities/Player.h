#ifndef INC_LD38_PLAYER_H
#define INC_LD38_PLAYER_H

#include "../incl.h"

class Player : public GengEntityDef<CTransformData, CMovableData, CRenderablesData, CWorldBodyData> {
public:

    enum {
        rBody,
        rGun,
        rHead,
        rCount
    };


    class Resources : public Singleton<Resources> {
    public:
        void init(GameBase &game);

        struct {
            Vec2 rest_pos;
            Vec2 size;
            Colorf color;
            Vec2 offset;
            u16 layer;
        } parts[rCount];

        f32 move_speed;
        f32 move_hop_height;
        f32 move_hop_dt;
        f32 jump_impulse;

        f32 machinegun_firerate;
        f32 machinegun_dispersion;
        f32 machinegun_recoil;
    };

    static void init(Entity &me, GameBase &game);
    static void update(Entity &me, GameBase &game);
    static void render(Entity &me, GameBase &game, f32 dt);
    static void onKilled(Entity &me, GameBase &game);

private:
    static void renderGunAiming_(Entity &me, GameBase &game);
    static void renderMotion_(Entity &me, GameBase &game, f32 dt);

    static Vec2 getGunTip_(Entity &me, Dir2* gun_dir_out = NULL);
};

#include "Player.inl"

#endif //INC_LD38_PLAYER_H