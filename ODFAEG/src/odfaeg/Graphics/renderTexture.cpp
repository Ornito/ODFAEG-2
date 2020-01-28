////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "renderTextureImplFBO.h"
#include "renderTextureImplDefault.h"
#include "../../../include/odfaeg/Graphics/renderTexture.h"
#include <SFML/OpenGL.hpp>
#include "glCheck.h"
namespace odfaeg
{
    namespace graphic {
        using namespace sf;
        ////////////////////////////////////////////////////////////
        RenderTexture::RenderTexture() :
        m_impl(NULL),
        m_context(NULL)
        {

        }


        ////////////////////////////////////////////////////////////
        RenderTexture::~RenderTexture()
        {
            //delete m_context;
            if (m_impl)
                delete m_impl;
            if (m_context)
                delete m_context;
        }


        ////////////////////////////////////////////////////////////
        bool RenderTexture::create(unsigned int width, unsigned int height, window::ContextSettings settings, unsigned int precision, unsigned int format, unsigned int type, bool useSeparateContext)
        {

            if (useSeparateContext)
                m_context = new window::Context(settings, width, height);
            RenderTarget::setVersionMajor(m_context->getSettings().versionMajor);
            RenderTarget::setVersionMinor(m_context->getSettings().versionMinor);
            // Create the texture
            if(!m_texture.create(width, height, precision, format, type))
            {
                std::cerr<< "Impossible to create render texture (failed to create the target texture)" << std::endl;
                return false;
            }
            // We disable smoothing by default for render textures
            setSmooth(false);

            // Create the implementation
            delete m_impl;
            if (priv::RenderTextureImplFBO::isAvailable())
            {
                // Use frame-buffer object (FBO)
                m_impl = new priv::RenderTextureImplFBO;
            }
            else
            {
                std::cout<<"FBO not avalaible"<<std::endl;
                // Use default implementation
                m_impl = new priv::RenderTextureImplDefault;
            }

            // Initialize the render texture
            if (!m_impl->create(width, height, (m_context) ? m_context->getSettings() : settings, m_texture.m_texture))
                return false;
            // We can now initialize the render target part
            RenderTarget::initialize(m_impl->getFramebufferId());
            return true;
        }


        ////////////////////////////////////////////////////////////
        void RenderTexture::setSmooth(bool smooth)
        {
            m_texture.setSmooth(smooth);
        }


        ////////////////////////////////////////////////////////////
        bool RenderTexture::isSmooth() const
        {
            return m_texture.isSmooth();
        }


        ////////////////////////////////////////////////////////////
        void RenderTexture::setRepeated(bool repeated)
        {
            m_texture.setRepeated(repeated);
        }


        ////////////////////////////////////////////////////////////
        bool RenderTexture::isRepeated() const
        {
            return m_texture.isRepeated();
        }


        ////////////////////////////////////////////////////////////
        bool RenderTexture::setActive(bool active)
        {
            return m_impl && m_context && m_context->setActive(active);
        }


        ////////////////////////////////////////////////////////////
        void RenderTexture::display()
        {
            // Update the target texture
            if (setActive(true))
            {
                m_impl->updateTexture(m_texture.m_texture);
                m_texture.m_pixelsFlipped = true;
            }
        }


        ////////////////////////////////////////////////////////////
        Vector2u RenderTexture::getSize() const
        {
            return m_texture.getSize();
        }


        ////////////////////////////////////////////////////////////
        const Texture& RenderTexture::getTexture() const
        {
            return m_texture;
        }
        const window::ContextSettings& RenderTexture::getSettings() const {
              ContextSettings empty(0, 0, 0, 0, 0);
              if(m_context) return m_context->getSettings();
        }
        bool RenderTexture::activate(bool active) {
            return setActive(active);
        }
        void RenderTexture::bind() {
            if (m_impl)
                m_impl->bind();
        }
    }
}
