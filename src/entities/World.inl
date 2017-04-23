#include "World.h"

inline void World::Resources::init(GameBase &game) {
    radius = 300.0f;
    gravity_max = 300.0f;
    gravity_min = 150.0f;
}


inline void World::init(Entity &me, GameBase &game) {
//static
    static Resources &res = Resources::get();

    CRenderables rs = me.get<CRenderables>();
    rs.resize(rCount);

    rs.set<CircleRenderable>(rBody)
            .setOuterRadius(res.radius)
            .setColor(Colorf::White())
            .setPosition({0, 0})
            .accRenderTask()
               ->setLayer(lWorld);

    rs.set<RectRenderable>(rZeroMark)
            .setSize({4, 20})
            .setColor(Colorf::Red())
            .setGeomNormOffset({-0.5f, 0})
            .setPosition({0, -res.radius})
            .accRenderTask()
               ->setLayer(lWorld);

}