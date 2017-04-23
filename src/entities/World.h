
#ifndef INC_LD38_WORLD_H
#define INC_LD38_WORLD_H

#include "../incl.h"

class World : public GengEntityDef<CTransformData, CRenderablesData> {
public:
    class Resources : public Singleton<Resources> {
    public:
        void init(GameBase &game);
        f32 radius;
        f32 gravity_max;
        f32 gravity_min;
    };

    enum {
        rBody,
        rZeroMark,
        rCount
    };

    static void init(Entity &me, GameBase &game);
};

#include "World.inl"

#endif //INC_LD38_WORLD_H