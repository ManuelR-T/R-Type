/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sprite_system
*/

#include "sprite_system.hpp"
#include "../components/animation.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "core/registry.hpp"
#include "core/zipper.hpp"
#include "core/indexed_zipper.hpp"

namespace ecs::systems {

void spriteSystem(Registry &reg, float dt, SpriteManager &spriteManager)
{
    auto &sprites = reg.getComponents<component::Sprite>();
    auto &positions = reg.getComponents<component::Position>();
    auto &animations = reg.getComponents<component::Animation>();

    Zipper<component::Sprite, component::Position> spriteZip(sprites, positions);

    for (auto &&[sprite_comp, pos_comp] : spriteZip) {
        if (sprite_comp.sprite_obj.getTexture() == nullptr) {
            sf::Texture &texture = spriteManager.getTexture(sprite_comp.texture_id);
            sprite_comp.sprite_obj.setTexture(texture);
        }
        sprite_comp.sprite_obj.setPosition(pos_comp.x, pos_comp.y);
    }
    IndexedZipper<component::Sprite, component::Animation> animZip(sprites, animations);

    for (auto &&[id, sprite_comp, anim_comp] : animZip) {
        anim_comp.elapsed_time += dt;
        if (anim_comp.elapsed_time >= anim_comp.frame_time) {
            anim_comp.updateState(reg, id, anim_comp);
            anim_comp.elapsed_time = 0.0f;
            anim_comp.current_frame = (anim_comp.current_frame + 1) % anim_comp.frames[anim_comp.state].size();
            sprite_comp.sprite_obj.setTextureRect(anim_comp.frames[anim_comp.state][anim_comp.current_frame]);
        }
    }
}

} // namespace ecs::systems
