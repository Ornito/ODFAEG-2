#include "../../../include/odfaeg/Graphics/tile.h"
namespace odfaeg {
    namespace graphic {

        /*G�re les diff�rents tiles, cette classe h�rite de Sprite.
        */
        //Cr�e un tile, avec une image pass�e en param�tre.

        Tile::Tile() : Entity (math::Vec3f(0, 0, 0), math::Vec3f(0, 0, 0),math::Vec3f(0, 0, 0),"E_TILE") {

        }
        Tile::Tile (const Tile& tile) : Entity (tile.getPosition(), tile.getSize(), tile.getSize() * 0.5f, "E_TILE"){
            Vertex v1(sf::Vector3f(tile.getPosition().x, tile.getPosition().y, tile.getPosition().z));
            Vertex v2(sf::Vector3f(tile.getPosition().x + tile.getSize().x, tile.getPosition().y, tile.getPosition().z));
            Vertex v3(sf::Vector3f(tile.getPosition().x + tile.getSize().x, getPosition().y + tile.getSize().y, tile.getPosition().z));
            Vertex v4(sf::Vector3f(tile.getPosition().x, tile.getPosition().y + tile.getSize().y, tile.getPosition().z));
            sf::IntRect subRect = tile.getFaces()[0]->getMaterial().getTexRect();
            v1.texCoords = sf::Vector2f(subRect.left, subRect.top);
            v2.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top);
            v3.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top + subRect.height);
            v4.texCoords = sf::Vector2f(subRect.left, subRect.top + subRect.height);
            Face* face = new Face(sf::TrianglesFan,getTransform());
            face->getMaterial().addTexture(tile.getFaces()[0]->getMaterial().getTexture(), subRect);
            face->append(v1, 0);
            face->append(v2, 1);
            face->append(v3, 2);
            face->append(v4, 3);
            face->getMaterial().setTexId(tile.getFaces()[0]->getMaterial().getTexId());
            addFace(face);
            parent = tile.parent;
        }
        Tile* Tile::clone() {
            return new Tile(getFaces()[0]->getMaterial().getTexture(), getPosition(), getSize(), getFaces()[0]->getMaterial().getTexRect(),getFaces()[0]->getVertexArray()[0].color,getParent());
        }
        Tile::Tile (const Texture *image, math::Vec3f position, math::Vec3f size, sf::IntRect subRect, sf::Color color, Entity *parent)
        : Entity (position, size, size * 0.5f, "E_TILE") {
            Face* face = new Face(sf::TrianglesFan,getTransform());
            Vertex v1(sf::Vector3f(position.x, position.y, position.z));
            Vertex v2(sf::Vector3f(position.x + size.x, position.y, position.z));
            Vertex v3(sf::Vector3f(position.x + size.x, position.y + size.y, position.z));
            Vertex v4(sf::Vector3f(position.x, position.y + size.y, position.z));
            v1.texCoords = sf::Vector2f(subRect.left, subRect.top);
            v2.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top);
            v3.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top + subRect.height);
            v4.texCoords = sf::Vector2f(subRect.left, subRect.top + subRect.height);
            v1.color = v2.color = v3.color = v4.color = color;
            face->append(v1, 0);
            face->append(v2, 1);
            face->append(v3, 2);
            face->append(v4, 3);
            face->getMaterial().addTexture(image, subRect);
            addFace(face);
        }
        void Tile::changeVerticesHeights(float h1, float h2, float h3, float h4) {
            getFaces()[0]->getVertexArray()[0].position.z = h1;
            getFaces()[0]->getVertexArray()[1].position.z = h2;
            getFaces()[0]->getVertexArray()[2].position.z = h3;
            getFaces()[0]->getVertexArray()[3].position.z = h4;
            float min, max;
            if (h1 < h2 && h1 < h3 && h1 < h4)
                min = h1;
            else if (h2 < h1 && h2 < h3 && h2 < h4)
                min = h2;
            else if (h3 < h1 && h3 < h2 && h3 < h4)
                min = h3;
            else
                min = h4;
            if (h1 > h2 && h1 > h3 && h1 > h4)
                max = h1;
            else if (h2 > h1 && h2 > h3 && h2 > h4)
                max = h2;
            else if (h3 > h1 && h3 > h2 && h3 > h4)
                max = h3;
            else
                max = h4;
            setPosition(math::Vec3f(getPosition().x, getPosition().y, 0));
            setSize(math::Vec3f(getSize().x, getSize().y, max - min));
            getFaces()[0]->getVertexArray().updateVBOBuffer();
        }
        bool Tile::operator== (Entity &other) {
            if (other.getType() != "E_TILE")
                return false;
            Tile &tile = static_cast<Tile&> (other);
            return getFaces()[0]->getMaterial() == other.getFaces()[0]->getMaterial() &&
                   getPosition().x == tile.getPosition().x &&
                   getPosition().y == tile.getPosition().y &&
                   getPosition().z == tile.getPosition().z &&
                   getSize().x == tile.getSize().x &&
                   getSize().y == tile.getSize().y &&
                   getFaces()[0]->getMaterial().getTexRect().left == tile.getFaces()[0]->getMaterial().getTexRect().left &&
                   getFaces()[0]->getMaterial().getTexRect().top == tile.getFaces()[0]->getMaterial().getTexRect().top &&
                   getFaces()[0]->getMaterial().getTexRect().height == tile.getFaces()[0]->getMaterial().getTexRect().height &&
                   getFaces()[0]->getMaterial().getTexRect().width == tile.getFaces()[0]->getMaterial().getTexRect().width;
        }

        bool Tile::operator!= (Tile &tile) {
            return !(*this == tile);
        }

        void Tile::onDraw(RenderTarget &target, RenderStates states) const {
            states.texture = const_cast<Tile*>(this)->getFaces()[0]->getMaterial().getTexture();
            target.draw(const_cast<Tile*>(this)->getFaces()[0]->getVertexArray(), states);

        }

        void Tile::setColor (sf::Color color) {
            for (unsigned int j = 0; j < getFaces()[0]->getVertexArray().getVertexCount(); j++)
                getFaces()[0]->getVertexArray()[j].color = color;
        }
        Tile::~Tile () {

        }
    }
}

