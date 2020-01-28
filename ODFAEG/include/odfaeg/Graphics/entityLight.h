#ifndef ODFAEG_ENTITY_LIGHT_HPP
#define ODFAEG_ENTITY_LIGHT_HPP
#include "entity.h"
#include "light.h"
/**
  *\namespace odfaeg
  * the namespace of the Opensource Development Framework Adapted for Every Games.
  */
namespace odfaeg {
    namespace graphic {
        /**
          * \file entityLight.h
          * \class EntityLight
          * \brief Represent a 2D entity.
          * \author Duroisin.L
          * \version 1.0
          * \date 1/02/2014
          * \brief Abstract class which can represent any type of lights (except ambient lights.)
          *  Each entity of the framework inherits from this class.
          */
        class ODFAEG_GRAPHICS_API EntityLight : public Light, public Entity {
            public :
            /**
            * \fn EntityLight (math::Vec3f center, sf::Color color, float radius1, float radius2, float radius3, int height, std::string type, Entity* parent)
            * \brief constructor.
            * \param center : the center of the light.
            * \param sf::Color : the color of the light.
            * \param float radius1 : the minor radius of the light.
            * \param float radius2 : the major radius of the light.
            *  create an entity light with the given center, height, color, etc...
            */
            EntityLight (math::Vec3f center, sf::Color color, float radius1, float radius2, float radius3, int height, std::string type, Entity* parent)
            : Light (center, height, color), Entity (math::Vec3f(center.x, center.y, center.z), math::Vec3f(radius1 * 2, radius2 * 2, radius3 * 2), math::Vec3f (radius1, radius2, radius3), type, parent) {
            }
            /**
            * \fn isLeaf() const
            * \brief virtual method of the entity class to redefine.
            * \return true because the entity is a leaf.
            */
            bool isLeaf() const {
                return true;
            }
            /**
            * \fn isModel() const
            * \brief virtual method of the entity class to redefine.
            * \return false because the entity is not model.
            */
            bool isModel () const {
                return false;
            }
            /**
            * \fn isModel() const
            * \brief virtual method of the entity class to redefine.
            * \return false because the entity is not animated.
            */
            bool isAnimated() const {
                return false;
            }
            /**
            * \fn selectable() const
            * \brief virtual method of the entity class to redefine.
            * \return true because the entity is selectable.
            */
            bool selectable() const {
                return true;
            }
            /**
            * \fn isLight() const
            * \brief virtual method of the entity class to redefine.
            * \return true because the entity is a light.
            */
            bool isLight() const {
                return true;
            }
            /**
            * \fn isShadow() const
            * \brief virtual method of the entity class to redefine.
            * \return true because the entity is a shadow.
            */
            bool isShadow() const {
                return false;
            }
            /**
            *\fn vtserialize(Archive & ar)
            *\brief Serialize an entity light.
            *
            */
            template <typename Archive>
            void vtserialize(Archive & ar) {
                Entity::vtserialize(ar);
                Light::vtserialize(ar);
            }
        };
    }
}
#endif // DYNAMIC_LIGHT
