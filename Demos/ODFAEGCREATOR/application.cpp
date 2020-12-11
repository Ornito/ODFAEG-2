#include "application.hpp"
#include "odfaeg/Core/action.h"
#include "odfaeg/Core/command.h"
#include <sys/stat.h>
#include <stdlib.h>
#include "odfaeg/Core/utilities.h"
using namespace odfaeg::core;
using namespace odfaeg::math;
using namespace odfaeg::graphic;
using namespace odfaeg::graphic::gui;
using namespace odfaeg::graphic::g2d;
using namespace odfaeg::physic;
using namespace odfaeg::window;
ODFAEGCreator::ODFAEGCreator(sf::VideoMode vm, std::string title) :
Application (vm, title, sf::Style::Resize|sf::Style::Close, ContextSettings(0, 0, 0, 3, 0)), isGuiShown (false), cursor(10), se(this) {
    dpSelectTexture = nullptr;
    dpSelectEm = nullptr;
    sTextRect = nullptr;
    dpSelectPSU = nullptr;
    showGrid = false;
    alignToGrid = false;
    showRectSelect = false;
    gridWidth = 100;
    gridHeight = 50;
    appliname = "";
}
void ODFAEGCreator::onLoad() {
    FontManager<Fonts> fm;
    fm.fromFileWithAlias("fonts/Arial.ttf", Serif);
    TextureManager<> tm;
    cache.addResourceManager(fm, "FontManager");
    cache.addResourceManager(tm, "TextureManager");
}
void ODFAEGCreator::onInit() {
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    menuBar = new MenuBar(getRenderWindow());
    getRenderComponentManager().addComponent(menuBar);
    menu1 = new Menu(getRenderWindow(),fm.getResourceByAlias(Fonts::Serif),"File");
    getRenderComponentManager().addComponent(menu1);
    menu2 = new Menu(getRenderWindow(),fm.getResourceByAlias(Fonts::Serif),"Compile");
    getRenderComponentManager().addComponent(menu2);
    menu3 = new Menu(getRenderWindow(),fm.getResourceByAlias(Fonts::Serif),"Add");
    getRenderComponentManager().addComponent(menu3);
    menu4 = new Menu(getRenderWindow(),fm.getResourceByAlias(Fonts::Serif),"Edition");
    getRenderComponentManager().addComponent(menu4);
    menuBar->addMenu(menu1);
    menuBar->addMenu(menu2);
    menuBar->addMenu(menu3);
    menuBar->addMenu(menu4);
    item11 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"New application");
    item11->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item11);
    item12 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"New scene");
    item12->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item12);
    item13 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"New component");
    item13->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item13);
    item14 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "New entities updater");
    item14->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item14);
    item15 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Open application");
    item15->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item15);
    item16 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"New anim updater");
    item16->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item16);
    item17 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Save project");
    item17->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item17);
    item18 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "New particle system updater");
    item18->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item18);
    menu1->addMenuItem(item11);
    menu1->addMenuItem(item12);
    menu1->addMenuItem(item13);
    menu1->addMenuItem(item14);
    menu1->addMenuItem(item15);
    menu1->addMenuItem(item16);
    menu1->addMenuItem(item17);
    menu1->addMenuItem(item18);
    item21 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Build");
    item21->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item21);
    item22 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Run");
    item22->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item22);
    item23 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Build and run");
    item23->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item23);
    menu2->addMenuItem(item21);
    menu2->addMenuItem(item22);
    menu2->addMenuItem(item23);
    item31 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Rectangle shape");
    item31->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item31);
    item32 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Circle Shape");
    getRenderComponentManager().addComponent(item32);
    item33 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Sprite");
    getRenderComponentManager().addComponent(item33);
    item34 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Tile");
    getRenderComponentManager().addComponent(item34);
    item34->addMenuItemListener(this);
    item35 = new MenuItem(getRenderWindow(),fm.getResourceByAlias(Fonts::Serif),"Decor");
    getRenderComponentManager().addComponent(item35);
    item35->addMenuItemListener(this);
    item36 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Wall");
    getRenderComponentManager().addComponent(item36);
    item36->addMenuItemListener(this);
    item37 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Animation");
    getRenderComponentManager().addComponent(item37);
    item37->addMenuItemListener(this);
    item38 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Particle System");
    item38->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item38);
    item39 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Emitter");
    item39->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item39);
    item310 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Affector");
    item310->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item310);
    menu3->addMenuItem(item31);
    menu3->addMenuItem(item32);
    menu3->addMenuItem(item33);
    menu3->addMenuItem(item34);
    menu3->addMenuItem(item35);
    menu3->addMenuItem(item36);
    menu3->addMenuItem(item37);
    menu3->addMenuItem(item38);
    menu3->addMenuItem(item39);
    menu3->addMenuItem(item310);
    item41 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Undo");
    item41->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item41);
    item42 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif),"Redo");
    item42->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item42);
    item43 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Show grid");
    item43->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item43);
    item44 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Align to grid");
    item44->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item44);
    item45 = new MenuItem(getRenderWindow(), fm.getResourceByAlias(Fonts::Serif), "Rect selection");
    item45->addMenuItemListener(this);
    getRenderComponentManager().addComponent(item45);
    menu4->addMenuItem(item41);
    menu4->addMenuItem(item42);
    menu4->addMenuItem(item43);
    menu4->addMenuItem(item44);
    menu4->addMenuItem(item45);
    Action a1 (Action::EVENT_TYPE::KEY_HELD_DOWN, IKeyboard::Key::Z);
    Action a2 (Action::EVENT_TYPE::KEY_HELD_DOWN, IKeyboard::Key::Q);
    Action a3 (Action::EVENT_TYPE::KEY_HELD_DOWN, IKeyboard::Key::S);
    Action a4 (Action::EVENT_TYPE::KEY_HELD_DOWN, IKeyboard::Key::D);
    Action combined = a1 || a2 || a3 || a4;
    Command moveAction (combined, FastDelegate<void>(&ODFAEGCreator::processKeyHeldDown, this, IKeyboard::Unknown));
    getListener().connect("MoveAction", moveAction);
    fdTexturePath = new FileDialog(Vec3f(0, 0, 0), Vec3f(800, 600, 0), fm.getResourceByAlias(Fonts::Serif));
    fdTexturePath->setVisible(false);
    fdTexturePath->setEventContextActivated(false);
    addWindow(&fdTexturePath->getWindow(), false);
    getRenderComponentManager().addComponent(fdTexturePath);
    fdProjectPath = new FileDialog(Vec3f(0, 0, 0), Vec3f(800, 600, 0), fm.getResourceByAlias(Fonts::Serif));
    fdProjectPath->setVisible(false);
    fdProjectPath->setEventContextActivated(false);
    addWindow(&fdProjectPath->getWindow(), false);
    getRenderComponentManager().addComponent(fdProjectPath);
    wApplicationNew = new RenderWindow(sf::VideoMode(400, 300), "Create ODFAEG Application", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    //New application.
    Label* label = new Label(*wApplicationNew, Vec3f(0, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Application name : ", 15);
    getRenderComponentManager().addComponent(label);
    ta = new TextArea(Vec3f(200, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "", *wApplicationNew);
    getRenderComponentManager().addComponent(ta);
    Label* label2 = new Label(*wApplicationNew, Vec3f(0, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "Application type : ", 15);
    getRenderComponentManager().addComponent(label2);
    dpList = new DropDownList(*wApplicationNew, Vec3f(200, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Normal", 15);
    dpList->addItem("Client", 15);
    dpList->addItem("Server", 15);
    getRenderComponentManager().addComponent(dpList);
    lWidth = new Label(*wApplicationNew, Vec3f(0, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Width : ", 15);
    lHeight = new Label(*wApplicationNew, Vec3f(200, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Height : ", 15);
    getRenderComponentManager().addComponent(lWidth);
    getRenderComponentManager().addComponent(lHeight);
    taWidth = new TextArea(Vec3f(50, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"", *wApplicationNew);
    taHeight =  new TextArea(Vec3f(250, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"", *wApplicationNew);
    getRenderComponentManager().addComponent(taWidth);
    getRenderComponentManager().addComponent(taHeight);
    bCreateAppli = new Button(Vec3f(0, 200, 0), Vec3f(400, 100, 0), fm.getResourceByAlias(Fonts::Serif),"Create", 15, *wApplicationNew);
    bCreateAppli->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateAppli);
    addWindow(wApplicationNew);
    wApplicationNew->setVisible(false);
    getRenderComponentManager().setEventContextActivated(false, *wApplicationNew);
    //Create map window.
    wNewMap = new RenderWindow(sf::VideoMode(400, 300), "Create new scene", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    Label* labMapName = new Label(*wNewMap, Vec3f(0, 0, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Map name : ", 15);
    getRenderComponentManager().addComponent(labMapName);
    taMapName = new TextArea(Vec3f(200, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "", *wNewMap);
    getRenderComponentManager().addComponent(taMapName);
    Label* labMapType = new Label(*wNewMap, Vec3f(0, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "Application type : ", 15);
    getRenderComponentManager().addComponent(labMapType);
    dpMapTypeList = new DropDownList(*wNewMap, Vec3f(200, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Normal", 15);
    dpMapTypeList->addItem("2D iso", 15);
    getRenderComponentManager().addComponent(dpMapTypeList);
    lMapWidth = new Label(*wNewMap, Vec3f(0, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"case width : ", 15);
    lMapHeight = new Label(*wNewMap, Vec3f(200, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"case height : ", 15);
    getRenderComponentManager().addComponent(lMapWidth);
    getRenderComponentManager().addComponent(lMapHeight);
    taMapWidth = new TextArea(Vec3f(80, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"100", *wNewMap);
    taMapHeight =  new TextArea(Vec3f(280, 100, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"50", *wNewMap);
    getRenderComponentManager().addComponent(taMapWidth);
    getRenderComponentManager().addComponent(taMapHeight);
    bCreateScene = new Button(Vec3f(0, 200, 0), Vec3f(400, 100, 0), fm.getResourceByAlias(Fonts::Serif),"Create scene", 15, *wNewMap);
    bCreateScene->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateScene);
    addWindow(wNewMap);
    wNewMap->setVisible(false);
    getRenderComponentManager().setEventContextActivated(false, *wNewMap);
    //Create component.
    wNewComponent = new RenderWindow(sf::VideoMode(1000, 300), "Create new render component", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    Label* labComponentExpression = new Label(*wNewComponent,Vec3f(0, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "entity's type(s) : ", 15);
    getRenderComponentManager().addComponent(labComponentExpression);
    taComponentExpression = new TextArea(Vec3f(200, 0, 0),Vec3f(800, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewComponent);
    getRenderComponentManager().addComponent(taComponentExpression);
    Label* lComponentLayer = new Label(*wNewComponent,Vec3f(0, 50, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "Layer number : ", 15);
    getRenderComponentManager().addComponent(lComponentLayer);
    taComponentLayer = new TextArea(Vec3f(200, 50, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",*wNewComponent);
    getRenderComponentManager().addComponent(taComponentLayer);
    Label* lComponentType = new Label(*wNewComponent, Vec3f(0, 100, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "component type : ", 15);
    getRenderComponentManager().addComponent(lComponentType);
    dpComponentType = new DropDownList(*wNewComponent, Vec3f(200, 100, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif),"LinkedList", 15);
    dpComponentType->addItem("Shadow", 15);
    dpComponentType->addItem("Light", 15);
    dpComponentType->addItem("Refraction", 15);
    getRenderComponentManager().addComponent(dpComponentType);
    Label* lComponentName = new Label(*wNewComponent,Vec3f(0, 150, 0),Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif),"Name : ", 15);
    getRenderComponentManager().addComponent(lComponentName);
    taComponentName = new TextArea(Vec3f(200, 150, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "", *wNewComponent);
    getRenderComponentManager().addComponent(taComponentName);
    bCreateComponent = new Button(Vec3f(0, 200, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif),"Create component",15,*wNewComponent);
    bCreateComponent->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateComponent);
    addWindow(wNewComponent);
    wNewComponent->setVisible(false);
    getRenderComponentManager().setEventContextActivated(false, *wNewComponent);
    //Create entities updater.
    wNewEntitiesUpdater = new RenderWindow(sf::VideoMode(400, 300), "Create new entities updater", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    Label* lEntitiesUpdaterName = new Label (*wNewEntitiesUpdater,Vec3f(0, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "entities updater name : ", 15);
    getRenderComponentManager().addComponent(lEntitiesUpdaterName);
    taEntitiesUpdaterName = new TextArea(Vec3f(200, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEntitiesUpdater);
    getRenderComponentManager().addComponent(taEntitiesUpdaterName);
    bCreateEntitiesUpdater = new Button(Vec3f(200, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Create entities updater",15,*wNewEntitiesUpdater);
    bCreateEntitiesUpdater->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateEntitiesUpdater);
    addWindow(wNewEntitiesUpdater);
    wNewEntitiesUpdater->setVisible(false);
    getRenderComponentManager().setEventContextActivated(false, *wNewEntitiesUpdater);
    //Create animation updater.
    wNewAnimUpdater = new RenderWindow(sf::VideoMode(400, 300), "Create new anim updater", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    Label* lAnimUpdaterName = new Label (*wNewAnimUpdater,Vec3f(0, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "animation updater name : ", 15);
    getRenderComponentManager().addComponent(lAnimUpdaterName);
    taAnimUpdaterName = new TextArea(Vec3f(200, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewAnimUpdater);
    getRenderComponentManager().addComponent(taAnimUpdaterName);
    bCreateAnimUpdater = new Button(Vec3f(200, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Create anim updater",15,*wNewAnimUpdater);
    bCreateAnimUpdater->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateAnimUpdater);
    addWindow(wNewAnimUpdater);
    wNewAnimUpdater->setVisible(false);
    getRenderComponentManager().setEventContextActivated(false, *wNewAnimUpdater);
    //Create particle system updater.
    wNewParticleSystemUpdater = new RenderWindow(sf::VideoMode(400, 300), "Create new particle system updater", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    Label* lPartSysUpdName = new Label(*wNewParticleSystemUpdater,Vec3f(0, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "particle system updater name : ", 15);
    getRenderComponentManager().addComponent(lPartSysUpdName);
    taParticleSystemUpdaterName = new TextArea(Vec3f(200, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewParticleSystemUpdater);
    getRenderComponentManager().addComponent(taParticleSystemUpdaterName);
    bCreateParticleSystemUpdater = new Button(Vec3f(200, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Create ps updater",15,*wNewParticleSystemUpdater);
    bCreateParticleSystemUpdater->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateParticleSystemUpdater);
    addWindow(wNewParticleSystemUpdater);
    wNewParticleSystemUpdater->setVisible(false);
    getRenderComponentManager().setEventContextActivated(false, *wNewParticleSystemUpdater);
    //Create emitter for particle systems.
    wNewEmitter = new RenderWindow(sf::VideoMode(800, 800), "Create new emitter", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    //Particle system name.
    Label* lParticleSystemName = new Label(*wNewEmitter,Vec3f(0, 0, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "particle system name : ", 15);
    getRenderComponentManager().addComponent(lParticleSystemName);
    taPSName = new TextArea(Vec3f(200, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taPSName);
    //emission rate.
    Label* lEmmisionRate = new Label(*wNewEmitter,Vec3f(400, 0, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "emission rate : ", 15);
    getRenderComponentManager().addComponent(lEmmisionRate);
    taEmissionRate = new TextArea(Vec3f(600, 0, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taEmissionRate);
    //Life time.
    Label* lMinLifeTime = new Label(*wNewEmitter,Vec3f(0, 50, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "min life time : ", 15);
    getRenderComponentManager().addComponent(lMinLifeTime);
    taMinLifeTime = new TextArea(Vec3f(200, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taMinLifeTime);
    Label* lMaxLifeTime = new Label(*wNewEmitter,Vec3f(400, 50, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "max life time : ", 15);
    getRenderComponentManager().addComponent(lMaxLifeTime);
    taMaxLifeTime = new TextArea(Vec3f(600, 50, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taMaxLifeTime);
    //Particle position type.
    Label* ppType = new Label(*wNewEmitter, Vec3f(0, 100, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"particle pos type : ",15);
    getRenderComponentManager().addComponent(ppType);
    dpSelectPPType = new DropDownList(*wNewEmitter, Vec3f(200, 100, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Rect", 15);
    dpSelectPPType->addItem("Circle", 15);
    getRenderComponentManager().addComponent(dpSelectPPType);
    //Particle position rect x.
    Label* lrectPosX = new Label(*wNewEmitter,Vec3f(400, 100, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "x pos : ", 15);
    getRenderComponentManager().addComponent(lrectPosX);
    taRCPosX = new TextArea(Vec3f(600, 100, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRCPosX);
    Label* lrectPosY = new Label(*wNewEmitter,Vec3f(0, 150, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "y pos : ", 15);
    getRenderComponentManager().addComponent(lrectPosY);
    taRCPosY = new TextArea(Vec3f(200, 150, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRCPosY);
    Label* lrectPosZ = new Label(*wNewEmitter,Vec3f(400, 150, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "z pos : ", 15);
    getRenderComponentManager().addComponent(lrectPosZ);
    taRCPosZ = new TextArea(Vec3f(600, 150, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRCPosZ);
    //Particle position rect half size.
    Label* lrectHSX = new Label(*wNewEmitter,Vec3f(0, 200, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "half size x : ", 15);
    getRenderComponentManager().addComponent(lrectHSX);
    taRCSizeX = new TextArea(Vec3f(200, 200, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRCSizeX);
    Label* lrectHSY = new Label(*wNewEmitter,Vec3f(400, 200, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "half size y : ", 15);
    getRenderComponentManager().addComponent(lrectHSY);
    taRCSizeY = new TextArea(Vec3f(600, 200, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRCSizeY);
    Label* lrectHSZ = new Label(*wNewEmitter,Vec3f(0, 250, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "half size z : ", 15);
    getRenderComponentManager().addComponent(lrectHSZ);
    taRCSizeZ = new TextArea(Vec3f(200, 250, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRCSizeZ);
    //Deflect direction.
    Label* ldeflDirX = new Label(*wNewEmitter,Vec3f(400, 250, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "defl dir x : ", 15);
    getRenderComponentManager().addComponent(ldeflDirX);
    taDeflX = new TextArea(Vec3f(600, 250, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taDeflX);
    Label* ldeflDirY = new Label(*wNewEmitter,Vec3f(0, 300, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "defl dir y : ", 15);
    getRenderComponentManager().addComponent(ldeflDirY);
    taDeflY = new TextArea(Vec3f(200, 300, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taDeflY);
    Label* ldeflDirZ = new Label(*wNewEmitter,Vec3f(400, 300, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "defl dir z : ", 15);
    getRenderComponentManager().addComponent(ldeflDirZ);
    taDeflZ = new TextArea(Vec3f(600, 300, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taDeflZ);
    //Deflect angle.
    Label* ldeflAngle = new Label(*wNewEmitter,Vec3f(0, 350, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "defl angle : ", 15);
    getRenderComponentManager().addComponent(ldeflAngle);
    taDeflAngle = new TextArea(Vec3f(200, 350, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taDeflAngle);
    //Rotation.
    Label* lRotMin = new Label(*wNewEmitter,Vec3f(400, 350, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "min rotation angle : ", 15);
    getRenderComponentManager().addComponent(lRotMin);
    taRotMin = new TextArea(Vec3f(600, 350, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRotMin);
    Label* lRotMax = new Label(*wNewEmitter,Vec3f(0, 400, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "max rotation angle : ", 15);
    getRenderComponentManager().addComponent(lRotMax);
    taRotMax = new TextArea(Vec3f(200, 400, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taRotMax);
    //Texture index.
    Label* lTexMin = new Label(*wNewEmitter,Vec3f(400, 400, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "min texture index : ", 15);
    getRenderComponentManager().addComponent(lTexMin);
    taTexIndexMin = new TextArea(Vec3f(600, 400, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taTexIndexMin);
    Label* lTexMax = new Label(*wNewEmitter,Vec3f(0, 450, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "max texture index : ", 15);
    getRenderComponentManager().addComponent(lTexMax);
    taTexIndexMax = new TextArea(Vec3f(200, 450, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taTexIndexMax);
    //Min scale.
    Label* lminScaleX = new Label(*wNewEmitter,Vec3f(400, 450, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "min scale x : ", 15);
    getRenderComponentManager().addComponent(lminScaleX);
    taScaleMinX = new TextArea(Vec3f(600, 450, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taScaleMinX);
    Label* lminScaleY = new Label(*wNewEmitter,Vec3f(0, 500, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "min scale y : ", 15);
    getRenderComponentManager().addComponent(lminScaleY);
    taScaleMinY = new TextArea(Vec3f(200, 500, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taScaleMinY);
    Label* lminScaleZ = new Label(*wNewEmitter,Vec3f(400, 500, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "min scale z : ", 15);
    getRenderComponentManager().addComponent(lminScaleZ);
    taScaleMinZ = new TextArea(Vec3f(600, 500, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taScaleMinZ);
    //Max scale.
    Label* lmaxScaleX = new Label(*wNewEmitter,Vec3f(0, 550, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "max scale x : ", 15);
    getRenderComponentManager().addComponent(lmaxScaleX);
    taScaleMaxX = new TextArea(Vec3f(200, 550, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taScaleMaxX);
    Label* lmaxScaleY = new Label(*wNewEmitter,Vec3f(400, 550, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "max scale y : ", 15);
    getRenderComponentManager().addComponent(lmaxScaleY);
    taScaleMaxY = new TextArea(Vec3f(600, 550, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taScaleMaxY);
    Label* lmaxScaleZ = new Label(*wNewEmitter,Vec3f(0, 600, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "max scale z : ", 15);
    getRenderComponentManager().addComponent(lmaxScaleZ);
    taScaleMaxZ = new TextArea(Vec3f(200, 600, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taScaleMaxZ);
    //Color
    Label* lcolor1 = new Label(*wNewEmitter,Vec3f(400, 600, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "color 1 (r;g;b;a) ", 15);
    getRenderComponentManager().addComponent(lcolor1);
    taColor1 = new TextArea(Vec3f(600, 600, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taColor1);
    Label* lcolor2 = new Label(*wNewEmitter,Vec3f(0, 650, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif), "color 2 (r;g;b;a) : ", 15);
    getRenderComponentManager().addComponent(lcolor2);
    taColor2 = new TextArea(Vec3f(200, 650, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewEmitter);
    getRenderComponentManager().addComponent(taColor2);
    //Button.
    bCreateEmitter = new Button(Vec3f(400, 650, 0), Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Create emitter",15,*wNewEmitter);
    bCreateEmitter->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateEmitter);
    wNewEmitter->setVisible(false);
    addWindow(wNewEmitter);
    getRenderComponentManager().setEventContextActivated(false, *wNewEmitter);
    //Create panel for project files.
    pProjects = new Panel(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    rootNode = std::make_unique<Node> ("projects", pProjects, Vec2f(0.f, 0.015f), Vec2f(1.f / 6.f, 1.f));
    pProjects->setBorderColor(sf::Color(128, 128, 128));
    pProjects->setBackgroundColor(sf::Color::White);
    pProjects->setBorderThickness(5);
    unsigned int i = 0;
    Label* lab = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 35, 0),fm.getResourceByAlias(Fonts::Serif),"GUI", 15);
    lab->setBackgroundColor(sf::Color::White);
    Node* node = new Node("GUI",lab,Vec2f(0.f, 0.f),Vec2f(1.f, 0.05f),rootNode.get());
    lab->setForegroundColor(sf::Color::Red);
    lab->setParent(pProjects);
    pProjects->addChild(lab);
    Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
    Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, lab), FastDelegate<void>(&ODFAEGCreator::showGUI, this, lab));
    lab->getListener().connect("SHOWGUI", cmd);
    getRenderComponentManager().addComponent(pProjects);
    pScriptsEdit = new Panel(getRenderWindow(),Vec3f(200, 10, 0),Vec3f(800, 700, 0));
    pScriptsEdit->setRelPosition(1.f / 6.f, 0.015f);
    pScriptsEdit->setRelSize(0.60f, 0.75f);
    pScriptsEdit->setBorderColor(sf::Color(128, 128, 128));
    pScriptsEdit->setBackgroundColor(sf::Color::White);
    pScriptsEdit->setBorderThickness(5);
    pScriptsEdit->setName("PSCRIPTEDIT");
    getRenderComponentManager().addComponent(pScriptsEdit);
    pScriptsFiles = new Panel(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pScriptsFiles->setBorderColor(sf::Color(128, 128, 128));
    pScriptsFiles->setBackgroundColor(sf::Color::White);
    pScriptsFiles->setBorderThickness(5);
    pScriptsFiles->setRelPosition(5.f / 6.5f, 0.015f);
    pScriptsFiles->setRelSize(1.5f / 6.f, 1.f);
    pScriptsFiles->setName("PSCRIPTFILES");
    getRenderComponentManager().addComponent(pScriptsFiles);
    pComponent = new Panel(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(500, 200, 0), 0);
    pComponent->setBorderColor(sf::Color(128, 128, 128));
    pComponent->setBackgroundColor(sf::Color::White);
    pComponent->setBorderThickness(5);
    pComponent->setRelPosition(1.f / 6.f, 0.75f);
    pComponent->setRelSize(1.f - 1.f / 6.f - 1.39f / 6.f, 0.25f);

    getRenderComponentManager().addComponent(pComponent);
    dpSelectComponent = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"MAIN WINDOW", 15);
    dpSelectComponent->setRelPosition(0, 0);
    dpSelectComponent->setRelSize(0.3, 0.1);
    dpSelectComponent->setParent(pComponent);
    pComponent->addChild(dpSelectComponent);
    Command cmdSelectComponent (FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectComponent), FastDelegate<void>(&ODFAEGCreator::onSelectedComponentChanged, this, dpSelectComponent));
    dpSelectComponent->getListener().connect("SELECTCOMPONENT", cmdSelectComponent);
    taChangeComponentExpression = new TextArea(Vec3f(0, 0, 0),Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif),"change component expression", getRenderWindow());
    taChangeComponentExpression->setRelPosition(0.3, 0);
    taChangeComponentExpression->setRelSize(0.7, 0.1);
    taChangeComponentExpression->setParent(pComponent);
    taChangeComponentExpression->setName("TaChangeComponentExpression");
    pComponent->addChild(taChangeComponentExpression);
    Command cmdCEChanged(FastDelegate<bool>(&TextArea::isTextChanged, taChangeComponentExpression), FastDelegate<void>(&ODFAEGCreator::onComponentExpressionChanged, this, taChangeComponentExpression));
    taChangeComponentExpression->getListener().connect("CECHANGED", cmdCEChanged);
    tabPane = new TabPane(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 700, 0));
    tabPane->setRelPosition(0, 0);
    tabPane->setRelSize(1, 1);
    tabPane->setParent(pScriptsFiles);
    pScriptsFiles->addChild(tabPane);
    pScriptsFiles->setBackgroundColor(sf::Color::Black);
    pInfos = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pInfos->setBackgroundColor(sf::Color::White);
    //pInfos->setScissorEnabled(false);
    rootInfosNode = std::make_unique<Node>("Infos", pInfos, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pInfos, "Informations", *fm.getResourceByAlias(Fonts::Serif));
    pTransform = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pTransform->setBackgroundColor(sf::Color::White);
    //pTransform->setScissorEnabled(false);
    rootPropNode = std::make_unique<Node>("Properties", pTransform, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pTransform, "Transform", *fm.getResourceByAlias(Fonts::Serif));
    pMaterial = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pMaterial->setBackgroundColor(sf::Color::White);
    //pMaterial->setScissorEnabled(false);
    rootMaterialNode = std::make_unique<Node>("Material", pMaterial, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pMaterial,"Material",*fm.getResourceByAlias(Fonts::Serif));
    pShadows = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pShadows->setBackgroundColor(sf::Color::White);
    //pShadows->setScissorEnabled(false);
    rootShadowsNode = std::make_unique<Node>("Shadows", pShadows, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pShadows,"Shadow",*fm.getResourceByAlias(Fonts::Serif));
    tScriptEdit = new TextArea(Vec3f(200, 20, 0),Vec3f(790,650,0),fm.getResourceByAlias(Fonts::Serif),"",getRenderWindow());
    tScriptEdit->setParent(pScriptsEdit);
    tScriptEdit->setRelPosition(0.f, 0.f);
    tScriptEdit->setRelSize(0.9f, 0.9f);
    pScriptsEdit->addChild(tScriptEdit);
    guiSize.x = getRenderWindow().getSize().x - pProjects->getSize().x - pScriptsFiles->getSize().x;
    guiSize.y = getRenderWindow().getSize().y - menuBar->getSize().y;
    guiPos.x = pProjects->getSize().x;
    guiPos.y = menuBar->getSize().y;
    cursor.setOutlineColor(sf::Color::Red);
    cursor.setOutlineThickness(5);
    cursor.setFillColor(sf::Color::Transparent);
    Action moveCursorAction (Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
    Command moveCursorCommand (moveCursorAction, FastDelegate<void>(&ODFAEGCreator::moveCursor, this, sf::Vector2f(-1, -1)));
    getListener().connect("MoveCursor", moveCursorCommand);
    cshapes.clear();
    for (int i = 0; i < getRenderWindow().getSize().x; i+=gridWidth) {
        for (int j = 0; j < getRenderWindow().getSize().y; j+=gridHeight) {
            ConvexShape cshape(4);
            cshape.setFillColor(sf::Color::Transparent);
            cshape.setOutlineColor(sf::Color(75, 75, 75));
            cshape.setOutlineThickness(1.f);
            Vec2f points[4];
            points[0] = Vec2f(0, 0);
            points[1] = Vec2f(gridWidth, 0);
            points[2] = Vec2f(gridWidth, gridHeight);
            points[3] = Vec2f(0, gridHeight);
            for (unsigned int n = 0; n < 4; n++) {
                points[n] += Vec2f(i, j);
                cshape.setPoint(n, sf::Vector3f(points[n].x, points[n].y, 0));
            }
            cshapes.push_back(cshape);
        }
    }
}
void ODFAEGCreator::updateScriptText(Shape* shape, const Texture* text) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        std::string relativePath = tm.getPathByResource(text);
        unsigned int id = tm.getIdByResource(text);
        if(content.find("text"+conversionUIntString(id)) == std::string::npos) {
            unsigned int pos = content.find("TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");");
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
            content.insert(pos, "const Texture* text"+conversionUIntString(id)+" = tm.getResourceByAlias(\"+realtivePath+\");\n");
        }
        if (content.find("shape"+conversionUIntString(shape->getId())+"->setTexture") == std::string::npos) {
            unsigned int pos = content.find("shape"+conversionUIntString(shape->getId()));
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
            content.insert(pos,"shape"+conversionUIntString(shape->getId())+"->setTexture(text"+conversionUIntString(id)+");\n");
        } else {
            unsigned int pos = content.find("shape"+conversionUIntString(shape->getId())+"->setTexture");
            std::string subs = content.substr(pos);
            unsigned int endpos = subs.find_first_of('\n') + pos + 1;
            content.erase(pos, endpos - pos);
            content.insert(pos,"shape"+conversionUIntString(shape->getId())+"->setTexture(text"+conversionUIntString(id)+");\n");
        }
    }
}
void ODFAEGCreator::updateScriptText(Tile* tile, const Texture* text) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        std::string relativePath = tm.getPathByResource(text);
        unsigned int id = tm.getIdByResource(text);
        if(content.find("text"+conversionUIntString(id)) == std::string::npos) {
            unsigned int pos = content.find("TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");");
            if (pos != std::string::npos) {
                std::string subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
                content.insert(pos, "const Texture* text"+conversionUIntString(id)+" = tm.getResourceByAlias(\"+realtivePath+\");\n");
            }
        }
        if (content.find("tile"+conversionUIntString(tile->getId())+"->getFace(0)->getMaterial()") == std::string::npos) {
            unsigned int pos = content.find("tile"+conversionUIntString(tile->getId()));
            if (pos != std::string::npos) {
                std::string subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
                content.insert(pos,"tile"+conversionUIntString(tile->getId())+"->getFace(0)->getMaterial().clearTextures();\n"+
                               "tile->getFace(0)->getMaterial().addTexture(text"+conversionUIntString(id)+", sf::IntRect(0, 0,"+
                               conversionIntString(text->getSize().x)+","+conversionIntString(text->getSize().y)+"));\n");
            }
        } else {
            unsigned int pos = content.find("tile"+conversionUIntString(tile->getId())+"->getFace(0).getMaterial().addTexture");
            if (pos != std::string::npos) {
                std::string subs = content.substr(pos);
                unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                content.erase(pos, endpos - pos);
                content.insert(pos,"tile->getFace(0)->getMaterial().addTexture(text"+conversionUIntString(id)+", sf::IntRect(0, 0,"+
                               conversionIntString(text->getSize().x)+","+conversionIntString(text->getSize().y)+"));\n");
            }
        }
    }
}
void ODFAEGCreator::onRender(RenderComponentManager *cm) {

}
void ODFAEGCreator::onDisplay(RenderWindow* window) {
    if (window == &getRenderWindow()) {
        for (unsigned int i = 0; i < shapes.size(); i++)
            window->draw(*shapes[i]);
        View currentView = window->getView();
        View defaultView = window->getDefaultView();
        window->setView(defaultView);
        if (isGuiShown) {
            window->draw(cursor);
            if (showGrid) {
                window->getView().move(getRenderWindow().getSize().x * 0.5f, 300, 0);
                for (unsigned int  i = 0; i < cshapes.size(); i++)
                    window->draw(cshapes[i]);
            }
        }
        window->setView(defaultView);
        if (showRectSelect) {
            window->draw(rectSelect);
        }
        window->setView(currentView);

    }
}
Vec3f ODFAEGCreator::getGridCellPos(Vec3f pos) {
    BaseChangementMatrix bcm = World::getBaseChangementMatrix();
    Vec3f c = bcm.unchangeOfBase(pos);
    Vec3f v1, p;
    p.x = (int) c.x / gridWidth;
    p.y = (int) c.y / gridHeight;
    if (c.x < 0)
        p.x--;
    if (c.y < 0)
        p.y--;
    v1.x = p.x * gridWidth;
    v1.y = p.y * gridHeight;
    Vec3f ve1(v1.x, v1.y, 0);
    Vec3f ve2(v1.x + gridWidth, v1.y, 0);
    Vec3f ve3(v1.x + gridWidth, v1.y + gridHeight, 0);
    Vec3f ve4(v1.x, v1.y + gridHeight, 0);
    Vec3f p1 = bcm.changeOfBase(ve1);
    Vec3f p2 = bcm.changeOfBase(ve2);
    Vec3f p3 = bcm.changeOfBase(ve3);
    Vec3f p4 = bcm.changeOfBase(ve4);
    std::vector<Vec3f> points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    std::array<std::array<float, 2>, 3> extends = Computer::getExtends(points);
    return Vec3f (extends[0][0], extends[1][0], extends[2][0]);
}
void ODFAEGCreator::onUpdate(RenderWindow* window, IEvent& event) {
    if (&getRenderWindow() == window && event.type == IEvent::KEYBOARD_EVENT && event.keyboard.type == IEvent::KEY_EVENT_PRESSED)
        getListener().setCommandSlotParams("MoveAction", this, static_cast<IKeyboard::Key>(event.keyboard.code));
    if (&getRenderWindow() == window && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {
        stop();
    }
    if (&getRenderWindow() == window && event.type == IEvent::MOUSE_BUTTON_EVENT && event.mouseButton.type == IEvent::BUTTON_EVENT_PRESSED && event.mouseButton.button == IMouse::Left) {
        sf::Vector2f mousePos (event.mouseButton.x, event.mouseButton.y);
        getListener().setCommandSlotParams("MoveCursor", this, mousePos);
    }
    if (IKeyboard::isKeyPressed(IKeyboard::D)) {
        View view = getRenderWindow().getView();
        view.move(speed * getClock("LoopTime").getElapsedTime().asSeconds(), 0, 0);
        getRenderWindow().setView(view);
    }
    if (IKeyboard::isKeyPressed(IKeyboard::Q)) {
        View view = getRenderWindow().getView();
        view.move(-speed * getClock("LoopTime").getElapsedTime().asSeconds(), 0, 0);
        getRenderWindow().setView(view);
    }
    if (IKeyboard::isKeyPressed(IKeyboard::Z)) {
        View view = getRenderWindow().getView();
        view.move(0, speed * getClock("LoopTime").getElapsedTime().asSeconds(), 0);
        getRenderWindow().setView(view);
    }
    if (IKeyboard::isKeyPressed(IKeyboard::S)) {
        View view = getRenderWindow().getView();
        view.move(0, -speed * getClock("LoopTime").getElapsedTime().asSeconds(), 0);
        getRenderWindow().setView(view);
    }
    if (&getRenderWindow() == window && event.type == IEvent::MOUSE_BUTTON_EVENT && event.mouseButton.type == IEvent::BUTTON_EVENT_PRESSED && event.mouseButton.button == IMouse::Right) {
        sf::Vector2f mousePos (event.mouseButton.x, event.mouseButton.y);
        if (showRectSelect && !pScriptsFiles->isPointInside(Vec3f(mousePos.x, mousePos.y, 0))) {
            if (alignToGrid) {
                if (World::getCurrentEntityManager() == nullptr) {
                    int x = ((int) mousePos.x / gridWidth * gridWidth) - getRenderWindow().getSize().x * 0.5f;
                    int y = ((int) mousePos.y / gridHeight * gridHeight) - getRenderWindow().getSize().y * 0.5f;
                    mousePosition = Vec3f(x, y, 0);
                } else {
                    int x = mousePos.x - getRenderWindow().getSize().x * 0.5f;
                    int y = mousePos.y - getRenderWindow().getSize().y * 0.5f;
                    mousePosition = getGridCellPos(Vec3f(x, y, 0));
                }
            } else {
                int x = mousePos.x-getRenderWindow().getSize().x * 0.5f;
                int y = mousePos.y-getRenderWindow().getSize().y * 0.5f;
                mousePosition = Vec3f(x, y, 0);
            }
        } else if (pScriptsFiles->isPointInside(Vec3f(mousePos.x, mousePos.y, 0))) {
            if (alignToGrid) {
                int x = ((int) mousePos.x / gridWidth * gridWidth);
                int y = ((int) mousePos.y / gridHeight * gridHeight);
                mousePosition = Vec3f(x, y, 0);
            } else {
                int x = mousePos.x;
                int y = mousePos.y;
                mousePosition = Vec3f(x, y, 0);
            }
        }
    }
    if (&getRenderWindow() == window && event.type == IEvent::MOUSE_MOTION_EVENT && IMouse::isButtonPressed(IMouse::Right)) {
        sf::Vector2f mousePos (event.mouseMotion.x, event.mouseMotion.y);
        if (showRectSelect && !pScriptsFiles->isPointInside(mousePosition)) {
            if (alignToGrid) {
                if (World::getCurrentEntityManager() == nullptr) {
                    int x = ((int) mousePos.x / gridWidth * gridWidth) - getRenderWindow().getSize().x * 0.5f;
                    int y = ((int) mousePos.y / gridHeight * gridHeight) - getRenderWindow().getSize().y * 0.5f;
                    int width = x - mousePosition.x;
                    int height = y - mousePosition.y;
                    if (width > 0 && height > 0)
                        rectSelect.setRect(mousePosition.x, mousePosition.y, width, height);
                } else {
                    int x = mousePos.x - getRenderWindow().getSize().x * 0.5f;
                    int y = mousePos.y - getRenderWindow().getSize().y * 0.5f;
                    Vec3f pos = getGridCellPos(Vec3f(x, y, 0));
                    int width = pos.x - mousePosition.x;
                    int height = pos.y - mousePosition.y;
                    if (width > 0 && height > 0)
                        rectSelect.setRect(mousePosition.x, mousePosition.y, width, height);
                }

            } else {
                int x = mousePos.x-getRenderWindow().getSize().x * 0.5f;
                int y = mousePos.y-getRenderWindow().getSize().y * 0.5f;
                int width = x - mousePosition.x;
                int height = y - mousePosition.y;
                if (width > 0 && height > 0)
                    rectSelect.setRect(mousePosition.x, mousePosition.y, width, height);
            }
        } else if (pScriptsFiles->isPointInside(mousePosition)) {
            if (alignToGrid) {
                int x = ((int) mousePos.x / gridWidth * gridWidth);
                int y = ((int) mousePos.y / gridHeight * gridHeight);
                int width = x - mousePosition.x;
                int height = y - mousePosition.y;
                if (width > 0 && height > 0 && sTextRect != nullptr) {
                    sTextRect->setPosition(Vec3f(mousePosition.x, mousePosition.y, 0));
                    sTextRect->setSize(Vec3f(width, height, 0));
                }
            } else {
                int x = mousePos.x;
                int y = mousePos.y;
                int width = x - mousePosition.x;
                int height = y - mousePosition.y;
                if (width > 0 && height > 0 && sTextRect != nullptr) {
                    sTextRect->setPosition(Vec3f(mousePosition.x, mousePosition.y, 0));
                    sTextRect->setSize(Vec3f(width, height, 0));
                }
            }
        }
    }
    if (&getRenderWindow() == window && event.type == IEvent::MOUSE_BUTTON_EVENT && event.mouseButton.type == IEvent::BUTTON_EVENT_RELEASED && event.mouseButton.button == IMouse::Right) {
        if (showRectSelect && !pScriptsFiles->isPointInside(mousePosition)) {
            std::vector<std::pair<Transformable*, sf::Color>> colors = rectSelect.getColors();
            for (unsigned int i = 0; i < colors.size(); i++) {
                if (dynamic_cast<Tile*>(colors[i].first)) {
                    static_cast<Tile*>(colors[i].first)->setColor(colors[i].second);
                }
                if (dynamic_cast<Shape*>(colors[i].first)) {
                    static_cast<Shape*>(colors[i].first)->setFillColor(colors[i].second);
                }
            }
            rectSelect.getItems().clear();
            BoundingBox box = rectSelect.getSelectionRect();
            Vec3f savedPos = box.getPosition();
            Vec3f pos = box.getPosition();
            pos = getRenderWindow().mapPixelToCoords(Vec3f(pos.x, getRenderWindow().getSize().y-pos.y, 0))+getRenderWindow().getView().getSize()*0.5f;
            rectSelect.setRect(pos.x, pos.y, box.getSize().x, box.getSize().y);
            if (World::getCurrentEntityManager() != nullptr) {
                std::vector<Entity*> entities = World::getVisibleEntities("*");
                for (unsigned int i = 0; i < entities.size(); i++) {
                    if (rectSelect.getSelectionRect().intersects(entities[i]->getGlobalBounds())) {
                        if (dynamic_cast<Tile*>(entities[i])) {
                            rectSelect.addItem(entities[i], static_cast<Tile*>(entities[i])->getColor());
                            static_cast<Tile*>(entities[i])->setColor(sf::Color::Yellow);
                        }
                    }
                }
            }
            for (unsigned int i = 0; i < shapes.size(); i++) {
                if (rectSelect.getSelectionRect().intersects(shapes[i]->getGlobalBounds())) {
                    if (dynamic_cast<Shape*>(shapes[i].get())) {
                        rectSelect.addItem(shapes[i].get(), static_cast<Shape*>(shapes[i].get())->getFillColor());
                        static_cast<Shape*>(shapes[i].get())->setFillColor(sf::Color::Yellow);
                    }
                }
            }
            if (rectSelect.getItems().size() > 0)
                selectedObject = rectSelect.getItems()[0];
            if (dynamic_cast<Tile*>(selectedObject)) {
                displayInfos(static_cast<Tile*>(selectedObject));
            }
            rectSelect.setRect(savedPos.x, savedPos.y, box.getSize().x, box.getSize().y);
        }
        if (pScriptsFiles->isPointInside(mousePosition) && sTextRect != nullptr) {
            int x = sTextRect->getPosition().x - pScriptsFiles->getPosition().x;
            int y = sTextRect->getPosition().y - (bChooseText->getPosition().y + bChooseText->getSize().y);
            tTexCoordX->setText(conversionIntString(x));
            tTexCoordY->setText(conversionIntString(y));
            tTexCoordW->setText(conversionIntString(sTextRect->getSize().x));
            tTexCoordH->setText(conversionIntString(sTextRect->getSize().y));
        }
    }
    if (wApplicationNew == window && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {
        wApplicationNew->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wApplicationNew);
        tScriptEdit->setEventContextActivated(true);
    }
    if (wNewEntitiesUpdater == window && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {
        wNewEntitiesUpdater->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewEntitiesUpdater);
        tScriptEdit->setEventContextActivated(true);
    }
    if (&getRenderWindow() == window && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_RESIZED) {
        getRenderWindow().getDefaultView().reset(BoundingBox(0, 0, getRenderWindow().getDefaultView().getPosition().z, event.window.data1, event.window.data2, getRenderWindow().getDefaultView().getDepth()));
        getRenderWindow().getDefaultView().setPerspective(-event.window.data1 * 0.5f, event.window.data1 * 0.5f, -event.window.data2 * 0.5f, event.window.data2 * 0.5f, getRenderWindow().getDefaultView().getViewport().getPosition().z, getRenderWindow().getDefaultView().getViewport().getSize().z);
        View view = getRenderWindow().getView();
        view.reset(BoundingBox(0, 0, getRenderWindow().getView().getPosition().z, event.window.data1, event.window.data2, getRenderWindow().getView().getDepth()));
        view.setPerspective(-event.window.data1 * 0.5f, event.window.data1 * 0.5f, -event.window.data2 * 0.5f, event.window.data2 * 0.5f, getRenderWindow().getView().getViewport().getPosition().z, getRenderWindow().getView().getViewport().getSize().z);
        getRenderWindow().setView(view);
    }
}
void ODFAEGCreator::onExec() {
    std::string path = fdTexturePath->getPathChosen();
    if (path != "") {
        unsigned int lastSlash;
        #if defined(ODFAEG_SYSTEM_LINUX)
        lastSlash = path.find_last_of("/");
        #else if defined (ODFAEG_SYSTEM_WINDOWS)
        lastSlash = path.find_last_of("\\");
        #endif // if
        std::string ImgName = path.substr(lastSlash+1);
        dpSelectTexture->addItem(ImgName,15);
        fdTexturePath->setVisible(false);
        fdTexturePath->setEventContextActivated(false);
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        tm.fromFileWithAlias(path, ImgName);
        textPaths.push_back(ImgName);
        std::map<std::string, std::string>::iterator it;
        it = cppAppliContent.find(minAppliname+".cpp");
        if (it != cppAppliContent.end()) {
            std::string& content = it->second;
            unsigned int pos = content.find("TextureManager<>& tm");
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
            content.insert(pos, "    tm.fromFileWithAlias("+path+","+ImgName+");");
        }
        fdTexturePath->setEventContextActivated(false);
        tScriptEdit->setEventContextActivated(true);
    }
    if (dpSelectTexture != nullptr && dpSelectTexture->isDroppedDown()) {
        bChooseText->setEventContextActivated(false);
    } else if (dpSelectTexture != nullptr && !dpSelectTexture->isDroppedDown()) {
        bChooseText->setEventContextActivated(true);
    }
    std::string projectPath = fdProjectPath->getPathChosen();
    if (projectPath != "") {
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        std::ifstream applis(projectPath);
        std::string line;
        if (applis) {
            if(getline(applis, line)) {
                Label* lab = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),line, 15);
                Node* node = new Node("test",lab,Vec2f(0, 0),Vec2f(1.f, 0.025f),rootNode.get());
                lab->setParent(pProjects);
                lab->setForegroundColor(sf::Color::Red);
                lab->setBackgroundColor(sf::Color::White);
                pProjects->addChild(lab);
                Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
                Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, lab), FastDelegate<void>(&ODFAEGCreator::showProjectsFiles, this, lab));
                lab->getListener().connect("SHOWPFILES", cmd);
                appliname = line;
            }
        }
        const char* lcars = appliname.c_str();
        char* ucars = new char[appliname.size()];
        for (unsigned int i = 0; i < appliname.length(); i++) {
            ucars[i] = std::tolower(lcars[i]);
        }
        minAppliname = std::string(ucars, appliname.length());
        if (appliname != "") {
            while(getline(applis, line)) {
                #if defined (ODFAEG_SYSTEM_LINUX)
                std::string path = fdProjectPath->getAppiDir() + "/" + appliname + "/" + line;
                #else if defined (ODFAEG_SYSTEM_WINDOWS
                std::string path = fdProjectPath->getAppiDir() + "\\" + appliname + "\\" + line;
                #endif // if
                std::ifstream source (path);
                std::string fileContent;
                std::string line2;
                while(getline(source, line2)) {
                    fileContent += line2+"\n";
                }
                source.close();
                cppAppliContent.insert(std::make_pair(line, fileContent));
            }
        }
        applis.close();
        //Load textures.
        std::ifstream ifs(appliname+"\\"+"textures.oc");
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        if (ifs) {
            std::cout<<"textures : "<<std::endl;
            ITextArchive ia(ifs);
            std::vector<std::string> paths;
            ia(paths);
            for (unsigned int i = 0; i < paths.size(); i++) {
                std::cout<<"load texture : "<<paths[i]<<std::endl;
                unsigned int lastSlash;
                #if defined(ODFAEG_SYSTEM_LINUX)
                lastSlash = paths[i].find_last_of("/");
                #else if defined (ODFAEG_SYSTEM_WINDOWS)
                lastSlash = paths[i].find_last_of("\\");
                #endif // if
                std::string ImgName = paths[i].substr(lastSlash+1);
                std::cout<<"alias : "<<ImgName<<std::endl;
                tm.fromFileWithAlias(paths[i], ImgName);
            }
            ifs.close();
        }
        std::ifstream ifs3(appliname+"\\"+"entities.oc");
        std::vector<Entity*> entities;
        if (ifs3) {
            ITextArchive ia3(ifs3);
            ia3(entities);
            for (unsigned int i = 0; i < entities.size(); i++) {
                std::cout<<"load entities"<<std::endl;
                for (unsigned int f = 0; f < entities[i]->getNbFaces(); f++) {
                    Face* face = entities[i]->getFace(f);
                    std::string alias = face->getMaterial().getTexId();
                    if (alias != "") {
                        face->getMaterial().clearTextures();
                        face->getMaterial().addTexture(tm.getResourceByAlias(alias), face->getMaterial().getTexRect());
                    } else {
                        face->getMaterial().clearTextures();
                        face->getMaterial().addTexture(nullptr, sf::IntRect(0, 0, 0, 0));
                    }
                }
            }
            ifs3.close();
        }
        std::ifstream ifs2(appliname+"\\"+"scenes.oc");
        unsigned int size;
        if (ifs2) {
            ITextArchive ia2(ifs2);
            ia2(size);
            for (unsigned int i = 0; i < size; i++) {
                std::string name;
                ia2(name);
                std::string type;
                ia2(type);
                int cellWidth;
                int cellHeight;
                bool is2DIsoMatrix;
                if (type == "Map") {
                    std::cout<<"load map"<<std::endl;
                    ia2(cellWidth);
                    ia2(cellHeight);
                    Map* scene = new Map(&getRenderComponentManager(),name,cellWidth,cellHeight,0);
                    ia2(is2DIsoMatrix);
                    BaseChangementMatrix bcm;
                    if (is2DIsoMatrix) {
                        bcm.set2DIsoMatrix();
                    }
                    scene->setBaseChangementMatrix(bcm);
                    World::addEntityManager(scene);
                    World::setCurrentEntityManager(name);
                    for (unsigned int e = 0; e < entities.size(); e++) {
                        std::cout<<"add entity"<<std::endl;
                        World::addEntity(entities[e]);
                    }
                }
            }
            ifs2.close();
        }
        std::ifstream ifs4(appliname+"\\"+"timers.oc");
        if (ifs4) {
            ITextArchive ia4(ifs4);
            ia4(size);
            for (unsigned int i  = 0; i < size; i++) {
                std::string name;
                std::string type;
                ia4(name);
                ia4(type);
                if (type == "AnimationUpdater") {
                    AnimUpdater* au = new AnimUpdater();
                    au->setName(name);
                    std::vector<int> animsIds;
                    ia4(animsIds);
                    for (unsigned int a = 0; a < animsIds.size(); a++) {
                        Entity* entity = World::getEntity(animsIds[a]);
                        if (entity != nullptr && dynamic_cast<Anim*>(entity)) {
                            au->addAnim(static_cast<Anim*>(entity));
                        }
                    }
                    World::addTimer(au);
                }
            }
            ifs4.close();
        }
        std::ifstream ifs5(appliname+"\\"+"components.oc");
        if (ifs5) {
            ITextArchive ia5(ifs5);
            ia5(size);
            for (unsigned int i = 0; i < size; i++) {
                std::string name;
                std::string type;
                ia5(name);
                ia5(type);
                if (type == "PerPixelLinkedList") {
                    std::cout<<"load components"<<std::endl;
                    int layer;
                    ia5(layer);
                    std::string expression;
                    ia5(expression);
                    std::cout<<"layer : "<<layer<<std::endl;
                    PerPixelLinkedListRenderComponent* ppll = new PerPixelLinkedListRenderComponent(getRenderWindow(),layer,expression,ContextSettings(0, 0, 4, 4, 6));
                    ppll->setName(name);
                    getRenderComponentManager().addComponent(ppll);
                    dpSelectComponent->addItem(name, 15);
                }
            }
            ifs5.close();
        }
        std::ifstream ifs6(appliname+"\\"+"workers.oc");
        if (ifs6) {
            ITextArchive ia6(ifs6);
            ia6(size);
            for (unsigned int i = 0; i < size; i++) {
                std::string name;
                std::string type;
                ia6(name);
                ia6(type);
                if (type == "EntityUpdater") {
                    std::cout<<"load entities updater"<<std::endl;
                    EntitiesUpdater* eu = new EntitiesUpdater();
                    eu->setName(name);
                    World::addWorker(eu);
                }
            }
            ifs6.close();
        }
        fdProjectPath->setVisible(false);
        fdProjectPath->setEventContextActivated(false);
        tScriptEdit->setEventContextActivated(true);
    }
    World::update();
}
void ODFAEGCreator::showGUI(Label* label) {
    isGuiShown = true;
    pScriptsEdit->setVisible(false);
}
void ODFAEGCreator::showProjectsFiles(Label* label) {
    isGuiShown = false;
    pScriptsEdit->setVisible(true);
    Node* node = rootNode->findNode(label);
    if (node->getNodes().size() == 0) {
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        std::vector<LightComponent*> children = pProjects->getChildren();
        Label* lHeaders = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200,17,0),fm.getResourceByAlias(Fonts::Serif),"headers", 15);
        Label* lSources = new Label(getRenderWindow(),Vec3f(0, 0,0),Vec3f(200,17,0),fm.getResourceByAlias(Fonts::Serif),"sources", 15);
        lHeaders->setBackgroundColor(sf::Color::White);
        lSources->setBackgroundColor(sf::Color::White);
        lHeaders->setParent(pProjects);
        Node* hNode = new Node ("headers", lHeaders, Vec2f(0, 0), Vec2f(1.f, 0.025f), node);
        pProjects->addChild(lHeaders);
        lSources->setParent(pProjects);
        Node* sNode = new Node("sources",lSources,Vec2f(0, 0), Vec2f(1.f, 0.025f), node);
        pProjects->addChild(lSources);
        lHeaders->setForegroundColor(sf::Color::Green);
        lSources->setForegroundColor(sf::Color::Green);
        pProjects->setAutoResized(true);
        Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
        Command cmd1(a, FastDelegate<bool>(&Label::isMouseInside, lHeaders), FastDelegate<void>(&ODFAEGCreator::showHeadersFiles, this, lHeaders));
        lHeaders->getListener().connect("SHOWHFILES", cmd1);
        Command cmd2(a, FastDelegate<bool>(&Label::isMouseInside, lSources), FastDelegate<void>(&ODFAEGCreator::showSourcesFiles, this, lSources));
        lSources->getListener().connect("SHOWSFILES", cmd2);
    } else if (!node->isNodeVisible()) {
        node->showAllNodes();
    } else {
        node->hideAllNodes();
    }
}
void ODFAEGCreator::showHeadersFiles(Label* label) {
    std::cout<<"show header files"<<std::endl;
    Node* node = rootNode->findNode(label);
    if (node->getNodes().size() == 0) {
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        std::vector<std::string> files;
        std::string cpath = getCurrentPath();
        #if defined (ODFAEG_SYSTEM_LINUX)
        findFiles("hpp", files, cpath+"/"+appliname);
        #else if defined (ODFAEG_SYSTEM_WINDOWS)
        findFiles("hpp", files, cpath+"\\"+appliname);
        #endif // if
        for (unsigned int i = 0; i < files.size(); i++) {
            Label* lab = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), files[i], 15);
            lab->setParent(pProjects);
            lab->setBackgroundColor(sf::Color::White);
            lab->setForegroundColor(sf::Color::Yellow);
            Node* lnode = new Node("header files", lab, Vec2f(0, 0),Vec2f(1,0.025f),node);
            pProjects->addChild(lab);
            Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
            Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, lab), FastDelegate<void>(&ODFAEGCreator::showFileContent, this, lab));
            lab->getListener().connect("SHOWHFILECONTENT"+lab->getText(), cmd);
        }
    } else if (!node->isNodeVisible()) {
        node->showAllNodes();
    } else {
        node->hideAllNodes();
    }
}
void ODFAEGCreator::showSourcesFiles(Label* label) {
     Node* node = rootNode->findNode(label);
     if (node->getNodes().size() == 0) {
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        std::vector<std::string> files;
        std::string cpath = getCurrentPath();
        #if defined (ODFAEG_SYSTEM_LINUX)
        findFiles("cpp", files, cpath+"/"+appliname);
        #else if defined (ODFAEG_SYSTEM_WINDOWS)
        findFiles("cpp", files, cpath+"\\"+appliname);
        #endif // if
        for (unsigned int i = 0; i < files.size(); i++) {
            Label* lab = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), files[i], 15);
            Node* lNode = new Node("source files", lab, Vec2f(0, 0), Vec2f(1.f, 0.025f), node);
            lab->setBackgroundColor(sf::Color::White);
            lab->setForegroundColor(sf::Color::Yellow);
            lab->setParent(pProjects);
            pProjects->addChild(lab);
            Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
            Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, lab), FastDelegate<void>(&ODFAEGCreator::showFileContent, this, lab));
            lab->getListener().connect("SHOWHFILECONTENT"+lab->getText(), cmd);
        }
    } else if (!node->isNodeVisible()){
        node->showAllNodes();
    } else {
        node->hideAllNodes();
    }
}
void ODFAEGCreator::showFileContent(Label* lab) {
    isGuiShown = false;
    pScriptsEdit->setVisible(true);
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(lab->getText());
    if (it != cppAppliContent.end()) {
        tScriptEdit->setTextSize(20);
        tScriptEdit->setText(it->second);
        Vec3f textSize = tScriptEdit->getTextSize();
        if (textSize.x > tScriptEdit->getSize().x)
            tScriptEdit->setSize(Vec3f(textSize.x, tScriptEdit->getSize().y, tScriptEdit->getSize().z));
        if (textSize.y > tScriptEdit->getSize().y)
            tScriptEdit->setSize(Vec3f(tScriptEdit->getSize().x, textSize.y, tScriptEdit->getSize().z));
        pScriptsEdit->updateScrolls();
    }
}
void ODFAEGCreator::processKeyHeldDown (IKeyboard::Key key) {

}
void ODFAEGCreator::actionPerformed(Button* button) {
    if (button->getText() == "Create") {
        appliname = ta->getText();
        std::ofstream applis(appliname+".oc");
        applis<<appliname<<std::endl;
        applitype = dpList->getSelectedItem();
        #if defined (ODFAEG_SYSTEM_LINUX)
        path = fdProjectPath->getAppiDir() + "/" + appliname;
        #else if defined (ODFAEG_SYSTEM_WINDOWS)
        path = fdProjectPath->getAppiDir() + "\\" + appliname;
        #endif // if
        const char* lcars = appliname.c_str();
        char* ucars = new char[appliname.size()];
        for (unsigned int i = 0; i < appliname.length(); i++) {
            ucars[i] = std::toupper(lcars[i]);
        }
        std::string majAppliname (ucars, appliname.length());
        delete ucars;
        ucars = new char[appliname.size()];
        for (unsigned int i = 0; i < appliname.length(); i++) {
            ucars[i] = std::tolower(lcars[i]);
        }
        minAppliname = std::string(ucars, appliname.length());
        if(mkdir(path.c_str()/*, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH*/) == -1) {
            std::cerr<<"Failed to create application directory!";
            std::cerr << "Error: " << strerror(errno);
        }
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        wApplicationNew->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wApplicationNew);
        tScriptEdit->setEventContextActivated(true);
        Label* lab = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),appliname, 15);
        Node* node = new Node("test",lab,Vec2f(0, 0),Vec2f(1.f, 0.025f),rootNode.get());
        lab->setParent(pProjects);
        lab->setForegroundColor(sf::Color::Red);
        lab->setBackgroundColor(sf::Color::White);
        pProjects->addChild(lab);
        Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
        Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, lab), FastDelegate<void>(&ODFAEGCreator::showProjectsFiles, this, lab));
        lab->getListener().connect("SHOWPFILES", cmd);
        if (applitype == "Normal") {
            #if defined (ODFAEG_SYSTEM_LINUX)
            std::ofstream header(path+"/"+minAppliname+".hpp");
            #else if defined (ODFAEG_SYSTEM_WINDOWS)
            std::ofstream header(path+"\\"+minAppliname+".hpp");
            #endif // if
            std::ostringstream oss;
            oss<<"#ifndef "<<majAppliname<<"_HPP"<<std::endl;
            oss<<"#define "<<majAppliname<<"_HPP"<<std::endl;
            oss<<"#include \"odfaeg/Core/application.h\""<<std::endl;
            oss<<"class "<<appliname<<" : public odfaeg::core::Application {"<<std::endl;
            oss<<"   public : "<<std::endl;
            oss<<"   "<<appliname<<"(sf::VideoMode vm, std::string title);"<<std::endl;
            oss<<"   void onLoad();"<<std::endl;
            oss<<"   void onInit();"<<std::endl;
            oss<<"   void onRender(odfaeg::graphic::RenderComponentManager* cm);"<<std::endl;
            oss<<"   void onDisplay(odfaeg::graphic::RenderWindow* window);"<<std::endl;
            oss<<"   void onUpdate (odfaeg::graphic::RenderWindow*, odfaeg::window::IEvent& event);"<<std::endl;
            oss<<"   void onExec ();"<<std::endl;
            oss<<"   private : "<<std::endl;
            oss<<"   std::vector<std::unique_ptr<Drawable>> drawables;"<<std::endl;
            oss<<"   ResourceCache<> cache;"<<std::endl;
            oss<<"};"<<std::endl;
            oss<<"#endif"<<std::endl;
            header<<oss.str();
            header.close();
            cppAppliContent.insert(std::make_pair(minAppliname+".hpp", oss.str()));
            oss.str("");
            oss.clear();
            #if defined (ODFAEG_SYSTEM_LINUX)
            std::ofstream source(path+"/"+minAppliname+".cpp");
            #else if defined (ODFAEG_SYSTEM_WINDOWS)
            std::ofstream source(path+"\\"+minAppliname+".cpp");
            #endif // if
            oss<<"#include \""+minAppliname+".hpp\""<<std::endl;
            oss<<"using namespace odfaeg::graphic;"<<std::endl;
            oss<<"using namespace odfaeg::math;"<<std::endl;
            oss<<"using namespace odfaeg::window;"<<std::endl;
            oss<<appliname<<"::"<<appliname<<"(sf::VideoMode vm, std::string title) : "<<std::endl;
            oss<<"Application (vm, title, sf::Style::Resize|sf::Style::Close, ContextSettings(0, 0, 0, 3, 0)) {"<<std::endl;
            oss<<"}"<<std::endl;
            oss<<"void "<<appliname<<"::onLoad() {"<<std::endl;
            oss<<"  TextureManager<> tm;"<<std::endl;
            oss<<"  cache.addResourceManager(tm, \"TextureManager\");"<<std::endl;
            oss<<"}"<<std::endl;
            oss<<"void "<<appliname<<"::onInit() {"<<std::endl;
            oss<<"  TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");"<<std::endl;
            oss<<"}"<<std::endl;
            oss<<"void "<<appliname<<"::onRender(RenderComponentManager *cm) {"<<std::endl;
            oss<<"}"<<std::endl;
            oss<<"void "<<appliname<<"::onDisplay(RenderWindow* window) {"<<std::endl;
            oss<<"   if (&getRenderWindow() == window) {"<<std::endl;
            oss<<"       for (unsigned int i = 0; i < drawables.size(); i++) {"<<std::endl;
            oss<<"           window->draw(*drawable[i]);"<<std::endl;
            oss<<"       }"<<std::endl;
            oss<<"   }"<<std::endl;
            oss<<"}"<<std::endl;
            oss<<"void "<<appliname<<"::onUpdate (RenderWindow* window, IEvent& event) {"<<std::endl;
            oss<<" if (&getRenderWindow() == window && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {"<<std::endl;
            oss<<"  stop();"<<std::endl;
            oss<<" }"<<std::endl;
            oss<<"}"<<std::endl;
            oss<<"void "<<appliname<<"::onExec () {"<<std::endl;
            oss<<"}"<<std::endl;
            source<<oss.str();
            source.close();
            cppAppliContent.insert(std::make_pair(minAppliname+".cpp", oss.str()));
            oss.str("");
            oss.clear();
            std::string width = taWidth->getText();
            std::string height = taHeight->getText();
            #if defined (ODFAEG_SYSTEM_LINUX)
            std::ofstream main(path+"/main.cpp");
            #else if defined (ODFAEG_SYSTEM_WINDOWS)
            std::ofstream main(path+"\\main.cpp");
            #endif // if
            oss<<"#include \""<<minAppliname<<".hpp\""<<std::endl;
            oss<<"int main(int argc, char* argv[]) {"<<std::endl;
            oss<<"    "<<appliname<<" app(sf::VideoMode("<<width<<","<<height<<"),\""<<appliname<<"\");"<<std::endl;
            oss<<"    return app.exec();"<<std::endl;
            oss<<"}"<<std::endl;
            main<<oss.str();
            main.close();
            cppAppliContent.insert(std::make_pair("main.cpp", oss.str()));
            applis<<minAppliname+".hpp"<<std::endl;
            applis<<minAppliname+".cpp"<<std::endl;
            applis<<"main.cpp"<<std::endl;
            applis.close();
        }
    }
    if (button->getText() == "New texture") {
        fdTexturePath->setVisible(true);
        fdTexturePath->setEventContextActivated(true);
        tScriptEdit->setEventContextActivated(false);
    }
    if (button == bAddTexRect) {
        sf::IntRect texRect;
        if (is_number(tTexCoordX->getText()))
            texRect.left = conversionStringInt(tTexCoordX->getText());
        if (is_number(tTexCoordY->getText()))
            texRect.top = conversionStringInt(tTexCoordY->getText());
        if (is_number(tTexCoordW->getText()))
            texRect.width = conversionStringInt(tTexCoordW->getText());
        if (is_number(tTexCoordH->getText()))
            texRect.height = conversionStringInt(tTexCoordH->getText());
        if (dynamic_cast<ParticleSystem*>(selectedObject)) {
            static_cast<ParticleSystem*>(selectedObject)->addTextureRect(texRect);
        }
    }
    if (button->getText() == "Create scene") {
        wNewMap->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewMap);
        tScriptEdit->setEventContextActivated(true);
        BaseChangementMatrix bcm;
        if (dpMapTypeList->getSelectedItem() == "2D iso")
            bcm.set2DIsoMatrix();
        theMap = new Map(&getRenderComponentManager(), taMapName->getText(), conversionStringInt(taMapWidth->getText()), conversionStringInt(taMapHeight->getText()), 0);
        theMap->setBaseChangementMatrix(bcm);
        World::addEntityManager(theMap);
        World::setCurrentEntityManager(taMapName->getText());
        cshapes.clear();
        for (int i = 0; i < getRenderWindow().getSize().x; i+=100) {
            for (int j = 0; j < getRenderWindow().getSize().y; j+=50) {
                ConvexShape cshape(4);
                cshape.setFillColor(sf::Color::Transparent);
                cshape.setOutlineColor(sf::Color(75, 75, 75));
                cshape.setOutlineThickness(1.f);
                Vec2f points[4];
                points[0] = Vec2f(0, 0);
                points[1] = Vec2f(100, 0);
                points[2] = Vec2f(100, 50);
                points[3] = Vec2f(0, 50);
                for (unsigned int n = 0; n < 4; n++) {
                    points[n] = bcm.changeOfBase(points[n]);
                    points[n] += Vec2f(i, j);
                    cshape.setPoint(n, sf::Vector3f(points[n].x, points[n].y, 0));
                }
                cshapes.push_back(cshape);
            }
        }
        if (dpSelectEm != nullptr) {
            dpSelectEm->addItem(taMapName->getText(), 15);
        }
    }
    if (button->getText() == "Create component") {
        wNewComponent->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewComponent);
        tScriptEdit->setEventContextActivated(true);
        std::cout<<"Create component"<<std::endl;
        if (dpComponentType->getSelectedItem() == "LinkedList") {
            PerPixelLinkedListRenderComponent* ppll = new PerPixelLinkedListRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 4, 6));
            getRenderComponentManager().addComponent(ppll);
            ppll->setName(taComponentName->getText());
            dpSelectComponent->addItem(taComponentName->getText(), 15);
        }
        if (dpComponentType->getSelectedItem() == "Shadow") {
            ShadowRenderComponent* src = new ShadowRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 4, 6));
            getRenderComponentManager().addComponent(src);
            src->setName(taComponentName->getText());
            dpSelectComponent->addItem(taComponentName->getText(), 15);
        }
        if (dpComponentType->getSelectedItem() == "Light") {
            LightRenderComponent* lrc = new LightRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 4, 6));
            getRenderComponentManager().addComponent(lrc);
            lrc->setName(taComponentName->getText());
            dpSelectComponent->addItem(taComponentName->getText(), 15);
        }
        if (dpComponentType->getSelectedItem() == "Refraction") {
            ReflectRefractRenderComponent* rrrc = new ReflectRefractRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 4, 6));
            getRenderComponentManager().addComponent(rrrc);
            rrrc->setName(taComponentName->getText());
            dpSelectComponent->addItem(taComponentName->getText(), 15);
        }
    }
    if(button==bCreateEntitiesUpdater) {
        std::string name = taEntitiesUpdaterName->getText();
        EntitiesUpdater* eu = new EntitiesUpdater();
        eu->setName(name);
        World::addWorker(eu);
        wNewEntitiesUpdater->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewEntitiesUpdater);
        tScriptEdit->setEventContextActivated(true);
    }
    if(button==bCreateAnimUpdater) {
        std::string name = taAnimUpdaterName->getText();
        AnimUpdater* au = new AnimUpdater();
        au->setName(name);
        World::addTimer(au);
        wNewAnimUpdater->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewAnimUpdater);
        tScriptEdit->setEventContextActivated(true);
        if (dpSelectAU != nullptr) {
            dpSelectAU->addItem(name, 15);
        }
    }
    if (button==bCreateEmitter) {
        std::string psName = taPSName->getText();
        emitterParams.push_back(taPSName->getText());
        UniversalEmitter emitter;
        emitter.setEmissionRate(conversionStringFloat(taEmissionRate->getText()));
        emitterParams.push_back(taEmissionRate->getText());
        emitter.setParticleLifetime(Distributions::uniform(sf::seconds(conversionStringFloat(taMinLifeTime->getText())), sf::seconds(conversionStringFloat(taMaxLifeTime->getText()))));
        emitterParams.push_back(taMinLifeTime->getText());
        emitterParams.push_back(taMaxLifeTime->getText());
        if (dpSelectPPType->getSelectedItem() == "Rect") {
            emitter.setParticlePosition(Distributions::rect(Vec3f(conversionStringFloat(taRCPosX->getText()), conversionStringFloat(taRCPosY->getText()), conversionStringFloat(taRCPosZ->getText())),
                                                           Vec3f(conversionStringFloat(taRCSizeX->getText()), conversionStringFloat(taRCSizeY->getText()), conversionStringFloat(taRCSizeZ->getText()))));
            emitterParams.push_back("Rect");
            emitterParams.push_back(taRCPosX->getText());
            emitterParams.push_back(taRCPosY->getText());
            emitterParams.push_back(taRCPosZ->getText());
            emitterParams.push_back(taRCSizeX->getText());
            emitterParams.push_back(taRCSizeY->getText());
            emitterParams.push_back(taRCSizeZ->getText());
        } else {
            emitter.setParticlePosition(Distributions::circle(Vec3f(conversionStringFloat(taRCPosX->getText()), conversionStringFloat(taRCPosY->getText()), conversionStringFloat(taRCPosZ->getText())),
                                                             conversionStringFloat(taRCSizeX->getText())));
            emitterParams.push_back("Circle");
            emitterParams.push_back(taRCPosX->getText());
            emitterParams.push_back(taRCPosY->getText());
            emitterParams.push_back(taRCPosZ->getText());
            emitterParams.push_back(taRCSizeX->getText());
        }
        emitter.setParticleVelocity(Distributions::deflect(Vec3f(conversionStringFloat(taDeflX->getText()), conversionStringFloat(taDeflY->getText()), conversionStringFloat(taDeflZ->getText())), conversionStringFloat(taDeflAngle->getText())));
        emitterParams.push_back(taDeflX->getText());
        emitterParams.push_back(taDeflY->getText());
        emitterParams.push_back(taDeflZ->getText());
        emitterParams.push_back(taDeflAngle->getText());
        emitter.setParticleRotation(Distributions::uniform(conversionStringFloat(taRotMin->getText()), conversionStringFloat(taRotMax->getText())));
        emitterParams.push_back(taRotMin->getText());
        emitterParams.push_back(taRotMax->getText());
        emitter.setParticleTextureIndex(Distributions::uniformui(conversionStringInt(taTexIndexMin->getText()),conversionStringInt(taTexIndexMax->getText())));
        emitterParams.push_back(taTexIndexMin->getText());
        emitterParams.push_back(taTexIndexMax->getText());
        emitter.setParticleScale(Distributions::rect(Vec3f(conversionStringFloat(taScaleMinX->getText()), conversionStringFloat(taScaleMinY->getText()), conversionStringFloat(taScaleMinZ->getText())),
                                                           Vec3f(conversionStringFloat(taScaleMaxX->getText()), conversionStringFloat(taScaleMaxY->getText()), conversionStringFloat(taScaleMaxZ->getText()))));
        emitterParams.push_back(taScaleMinX->getText());
        emitterParams.push_back(taScaleMinY->getText());
        emitterParams.push_back(taScaleMinZ->getText());
        emitterParams.push_back(taScaleMaxX->getText());
        emitterParams.push_back(taScaleMaxY->getText());
        emitterParams.push_back(taScaleMaxZ->getText());
        std::vector<std::string> color1 = split(taColor1->getText(), ";");
        std::vector<std::string> color2 = split(taColor2->getText(), ";");
        Vec3f c1(conversionStringInt(color1[0]), conversionStringInt(color1[1]), conversionStringInt(color1[2]), conversionStringInt(color1[3]));
        Vec3f c2(conversionStringInt(color2[0]), conversionStringInt(color2[1]), conversionStringInt(color2[2]), conversionStringInt(color2[3]));
        emitter.setParticleColor(Distributions::color(c1, c2));
        emitterParams.push_back(taColor1->getText());
        emitterParams.push_back(taColor2->getText());
        Entity* ps = World::getEntity(psName);
        if (dynamic_cast<ParticleSystem*>(ps)) {
            std::cout<<"add emitter"<<std::endl;
            static_cast<ParticleSystem*>(ps)->addEmitter(emitter);
        }
        wNewEmitter->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewEmitter);
        tScriptEdit->setEventContextActivated(true);
    }
    if (button == bCreateParticleSystemUpdater) {
        std::cout<<"add particle system"<<std::endl;
        std::string name = taParticleSystemUpdaterName->getText();
        ParticleSystemUpdater* psu = new ParticleSystemUpdater();
        psu->setName(name);
        World::addWorker(psu);
        wNewParticleSystemUpdater->setVisible(false);
        getRenderComponentManager().setEventContextActivated(false, *wNewParticleSystemUpdater);
        tScriptEdit->setEventContextActivated(true);
        if (dpSelectPSU != nullptr) {
            dpSelectPSU->addItem(name, 15);
        }
    }
    if (button == bAddTexRect) {
        std::cout<<"add text rect"<<std::endl;
        sf::IntRect rect;
        rect.left = conversionStringInt(tTexCoordX->getText());
        rect.top = conversionStringInt(tTexCoordY->getText());
        rect.width = conversionStringInt(tTexCoordW->getText());
        rect.height = conversionStringInt(tTexCoordH->getText());
        if (dynamic_cast<ParticleSystem*>(selectedObject)) {
            static_cast<ParticleSystem*>(selectedObject)->addTextureRect(rect);
        }
    }
}
void ODFAEGCreator::actionPerformed(MenuItem* item) {
    if (item->getText() == "New application") {
        std::cout<<"new application"<<std::endl;
        wApplicationNew->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wApplicationNew);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item->getText() == "Build") {
        std::string command = "clang++-3.6 -Wall -std=c++14 -g "+appliname+"/"+"*.cpp -o "+appliname+"/"+minAppliname+".out "
        "/usr/local/lib/libodfaeg-network-s-d.a /usr/local/lib/libodfaeg-audio-s-d.a /usr/local/lib/libodfaeg-graphics-s-d.a "
        "/usr/local/lib/libodfaeg-physics-s-d.a /usr/local/lib/libodfaeg-math-s-d.a /usr/local/lib/libodfaeg-core-s-d.a "
        "/usr/local/lib/libsfml-audio.so /usr/local/lib/libsfml-network.so /usr/local/lib/libsfml-graphics.so /usr/local/lib/libsfml-window.so "
        "/usr/local/lib/libsfml-system.so /usr/lib/x86_64-linux-gnu/libfreetype.so /usr/lib/x86_64-linux-gnu/libpthread.so /usr/lib/x86_64-linux-gnu/libGLEW.so "
        "-lGL -lssl -lcrypto /usr/lib/x86_64-linux-gnu/libSDL2.so 2> errors.err";
        std::system(command.c_str());
    }
    if (item->getText() == "Run") {
        std::string command = std::string("./"+appliname+"/"+minAppliname+".out");
        std::system(command.c_str());
    }
    if (item->getText() == "Build and run") {
        std::string command = "clang++-3.6 -Wall -std=c++14 -g "+appliname+"/"+"*.cpp -o "+appliname+"/"+minAppliname+".out "
        "/usr/local/lib/libodfaeg-network-s-d.a /usr/local/lib/libodfaeg-audio-s-d.a /usr/local/lib/libodfaeg-graphics-s-d.a "
        "/usr/local/lib/libodfaeg-physics-s-d.a /usr/local/lib/libodfaeg-math-s-d.a /usr/local/lib/libodfaeg-core-s-d.a "
        "/usr/local/lib/libsfml-audio.so /usr/local/lib/libsfml-network.so /usr/local/lib/libsfml-graphics.so /usr/local/lib/libsfml-window.so "
        "/usr/local/lib/libsfml-system.so /usr/lib/x86_64-linux-gnu/libfreetype.so /usr/lib/x86_64-linux-gnu/libpthread.so /usr/lib/x86_64-linux-gnu/libGLEW.so "
        "-lGL -lssl -lcrypto /usr/lib/x86_64-linux-gnu/libSDL2.so /usr/lib/x86_64-linux-gnu/libgmp.so 2> errors.err";
        std::system(command.c_str());
        command = std::string("./"+appliname+"/"+minAppliname+".out");
        std::system(command.c_str());
    }
    if (item->getText() == "Rectangle shape") {
        if (appliname != "") {
            std::unique_ptr<RectangleShape> shape = std::make_unique<RectangleShape>(Vec3f(100, 50, 0));
            Vec3f position = getRenderWindow().mapPixelToCoords(Vec3f(cursor.getPosition().x, getRenderWindow().getSize().y - cursor.getPosition().y, 0))+getRenderWindow().getView().getSize()*0.5f;
            shape->setPosition(position);
            displayInfos(shape.get());
            selectedObject = shape.get();
            addShape(shape.get());
            shapes.push_back(std::move(shape));
        }
    }
    if (item->getText() == "Tile") {
        if (appliname != "" && World::getCurrentEntityManager() != nullptr) {
            if (!showRectSelect) {
                Vec3f position = getRenderWindow().mapPixelToCoords(Vec3f(cursor.getPosition().x, getRenderWindow().getSize().y - cursor.getPosition().y, 0))+getRenderWindow().getView().getSize()*0.5f;
                Tile* tile = new Tile(nullptr, position,Vec3f(100, 50, 0), sf::IntRect(0, 0, gridWidth, gridHeight));
                selectedObject = tile;
                displayInfos(tile);
                World::addEntity(tile);
                addTile(tile);
            } else {
                BoundingBox rect = rectSelect.getSelectionRect();
                Vec3f savedPos = rectSelect.getSelectionRect().getPosition();
                Vec3f pos = rectSelect.getSelectionRect().getPosition();
                pos = getRenderWindow().mapPixelToCoords(Vec3f(pos.x, getRenderWindow().getSize().y - pos.y, 0))+getRenderWindow().getView().getSize()*0.5f;
                rectSelect.setRect(pos.x, pos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
                //In 2D iso the tiles are in a staggered arrangement so we need to shift the x position every two times in the loop.
                if (World::getBaseChangementMatrix().isIso2DMatrix()) {
                    int i = 0;
                    int x = rect.getPosition().x;
                    int y = rect.getPosition().y;
                    int endX = rect.getPosition().x + rect.getSize().x;
                    int endY = rect.getPosition().y + rect.getSize().y;
                    while (y <= endY) {
                        int offsetX;
                        if (i%2==0)
                            offsetX = 0;
                        else
                            offsetX = gridWidth*0.5f;
                        x = rect.getPosition().x+offsetX;
                        while (x <= endX) {
                            Vec3f position = getGridCellPos(Vec3f(x, y, 0));
                            Tile* tile = new Tile(nullptr,position,Vec3f(gridWidth, gridHeight, 0),sf::IntRect(0, 0, gridWidth, gridHeight));
                            rectSelect.addItem(tile, tile->getColor());
                            if (rectSelect.getItems().size() == 1) {
                                selectedObject = tile;
                                displayInfos(tile);
                            }
                            World::addEntity(tile);
                            x += gridWidth;
                        }
                        y += gridHeight*0.5f;
                        i++;
                    }
                } else {
                    for (int x = rect.getPosition().x; x < rect.getPosition().x + rect.getSize().x; x+=gridWidth) {
                        for (int y = rect.getPosition().y; y <  rect.getPosition().y + rect.getSize().y; y+=gridHeight) {
                            Vec3f position = getGridCellPos(Vec3f(x, y, 0));
                            Tile* tile = new Tile(nullptr,position,Vec3f(gridWidth, gridHeight, 0),sf::IntRect(0, 0, gridWidth, gridHeight));
                            rectSelect.addItem(tile, tile->getColor());
                            if (rectSelect.getItems().size() == 1) {
                                selectedObject = tile;
                                displayInfos(tile);
                            }
                            World::addEntity(tile);
                        }
                    }
                }
                rectSelect.setRect(savedPos.x, savedPos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
            }
        }
    }
    if(item->getText() == "Decor") {
        if (appliname != "" && World::getCurrentEntityManager() != nullptr) {
            if (!showRectSelect) {
                Decor* decor = new Decor();
                selectedObject = decor;
                displayInfos(decor);
                World::addEntity(decor);
                //addDecor(decor);
            } else {
                BoundingBox rect = rectSelect.getSelectionRect();
                Vec3f savedPos = rectSelect.getSelectionRect().getPosition();
                Vec3f pos = rectSelect.getSelectionRect().getPosition();
                pos = getRenderWindow().mapPixelToCoords(Vec3f(pos.x, getRenderWindow().getSize().y - pos.y, 0))+getRenderWindow().getView().getSize()*0.5f;
                rectSelect.setRect(pos.x, pos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
                for (int x = rect.getPosition().x; x < rect.getPosition().x + rect.getSize().x-gridWidth; x+=gridWidth) {
                    for (int y = rect.getPosition().y; y <  rect.getPosition().y + rect.getSize().y-gridHeight; y+=gridHeight) {
                        Decor* decor = new Decor();
                        rectSelect.addItem(decor, sf::Color::White);
                        if (rectSelect.getItems().size() == 1) {
                            selectedObject = decor;
                            displayInfos(decor);
                        }
                        World::addEntity(decor);
                    }
                }
                rectSelect.setRect(savedPos.x, savedPos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
            }
        }
    }
    if (item->getText() == "Wall") {
         if (appliname != "" && World::getCurrentEntityManager() != nullptr) {
            if (!showRectSelect) {
                Wall* wall = new Wall();
                selectedObject = wall;
                displayInfos(wall);
                World::addEntity(wall);
                //addDecor(decor);
            } else {
                BoundingBox rect = rectSelect.getSelectionRect();
                Vec3f savedPos = rectSelect.getSelectionRect().getPosition();
                Vec3f pos = rectSelect.getSelectionRect().getPosition();
                pos = getRenderWindow().mapPixelToCoords(Vec3f(pos.x, getRenderWindow().getSize().y - pos.y, 0))+getRenderWindow().getView().getSize()*0.5f;
                rectSelect.setRect(pos.x, pos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
                for (int x = rect.getPosition().x; x < rect.getPosition().x + rect.getSize().x-gridWidth; x+=gridWidth) {
                    for (int y = rect.getPosition().y; y <  rect.getPosition().y + rect.getSize().y-gridHeight; y+=gridHeight) {
                        Wall* wall = new Wall();
                        rectSelect.addItem(wall, sf::Color::White);
                        if (rectSelect.getItems().size() == 1) {
                            selectedObject = wall;
                            displayInfos(wall);
                        }
                        World::addEntity(wall);
                    }
                }
                rectSelect.setRect(savedPos.x, savedPos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
            }
        }
    }
    if (item->getText() == "Animation") {
        if (appliname != "" && World::getCurrentEntityManager() != nullptr) {
            if (!showRectSelect) {
                Anim* anim = new Anim();
                selectedObject = anim;
                displayInfos(anim);
                World::addEntity(anim);
                //addDecor(decor);
            } else {
                BoundingBox rect = rectSelect.getSelectionRect();
                Vec3f savedPos = rectSelect.getSelectionRect().getPosition();
                Vec3f pos = rectSelect.getSelectionRect().getPosition();
                pos = getRenderWindow().mapPixelToCoords(Vec3f(pos.x, getRenderWindow().getSize().y - pos.y, 0))+getRenderWindow().getView().getSize()*0.5f;
                rectSelect.setRect(pos.x, pos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
                for (int x = rect.getPosition().x; x < rect.getPosition().x + rect.getSize().x-gridWidth; x+=gridWidth) {
                    for (int y = rect.getPosition().y; y <  rect.getPosition().y + rect.getSize().y-gridHeight; y+=gridHeight) {
                        Anim* anim = new Anim();
                        rectSelect.addItem(anim, sf::Color::White);
                        if (rectSelect.getItems().size() == 1) {
                            selectedObject = anim;
                            displayInfos(anim);
                        }
                        World::addEntity(anim);
                    }
                }
                rectSelect.setRect(savedPos.x, savedPos.y, rectSelect.getSelectionRect().getSize().x,rectSelect.getSelectionRect().getSize().y);
            }
        }
    }
    if (item->getText() == "Particle System") {
        if (appliname != "" && World::getCurrentEntityManager() != nullptr) {
            if (!showRectSelect) {
                ParticleSystem* ps = new ParticleSystem(Vec3f(0, 0, 0),Vec3f(100, 100, 0));
                selectedObject = ps;
                displayInfos(ps);
                World::addEntity(ps);
                //addDecor(decor);
            }
        }
    }
    if (item->getText() == "Emitter") {
        wNewEmitter->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wNewEmitter);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item->getText() == "Affector") {
        //wNewAffector->setVisbile(true);
    }
    if (item->getText() == "Undo") {
        stateStack.undo();
    }
    if (item->getText() == "Redo") {
        stateStack.redo();
    }
    if (item == item43) {
        showGrid = !showGrid;
        if (showGrid) {
            item43->setText("v Show grid");
        } else {
            item43->setText("Show grid");
        }
    }
    if (item == item44) {
        alignToGrid = !alignToGrid;
        if (alignToGrid) {
            item44->setText("v Align to grid");
        } else {
            item44->setText("Align to grid");
        }
    }
    if (item == item45) {
        showRectSelect = !showRectSelect;
        if (showRectSelect) {
            item45->setText("v Rect selection");
        } else {
            item45->setText("Rect selection");
        }
    }
    if (item->getText() == "New scene") {
        wNewMap->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wNewMap);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item->getText() == "New component") {
        wNewComponent->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wNewComponent);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item->getText() == "New entities updater") {
        wNewEntitiesUpdater->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wNewEntitiesUpdater);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item == item15) {
        fdProjectPath->setVisible(true);
        fdProjectPath->setEventContextActivated(true);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item == item16) {
        wNewAnimUpdater->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wNewAnimUpdater);
        tScriptEdit->setEventContextActivated(false);
    }
    if (item == item17) {
        //Save textures.
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        std::vector<std::string> paths = tm.getPaths();
        std::ofstream file(appliname+"\\"+"textures.oc");
        OTextArchive oa(file);
        oa(paths);
        file.close();
        //Save entities.
        std::vector<Entity*> entities = World::getEntities("*");
        std::ofstream file2(appliname+"\\"+"entities.oc");
        OTextArchive oa2(file2);
        oa2(entities);
        file.close();
        //Save components.
        std::ofstream file3(appliname+"\\"+"components.oc");
        OTextArchive oa3(file3);
        std::vector<Component*> components = getRenderComponentManager().getRenderComponents();
        unsigned int size = components.size();
        oa3(size);
        for (unsigned int i = 0; i < components.size(); i++) {
            std::string name = components[i]->getName();
            std::string componentType="";
            int layer=0;
            std::string expression="";
            if (dynamic_cast<PerPixelLinkedListRenderComponent*>(components[i])) {
                componentType = "PerPixelLinkedList";
                layer = static_cast<PerPixelLinkedListRenderComponent*>(components[i])->getLayer();
                expression = static_cast<PerPixelLinkedListRenderComponent*>(components[i])->getExpression();
                std::cout<<"layer : "<<layer<<std::endl;
            }
            oa3(name);
            oa3(componentType);
            oa3(layer);
            oa3(expression);
        }
        file3.close();
        //Save entities updater.
        std::ofstream file4(appliname+"\\"+"timers.oc");
        OTextArchive oa4(file4);
        std::vector<Timer*> timers = World::getTimers();
        size = timers.size();
        oa4(size);
        for (unsigned int i = 0; i < timers.size(); i++) {
            std::string name = timers[i]->getName();
            std::string timerType;
            std::vector<int> animIds;
            if (dynamic_cast<AnimUpdater*>(timers[i])) {
                timerType = "AnimationUpdater";
                std::vector<Anim*> animations = static_cast<AnimUpdater*>(timers[i])->getAnims();
                for (unsigned int j = 0; j < animations.size(); j++) {
                    animIds.push_back(animations[i]->getId());
                }
            }
            oa4(name);
            oa4(timerType);
            oa4(animIds);
        }
        file4.close();
        std::ofstream file5(appliname+"\\"+"workers.oc");
        OTextArchive oa5(file5);
        std::vector<EntitySystem*> workers = World::getWorkers();
        size = workers.size();
        oa5(size);
        for (unsigned int i = 0; i < workers.size(); i++) {
            std::string name = workers[i]->getName();
            std::string workerType;
            if (dynamic_cast<EntitiesUpdater*>(workers[i])) {
                workerType = "EntityUpdater";
            }
            oa5(name);
            oa5(workerType);
        }
        file5.close();
        std::ofstream file6(appliname+"\\"+"scenes.oc");
        OTextArchive oa6(file6);
        std::vector<EntityManager*> scenes = World::getEntityManagers();
        size = scenes.size();
        oa6(size);
        for (unsigned int i = 0; i < scenes.size(); i++) {
            std::string name;
            std::string type;
            int cellWidth;
            int cellHeight;
            bool is2DisoMatrix;
            if (dynamic_cast<Map*>(scenes[i])) {
                Map* scene = static_cast<Map*>(scenes[i]);
                name = scene->getName();
                cellWidth = scene->getCellWidth();
                cellHeight = scene->getCellHeight();
                type = "Map";
                BaseChangementMatrix bcm = scene->getBaseChangementMatrix();
                is2DisoMatrix = bcm.isIso2DMatrix();
            }
            oa6(name);
            oa6(type);
            oa6(cellWidth);
            oa6(cellHeight);
            oa6(is2DisoMatrix);
        }
        file6.close();
     }
     if (item == item18) {
        wNewParticleSystemUpdater->setVisible(true);
        getRenderComponentManager().setEventContextActivated(true, *wNewParticleSystemUpdater);
        tScriptEdit->setEventContextActivated(false);
     }
}
void ODFAEGCreator::addShape(Shape *shape) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        unsigned int pos = content.find("tm.getResourceByAlias");
        if (pos != std::string::npos && pos < content.size()) {
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
            while (subs.find("tm.getResourceByAlias") != std::string::npos) {
                subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
            }
        } else {
            pos = content.find("TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");");
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
        }
        std::string toInsert = "    std::unique_ptr<Shape> shape"+conversionUIntString(shape->getId())+" = std::make_unique<RectangleShape>(Vec3f(100, 50, 0));\n"
                               "    drawables.push_back(std::move(shape));\n";
        content.insert(pos, toInsert);
    }
}
void ODFAEGCreator::addTile(Tile* tile) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        unsigned int pos = content.find("tm.getResourceByAlias");
        if (pos != std::string::npos && pos < content.size()) {
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
            while (subs.find("tm.getResourceByAlias") != std::string::npos) {
                subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
            }
        } else {
            pos = content.find("TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");");
            std::string subs = content.substr(pos);
            pos += subs.find_first_of('\n') + 1;
        }
        std::string toInsert = "Tile* tile"+conversionUIntString(tile->getId())+" = new Tile (nullptr,Vec3f("+conversionFloatString(cursor.getPosition().x)+","+
        conversionFloatString(cursor.getPosition().y)+","+conversionFloatString(cursor.getPosition().z)+"),Vec3f(100,50,0),sf::IntRect(0, 0, "+conversionIntString(gridWidth)+","+
        conversionIntString(gridHeight)+"));\n"+"World::addEntity(tile"+conversionUIntString(tile->getId())+");\n";
        content.insert(pos, toInsert);

    }
}
bool ODFAEGCreator::removeShape (unsigned int id) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string content = it->second;
        for (auto it = shapes.begin(); it != shapes.end();it++) {
            if ((*it)->getId() == id) {
                unsigned int pos = content.find("std::unique_ptr<Shape*> shape"+conversionUIntString(id));
                std::string subs = content.substr(pos);
                unsigned int endpos = pos + subs.find_first_of('\n') + 1;
                content.erase(pos, pos - endpos);
                pos = content.find("drawables.push_back(std::move(shape"+conversionUIntString(id)+"));\n");
                do {
                    std::string subs = content.substr(pos);
                    unsigned int endpos = pos + subs.find_first_of('\n') + 1;
                    content.erase(pos, pos - endpos);
                    pos = content.find("shape"+conversionUIntString(id));
                } while(pos != std::string::npos);
                it = shapes.erase(it);
                return true;
            }
        }
    }
    return false;
}
void ODFAEGCreator::displayInfos (Shape* shape) {
    displayTransformInfos(shape);
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lId = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Id : shape-"+conversionIntString(shape->getId()), 15);
    lId->setParent(pInfos);
    Node* lIdNode = new Node("LabId", lId, Vec2f(0, 0), Vec2f(1, 0.025), rootInfosNode.get());
    pInfos->addChild(lId);
    lColor = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Color : ", 15);
    lColor->setParent(pMaterial);
    Node* lColorNode = new Node("LabColor",lColor,Vec2f(0, 0), Vec2f(1.f, 0.025f), rootMaterialNode.get());
    pMaterial->addChild(lColor);
    lRColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"r : ", 15);
    lRColor->setParent(pMaterial);
    Node* lRColorNode = new Node("LabRColor",lRColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lRColor);
    tRColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getFillColor().r), getRenderWindow());
    tRColor->setTextSize(15);
    tRColor->setParent(pMaterial);
    lRColorNode->addOtherComponent(tRColor, Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tRColor);
    lGColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"g : ", 15);
    lGColor->setParent(pMaterial);
    Node* lGColorNode = new Node("LabRColor",lGColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lGColor);
    tGColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getFillColor().g), getRenderWindow());
    tGColor->setTextSize(15);
    tGColor->setParent(pMaterial);
    lGColorNode->addOtherComponent(tGColor, Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tGColor);
    lBColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"b : ", 15);
    lBColor->setParent(pMaterial);
    Node* lBColorNode = new Node("LabBColor",lBColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lBColor);
    tBColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getFillColor().b), getRenderWindow());
    tBColor->setTextSize(15);
    tBColor->setParent(pMaterial);
    lBColorNode->addOtherComponent(tBColor, Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tBColor);
    lAColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"a : ", 15);
    lAColor->setParent(pMaterial);
    Node* lAColorNode = new Node("LabAColor",lAColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lAColor);
    tAColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getFillColor().a), getRenderWindow());
    tAColor->setTextSize(15);
    tAColor->setParent(pMaterial);
    lAColorNode->addOtherComponent(tAColor,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tAColor);
    Command cmdRColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tRColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tRColor));
    tRColor->getListener().connect("TRColorChanged", cmdRColChanged);
    Command cmdGColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tGColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tGColor));
    tGColor->getListener().connect("TGColorChanged", cmdGColChanged);
    Command cmdBColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tBColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tBColor));
    tBColor->getListener().connect("TBColorChanged", cmdBColChanged);
    Command cmdAColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tAColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tAColor));
    tAColor->getListener().connect("TAColorChanged", cmdAColChanged);
    lTexture = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0), fm.getResourceByAlias(Fonts::Serif),"Texture : ", 15);
    lTexture->setParent(pMaterial);
    Node* lTextureNode = new Node("LabTexture",lTexture,Vec2f(0, 0), Vec2f(1.f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexture);
    lTexCoordX = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex x : ", 15);
    lTexCoordX->setParent(pMaterial);
    Node* lTexCoordXNode = new Node("LTexCoordX", lTexCoordX,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordX);
    tTexCoordX = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordX->setTextSize(15);
    tTexCoordX->setParent(pMaterial);
    lTexCoordXNode->addOtherComponent(tTexCoordX,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordX);
    lTexCoordY = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex y : ", 15);
    lTexCoordY->setParent(pMaterial);
    Node* lTexCoordYNode = new Node("LTexCoordY", lTexCoordY,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordY);
    tTexCoordY = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordY->setTextSize(15);
    tTexCoordY->setParent(pMaterial);
    lTexCoordYNode->addOtherComponent(tTexCoordY,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordY);
    lTexCoordW = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex w : ", 15);
    lTexCoordW->setParent(pMaterial);
    Node* lTexCoordWNode = new Node("lTexCoordW", lTexCoordW,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordW);
    tTexCoordW = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordW->setTextSize(15);
    tTexCoordW->setParent(pMaterial);
    lTexCoordWNode->addOtherComponent(tTexCoordW,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordW);
    lTexCoordH = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex h : ", 15);
    lTexCoordH->setParent(pMaterial);
    Node* lTexCoordHNode = new Node("LTexCoordH", lTexCoordH,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordH);
    tTexCoordH = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordH->setTextSize(15);
    tTexCoordH->setParent(pMaterial);
    lTexCoordHNode->addOtherComponent(tTexCoordH,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordH);
    Command cmdTexCoordXChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordX), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordX));
    tTexCoordX->getListener().connect("TTexCoordXChanged", cmdTexCoordXChanged);
    Command cmdTexCoordYChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordY), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordY));
    tTexCoordY->getListener().connect("TTexCoordYChanged", cmdTexCoordYChanged);
    Command cmdTexCoordWChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordW), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordW));
    tTexCoordW->getListener().connect("TTexCoordWChanged", cmdTexCoordWChanged);
    Command cmdTexCoordHChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordH), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordH));
    tTexCoordH->getListener().connect("TTexCoordXChanged", cmdTexCoordHChanged);
    lTexImage = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Tex Image : ", 15);
    lTexImage->setParent(pMaterial);
    Node* selectTextNode = new Node("SelectTexture",lTexImage,Vec2f(0, 0),Vec2f(0.25f, 0.025f), rootMaterialNode.get());
    pMaterial->addChild(lTexImage);
    dpSelectTexture = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif),"NONE", 15);
    dpSelectTexture->setName("SELECTTEXT");
    dpSelectTexture->setParent(pMaterial);
    Command cmdTxtChanged(FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectTexture), FastDelegate<void>(&ODFAEGCreator::onSelectedTextureChanged, this, dpSelectTexture));
    dpSelectTexture->getListener().connect("TextureChanged", cmdTxtChanged);
    selectTextNode->addOtherComponent(dpSelectTexture,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(dpSelectTexture);
    bChooseText = new Button(Vec3f(0, 0, 0), Vec3f(100, 100, 0), fm.getResourceByAlias(Fonts::Serif),"New texture", 15, getRenderWindow());
    bChooseText->setParent(pMaterial);
    Node* chooseTextNode = new Node("ChooseText", bChooseText,Vec2f(0, 0), Vec2f(1.f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(bChooseText);
    bChooseText->setName("CHOOSETEXT");
    bChooseText->addActionListener(this);
    StateGroup* sg = new StateGroup("SGADDRECTANGLESHAPE");
    State* stAddRemoveShape = new State("ADDREMOVESHAPE", &se);
    std::ostringstream oss;
    OTextArchive ota(oss);
    ota(shape->getId());
    ota(shape);
    stAddRemoveShape->addParameter("ADDREMOVESHAPE", oss.str());
    sg->addState(stAddRemoveShape);
    stateStack.addStateGroup(sg);
    pScriptsFiles->setAutoResized(true);
}
void ODFAEGCreator::displayChildren(Label* label) {
    Node* node = rootInfosNode->findNode(label);
    std::vector<std::string> parts = split(label->getText(), "-");
    Entity* entity = World::getEntity(conversionStringInt(parts[1]));
    if (node->getNodes().size() == 0 && entity != nullptr) {
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        for (unsigned int i = 0; i < entity->getChildren().size(); i++) {

            /*Label* label = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Id : entity-"+conversionIntString(entity->getChild(i)->getId()), 15);
            Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
            Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, label), FastDelegate<void>(&ODFAEGCreator::displayChildren, this, label));
            label->getListener().connect("SHOWCHILDREN"+label->getText(), cmd);
            label->setParent(pInfos);
            Node* lNode = new Node(label->getText() + "-child-" + conversionIntString(i), label, Vec2f(0, 0), Vec2f(1.f, 0.025f), node);
            pInfos->addChild(label);*/
        }
    } else if (node->isNodeVisible()) {
        node->hideAllNodes();
    } else {
        node->showAllNodes();
    }
}
void ODFAEGCreator::displayTransformInfos(Transformable* tile) {
    rootPropNode->deleteAllNodes();
    rootMaterialNode->deleteAllNodes();
    rootInfosNode->deleteAllNodes();
    rootShadowsNode->deleteAllNodes();
    pTransform->removeAll();
    pMaterial->removeAll();
    pInfos->removeAll();
    pShadows->removeAll();
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    //Position.
    lPosition = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Position : ", 15);
    lPosition->setParent(pTransform);
    Node* lPosNode = new Node("LabPosition",lPosition,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lPosition);
    //X
    lPosX = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"x : ",15);
    lPosX->setParent(pTransform);
    Node* lPosXNode = new Node("LabX",lPosX,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosX);
    tPosX = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getPosition().x),getRenderWindow());
    tPosX->setParent(pTransform);
    tPosX->setTextSize(15);
    lPosXNode->addOtherComponent(tPosX, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosX);
    //Y
    lPosY = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"y : ",15);
    lPosY->setParent(pTransform);
    Node* lPosYNode = new Node("LabY",lPosY,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosY);
    tPosY = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getPosition().y),getRenderWindow());
    tPosY->setParent(pTransform);
    tPosY->setTextSize(15);
    lPosYNode->addOtherComponent(tPosY, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosY);
    //Z
    lPosZ = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"z : ",15);
    lPosZ->setParent(pTransform);
    Node* lPosZNode = new Node("LabZ",lPosZ,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosZ);
    tPosZ = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getPosition().z),getRenderWindow());
    tPosZ->setParent(pTransform);
    tPosZ->setTextSize(15);
    lPosZNode->addOtherComponent(tPosZ, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosZ);
    Command cmdPosX (FastDelegate<bool>(&TextArea::isTextChanged, tPosX), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosX));
    Command cmdPosY (FastDelegate<bool>(&TextArea::isTextChanged, tPosY), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosY));
    Command cmdPosZ (FastDelegate<bool>(&TextArea::isTextChanged, tPosZ), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosZ));
    tPosX->getListener().connect("tPosXChanged", cmdPosX);
    tPosY->getListener().connect("tPosYChanged", cmdPosY);
    tPosZ->getListener().connect("tPosZChanged", cmdPosZ);
    //Size.
    Label* lSize = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Size : ", 15);
    lSize->setParent(pTransform);
    Node* lSizeNode = new Node("LabSize",lSize,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lSize);
    //Width
    Label* lSizeW = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"w : ",15);
    lSizeW->setParent(pTransform);
    Node* lSizeWNode = new Node("LabW",lSizeW,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lSizeW);
    tSizeW = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getSize().x),getRenderWindow());
    tSizeW->setParent(pTransform);
    tSizeW->setTextSize(15);
    lSizeWNode->addOtherComponent(tSizeW, Vec2f(0.75, 0.025));
    pTransform->addChild(tSizeW);
    //Height
    Label* lSizeH = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"h : ",15);
    lSizeH->setParent(pTransform);
    Node* lSizeHNode = new Node("LabH",lSizeH,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lSizeH);
    tSizeH = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getSize().y),getRenderWindow());
    tSizeH->setParent(pTransform);
    tSizeH->setTextSize(15);
    lSizeHNode->addOtherComponent(tSizeH, Vec2f(0.75, 0.025));
    pTransform->addChild(tSizeH);
    //Depth
    Label* lSizeD = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"d : ",15);
    lSizeD->setParent(pTransform);
    Node* lSizeDNode = new Node("LabD",lSizeD,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lSizeD);
    tSizeD = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getSize().z),getRenderWindow());
    tSizeD->setParent(pTransform);
    tSizeD->setTextSize(15);
    lSizeDNode->addOtherComponent(tSizeD, Vec2f(0.75, 0.025));
    pTransform->addChild(tSizeD);
    Command cmdSizeW (FastDelegate<bool>(&TextArea::isTextChanged, tSizeW), FastDelegate<void>(&ODFAEGCreator::onObjectSizeChanged, this,tSizeW));
    Command cmdSizeH (FastDelegate<bool>(&TextArea::isTextChanged, tSizeH), FastDelegate<void>(&ODFAEGCreator::onObjectSizeChanged, this,tSizeH));
    Command cmdSizeD (FastDelegate<bool>(&TextArea::isTextChanged, tSizeD), FastDelegate<void>(&ODFAEGCreator::onObjectSizeChanged, this,tSizeD));
    tSizeW->getListener().connect("tSizeWChanged", cmdSizeW);
    tSizeH->getListener().connect("tSizeHChanged", cmdSizeH);
    tSizeD->getListener().connect("tSizeDChanged", cmdSizeD);
    //Move
    Label* lMove = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Translation : ", 15);
    lMove->setParent(pTransform);
    Node* lMoveNode = new Node("LabPosition",lMove,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lMove);
    //X
    Label *lMoveX = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"x : ",15);
    lMoveX->setParent(pTransform);
    Node* lMoveXNode = new Node("MOVEX",lMoveX,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lMoveX);
    tMoveX = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getTranslation().x),getRenderWindow());
    tMoveX->setParent(pTransform);
    tMoveX->setTextSize(15);
    lMoveXNode->addOtherComponent(tMoveX, Vec2f(0.75, 0.025));
    pTransform->addChild(tMoveX);
    //Y
    Label* lMoveY = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"y : ",15);
    lMoveY->setParent(pTransform);
    Node* lMoveYNode = new Node("MOVEY",lMoveY,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lMoveY);
    tMoveY = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getTranslation().y),getRenderWindow());
    tMoveY->setParent(pTransform);
    tMoveY->setTextSize(15);
    lMoveYNode->addOtherComponent(tMoveY, Vec2f(0.75, 0.025));
    pTransform->addChild(tMoveY);
    //Z
    Label* lMoveZ = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"z : ",15);
    lMoveZ->setParent(pTransform);
    Node* lMoveZNode = new Node("MOVEZ",lMoveZ,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lMoveZ);
    tMoveZ = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getTranslation().z),getRenderWindow());
    tMoveZ->setParent(pTransform);
    tMoveZ->setTextSize(15);
    lMoveZNode->addOtherComponent(tMoveZ, Vec2f(0.75, 0.025));
    pTransform->addChild(tMoveZ);
    Command cmdMoveX (FastDelegate<bool>(&TextArea::isTextChanged, tMoveX), FastDelegate<void>(&ODFAEGCreator::onObjectMoveChanged, this,tMoveX));
    Command cmdMoveY (FastDelegate<bool>(&TextArea::isTextChanged, tMoveY), FastDelegate<void>(&ODFAEGCreator::onObjectMoveChanged, this,tMoveY));
    Command cmdMoveZ (FastDelegate<bool>(&TextArea::isTextChanged, tMoveZ), FastDelegate<void>(&ODFAEGCreator::onObjectMoveChanged, this,tMoveZ));
    tMoveX->getListener().connect("tMoveXChanged", cmdMoveX);
    tMoveY->getListener().connect("tMoveYChanged", cmdMoveY);
    tMoveZ->getListener().connect("tMoveZChanged", cmdMoveZ);
    //Scale.
    Label* lScale = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Scale : ", 15);
    lScale->setParent(pTransform);
    Node* lScaleNode = new Node("LabScale",lScale,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lScale);
    //X
    Label *lScaleX = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"x : ",15);
    lScaleX->setParent(pTransform);
    Node* lScaleXNode = new Node("SCALEX",lScaleX,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lScaleX);
    tScaleX = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getScale().x),getRenderWindow());
    tScaleX->setParent(pTransform);
    tScaleX->setTextSize(15);
    lScaleXNode->addOtherComponent(tScaleX, Vec2f(0.75, 0.025));
    pTransform->addChild(tScaleX);
    //Y
    Label* lScaleY = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"y : ",15);
    lScaleY->setParent(pTransform);
    Node* lScaleYNode = new Node("SCALEY",lScaleY,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lScaleY);
    tScaleY = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getScale().y),getRenderWindow());
    tScaleY->setParent(pTransform);
    tScaleY->setTextSize(15);
    lScaleYNode->addOtherComponent(tScaleY, Vec2f(0.75, 0.025));
    pTransform->addChild(tScaleY);
    //Z
    Label* lScaleZ = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"z : ",15);
    lScaleZ->setParent(pTransform);
    Node* lScaleZNode = new Node("SCALEZ",lScaleZ,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lScaleZ);
    tScaleZ = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getScale().z),getRenderWindow());
    tScaleZ->setParent(pTransform);
    tScaleZ->setTextSize(15);
    lScaleZNode->addOtherComponent(tScaleZ, Vec2f(0.75, 0.025));
    pTransform->addChild(tScaleZ);
    Command cmdScaleX (FastDelegate<bool>(&TextArea::isTextChanged, tScaleX), FastDelegate<void>(&ODFAEGCreator::onObjectScaleChanged, this,tScaleX));
    Command cmdScaleY (FastDelegate<bool>(&TextArea::isTextChanged, tScaleY), FastDelegate<void>(&ODFAEGCreator::onObjectScaleChanged, this,tScaleY));
    Command cmdScaleZ (FastDelegate<bool>(&TextArea::isTextChanged, tScaleZ), FastDelegate<void>(&ODFAEGCreator::onObjectScaleChanged, this,tScaleZ));
    tScaleX->getListener().connect("tScaleXChanged", cmdScaleX);
    tScaleY->getListener().connect("tScaleYChanged", cmdScaleY);
    tScaleZ->getListener().connect("tScaleZChanged", cmdScaleZ);
    //Rotation.
    Label* lRotation = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"rotation : ", 15);
    lRotation->setParent(pTransform);
    Node* lRotationNode = new Node("ROTATION",lRotation,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lRotation);
    Label* lAngle = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"angle : ", 15);
    lAngle->setParent(pTransform);
    Node* lRotAngleNode = new Node("ANGLE",lAngle,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lAngle);
    tRotAngle = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getRotation()),getRenderWindow());
    tRotAngle->setParent(pTransform);
    tRotAngle->setTextSize(15);
    lRotAngleNode->addOtherComponent(tRotAngle, Vec2f(0.75, 0.025));
    pTransform->addChild(tRotAngle);
    Command cmdRotAngle (FastDelegate<bool>(&TextArea::isTextChanged, tRotAngle), FastDelegate<void>(&ODFAEGCreator::onObjectRotationChanged, this, tRotAngle));
    tRotAngle->getListener().connect("ROTANGLECHANGED", cmdRotAngle);
}
void ODFAEGCreator::displayEntityInfos(Entity* tile) {
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lId = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Id : entity-"+conversionIntString(tile->getId()), 15);
    lId->setParent(pInfos);
    Node* lIdNode = new Node("LabId", lId, Vec2f(0, 0), Vec2f(1, 0.025), rootInfosNode.get());
    pInfos->addChild(lId);
    Label* lName = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Name : ", 15);
    lName->setParent(pInfos);
    Node* nameNode = new Node("Name", lName, Vec2f(0, 0), Vec2f(0.25, 0.025), rootInfosNode.get());
    pInfos->addChild(lName);
    taName = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",getRenderWindow());
    taName->setTextSize(15);
    taName->setParent(pInfos);
    nameNode->addOtherComponent(taName, Vec2f(0.75, 0.025));
    pInfos->addChild(taName);
    Command cmdName (FastDelegate<bool>(&TextArea::isTextChanged, taName), FastDelegate<void>(&ODFAEGCreator::onObjectNameChanged, this,taName));
    taName->getListener().connect("CMDNAMECHANGED", cmdName);
    if (tile->getParent() == nullptr) {
        lParent = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Parent entity : NONE", 15);
    } else {
        lParent = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Parent entity : "+tile->getParent()->getName(), 15);
    }
    lParent->setParent(pInfos);
    Action a(Action::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
    Command cmdParentClicked(a, FastDelegate<bool>(&Label::isMouseInside, lParent), FastDelegate<void>(&ODFAEGCreator::onParentClicked, this, lParent));
    lParent->getListener().connect("CMDPARENTCLICKED", cmdParentClicked);
    Node* parentNode = new Node("Parent",lParent,Vec2f(0, 0),Vec2f(1, 0.025),rootInfosNode.get());
    pInfos->addChild(lParent);
    Label* lChildren = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Children-"+conversionIntString(tile->getId()), 15);
    Action aChildren(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
    Command cmd(aChildren, FastDelegate<bool>(&Label::isMouseInside, lChildren), FastDelegate<void>(&ODFAEGCreator::displayChildren, this, lChildren));
    lChildren->getListener().connect("SHOWCHILDREN", cmd);
    lChildren->setParent(pInfos);
    Node* childrenNode = new Node("Children",lChildren,Vec2f(0, 0),Vec2f(1, 0.025),rootInfosNode.get());
    pInfos->addChild(lChildren);
    std::vector<EntityManager*> ems = World::getEntityManagers();
    Label* lEmList = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 14, 0), fm.getResourceByAlias(Fonts::Serif), "Scene : ", 15);
    lEmList->setParent(pInfos);
    pInfos->addChild(lEmList);
    Node* lEmListNode = new Node("LabEmList", lEmList, Vec2f(0, 0), Vec2f(0.25, 0.025), rootInfosNode.get());
    dpSelectEm = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif),"NONE", 15);
    dpSelectEm->setName("DPSELECTEM");
    for (unsigned int i = 0; i < ems.size(); i++) {
        dpSelectEm->addItem(ems[i]->getName(), 15);
    }
    dpSelectEm->setParent(pInfos);
    pInfos->addChild(dpSelectEm);
    Command cmdEmChanged(FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectEm), FastDelegate<void>(&ODFAEGCreator::onSelectedEmChanged, this, dpSelectEm));
    dpSelectEm->getListener().connect("EmChanged", cmdEmChanged);
    dpSelectEm->setPriority(-2);
    lEmListNode->addOtherComponent(dpSelectEm, Vec2f(0.75, 0.025));
    Label* lSelectParent = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Parent : ",15);
    lSelectParent->setParent(pInfos);
    pInfos->addChild(lSelectParent);
    Node* selectParentNode = new Node("SelectParent",lSelectParent,Vec2f(0, 0),Vec2f(0.25, 0.025),rootInfosNode.get());
    dpSelectParent = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"NONE", 15);
    std::vector<Entity*> parents = World::getEntities("*");
    for (unsigned int i = 0; i < parents.size(); i++) {
        if (!parents[i]->isLeaf()) {
            dpSelectParent->addItem(parents[i]->getName(), 15);
        }
    }
    dpSelectParent->setParent(pInfos);
    pInfos->addChild(dpSelectParent);
    Command cmdParentChanged(FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectParent), FastDelegate<void>(&ODFAEGCreator::onSelectedParentChanged, this, dpSelectParent));
    dpSelectParent->getListener().connect("ParentChanged",cmdParentChanged);
    selectParentNode->addOtherComponent(dpSelectParent,Vec2f(0.75, 0.025));
    //Shadow center.
    Label* lShadowCenter = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Shadow center : ", 15);
    Node* nShadowCenter = new Node("ShadowCenter", lShadowCenter, Vec2f(0, 0), Vec2f(1, 0.025),rootShadowsNode.get());
    lShadowCenter->setParent(pShadows);
    pShadows->addChild(lShadowCenter);
    //X
    Label* lXShadowCenter = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "X : ", 15);
    Node* nXShadowCenter = new Node("XShadowCenter", lXShadowCenter, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lXShadowCenter->setParent(pShadows);
    pShadows->addChild(lXShadowCenter);
    taXShadowCenter = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowCenter().x),getRenderWindow());
    nXShadowCenter->addOtherComponent(taXShadowCenter, Vec2f(0.75, 0.025));
    taXShadowCenter->setTextSize(15);
    taXShadowCenter->setParent(pShadows);
    pShadows->addChild(taXShadowCenter);
    //Y
    Label* lYShadowCenter = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Y : ", 15);
    Node* nYShadowCenter = new Node("YShadowCenter", lYShadowCenter, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lYShadowCenter->setParent(pShadows);
    pShadows->addChild(lYShadowCenter);
    taYShadowCenter = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowCenter().y),getRenderWindow());
    nYShadowCenter->addOtherComponent(taYShadowCenter, Vec2f(0.75, 0.025));
    taYShadowCenter->setTextSize(15);
    taYShadowCenter->setParent(pShadows);
    pShadows->addChild(taYShadowCenter);
    //Z
    Label* lZShadowCenter = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Z : ", 15);
    Node* nZShadowCenter = new Node("ZShadowCenter", lZShadowCenter, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lZShadowCenter->setParent(pShadows);
    pShadows->addChild(lZShadowCenter);
    taZShadowCenter = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowCenter().z),getRenderWindow());
    nZShadowCenter->addOtherComponent(taZShadowCenter, Vec2f(0.75, 0.025));
    taZShadowCenter->setTextSize(15);
    taZShadowCenter->setParent(pShadows);
    pShadows->addChild(taZShadowCenter);
    Command cmdXShadowCenter(FastDelegate<bool>(&TextArea::isTextChanged, taXShadowCenter), FastDelegate<void>(&ODFAEGCreator::onShadowCenterChanged, this, taXShadowCenter));
    taXShadowCenter->getListener().connect("XSHADOWCENTERCHANGED", cmdXShadowCenter);
    Command cmdYShadowCenter(FastDelegate<bool>(&TextArea::isTextChanged, taYShadowCenter), FastDelegate<void>(&ODFAEGCreator::onShadowCenterChanged, this, taYShadowCenter));
    taYShadowCenter->getListener().connect("YSHADOWCENTERCHANGED", cmdYShadowCenter);
    Command cmdZShadowCenter(FastDelegate<bool>(&TextArea::isTextChanged, taZShadowCenter), FastDelegate<void>(&ODFAEGCreator::onShadowCenterChanged, this, taZShadowCenter));
    taZShadowCenter->getListener().connect("ZSHADOWCENTERCHANGED", cmdZShadowCenter);
    //Shadow scale.
    Label* lShadowScale = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Shadow scale : ", 15);
    Node* nShadowScale = new Node("ShadowSale", lShadowScale, Vec2f(0, 0), Vec2f(1, 0.025),rootShadowsNode.get());
    lShadowScale->setParent(pShadows);
    pShadows->addChild(lShadowScale);
    //X
    Label* lXShadowScale = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "X : ", 15);
    Node* nXShadowScale = new Node("XShadowCenter", lXShadowScale, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lXShadowScale->setParent(pShadows);
    pShadows->addChild(lXShadowScale);
    taXShadowScale = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowScale().x),getRenderWindow());
    nXShadowScale->addOtherComponent(taXShadowScale, Vec2f(0.75, 0.025));
    taXShadowScale->setTextSize(15);
    taXShadowScale->setParent(pShadows);
    pShadows->addChild(taXShadowScale);
    //Y
    Label* lYShadowScale = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Y : ", 15);
    Node* nYShadowScale = new Node("YShadowScale", lYShadowScale, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lYShadowScale->setParent(pShadows);
    pShadows->addChild(lYShadowScale);
    taYShadowScale = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowScale().y),getRenderWindow());
    nYShadowScale->addOtherComponent(taYShadowScale, Vec2f(0.75, 0.025));
    taYShadowScale->setTextSize(15);
    taYShadowScale->setParent(pShadows);
    pShadows->addChild(taYShadowScale);
    //Z
    Label* lZShadowScale = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Z : ", 15);
    Node* nZShadowScale = new Node("ZShadowScale", lZShadowScale, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lZShadowScale->setParent(pShadows);
    pShadows->addChild(lZShadowScale);
    taZShadowScale = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowScale().z),getRenderWindow());
    nZShadowScale->addOtherComponent(taZShadowScale, Vec2f(0.75, 0.025));
    taZShadowScale->setTextSize(15);
    taZShadowScale->setParent(pShadows);
    pShadows->addChild(taZShadowScale);
    Command cmdXShadowScale(FastDelegate<bool>(&TextArea::isTextChanged, taXShadowScale), FastDelegate<void>(&ODFAEGCreator::onShadowScaleChanged, this, taXShadowScale));
    taXShadowScale->getListener().connect("XSHADOWSCALECHANGED", cmdXShadowScale);
    Command cmdYShadowScale(FastDelegate<bool>(&TextArea::isTextChanged, taYShadowScale), FastDelegate<void>(&ODFAEGCreator::onShadowScaleChanged, this, taYShadowScale));
    taYShadowScale->getListener().connect("YSHADOWSCALECHANGED", cmdYShadowScale);
    Command cmdZShadowScale(FastDelegate<bool>(&TextArea::isTextChanged, taZShadowScale), FastDelegate<void>(&ODFAEGCreator::onShadowScaleChanged, this, taZShadowScale));
    taZShadowScale->getListener().connect("ZSHADOWSCALECHANGED", cmdZShadowScale);
    //Shadow rotation angle.
    Label* lShadowRotation = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "Shadow rotation : ", 15);
    Node* nShadowRotation = new Node("ShadowRotation", lShadowRotation, Vec2f(0, 0), Vec2f(1, 0.025),rootShadowsNode.get());
    lShadowRotation->setParent(pShadows);
    pShadows->addChild(lShadowRotation);
    //X
    Label* lShadowRotAngle = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif), "A : ", 15);
    Node* nShadowRotAngle = new Node("ShadowRotAngle", lShadowRotAngle, Vec2f(0, 0), Vec2f(0.25, 0.025), rootShadowsNode.get());
    lShadowRotAngle->setParent(pShadows);
    pShadows->addChild(lShadowRotAngle);
    taShadowRotAngle = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getShadowRotationAngle()),getRenderWindow());
    nShadowRotAngle->addOtherComponent(taShadowRotAngle, Vec2f(0.75, 0.025));
    taShadowRotAngle->setTextSize(15);
    taShadowRotAngle->setParent(pShadows);
    pShadows->addChild(taShadowRotAngle);
    Command cmdRotAngle(FastDelegate<bool>(&TextArea::isTextChanged, taShadowRotAngle), FastDelegate<void>(ODFAEGCreator::onShadowRotAngleChanged, this, taShadowRotAngle));
    taShadowRotAngle->getListener().connect("SHADOWROTANGLECHANGED", cmdRotAngle);
}
void ODFAEGCreator::displayInfos(Decor* decor) {
    displayTransformInfos(decor);
    displayEntityInfos(decor);
}
void ODFAEGCreator::displayInfos(Anim* anim) {
    displayTransformInfos(anim);
    displayEntityInfos(anim);
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lFrameRate = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"frame rate : ", 15);
    Node* frNode = new Node("FRAMERATE",lFrameRate,Vec2f(0, 0),Vec2f(0.25, 0.025),rootInfosNode.get());
    lFrameRate->setParent(pInfos);
    pInfos->addChild(lFrameRate);
    TextArea* taFrameRate = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif),"",getRenderWindow());
    taFrameRate->setParent(pInfos);
    pInfos->addChild(taFrameRate);
    frNode->addOtherComponent(taFrameRate, Vec2f(0.75, 0.025));
    Command cmdFRChanged(FastDelegate<bool>(&TextArea::isTextChanged, taFrameRate), FastDelegate<void>(&ODFAEGCreator::onFrameRateChanged, this, taFrameRate));
    taFrameRate->getListener().connect("FRCHANGED", cmdFRChanged);
    Label* lSelectAU = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Anim updater : ", 15);
    Node* animUpdaterNode = new Node("ANIMUPDATER",lSelectAU,Vec2f(0, 0),Vec2f(0.25, 0.025),rootInfosNode.get());
    lSelectAU->setParent(pInfos);
    pInfos->addChild(lSelectAU);
    dpSelectAU = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"NONE",15);
    std::vector<Timer*> timers = World::getTimers();
    for (unsigned int i = 0; i < timers.size(); i++) {
        dpSelectAU->addItem(timers[i]->getName(), 15);
    }
    dpSelectAU->setParent(pInfos);
    pInfos->addChild(dpSelectAU);
    animUpdaterNode->addOtherComponent(dpSelectAU, Vec2f(0.75, 0.025));
    Command cmdAUChanged(FastDelegate<bool>(&DropDownList::isValueChanged,dpSelectAU), FastDelegate<void>(&ODFAEGCreator::onAnimUpdaterChanged, this, dpSelectAU));
    dpSelectAU->getListener().connect("ANIMUPDATERCHANGED", cmdAUChanged);
}
void ODFAEGCreator::displayInfos(ParticleSystem* ps) {
    displayTransformInfos(ps);
    displayEntityInfos(ps);
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lselectPSU = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Particle system updater : ", 15);
    Node* psUpdaterNode = new Node("PSUPDATER",lselectPSU,Vec2f(0, 0),Vec2f(0.25, 0.025),rootInfosNode.get());
    lselectPSU->setParent(pInfos);
    pInfos->addChild(lselectPSU);
    dpSelectPSU = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"NONE",15);
    std::vector<EntitySystem*> workers = World::getWorkers();
    for (unsigned int i = 0; i < workers.size(); i++) {
        if (dynamic_cast<ParticleSystemUpdater*>(workers[i]))
            dpSelectPSU->addItem(workers[i]->getName(), 15);
    }
    dpSelectPSU->setParent(pInfos);
    pInfos->addChild(dpSelectPSU);
    psUpdaterNode->addOtherComponent(dpSelectPSU, Vec2f(0.75, 0.025));
    Command cmdPSUChanged(FastDelegate<bool>(&DropDownList::isValueChanged,dpSelectPSU), FastDelegate<void>(&ODFAEGCreator::onParticleSystemUpdaterChanged, this, dpSelectPSU));
    dpSelectPSU->getListener().connect("PSUUPDATERCHANGED", cmdPSUChanged);
    lTexture = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0), fm.getResourceByAlias(Fonts::Serif),"Texture : ", 15);
    lTexture->setParent(pMaterial);
    Node* lTextureNode = new Node("LabTexture",lTexture,Vec2f(0, 0), Vec2f(1.f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexture);
    lTexCoordX = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex x : ", 15);
    lTexCoordX->setParent(pMaterial);
    Node* lTexCoordXNode = new Node("LTexCoordX", lTexCoordX,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordX);
    tTexCoordX = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordX->setTextSize(15);
    tTexCoordX->setParent(pMaterial);
    lTexCoordXNode->addOtherComponent(tTexCoordX,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordX);
    lTexCoordY = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex y : ", 15);
    lTexCoordY->setParent(pMaterial);
    Node* lTexCoordYNode = new Node("LTexCoordY", lTexCoordY,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordY);
    tTexCoordY = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordY->setTextSize(15);
    tTexCoordY->setParent(pMaterial);
    lTexCoordYNode->addOtherComponent(tTexCoordY,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordY);
    lTexCoordW = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex w : ", 15);
    lTexCoordW->setParent(pMaterial);
    Node* lTexCoordWNode = new Node("lTexCoordW", lTexCoordW,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordW);
    tTexCoordW = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordW->setTextSize(15);
    tTexCoordW->setParent(pMaterial);
    lTexCoordWNode->addOtherComponent(tTexCoordW,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordW);
    lTexCoordH = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex h : ", 15);
    lTexCoordH->setParent(pMaterial);
    Node* lTexCoordHNode = new Node("LTexCoordH", lTexCoordH,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordH);
    tTexCoordH = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordH->setTextSize(15);
    tTexCoordH->setParent(pMaterial);
    lTexCoordHNode->addOtherComponent(tTexCoordH,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordH);
    Command cmdTexCoordXChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordX), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordX));
    tTexCoordX->getListener().connect("TTexCoordXChanged", cmdTexCoordXChanged);
    Command cmdTexCoordYChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordY), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordY));
    tTexCoordY->getListener().connect("TTexCoordYChanged", cmdTexCoordYChanged);
    Command cmdTexCoordWChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordW), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordW));
    tTexCoordW->getListener().connect("TTexCoordWChanged", cmdTexCoordWChanged);
    Command cmdTexCoordHChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordH), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordH));
    tTexCoordH->getListener().connect("TTexCoordXChanged", cmdTexCoordHChanged);
    lTexImage = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Tex Image : ", 15);
    lTexImage->setParent(pMaterial);
    Node* selectTextNode = new Node("SelectTexture",lTexImage,Vec2f(0, 0),Vec2f(0.25f, 0.025f), rootMaterialNode.get());
    pMaterial->addChild(lTexImage);
    dpSelectTexture = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif),"NONE", 15);
    dpSelectTexture->setName("SELECTTEXT");
    dpSelectTexture->setParent(pMaterial);
    Command cmdTxtChanged(FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectTexture), FastDelegate<void>(&ODFAEGCreator::onSelectedTextureChanged, this, dpSelectTexture));
    dpSelectTexture->getListener().connect("TextureChanged", cmdTxtChanged);
    selectTextNode->addOtherComponent(dpSelectTexture,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(dpSelectTexture);
    bChooseText = new Button(Vec3f(0, 0, 0), Vec3f(100, 100, 0), fm.getResourceByAlias(Fonts::Serif),"New texture", 15, getRenderWindow());
    bChooseText->setParent(pMaterial);
    Node* chooseTextNode = new Node("ChooseText", bChooseText,Vec2f(0, 0), Vec2f(0.5f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(bChooseText);
    bChooseText->setName("CHOOSETEXT");
    bChooseText->addActionListener(this);
    bAddTexRect = new Button(Vec3f(0, 0, 0), Vec3f(100, 100, 0), fm.getResourceByAlias(Fonts::Serif),"Add tex rect", 15, getRenderWindow());
    bAddTexRect->setParent(pMaterial);
    pMaterial->addChild(bAddTexRect);
    chooseTextNode->addOtherComponent(bAddTexRect,Vec2f(0.5f, 0.025f));
    bAddTexRect->addActionListener(this);
}
void ODFAEGCreator::displayInfos(Wall* wall) {
    displayTransformInfos(wall);
    displayEntityInfos(wall);
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lType = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"type : ",15);
    lType->setParent(lType);
    pInfos->addChild(lType);
    Node* node = new Node("WALLTYPE",lType,Vec2f(0, 0),Vec2f(0.25, 0.025),rootInfosNode.get());
    taWallType = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",getRenderWindow());
    Command cmdWallType(FastDelegate<bool>(&TextArea::isTextChanged, taWallType), FastDelegate<void>(&ODFAEGCreator::onWallTypeChanged,this,taWallType));
    taWallType->getListener().connect("WallTypeChanged", cmdWallType);
    taWallType->setParent(pInfos);
    pInfos->addChild(taWallType);
    node->addOtherComponent(taWallType, Vec2f(0.75, 0.025));
}
void ODFAEGCreator::displayInfos (Tile* tile) {
    displayTransformInfos(tile);
    displayEntityInfos(tile);
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    lColor = new Label(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Color : ", 15);
    lColor->setParent(pMaterial);
    Node* lColorNode = new Node("LabColor",lColor,Vec2f(0, 0), Vec2f(1.f, 0.025f), rootMaterialNode.get());
    pMaterial->addChild(lColor);
    lRColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"r : ", 15);
    lRColor->setParent(pMaterial);
    Node* lRColorNode = new Node("LabRColor",lRColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lRColor);
    tRColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getColor().r), getRenderWindow());
    tRColor->setTextSize(15);
    tRColor->setParent(pMaterial);
    lRColorNode->addOtherComponent(tRColor, Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tRColor);
    lGColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"g : ", 15);
    lGColor->setParent(pMaterial);
    Node* lGColorNode = new Node("LabRColor",lGColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lGColor);
    tGColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getColor().g), getRenderWindow());
    tGColor->setTextSize(15);
    tGColor->setParent(pMaterial);
    lGColorNode->addOtherComponent(tGColor, Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tGColor);
    lBColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"b : ", 15);
    lBColor->setParent(pMaterial);
    Node* lBColorNode = new Node("LabBColor",lBColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lBColor);
    tBColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getColor().b), getRenderWindow());
    tBColor->setTextSize(15);
    tBColor->setParent(pMaterial);
    lBColorNode->addOtherComponent(tBColor, Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tBColor);
    lAColor = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"a : ", 15);
    lAColor->setParent(pMaterial);
    Node* lAColorNode = new Node("LabAColor",lAColor,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lAColor);
    tAColor = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getColor().a), getRenderWindow());
    tAColor->setTextSize(15);
    tAColor->setParent(pMaterial);
    lAColorNode->addOtherComponent(tAColor,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tAColor);
    Command cmdRColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tRColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tRColor));
    tRColor->getListener().connect("TRColorChanged", cmdRColChanged);
    Command cmdGColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tGColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tGColor));
    tGColor->getListener().connect("TGColorChanged", cmdGColChanged);
    Command cmdBColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tBColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tBColor));
    tBColor->getListener().connect("TBColorChanged", cmdBColChanged);
    Command cmdAColChanged(FastDelegate<bool>(&TextArea::isTextChanged, tAColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tAColor));
    tAColor->getListener().connect("TAColorChanged", cmdAColChanged);
    lTexture = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0), fm.getResourceByAlias(Fonts::Serif),"Texture : ", 15);
    lTexture->setParent(pMaterial);
    Node* lTextureNode = new Node("LabTexture",lTexture,Vec2f(0, 0), Vec2f(1.f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexture);
    lTexCoordX = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex x : ", 15);
    lTexCoordX->setParent(pMaterial);
    Node* lTexCoordXNode = new Node("LTexCoordX", lTexCoordX,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordX);
    tTexCoordX = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordX->setTextSize(15);
    tTexCoordX->setParent(pMaterial);
    lTexCoordXNode->addOtherComponent(tTexCoordX,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordX);
    lTexCoordY = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex y : ", 15);
    lTexCoordY->setParent(pMaterial);
    Node* lTexCoordYNode = new Node("LTexCoordY", lTexCoordY,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordY);
    tTexCoordY = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordY->setTextSize(15);
    tTexCoordY->setParent(pMaterial);
    lTexCoordYNode->addOtherComponent(tTexCoordY,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordY);
    lTexCoordW = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex w : ", 15);
    lTexCoordW->setParent(pMaterial);
    Node* lTexCoordWNode = new Node("lTexCoordW", lTexCoordW,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordW);
    tTexCoordW = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordW->setTextSize(15);
    tTexCoordW->setParent(pMaterial);
    lTexCoordWNode->addOtherComponent(tTexCoordW,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordW);
    lTexCoordH = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Tex h : ", 15);
    lTexCoordH->setParent(pMaterial);
    Node* lTexCoordHNode = new Node("LTexCoordH", lTexCoordH,Vec2f(0, 0), Vec2f(0.25f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(lTexCoordH);
    tTexCoordH = new TextArea (Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"0",getRenderWindow());
    tTexCoordH->setTextSize(15);
    tTexCoordH->setParent(pMaterial);
    lTexCoordHNode->addOtherComponent(tTexCoordH,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(tTexCoordH);
    Command cmdTexCoordXChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordX), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordX));
    tTexCoordX->getListener().connect("TTexCoordXChanged", cmdTexCoordXChanged);
    Command cmdTexCoordYChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordY), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordY));
    tTexCoordY->getListener().connect("TTexCoordYChanged", cmdTexCoordYChanged);
    Command cmdTexCoordWChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordW), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordW));
    tTexCoordW->getListener().connect("TTexCoordWChanged", cmdTexCoordWChanged);
    Command cmdTexCoordHChanged (FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordH), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordH));
    tTexCoordH->getListener().connect("TTexCoordXChanged", cmdTexCoordHChanged);
    lTexImage = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Tex Image : ", 15);
    lTexImage->setParent(pMaterial);
    Node* selectTextNode = new Node("SelectTexture",lTexImage,Vec2f(0, 0),Vec2f(0.25f, 0.025f), rootMaterialNode.get());
    pMaterial->addChild(lTexImage);
    dpSelectTexture = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif),"NONE", 15);
    dpSelectTexture->setName("SELECTTEXT");
    dpSelectTexture->setParent(pMaterial);
    Command cmdTxtChanged(FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectTexture), FastDelegate<void>(&ODFAEGCreator::onSelectedTextureChanged, this, dpSelectTexture));
    dpSelectTexture->getListener().connect("TextureChanged", cmdTxtChanged);
    selectTextNode->addOtherComponent(dpSelectTexture,Vec2f(0.75f, 0.025f));
    pMaterial->addChild(dpSelectTexture);
    bChooseText = new Button(Vec3f(0, 0, 0), Vec3f(100, 100, 0), fm.getResourceByAlias(Fonts::Serif),"New texture", 15, getRenderWindow());
    bChooseText->setParent(pMaterial);
    Node* chooseTextNode = new Node("ChooseText", bChooseText,Vec2f(0, 0), Vec2f(1.f, 0.025f),rootMaterialNode.get());
    pMaterial->addChild(bChooseText);
    bChooseText->setName("CHOOSETEXT");
    bChooseText->addActionListener(this);
    StateGroup* sg = new StateGroup("SGADDRECTANGLETILE");
    State* stAddRemoveShape = new State("ADDREMOVETILE", &se);
    std::ostringstream oss;
    OTextArchive ota(oss);
    ota(tile->getId());
    ota(tile);
    stAddRemoveShape->addParameter("ADDREMOVETILE", oss.str());
    sg->addState(stAddRemoveShape);
    stateStack.addStateGroup(sg);
    pScriptsFiles->setAutoResized(true);
}
void ODFAEGCreator::moveCursor(sf::Vector2f mousePos) {
    BoundingBox bb (guiPos.x, guiPos.y, guiPos.z, guiSize.x, guiSize.y, guiSize.z);
    if (bb.isPointInside(Vec3f(mousePos.x, mousePos.y, 0))) {
        if (!alignToGrid) {
            cursor.setPosition(Vec3f(mousePos.x-getRenderWindow().getView().getSize().x * 0.5f, mousePos.y-getRenderWindow().getView().getSize().y * 0.5f, 0));
        } else {
            if (World::getCurrentEntityManager() == nullptr) {
                int x = ((int) mousePos.x/gridWidth*gridWidth)-getRenderWindow().getView().getSize().x * 0.5f;
                int y = ((int) mousePos.y/gridHeight*gridHeight)-getRenderWindow().getView().getSize().y * 0.5f;
                cursor.setPosition(Vec3f(x, y, 0));
            } else {
                int x = mousePos.x-getRenderWindow().getView().getSize().x * 0.5f;
                int y = mousePos.y-getRenderWindow().getView().getSize().y * 0.5f;
                Vec3f newPos = getGridCellPos(Vec3f(x, y, 0));
                cursor.setPosition(newPos);
            }
        }
    }
}
void ODFAEGCreator::updateScriptPos(Transformable* shape) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        if (dynamic_cast<Shape*>(shape)) {
            if(content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setPosition") == std::string::npos) {
                unsigned int pos = content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+" = std::make_unique<RectangleShape>");
                std::string subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
                content.insert(pos,"    shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setPosition(Vec3f("+conversionIntString(shape->getPosition().x)+","
                +conversionIntString(shape->getPosition().y)+","+conversionIntString(shape->getPosition().z)+");\n");
            } else {
                unsigned int pos = content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setPosition");
                std::string subs = content.substr(pos);
                unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                content.erase(pos, endpos - pos);
                content.insert(pos,"shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setPosition(Vec3f("+conversionIntString(shape->getPosition().x)+","
                +conversionIntString(shape->getPosition().y)+","+conversionIntString(shape->getPosition().z)+"));\n");
            }
        }
        if (dynamic_cast<Tile*>(shape)) {
            if(content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setPosition") == std::string::npos) {
                unsigned int pos = content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+" = new Tile");
                std::string subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
                content.insert(pos,"    tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setPosition(Vec3f("+conversionIntString(shape->getPosition().x)+","
                +conversionIntString(shape->getPosition().y)+","+conversionIntString(shape->getPosition().z)+");\n");
            } else {
                unsigned int pos = content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setPosition");
                std::string subs = content.substr(pos);
                unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                content.erase(pos, endpos - pos);
                content.insert(pos,"tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setPosition(Vec3f("+conversionIntString(shape->getPosition().x)+","
                +conversionIntString(shape->getPosition().y)+","+conversionIntString(shape->getPosition().z)+"));\n");
            }
        }
    }
}
void ODFAEGCreator::onShadowScaleChanged(TextArea* ta) {
    if (ta == taXShadowScale) {
        if (is_number(ta->getText())) {
            float xShadScale = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEXSHADSCALE"+conversionFloatString(xShadScale));
            State* state = new State("SCHANGEXSHADSCALE", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowScale().x);
            state->addParameter("NEWVALUE", xShadScale);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowScale(Vec3f(xShadScale, static_cast<Entity*>(selectedObject)->getShadowScale().y, static_cast<Entity*>(selectedObject)->getShadowScale().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEXSHADSCALE", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().x);
                selectState->addParameter("NEWVALUE", xShadScale);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowScale(Vec3f(xShadScale, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().y, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().z));
                sg->addState(selectState);
            }
        }
    }
    if (ta == taYShadowScale) {
        if (is_number(ta->getText())) {
            float yShadScale = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEYSHADSCALE"+conversionFloatString(yShadScale));
            State* state = new State("SCHANGEYSHADSCALE", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowScale().y);
            state->addParameter("NEWVALUE", yShadScale);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowScale(Vec3f(static_cast<Entity*>(selectedObject)->getShadowScale().x, yShadScale, static_cast<Entity*>(selectedObject)->getShadowScale().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEYSHADSCALE", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().y);
                selectState->addParameter("NEWVALUE", yShadScale);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowScale(Vec3f(static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().x, yShadScale, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().z));
                sg->addState(selectState);
            }
        }
    }
    if (ta == taZShadowScale) {
        if (is_number(ta->getText())) {
            float zShadScale = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEZSHADSCALE"+conversionFloatString(zShadScale));
            State* state = new State("SCHANGEZSHADSCALE", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowScale().z);
            state->addParameter("NEWVALUE", zShadScale);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowScale(Vec3f(static_cast<Entity*>(selectedObject)->getShadowScale().x, static_cast<Entity*>(selectedObject)->getShadowScale().y, zShadScale));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEZSHADSCALE", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().z);
                selectState->addParameter("NEWVALUE", zShadScale);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowScale(Vec3f(static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().x, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowScale().y, zShadScale));
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onShadowRotAngleChanged(TextArea* ta) {
    if (ta == taShadowRotAngle) {
        if (is_number(ta->getText())) {
            float rotAngle = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESHADROTANGLE"+conversionFloatString(rotAngle));
            State* state = new State("SGCHANGESHADROTANGLE", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowRotationAngle());
            state->addParameter("NEWVALUE", rotAngle);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowRotation(rotAngle);
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SGCHANGESHADROTANGLE", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowRotationAngle());
                selectState->addParameter("NEWVALUE", rotAngle);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowRotation(rotAngle);
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onShadowCenterChanged(TextArea* ta) {
    if (ta == taXShadowCenter) {
        if (is_number(ta->getText())) {
            float xShadCenter = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEXSHADCENTER"+conversionFloatString(xShadCenter));
            State* state = new State("SCHANGEXSHADCENTER", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowCenter().x);
            state->addParameter("NEWVALUE", xShadCenter);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowCenter(Vec3f(xShadCenter, static_cast<Entity*>(selectedObject)->getShadowCenter().y, static_cast<Entity*>(selectedObject)->getShadowCenter().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEXSHADCENTER", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().x);
                selectState->addParameter("NEWVALUE", xShadCenter);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowCenter(Vec3f(xShadCenter, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().y, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == taYShadowCenter) {
        if(is_number(ta->getText())) {
            float yShadCenter = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEYSHADCENTER"+conversionFloatString(yShadCenter));
            State* state = new State("SCHANGEYSHADCENTER", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowCenter().y);
            state->addParameter("NEWVALUE", yShadCenter);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowCenter(Vec3f(static_cast<Entity*>(selectedObject)->getShadowCenter().x, yShadCenter, static_cast<Entity*>(selectedObject)->getShadowCenter().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEYSHADCENTER", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().x);
                selectState->addParameter("NEWVALUE", yShadCenter);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowCenter(Vec3f(static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().x, yShadCenter, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == taZShadowCenter) {
        if(is_number(ta->getText())) {
            float zShadCenter = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEZSHADCENTER"+conversionFloatString(zShadCenter));
            State* state = new State("SCHANGEZSHADCENTER", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", static_cast<Entity*>(selectedObject)->getShadowCenter().z);
            state->addParameter("NEWVALUE", zShadCenter);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            static_cast<Entity*>(selectedObject)->setShadowCenter(Vec3f(static_cast<Entity*>(selectedObject)->getShadowCenter().x, static_cast<Entity*>(selectedObject)->getShadowCenter().y, zShadCenter));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEYSHADCENTER", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().z);
                selectState->addParameter("NEWVALUE", zShadCenter);
                static_cast<Entity*>(rectSelect.getItems()[i])->setShadowCenter(Vec3f(static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().x, static_cast<Entity*>(rectSelect.getItems()[i])->getShadowCenter().y, zShadCenter));
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onObjectPosChanged(TextArea* ta) {
    if (ta == tPosX) {
        if (is_number(ta->getText())) {
            float newXPos = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEXPOS"+conversionFloatString(newXPos));
            State* state = new State("SCHANGEXPOS", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getPosition().x);
            state->addParameter("NEWVALUE", newXPos);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setPosition(Vec3f(newXPos, selectedObject->getPosition().y, selectedObject->getPosition().z));
            updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEXPOS", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getPosition().x);
                selectState->addParameter("NEWVALUE", newXPos);
                rectSelect.getItems()[i]->setPosition(Vec3f(newXPos, rectSelect.getItems()[i]->getPosition().y, rectSelect.getItems()[i]->getPosition().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tPosY) {
        if(is_number(ta->getText())) {
            float newYPos = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEYPOS"+conversionFloatString(newYPos));
            State* state = new State("SCHANGEYPOS", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getPosition().y);
            state->addParameter("NEWVALUE", newYPos);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setPosition(Vec3f(selectedObject->getPosition().x, newYPos, selectedObject->getPosition().z));
            updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEYPOS", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getPosition().y);
                selectState->addParameter("NEWVALUE", newYPos);
                rectSelect.getItems()[i]->setPosition(Vec3f(rectSelect.getItems()[i]->getPosition().x, newYPos, rectSelect.getItems()[i]->getPosition().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tPosZ) {
        if(is_number(ta->getText())) {
            float newZPos = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEZPOS"+conversionFloatString(newZPos));
            State* state = new State("SCHANGEZPOS", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getPosition().z);
            state->addParameter("NEWVALUE", newZPos);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setPosition(Vec3f(selectedObject->getPosition().x, selectedObject->getPosition().y, newZPos));
            updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEZPOS", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getPosition().z);
                selectState->addParameter("NEWVALUE", newZPos);
                rectSelect.getItems()[i]->setPosition(Vec3f(rectSelect.getItems()[i]->getPosition().x, rectSelect.getItems()[i]->getPosition().y, newZPos));
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onObjectRotationChanged(TextArea* ta) {
    if (ta == tRotAngle) {
        if (is_number(ta->getText())) {
            float newAngle = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEROTANGLE"+conversionFloatString(newAngle));
            State* state = new State("SCHANGEROTANGLE", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getRotation());
            state->addParameter("NEWVALUE", newAngle);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setRotation(newAngle);
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEROTANGLE", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getRotation());
                selectState->addParameter("NEWVALUE", newAngle);
                rectSelect.getItems()[i]->setRotation(newAngle);
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onObjectSizeChanged(TextArea* ta) {
    if (ta == tSizeW) {
        if (is_number(ta->getText())) {
            float newSizeW = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESIZEW"+conversionFloatString(newSizeW));
            State* state = new State("SCHANGESIZEW", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getSize().x);
            state->addParameter("NEWVALUE", newSizeW);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setSize(Vec3f(newSizeW, selectedObject->getSize().y, selectedObject->getSize().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGESIZEW", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getSize().x);
                selectState->addParameter("NEWVALUE", newSizeW);
                rectSelect.getItems()[i]->setSize(Vec3f(newSizeW, rectSelect.getItems()[i]->getSize().y, rectSelect.getItems()[i]->getSize().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tSizeH) {
        if(is_number(ta->getText())) {
            float newSizeH = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESIZEW"+conversionFloatString(newSizeH));
            State* state = new State("SCHANGESIZEW", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getSize().y);
            state->addParameter("NEWVALUE", newSizeH);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setSize(Vec3f(selectedObject->getSize().x, newSizeH, selectedObject->getSize().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGESIZEH", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getSize().y);
                selectState->addParameter("NEWVALUE", newSizeH);
                rectSelect.getItems()[i]->setSize(Vec3f(rectSelect.getItems()[i]->getSize().x, newSizeH, rectSelect.getItems()[i]->getSize().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tSizeD) {
        if(is_number(ta->getText())) {
            float newSizeD = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESIZEZ"+conversionFloatString(newSizeD));
            State* state = new State("SCHANGESIZEZ", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getSize().z);
            state->addParameter("NEWVALUE", newSizeD);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->setSize(Vec3f(selectedObject->getSize().x, selectedObject->getSize().y, newSizeD));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGESIZEZ", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getSize().z);
                selectState->addParameter("NEWVALUE", newSizeD);
                rectSelect.getItems()[i]->setSize(Vec3f(rectSelect.getItems()[i]->getSize().x, rectSelect.getItems()[i]->getSize().y, newSizeD));
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onObjectScaleChanged(TextArea* ta) {
    if (ta == tScaleX) {
        if (is_number(ta->getText())) {
            float newScaleX = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESCALEX"+conversionFloatString(newScaleX));
            State* state = new State("SCHANGESCALEX", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getScale().x);
            state->addParameter("NEWVALUE", newScaleX);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->scale(Vec3f(newScaleX / selectedObject->getScale().x, selectedObject->getScale().y, selectedObject->getScale().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGESCALEX", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getScale().x);
                selectState->addParameter("NEWVALUE", newScaleX);
                rectSelect.getItems()[i]->scale(Vec3f(newScaleX / rectSelect.getItems()[i]->getScale().x, rectSelect.getItems()[i]->getScale().y, rectSelect.getItems()[i]->getScale().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tScaleY) {
        if(is_number(ta->getText())) {
            float newScaleY = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESCALEY"+conversionFloatString(newScaleY));
            State* state = new State("SCHANGESCALEY", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getScale().y);
            state->addParameter("NEWVALUE", newScaleY);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->scale(Vec3f(selectedObject->getScale().x, newScaleY/selectedObject->getScale().y, selectedObject->getScale().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGESCALEY", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getScale().y);
                selectState->addParameter("NEWVALUE", newScaleY);
                rectSelect.getItems()[i]->scale(Vec3f(rectSelect.getItems()[i]->getScale().x, newScaleY/rectSelect.getItems()[i]->getScale().y, rectSelect.getItems()[i]->getScale().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tScaleZ) {
        if(is_number(ta->getText())) {
            float newScaleZ = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGESCALEZ"+conversionFloatString(newScaleZ));
            State* state = new State("SCHANGESCALEZ", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getScale().z);
            state->addParameter("NEWVALUE", newScaleZ);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->scale(Vec3f(selectedObject->getScale().x, selectedObject->getScale().y, newScaleZ/selectedObject->getScale().z));
            updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGESCALEZ", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getScale().z);
                selectState->addParameter("NEWVALUE", newScaleZ);
                rectSelect.getItems()[i]->setPosition(Vec3f(rectSelect.getItems()[i]->getScale().x, rectSelect.getItems()[i]->getScale().y, newScaleZ/rectSelect.getItems()[i]->getScale().z));
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::onObjectMoveChanged(TextArea* ta) {
    if (ta == tMoveX) {
        if (is_number(ta->getText())) {
            float newMoveX = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEXTRANS"+conversionFloatString(newMoveX));
            State* state = new State("SCHANGEXTRANS", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getTranslation().x);
            state->addParameter("NEWVALUE", newMoveX);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->move(Vec3f(newMoveX-selectedObject->getTranslation().x, selectedObject->getTranslation().y, selectedObject->getTranslation().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEXTRANS", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getTranslation().x);
                selectState->addParameter("NEWVALUE", newMoveX);
                rectSelect.getItems()[i]->setPosition(Vec3f(newMoveX-rectSelect.getItems()[i]->getTranslation().x, rectSelect.getItems()[i]->getTranslation().y, rectSelect.getItems()[i]->getTranslation().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tMoveY) {
        if(is_number(ta->getText())) {
            float newMoveY = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEYTRANS"+conversionFloatString(newMoveY));
            State* state = new State("SCHANGEYTRANS", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getTranslation().y);
            state->addParameter("NEWVALUE", newMoveY);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->move(Vec3f(selectedObject->getTranslation().x, newMoveY-selectedObject->getTranslation().y, selectedObject->getTranslation().z));
            //updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEYTRANS", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getTranslation().y);
                selectState->addParameter("NEWVALUE", newMoveY);
                rectSelect.getItems()[i]->move(Vec3f(rectSelect.getItems()[i]->getTranslation().x, newMoveY-rectSelect.getItems()[i]->getTranslation().y, rectSelect.getItems()[i]->getTranslation().z));
                sg->addState(selectState);
            }
        }
    } else if (ta == tMoveZ) {
        if(is_number(ta->getText())) {
            float newMoveZ = conversionStringFloat(ta->getText());
            StateGroup* sg = new StateGroup("SGCHANGEZTRANS"+conversionFloatString(newMoveZ));
            State* state = new State("SCHANGEZTRANS", &se);
            state->addParameter("OBJECT", selectedObject);
            state->addParameter("OLDVALUE", selectedObject->getTranslation().z);
            state->addParameter("NEWVALUE", newMoveZ);
            sg->addState(state);
            stateStack.addStateGroup(sg);
            selectedObject->move(Vec3f(selectedObject->getTranslation().x, selectedObject->getTranslation().y, newMoveZ-selectedObject->getTranslation().z));
            updateScriptPos(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEMOVEZ", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                selectState->addParameter("OLDVALUE", rectSelect.getItems()[i]->getTranslation().z);
                selectState->addParameter("NEWVALUE", newMoveZ);
                rectSelect.getItems()[i]->setPosition(Vec3f(rectSelect.getItems()[i]->getTranslation().x, rectSelect.getItems()[i]->getTranslation().y, newMoveZ-rectSelect.getItems()[i]->getTranslation().z));
                sg->addState(selectState);
            }
        }
    }
}
void ODFAEGCreator::updateScriptColor(Transformable* shape) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        if (dynamic_cast<Shape*>(shape)) {
            if(content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setFillColor") == std::string::npos) {
                unsigned int pos = content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+" = std::make_unique<RectangleShape>");
                std::string subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
                content.insert(pos,"    shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setFillColor(sf::Color("+conversionIntString(static_cast<Shape*>(shape)->getFillColor().r)+","
                +conversionIntString(static_cast<Shape*>(shape)->getFillColor().g)+","+conversionIntString(static_cast<Shape*>(shape)->getFillColor().b)+","+conversionIntString(static_cast<Shape*>(shape)->getFillColor().a)+"));\n");
            } else {
                unsigned int pos = content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setFillColor");
                std::string subs = content.substr(pos);
                unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                content.erase(pos, endpos - pos);
                content.insert(pos,"    shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setFillColor(sf::Color("+conversionIntString(static_cast<Shape*>(shape)->getFillColor().r)+","
                +conversionIntString(static_cast<Shape*>(shape)->getFillColor().g)+","+conversionIntString(static_cast<Shape*>(shape)->getFillColor().b)+","+conversionIntString(static_cast<Shape*>(shape)->getFillColor().a)+"));\n");
            }
        }
        if (dynamic_cast<Tile*>(shape)) {
            if(content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setColor") == std::string::npos) {
                unsigned int pos = content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+" = new Tile");
                std::string subs = content.substr(pos);
                pos += subs.find_first_of('\n') + 1;
                content.insert(pos,"    tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setColor(sf::Color("+conversionIntString(static_cast<Tile*>(shape)->getColor().r)+","
                +conversionIntString(static_cast<Tile*>(shape)->getColor().g)+","+conversionIntString(static_cast<Tile*>(shape)->getColor().b)+","+conversionIntString(static_cast<Tile*>(shape)->getColor().a)+"));\n");
            } else {
                unsigned int pos = content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setPosition");
                std::string subs = content.substr(pos);
                unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                content.erase(pos, endpos - pos);
                content.insert(pos,"    tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setColor(sf::Color("+conversionIntString(static_cast<Tile*>(shape)->getColor().r)+","
                +conversionIntString(static_cast<Tile*>(shape)->getColor().g)+","+conversionIntString(static_cast<Tile*>(shape)->getColor().b)+","+conversionIntString(static_cast<Tile*>(shape)->getColor().a)+"));\n");
            }
        }
    }
}
void ODFAEGCreator::onObjectNameChanged(TextArea* ta) {
    selectedObject->setName(ta->getText());
}
void ODFAEGCreator::onWallTypeChanged(TextArea* ta) {
    if (dynamic_cast<Wall*>(selectedObject) && is_number(ta->getText())) {
        static_cast<Wall*>(selectedObject)->setType(ta->getText());
    }
}
void ODFAEGCreator::onFrameRateChanged(TextArea* ta) {
    if (dynamic_cast<Anim*>(selectedObject) && is_number(ta->getText())) {
        static_cast<Anim*>(selectedObject)->setFrameRate(conversionStringFloat(ta->getText()));
    }
}
void ODFAEGCreator::onObjectColorChanged(TextArea* ta) {
    if (ta == tRColor) {
        if (is_number(tRColor->getText())) {
            unsigned int color = conversionStringInt(tRColor->getText());
            StateGroup* sg = new StateGroup("SGCHANGERCOLOR"+conversionUIntString(color));
            State* state = new State("SCHANGERCOLOR", &se);
            state->addParameter("OBJECT", selectedObject);
            if (dynamic_cast<Shape*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getFillColor().r);
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getColor().r);
            }
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Shape*>(selectedObject)->getFillColor().g,static_cast<Shape*>(selectedObject)->getFillColor().b, static_cast<Shape*>(selectedObject)->getFillColor().a));
                rectSelect.setColor(selectedObject, static_cast<Shape*>(selectedObject)->getFillColor());
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Tile*>(selectedObject)->getColor().g,static_cast<Tile*>(selectedObject)->getColor().b, static_cast<Tile*>(selectedObject)->getColor().a));
                rectSelect.setColor(selectedObject, static_cast<Tile*>(selectedObject)->getColor());
            }
            updateScriptColor(selectedObject);
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGERCOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Shape*>(selectedObject)->getFillColor().g,static_cast<Shape*>(selectedObject)->getFillColor().b, static_cast<Shape*>(selectedObject)->getFillColor().a));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor());
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g,static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Tile*>(rectSelect.getItems()[i])->getColor());
                }
                selectState->addParameter("NEWVALUE", color);
                sg->addState(selectState);
            }
            stateStack.addStateGroup(sg);
        }
    }
    if (ta == tGColor) {
        if (is_number(tGColor->getText())) {
            unsigned int color = conversionStringInt(tGColor->getText());
            StateGroup* sg = new StateGroup("SGCHANGEGCOLOR"+conversionUIntString(color));
            State* state = new State("SCHANGEGCOLOR", &se);
            state->addParameter("OBJECT", selectedObject);
            if (dynamic_cast<Shape*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getFillColor().g);
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getColor().g);
            }
            updateScriptColor(selectedObject);
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(static_cast<Shape*>(selectedObject)->getFillColor().r, Math::clamp(color, 0, 255),static_cast<Shape*>(selectedObject)->getFillColor().b, static_cast<Shape*>(selectedObject)->getFillColor().a));
                rectSelect.setColor(selectedObject, static_cast<Shape*>(selectedObject)->getFillColor());
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(static_cast<Tile*>(selectedObject)->getColor().r, Math::clamp(color, 0, 255),static_cast<Tile*>(selectedObject)->getColor().b, static_cast<Tile*>(selectedObject)->getColor().a));
                rectSelect.setColor(selectedObject, static_cast<Tile*>(selectedObject)->getColor());
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEGCOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().g);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r, Math::clamp(color, 0, 255),static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().b, static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().a));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor());
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r, Math::clamp(color, 0, 255),static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Tile*>(rectSelect.getItems()[i])->getColor());
                }
                selectState->addParameter("NEWVALUE", color);
                sg->addState(selectState);
            }
            stateStack.addStateGroup(sg);
        }
    }
    if (ta == tBColor) {
        if (is_number(tBColor->getText())) {
            unsigned int color = conversionStringInt(tBColor->getText());
            StateGroup* sg = new StateGroup("SGCHANGEBCOLOR"+conversionUIntString(color));
            State* state = new State("SCHANGEBCOLOR", &se);
            state->addParameter("OBJECT", selectedObject);
            if (dynamic_cast<Shape*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getFillColor().b);
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getColor().b);
            }
            updateScriptColor(selectedObject);
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(static_cast<Shape*>(selectedObject)->getFillColor().r, static_cast<Shape*>(selectedObject)->getFillColor().g, Math::clamp(color, 0, 255), static_cast<Shape*>(selectedObject)->getFillColor().a));
                rectSelect.setColor(selectedObject, static_cast<Shape*>(selectedObject)->getFillColor());
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(static_cast<Tile*>(selectedObject)->getColor().r, static_cast<Tile*>(selectedObject)->getColor().g, Math::clamp(color, 0, 255), static_cast<Tile*>(selectedObject)->getColor().a));
                rectSelect.setColor(selectedObject, static_cast<Tile*>(selectedObject)->getColor());
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEBCOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().b);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r, static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().g, Math::clamp(color, 0, 255), static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().a));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor());
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g, Math::clamp(color, 0, 255), static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Tile*>(rectSelect.getItems()[i])->getColor());
                }
                selectState->addParameter("NEWVALUE", color);
                sg->addState(selectState);
            }
            stateStack.addStateGroup(sg);
        }
    }
    if (ta == tAColor) {
        if (is_number(tAColor->getText())) {
            unsigned int color = conversionStringInt(tAColor->getText());
            StateGroup* sg = new StateGroup("SGCHANGEACOLOR"+conversionUIntString(color));
            State* state = new State("SCHANGEACOLOR", &se);
            state->addParameter("OBJECT", selectedObject);
            if (dynamic_cast<Shape*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getFillColor().a);
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getColor().a);
            }
            updateScriptColor(selectedObject);
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(static_cast<Shape*>(selectedObject)->getFillColor().r, static_cast<Shape*>(selectedObject)->getFillColor().g,static_cast<Shape*>(selectedObject)->getFillColor().b, Math::clamp(color, 0, 255)));
                rectSelect.setColor(selectedObject, static_cast<Shape*>(selectedObject)->getFillColor());
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(static_cast<Tile*>(selectedObject)->getColor().r, static_cast<Tile*>(selectedObject)->getColor().g,static_cast<Tile*>(selectedObject)->getColor().b, Math::clamp(color, 0, 255)));
                rectSelect.setColor(selectedObject, static_cast<Tile*>(selectedObject)->getColor());
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEACOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().a);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r, static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().g,static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().b, Math::clamp(color, 0, 255)));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor());
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g,static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b, Math::clamp(color, 0, 255)));
                    rectSelect.setColor(rectSelect.getItems()[i], static_cast<Tile*>(rectSelect.getItems()[i])->getColor());
                }
                selectState->addParameter("NEWVALUE", color);
                sg->addState(selectState);
            }
            stateStack.addStateGroup(sg);
        }
    }
}
void ODFAEGCreator::onSelectedComponentChanged(DropDownList* dp) {
    std::string name = dp->getSelectedItem();
    if(name == "MAIN WINDOW") {
        taComponentExpression->setText("");
    } else {
        std::vector<Component*> components = getRenderComponentManager().getRenderComponents();
        for (unsigned int i = 0; i < components.size(); i++) {
            if (components[i]->getName() == name && dynamic_cast<HeavyComponent*>(components[i])) {
                taChangeComponentExpression->setText(static_cast<HeavyComponent*>(components[i])->getExpression());
            }
        }
    }
}
void ODFAEGCreator::onParentClicked(Label* label) {
    std::vector<std::string> parts = split(label->getText(), ":");
    std::string pName = parts[1].substr(1, parts[1].size()-1);
    if (pName != "NONE") {
        if (dynamic_cast<Entity*>(selectedObject) && dynamic_cast<Anim*>(static_cast<Entity*>(selectedObject)->getParent())) {
            displayInfos(static_cast<Anim*>(static_cast<Entity*>(selectedObject)->getParent()));
            selectedObject = static_cast<Entity*>(selectedObject)->getParent();
        }
    }
}
void ODFAEGCreator::onSelectedParentChanged(DropDownList* dp) {
    if (dp->getSelectedItem() == "NONE") {
        static_cast<Entity*>(selectedObject)->setParent(nullptr);
    } else if (dp->getSelectedItem() != "") {
        Entity* entity = World::getEntity(dp->getSelectedItem());
        if (entity != nullptr && dynamic_cast<Entity*>(selectedObject)) {

            if (!entity->isAnimated()) {
                static_cast<Entity*>(selectedObject)->setParent(entity);
            } else {
                if (dynamic_cast<Anim*>(entity)) {
                    World::removeEntity(static_cast<Entity*>(selectedObject));
                    World::removeEntity(entity);
                    static_cast<Anim*>(entity)->addFrame(static_cast<Entity*>(selectedObject));
                    static_cast<Anim*>(entity)->play(true);
                    World::addEntity(entity);
                }
            }
            lParent->setText("Parent entity : "+entity->getName());
        }
    }
}
void ODFAEGCreator::onAnimUpdaterChanged(DropDownList* dp) {
    Timer* timer = World::getTimer(dp->getSelectedItem());
    if (dynamic_cast<AnimUpdater*>(timer) && dynamic_cast<Anim*>(selectedObject)) {
        static_cast<AnimUpdater*>(timer)->addAnim(static_cast<Anim*>(selectedObject));
    }
}
void ODFAEGCreator::onParticleSystemUpdaterChanged(DropDownList* dp) {
    EntitySystem* worker = World::getWorker(dp->getSelectedItem());
    if (dynamic_cast<ParticleSystemUpdater*>(worker) && dynamic_cast<ParticleSystem*>(selectedObject)) {
        std::cout<<"add particle system to updater"<<std::endl;
        static_cast<ParticleSystemUpdater*>(worker)->addParticleSystem(static_cast<ParticleSystem*>(selectedObject));
    }
}
void ODFAEGCreator::onSelectedTextureChanged(DropDownList* dp) {
    pMaterial->removeSprites();

    const Texture* oldTexture;
    if (dynamic_cast<Shape*>(selectedObject)) {
        oldTexture = static_cast<Shape*>(selectedObject)->getTexture();
    }
    if (dynamic_cast<Tile*>(selectedObject)) {
        oldTexture = static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().getTexture();
    }
    if (dp->getSelectedItem() == "NONE") {
        if (dynamic_cast<Shape*>(selectedObject)) {
            static_cast<Shape*>(selectedObject)->setTexture(nullptr);
            updateScriptText(static_cast<Shape*>(selectedObject), nullptr);
        }
        if (dynamic_cast<Tile*>(selectedObject)) {
            Tile* selectedTile = static_cast<Tile*>(selectedObject);
            selectedTile->getFace(0)->getMaterial().clearTextures();
            selectedTile->getFace(0)->getMaterial().addTexture(nullptr, sf::IntRect(0, 0, gridWidth, gridHeight));
            static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().setTexId("");
            updateScriptText(static_cast<Tile*>(selectedObject), nullptr);
        }
    } else {
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        /*for (unsigned int i = 0; i < textPaths.size(); i++) {
            std::cout<<"searching tex path : "<<dp->getSelectedItem()<<std::endl;
            if (textPaths[i].find(dp->getSelectedItem())) {*/
                const Texture* text = tm.getResourceByAlias(dp->getSelectedItem());
                std::vector<std::string> alias = tm.getAliasByResource(const_cast<Texture*>(text));
                pMaterial->clearDrawables();
                Sprite sprite (*text, Vec3f(pScriptsFiles->getPosition().x, bChooseText->getPosition().y + bChooseText->getSize().y+10, 0),Vec3f(text->getSize().x, text->getSize().y, 0),sf::IntRect(0, 0, text->getSize().x,text->getSize().y));
                pMaterial->addSprite(sprite);
                sf::IntRect textRect;
                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTexture(text);
                    textRect = static_cast<Shape*>(selectedObject)->getTextureRect();
                    updateScriptText(static_cast<Shape*>(selectedObject), text);
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().clearTextures();
                    static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().addTexture(text, sf::IntRect(0, 0, text->getSize().x, text->getSize().y));
                    static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().setTexId(alias[0]);
                    textRect = sf::IntRect(0, 0, text->getSize().x, text->getSize().y);
                    updateScriptText(static_cast<Tile*>(selectedObject), text);
                }
                if (dynamic_cast<ParticleSystem*>(selectedObject)) {
                    static_cast<ParticleSystem*>(selectedObject)->setTexture(*text);
                    textRect = sf::IntRect(0, 0, text->getSize().x, text->getSize().y);
                }
                tTexCoordX->setText(conversionIntString(textRect.left));
                tTexCoordY->setText(conversionIntString(textRect.top));
                tTexCoordW->setText(conversionIntString(textRect.width));
                tTexCoordH->setText(conversionIntString(textRect.height));
                sTextRect = new RectangleShape(Vec3f(textRect.width, textRect.height, 0));
                sTextRect->setPosition(Vec3f(textRect.left+pScriptsFiles->getPosition().x, textRect.top + bChooseText->getPosition().y + bChooseText->getSize().y+10, 0));
                sTextRect->setFillColor(sf::Color::Transparent);
                sTextRect->setOutlineColor(sf::Color::Red);
                sTextRect->setOutlineThickness(1);
                sTextRect->setName("TexRect");
                pMaterial->addShape(sTextRect);
            /*}
        }*/
    }
    StateGroup* sg = new StateGroup("SGCHANGETEXTURE"+conversionLongString(reinterpret_cast<unsigned long>(oldTexture)));
    State* state = new State("SCHANGETEXTURE", &se);
    state->addParameter("OLDVALUE",oldTexture);
    if (dynamic_cast<Shape*>(selectedObject)) {
        state->addParameter("NEWVALUE",static_cast<Shape*>(selectedObject)->getTexture());
    }
    if (dynamic_cast<Tile*>(selectedObject)) {
        state->addParameter("NEWVALUE",static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().getTexture());
    }
    state->addParameter("OBJECT", selectedObject);
    sg->addState(state);
    for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
        if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
            oldTexture = static_cast<Shape*>(rectSelect.getItems()[i])->getTexture();
        }
        if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
            oldTexture = static_cast<Tile*>(rectSelect.getItems()[i])->getFace(0)->getMaterial().getTexture();
        }
        if (dp->getSelectedItem() == "NONE") {
            if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                static_cast<Shape*>(rectSelect.getItems()[i])->setTexture(nullptr);
            }
            if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                Tile* selectedTile = static_cast<Tile*>(rectSelect.getItems()[i]);
                selectedTile->getFace(0)->getMaterial().clearTextures();
                selectedTile->getFace(0)->getMaterial().addTexture(nullptr, sf::IntRect(0, 0, gridWidth, gridHeight));
            }
        } else {
            TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
            const Texture* text = tm.getResourceByAlias(dp->getSelectedItem());
            if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                static_cast<Shape*>(rectSelect.getItems()[i])->setTexture(text);
            }
            if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                static_cast<Tile*>(rectSelect.getItems()[i])->getFace(0)->getMaterial().clearTextures();
                static_cast<Tile*>(rectSelect.getItems()[i])->getFace(0)->getMaterial().addTexture(text, sf::IntRect(0, 0, text->getSize().x, text->getSize().y));
            }
        }
        State* selectState = new State("SCHANGETEXTURE", &se);
        selectState->addParameter("OLDVALUE", oldTexture);
        if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
            selectState->addParameter("NEWVALUE",static_cast<Shape*>(rectSelect.getItems()[i])->getTexture());
        }
        if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
            selectState->addParameter("NEWVALUE",static_cast<Tile*>(rectSelect.getItems()[i])->getFace(0)->getMaterial().getTexture());
        }
        sg->addState(selectState);
    }
    stateStack.addStateGroup(sg);
    pMaterial->updateScrolls();
}
void ODFAEGCreator::onTexCoordsChanged (TextArea* ta) {
    const Texture* tex;
    sf::IntRect texRect;
    if (dynamic_cast<Shape*>(selectedObject)) {
        tex = static_cast<Shape*>(selectedObject)->getTexture();
        texRect = static_cast<Shape*>(selectedObject)->getTextureRect();
    }
    if (dynamic_cast<Tile*>(selectedObject)) {
        tex = static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().getTexture();
        texRect = static_cast<Tile*>(selectedObject)->getTexCoords();
    }
    if (tex != nullptr) {
        if (ta == tTexCoordX) {
            if (is_number(ta->getText())) {
                int texCoordX = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEXTEXCOORD");
                State* state = new State("SCHANGEXTEXCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().left);
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getTexCoords().left);
                }
                state->addParameter("NEWVALUE", texCoordX);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);

                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(Math::abs(texCoordX), texRect.top, texRect.width, texRect.height));
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    static_cast<Tile*>(selectedObject)->setTexRect(sf::IntRect(Math::abs(texCoordX), texRect.top, texRect.width, texRect.height));
                }
                for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                    State* selectState = new State("SCHANGEXTEXCOORD", &se);
                    state->addParameter("OBJECT", rectSelect.getItems()[i]);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getTextureRect().left);
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getTexCoords().left);
                    }
                    selectState->addParameter("NEWVALUE", texCoordX);
                    sg->addState(state);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        static_cast<Shape*>(rectSelect.getItems()[i])->setTextureRect(sf::IntRect(Math::abs(texCoordX), texRect.top, texRect.width, texRect.height));
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        static_cast<Tile*>(rectSelect.getItems()[i])->setTexRect(sf::IntRect(Math::abs(texCoordX), texRect.top, texRect.width, texRect.height));
                    }
                }
                stateStack.addStateGroup(sg);
            }
        }
        if (ta == tTexCoordY) {
            if (is_number(ta->getText())) {
                int texCoordY = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEYTEXCOORD");
                State* state = new State("SCHANGEYTEXCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().top);
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getTexCoords().top);
                }
                state->addParameter("NEWVALUE", texCoordY);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);

                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(texRect.left, Math::abs(texCoordY), texRect.width, texRect.height));
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    static_cast<Tile*>(selectedObject)->setTexRect(sf::IntRect(texRect.left, Math::abs(texCoordY), texRect.width, texRect.height));
                }
                for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                    State* selectState = new State("SCHANGEXTEYCOORD", &se);
                    state->addParameter("OBJECT", rectSelect.getItems()[i]);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getTextureRect().top);
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getTexCoords().top);
                    }
                    selectState->addParameter("NEWVALUE", texCoordY);
                    sg->addState(state);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        static_cast<Shape*>(rectSelect.getItems()[i])->setTextureRect(sf::IntRect(texRect.left, Math::abs(texCoordY), texRect.width, texRect.height));
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        static_cast<Tile*>(rectSelect.getItems()[i])->setTexRect(sf::IntRect(texRect.left, Math::abs(texCoordY), texRect.width, texRect.height));
                    }
                }
                stateStack.addStateGroup(sg);
            }
        }
        if (ta == tTexCoordW) {
            if (is_number(ta->getText())) {
                int texCoordW = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEWTEXCOORD"+conversionIntString(texCoordW));
                State* state = new State("SCHANGEWTEXCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().width);
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getTexCoords().width);
                }
                state->addParameter("NEWVALUE", texCoordW);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);

                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(texRect.left, texRect.top, Math::abs(texCoordW), texRect.height));
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    static_cast<Tile*>(selectedObject)->setTexRect(sf::IntRect(texRect.left, texRect.top, Math::abs(texCoordW), texRect.height));
                }
                for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                    State* selectState = new State("SCHANGEWTEXCOORD", &se);
                    state->addParameter("OBJECT", rectSelect.getItems()[i]);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getTextureRect().width);
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getTexCoords().width);
                    }
                    selectState->addParameter("NEWVALUE", texCoordW);
                    sg->addState(state);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        static_cast<Shape*>(rectSelect.getItems()[i])->setTextureRect(sf::IntRect(texRect.left, texRect.top, Math::abs(texCoordW), texRect.height));
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        static_cast<Tile*>(rectSelect.getItems()[i])->setTexRect(sf::IntRect(texRect.left, texRect.top, Math::abs(texCoordW), texRect.height));
                    }
                }
                stateStack.addStateGroup(sg);
            }
        }
        if (ta == tTexCoordH) {
            if (is_number(ta->getText())) {
                int texCoordH = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEHTEXCOORD"+conversionIntString(texCoordH));
                State* state = new State("SCHANGEHTEXCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().height);
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Tile*>(selectedObject)->getTexCoords().height);
                }
                state->addParameter("NEWVALUE", texCoordH);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);

                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(texRect.left, texRect.top, texRect.width, Math::abs(texCoordH)));
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    static_cast<Tile*>(selectedObject)->setTexRect(sf::IntRect(texRect.left, texRect.top, texRect.width, Math::abs(texCoordH)));
                }
                for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                    State* selectState = new State("SCHANGEHTEXCOORD", &se);
                    state->addParameter("OBJECT", rectSelect.getItems()[i]);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getTextureRect().height);
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        state->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getTexCoords().height);
                    }
                    selectState->addParameter("NEWVALUE", texCoordH);
                    sg->addState(state);
                    if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                        static_cast<Shape*>(rectSelect.getItems()[i])->setTextureRect(sf::IntRect(texRect.left, texRect.top, texRect.width, Math::abs(texCoordH)));
                    }
                    if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                        static_cast<Tile*>(rectSelect.getItems()[i])->setTexRect(sf::IntRect(texRect.left, texRect.top, texRect.width, Math::abs(texCoordH)));
                    }
                }
                stateStack.addStateGroup(sg);
            }
        }
        if ((ta == tTexCoordX || ta == tTexCoordY || ta == tTexCoordW || ta == tTexCoordH)
            && is_number(tTexCoordX->getText()) && is_number(tTexCoordY->getText()) && is_number(tTexCoordW->getText()) && is_number(tTexCoordH->getText())) {

            sTextRect->setPosition(Vec3f(conversionStringInt(tTexCoordX->getText())+pScriptsFiles->getPosition().x, conversionStringInt(tTexCoordY->getText()) + bChooseText->getPosition().y + bChooseText->getSize().y+10, 0));
            sTextRect->setSize(Vec3f(conversionStringInt(tTexCoordW->getText()), conversionStringInt(tTexCoordH->getText()), 0));
        }
    }
}
void ODFAEGCreator::updateScriptTextCoords(Transformable* shape) {
    std::map<std::string, std::string>::iterator it;
    it = cppAppliContent.find(minAppliname+".cpp");
    if (it != cppAppliContent.end()) {
        std::string& content = it->second;
        if (dynamic_cast<Shape*>(shape)) {
            if(content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setTextureRect") == std::string::npos) {
                unsigned int pos = content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+" = std::make_unique<RectangleShape>");
                if (pos != std::string::npos) {
                    std::string subs = content.substr(pos);
                    pos += subs.find_first_of('\n') + 1;
                    content.insert(pos,"    shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setTextureRect(sf::IntRect("+conversionIntString(static_cast<Shape*>(shape)->getTextureRect().left)+","
                    +conversionIntString(static_cast<Shape*>(shape)->getTextureRect().top)+","+conversionIntString(static_cast<Shape*>(shape)->getTextureRect().width)+","+conversionIntString(static_cast<Shape*>(shape)->getTextureRect().height)+"));\n");
                }
            } else {
                unsigned int pos = content.find("shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setTextureRect");
                if (pos != std::string::npos) {
                    std::string subs = content.substr(pos);
                    unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                    content.erase(pos, endpos - pos);
                    content.insert(pos,"    shape"+conversionUIntString(static_cast<Shape*>(shape)->getId())+"->setTextureRect(sf::IntRect("+conversionIntString(static_cast<Shape*>(shape)->getTextureRect().left)+","
                    +conversionIntString(static_cast<Shape*>(shape)->getTextureRect().top)+","+conversionIntString(static_cast<Shape*>(shape)->getTextureRect().width)+","+conversionIntString(static_cast<Shape*>(shape)->getTextureRect().height)+"));\n");
                }
            }
        }
        if (dynamic_cast<Tile*>(shape)) {
            if(content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setTexRect") == std::string::npos) {
                unsigned int pos = content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+" = new Tile");
                if (pos != std::string::npos) {
                    std::string subs = content.substr(pos);
                    pos += subs.find_first_of('\n') + 1;
                    content.insert(pos,"    tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setTexRect(sf::IntRect("+conversionIntString(static_cast<Tile*>(shape)->getTexCoords().left)+","
                    +conversionIntString(static_cast<Tile*>(shape)->getTexCoords().top)+","+conversionIntString(static_cast<Tile*>(shape)->getTexCoords().width)+","+conversionIntString(static_cast<Tile*>(shape)->getTexCoords().height)+"));\n");
                }
            } else {
                unsigned int pos = content.find("tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setPosition");
                if (pos != std::string::npos) {
                    std::string subs = content.substr(pos);
                    unsigned int endpos = subs.find_first_of('\n') + pos + 1;
                    content.erase(pos, endpos - pos);
                    content.insert(pos,"    tile"+conversionUIntString(static_cast<Tile*>(shape)->getId())+"->setTexRect(sf::IntRect("+conversionIntString(static_cast<Tile*>(shape)->getTexCoords().left)+","
                    +conversionIntString(static_cast<Tile*>(shape)->getTexCoords().top)+","+conversionIntString(static_cast<Tile*>(shape)->getTexCoords().width)+","+conversionIntString(static_cast<Tile*>(shape)->getTexCoords().height)+"));\n");
                }
            }
        }
    }
}
void ODFAEGCreator::onSelectedEmChanged(DropDownList* dp) {
    if (dp->getSelectedItem() == "NONE") {
        if (dynamic_cast<Entity*>(selectedObject)) {
            if (World::getCurrentEntityManager != nullptr)
                World::removeEntity(static_cast<Entity*>(selectedObject));
        }
    } else {
        if (dynamic_cast<Entity*>(selectedObject)) {
            World::setCurrentEntityManager(dp->getSelectedItem());
            World::addEntity(static_cast<Entity*>(selectedObject));
        }
    }
}
void ODFAEGCreator::onComponentExpressionChanged(TextArea* ta) {
    //std::cout<<"component expression changed"<<std::endl;
    std::string name = dpSelectComponent->getSelectedItem();
    std::vector<Component*> components = getRenderComponentManager().getRenderComponents();
    for (unsigned int i = 0; i < components.size(); i++) {
        if (name == components[i]->getName() && dynamic_cast<HeavyComponent*>(components[i])) {
            static_cast<HeavyComponent*>(components[i])->setExpression(ta->getText());
        }
    }
}
