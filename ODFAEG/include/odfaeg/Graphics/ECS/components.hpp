#ifndef ODFAEG_ECS_COMPONENTS_HPP
#define ODFAEG_ECS_COMPONENTS_HPP
#include "../../config.hpp"
#ifndef VULKAN
#include "../glCheck.h"
#include <GL/glew.h>
#else
#include <vulkan/vulkan.h>
#endif
#include "../../Math/vec4.h"
#include "../../Math/transformMatrix.h"
#include "../batcher.h"
#include "../renderWindow.h"
#include "../view.h"
#include "../rectangleShape.h"
#include "../../Core/entityFactory.hpp"

namespace odfaeg {
    namespace graphic {
        namespace ecs {
            enum WallType {
                    TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT, TOP_BOTTOM, RIGHT_LEFT, T_TOP, T_RIGHT, T_LEFT, T_BOTTOM, X, NB_WALL_TYPES
            };
            enum DrawMode {
                    NORMAL, INSTANCED, BASE_INSTANCED
            };
            struct ColliderComponent {
                physic::BoundingVolume* boundingVolume;
            };
            struct EntityInfoComponent {
                std::string groupName;
                size_t animIndex;
                DrawMode drawMode = NORMAL;
            };
            struct WallTypeComponent {
                WallType wallType;
            };
            struct TransformComponent {
                math::Vec3f position, size, center, scale, rotationAxis, origin;
                float rotation;
                TransformMatrix transformMatrix;
                physic::BoundingBox globalBounds, localBounds;
            };
            struct AnimationComponent {
                bool playing, loop;
                EntityId previousFrame, currentFrame, nextFrame;
                std::size_t nbFrames;
                std::size_t interpLevels, interpPerc;
                EntityId interpolatedFrame;
                std::vector<EntityId> frames;
            };
            struct SelectedAnimationComponent {
                size_t selectedAnimIndex;
            };
            struct MeshComponent {
                std::vector<Face> faces;
            };
        }
    }
}
#endif
