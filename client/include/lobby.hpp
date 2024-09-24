/*
** EPITECH PROJECT, 2024
** r-type-client
** File description:
** lobby
*/

#include <SFML/Graphics.hpp>

#ifndef LOBBY_HPP_
    #define LOBBY_HPP_

namespace Cli {
class Lobby {
    enum BoxType {
        VISIBLE,
        NOT_VISIBLE,
        VISIBLE_CREATE,
        VISIBLE_READY
    };

    struct MovableBox {
        sf::RectangleShape rectangle;
        sf::RectangleShape deleteButton;
        sf::Text roomName;
        sf::Text nbPlayers;
        sf::Text deleteName;
    };

    struct StaticBox {
        sf::RectangleShape rectangle;
        BoxType type;
    };

    struct Player {
        sf::RectangleShape rectangle;
        sf::Text nameText;
        sf::Text isReadyText;
        bool isReady;
    };

    public:
    void initLobby();
    void runLobby(sf::RenderWindow &window);

    private:
    void boxScrolling(const float &delta);
    void addComponentLobby();
    void addNewRoom();
    void removeRoom(const std::size_t &indexRoom);
    void moveLeftRooms(const std::size_t &startIdxRoom);
    void createStaticVisibleBox(float screenWidth, float screenHeight);
    void createUnseenStaticBox(float screenWidth, float screenHeight);
    StaticBox createStaticBox(
        const sf::Vector2f &size,
        const sf::Vector2f &pos,
        const sf::Color &color,
        const BoxType &boxType
    );
    MovableBox createMovableBox(const sf::Vector2f &size, const sf::Vector2f &pos, const sf::Color &color);
    void initializeTextBox(MovableBox &box, const sf::Vector2f &pos);
    void updateBoxesPosition(MovableBox &box, const sf::Vector2f &pos, const float &offset);

    void fetchListPlayerFromRoom();
    void createPlayerLine(const std::string &namePlayer, const bool &isReady);

    // RUN
    void processEvents(sf::RenderWindow &window, bool &isInPlayerList);
    void handleWindowCloseEvent(const sf::Event &event, sf::RenderWindow &window);
    void handleMouseScrollEvent(const sf::Event &event);
    void handleMouseClickEvent(const sf::Event &event, sf::RenderWindow &window, bool &isInPlayerList);
    void handleStaticBoxClick(const sf::Vector2i &mousePos, bool &isInPlayerList);
    void handleMovableBoxClick(const sf::Vector2i &mousePos, bool &isInPlayerList);
    void render(sf::RenderWindow &window, bool isInPlayerList);
    void renderPlayerList(sf::RenderWindow &window);
    void renderMovableBoxes(sf::RenderWindow &window);
    void renderStaticBoxes(sf::RenderWindow &window, bool isInPlayerList);

    std::vector<StaticBox> _staticBoxes;
    std::vector<MovableBox> _movableBoxes;
    std::vector<Player> _playersList;
    sf::Font _font;
    std::size_t _nbRoom = 0;
};
} // namespace Cli

#endif /* !MY_TRACKED_EXCEPTION_HPP_ */
