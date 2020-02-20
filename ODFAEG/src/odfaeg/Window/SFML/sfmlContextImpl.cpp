#include "../../../../include/odfaeg/Window/SFML/sfmlContextImpl.hpp"
namespace odfaeg {
    namespace window {
        bool SFMLContextImpl::setActive(bool active) {
            if (context)
                return context->setActive(active);
            return false;
        }
        void SFMLContextImpl::create(IContext* sharedContext) {
        }
        void SFMLContextImpl::create(sf::WindowHandle handle, const ContextSettings& settings, IContext* sharedContext, unsigned int bitsPerPixel) {

        }
        void SFMLContextImpl::create(const ContextSettings& settings, unsigned int width, unsigned int height, IContext* sharedContext) {
            context = new sf::Context(sf::ContextSettings(settings.depthBits, settings.stencilBits, settings.antiAliasingLevel, settings.versionMajor, settings.versionMinor), width, height);
        }
        const ContextSettings& SFMLContextImpl::getSettings() const {
            if (context) {
                static sf::ContextSettings settings = context->getSettings();
                return ContextSettings(settings.depthBits, settings.stencilBits, settings.antialiasingLevel, settings.majorVersion, settings.minorVersion);
            } else {
                static ContextSettings empty(0, 0, 0, 0, 0);
                return empty;
            }
        }
        void SFMLContextImpl::display() {

        }
        void SFMLContextImpl::initialize(const ContextSettings& settings) {
        }
        void SFMLContextImpl::setVerticalSyncEnabled(bool enabled) {

        }
    }
}
