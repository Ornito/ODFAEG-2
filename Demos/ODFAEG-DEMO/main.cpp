#include "application.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stddef.h>
#include <vector>
using namespace odfaeg::core;
using namespace odfaeg::math;
using namespace odfaeg::physic;
using namespace odfaeg::graphic;
using namespace odfaeg::window;
using namespace odfaeg::audio;
using namespace sorrok;
using namespace std;
int main(int argc, char* argv[])
{
    EXPORT_CLASS_GUID(BoundingVolumeBoundingBox, BoundingVolume, BoundingBox)
    EXPORT_CLASS_GUID(EntityTile, Entity, Tile)
    EXPORT_CLASS_GUID(EntityTile, Entity, BigTile)
    EXPORT_CLASS_GUID(EntityWall, Entity, g2d::Wall)
    EXPORT_CLASS_GUID(EntityDecor, Entity, g2d::Decor)
    EXPORT_CLASS_GUID(EntityAnimation, Entity, Anim)
    EXPORT_CLASS_GUID(EntityHero, Entity, Hero)
    EXPORT_CLASS_GUID(EntityMesh, Entity, Mesh)
    MyAppli app(sf::VideoMode(800, 600), "Test odfaeg");
    return app.exec();
    /*RenderWindow window(sf::VideoMode(800, 600), "Test odfaeg", sf::Style::Default, ContextSettings(0, 0, 4, 3, 0));
    window.getView().move(0, 300, 0);
    Texture water;
    water.loadFromFile("tilesets/eau.png");
    std::vector<Tile*> tiles;
    std::vector<Tile*> walls;
    tiles.push_back(new Tile(&water, Vec3f(0, 0, 0), Vec3f(120, 60, 0),sf::IntRect(0, 0, 100, 50)));
    RenderComponentManager* rcm = new RenderComponentManager(window);
    Map* theMap = new Map (rcm, "Map test", 100, 50, 0);
    BaseChangementMatrix bm;
    bm.set2DIsoMatrix();
    theMap->setBaseChangementMatrix(bm);
    World::addEntityManager(theMap);
    World::setCurrentEntityManager("Map test");
    BoundingBox mapZone(0, 0, 0, 1500, 1000, 0);
    World::generate_map(tiles, walls, Vec2f(100, 50), mapZone, false);
    EntitiesUpdater* eu = new EntitiesUpdater();
    World::addWorker(eu);
    World::update();
    std::vector<Entity*> visibleEntities = World::getVisibleEntities("*");
    PerPixelLinkedListRenderComponent ppll (window, 0, "*", ContextSettings(0, 0, 4, 4, 6));
    ppll.loadEntitiesOnComponent(visibleEntities);
    ppll.clear();
    ppll.drawNextFrame();
    while (window.isOpen()) {
        IEvent event;
        while (window.pollEvent(event)) {
            if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {
                window.close();
            }
        }
        window.clear();
        window.draw(ppll);
        window.display();
    }
    return 0;*/
}



