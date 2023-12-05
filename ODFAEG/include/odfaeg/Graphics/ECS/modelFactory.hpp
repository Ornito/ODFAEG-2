#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP
#include "components.hpp"
#include "../../Core/ecs.hpp"
#include "../rectangleShape.h"
#include <GLFW/glfw3.h>
#include <set>
namespace odfaeg {
    namespace graphic {
        namespace ecs {
        class ModelFactory {
                public :
                static EntityId createTileModel(EntityFactory& entityFactory, const Texture *image, math::Vec3f position, math::Vec3f size, sf::IntRect subRect,  sf::Color color = sf::Color::White) {
                    EntityId tile = entityFactory.createEntity("E_TILE");
                    EntityInfoComponent eic;
                    eic.groupName = "E_TILE";
                    TransformComponent tc;
                    TransformMatrix tm;
                    tc.localBounds = physic::BoundingBox(0, 0, 0, size.x, size.y, size.z);
                    tc.position = position;
                    tc.origin = size * 0.5f;
                    tc.center = position + tc.origin;
                    tm.setOrigin(tc.origin);
                    tm.setRotation(math::Vec3f::zAxis, 0);
                    tm.setTranslation(tc.center);
                    math::Vec3f scale(1.f, 1.f, 1.f);
                    tm.setScale(scale);
                    tc.globalBounds = tc.localBounds.transform(tm);
                    tc.transformMatrix = tm;
                    MeshComponent mesh;
                    VertexArray va(sf::Quads, 4);
                    Vertex v1(sf::Vector3f(0, 0, 0), color);
                    Vertex v2(sf::Vector3f(size.x, 0, 0), color);
                    Vertex v3(sf::Vector3f(size.x, size.y, 0), color);
                    Vertex v4(sf::Vector3f(0, size.y, 0), color);
                    v1.texCoords = sf::Vector2f(subRect.left, subRect.top);
                    v2.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top);
                    v3.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top + subRect.height);
                    v4.texCoords = sf::Vector2f(subRect.left, subRect.top + subRect.height);
                    //std::cout<<"tex coords : "<<v2.texCoords.x<<" "<<v2.texCoords.y<<std::endl;
                    //v1.color = v2.color = v3.color = v4.color = color;


                    va[0] = v1;
                    va[1] = v2;
                    va[2] = v3;
                    va[3] = v4;
                    Material material;
                    material.addTexture(image, sf::IntRect(0, 0, 0, 0));

                    Face face(va, material, tm);
                    mesh.faces.push_back(face);
                    addComponent(tile, tc, entityFactory);
                    addComponent(tile, eic, entityFactory);
                    addComponent(tile, mesh, entityFactory);
                    return tile;
                }
                static EntityId createBigTileModel(EntityFactory& factory, math::Vec3f position) {
                    EntityId bigTile = factory.createEntity("E_BIGTILE");
                    EntityInfoComponent eic;
                    eic.groupName = "E_BIGTILE";
                    TransformMatrix tm;
                    TransformComponent tc;
                    tc.localBounds = physic::BoundingBox(0, 0, 0, 0, 0, 0);
                    tc.position = position;
                    tc.origin = tc.size * 0.5f;
                    tc.center = position + tc.origin;
                    tm.setOrigin(tc.origin);
                    tm.setRotation(math::Vec3f::zAxis, 0);
                    tm.setTranslation(tc.center);
                    math::Vec3f scale(1.f, 1.f, 1.f);
                    tm.setScale(scale);
                    tc.globalBounds = tc.localBounds.transform(tm);
                    tc.transformMatrix = tm;
                    addComponent(bigTile, eic, factory);
                    addComponent(bigTile, tc, factory);
                    return bigTile;
                }
            };
        }
    }
}
#endif
