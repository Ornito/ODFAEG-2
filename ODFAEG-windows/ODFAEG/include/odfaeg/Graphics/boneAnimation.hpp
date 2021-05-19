#ifndef ODFAEG_BONE_ANIMATION_HPP
#define ODFAEG_BONE_ANIMATION_HPP
#include "animatedEntity.h"
namespace odfaeg {
    namespace graphic {
        class BoneAnimation : public Entity {
            public :
            BoneAnimation(math::Vec3f position, math::Vec3f size, std::string type);
            void setBoneIndex(unsigned int index);
            unsigned int getBoneIndex();
            bool isAnimated() const {
                return false;
            }
            bool isModel() const {
                return true;
            }
            bool isLight() const {
                return false;
            }
            bool isShadow() const {
                return false;
            }
            bool isLeaf() const {
                return false;
            }
            private :
                unsigned int boneIndex;
        };
    }
}
#endif // BONE_ANIMATION_HPP
