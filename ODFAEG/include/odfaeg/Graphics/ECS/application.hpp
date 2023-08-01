#ifndef ODFAEG_ECS_APPLICATION_HPP
#define ODFAEG_ECS_APPLICATION_HPP
#include <string>
#include "../../Network/network.hpp"
#include "../renderWindow.h"
#include "../renderStates.h"
#include "../renderComponentManager.h"
#include "world.hpp"
#include "../../Window/listener.h"

#include "../../Window/iEvent.hpp"
#include "../../Core/erreur.h"
namespace odfaeg {
    namespace graphic {
        namespace ecs {
            template <typename D, typename Alias=std::string>
            class Application {
                public :
                Application(sf::VideoMode vm, std::string title, sf::Uint32 style = sf::Style::Default, window::ContextSettings settings = window::ContextSettings())

                {
                    graphic::RenderWindow* window = new graphic::RenderWindow (vm, title, style, settings);
                    windows.push_back(std::make_pair(window, true));
                    componentManager = std::make_unique<graphic::RenderComponentManager>(*window);
                    running = false;
                    sf::Clock loopSpeed;
                    addClock(loopSpeed, "LoopTime");
                    sf::Clock timeClock;
                    addClock(timeClock, "TimeClock");
                    listener = std::make_unique<core::Listener>();
                    eventContextActivated = true;
                    nbEntities = nbEntitiesTypes = nbComponents = nbMaterials = 0;
                }
                Application () {
                    running = false;
                    sf::Clock loopTime;
                    addClock(loopTime, "LoopTime");
                    sf::Clock timeClock;
                    addClock(timeClock, "TimeClock");
                    eventContextActivated = true;
                    nbEntities = nbEntitiesTypes = nbComponents = nbMaterials = 0;
                }
                unsigned int getNbMaterials() {
                    return nbMaterials;
                }
                void setNbMaterials(unsigned int nbMaterials) {
                    this->nbMaterials = nbMaterials;
                }
                std::vector<graphic::Material*>& getMaterials() {
                    return materials;
                }
                std::vector<graphic::Material*>& getSameMaterials() {
                    return sameMaterials;
                }
                int getIntOfType(std::string sType) {
                    std::map<int, std::string>::iterator it;
                    for (it = types.begin(); it != types.end(); ++it) {
                        if (it->second == sType)
                            return it->first;
                    }
                    return -1;
                }
                int getUniqueId() {
                    nbEntities++;
                    return nbEntities;
                }
                int getComponentId() {
                    nbComponents++;
                    return nbComponents;
                }
                std::pair<int, std::string> updateTypes(std::string sType) {
                    int iType = getIntOfType(sType);
                    if (iType == -1) {
                        std::pair<int, std::string> type = std::pair<int, std::string> (nbEntitiesTypes, sType);
                        types.insert(type);
                        nbEntitiesTypes++;
                        return type;
                    } else {
                        std::map<int, std::string>::iterator it = types.find(iType);
                        return *it;
                    }
                }
                unsigned int getNbEntities() {
                    return nbEntities;
                }
                unsigned int getNbEntitiesTypes() {
                    return nbEntitiesTypes;
                }
                void addWindow(graphic::RenderWindow* window, bool holdWindow = true) {
                    if (windows.size() != 0) {
                        windows.push_back(std::make_pair(window, holdWindow));
                        componentManager->addWindow(*window);
                    }
                }
                /** \fn int exec()
                *   \brief main loop of the odfaeg application.
                *   \return true if the application runs without errors, false otherwise.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                int exec(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca) {
                    if (!running) {
                        load();
                        init(sya, sa, ra, eca);
                        running = true;
                    }
                    while (running) {
                        /*for (unsigned int i = 0; i < windows.size(); i++) {
                            windows[i].first->setActive(false);
                        }*/
                        if (windows.size() != 0 && windows[0].first->isOpen()) {
                            //rendering_thread = std::thread(Application::render, this);
                            render(sya, sa, ra, eca);
                            update(sya, sa, ra, eca);
                        }
                        if (network::Network::getCliInstance().isRunning()) {
                            network::Network::getCliInstance().checkMessages();
                        }
                        if (network::Network::getSrvInstance().isRunning()) {
                            network::Network::getSrvInstance().checkMessages();
                        }
                        static_cast<D&>(*this).onExec(sya, sa, ra, eca);
                        getClock("LoopTime").restart();
                    }
                    return EXIT_SUCCESS;
                }
                /** \fn void stop()
                *   \brief stop the odfaeg application and close the window if it's a graphic application.
                */
                void stop() {
                    running = false;
                    //rendering_thread.join();
                    for(unsigned int i = 0; i < windows.size(); i++)
                        windows[i].first->close();
                }
                /** \fn void load()
                *   \brief call the onLoad function, this is where all resources used by the application are loaded.
                */
                void load() {
                    static_cast<D&>(*this).onLoad();
                }
                /** \fn void init()
                *   \brief call the onInit function, this is where all the entities used by the application are initialized.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void init(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca) {
                    auto sya2 = world.initSystems(sya);
                    static_cast<D&>(*this).onInit(sya2, sa, ra, eca);
                }
                /** \fn void render()
                *   \brief call the rendering functions used to render entities on components or on the window.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void render(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca) {
                    if (windows.size() != 0 && windows[0].first->isOpen()) {
                        for (unsigned int i = 0; i < windows.size(); i++) {
                            windows[i].first->clear(clearColor);
                        }
                        static_cast<D&>(*this).onRender(sya, sa, ra, eca, componentManager.get());
                        componentManager->clearComponents();
                        if (eventContextActivated) {
                           listener->processEvents();
                        }
                        world.draw(sya, ra);

                        static_cast<D&>(*this).onDisplay(sya, sa, ra, eca, windows[0].first);
                        componentManager->drawGuiComponents();
                        for (unsigned int i = 0; i < windows.size(); i++)
                            windows[i].first->display();
                    }
                }
                void setEventContextActivated(bool eventContextActivated) {
                    this->eventContextActivated = eventContextActivated;
                    if (!eventContextActivated)
                        listener->clearEventsStack();
                }
                bool isEventContextActivated() {
                    return eventContextActivated;
                }
                /** \fn void update()
                *   \brief call the updates functions to update the entities
                *   filter the window::IEvents and pass window events which are generated to the listener.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void update(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca) {
                    if (windows.size() != 0) {
                        window::IEvent event;
                        events.clear();
                        for (unsigned int i = 0; i < windows.size(); i++) {
                            while (windows[i].first->pollEvent(event)) {
                                events.insert(std::make_pair(windows[i].first, event));
                            }
                        }
                        if (events.size() > 0) {
                            for (it = events.begin(); it != events.end(); it++) {
                                static_cast<D&>(*this).onUpdate(sya, sa, ra, eca, it->first, it->second);
                                if (eventContextActivated) {
                                    listener->pushEvent(it->second);
                                }
                                for (unsigned int i = 0; i < componentManager->getNbComponents(); i++) {
                                    componentManager->getComponent(i)->onUpdate(it->first, it->second);
                                    if (componentManager->getComponent(i)->isEventContextActivated()) {
                                        componentManager->getComponent(i)->pushEvent(it->second, *(it->first));
                                    }
                                }
                            }
                        }
                    }
                    //world.updateTimers();
                }
                /**
                * \fn void onLoad()
                * \brief function which can be redefined if the application have to load resources at the start.
                */
                void onLoad (){}
                /**
                * \fn void onLoad()
                * \brief function which can be redefined if the application have to init entities at the start.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void onInit(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca) {}
                /**
                * \fn void onLoad()
                * \brief function which can be redefined if the application have to render entities on components.
                * \param RenderComponentManager : the manager of all render components.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void onRender (SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca, graphic::RenderComponentManager* cm){}
                /**
                * \fn void onLoad()
                * \brief function which can be redefined if the application have to render entities on the window.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void onDisplay(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca, graphic::RenderWindow *rw){}
                /**
                * \fn void onUpdate()
                * \brief function which can be redefined if the application have to update entities when window's events are generated.
                * \param the generated event.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void onUpdate (SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca, graphic::RenderWindow* window, window::IEvent& event) {}
                /**
                * \fn void onExec()
                * \brief function which can be redefined if the application need to do something at each loop.
                * by example if the application need to do something when a networking message is arrived.
                */
                template <typename SystemArray, typename SceneArray, typename RendererArray, typename EntityComponentArray>
                void onExec(SystemArray& sya, SceneArray& sa, RendererArray& ra, EntityComponentArray& eca) {}
                /** \fn void addClock(sf::Clock clock, std::string name)
                *   \brief add a clock to the application, the clock is so accessible everywhere in the source code.
                *   \param Clock : the clock to add.
                *   \param name : a name which identify the clock.
                */
                void addClock(sf::Clock clock, std::string name)  {

                    clocks.insert(std::pair<std::string, sf::Clock>(name, clock));
                }
                /** \fn sf::Clock& getClock(std::string name)
                *   \brief return a clock from its name.
                *   \param std::string : the name of the clock.
                *   \return the clock corresponding to the given name.
                */
                sf::Clock& getClock(std::string name) {
                    std::map<std::string, sf::Clock>::iterator it = clocks.find(name);
                    if (it == clocks.end())
                        throw core::Erreur (14, "Clock not found!", 0);
                    return it->second;
                }
                /** \fn RenderWindow& getRenderWindow()
                *   \brief return a reference to the windows of the appliation
                *   \return the render window of the application.
                */
                graphic::RenderWindow& getRenderWindow(unsigned int i = 0) {
                    //std::cout<<windows[i].first<<std::endl;
                    return *windows[i].first;
                }
                unsigned int getNbWindows() {
                    return windows.size();
                }
                /** \fn RenderComponentManager& getRenderComponentManager()
                *   \brief return a reference to the render component manager.
                *   \return the render component manager of the application.
                */
                graphic::RenderComponentManager& getRenderComponentManager() {
                    return *componentManager;
                }
                /** \fn View& getView()
                *   \brief return a reference to the current view of the window.
                *   \return a reference to the view.
                */
                graphic::View& getView() {
                    return windows[0].first->getView();
                }
                /** \fn View& getDefaultView()
                *   \brief return the default view of the window.
                *   \return the default view.
                */
                graphic::View getDefaultView() {
                    return windows[0].first->getDefaultView();
                }
                /** \fn setClearColor (sf::Color clearColor)
                *   \brief define the clear color of the window.
                *   \param sf::Color : the clear color.
                */
                void setClearColor(sf::Color clearColor) {
                    this->clearColor = clearColor;
                }
                core::Listener& getListener() {
                    return *listener;
                }
                /** \fn ~Application()
                *   \brief destructor : stop the application and destroy the window if any.
                */
                virtual void onDisconnected(network::User* user) {
                }
                World<Alias>* getWorld() {
                    return &world;
                }
                virtual ~Application() {
                    stop();
                    for (unsigned int i = 0; i < windows.size(); i++) {
                        if (windows[i].second)
                            delete windows[i].first;
                    }
                }
                /** > a pointer to the current odfaeg application*/
                 World<Alias> world;
            private :
                std::vector<std::pair<graphic::RenderWindow*, bool>> windows; /** > the render window*/
                std::unique_ptr<graphic::RenderComponentManager> componentManager; /** > the render component manager which draw components on the window*/
                std::map<std::string, sf::Clock> clocks; /** > all the clocks used by the application to measure the time.*/
                bool running; /** > determine if the application running or not.*/
                sf::Color clearColor; /** > keep the clear color of the window*/
                std::multimap<graphic::RenderWindow*, window::IEvent> events; /** > store the windows events generated by the application*/
                std::multimap<graphic::RenderWindow*, window::IEvent>::iterator it; /** > an iterator to the window::IEvents generated by the application*/
                std::unique_ptr<core::Listener> listener;
                bool eventContextActivated;
                /*std::thread rendering_thread;
                std::recursive_mutex rec_mutex;*/

                unsigned int nbEntities, nbEntitiesTypes, nbComponents, nbMaterials;
                std::vector<graphic::Material*> materials;
                std::vector<graphic::Material*> sameMaterials;
                std::map<int, std::string> types;
            };
        }
    }
}
#endif
