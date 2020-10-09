#include "../../../../include/odfaeg/Graphics/2D/decor.h"
namespace odfaeg {
    namespace graphic {
        namespace g2d {
            using namespace sf;
            Decor::Decor (Tile *t, Light *light) : Model(t->getPosition(), t->getSize(), t->getSize() * 0.5f, "E_DECOR") {
                addChild(t);
                t->setParent(this);
                float sy = light->getHeight() / (light->getHeight() * 0.75f);
                setShadowScale(math::Vec3f(1.f, -sy, 1.f));
                shadowOrigin = math::Vec3f(t->getPosition().x, t->getPosition().y, 0) + getSize();
                setShadowOrigin(shadowOrigin);
            }
            void Decor::onMove(math::Vec3f &t) {
                Entity::onMove(t);
            }
            bool Decor::operator==(Entity &other) {
                if (other.getType() != "E_DECOR")
                    return false;
                Decor &decor = static_cast<Decor&> (other);
                if (getNbChildren() != decor.getNbChildren())
                    return false;
                for (unsigned int i = 0; i < getNbChildren(); i++) {
                    if(!(*getChildren()[i] == *decor.getChildren()[i]))
                        return false;

                }
                return true;
            }
        }
    }
}
