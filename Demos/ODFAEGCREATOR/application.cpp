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
using namespace odfaeg::physic;
using namespace odfaeg::window;
ODFAEGCreator::ODFAEGCreator(sf::VideoMode vm, std::string title) :
Application (vm, title, sf::Style::Resize|sf::Style::Close, ContextSettings(0, 0, 0, 3, 0)), isGuiShown (false), cursor(10), se(this) {
    dpSelectTexture = nullptr;
    showGrid = false;
    alignToGrid = false;
    showRectSelect = false;
    gridWidth = 100;
    gridHeight = 50;
    appliname = "";
}
void ODFAEGCreator::onLoad() {
    FontManager<Fonts> fm;
    fm.fromFileWithAlias("fonts/FreeSerif.ttf", Serif);
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
    menu1->addMenuItem(item11);
    menu1->addMenuItem(item12);
    menu1->addMenuItem(item13);
    menu1->addMenuItem(item14);
    menu1->addMenuItem(item15);
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
    menu3->addMenuItem(item31);
    menu3->addMenuItem(item32);
    menu3->addMenuItem(item33);
    menu3->addMenuItem(item34);
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
    //wApplicationNew->setName("WAPPLICATIONNEW");
    /*View waView = wApplicationNew->getDefaultView();
    waView.setCenter(Vec3f(wApplicationNew->getSize().x * 0.5f, wApplicationNew->getSize().y * 0.5f, 0));
    wApplicationNew->setView(waView);*/
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
    //Create component.
    wNewComponent = new RenderWindow(sf::VideoMode(400, 300), "Create new render component", sf::Style::Default, ContextSettings(0, 0, 0, 3, 0));
    Label* labComponentExpression = new Label(*wNewComponent,Vec3f(0, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif), "entity's type(s) : ", 15);
    getRenderComponentManager().addComponent(labComponentExpression);
    taComponentExpression = new TextArea(Vec3f(200, 0, 0),Vec3f(200, 50, 0),fm.getResourceByAlias(Fonts::Serif),"",*wNewComponent);
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
    bCreateComponent = new Button(Vec3f(0, 150, 0), Vec3f(200, 50, 0), fm.getResourceByAlias(Fonts::Serif),"Create component",15,*wNewComponent);
    bCreateComponent->addActionListener(this);
    getRenderComponentManager().addComponent(bCreateComponent);
    addWindow(wNewComponent);
    wNewComponent->setVisible(false);
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
    pScriptsEdit->setRelPosition(1.f / 6.f, 0.01f);
    pScriptsEdit->setRelSize(0.60f, 1.f);
    pScriptsEdit->setBorderColor(sf::Color(128, 128, 128));
    pScriptsEdit->setBackgroundColor(sf::Color::White);
    pScriptsEdit->setBorderThickness(5);
    getRenderComponentManager().addComponent(pScriptsEdit);
    pScriptsFiles = new Panel(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pScriptsFiles->setBorderColor(sf::Color(128, 128, 128));
    pScriptsFiles->setBackgroundColor(sf::Color::White);
    pScriptsFiles->setBorderThickness(5);
    pScriptsFiles->setRelPosition(5.f / 6.5f, 0.015f);
    pScriptsFiles->setRelSize(1.5f / 6.f, 1.f);
    pScriptsFiles->setName("PSCRIPTFILES");
    getRenderComponentManager().addComponent(pScriptsFiles);
    tabPane = new TabPane(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(200, 700, 0));
    tabPane->setRelPosition(0, 0);
    tabPane->setRelSize(1, 1);
    tabPane->setParent(pScriptsFiles);
    pScriptsFiles->addChild(tabPane);
    pScriptsFiles->setBackgroundColor(sf::Color::Black);
    pInfos = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pInfos->setBackgroundColor(sf::Color::White);
    rootInfosNode = std::make_unique<Node>("Infos", pInfos, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pInfos, "Informations", *fm.getResourceByAlias(Fonts::Serif));
    pTransform = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pTransform->setBackgroundColor(sf::Color::White);
    rootPropNode = std::make_unique<Node>("Properties", pTransform, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pTransform, "Transform", *fm.getResourceByAlias(Fonts::Serif));
    pMaterial = new Panel(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 700, 0), 0);
    pMaterial->setBackgroundColor(sf::Color::White);
    rootMaterialNode = std::make_unique<Node>("Material", pMaterial, Vec2f(0.f, 0.05f), Vec2f(1.f, 1.f-0.05f));
    tabPane->addTab(pMaterial,"Material",*fm.getResourceByAlias(Fonts::Serif));
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
void ODFAEGCreator::updateScriptText(Shape* shape, Texture* text) {
    /*TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
    std::string relativePath = tm.getPathByResource(text);
    unsigned int id = tm.getIdByResource(text);
    if(cppAppliContent.find("text"+conversionUIntString(id)) == std::string::npos) {
        unsigned int pos = cppAppliContent.find("TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");");
        std::string subs = cppAppliContent.substr(pos);
        pos += subs.find_first_of('\n') + 1;
        cppAppliContent.insert(pos, "const Texture* text"+conversionUIntString(id)+" = tm.getResourceByAlias(\"+realtivePath+\");\n");
    }
    if (cppAppliContent.find("shape"+conversionUIntString(shape->getId())+"->setTexture") == std::string::npos) {
        unsigned int pos = cppAppliContent.find("shape"+conversionUIntString(shape->getId()));
        std::string subs = cppAppliContent.substr(pos);
        pos += subs.find_first_of('\n') + 1;
        cppAppliContent.insert(pos,"shape"+conversionUIntString(shape->getId())+"->setTexture(text"+conversionUIntString(id)+");\n");
    } else {
    }*/
}
void ODFAEGCreator::onRender(RenderComponentManager *cm) {

}
void ODFAEGCreator::onDisplay(RenderWindow* window) {
    if (window == &getRenderWindow()) {
        std::vector<Entity*> visibleEntities = World::getVisibleEntities("*");
        for (unsigned int i = 0; i < visibleEntities.size(); i++) {
            if (!visibleEntities[i]->getDrawOnComponent())
                window->draw(*visibleEntities[i]);
        }
        for (unsigned int i = 0; i < shapes.size(); i++)
            window->draw(*shapes[i]);

        for (unsigned int i = 0; i < entities.size(); i++) {
            window->draw(*entities[i]);
        }
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
        if (showRectSelect) {
            window->draw(rectSelect);
        }
        window->setView(currentView);

    }
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
    if (&getRenderWindow() == window && event.type == IEvent::MOUSE_BUTTON_EVENT && event.mouseButton.type == IEvent::BUTTON_EVENT_PRESSED && event.mouseButton.button == IMouse::Right) {
        sf::Vector2f mousePos (event.mouseButton.x, event.mouseButton.y);
        if (showRectSelect && !pScriptsFiles->isMouseInside()) {
            if (alignToGrid) {
                int x = ((int) mousePos.x / gridWidth * gridWidth)-getRenderWindow().getSize().x * 0.5f;
                int y = ((int) mousePos.y / gridHeight * gridHeight)-getRenderWindow().getSize().y * 0.5f;
                mousePosition = Vec3f(x, y, 0);
            } else {
                int x = mousePos.x-getRenderWindow().getSize().x * 0.5f;
                int y = mousePos.y-getRenderWindow().getSize().y * 0.5f;
                mousePosition = Vec3f(x, y, 0);
            }
        } else if (pScriptsFiles->isMouseInside()) {
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
        if (showRectSelect && !pScriptsFiles->isMouseInside()) {
            if (alignToGrid) {
                int x = ((int) mousePos.x / gridWidth * gridWidth)-getRenderWindow().getSize().x * 0.5f;
                int y = ((int) mousePos.y / gridHeight * gridHeight)-getRenderWindow().getSize().y * 0.5f;
                int width = x - mousePosition.x;
                int height = y - mousePosition.y;
                if (width > 0 && height > 0)
                    rectSelect.setRect(mousePosition.x, mousePosition.y, width, height);

            } else {
                int x = mousePos.x-getRenderWindow().getSize().x * 0.5f;
                int y = mousePos.y-getRenderWindow().getSize().y * 0.5f;
                int width = x - mousePosition.x;
                int height = y - mousePosition.y;
                if (width > 0 && height > 0)
                    rectSelect.setRect(mousePosition.x, mousePosition.y, width, height);
            }
        }
        if (alignToGrid) {
            int x = ((int) mousePos.x / gridWidth * gridWidth);
            int y = ((int) mousePos.y / gridHeight * gridHeight);
            int width = x - mousePosition.x;
            int height = y - mousePosition.y;
            if (pScriptsFiles->isMouseInside()) {
                if (width > 0 && height > 0) {
                    sTextRect->setPosition(Vec3f(mousePosition.x, mousePosition.y, 0));
                    sTextRect->setSize(Vec3f(width, height, 0));
                }
            }
        } else {
            int x = mousePos.x;
            int y = mousePos.y;
            int width = x - mousePosition.x;
            int height = y - mousePosition.y;
            if (pScriptsFiles->isMouseInside()) {
                if (width > 0 && height > 0) {
                    sTextRect->setPosition(Vec3f(mousePosition.x, mousePosition.y, 0));
                    sTextRect->setSize(Vec3f(width, height, 0));
                }
            }
        }
    }
    if (&getRenderWindow() == window && event.type == IEvent::MOUSE_BUTTON_EVENT && event.mouseButton.type == IEvent::BUTTON_EVENT_RELEASED && event.mouseButton.button == IMouse::Right) {
        if (showRectSelect && !pScriptsFiles->isMouseInside()) {
            rectSelect.getItems().clear();
            if (World::getCurrentEntityManager() != nullptr) {
                std::vector<Entity*> entities = World::getVisibleEntities("*");
                for (unsigned int i = 0; i < entities.size(); i++) {
                    if (rectSelect.getSelectionRect().intersects(entities[i]->getGlobalBounds())) {
                        rectSelect.getItems().push_back(entities[i]);
                    }
                }
            }
            for (unsigned int i = 0; i < entities.size(); i++) {
                if (rectSelect.getSelectionRect().intersects(entities[i]->getGlobalBounds())) {
                    rectSelect.getItems().push_back(entities[i].get());
                }
            }
            for (unsigned int i = 0; i < shapes.size(); i++) {
                if (rectSelect.getSelectionRect().intersects(shapes[i]->getGlobalBounds())) {
                    std::cout<<"add shape to selection : "<<shapes[i].get()<<std::endl;
                    rectSelect.getItems().push_back(shapes[i].get());
                }
            }
            if (rectSelect.getItems().size() > 0)
                selectedObject = rectSelect.getItems()[0];
        }
        if (pScriptsFiles->isMouseInside()) {
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
    }
    if (wNewEntitiesUpdater == window && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {
        wNewEntitiesUpdater->setVisible(false);
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
        /*std::string appliDir = fdTexturePath->getAppiDir();
        std::cout<<"appli dir : "<<appliDir<<std::endl;
        std::string relativePath = path.substr(appliDir.size()+1);*/
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        tm.fromFileWithAlias(path, ImgName);
        textPaths.push_back(ImgName);
        /*unsigned int pos = cppAppliContent.find("TextureManager<>& tm");
        std::string subs = cppAppliContent.substr(pos);
        pos += subs.find_first_of('\n') + 1;
        cppAppliContent.insert(pos, "    tm.fromFileWithAlias("+relativePath+","+relativePath+");");*/
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
            while(getline(applis, line)) {
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
        applis.close();
        const char* lcars = appliname.c_str();
        char* ucars = new char[appliname.size()];
        for (unsigned int i = 0; i < appliname.length(); i++) {
            ucars[i] = std::tolower(lcars[i]);
        }
        minAppliname = std::string(ucars, appliname.length());
        if (appliname != "") {
            std::string path = fdProjectPath->getAppiDir() + "/" + appliname;
            std::ifstream source (path+"/"+minAppliname+".cpp");
            while(getline(source, line)) {
                cppAppliContent += line+"\n";
            }
            source.close();
        }
        fdProjectPath->setVisible(false);
        fdProjectPath->setEventContextActivated(false);
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
    std::ifstream file(appliname+"/"+lab->getText());
    if (file) {
        std::string content, line;
        unsigned int nbLines = 0;
        while (getline(file, line)) {
            content += line + "\n";
            nbLines++;
        }
        tScriptEdit->setTextSize(20);
        tScriptEdit->setText(content);
        pScriptsEdit->updateScrolls();
        file.close();
    }
}
void ODFAEGCreator::processKeyHeldDown (IKeyboard::Key key) {

}
void ODFAEGCreator::actionPerformed(Button* button) {
    if (button->getText() == "Create") {
        appliname = ta->getText();
        std::ofstream applis(appliname+".oc");
        applis<<appliname<<std::endl;
        applis.close();
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
        bCreateAppli->setEventContextActivated(false);
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
            header<<"#ifndef "<<majAppliname<<"_HPP"<<std::endl;
            header<<"#define "<<majAppliname<<"_HPP"<<std::endl;
            header<<"#include \"odfaeg/Core/application.h\""<<std::endl;
            header<<"class "<<appliname<<" : public odfaeg::core::Application {"<<std::endl;
            header<<"   public : "<<std::endl;
            header<<"   "<<appliname<<"(sf::VideoMode vm, std::string title);"<<std::endl;
            header<<"   void onLoad();"<<std::endl;
            header<<"   void onInit();"<<std::endl;
            header<<"   void onRender(odfaeg::graphic::RenderComponentManager* cm);"<<std::endl;
            header<<"   void onDisplay(odfaeg::graphic::RenderWindow* window);"<<std::endl;
            header<<"   void onUpdate (odfaeg::graphic::RenderWindow*, odfaeg::window::IEvent& event);"<<std::endl;
            header<<"   void onExec ();"<<std::endl;
            header<<"   private : "<<std::endl;
            header<<"   std::vector<std::unique_ptr<Drawable>> drawables;"<<std::endl;
            header<<"   ResourceCache<> cache;"<<std::endl;
            header<<"};"<<std::endl;
            header<<"#endif"<<std::endl;
            header.close();
            std::ostringstream oss;
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
            cppAppliContent = oss.str();
            #if defined (ODFAEG_SYSTEM_LINUX)
            std::ofstream source(path+"/"+minAppliname+".cpp");
            #else if defined (ODFAEG_SYSTEM_WINDOWS)
            std::ofstream source(path+"\\"+minAppliname+".cpp");
            #endif // if
            source<<cppAppliContent;
            source.close();
            std::string width = taWidth->getText();
            std::string height = taHeight->getText();
            #if defined (ODFAEG_SYSTEM_LINUX)
            std::ofstream main(path+"/main.cpp");
            #else if defined (ODFAEG_SYSTEM_WINDOWS)
            std::ofstream main(path+"\\main.cpp");
            #endif // if
            main<<"#include \""<<minAppliname<<".hpp\""<<std::endl;
            main<<"int main(int argc, char* argv[]) {"<<std::endl;
            main<<"    "<<appliname<<" app(sf::VideoMode("<<width<<","<<height<<"),\""<<appliname<<"\");"<<std::endl;
            main<<"    return app.exec();"<<std::endl;
            main<<"}"<<std::endl;
            main.close();
        }
    }
    if (button->getText() == "New texture") {
        fdTexturePath->setVisible(true);
        fdTexturePath->setEventContextActivated(true);
    }
    if (button->getText() == "Create scene") {
        wNewMap->setVisible(false);
        button->setEventContextActivated(false);
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
        button->setEventContextActivated(false);
        std::cout<<"Create component"<<std::endl;
        if (dpComponentType->getSelectedItem() == "LinkedList") {
            PerPixelLinkedListRenderComponent* ppll = new PerPixelLinkedListRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 3, 0));
            getRenderComponentManager().addComponent(ppll);
        }
        if (dpComponentType->getSelectedItem() == "Shadow") {
            ShadowRenderComponent* src = new ShadowRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 3, 0));

            getRenderComponentManager().addComponent(src);
        }
        if (dpComponentType->getSelectedItem() == "Light") {
            LightRenderComponent* lrc = new LightRenderComponent(getRenderWindow(),conversionStringInt(taComponentLayer->getText()),taComponentExpression->getText(),ContextSettings(0, 0, 4, 3, 0));
            getRenderComponentManager().addComponent(lrc);
        }
    }
    if(button==bCreateEntitiesUpdater) {
        std::string name = taEntitiesUpdaterName->getText();
        EntitiesUpdater* eu = new EntitiesUpdater();
        eu->setName(name);
        World::addWorker(eu);
        wNewEntitiesUpdater->setVisible(false);
    }
}
void ODFAEGCreator::actionPerformed(MenuItem* item) {
    if (item->getText() == "New application") {
        std::cout<<"new application"<<std::endl;
        wApplicationNew->setVisible(true);
        bCreateAppli->setEventContextActivated(true);
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
            shape->setPosition(cursor.getPosition());
            displayInfos(shape.get());
            selectedObject = shape.get();
            addShape(shape.get());
            shapes.push_back(std::move(shape));
        }
    }
    if (item->getText() == "Tile") {
        if (appliname != "") {
            if (!showRectSelect) {
                std::unique_ptr<Tile> tile = std::make_unique<Tile>(nullptr, cursor.getPosition(),Vec3f(100, 50, 0), sf::IntRect(0, 0, gridWidth, gridHeight));
                selectedObject = tile.get();
                displayInfos(tile.get());
                if (World::getCurrentEntityManager() == nullptr)
                    entities.push_back(std::move(tile));
                else
                    World::addEntity(tile.release());
            } else {
                BoundingBox rect = rectSelect.getSelectionRect();
                for (int x = rect.getPosition().x; x < rect.getPosition().x + rect.getSize().x-gridWidth; x+=gridWidth) {
                    for (int y = rect.getPosition().y; y <  rect.getPosition().y + rect.getSize().y-gridHeight; y+=gridHeight) {
                        std::unique_ptr<Tile> tile = std::make_unique<Tile>(nullptr,Vec3f(x, y, 0),Vec3f(gridWidth, gridHeight, 0),sf::IntRect(0, 0, gridWidth, gridHeight));
                        rectSelect.addItem(tile.get());
                        if (rectSelect.getItems().size() == 1) {
                            selectedObject = tile.get();
                            displayInfos(tile.get());
                        }
                        if (World::getCurrentEntityManager() == nullptr)
                            entities.push_back(std::move(tile));
                        else
                            World::addEntity(tile.release());
                    }
                }
            }
        }
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
        bCreateScene->setEventContextActivated(true);
    }
    if (item->getText() == "New component") {
        wNewComponent->setVisible(true);
        bCreateComponent->setEventContextActivated(true);
    }
    if (item->getText() == "New entities updater") {
        wNewEntitiesUpdater->setVisible(true);
        bCreateEntitiesUpdater->setEventContextActivated(true);
    }
    if (item == item15) {
        fdProjectPath->setVisible(true);
        fdProjectPath->setEventContextActivated(true);
    }
}
void ODFAEGCreator::addShape(Shape *shape) {
    /*unsigned int pos = cppAppliContent.find("tm.getResourceByAlias");
    if (pos != std::string::npos && pos < cppAppliContent.size()) {
        std::string subs = cppAppliContent.substr(pos);
        pos += subs.find_first_of('\n') + 1;
        while (subs.find("tm.getResourceByAlias") != std::string::npos) {
            subs = cppAppliContent.substr(pos);
            pos += subs.find_first_of('\n') + 1;
        }
    } else {
        pos = cppAppliContent.find("TextureManager<>& tm = cache.resourceManager<Texture, std::string>(\"TextureManager\");");
        std::string subs = cppAppliContent.substr(pos);
        pos += subs.find_first_of('\n') + 1;
    }
    std::string toInsert = "    std::unique_ptr<sf::RectangleShape> shape"+conversionUIntString(shape->getId())+" = std::make_unique<RectangleShape>(Vec3f(100, 50, 0));\n"
                           "    drawables.push_back(std::move(shape));\n";
    cppAppliContent.insert(pos, toInsert);*/
}
bool ODFAEGCreator::removeShape (unsigned int id) {
    for (auto it = shapes.begin(); it != shapes.end();it++) {
        if ((*it)->getId() == id) {
            /*unsigned int pos = cppAppliContent.find("shape"+conversionUIntString(id));
            do {
                std::string subs = cppAppliContent.substr(pos);
                unsigned int endpos = pos + subs.find_first_of('\n') + 1;
                cppAppliContent.erase(pos, pos - endpos);
                pos = cppAppliContent.find("shape"+conversionUIntString(id));
            } while(pos != std::string::npos);*/
            it = shapes.erase(it);
            return true;
        }
    }
    return false;
}
void ODFAEGCreator::displayInfos (Shape* shape) {
    rootPropNode->deleteAllNodes();
    rootMaterialNode->deleteAllNodes();
    rootInfosNode->deleteAllNodes();
    pTransform->removeAll();
    pMaterial->removeAll();
    pInfos->removeAll();
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lId = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Id : shape-"+conversionIntString(shape->getId()), 15);
    lId->setParent(pInfos);
    Node* lIdNode = new Node("LabId", lId, Vec2f(0, 0), Vec2f(1, 0.025), rootInfosNode.get());
    pInfos->addChild(lId);
    lPosition = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Position : ", 15);
    lPosition->setParent(pTransform);
    Node* lPosNode = new Node("LabPosition",lPosition,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lPosition);
    lPosX = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"x : ",15);
    lPosX->setParent(pTransform);
    Node* lPosXNode = new Node("LabX",lPosX,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosX);
    tPosX = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getPosition().x),getRenderWindow());
    tPosX->setParent(pTransform);
    tPosX->setTextSize(15);
    lPosXNode->addOtherComponent(tPosX, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosX);
    lPosY = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"y : ",15);
    lPosY->setParent(pTransform);
    Node* lPosYNode = new Node("LabY",lPosY,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosY);
    tPosY = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getPosition().y),getRenderWindow());
    tPosY->setParent(pTransform);
    tPosY->setTextSize(15);
    lPosYNode->addOtherComponent(tPosY, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosY);
    lPosZ = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"z : ",15);
    lPosZ->setParent(pTransform);
    Node* lPosZNode = new Node("LabZ",lPosZ,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosZ);
    tPosZ = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(shape->getPosition().z),getRenderWindow());
    tPosZ->setParent(pTransform);
    tPosZ->setTextSize(15);
    lPosZNode->addOtherComponent(tPosZ, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosZ);
    Action a (Action::EVENT_TYPE::TEXT_ENTERED);
    Command cmdPosX (a, FastDelegate<bool>(&TextArea::isTextChanged, tPosX), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosX));
    Command cmdPosY (a, FastDelegate<bool>(&TextArea::isTextChanged, tPosY), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosY));
    Command cmdPosZ (a, FastDelegate<bool>(&TextArea::isTextChanged, tPosZ), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosZ));
    tPosX->getListener().connect("tPosXChanged", cmdPosX);
    tPosY->getListener().connect("tPosYChanged", cmdPosY);
    tPosZ->getListener().connect("tPosZChanged", cmdPosZ);
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
    Command cmdRColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tRColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tRColor));
    tRColor->getListener().connect("TRColorChanged", cmdRColChanged);
    Command cmdGColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tGColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tGColor));
    tGColor->getListener().connect("TGColorChanged", cmdGColChanged);
    Command cmdBColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tBColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tBColor));
    tBColor->getListener().connect("TBColorChanged", cmdBColChanged);
    Command cmdAColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tAColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tAColor));
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
    Command cmdTexCoordXChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordX), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordX));
    tTexCoordX->getListener().connect("TTexCoordXChanged", cmdTexCoordXChanged);
    Command cmdTexCoordYChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordY), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordY));
    tTexCoordY->getListener().connect("TTexCoordYChanged", cmdTexCoordYChanged);
    Command cmdTexCoordWChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordW), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordW));
    tTexCoordW->getListener().connect("TTexCoordWChanged", cmdTexCoordWChanged);
    Command cmdTexCoordHChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordH), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordH));
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
    int id = conversionStringInt(parts[1]);
    Entity* entity = World::getEntity(id);
    if (node->getNodes().size() == 0) {
        FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
        for (unsigned int i = 0; i < entity->getChildren().size(); i++) {
            Label* label = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Id : entity-"+conversionIntString(entity->getChild(i)->getId()), 15);
            Action a(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
            Command cmd(a, FastDelegate<bool>(&Label::isMouseInside, label), FastDelegate<void>(&ODFAEGCreator::displayChildren, this, label));
            label->getListener().connect("SHOWCHILDREN"+label->getText(), cmd);
            label->setParent(pInfos);
            Node* lNode = new Node(label->getText() + "-child-" + conversionIntString(i), label, Vec2f(0, 0), Vec2f(1.f, 0.025f), node);
            pInfos->addChild(label);
        }
    } else if (node->isNodeVisible()) {
        node->hideAllNodes();
    } else {
        node->showAllNodes();
    }
}
void ODFAEGCreator::displayInfos (Tile* tile) {
    rootPropNode->deleteAllNodes();
    rootMaterialNode->deleteAllNodes();
    rootInfosNode->deleteAllNodes();
    pTransform->removeAll();
    pMaterial->removeAll();
    pInfos->removeAll();
    FontManager<Fonts>& fm = cache.resourceManager<Font, Fonts>("FontManager");
    Label* lId = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif), "Id : entity-"+conversionIntString(tile->getId()), 15);
    Action aLId(Action::EVENT_TYPE::MOUSE_BUTTON_PRESSED_ONCE, IMouse::Left);
    Command cmd(aLId, FastDelegate<bool>(&Label::isMouseInside, lId), FastDelegate<void>(&ODFAEGCreator::displayChildren, this, lId));
    lId->getListener().connect("SHOWCHILDREN"+lId->getText(), cmd);
    lId->setParent(pInfos);
    Node* lIdNode = new Node("LabId", lId, Vec2f(0, 0), Vec2f(1, 0.025), rootInfosNode.get());
    pInfos->addChild(lId);
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
    lEmListNode->addOtherComponent(dpSelectEm, Vec2f(0.75, 0.025));
    dpSelectEm->setPriority(-2);
    Label* lSelectRT = new Label(getRenderWindow(),Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),"Render target : ", 15);
    lSelectRT->setParent(pInfos);
    pInfos->addChild(lSelectRT);
    Node* selectRT = new Node("SelectRT", lSelectRT, Vec2f(0, 0), Vec2f(0.25, 0.025), rootInfosNode.get());
    DropDownList* dpSelectRT = new DropDownList(getRenderWindow(),Vec3f(0, 0, 0),Vec3f(100, 50, 0), fm.getResourceByAlias(Fonts::Serif),"COMPONENTS", 15);
    dpSelectRT->setName("DPSELECTRT");
    dpSelectRT->addItem("WINDOW", 15);
    dpSelectRT->setParent(pInfos);
    pInfos->addChild(dpSelectRT);
    Command cmdRTChanged(FastDelegate<bool>(&DropDownList::isValueChanged, dpSelectRT), FastDelegate<void>(&ODFAEGCreator::onSelectedRTChanged, this, dpSelectRT));
    dpSelectRT->getListener().connect("RTChanged", cmdRTChanged);
    selectRT->addOtherComponent(dpSelectRT, Vec2f(0.75, 0.025));
    lPosition = new Label(getRenderWindow(),Vec3f(0,0,0),Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"Position : ", 15);
    lPosition->setParent(pTransform);
    Node* lPosNode = new Node("LabPosition",lPosition,Vec2f(0, 0), Vec2f(1, 0.025),rootPropNode.get());
    pTransform->addChild(lPosition);
    lPosX = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"x : ",15);
    lPosX->setParent(pTransform);
    Node* lPosXNode = new Node("LabX",lPosX,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosX);
    tPosX = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getPosition().x),getRenderWindow());
    tPosX->setParent(pTransform);
    tPosX->setTextSize(15);
    lPosXNode->addOtherComponent(tPosX, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosX);
    lPosY = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"y : ",15);
    lPosY->setParent(pTransform);
    Node* lPosYNode = new Node("LabY",lPosY,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosY);
    tPosY = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getPosition().y),getRenderWindow());
    tPosY->setParent(pTransform);
    tPosY->setTextSize(15);
    lPosYNode->addOtherComponent(tPosY, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosY);
    lPosZ = new Label(getRenderWindow(), Vec3f(0, 0, 0), Vec3f(200, 17, 0),fm.getResourceByAlias(Fonts::Serif),"z : ",15);
    lPosZ->setParent(pTransform);
    Node* lPosZNode = new Node("LabZ",lPosZ,Vec2f(0, 0),Vec2f(0.25, 0.025), rootPropNode.get());
    pTransform->addChild(lPosZ);
    tPosZ = new TextArea(Vec3f(0, 0, 0), Vec3f(100, 50, 0),fm.getResourceByAlias(Fonts::Serif),conversionFloatString(tile->getPosition().z),getRenderWindow());
    tPosZ->setParent(pTransform);
    tPosZ->setTextSize(15);
    lPosZNode->addOtherComponent(tPosZ, Vec2f(0.75, 0.025));
    pTransform->addChild(tPosZ);
    Action a (Action::EVENT_TYPE::TEXT_ENTERED);
    Command cmdPosX (a, FastDelegate<bool>(&TextArea::isTextChanged, tPosX), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosX));
    Command cmdPosY (a, FastDelegate<bool>(&TextArea::isTextChanged, tPosY), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosY));
    Command cmdPosZ (a, FastDelegate<bool>(&TextArea::isTextChanged, tPosZ), FastDelegate<void>(&ODFAEGCreator::onObjectPosChanged, this,tPosZ));
    tPosX->getListener().connect("tPosXChanged", cmdPosX);
    tPosY->getListener().connect("tPosYChanged", cmdPosY);
    tPosZ->getListener().connect("tPosZChanged", cmdPosZ);
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
    Command cmdRColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tRColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tRColor));
    tRColor->getListener().connect("TRColorChanged", cmdRColChanged);
    Command cmdGColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tGColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tGColor));
    tGColor->getListener().connect("TGColorChanged", cmdGColChanged);
    Command cmdBColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tBColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tBColor));
    tBColor->getListener().connect("TBColorChanged", cmdBColChanged);
    Command cmdAColChanged(a, FastDelegate<bool>(&TextArea::isTextChanged, tAColor), FastDelegate<void>(&ODFAEGCreator::onObjectColorChanged, this, tAColor));
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
    Command cmdTexCoordXChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordX), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordX));
    tTexCoordX->getListener().connect("TTexCoordXChanged", cmdTexCoordXChanged);
    Command cmdTexCoordYChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordY), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordY));
    tTexCoordY->getListener().connect("TTexCoordYChanged", cmdTexCoordYChanged);
    Command cmdTexCoordWChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordW), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordW));
    tTexCoordW->getListener().connect("TTexCoordWChanged", cmdTexCoordWChanged);
    Command cmdTexCoordHChanged (a, FastDelegate<bool>(&TextArea::isTextChanged,tTexCoordH), FastDelegate<void>(&ODFAEGCreator::onTexCoordsChanged, this, tTexCoordH));
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
                BaseChangementMatrix bcm = World::getBaseChangementMatrix();
                int x = mousePos.x-getRenderWindow().getView().getSize().x * 0.5f;
                int y = mousePos.y-getRenderWindow().getView().getSize().y * 0.5f;
                Vec3f c = bcm.unchangeOfBase(Vec3f(x, y, 0));
                Vec3f v1, p;
                p.x = (int) c.x / gridWidth;
                p.y = (int) c.y / gridHeight;
                if (c.x < 0)
                    p.x--;
                if (c.y < 0)
                    p.y--;
                v1.x = p.x * gridWidth;
                v1.y = p.y * gridHeight;
                Vec3f newPos = bcm.changeOfBase(Vec3f(v1.x, v1.y, 0));
                if (bcm.isIso2DMatrix())
                    newPos.x -= gridWidth * 0.5f;
                cursor.setPosition(newPos);
            }
        }
    }
}
void ODFAEGCreator::updateScriptPos(Transformable* shape) {
    /*if(cppAppliContent.find("shape"+conversionUIntString(shape->getId())+"->setPosition") == std::string::npos) {
        unsigned int pos = cppAppliContent.find("shape"+conversionUIntString(shape->getId())+" = std::make_unique<RectangleShape>");
        std::string subs = cppAppliContent.substr(pos);
        pos += subs.find_first_of('\n') + 1;
        cppAppliContent.insert(pos,"    shape"+conversionUIntString(shape->getId())+"->setPosition(Vec3f("+conversionIntString(shape->getPosition().x)+","
        +conversionIntString(selectedObject->getPosition().y)+","+conversionIntString(selectedObject->getPosition().z)+");\n");
    } else {
        unsigned int pos = cppAppliContent.find("shape"+conversionUIntString(shape->getId())+"->setPosition");
        std::string subs = cppAppliContent.substr(pos);
        unsigned int endpos = subs.find_first_of('\n') + pos + 1;
        cppAppliContent.erase(pos, endpos - pos);
        cppAppliContent.insert(pos,"shape"+conversionUIntString(shape->getId())+"->setPosition(Vec3f("+conversionIntString(shape->getPosition().x)+","
        +conversionIntString(shape->getPosition().y)+","+conversionIntString(shape->getPosition().z)+");\n");
    }*/
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
        }
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
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Tile*>(selectedObject)->getColor().g,static_cast<Tile*>(selectedObject)->getColor().b, static_cast<Tile*>(selectedObject)->getColor().a));
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGERCOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Shape*>(selectedObject)->getFillColor().g,static_cast<Shape*>(selectedObject)->getFillColor().b, static_cast<Shape*>(selectedObject)->getFillColor().a));
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(Math::clamp(color, 0, 255), static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g,static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a));
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
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(static_cast<Shape*>(selectedObject)->getFillColor().r, Math::clamp(color, 0, 255),static_cast<Shape*>(selectedObject)->getFillColor().b, static_cast<Shape*>(selectedObject)->getFillColor().a));
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(static_cast<Tile*>(selectedObject)->getColor().r, Math::clamp(color, 0, 255),static_cast<Tile*>(selectedObject)->getColor().b, static_cast<Tile*>(selectedObject)->getColor().a));
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEGCOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().g);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r, Math::clamp(color, 0, 255),static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().b, static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().a));
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r, Math::clamp(color, 0, 255),static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a));
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
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(static_cast<Shape*>(selectedObject)->getFillColor().r, static_cast<Shape*>(selectedObject)->getFillColor().g, Math::clamp(color, 0, 255), static_cast<Shape*>(selectedObject)->getFillColor().a));
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(static_cast<Tile*>(selectedObject)->getColor().r, static_cast<Tile*>(selectedObject)->getColor().g, Math::clamp(color, 0, 255), static_cast<Tile*>(selectedObject)->getColor().a));
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEBCOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().b);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r, static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().g, Math::clamp(color, 0, 255), static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().a));
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g, Math::clamp(color, 0, 255), static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a));
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
            state->addParameter("NEWVALUE", color);
            sg->addState(state);
            if (dynamic_cast<Shape*>(selectedObject)) {
                static_cast<Shape*>(selectedObject)->setFillColor(sf::Color(static_cast<Shape*>(selectedObject)->getFillColor().r, static_cast<Shape*>(selectedObject)->getFillColor().g,static_cast<Shape*>(selectedObject)->getFillColor().b, Math::clamp(color, 0, 255)));
            }
            if (dynamic_cast<Tile*>(selectedObject)) {
                static_cast<Tile*>(selectedObject)->setColor(sf::Color(static_cast<Tile*>(selectedObject)->getColor().r, static_cast<Tile*>(selectedObject)->getColor().g,static_cast<Tile*>(selectedObject)->getColor().b, Math::clamp(color, 0, 255)));
            }
            for (unsigned int i = 1; i < rectSelect.getItems().size(); i++) {
                State* selectState = new State("SCHANGEACOLOR", &se);
                selectState->addParameter("OBJECT", rectSelect.getItems()[i]);
                if (dynamic_cast<Shape*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().a);
                    static_cast<Shape*>(rectSelect.getItems()[i])->setFillColor(sf::Color(static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().r, static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().g,static_cast<Shape*>(rectSelect.getItems()[i])->getFillColor().b, Math::clamp(color, 0, 255)));
                }
                if (dynamic_cast<Tile*>(rectSelect.getItems()[i])) {
                    selectState->addParameter("OLDVALUE", static_cast<Tile*>(rectSelect.getItems()[i])->getColor().a);
                    static_cast<Tile*>(rectSelect.getItems()[i])->setColor(sf::Color(static_cast<Tile*>(rectSelect.getItems()[i])->getColor().r, static_cast<Tile*>(rectSelect.getItems()[i])->getColor().g,static_cast<Tile*>(rectSelect.getItems()[i])->getColor().b, Math::clamp(color, 0, 255)));
                }
                selectState->addParameter("NEWVALUE", color);
                sg->addState(selectState);
            }
            stateStack.addStateGroup(sg);
        }
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
        }
        if (dynamic_cast<Tile*>(selectedObject)) {
            Tile* selectedTile = static_cast<Tile*>(selectedObject);
            selectedTile->getFace(0)->getMaterial().clearTextures();
            selectedTile->getFace(0)->getMaterial().addTexture(nullptr, sf::IntRect(0, 0, gridWidth, gridHeight));
        }
    } else {
        TextureManager<>& tm = cache.resourceManager<Texture, std::string>("TextureManager");
        /*for (unsigned int i = 0; i < textPaths.size(); i++) {
            std::cout<<"searching tex path : "<<dp->getSelectedItem()<<std::endl;
            if (textPaths[i].find(dp->getSelectedItem())) {*/
                const Texture* text = tm.getResourceByAlias(dp->getSelectedItem());
                Sprite sprite (*text, Vec3f(0, bChooseText->getPosition().y + bChooseText->getSize().y, 0),Vec3f(text->getSize().x, text->getSize().y, 0),sf::IntRect(0, 0, text->getSize().x,text->getSize().y));
                pMaterial->addSprite(sprite);
                sf::IntRect textRect;
                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTexture(text);
                    textRect = static_cast<Shape*>(selectedObject)->getTextureRect();
                }
                if (dynamic_cast<Tile*>(selectedObject)) {
                    static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().clearTextures();
                    static_cast<Tile*>(selectedObject)->getFace(0)->getMaterial().addTexture(text, sf::IntRect(0, 0, text->getSize().x, text->getSize().y));
                    textRect = sf::IntRect(0, 0, text->getSize().x, text->getSize().y);
                }
                tTexCoordX->setText(conversionIntString(textRect.left));
                tTexCoordY->setText(conversionIntString(textRect.top));
                tTexCoordW->setText(conversionIntString(textRect.width));
                tTexCoordH->setText(conversionIntString(textRect.height));
                sTextRect = new RectangleShape(Vec3f(textRect.width, textRect.height, 0));
                sTextRect->setPosition(Vec3f(textRect.left, textRect.top + bChooseText->getPosition().y + bChooseText->getSize().y, 0));
                sTextRect->setFillColor(sf::Color::Transparent);
                sTextRect->setOutlineColor(sf::Color::Red);
                sTextRect->setOutlineThickness(1);
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
    if (tex != nullptr) {
        if (ta == tTexCoordX) {
            if (is_number(ta->getText())) {
                int texCoordX = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEXTEXCOORD");
                State* state = new State("SCHANGEXTEYCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().left);
                }
                state->addParameter("NEWVALUE", texCoordX);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);
                stateStack.addStateGroup(sg);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(Math::abs(texCoordX), texRect.top, texRect.width, texRect.height));
                }
            }
        }
        if (ta == tTexCoordY) {
            if (is_number(ta->getText())) {
                int texCoordY = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEYTEXCOORD"+conversionIntString(texCoordY));
                State* state = new State("SCHANGEXTEYCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().top);
                }
                state->addParameter("NEWVALUE", texCoordY);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);
                stateStack.addStateGroup(sg);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(texRect.left, Math::abs(texCoordY), texRect.width, texRect.height));
                }
            }
        }
        if (ta == tTexCoordW) {
            if (is_number(ta->getText())) {
                int texCoordW = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEWTEWCOORD"+conversionIntString(texCoordW));
                State* state = new State("SCHANGEXTEWCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().width);
                }
                state->addParameter("NEWVALUE", texCoordW);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);
                stateStack.addStateGroup(sg);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(texRect.left, texRect.top, Math::abs(texCoordW), texRect.height));
                }
            }
        }
        if (ta == tTexCoordH) {
            if (is_number(ta->getText())) {
                int texCoordH = conversionStringInt(ta->getText());
                StateGroup* sg = new StateGroup("SGCHANGEHTEXCOORD"+conversionIntString(texCoordH));
                State* state = new State("SCHANGEXTEHCOORD", &se);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    state->addParameter("OLDVALUE", static_cast<Shape*>(selectedObject)->getTextureRect().height);
                }
                state->addParameter("NEWVALUE", texCoordH);
                state->addParameter("OBJECT", selectedObject);
                sg->addState(state);
                stateStack.addStateGroup(sg);
                if (dynamic_cast<Shape*>(selectedObject)) {
                    static_cast<Shape*>(selectedObject)->setTextureRect(sf::IntRect(texRect.left, texRect.top, texRect.width, Math::abs(texCoordH)));
                }
            }
        }
    }
}
void ODFAEGCreator::onSelectedEmChanged(DropDownList* dp) {
    if (dp->getSelectedItem() == "NONE") {
        if (dynamic_cast<Entity*>(selectedObject)) {
            if (World::getCurrentEntityManager != nullptr)
                World::removeEntity(dynamic_cast<Entity*>(selectedObject));
            std::unique_ptr<Entity> entity;
            entity.reset(dynamic_cast<Entity*>(selectedObject));
            entities.push_back(std::move(entity));
        }
    } else {
        if (dynamic_cast<Entity*>(selectedObject)) {
            std::vector<std::unique_ptr<Entity>>::iterator it;
            Entity* entity;
            for (it = entities.begin(); it != entities.end(); it++) {
                if ((*it).get() == dynamic_cast<Entity*>(selectedObject)) {
                   entity = it->release();
                }
            }
            const auto itEntityToRemove =
                std::find_if(entities.begin(), entities.end(),
                             [&](auto& p) { return p.get() == dynamic_cast<Entity*>(selectedObject); });

            const bool found = (itEntityToRemove != entities.end());
            if(found)
                entities.erase(itEntityToRemove);
            World::setCurrentEntityManager(dp->getSelectedItem());
            World::addEntity(entity);
        }
    }
}
void ODFAEGCreator::onSelectedRTChanged(DropDownList *dp) {
    if (dp->getSelectedItem() == "WINDOW") {
        if (dynamic_cast<Entity*>(selectedObject)) {
            static_cast<Entity*>(selectedObject)->setDrawOnComponent(false);
        }
    } else {
        if (dynamic_cast<Entity*>(selectedObject)) {
            static_cast<Entity*>(selectedObject)->setDrawOnComponent(true);
        }
    }
}
