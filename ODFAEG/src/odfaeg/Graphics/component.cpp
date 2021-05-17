#include "../../../include/odfaeg/Graphics/component.h"
#include "../../../include/odfaeg/Graphics/application.h"
namespace odfaeg {
    namespace graphic {
        //int Component::nbComponents = 0;
        Component::Component(RenderWindow& window, math::Vec3f position, math::Vec3f size, math::Vec3f origin, unsigned int priority)
        : Transformable(position, size, origin),
        listener(),
        priority(priority),
        window(window) {
            activateEventContext = true;
            visible = true;
            id = -1;
            if (core::Application::app != nullptr)
                id = core::Application::app->getComponentId()-1;
            /*id = nbComponents;
            nbComponents++;*/
            autoResize = false;
            relPosition = false;
        }
        void Component::setEventContextActivated(bool activateEventContext) {
            this->activateEventContext = activateEventContext;
            //listener.clearEventsStack();
            onEventContextActivated(activateEventContext);
        }
        bool Component::isEventContextActivated() {
            return activateEventContext;
        }
        void Component::setVisible (bool visible) {
            onVisibilityChanged(visible);
            this->visible = visible;
        }
        bool Component::isVisible() {
            return visible;
        }
        void Component::processEvents() {
           listener.processEvents();
        }
        void Component::onVisibilityChanged(bool visible) {}
        void Component::onEventContextActivated(bool activate) {}
    }
}
