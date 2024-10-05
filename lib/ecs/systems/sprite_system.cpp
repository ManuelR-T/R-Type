/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sprite_system
*/

#include "sprite_system.hpp"
#include <iostream>
#include "../components/animation.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "core/IndexedZipper.hpp"
#include "core/Registry.hpp"
#include "core/Zipper.hpp"

namespace ecs::systems {

void spriteSystem(Registry &reg, float dt, SpriteManager &spriteManager)
{
    auto &sprites = reg.getComponents<component::Sprite>();
    auto &positions = reg.getComponents<component::Position>();
    auto &animations = reg.getComponents<component::Animation>();

    Zipper<component::Sprite, component::Position> spriteZip(sprites, positions);

    for (auto &&[sprite_comp, pos_comp] : spriteZip) {
        if (sprite_comp.spriteObj.getTexture() == nullptr) {
            sf::Texture &texture = spriteManager.getTexture(sprite_comp.textureId);
            sprite_comp.spriteObj.setTexture(texture);
        }
        sprite_comp.spriteObj.setPosition(pos_comp.x, pos_comp.y);
    }
    IndexedZipper<component::Sprite, component::Animation> animZip(sprites, animations);

    for (auto &&[id, sprite_comp, anim_comp] : animZip) {
        anim_comp.elapsedTime += dt;
        if (anim_comp.elapsedTime >= anim_comp.frameTime) {
            anim_comp.updateState(reg, id, anim_comp);
            anim_comp.elapsedTime = 0.0f;
            auto size = anim_comp.frames[anim_comp.state].size();
            if (size == 0) {
                std::cerr << "No frames for state " << anim_comp.state << std::endl;
                continue;
            }
            anim_comp.currentFrame = (anim_comp.currentFrame + 1) % anim_comp.frames[anim_comp.state].size();
            sprite_comp.spriteObj.setTextureRect(anim_comp.frames[anim_comp.state][anim_comp.currentFrame]);
        }
    }
}

} // namespace ecs::systems
