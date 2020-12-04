#include "../../../include/odfaeg/Graphics/anim.h"
namespace odfaeg {
    namespace graphic {
        using namespace sf;
        using namespace std;
        Anim::Anim() : AnimatedEntity (math::Vec3f(0, 0, 0), math::Vec3f(0, 0, 0), math::Vec3f(0, 0, 0), "E_ANIMATION", nullptr) {
            currentFrameIndex = 0;
            running = false;
            currentFrameChanged = false;
            currentFrame = nullptr;
            previousFrame = nullptr;
            nextFrame = nullptr;
            loop = false;
            interpLevels = 1;
            interpPerc = 0;
            interpolatedFrame = std::make_unique<Mesh>(math::Vec3f(0, 0, 0), math::Vec3f(0, 0, 0), "E_MESH");
            interpolatedFrame->setParent(this);
        }
        Anim::Anim (float fr, math::Vec3f position, math::Vec3f size, Entity *parent) : AnimatedEntity (position, size, size * 0.5f, "E_ANIMATION", parent) {
            this->fr = fr;
            currentFrameIndex = 0;
            running = false;
            currentFrameChanged = false;
            currentFrame = nullptr;
            previousFrame = nullptr;
            nextFrame = nullptr;
            loop = false;
            interpLevels = 1;
            interpPerc = 0;
            interpolatedFrame = std::make_unique<Mesh>(position, size, "E_MESH");
            interpolatedFrame->setParent(this);
        }
        bool Anim::isCurrentFrameChanged() {
            return currentFrameChanged;
        }
        void Anim::setCurrentFrameChanged(bool b) {
            currentFrameChanged = b;
        }
        void Anim::addFrame (Entity *entity) {
            entity->setParent(this);
            if (getChildren().size() == 0) {
                currentFrame = entity;
                interpolatedFrame->setType(currentFrame->getType());
                createFirstInterpolatedFrame(entity);
            }
            else if (getChildren().size() == 1)
                nextFrame = entity;
            addChild(entity);
        }
        void Anim::removeFrame (Entity *entity) {
            removeChild(entity);
        }
        int Anim::getCurrentFrameIndex () {
            return currentFrameIndex;
        }
        void Anim::setCurrentFrame (int index, EntityManager* scene) {
            if (getChildren().size() >= 2) {
                previousFrame = getChildren()[(index -1 < 0) ? 0 : index - 1];
                currentFrame = getChildren()[index];
                nextFrame = getChildren()[(index + 1 >= getChildren().size()) ? 0 : index + 1];
                currentFrameIndex = index;
                currentFrameChanged = true;
                interpolatedFrame->setType(currentFrame->getType());
                changeInterpolatedFrame(currentFrame, scene);
            }
        }
        Entity* Anim::getPreviousFrame() {
            return previousFrame;
        }
        void Anim::setFrameRate (float fr) {
            this->fr = fr;
        }
        bool Anim::isRunning () {
            return running;
        }
        void Anim::play (bool loop) {
            if (getChildren().size() > 1 && !running) {
                running = true;
                this->loop = loop;
            }
        }
        void Anim::stop (){
            if (running) {
                running = false;
            }
        }
        void Anim::computeNextFrame (EntityManager* scene) {
            onFrameChanged(scene);
        }
        void Anim::onFrameChanged(EntityManager* scene) {
            if (getChildren().size() != 0) {
                for (unsigned int i = 0; i < getChildren().size(); i++) {
                    getChildren()[i]->onFrameChanged(scene);
                }
            }
            if (getChildren().size() >= 2) {
                if (previousFrame == nullptr) {
                    previousFrame = getChildren()[(currentFrameIndex - 1 < 0) ? 0 : getChildren().size() - 1];
                }
                interpPerc++;
                if (interpPerc >= interpLevels) {
                    previousFrame = getChildren()[currentFrameIndex];
                    currentFrameChanged = true;
                    currentFrameIndex++;
                    if (currentFrameIndex >= getChildren().size()) {
                        currentFrameIndex = 0;
                        if (!loop) {
                            running = false;
                        }
                    }
                    currentFrame = getChildren()[currentFrameIndex];
                    nextFrame = getChildren()[(currentFrameIndex + 1 >= getChildren().size()) ? 0 : currentFrameIndex+1];
                    interpPerc = 0;
                }
                interpolatedFrame->setType(currentFrame->getType());
                interpolatedFrame->setPosition(currentFrame->getPosition());
                interpolatedFrame->setSize(currentFrame->getSize());
                interpolatedFrame->setParent(currentFrame->getParent());
                interpolate(currentFrame, nextFrame, scene);
            }
        }
        void Anim::createFirstInterpolatedFrame (Entity* currentFrame) {
            if (currentFrame->getChildren().size() > 0) {
                for (unsigned int i = 0; i < currentFrame->getChildren().size(); i++) {
                    createFirstInterpolatedFrame(currentFrame->getChildren()[i]);
                }
            }
            if (currentFrame->isLeaf()) {
                for (unsigned int i = 0; i < currentFrame->getFaces().size(); i++) {
                    VertexArray va = currentFrame->getFaces()[i]->getVertexArray();
                    Face* face = new Face(va,currentFrame->getFaces()[i]->getMaterial(), currentFrame->getTransform());
                    interpolatedFrame->addFace(face);
                }
                interpolatedFrame->setType(currentFrame->getType());
                interpolatedFrame->setPosition(currentFrame->getPosition());
                interpolatedFrame->setSize(currentFrame->getSize());
                interpolatedFrame->setParent(currentFrame->getParent());
                //addChild(interpolatedFrame);
                /*if (interpolatedFrame->getRootType() == "E_ANIMATION")
                    std::cout<<"interpolated frame type : "<<interpolatedFrame->getType()<<std::endl;*/
            }
        }
        void Anim::changeInterpolatedFrame(Entity* currentFrame, EntityManager* scene) {
            if (currentFrame->getChildren().size() > 0) {
                for (unsigned int i = 0; i < currentFrame->getChildren().size(); i++) {
                    changeInterpolatedFrame(currentFrame->getChildren()[i], scene);
                }
            }
            if (currentFrame->getFaces().size() == interpolatedFrame->getFaces().size()) {
                for (unsigned int i = 0; i < currentFrame->getFaces().size(); i++) {
                    VertexArray& iva = interpolatedFrame->getFace(i)->getVertexArray();
                    VertexArray va = currentFrame->getFaces()[i]->getVertexArray();
                    for (unsigned int n = 0; n < va.getVertexCount(); n++) {
                        iva[n] = va[n];
                    }
                    if(scene != nullptr) {
                        scene->updateVertices(iva);
                    }
                    interpolatedFrame->getFaces()[i]->setMaterial(currentFrame->getFaces()[i]->getMaterial());
                    interpolatedFrame->getFaces()[i]->setTransformMatrix(currentFrame->getFaces()[i]->getTransformMatrix());
                }
                interpolatedFrame->setPosition(currentFrame->getPosition());
                interpolatedFrame->setSize(currentFrame->getSize());
                interpolatedFrame->setParent(currentFrame->getParent());
            }
        }
        void Anim::interpolate(Entity* currentFrame, Entity* nextFrame, EntityManager* scene) {
            if (currentFrame->getNbChildren() == nextFrame->getNbChildren()) {
                if (currentFrame->getNbChildren() != 0 && nextFrame->getNbChildren() != 0) {
                    for (unsigned int i = 0; i < currentFrame->getNbChildren(); i++) {
                         interpolate(currentFrame->getChild(i), nextFrame->getChild(i), scene);
                    }
                }
                if (currentFrame->getNbFaces() == nextFrame->getNbFaces()
                    && currentFrame->getNbFaces() == interpolatedFrame->getNbFaces()) {
                    for (unsigned int i = 0; i < currentFrame->getNbFaces(); i++) {
                        const VertexArray& cva = currentFrame->getFace(i)->getVertexArray();
                        VertexArray& iva = interpolatedFrame->getFace(i)->getVertexArray();
                        const VertexArray& nva = nextFrame->getFace(i)->getVertexArray();
                        if (cva.getVertexCount() == nva.getVertexCount()) {
                            for (unsigned int j = 0; j < cva.getVertexCount(); j++) {
                                iva[j].position.x = cva[j].position.x + (nva[j].position.x - cva[j].position.x) * (interpPerc / interpLevels);
                                iva[j].position.y = cva[j].position.y + (nva[j].position.y - cva[j].position.y) * (interpPerc / interpLevels);
                                iva[j].position.z = cva[j].position.z + (nva[j].position.z - cva[j].position.z) * (interpPerc / interpLevels);
                                iva[j].color = cva[j].color;
                                iva[j].texCoords = cva[j].texCoords;
                            }
                            interpolatedFrame->getFace(i)->setMaterial(currentFrame->getFace(i)->getMaterial());
                            interpolatedFrame->getFace(i)->setTransformMatrix(currentFrame->getFace(i)->getTransformMatrix());
                            if (scene != nullptr)
                                scene->updateVertices(iva);
                        }
                    }
                }
            }
        }
        Time Anim::getElapsedTime () {
            return clock.getElapsedTime();
        }
        void Anim::resetClock () {
            clock.restart();
        }
        bool Anim::operator== (Entity &other) {

            if (other.getType() != "E_ANIMATION")
                return false;
            Anim &a = static_cast<Anim&> (other);
            if (getPosition().x != a.getPosition().x || getPosition().y != a.getPosition().y
                || getSize().x != a.getSize().x || getSize().y != a.getSize().y)
                return false;
            for (unsigned int i = 0; i < getChildren().size(); i++) {
                if (!(*getChildren()[i] == *a.getChildren()[i]))
                    return false;
            }
            return true;
        }

        float Anim::getFrameRate () {
            return fr;
        }
        Entity* Anim::getCurrentFrame () const {
              return interpolatedFrame.get();
        }
        void Anim::onDraw(RenderTarget &target, RenderStates states) {
             target.draw(*interpolatedFrame, states);
        }
        void Anim::onMove(math::Vec3f& t) {
            Entity::onMove(t);
            interpolatedFrame->move(t);
        }
        Anim::~Anim () {

        }
    }
}

