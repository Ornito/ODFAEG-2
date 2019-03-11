#include "../../../include/odfaeg/Window/context.hpp"
#include "../../../include/odfaeg/Window/contextImpl.hpp"
namespace odfaeg {
    namespace window {
        Context::Context() :
        m_context(nullptr) {
        }
        Context::Context(ContextSettings settings, unsigned int width, unsigned int height) {
            m_context = ContextFactory::create();
            m_context->create(settings, width, height);
            m_context->setActive(true);
        }
        Context::Context(sf::WindowHandle handle) {
            m_context = ContextFactory::create();
            m_context->create(handle);
            m_context->setActive(true);
        }
        void Context::create(ContextSettings settings, unsigned int width, unsigned int height) {
            if (!m_context)
                delete m_context;
            m_context = ContextFactory::create();
            m_context->create(settings, width, height);
            m_context->setActive(true);
        }
        void Context::create(sf::WindowHandle handle) {
            if (!m_context)
                delete m_context;
            m_context = ContextFactory::create();
            m_context->create(handle);
            m_context->setActive(true);
        }
        void Context::display() {
            if (m_context)
                m_context->display();
        }
        void Context::setVerticalSyncEnabled(bool enabled) {
            if (m_context)
                m_context->setVerticalSyncEnabled(enabled);
        }
        bool Context::setActive(bool active) {
            return m_context && m_context->setActive(true);
        }
        const ContextSettings& Context::getSettings() const {
            ContextSettings empty(0, 0, 0, 0, 0);
            return m_context ? m_context->getSettings() : empty;
        }
        ////////////////////////////////////////////////////////////
        GlFunctionPointer Context::getFunction(const char* name)
        {
            #if defined (SFML)
                return sf::Context::getFunction(name);
            #else
                return ContextImpl::getFunction(name);
            #endif
        }
        ////////////////////////////////////////////////////////////
        bool Context::isExtensionAvailable(const char* name)
        {
            #if defined (SFML)
                return sf::Context::isExtensionAvailable(name);
            #else
                return ContextImpl::isExtensionAvalaible(name);
            #endif
        }
        Context::~Context() {
            if (m_context)
                delete m_context;
        }
    }
}
