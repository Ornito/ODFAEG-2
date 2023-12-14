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
        tm.fromFileWithAlias("tilesets/maison.png", "HOUSE");
        tm.fromFileWithAlias("tilesets/flemmes1.png", "FIRE1");
        tm.fromFileWithAlias("tilesets/flemmes2.png", "FIRE2");
        tm.fromFileWithAlias("tilesets/flemmes3.png", "FIRE3");
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
        walls[WallType::BOTTOM_RIGHT] = ModelFactory::createWallModel(factory, WallType::BOTTOM_RIGHT, tileWBotLeft, *getWorld());

        EntityId tileWTopRight = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 300, 100, 100));
        walls[WallType::TOP_LEFT] = ModelFactory::createWallModel(factory, WallType::TOP_LEFT, tileWTopRight, *getWorld());

        EntityId tileWTopLeft = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 400, 100, 100));
        walls[WallType::TOP_RIGHT] = ModelFactory::createWallModel(factory, WallType::TOP_RIGHT, tileWTopLeft, *getWorld());

        EntityId tileWBotRight = ModelFactory::createTileModel(factory, tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 500, 100, 100));
        walls[WallType::BOTTOM_LEFT] = ModelFactory::createWallModel(factory, WallType::BOTTOM_LEFT, tileWBotRight, *getWorld());

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
        getWorld()->generate_map(tiles, walls, Vec2f(100, 50), mapZone, factory);

        std::vector<EntityId> frames;
        EntityId tf1 = ModelFactory::createTileModel(factory, tm.getResourceByAlias("FIRE1"),  Vec3f(0, 100, 150), Vec3f(100, 100, 0),sf::IntRect(0, 0, 150 , 200));
        EntityId fire1 = ModelFactory::createDecorModel(factory, tf1, *getWorld());
        frames.push_back(fire1);

        EntityId tf2 = ModelFactory::createTileModel(factory, tm.getResourceByAlias("FIRE2"),  Vec3f(0, 100, 150), Vec3f(100, 100, 0),sf::IntRect(0, 0, 150 , 200));
        EntityId fire2 = ModelFactory::createDecorModel(factory, tf2, *getWorld());
        frames.push_back(fire2);

        EntityId tf3 = ModelFactory::createTileModel(factory, tm.getResourceByAlias("FIRE3"),  Vec3f(0, 100, 150), Vec3f(100, 100, 0),sf::IntRect(0, 0, 150 , 200));
        EntityId fire3 = ModelFactory::createDecorModel(factory, tf3, *getWorld());
        frames.push_back(fire3);

        EntityId animation = ModelFactory::createAnimationModel(factory, 0.1f, Vec3f(0, 0, 0), Vec3f(100, 100, 0), true, frames, *getWorld());
        AnimationUpdater* animUpdater = new AnimationUpdater(*getWorld(), factory);
        animUpdater->addAnim(animation);
        getWorld()->addTimer(animUpdater);
        getWorld()->addEntity(animation);

        PerPixelLinkedListRenderComponent *frc1 = new PerPixelLinkedListRenderComponent(getRenderWindow(),0, "E_BIGTILE", ContextSettings(0, 0, 4, 4, 6));
        PerPixelLinkedListRenderComponent *frc2 = new PerPixelLinkedListRenderComponent(getRenderWindow(),1, "E_WALL+E_DECOR", ContextSettings(0, 0, 4, 4, 6));
        getRenderComponentManager().addECSComponent(frc1);
        getRenderComponentManager().addECSComponent(frc2);

    }
    void MyECSAppli::onRender(odfaeg::graphic::RenderComponentManager *cm) {
        getWorld()->drawOnComponents("E_BIGTILE", 0);
        getWorld()->drawOnComponents("E_WALL+E_DECOR", 1);
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
