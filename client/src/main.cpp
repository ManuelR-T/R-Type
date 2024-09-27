/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include <thread>
#include "GameProtocol.hpp"
#include "UDPClient.hpp"
#include "argParser.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/SpriteManager.hpp"
#include "core/constants.hpp"
#include "core/entity.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "core/input_manager.hpp"
#include "core/shared_entity.hpp"
#include "core/tick_rate_manager.hpp"
#include "my_log.hpp"
#include "systems/ai_act.hpp"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/share_movement.hpp"
#include "systems/sprite_system.hpp"

static void register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::sprite>();
    reg.register_component<ecs::component::animation>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
    reg.register_component<ecs::component::share_movement>();
    reg.register_component<ecs::component::shared_entity>();
    reg.register_component<ecs::component::missile>();
    reg.register_component<ecs::component::ai_actor>();
}

static void register_systems(
    ecs::registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input,
    ecs::tick_rate_manager &tick_rate_manager,
    SpriteManager &sprite_manager
)
{
    tick_rate_manager.add_tick_rate(ecs::constants::movement_tick_rate);
    tick_rate_manager.add_tick_rate(10);

    reg.add_system([&reg, &input, &udpClient]() { ecs::systems::control_move(reg, input); });
    reg.add_system([&reg, &input, &udpClient, &sprite_manager]() {
        ecs::systems::control_special(reg, input, udpClient, sprite_manager);
    });
    reg.add_system([&reg, &dt, &tick_rate_manager]() {
        if (tick_rate_manager.need_update(10, dt)) {
            ecs::systems::ai_act(reg);
        }
    });
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &udpClient, &tick_rate_manager, &dt]() {
        if (tick_rate_manager.need_update(ecs::constants::movement_tick_rate, dt)) {
            ecs::systems::share_movement(reg, udpClient);
        }
    });
    reg.add_system([&reg]() { ecs::systems::missiles_stop(reg); });
    reg.add_system([&reg, &dt, &sprite_manager]() { ecs::systems::sprite_system(reg, dt, sprite_manager); });
    reg.add_system([&reg, &window]() {
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
}

static void create_player(ecs::registry &reg, client::UDPClient &udpClient, SpriteManager &sprite_manager)
{
    auto player = reg.spawn_shared_entity(ecs::generate_shared_entity_id());
    reg.add_component(player, ecs::component::position{400.f, 300.f});
    reg.add_component(player, ecs::component::velocity{0.f, 0.f});
    reg.add_component(player, ecs::component::controllable{});

    ecs::component::sprite playerSprite;
    playerSprite.texture_id = "assets/typesheets/r-typesheet1.gif";
    playerSprite.sprite_obj.setTexture(sprite_manager.get_texture(playerSprite.texture_id));
    playerSprite.sprite_obj.setPosition(400.f, 300.f);

    playerSprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 16));
    ecs::component::animation playerAnimation;
    playerAnimation.frames["up"] = {{135, 2, 32, 16}};
    playerAnimation.frames["top"] = {{102, 2, 32, 16}};
    playerAnimation.frames["neutral"] = {{168, 2, 32, 16}};
    playerAnimation.frames["down"] = {{201, 2, 32, 16}};
    playerAnimation.frames["bottom"] = {{234, 2, 32, 16}};
    playerAnimation.frame_time = 0.1f;
    playerAnimation.updateState = [](ecs::registry &reg, entity_t id, ecs::component::animation &anim) {
        auto vel_opt = reg.get_component<ecs::component::velocity>(id);
        if (!vel_opt) {
            return;
        }

        auto &vel = *vel_opt;
        float vy = vel.vy;
        std::string &state = anim.state;

        if (vy > 0) {
            if (state == "up" || state == "top") {
                state = "top";
            } else if (state == "neutral") {
                state = "up";
            } else {
                state = "neutral";
            }
        } else if (vy < 0) {
            if (state == "down" || state == "bottom") {
                state = "bottom";
            } else if (state == "neutral") {
                state = "down";
            } else {
                state = "neutral";
            }
        } else {
            if (state == "top") {
                state = "up";
            } else if (state == "bottom") {
                state = "down";
            } else {
                state = "neutral";
            }
        }
    };

    reg.add_component(player, std::move(playerAnimation));
    reg.add_component(player, std::move(playerSprite));
    reg.add_component(player, ecs::component::hitbox{32.f, 16.f});
    reg.add_component(player, ecs::component::share_movement{});

    // ! will be changed with lobby
    ecs::protocol msg = {
        .action = ecs::ntw_action::NEW_PLAYER,
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(player).value().shared_entity_id
    };
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    // ! will be changed with lobby
}

static void create_static(ecs::registry &reg, SpriteManager &sprite_manager, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::sprite entitySprite;
    entitySprite.texture_id = "assets/typesheets/r-typesheet5.gif";
    entitySprite.sprite_obj.setTexture(sprite_manager.get_texture(entitySprite.texture_id));
    entitySprite.sprite_obj.setPosition(x, y);

    entitySprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::animation entityAnimation;
    entityAnimation.frames["neutral"] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32}
    };

    reg.add_component(entity, std::move(entityAnimation));
    reg.add_component(entity, std::move(entitySprite));
    reg.add_component(entity, ecs::component::hitbox{32.f, 32.f});
}

static void create_ai(ecs::registry &reg, SpriteManager &sprite_manager, float x, float y)
{
    auto entity = reg.spawn_entity();

    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::sprite entitySprite;
    entitySprite.texture_id = "assets/typesheets/r-typesheet5.gif";
    entitySprite.sprite_obj.setTexture(sprite_manager.get_texture(entitySprite.texture_id));
    entitySprite.sprite_obj.setPosition(x, y);

    entitySprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::animation entityAnimation;
    entityAnimation.frames["neutral"] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32}
    };

    reg.add_component(entity, std::move(entityAnimation));
    reg.add_component(entity, std::move(entitySprite));

    auto func = [](ecs::registry &reg, entity_t e) {
        auto &pos = reg.get_component<ecs::component::position>(e);
        auto &val = reg.get_component<ecs::component::ai_actor>(e)->val;

        if (val) {
            pos->y += 20;
        } else {
            pos->y -= 20;
        }
        val = !val;
    };
    reg.add_component(entity, ecs::component::ai_actor{true, std::move(func)});
}

static void run(
    ecs::registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input
)
{
    sf::Clock clock;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            input.update(event);
        }
        reg.run_systems();
    }
}

int main(int ac, char **av)
{
    ArgParser argParser;
    argParser.addArgument("ip", "i", ArgParser::ArgType::STRING, true, "Server IP address");
    argParser.addArgument("port", "p", ArgParser::ArgType::INT, true, "Server port");
    argParser.addArgument("help", "h", ArgParser::ArgType::BOOL, false, "Print this help message");

    if (!argParser.parse(ac, av)) {
        argParser.printHelp();
        return 84;
    }
    if (argParser.getValue<bool>("help")) {
        argParser.printHelp();
        return 0;
    }

    auto ip = argParser.getValue<std::string>("ip");
    auto port = argParser.getValue<int>("port");
    SpriteManager sprite_manager;
    try {
        client::UDPClient udpClient(ip, port);
        std::thread receiveThread([&udpClient]() { udpClient.run(); });

        ecs::registry reg;
        float dt = 0.f;
        sf::RenderWindow window(sf::VideoMode(ecs::constants::screen_width, ecs::constants::screen_height), "R-Type");
        ecs::input_manager input_manager;
        ecs::tick_rate_manager tick_rate_manager;

        window.setFramerateLimit(ecs::constants::fps_limit);
        register_components(reg);
        register_systems(reg, window, dt, udpClient, input_manager, tick_rate_manager, sprite_manager);

        create_player(reg, udpClient, sprite_manager);

        for (int i = 0; i < 20; ++i) {
            create_static(reg, sprite_manager, 48.f * i, 48.f * i);
        }

        // create_ai(reg, dt);
        run(reg, window, dt, udpClient, input_manager);

        receiveThread.join();
    } catch (const std::exception &exception) {
        my::log::error(exception.what());
        return 84;
    } catch (...) {
        my::log::error("Unknow error.");
        return 84;
    }
    return 0;
}
