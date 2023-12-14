#include "../../../../include/odfaeg/Graphics/ECS/animationUpdater.hpp"
namespace odfaeg {
    namespace graphic {
        namespace ecs {
            AnimationUpdater::AnimationUpdater(World& world, EntityFactory& entityFactory) : world(world), entityFactory(entityFactory) {
            }
            void AnimationUpdater::addAnim(EntityId anim) {
                anims.push_back(anim);
            }
            void AnimationUpdater::onUpdate() {
                for (unsigned int i = 0; i < anims.size(); i++) {
                    AnimationComponent* ac = getComponent<AnimationComponent>(anims[i]);
                    if (ac->playing && ac->clock.getElapsedTime().asSeconds() > ac->fr) {
                        if (world.getComponentMapping().getChildren(anims[i]).size() > 2) {
                            ac->interpPerc++;
                            if (ac->interpPerc >= ac->interpLevels) {
                                ac->previousFrame = world.getComponentMapping().getChildren(anims[i])[ac->currentFrame];
                                ac->currentFrameIndex++;
                                if (ac->currentFrameIndex >= world.getComponentMapping().getChildren(anims[i]).size()) {
                                    ac->currentFrameIndex = 0;
                                    if (!ac->loop) {
                                        ac->playing = false;
                                    }
                                }
                                ac->currentFrame = world.getComponentMapping().getChildren(anims[i])[ac->currentFrame];
                                ac->nextFrame = world.getComponentMapping().getChildren(anims[i])[(ac->currentFrameIndex + 1 >= world.getComponentMapping().getChildren(anims[i]).size()) ? 0 : ac->currentFrameIndex+1];
                                getComponent<MeshComponent>(ac->interpolatedFrame)->faces[i].setMaterial(getComponent<MeshComponent>(ac->currentFrame)->faces[i].getMaterial());
                                getComponent<MeshComponent>(ac->interpolatedFrame)->faces[i].setTransformMatrix(getComponent<MeshComponent>(ac->currentFrame)->faces[i].getTransformMatrix());
                            }
                            interpolate(ac->currentFrame, ac->nextFrame, ac->interpolatedFrame, ac->interpPerc, ac->interpLevels);
                        }
                    }
                }
            }
            void AnimationUpdater::interpolate(EntityId currentFrame, EntityId nextFrame, EntityId interpolatedFrame, std::size_t interpPerc, std::size_t interpLevels) {
                if (world.getComponentMapping().getChildren(currentFrame) == world.getComponentMapping().getChildren(nextFrame)) {
                    for (unsigned int i = 0; i < world.getComponentMapping().getChildren(currentFrame).size(); i++) {
                        interpolate(world.getComponentMapping().getChildren(currentFrame)[i], world.getComponentMapping().getChildren(nextFrame)[i], world.getComponentMapping().getChildren(interpolatedFrame)[i], interpPerc, interpLevels);
                    }
                }
                if (getComponent<MeshComponent>(currentFrame) != nullptr && getComponent<MeshComponent>(nextFrame) != nullptr
                    && getComponent<MeshComponent>(currentFrame)->faces.size() == getComponent<MeshComponent>(nextFrame)->faces.size()) {
                    for (unsigned int i = 0; i < getComponent<MeshComponent>(currentFrame)->faces.size(); i++) {
                        const VertexArray& cva = getComponent<MeshComponent>(currentFrame)->faces[i].getVertexArray();
                        VertexArray& iva = getComponent<MeshComponent>(interpolatedFrame)->faces[i].getVertexArray();
                        const VertexArray& nva = getComponent<MeshComponent>(nextFrame)->faces[i].getVertexArray();
                        if (cva.getVertexCount() == nva.getVertexCount()) {
                            for (unsigned int j = 0; j < cva.getVertexCount(); j++) {
                                iva[j].position.x = cva[j].position.x + (nva[j].position.x - cva[j].position.x) * (interpPerc / interpLevels);
                                iva[j].position.y = cva[j].position.y + (nva[j].position.y - cva[j].position.y) * (interpPerc / interpLevels);
                                iva[j].position.z = cva[j].position.z + (nva[j].position.z - cva[j].position.z) * (interpPerc / interpLevels);
                                iva[j].color = cva[j].color;
                                iva[j].texCoords = cva[j].texCoords;
                            }
                        }
                    }
                }
            }
        }
    }
}
