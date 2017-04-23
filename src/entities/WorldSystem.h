#ifndef LD38_WORLDSYSTEM_H
#define LD38_WORLDSYSTEM_H

#include "../incl.h"

class WorldSystem : public grynca::SystemAll {
public:
    virtual RolesMask NeededRoles() override {
        return MyRoles::erWorldBodyMask() | MyRoles::erTransformMask() | MyRoles::erMovableMask();
    }

    void init() {
        Game& g = Game::get();
        world_ent_id = g.createEntity<World>(World::init, g).getIndex();
        wres = &World::Resources::get();
    }

    virtual void update(f32 dt, EntitiesList& entities) override {
        world = Game::get().getEntity(world_ent_id);
        world_pos = world.get<CTransform>().get().getPosition();

        entities.loopEntities([this, dt] (Entity& ent) {
            // apply gravity
            CMovableData& cmd = ent.getData<CMovableData>();
            CTransform ctr = ent.get<CTransform>();
            CWorldBodyData& cwb = ent.getData<CWorldBodyData>();

            Vec2 to_world = world_pos - ctr.get().getPosition();
            f32 to_world_dst = to_world.getLen() - cwb.half_height;
            Angle world_a = to_world.getAngle();
            Vec2 to_world_dir = world_a.getDir();

            f32 dist_to_land = to_world_dst - wres->radius;
            if (dist_to_land <= 0) {
                // collides with world -> remove speed direction towards world
                f32 proj = cmd.speed.getLinearSpeed().calcDirProjection(to_world_dir);
                cmd.speed.addLinearSpeed(-to_world_dir*proj);
                ctr.setPosition(-to_world_dir*(wres->radius+cwb.half_height));
            }
            else {
                f32 t = clampToRange(wres->radius/to_world_dst, 0.0f, 1.0f);
                f32 grav_pull = lerp(t, wres->gravity_min, wres->gravity_max) *dt;
                Vec2 grav_vec = to_world_dir*grav_pull;
                cmd.speed.addLinearSpeed(grav_vec);
            }
            cwb.state_flags.set(sfLanded, dist_to_land < 5);

            // rotate entity to point to center of gravity
            ctr.setRotation(world_a - Angle::Pi_2);     // make top of world origin pos
        });
    }


    EntityIndex world_ent_id;
    Entity world;
    Vec2 world_pos;
    const World::Resources* wres;
};

#endif //LD38_WORLDSYSTEM_H
