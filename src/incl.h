#ifndef ENTS_H
#define ENTS_H

#include "geng_enums.h"
using namespace grynca;

DEFINE_ENUM_E(MyRoles, GERoles,
        erWorldBody
);

#define SYSENT_MAX_ROLES MyRoles::end
#define SYSENT_MAX_FLAGS GEFlags::end
#include "geng.h"

enum Layers {
    lWorld,
    lPlayer,
    lPlayerGun,
    lProjectiles
};

#include "entities/CWorldBodyData.h"

#include "entities/World.h"
#include "entities/Projectile.h"
#include "entities/Player.h"

#include "entities/WorldSystem.h"

typedef grynca::TypesPack<
    World, Player, Projectile
> EntityTypes;

#include "MyGame.h"

#endif //ENTS_H
