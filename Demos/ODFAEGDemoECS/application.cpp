#include "application.hpp"
using namespace odfaeg::window;
using namespace odfaeg::core;
using namespace odfaeg::graphic;
using namespace odfaeg::graphic::ecs;
using namespace odfaeg::math;
using namespace odfaeg::physic;
namespace sorrok {
    MyECSAppli::MyECSAppli(sf::VideoMode vm, std::string title) : Application(vm, title, sf::Style::Default, ContextSettings(0, 0, 4, 4, 6)) {
        fpsCounter = 0;
        getView().move(0, 400, 400);
        addClock(sf::Clock(), "FPS");
    }
    void MyECSAppli::onLoad() {
        TextureManager<> tm;
        tm.fromFileWithAlias("tilesets/herbe.png", "GRASS");
        tm.fromFileWithAlias("tilesets/murs.png", "WALLS");
        cache.addResourceManager(tm, "TextureManager");
    }
    void MyECSAppli::onInit () {
        TextureManager<> &tm = cache.resourceManager<Texture, std::string>("TextureManager");
        EntityId tile = ModelFactory::createTileModel(factory, tm.getResourceByAlias("GRASS"), Vec3f(0, 0, 0), Vec3f(120, 60, 0),sf::IntRect(0, 0, 100, 50));
        std::vector<EntityId> tiles;
        tiles.push_back(tile);
        std::vector<EntityId> walls;
        walls.resize(WallType::NB_WALL_TYPES);
        EntityId tileWTopBot = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 0, 100, 100));
        walls[WallType::TOP_BOTTOM] = ModelFactory::createWallModel(factory, WallType::TOP_BOTTOM, tileWTopBot, *getWorld());

        EntityId tileWRightLeft = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 100, 100, 100));
        walls[WallType::RIGHT_LEFT] = ModelFactory::createWallModel(factory, WallType::RIGHT_LEFT, tileWRightLeft, *getWorld());

        EntityId tileWBotLeft = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 200, 100, 100));
        walls[WallType::BOTTOM_LEFT] = ModelFactory::createWallModel(factory, WallType::BOTTOM_LEFT, tileWBotLeft, *getWorld());

        EntityId tileWTopRight = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 300, 100, 100));
        walls[WallType::TOP_RIGHT] = ModelFactory::createWallModel(factory, WallType::TOP_RIGHT, tileWTopRight, *getWorld());

        EntityId tileWTopLeft = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 400, 100, 100));
        walls[WallType::TOP_LEFT] = ModelFactory::createWallModel(factory, WallType::TOP_LEFT, tileWTopLeft, *getWorld());

        EntityId tileWBotRight = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 500, 100, 100));
        walls[WallType::BOTTOM_RIGHT] = ModelFactory::createWallModel(factory, WallType::BOTTOM_RIGHT, tileWBotRight, *getWorld());

        scene = new Scene(&getRenderComponentManager(), "Map test", 100, 50, 0);
        BaseChangementMatrix bm;
        bm.set2DIsoMatrix();
        scene->setBaseChangementMatrix(bm);
        getWorld()->addSceneManager(scene);
        getWorld()->setCurrentSceneManager("Map test");
        eu = new EntitiesUpdater(factory, *getWorld());
        eu->setName("EntitiesUpdater");
        getWorld()->addWorker(eu);
        BoundingBox mapZone(0, 0, 0, 1500, 1000, 0);
        getWorld()->generate_map(tiles, walls, Vec2f(100, 50), mapZone, false, factory);
        PerPixelLinkedListRenderComponent *frc1 = new PerPixelLinkedListRenderComponent(getRenderWindow(),0, "E_BIGTILE", ContextSettings(0, 0, 4, 4, 6));
        PerPixelLinkedListRenderComponent *frc2 = new PerPixelLinkedListRenderComponent(getRenderWindow(),1, "E_WALL", ContextSettings(0, 0, 4, 4, 6));
        getRenderComponentManager().addECSComponent(frc1);
        getRenderComponentManager().addECSComponent(frc2);

    }
    void MyECSAppli::onRender(odfaeg::graphic::RenderComponentManager *cm) {
        getWorld()->drawOnComponents("E_BIGTILE", 0);
        getWorld()->drawOnComponents("E_WALL", 1);
    }
    void MyECSAppli::onDisplay(odfaeg::graphic::RenderWindow* window){
    }
    void MyECSAppli::onUpdate(RenderWindow* window, IEvent& event) {
        if (window == &getRenderWindow() && event.type == odfaeg::window::IEvent::WINDOW_EVENT && event.window.type == odfaeg::window::IEvent::WINDOW_EVENT_CLOSED) {
            stop();
        }
    }
    void MyECSAppli::onExec () {
        getWorld()->update();
        fpsCounter++;
        if (getClock("FPS").getElapsedTime() >= sf::seconds(1.f)) {
            std::cout<<"FPS : "<<fpsCounter<<std::endl;
            fpsCounter = 0;
            getClock("FPS").restart();
        }
    }
}
