/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "UDPClient.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/constants.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/control.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "core/input_manager.hpp"
#include "core/shared_entity.hpp"
#include "core/tick_rate_manager.hpp"
#include "systems/control_special.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/share_movement.hpp"

#include <SFML/Graphics.hpp>
#include <thread>

#include "GameProtocol.hpp"
#include "argParser.hpp"
#include "my_log.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

static void register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
    reg.register_component<ecs::component::share_movement>();
    reg.register_component<ecs::component::shared_entity>();
    reg.register_component<ecs::component::missile>();
}

static void register_systems(
    ecs::registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input,
    ecs::tick_rate_manager &tick_rate_manager
)
{
    tick_rate_manager.add_tick_rate(ecs::constants::movement_tick_rate);

    reg.add_system([&reg, &input, &udpClient]() { ecs::systems::control(reg, input); });
    reg.add_system([&reg, &input, &udpClient]() { ecs::systems::control_special(reg, input, udpClient); });
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &window]() {
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
    reg.add_system([&reg, &udpClient, &tick_rate_manager, &dt]() {
        if (tick_rate_manager.need_update(ecs::constants::movement_tick_rate, dt)) {
            ecs::systems::share_movement(reg, udpClient);
        }
    });
    reg.add_system([&reg]() { ecs::systems::missiles_stop(reg); });
}

static void create_player(ecs::registry &reg, client::UDPClient &udpClient)
{
    auto player = reg.spawn_shared_entity(ecs::generate_shared_entity_id());
    reg.add_component(player, ecs::component::position{400.f, 300.f});

    reg.add_component(player, ecs::component::velocity{0.f, 0.f});
    reg.add_component(player, ecs::component::controllable{});
    ecs::component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    playerDrawable.shape.setFillColor(sf::Color::Green);
    reg.add_component(player, std::move(playerDrawable));

    reg.add_component(player, ecs::component::hitbox{50.f, 50.f});
    reg.add_component(player, ecs::component::share_movement{});

    // ! will be changed with lobby
    ecs::protocol msg = {
        .action = ecs::ntw_action::NEW_PLAYER,
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(player).value().shared_entity_id
    };
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    // ! will be changed with lobby
}

static void create_static(ecs::registry &reg, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::drawable entityDrawable;
    entityDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    entityDrawable.shape.setFillColor(sf::Color::Red);
    reg.add_component(entity, std::move(entityDrawable));

    reg.add_component(entity, ecs::component::hitbox{50.f, 50.f});
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

// ! Start ImGui
bool inRoom = false;
int nbRoom = -1;

struct Room {
    std::string name;
    int nbPlayers;
};

struct Player {
    std::string name;
    std::string status;
};

// ! This function is call when you click on a room in the lobby array.
static void renderInsideRoom(const std::string &name, const std::vector<Player> &players)
{
    // ! Window
    ImVec2 windowSize(850, 600);
    ImVec2 windowPos(100, 50);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::Begin(
        name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
    );

    // ! Table
    ImGui::BeginTable("playersTable", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();

    // ! Action with table
    for (const auto &player : players) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", player.name.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", player.status.c_str());
    }
    ImGui::EndTable();

    // ! Back to lobby button
    ImVec2 buttonSize(200, 50);
    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 buttonPos =
        ImVec2(windowContentRegionMax.x - buttonSize.x - 20, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Back to lobby", buttonSize)) {
        nbRoom = -1;
        inRoom = false;
    }

    ImGui::End();
}

static int getLastPartAfterSpace(const std::string &name)
{
    size_t pos = name.find_last_of(' ');
    if (pos != std::string::npos) {
        return std::stoi(name.substr(pos + 1));
    }
    return -1;
}

// ! This function is call when you are on the lobby page.
static void renderLobbyWindow(std::vector<Room> &rooms)
{
    // ! Window
    ImVec2 windowSize(850, 600);
    ImVec2 windowPos(100, 50);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::Begin("Lobby", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    // ! Table
    ImGui::BeginTable("table", 3);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Number of players");
    ImGui::TableSetupColumn("Actions");
    ImGui::TableHeadersRow();

    int playerToDelete = -1;

    // ! Action with table
    for (size_t i = 0; i < rooms.size(); ++i) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button(rooms.at(i).name.c_str())) {
            inRoom = true;
            nbRoom = getLastPartAfterSpace(rooms.at(i).name.c_str());
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d/4", rooms.at(i).nbPlayers);
        ImGui::TableSetColumnIndex(2);
        std::string deleteButtonId = "Delete##" + std::to_string(i);
        if (ImGui::Button(deleteButtonId.c_str())) {
            playerToDelete = i;
        }
    }

    // ! Delete action
    if (playerToDelete != -1) {
        rooms.erase(rooms.begin() + playerToDelete);
        playerToDelete = -1;
    }
    ImGui::EndTable();

    // ! Create room
    ImVec2 buttonSize(200, 50);
    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 buttonPos =
        ImVec2(windowContentRegionMax.x - buttonSize.x - 20, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Create", buttonSize)) {
        rooms.push_back(Room("Room " + std::to_string(rooms.size() + 1), 0));
    }

    ImGui::End();
}

// ! This function is call every time.
static void renderPlayerListWindow()
{
    // ! Window
    ImVec2 windowSize(200, 600);
    ImVec2 windowPos(1000, 50);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::Begin(
        "Player List", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
    );

    // ! List with scroll
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (int i = 0; i < 100; ++i) {
        ImGui::Text("Player %d", i + 1);
    }
    ImGui::EndChild();

    ImGui::End();
}

static void runGui(
    sf::RenderWindow &window,
    sf::Clock &deltaClock,
    std::vector<Room> &rooms,
    std::vector<Player> &players
)
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        // ! Render our window
        if (!inRoom) {
            renderLobbyWindow(rooms);
        } else {
            renderInsideRoom(("Room " + std::to_string(nbRoom)), players);
        }
        renderPlayerListWindow();
        // ! End render our window

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}

int main(int ac, char **av)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "R-Type");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    // ! Fill rooms and players
    std::vector<Room> rooms;
    std::vector<Player> players = {{"Alice", "Ready"}, {"Bob", "Not Ready"}, {"Charlie", "Ready"}, {"Diana", "Ready"}};
    for (int i = 0; i < 3; ++i) {
        rooms.push_back(Room("Room " + std::to_string(i + 1), 0));
    }

    // ! Start game loop
    runGui(window, deltaClock, rooms, players);
    return 0;

    // ArgParser argParser;
    // argParser.addArgument("ip", "i", ArgParser::ArgType::STRING, true, "Server IP address");
    // argParser.addArgument("port", "p", ArgParser::ArgType::INT, true, "Server port");
    // argParser.addArgument("help", "h", ArgParser::ArgType::BOOL, false, "Print this help message");

    // if (!argParser.parse(ac, av)) {
    //     argParser.printHelp();
    //     return 84;
    // }
    // if (argParser.getValue<bool>("help")) {
    //     argParser.printHelp();
    //     return 0;
    // }

    // auto ip = argParser.getValue<std::string>("ip");
    // auto port = argParser.getValue<int>("port");

    // try {
    //     client::UDPClient udpClient(ip, port);
    //     std::thread receiveThread([&udpClient]() { udpClient.run(); });

    //     ecs::registry reg;
    //     float dt = 0.f;
    //     sf::RenderWindow window(sf::VideoMode(ecs::constants::screen_width, ecs::constants::screen_height),
    //     "R-Type"); ecs::input_manager input_manager; ecs::tick_rate_manager tick_rate_manager;

    //     window.setFramerateLimit(ecs::constants::fps_limit);
    //     register_components(reg);
    //     register_systems(reg, window, dt, udpClient, input_manager, tick_rate_manager);

    //     create_player(reg, udpClient);

    //     for (int i = 0; i < 10; ++i) {
    //         create_static(reg, 100.f * i, 100.f * i);
    //     }

    //     run(reg, window, dt, udpClient, input_manager);

    //     receiveThread.join();
    // } catch (const std::exception &exception) {
    //     my::log::error(exception.what());
    //     return 84;
    // } catch (...) {
    //     my::log::error("Unknow error.");
    //     return 84;
    // }
    // return 0;
}
