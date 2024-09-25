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
#include "core/zipper.hpp"

namespace ecs::systems {

void sprite_system(registry &reg, float dt, SpriteManager &sprite_manager)
{
    auto &sprites = reg.get_components<component::sprite>();
    auto &positions = reg.get_components<component::position>();
    auto &animations = reg.get_components<component::animation>();

    zipper<component::sprite, component::position> sprite_zip(sprites, positions);

    for (auto &&[sprite_comp, pos_comp] : sprite_zip) {
        if (sprite_comp.sprite_obj.getTexture() == nullptr) {
            sf::Texture &texture = sprite_manager.get_texture(sprite_comp.texture_id);
            sprite_comp.sprite_obj.setTexture(texture);
        }
        sprite_comp.sprite_obj.setPosition(pos_comp.x, pos_comp.y);
    }
    zipper<component::sprite, component::animation> anim_zip(sprites, animations);

    for (auto &&[sprite_comp, anim_comp] : anim_zip) {
        anim_comp.elapsed_time += dt;
        if (anim_comp.elapsed_time >= anim_comp.frame_time) {
            anim_comp.elapsed_time = 0.0f;
            anim_comp.current_frame = (anim_comp.current_frame + 1) % anim_comp.frames.size();
            sprite_comp.sprite_obj.setTextureRect(anim_comp.frames[anim_comp.current_frame]);
        }
    }
}

} // namespace ecs::systems
