#ifndef ODFAEG_REFLECT_REFRACT_RENDER_COMPONENT
#define ODFAEG_REFLECT_REFRACT_RENDER_COMPONENT
#include "ExtLib/GLEW/glew-2.0.0/include/GL/glew.h"
#include <SFML/OpenGL.hpp>
#include "heavyComponent.h"
#include "renderTexture.h"
#include "sprite.h"
#include "rectangleShape.h"
#include "world.h"
#include "perPixelLinkedListRenderComponent.hpp"
#include "3D/cube.h"
namespace odfaeg {
    namespace graphic {
        class ReflectRefractRenderComponent : public HeavyComponent {
        public :
            ReflectRefractRenderComponent (RenderWindow& window, int layer, std::string expression, window::ContextSettings settings);
            std::vector<Entity*> getEntities();
            bool loadEntitiesOnComponent(std::vector<Entity*> visibleEntities);
            bool needToUpdate();
            /**
            * \fn void clearBufferBits()
            * \brief clear the buffer bits of the component.
            */
            void clear ();
            /**
            * \fn void setBackgroundColor(sf::Color color)
            * \brief set the background color of the component. (TRansparent by default)
            * \param sf::Color color : the color.
            */
            void setBackgroundColor(sf::Color color);
            /**
            * \fn void drawNextFrame()
            * \brief draw the next frame of the component.
            */
            void drawNextFrame();
            void setExpression (std::string expression);
            /**
            * \fn draw(Drawable& drawable, RenderStates states = RenderStates::Default);
            * \brief draw a drawable object onto the component.
            * \param Drawable drawable : the drawable object to draw.
            * \param RenderStates states : the render states.
            */
            void draw(Drawable& drawable, RenderStates states = RenderStates::Default);
            /**
            * \fn void draw(RenderTarget& target, RenderStates states)
            * \brief draw the frame on a render target.
            * \param RenderTarget& target : the render target.
            * \param RenderStates states : the render states.
            */
            void draw(RenderTarget& target, RenderStates states);
            std::string getExpression();
            /**
            * \fn int getLayer()
            * \brief get the layer of the component.
            * \return the number of the layer.
            */
            int getLayer();
            /**
            * \fn void setView(View& view)
            * \brief set the view of the component.
            * \param the view of the component.
            */
            void changeVisibleEntities(Entity* toRemove, Entity* toAdd, EntityManager* em);
            /**
            * \fn register an event to the event stack of the component.
            * \param window::IEvent : the event to register.
            * \param Renderwindow : the window generating the event.
            */
            void pushEvent(window::IEvent event, RenderWindow& window);
            void updateParticleSystems();
            void setView(View view);
            View& getView();
            ~ReflectRefractRenderComponent();
        private :
            PerPixelLinkedListRenderComponent* pplls[6];
            Batcher batcher, normalBatcher, reflBatcher, reflNormalBatcher;
            sf::Color backgroundColor; /**> The background color.*/
            std::vector<Instance> m_instances, m_normals, m_reflInstances, m_reflNormals; /**> Instances to draw. (Instanced rendering.) */
            std::vector<Entity*> visibleEntities;
            RenderTexture depthBuffer, reflectRefractTex;
            Shader sBuildDepthBuffer, sBuildDepthBufferNormal, sReflectRefract, sReflectRefractNormal;
            View view;
            std::string expression;
            bool update, cubeMapCreated;
            unsigned int vboWorldMatrices;
            Texture cubeMapTex;
            Sprite depthBufferSprite, reflectRefractTexSprite, ppllsSprites[6];
            VertexBuffer vb;
            std::vector<float> matrices;
            std::vector<sf::Image> images;
            math::Vec3f dirs[6];
        };
    }
}
#endif // ODFAEG_REFLECT_REFRACT_RENDER_COMPONENT
