#ifndef LD38_MYGAME_H
#define LD38_MYGAME_H

#include "incl.h"

class MyGame : public grynca::Game {
public:
    enum TextureIds {
        tidSprites = 0,
        tidFonts = 1
    };

    struct Assets {
        Index fonts_pack_id;
        Index images_pack_id;
    } assets;


private:
    EntityIndex player_id;

    virtual void init() override;
    virtual void update() override;
    virtual void render(f32 dt) override;
    virtual void tick() override;

    void initAssets();
};

#include "MyGame.inl"
#endif //LD38_MYGAME_H
