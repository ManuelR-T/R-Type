/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientEntityFactory
*/

#include "ClientEntityFactory.hpp"
#include <memory>
#include <string>
#include <utility>
#include "Candle/LightSource.hpp"
#include "Particles/ParticleGenerator.h"
#include "Particles/ParticleSpawner.h"
#include "Particles/ParticleSystem.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SpriteManager.hpp"
#include "components/animation.hpp"
#include "components/beam.hpp"
#include "components/hitbox.hpp"
#include "components/parallax.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "imgui.h"
#include "components/client_share_movement.hpp"
#include "components/death_timer.hpp"
#include "components/light_edge.hpp"
#include "components/music_component.hpp"
#include "components/on_death.hpp"
#include "components/particle_spawner.hpp"
#include "components/radial_light.hpp"
#include "components/score_earned.hpp"
#include "components/sound_emitter.hpp"

namespace ecs {

const std::unordered_map<std::string, std::function<void(Registry &, entity_t, ecs::component::Animation &)>>
    ClientEntityFactory::ANIM_MAP = {
        {"player",
         [](ecs::Registry &reg, entity_t id, ecs::component::Animation &anim) {
             auto velOpt = reg.getComponent<ecs::component::Velocity>(id);
             if (!velOpt) {
                 return;
             }

             auto &vel = *velOpt;
             float vy = vel.vy;
             std::string &state = anim.state;

             if (vy > 0) {
                 if (state == "up" || state == "top") {
                     state = "top";
                 } else if (state == "idle") {
                     state = "up";
                 } else {
                     state = "idle";
                 }
             } else if (vy < 0) {
                 if (state == "down" || state == "bottom") {
                     state = "bottom";
                 } else if (state == "idle") {
                     state = "down";
                 } else {
                     state = "idle";
                 }
             } else {
                 if (state == "top") {
                     state = "up";
                 } else if (state == "bottom") {
                     state = "down";
                 } else {
                     state = "idle";
                 }
             }
             std::string &stateSub = reg.getComponent<ecs::component::Sprite>(id)->subSprites[0].animation.state;
             float vx = vel.vx;
             if (vx < 0) {
                 stateSub = "left";
             } else if (vx > 0) {
                 stateSub = "right";
             } else {
                 stateSub = "idle";
             }
             if (reg.getComponent<ecs::component::Beam>(id)->isCharging) {
                 reg.getComponent<ecs::component::Sprite>(id)->subSprites[1].animation.state = "loading";
             } else {
                 reg.getComponent<ecs::component::Sprite>(id)->subSprites[1].animation.state = "idle";
             }
         }},
        {"robotGround",
         [](ecs::Registry &reg, entity_t id, ecs::component::Animation &anim) {
             auto velOpt = reg.getComponent<ecs::component::Velocity>(id);
             if (!velOpt) {
                 return;
             }

             float vx = velOpt->vx;
             std::string &state = anim.state;

             if (vx < 0) {
                 state = "left";
             } else {
                 state = "right";
             }
         }},
        {"none", [](ecs::Registry &, entity_t, ecs::component::Animation &) {}},
};

void ClientEntityFactory::addComponents(
    Registry &reg,
    SpriteManager &spriteManager,
    entity_t entity,
    const nlohmann::json &componentsJson,
    bool /*isShared*/,
    int x,
    int y,
    float vx,
    float vy,
    std::shared_ptr<ImFont> font
)
{
    addCommonComponents(reg, entity, componentsJson, x, y, vx, vy, std::move(font));

    if (componentsJson.contains("sprite")) {
        auto spriteJson = componentsJson["sprite"];
        ecs::component::Sprite spriteComp;
        spriteComp.textureId = spriteJson["texture"];
        spriteComp.spriteObj.setTexture(spriteManager.getTexture(spriteComp.textureId));
        auto frameJson = spriteJson["initial_frame"];
        spriteComp.spriteObj.setTextureRect(sf::IntRect(
            frameJson["x"].get<int>(),
            frameJson["y"].get<int>(),
            frameJson["width"].get<int>(),
            frameJson["height"].get<int>()
        ));
        if (spriteJson.contains("width") && spriteJson.contains("height")) {
            spriteComp.spriteObj.setScale(
                spriteJson["width"].get<float>() / frameJson["width"].get<float>(),
                spriteJson["height"].get<float>() / frameJson["height"].get<float>()
            );
        }
        if (spriteJson.contains("sub_sprites")) {
            for (const auto &subSpriteJson : spriteJson["sub_sprites"]) {
                ecs::component::SubSprite subSprite;
                subSprite.textureId = subSpriteJson["texture"];
                subSprite.spriteObj.setTexture(spriteManager.getTexture(subSprite.textureId));
                auto subFrameJson = subSpriteJson["initial_frame"];
                subSprite.spriteObj.setTextureRect(sf::IntRect(
                    subFrameJson["x"].get<int>(),
                    subFrameJson["y"].get<int>(),
                    subFrameJson["width"].get<int>(),
                    subFrameJson["height"].get<int>()
                ));
                if (subSpriteJson.contains("x") && subSpriteJson.contains("y")) {
                    subSprite.x = subSpriteJson["x"].get<int>();
                    subSprite.y = subSpriteJson["y"].get<int>();
                }
                if (subSpriteJson.contains("animation")) {
                    auto animJson = subSpriteJson["animation"];
                    subSprite.animation.frameTime = animJson["frame_time"].get<float>();
                    for (const auto &[stateName, framesJson] : animJson["frames"].items()) {
                        for (const auto &frameJson : framesJson) {
                            subSprite.animation.frames[stateName].emplace_back(
                                frameJson["x"].get<int>(),
                                frameJson["y"].get<int>(),
                                frameJson["width"].get<int>(),
                                frameJson["height"].get<int>()
                            );
                        }
                    }
                    if (subSpriteJson.contains("current_frame")) {
                        subSprite.animation.currentFrame = subSpriteJson["current_frame"].get<size_t>();
                    }
                    if (subSpriteJson.contains("state")) {
                        subSprite.animation.state = subSpriteJson["state"];
                    } else {
                        subSprite.animation.state = "idle";
                    }
                    if (subSpriteJson.contains("update_state")) {
                        subSprite.animation.updateState = ANIM_MAP.at(subSpriteJson["update_state"]);
                    }
                }
                spriteComp.subSprites.push_back(subSprite);
            }
        }
        reg.addComponent(entity, std::move(spriteComp));
    }

    if (componentsJson.contains("animation")) {
        auto animJson = componentsJson["animation"];
        ecs::component::Animation animComp;
        animComp.frameTime = animJson["frame_time"].get<float>();

        for (const auto &[stateName, framesJson] : animJson["frames"].items()) {
            for (auto &frameJson : framesJson) {
                animComp.frames[stateName].emplace_back(
                    frameJson["x"].get<int>(),
                    frameJson["y"].get<int>(),
                    frameJson["width"].get<int>(),
                    frameJson["height"].get<int>()
                );
            }
        }
        if (animJson.contains("current_frame")) {
            animComp.currentFrame = animJson["current_frame"].get<size_t>();
        }
        if (animJson.contains("state")) {
            animComp.state = animJson["state"];
        } else {
            animComp.state = "idle";
        }
        if (animJson.contains("update_state")) {
            animComp.updateState = ANIM_MAP.at(animJson["update_state"]);
        }

        reg.addComponent(entity, std::move(animComp));
    }
    if (componentsJson.contains("sound_emitter")) {
        auto soundEmitterJson = componentsJson["sound_emitter"];
        ecs::component::SoundEmitter soundEmitterComp;
        soundEmitterComp.soundBufferId = soundEmitterJson["sound"];
        soundEmitterComp.volume = soundEmitterJson["volume"].get<float>();
        reg.addComponent(entity, std::move(soundEmitterComp));
    }
    if (componentsJson.contains("music")) {
        auto musicJson = componentsJson["music"];
        ecs::component::MusicComponent musicComp;
        musicComp.musicFilePath = musicJson["file_path"];
        musicComp.volume = musicJson["volume"].get<float>();
        musicComp.loop = musicJson["loop"].get<bool>();
        musicComp.isPlaying = musicJson["is_playing"].get<bool>();
        reg.addComponent(entity, std::move(musicComp));
    }
    if (componentsJson.contains("client_share_movement")) {
        reg.addComponent(entity, ecs::component::ClientShareMovement{});
    }
    if (componentsJson.contains("death_timer")) {
        auto deathTimerJson = componentsJson["death_timer"];
        ecs::component::DeathTimer deathTimerComp;
        deathTimerComp.timeToDeath = deathTimerJson["time"].get<float>();
        reg.addComponent(entity, std::move(deathTimerComp));
    }
    if (componentsJson.contains("score_earned")) {
        auto scoreJson = componentsJson["score_earned"];
        reg.addComponent(entity, ecs::component::ScoreEarned{scoreJson["points"].get<int>()});
    }
    if (componentsJson.contains("radial_light")) {
        auto radialLightJson = componentsJson["radial_light"];
        ecs::component::RadialLight light;
        auto componentsOffset = radialLightJson["offset"];
        light.offset.x = componentsOffset["x"].get<float>();
        light.offset.y = componentsOffset["y"].get<float>();
        auto componentsColor = radialLightJson["color"];
        light.light.setColor(
            sf::Color(componentsColor["r"].get<int>(), componentsColor["g"].get<int>(), componentsColor["b"].get<int>())
        );
        light.light.setRange(radialLightJson["range"].get<float>());
        light.light.setFade(radialLightJson["fade"].get<bool>());
        light.light.setBeamAngle(radialLightJson["angle"].get<float>());
        light.light.setRotation(radialLightJson["rotation"].get<float>());
        light.light.setIntensity(radialLightJson["intensity"].get<float>());
        reg.addComponent(entity, std::move(light));
    }
    if (componentsJson.contains("light_edge") && reg.hasComponent<ecs::component::Hitbox>(entity)) {
        ecs::component::LightEdge lightEdge;

        auto hitbox = reg.getComponent<ecs::component::Hitbox>(entity);
        auto pos = reg.getComponent<ecs::component::Position>(entity);

        float x1 = pos->x;
        float y1 = pos->y;
        float x2 = x1 + hitbox->width;
        float y2 = y1 + hitbox->height;

        lightEdge.edge.emplace_back(sf::Vector2f(x1, y1), sf::Vector2f(x2, y1));
        lightEdge.edge.emplace_back(sf::Vector2f(x1, y2), sf::Vector2f(x2, y2));
        lightEdge.edge.emplace_back(sf::Vector2f(x1, y1), sf::Vector2f(x1, y2));
        lightEdge.edge.emplace_back(sf::Vector2f(x2, y1), sf::Vector2f(x2, y2));

        reg.addComponent(entity, std::move(lightEdge));
    }
    if (componentsJson.contains("on_death")) {
        auto onDeathJson = componentsJson["on_death"];
        float x = 0;
        float y = 0;
        if (onDeathJson.contains("x") && onDeathJson.contains("y")) {
            x = onDeathJson["x"].get<float>();
            y = onDeathJson["y"].get<float>();
        }
        reg.addComponent(entity, ecs::component::OnDeath{onDeathJson["entity"], x, y});
    }
    if (componentsJson.contains("particle")) {
        auto particleJson = componentsJson["particle"];
        ecs::component::ParticleSpawner particle;

        auto offsetJson = particleJson["offset"];
        particle.offset.x = offsetJson["x"].get<float>();
        particle.offset.y = offsetJson["y"].get<float>();

        particle.texture = std::make_shared<sf::Texture>();
        particle.texture->loadFromFile("assets/particule/blob.png");
        particle.texture->setSmooth(true);

        particle.system = std::make_shared<particles::TextureParticleSystem>(
            particleJson["max_nb_particle"].get<int>(), particle.texture.get()
        );
        particle.system->additiveBlendMode = particleJson["additive_blend_mode"].get<bool>();
        particle.system->emitRate = particleJson["emit_rate"].get<int>();

        particle.spawner = particle.system->addSpawner<particles::PointSpawner>();
        particle.spawner->center = sf::Vector2f();

        auto *timeGenerator = particle.system->addGenerator<particles::TimeGenerator>();
        auto lifeTimeJson = particleJson["life_time"];
        timeGenerator->minTime = lifeTimeJson["min"].get<float>();
        timeGenerator->maxTime = lifeTimeJson["max"].get<float>();
        auto *sizeGenerator = particle.system->addGenerator<particles::SizeGenerator>();
        auto sizeJson = particleJson["size"];
        sizeGenerator->minStartSize = sizeJson["min_start"].get<float>();
        sizeGenerator->maxStartSize = sizeJson["max_start"].get<float>();
        sizeGenerator->minEndSize = sizeJson["min_end"].get<float>();
        sizeGenerator->maxEndSize = sizeJson["max_end"].get<float>();
        auto *velocityGenerator = particle.system->addGenerator<particles::AngledVelocityGenerator>();
        auto velocityJson = particleJson["velocity"];
        velocityGenerator->minAngle = velocityJson["min_angle"].get<float>();
        velocityGenerator->maxAngle = velocityJson["max_angle"].get<float>();
        velocityGenerator->minStartSpeed = velocityJson["min_start_speed"].get<float>();
        velocityGenerator->maxStartSpeed = velocityJson["max_start_speed"].get<float>();
        auto *colorGenerator = particle.system->addGenerator<particles::ColorGenerator>();
        auto colorJson = particleJson["color"];
        colorGenerator->minStartCol = sf::Color(
            colorJson["min_start"]["r"].get<int>(),
            colorJson["min_start"]["g"].get<int>(),
            colorJson["min_start"]["b"].get<int>(),
            colorJson["min_start"]["a"].get<int>()
        );
        colorGenerator->maxStartCol = sf::Color(
            colorJson["max_start"]["r"].get<int>(),
            colorJson["max_start"]["g"].get<int>(),
            colorJson["max_start"]["b"].get<int>(),
            colorJson["max_start"]["a"].get<int>()
        );
        colorGenerator->minEndCol = sf::Color(
            colorJson["min_end"]["r"].get<int>(),
            colorJson["min_end"]["g"].get<int>(),
            colorJson["min_end"]["b"].get<int>(),
            colorJson["min_end"]["a"].get<int>()
        );
        colorGenerator->maxEndCol = sf::Color(
            colorJson["max_end"]["r"].get<int>(),
            colorJson["max_end"]["g"].get<int>(),
            colorJson["max_end"]["b"].get<int>(),
            colorJson["max_end"]["a"].get<int>()
        );

        particle.system->addUpdater<particles::TimeUpdater>();
        particle.system->addUpdater<particles::ColorUpdater>();
        particle.system->addUpdater<particles::SizeUpdater>();
        particle.system->addUpdater<particles::EulerUpdater>();

        reg.addComponent(entity, std::move(particle));
    }
}

} // namespace ecs
