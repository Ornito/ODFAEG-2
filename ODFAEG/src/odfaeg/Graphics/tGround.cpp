#include "../../../include/odfaeg/Graphics/tGround.h"
namespace odfaeg {
    namespace graphic {
        using namespace sf;
        BigTile::BigTile (math::Vec3f pos, math::Vec2f tileSize, int nbTilesPerRow)
        : Entity (pos, math::Vec3f (0, 0, 0), math::Vec3f (0, 0, 0), "E_BIGTILE"),
        tileSize(tileSize), nbTilesPerRow(nbTilesPerRow) {

        }
        void BigTile::addTile (Tile *tile, math::Vec2f tilePos, float* heights) {
           if (heights != nullptr) {
                int xPosition = math::Math::abs(getPosition().x) / tileSize.x + tilePos.x / tileSize.x;
                int yPosition = math::Math::abs(getPosition().y) / tileSize.y + tilePos.y / tileSize.y;
                int position = yPosition * nbTilesPerRow + xPosition;
                int belowPos = (yPosition - 1) * nbTilesPerRow  + xPosition;
                if (xPosition > 0 && position - 1 < m_tiles.size()) {
                    heights[0] = m_tiles[position-1]->getFaces()[0]->getVertexArray()[1].position.z;
                    heights[3] = m_tiles[position-1]->getFaces()[0]->getVertexArray()[2].position.z;
                }
                if (belowPos  > 0 && belowPos < getChildren().size()) {
                    heights[0] = m_tiles[belowPos]->getFaces()[0]->getVertexArray()[3].position.z;
                    heights[1] = m_tiles[belowPos]->getFaces()[0]->getVertexArray()[2].position.z;
                }
                tile->changeVerticesHeights(
                            heights[0],
                            heights[1],
                            heights[2],
                            heights[3]);
                for (unsigned int i = 0; i < getNbFaces(); i++) {
                    if (tile->getFace(0)->getMaterial() == getFace(i)->getMaterial()) {
                        VertexArray& va = getFace(0)->getVertexArray();
                        for (unsigned int j = 0; j < tile->getFace(0)->getVertexArray().getVertexCount(); j++) {
                            va.append(tile->getFace(0)->getVertexArray()[j]);
                        }
                    } else {
                        VertexArray va;
                        for (unsigned int j = 0; j < tile->getFace(0)->getVertexArray().getVertexCount(); j++) {
                            va.append(tile->getFace(0)->getVertexArray()[j]);
                        }
                        Face* face = new Face(va,tile->getFace(0)->getMaterial(), getTransform());
                        addFace(face);
                    }
                }
            } else {
                tile->setParent(this);
                addChild(tile);
            }
            m_tiles.push_back(tile);
        }
        float BigTile::getHeight(math::Vec2f point) {
             int xPosition = math::Math::abs(getPosition().x) / tileSize.x + point.x / tileSize.x;
             int yPosition = math::Math::abs(getPosition().y) / tileSize.y + point.y / tileSize.y;
             int position = yPosition * nbTilesPerRow + xPosition;
             float dx = (point.x - m_tiles[position]->getPosition().x) / tileSize.x;
             float dy = (point.y - m_tiles[position]->getPosition().y) / tileSize.y;
             int h1 = m_tiles[position]->getFaces()[0]->getVertexArray()[0].position.z;
             int h2 = m_tiles[position]->getFaces()[0]->getVertexArray()[1].position.z;
             int h3 = m_tiles[position]->getFaces()[0]->getVertexArray()[2].position.z;
             int h4 = m_tiles[position]->getFaces()[0]->getVertexArray()[3].position.z;
             return ((1 - dx) * h1 + dx * h2) * (1 - dy) + ((1 - dx) * h4 + dx * h3) * dy;
        }
        bool BigTile::operator== (Entity &other) {
            if (other.getType() != "E_BIGTILE")
                return false;
            BigTile &tGround = static_cast<BigTile&> (other);
            if (getChildren().size() != other.getChildren().size())
                return false;
            for (unsigned int i = 0; i < getChildren().size(); i++) {
                if (!(*getChildren()[i] == *other.getChildren()[i]))
                    return false;
            }
            return getPosition().x == tGround.getPosition().x
                    && getPosition().y == tGround.getPosition().y
                    && getSize().x == tGround.getSize().x
                    && getSize().y == tGround.getSize().y;
        }
    }
}

