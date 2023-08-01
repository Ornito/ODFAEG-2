#ifndef ODFAEG_ECS_CELL_HPP
#define ODFAEG_ECS_CELL_HPP
#include "../../Physics/boundingPolyhedron.h"
#include "../../Core/ecs.hpp"
#include <vector>
namespace odfaeg {
    namespace graphic {
        namespace ecs {
            /**
          * \file Cell.h
          * \class Cell
          * \brief Represent a cell of a space partitioning structure.
          * \author Duroisin.L
          * \version 1.0
          * \date 1/02/2014
          *  Represent a cell of a space partitioning structure.
          */
            class Cell {
                public :
                    /**
                    * \fn Cell (BoundingPolyhedron* bp, Vec3f coords);
                    * \brief constructor.
                    * \param bp : the volume of the cell.
                    * \param coords : the coords of the cell.
                    */
                    Cell (physic::BoundingPolyhedron bp, math::Vec3f coords);
                    /**
                    * \fn BoundingPolyhedron* getCellVolume ();
                    * \return the volume of the cell.
                    */
                    physic::BoundingPolyhedron getCellVolume ();
                    /**
                    * \fn E* getEntityInside (int index);
                    * \brief get an entity which is into the cell by its index.
                    * \param the index.
                    * \return a pointer to the entity.
                    */
                    EntityId getEntityInside (unsigned int index);
                    unsigned int getNbEntitiesInside();
                    /**
                    * \fn std::vector<E*> getEntitiesInside ();
                    * \brief get the visible entities which are inside the cell.
                    * \return all the visible entities which are inside the cells.
                    */
                    std::vector<EntityId> getEntitiesInside ();
                    /**
                    * \fn bool isEntityInside ();
                    * \brief test if there is at least an entity is inside a cell.
                    * \return if there is at least an entity inside the cell.
                    */
                    bool isEntityInside ();
                    /**
                    * \fn bool containsEntity (E *entity);
                    * \brief test if a cell contains an entity.
                    * \param entity : the entity.
                    * \return if the cells contains the entity.
                    */
                    bool containsEntity (EntityId entity);
                    /**
                    * \fn  void addEntity (E *entity);
                    * \brief add an entity into the cell.
                    * \param entity : the entity.
                    */
                    void addEntity (EntityId entity);
                    /**
                    * \fn bool removeEntity (E * entity);
                    * \brief remove an entity from the cell.
                    * \param entity : the entity to remove.
                    */
                    bool removeEntity (EntityId entity);
                    /**
                    * \fn void setStateChanged (bool b);
                    * \brief set if the state of the cells have been changed. (This happens when the cell become passable or not passable)
                    * \param b : if the state have been changed.
                    */
                    void setStateChanged (bool b);
                    /**
                    * \fn  bool isStateChanged ();
                    * \brief tells is the state of the cell has been changed.
                    * \return if the cell has already been visited.
                    */
                    bool isStateChanged ();
                    /**
                    * \fn bool isPassable()
                    * \brief check if we can pass on the cell or not.
                    * \return if we can pass on the cell.
                    */
                    bool isPassable ();
                    /**
                    * \fn void setPassable (bool passable);
                    * \brief set if the cell is passable or not.
                    * \param passable : if the cell is passable or not.
                    */
                    void setPassable (bool passable);
                    /**
                    * \fn  bool operator== (const Cell &cell);
                    * \brief test if the cell is equal with another one.
                    * \param cell : the other cell.
                    * \return if the two cells are equal.
                    */
                    bool operator== (const Cell &cell);
                    /**
                    * \fn Vec3f getCoords ();
                    * \brief get the coordinates of the cells. (Usefull to store the cell into the vector of a grid)
                    * \return the coordinates of the cell.
                    */
                    math::Vec3f getCoords ();
                    /** \fn Vec3f& getCenter ();
                    *   \brief get the center of the cell.
                    *   \return the center of the cell.
                    */
                    math::Vec3f getCenter ();
                    /**
                    * \fn bool isTraveled ();
                    * \brief test is the cell has already been visited or not. (Used for the pathfinding algorithm)
                    * \return if the cell has already been visited.
                    */
                    bool isTraveled ();
                    /**
                    * \fn void setTraveled (bool traveled);
                    * \brief set if the cell has already been traveled or not.
                    * \param traveled : if the cell has already been visited.
                    */
                    void setTraveled (bool traveled);
                    /**
                    * \fn ~Cell();
                    * \brief destructor.
                    */
                    /*bool removeEntity (std::string type);
                    bool deleteEntity (std::string type);*/
                private :
                    std::vector<EntityId> entitiesInside; /**> the entities which are inside the cell.*/
                    physic::BoundingPolyhedron cellVolume; /**> the volume of the cell.*/
                    bool passable, stateChanged, traveled; /**> if the cell is passable, if the state of the cell have been changed and if the cell have been visited.*/
                    math::Vec3f coords; /**> the coordinates and the center of the cell.*/
            };
        }
    }
}
#endif
