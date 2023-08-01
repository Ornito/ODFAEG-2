#ifndef ODFAEG_ECS_GRID_HPP
#define ODFAEG_ECS_GRID_HPP
#include "cell.hpp"
#include "../../Math/vec4.h"
#include "../baseChangementMatrix.h"
#include "../../Physics/boundingBox.h"

namespace odfaeg {
    namespace graphic {
        namespace ecs {
            class ColliderComponent;
            class TransformComponent;
            class Grid {
                public :
                    /**
                    * \fn std::vector<CellMap<Entity>*> getNeightbours(Entity* entity, CellMap<Entity> *cell, bool getCellOnPassable);
                    * \brief get the neightbour cells of the given cell.
                    * \param Entity* entity : cell.
                    * \param bool getCellOnPassable : if we want to get the cells which are passable or not.
                    * \return the neightbour cells.
                    */
                    template <typename EntityComponentArray>
                    std::vector<Cell*> getNeightbours(EntityComponentArray eca, ComponentMapping mapping, EntityId entity, Cell *cell, bool getCellOnPassable) {
                        math::Vec3f coords = cell->getCoords();
                        std::vector<Cell*> neightbours;
                        for (int i = coords.x - 1; i <= coords.x + 1; i++) {
                            for (int j = coords.y - 1; j <= coords.y + 1; j++) {
                                for (int k = coords.z - 1; k <= coords.z + 1; k++) {
                                    if (!(i == coords.x && j == coords.y && k == coords.z)) {
                                        math::Vec2f neightbourCoords(i, j);
                                        Cell *neightbour = getGridCellAtFromCoords(neightbourCoords);
                                        if (neightbour != nullptr) {
                                            if (getCellOnPassable)
                                                neightbours.push_back(neightbour);
                                            else {
                                                ColliderComponent* collider = mapping.getAgregate<ColliderComponent>(eca, entity);
                                                if (collider != nullptr) {
                                                    bool collide = false;
                                                    math::Vec3f t = neightbour->getCenter() - collider->collisionVolume->getCenter();
                                                    std::unique_ptr<physic::BoundingVolume> cv = collider->collisionVolume->clone();
                                                    cv->move(t);
                                                    for (unsigned int k = 0; k < neightbour->getEntitiesInside().size() && !collide; k++) {
                                                        ColliderComponent* collider2 = mapping.getAgregate<ColliderComponent>(eca, cell->getEntitiesInside()[k]);
                                                        if (collider2 != nullptr && neightbour->getEntitiesInside()[k].get().load() != entity.get().load()) {
                                                            physic::CollisionResultSet::Info info;
                                                            if (cv->intersects(*collider2->collisionVolume, info)) {
                                                                if (cv->getChildren().size() == 0) {
                                                                    collide = true;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if (!collide) {
                                                        neightbours.push_back(neightbour);
                                                    }
                                                } else {
                                                    if (neightbour->isPassable()) {
                                                        neightbours.push_back(neightbour);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        return neightbours;
                    }
                    /**
                    * \fn std::vector<Vec3f> getPath (Entity* entity, Vec3f finalPos);
                    * \brief get the path between an entity and a destination
                    * \param Entity* entity : an entity.
                    * \param Vec3f finalPos : the destination.
                    * \return the points of the curve of the path.
                    */
                    template <typename EntityComponentArray>
                    std::vector<math::Vec3f> getPath (EntityComponentArray eca, ComponentMapping mapping, EntityId entity, math::Vec3f finalPos) {
                        TransformComponent* tc = mapping.getAgregate<TransformComponent>(eca, entity);
                        std::vector<math::Vec3f> positions;
                        if (tc != nullptr) {
                            math::Vec3f startPos = math::Vec3f(tc->center().x, tc->center.y, tc->center.z);
                            if (getGridCellAt(finalPos) != nullptr) {
                                unsigned int cpt = 0;
                                positions.push_back(startPos);
                                math::Vec3f currentPos = getGridCellAt(startPos)->getCenter();
                                if (currentPos != startPos)
                                    positions.push_back(currentPos);
                                std::vector<Cell*> children;
                                while (!getGridCellAt(finalPos)->isPassable() && cpt < 1000) {
                                    Cell *parent = getGridCellAt(finalPos);

                                    std::vector<Cell*> children = getNeightbours(eca, mapping, entity, parent, true);

                                    int distMin = children[0]->getCenter().computeDist(getGridCellAt(startPos)->getCenter());
                                    int indMin = 0;
                                    for (unsigned int i = 1; i < children.size(); i++) {
                                        std::cout<<children[i]->getCenter()<<std::endl;
                                        int dist = children[i]->getCenter().computeDist(getGridCellAt(startPos)->getCenter());
                                        if (dist < distMin) {
                                            distMin = dist;
                                            indMin = i;
                                        }
                                    }
                                    finalPos = children[indMin]->getCenter();
                                    cpt++;
                                }
                                //Tant qu'on est pas arrivé sur la case finale.
                                while (getGridCellAt(currentPos) != getGridCellAt(finalPos) && cpt < 1000) {
                                    //On recherche les Cells fils. (ou voisines.)

                                    Cell *parent = getGridCellAt(currentPos);
                                    parent->setTraveled(true);
                                    std::vector<Cell*> children = getNeightbours(eca, mapping, entity, parent, false);
                                    //std::cout<<"size : "<<children.size()<<std::endl;
                                    unsigned int j = 0;
                                    while (j < children.size() && children[j]->isTraveled())
                                        j++;
                                    if (j == children.size())
                                        j--;
                                    int distMin = children[j]->getCenter().computeDist(getGridCellAt(finalPos)->getCenter());
                                    int indMin = j;
                                    for (unsigned int i = j; i < children.size(); i++) {
                                        if (!children[i]->isTraveled()) {
                                            int dist = children[i]->getCenter().computeDist(getGridCellAt(finalPos)->getCenter());

                                            if (dist < distMin) {

                                                distMin = dist;
                                                indMin = i;
                                            }
                                        }
                                    }
                                    currentPos = children[indMin]->getCenter();
                                    if (positions.size() > 1) {
                                        math::Vec3f v1 = positions[positions.size()-1] - positions[positions.size() - 2];
                                        math::Vec3f v2 = currentPos - positions[positions.size()-1];
                                        if (math::Math::abs(math::Math::toDegrees(v1.getAngleBetween(v2, v1.cross(v2)))) != 180)
                                            positions.push_back(currentPos);
                                    } else {
                                         positions.push_back(currentPos);
                                    }
                                    cpt++;
                                }
                                if (finalPos != positions.back())
                                    positions.push_back(finalPos);
                                for (unsigned int i = 0; i < positions.size(); i++) {
                                     getGridCellAt(positions[i])->setTraveled(false);
                                }
                                if (cpt == 1000)
                                    positions.clear();
                            }
                        }
                        return positions;
                    }
                    /**
                    * \fn  CellMap<Entity>* getGridCellAtFromCoords(Vec3f coords)
                    * \brief get the cell at a given position.
                    * \param Vec3f coords : the coordinates of the cell.
                    */
                    Cell* getGridCellAtFromCoords(math::Vec3f coords);
                    /**
                    * \fn GridMap(int cellWith, int cellHeight, int nbLayers)
                    * \brief constructor.
                    * \param cellWidth : the width of the cells.
                    * \param cellHeight : the height of the cells.
                    * \param nbLayers : the depth of the cells.
                    */
                    Grid (int cellWidth=100, int cellHeight=50, int cellDepth=0);
                    /** \fn  ~GridMap ();
                    *   \brief destructor.
                    */
                    ~Grid ();
                    /**
                    * \fn bool addEntity (Entity *entity);
                    * \brief add an entity into the grid.
                    * \param Entity* entity : the entity to add.
                    * \return if the entity has been successfully added.
                    */
                    bool addEntity (physic::BoundingBox globalBounds, EntityId entity);
                    /**
                    * \fn Entity* getEntity (int id);
                    * \brief get an entity depending on the given id.
                    * \param id : the id of the entity.
                    * \return the entity.
                    */
                    EntityId getEntity (ComponentMapping mapping, int id);
                    //EntityId getEntity (std::string name);
                    /**
                    * \fn deleteEntity(Entity* entity)
                    * \brief remove an entity from the grid and from the memory.
                    * \param entity : the entity to delete.
                    * \return if the entity has been successfully deleted.
                    */
                    bool removeEntity(physic::BoundingBox globalBounds, EntityId entity);
                    /**
                    * \fn  bool collideWithEntity(Entity* entity);
                    * \brief check if an entity collide with the entity of the grid.
                    * \param entity : check if the entity is colliding with an entity (or a cell) of the grid.
                    * \return if the entity is in collision with the grid.
                    */
                    template <typename EntityComponentArray>
                    bool collideWithEntity(EntityComponentArray eca, ComponentMapping mapping, EntityId entity, math::Vec3f position) {
                        Cell* cell = getGridCellAt(position);
                        if (cell != nullptr) {
                            if (!cell->isPassable())
                                return true;
                            std::vector<Cell*> neightbours = getNeightbours(eca, mapping, entity,cell,true);
                            for (unsigned int i = 0; i < neightbours.size(); i++) {
                                if (!neightbours[i]->isPassable()) {
                                    return true;
                                }
                            }
                            EntityId rootId = mapping.getRoot(entity);
                            ColliderComponent* collider = mapping.getAgregate<ColliderComponent>(eca, rootId);

                            if (collider != nullptr) {
                                math::Vec3f t = position - collider->collisionVolume->getCenter();
                                physic::BoundingVolume* cv = collider->colidionVolume->clone().release();
                                cv->move(t);
                                for (unsigned int k = 0; k < cell->getEntitiesInside().size(); k++)  {
                                    ColliderComponent* collider2 = mapping.getAgregate<ColliderComponent>(eca, cell->getEntitiesInside()[k]);
                                    if (collider2 != nullptr && cell->getEntitiesInside()[k].get().load() != entity.get().load()) {
                                        physic::BoundingVolume* cv2 = collider2->boundingVolume;
                                        physic::CollisionResultSet::Info info;
                                        if (cv->intersects(*cv2, info)) {
                                            /*info.entity = cell->getEntitiesInside()[k]->getRootEntity();
                                            info.center = cv->getCenter();
                                            physic::CollisionResultSet::pushCollisionInfo(info);*/
                                            if (cv->getChildren().size() == 0) {
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        return false;
                    }
                    /**
                    * \fn std::vector<Entity*> getEntities()
                    * \brief return every entities which are stored into each cells of the grid.
                    */
                    std::vector<EntityId> getEntities ();
                    /**
                    * \fn  std::vector<CellMap<Entity>*> getCasesMap()
                    * \brief get every cells of the map.
                    * \return the cells.
                    */
                    std::vector<Cell*> getCells ();
                    /**
                    * \fn std::vector<CellMap<Entity>*> getCasesInBox(BoundingBox bx)
                    * \brief return every cells which are in the given bounding box.
                    * \param bx : the bounding box.
                    * \return the cells in the bounding box.
                    */
                    std::vector<Cell*> getCellsInBox(physic::BoundingBox bx);
                    /**
                    * \fn bool  containsEntity (Entity *entity, Vec3f pos);
                    * \brief check if the grid contains the entity at the given pos.
                    * \param entity : the entity.
                    * \param Vec3f pos : the given position.
                    */
                    bool  containsEntity (EntityId entity, math::Vec3f pos);
                    /**
                    * \fn std::vector<Entity*> getEntitiesInBox(BoundingBox bx)
                    * \brief get the entities which are in the given bounding box.
                    * \param bx : the bounding box.
                    * \return std::vector<Entity*> : the entities.
                    */
                    template <typename EntityComponentArray>
                    std::vector<EntityId> getEntitiesInBox(EntityComponentArray da, ComponentMapping mapping, physic::BoundingBox box) {
                        std::vector<EntityId> entities;
                        int x = box.getPosition().x;
                        int y = box.getPosition().y;
                        int z = box.getPosition().z;
                        int endX = box.getPosition().x + box.getWidth();
                        int endY = box.getPosition().y + box.getHeight();
                        int endZ = box.getPosition().z + box.getDepth();
                        physic::BoundingBox bx (x, y, z, endX-x, endY-y, z - endZ);
                        for (int i = x; i <= endX; i+=offsetX) {
                            for (int j = y; j <= endY; j+=offsetY) {
                                for (int k = z; k <= endZ; k+= offsetZ) {
                                    math::Vec3f point(i, j, k);
                                    Cell* cell = getGridCellAt(point);
                                    if (cell != nullptr) {
                                        for (unsigned int n = 0; n < cell->getEntitiesInside().size(); n++) {
                                           EntityId entity = cell->getEntityInside(n);
                                           bool contains = false;
                                           for (unsigned int k = 0; k < entities.size() && !contains; k++) {
                                                if (entities[k].get().load() == entity.get().load())
                                                    contains = true;
                                           }
                                           physic::BoundingBox globalBounds = mapping.getAgregate<TransformComponent>(da, entity);
                                           if (!contains && bx.intersects(globalBounds) || bx.isInside(globalBounds) || globalBounds.isInside(bx)) {

                                                entities.push_back(entity);
                                           }
                                        }
                                    }
                                }
                            }
                        }
                        return entities;
                    }
                    /**
                    * \fn  CellMap<Entity>* getGridCellAt (Vec3f point)
                    * \brief get the cell at the given position.
                    * \param point : the point.
                    * \return the cell.
                    */
                    Cell* getGridCellAt (math::Vec3f point);
                    /**
                    * \fn Vec3f getCoordinatesAt (Vec3f &point);
                    * \brief get the cell's coordinates at the given pos.
                    * \param point : the position.
                    * \return Vec3f the coordinates.
                    */
                    math::Vec3f getCoordinatesAt (math::Vec3f &point);
                    /**
                    * \fn void createCellMap(Vec3f &point);
                    * \brief create a cell a the given position.
                    * \param point : the position.
                    */
                    void createCell(math::Vec3f &point);
                    /**
                    * \fn Vec3f getMins ()
                    * \brief return the minimums of the grid.
                    * \return the minimums of the grid.
                    */
                    math::Vec3f getMins ();
                    /**
                    * \fn Vec3f getSize ();
                    * \brief return the size of the grid.
                    * \return Vec3f the size.
                    */
                    math::Vec3f getSize ();
                    /**
                    * \fn int getNbCasesPerRow ();
                    * \brief return the number of cases per row.
                    * \return the number of cases per row.
                    */
                    int getNbCellsPerRow ();
                    /**
                    * \fn int getNbCasesPerCol ();
                    * \brief return the number of cases per col.
                    * \return the number of cases per col.
                    */
                    int getNbCellsPerCol ();
                    /**
                    * \fn void setBaseChangementMatrix (BaseChangementMatrix bm)
                    * \brief change the base changement matrix.
                    * \param bm : the base changement matrix.
                    */
                    void setBaseChangementMatrix (BaseChangementMatrix bm);
                    /**
                    * \fn BaseChangementMatrix getBaseChangementMatrix()
                    * \brief get the base changement matrix.
                    * \return the base changement matrix.
                    */
                    BaseChangementMatrix getBaseChangementMatrix();
                    /**
                    * \fn int getCellDepth();
                    * \brief get the depth of the cell.
                    * \return the depth of the cell.
                    */
                    int getCellDepth();
                    /**
                    * \fn int getCellWidth();
                    * \brief get the width of the cell.
                    * \return the with of the cell.
                    */
                    int getCellWidth ();
                    /**
                    * \fn int getCellHeight();
                    * \brief get the height of the cell.
                    * \return the height of the cell.
                    */
                    int getCellHeight ();
                    int getOffsetX() {
                        return offsetX;
                    }
                    int getOffsetY() {
                        return offsetY;
                    }
                    int getOffsetZ() {
                        return offsetZ;
                    }
                    private:
                    BaseChangementMatrix bm; /**> the base changement matrix.*/
                    /**
                    * \fn void checkExts()
                    * \brief check the extremity of the grid.
                    */
                    void checkExts();
                    /**
                    * \fn void removeCellMap (CellMap<Entity> *cell)
                    * \brief remove a cell from the grid and from the memory.
                    * \param cell : the cell to remove.
                    */
                    void removeCell (Cell *cell);
                private :
                    std::vector<Cell*> cells; /**Cells of the grid, I use a raw pointer here because, the grid is dynamic so I have to copy the pointers.*/
                    int nbCellsPerRow, nbCellsPerCol, /**> the number of cases per row, the number of cases per columns*/
                    minX, minY, minZ, maxX, maxY, maxZ, /**> the mins and maxs of the grid.*/
                    cellWidth, cellHeight, cellDepth, /**> the dimentions of the cells of the grid.*/
                    offsetX, offsetY, offsetZ; /**> the offsets of the cells of the grid.*/
            };
        }
    }
}
#endif
