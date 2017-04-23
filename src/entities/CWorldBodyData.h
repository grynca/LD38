#ifndef LD38_WORLDBODY_H
#define LD38_WORLDBODY_H

enum StateFlags {
    sfShooting,
    sfLanded,

    sfCount
};

struct CWorldBodyData {
    static RolesMask componentRoles() {
        return MyRoles::erWorldBodyMask();
    }

    CWorldBodyData() : last_shot_time(0.0f) {}

    f32 half_height;
    Mask<sfCount> state_flags;

    f32 last_shot_time;
};

#endif //LD38_WORLDBODY_H
