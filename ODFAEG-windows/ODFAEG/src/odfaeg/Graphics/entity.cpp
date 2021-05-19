#include "../../../include/odfaeg/Graphics/entity.h"
#include "../../../include/odfaeg/Graphics/application.h"
namespace odfaeg {
    namespace graphic {
        /*int Entity::nbEntities = 0;
        int Entity::nbEntitiesTypes = 0;
        std::map<int, std::string>* Entity::types = Entity::initTypes();*/
        float Entity::nbLayers = 1;
        Entity::Entity (math::Vec3f position, math::Vec3f size, math::Vec3f origin, std::string sType, Entity *parent) :
            Transformable (position, size, origin), Drawable(), entityState("Entity State", nullptr) {
            this->parent = parent;
            /*types = initTypes();
            int iType = getIntOfType(sType);
            if (iType == -1) {
                type = std::pair<int, std::string> (nbEntitiesTypes, sType);
                types->insert(type);
                nbEntitiesTypes++;
            } else {
                std::map<int, std::string>::iterator it = types->find(iType);
                type = *it;
            }*/
            /*type.first = -1;
            type.second = sType;
            id = nbEntities;*/
            id = -1;
            if (core::Application::app != nullptr) {
                id = core::Application::app->getUniqueId()-1;
                type = core::Application::app->updateTypes(sType);
            }
            getTransform().setEntityId(id);
            //nbEntities++;
            alreadySerialized = false;
            collisionVolume = nullptr;
            shadowOrigin = math::Vec3f (0, 0, 0);
            shadowCenter = math::Vec3f (0, 0, 0);
            shadowScale = math::Vec3f(1.f, 1.f, 1.f);
            shadowRotationAngle = 0;
            shadowRotationAxis = math::Vec3f::zAxis;
            boneIndex = 0;
            drawMode = NORMAL;
            reflectable = false;
            refractDir = math::Vec3f(0, 0, 1);
            water = false;
            layer = 0;
            external = false;

        }
        void Entity::onLoad() {
            if (core::Application::app != nullptr) {
                type = core::Application::app->updateTypes(getType());
            }
        }
        /*void Entity::setTypeInt (int iType) {
            type.first = iType;
        }*/
        void Entity::copy(Entity* entity) {
            //std::cout<<"copy entity!"<<std::endl;
            entity->setPosition(getPosition());
            entity->setSize(getSize());
            entity->setOrigin(getOrigin());
            entity->setRotation(getRotation());
            entity->parent = parent;
            //std::cout<<"parent : "<<parent<<",this : "<<this<<std::endl;
            entity->entityState = entityState;
            entity->alreadySerialized = false;
            entity->collisionVolume = (collisionVolume == nullptr) ? nullptr : getCollisionVolume()->clone();
            entity->shadowOrigin = getShadowOrigin();
            entity->shadowScale = getShadowScale();
            entity->shadowRotationAngle = getShadowRotationAngle();
            entity->shadowRotationAxis = getShadowRotationAxis();
            entity->boneIndex = boneIndex;
            entity->drawMode = drawMode;
            entity->reflectable = reflectable;
            entity->water = water;
            entity->layer = layer;
            entity->type = type;
            entity->externalObjectName = externalObjectName;
            entity->external = external;
            //std::cout<<"clone id : "<<entity->getId()<<std::endl;
            for (unsigned int i = 0; i < faces.size(); i++) {
                entity->addFace(new Face(faces[i]->getVertexArray(), faces[i]->getMaterial(), entity->getTransform()));
                entity->getFace(i)->getVertexArray().setEntity(entity);
            }
            for (unsigned int i = 0; i < children.size(); i++) {
                Entity* child = children[i]->clone();
                child->setParent(entity);
                entity->addChild(child);
            }
        }
        void Entity::setExternalObjectName(std::string externalObjectName) {
            this->externalObjectName = externalObjectName;
        }
        std::string Entity::getExternalObjectName() {
            return externalObjectName;
        }
        void Entity::setExternal(bool external) {
            this->external = external;
        }
        bool Entity::isExternal() {
            return external;
        }
        void Entity::setLayer(float layer) {
            if (layer > nbLayers)
                nbLayers = layer;
            this->layer = layer;
        }
        float Entity::getLayer() {
            return layer;
        }
        float Entity::getNbLayers() {
            return nbLayers;
        }
        void Entity::setWater (bool water) {
            this->water = water;
        }
        bool Entity::isWater() {
            return water;
        }
        void Entity::setDrawMode(DrawMode dm) {
            drawMode = dm;
        }
        Entity::DrawMode Entity::getDrawMode() {
            return drawMode;
        }
        std::string Entity::getRootType() {
            if (parent != nullptr) {
                return parent->getRootType();
            }
            return getType();
        }
        int Entity::getRootTypeInt() {
            if (parent != nullptr) {
                return parent->getTypeInt();
            }
            return getTypeInt();
        }
        Entity* Entity::getRootEntity() {
            if (parent == nullptr)
                return this;
            return parent->getRootEntity();
        }
        /*int Entity::getNbEntities () {
            return nbEntities;
        }*/
        void Entity::draw (RenderTarget& target, RenderStates states) {
            states.transform = getTransform();
            onDraw(target, states);
            for (unsigned int i = 0; i < children.size(); i++) {
                children[i]->draw(target, states);
            }
        }
        void Entity::setType(std::string sType) {
            /*int iType = getIntOfType(sType);
            if (iType == -1) {
                type = std::pair<int, std::string> (nbEntitiesTypes, sType);
                types->insert(type);
                nbEntitiesTypes++;
            } else {
                std::map<int, std::string>::iterator it = types->find(iType);
                type = *it;
            }*/
            if (core::Application::app != nullptr) {
                type = core::Application::app->updateTypes(sType);
            }
        }
        std::string Entity::getType () const {
            return type.second;
        }
        void Entity::setId (int id) {
            this->id = id;
            onIDUpdated(id);
        }
        int& Entity::getId () {

            return id;
        }
        int Entity::getTypeInt () {
            return type.first;
        }
        /*int Entity::getIntOfType(std::string sType) {

            std::map<int, std::string>::iterator it;
            for (it = types->begin(); it != types->end(); ++it) {
                if (it->second == sType)
                    return it->first;
            }
            return -1;
        }
        std::string Entity::getTypeOfInt (int type) {
            std::map<int, std::string>::iterator it = types->find(type);
            return it->second;
        }*/
        Entity* Entity::getChild(unsigned int n) {
            if (n >= 0 && n < children.size())
                return children[n].get();
            return nullptr;
        }
        void Entity::addChild (Entity* child) {
            std::vector<math::Vec3f> vecs;
            std::unique_ptr<Entity> ptr;
            ptr.reset(child);
            children.push_back(std::move(ptr));
            for (unsigned int i = 0; i < children.size(); i++) {
                vecs.push_back(children[i]->getPosition());
                vecs.push_back(children[i]->getPosition() + children[i]->getSize());
            }
            std::array<std::array<float, 2>, 3> minsMaxs;
            minsMaxs = math::Computer::getExtends(vecs);
            math::Vec3f pos((int) minsMaxs[0][0], (int) minsMaxs[1][0], (int) minsMaxs[2][0]);
            math::Vec3f size((int) minsMaxs[0][1] - (int) minsMaxs[0][0], (int) minsMaxs[1][1] - (int) minsMaxs[1][0], (int) minsMaxs[2][1] - (int) minsMaxs[2][0]);
            setLocalBounds(physic::BoundingBox(pos.x, pos.y, pos.z, size.x, size.y, size.z));
            vecs.clear();
        }
        void Entity::removeChild (Entity *child) {
            std::vector<std::unique_ptr<Entity>>::iterator it;
            for (it = children.begin(); it != children.end();) {
                if (it->get() == child) {
                    delete it->release();
                    it = children.erase(it);
                } else
                    it++;
            }
        }
        void Entity::updateTransform() {

            getTransform().update();
            for (unsigned int i = 0; i < children.size(); i++) {
                children[i]->updateTransform();
            }
            /*if (!(RenderTarget::getMajorVersion() >= 3 && RenderTarget::getMinorVersion() >= 3)) {
                for (unsigned int i = 0; i < faces.size(); i++) {
                    VertexArray& va = faces[i]->getVertexArray();
                    va.transform(getTransform());
                }
            }*/
        }
        //Return the children of the entities.
        std::vector<Entity*> Entity::getChildren() const {
            std::vector<Entity*> childs;
            for (unsigned int i = 0; i < children.size(); i++)
                childs.push_back(children[i].get());
            return childs;
        }

        //Return the number of entity's children.
        unsigned int Entity::getNbChildren () {
            return children.size();
        }
        void Entity::setParent(Entity *parent) {
            this->parent = parent;
        }
        Entity* Entity::getParent() const {
            return parent;
        }

        void Entity::onMove(math::Vec3f &t) {
            //updateTransform();
            for (unsigned int i = 0; i < children.size(); i++) {
                children[i]->move(t);
            }


        }
        void Entity::onScale(math::Vec3f &s) {
            //updateTransform();
            for (unsigned int i = 0; i < children.size(); i++) {
                children[i]->scale(s);
            }

        }
        void Entity::onRotate(float angle) {
            //updateTransform();
            for (unsigned int i = 0; i < children.size(); i++) {
                children[i]->rotate(angle);
            }
        }
        void Entity::addFace (Face* face) {
            std::unique_ptr<Face> ptr;
            ptr.reset(face);
            faces.push_back(std::move(ptr));
        }
        std::vector<Face*> Entity::getFaces() const {
            std::vector<Face*> fcs;
            for (unsigned int i = 0; i < faces.size(); i++) {
                fcs.push_back(faces[i].get());
            }
            return fcs;
        }
        unsigned int Entity::getNbFaces() {
            return faces.size();
        }
        Face* Entity::getFace(unsigned int n) {
            if (n >= 0 && n < faces.size())
                return faces[n].get();
            return nullptr;
        }
         void Entity::setShadowCenter(math::Vec3f shadowCenter) {
                this->shadowCenter = shadowCenter;
                for (unsigned int i = 0; i < children.size(); i++) {
                    children[i]->setShadowCenter(shadowCenter);
                }
            }
            math::Vec3f Entity::getShadowCenter() {
                return shadowCenter;
            }
            void Entity::setShadowScale(math::Vec3f shadowScale) {
                this->shadowScale = shadowScale;
                for (unsigned int i = 0; i < children.size(); i++) {
                    children[i]->setShadowScale(shadowScale);
                }
            }
            math::Vec3f Entity::getShadowScale() {
                return shadowScale;
            }
            void Entity::setShadowRotation(float angle, math::Vec3f axis) {
                this->shadowRotationAngle = angle;
                this->shadowRotationAxis = axis;
                for (unsigned int i = 0; i < children.size(); i++) {
                    children[i]->setShadowRotation(angle, axis);
                }
            }
            math::Vec3f Entity::getShadowRotationAxis() {
                return shadowRotationAxis;
            }
            float Entity::getShadowRotationAngle() {
                return shadowRotationAngle;
            }
            void Entity::setShadowOrigin(math::Vec3f origin) {
                shadowOrigin = origin;
                for (unsigned int i = 0; i < children.size(); i++) {
                    children[i]->setShadowOrigin(origin);
                }
            }
            math::Vec3f Entity::getShadowOrigin() {
                return shadowOrigin;
            }
            /*int Entity::getNbEntitiesTypes () {
                return nbEntitiesTypes;
            }*/
            void Entity::setBoneIndex(unsigned int boneIndex) {
                this->boneIndex = boneIndex;
            }
            unsigned int Entity::getBoneIndex() {
                return boneIndex;
            }
            void Entity::setRefractDir (math::Vec3f refractDir) {
                this->refractDir = refractDir;
            }
            math::Vec3f Entity::getRefractDir() {
                return refractDir;
            }
            bool Entity::isReflectable() {
                return reflectable;
            }
            void Entity::setReflectable(bool reflectable) {
                this->reflectable = reflectable;
            }

    }
}
