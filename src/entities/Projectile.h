
#ifndef LD38_PROJECTILE_H
#define LD38_PROJECTILE_H

class Projectile : public GengEntityDef<CTransformData, CMovableData, CRenderablesData, CWorldBodyData> {
public:
    static void init(Entity &me, GameBase &game, const Vec2& pos, const Dir2& dir) {
        CRenderables crs = me.get<CRenderables>();
        crs.resize(1);
        crs.set<CircleRenderable>(0)
                .setPosition({0,0})
                .setColor(Colorf::Yellow())
                .setOuterRadius(4)
                .accRenderTask()
                        ->setLayer(lProjectiles);

        CTransform ctr = me.get<CTransform>();
        ctr.setPosition(pos);

        CMovableData& cmd = me.getData<CMovableData>();
        cmd.speed.setLinearSpeed(dir*500);

        CWorldBodyData& cwd = me.getData<CWorldBodyData>();
        cwd.half_height = 2;
    }
};

#endif //LD38_PROJECTILE_H
