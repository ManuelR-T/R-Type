#include "ServerEntityFactory.hpp"
#include <iostream>
#include "components/drawable.hpp"

namespace ecs {

ServerEntityFactory::ServerEntityFactory(ecs::Registry &reg) : EntityFactory(reg, true) {}

void ServerEntityFactory::addComponents(
    entity_t entity,
    const nlohmann::json &componentsJson,
    bool isShared,
    int x,
    int y
)
{
    std::cout << "before addCommonComponent" << std::endl;
    addCommonComponents(entity, componentsJson, x, y);
    std::cout << "after addCommonComponent" << std::endl;

    if (componentsJson.contains("drawable")) {
        auto drawableJson = componentsJson["drawable"];
        ecs::component::Drawable drawableComp;
        auto sizeJson = drawableJson["size"];
        std::cout << "Setting drawable size to: " << sizeJson["x"] << ", " << sizeJson["y"] << std::endl;

        drawableComp.shape.setSize(sf::Vector2f(sizeJson["x"].get<float>(), sizeJson["y"].get<float>()));
        auto fillColorJson = drawableJson["fill_color"];
        std::cout << "Setting fill color to: " << fillColorJson["r"] << ", " << fillColorJson["g"] << ", "
                  << fillColorJson["b"] << std::endl;

        drawableComp.shape.setFillColor(
            sf::Color(fillColorJson["r"].get<int>(), fillColorJson["g"].get<int>(), fillColorJson["b"].get<int>())
        );

        std::cout << "Adding Drawable component to entity: " << entity << std::endl;
        _registry.addComponent(entity, std::move(drawableComp));
        std::cout << "Drawable component added successfully!" << std::endl;
    }
}

void ServerEntityFactory::handleNetworkSync(entity_t entity, const nlohmann::json &entityJson, bool isShared)
{
    // Server does not handle UDP network synchronization
}

} // namespace ecs
