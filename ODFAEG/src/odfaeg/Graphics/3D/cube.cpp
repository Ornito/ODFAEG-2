#include "../../../../include/odfaeg/Graphics/3D/cube.h"
namespace odfaeg {
    namespace graphic {
        namespace g3d {
            using namespace sf;
            Cube::Cube (math::Vec3f position, float w, float h, float d, Color color) : Entity(position,math::Vec3f(w, h, d),math::Vec3f(w*0.5f,h*0.5f,d*0.5f),"E_CUBE") {
                    m_color = color;
                    Face* face1 = new Face(Quads, getTransform());
                    Face* face2 = new Face(Quads, getTransform());
                    Face* face3 = new Face(Quads, getTransform());
                    Face* face4 = new Face(Quads, getTransform());
                    Face* face5 = new Face(Quads, getTransform());
                    Face* face6 = new Face(Quads, getTransform());
                    Vertex vertices[24];
                    for (unsigned int i = 0; i < 24; i++)
                        vertices[i].color = color;
                    //Devant
                    vertices[0].position = Vector3f(position.x, position.y + d, position.z);
                    vertices[1].position = Vector3f(position.x, position.y + d, position.z + h);
                    vertices[2].position = Vector3f(position.x + w, position.y + d, position.z + h);
                    vertices[3].position = Vector3f(position.x + w, position.y + d, position.z);
                    face1->append(vertices[0], 0);
                    face1->append(vertices[1], 1);
                    face1->append(vertices[2], 2);
                    face1->append(vertices[3], 3);
                    face1->getVertexArray().setEntity(this);
                    //Dessus
                    vertices[4].position = Vector3f(position.x, position.y, position.z + h);
                    vertices[5].position = Vector3f(position.x + w, position.y, position.z + h);
                    vertices[6].position = Vector3f(position.x + w, position.y + d, position.z + h);
                    vertices[7].position = Vector3f(position.x, position.y + d, position.z + h);
                    face2->append(vertices[4], 0);
                    face2->append(vertices[5], 1);
                    face2->append(vertices[6], 2);
                    face2->append(vertices[7], 3);
                    face2->getVertexArray().setEntity(this);
                    //Derrière.
                    vertices[8].position = Vector3f(position.x, position.y, position.z);
                    vertices[9].position = Vector3f(position.x, position.y, position.z + h);
                    vertices[10].position = Vector3f(position.x + w, position.y, position.z + h);
                    vertices[11].position = Vector3f(position.x + w, position.y, position.z);
                    face3->append(vertices[8], 0);
                    face3->append(vertices[9], 1);
                    face3->append(vertices[10], 2);
                    face3->append(vertices[11], 3);
                    face3->getVertexArray().setEntity(this);
                    //Dessous.
                    vertices[12].position = Vector3f(position.x, position.y, position.z);
                    vertices[13].position = Vector3f(position.x + w, position.y, position.z);
                    vertices[14].position = Vector3f(position.x + w, position.y + d, position.z);
                    vertices[15].position = Vector3f(position.x, position.y + d, position.z);
                    face4->append(vertices[12], 0);
                    face4->append(vertices[13], 1);
                    face4->append(vertices[14], 2);
                    face4->append(vertices[15], 3);
                    face4->getVertexArray().setEntity(this);
                    //Gauche.
                    vertices[16].position = Vector3f(position.x, position.y, position.z);
                    vertices[17].position = Vector3f(position.x, position.y, position.z + h);
                    vertices[18].position = Vector3f(position.x, position.y + d, position.z + h);
                    vertices[19].position = Vector3f(position.x, position.y + d, position.z);
                    face5->append(vertices[16], 0);
                    face5->append(vertices[17], 1);
                    face5->append(vertices[18], 2);
                    face5->append(vertices[19], 3);
                    face5->getVertexArray().setEntity(this);
                    //Droite.
                    vertices[20].position = Vector3f(position.x + w, position.y, position.z);
                    vertices[21].position = Vector3f(position.x + w, position.y, position.z + h);
                    vertices[22].position = Vector3f(position.x + w, position.y + d, position.z + h);
                    vertices[23].position = Vector3f(position.x + w, position.y + d, position.z);
                    face6->append(vertices[20], 0);
                    face6->append(vertices[21], 1);
                    face6->append(vertices[22], 2);
                    face6->append(vertices[23], 3);
                    face6->getVertexArray().setEntity(this);
                    addFace(face1);
                    addFace(face2);
                    addFace(face3);
                    addFace(face4);
                    addFace(face5);
                    addFace(face6);
                    setDrawMode(Entity::NORMAL);
            }
            void Cube::setTexCoords(int face, sf::IntRect texCoords) {
                VertexArray& va = getFace(face)->getVertexArray();
                va[0].texCoords = sf::Vector2f(texCoords.left, texCoords.top);
                va[1].texCoords = sf::Vector2f(texCoords.width, texCoords.top);
                va[2].texCoords = sf::Vector2f(texCoords.width, texCoords.height);
                va[3].texCoords = sf::Vector2f(texCoords.left, texCoords.height);
            }
            void Cube::onDraw (RenderTarget &target, RenderStates states) {
                for (unsigned int i = 0; i < getFaces().size(); i++) {
                    states.texture = const_cast<Cube*>(this)->getFaces()[i]->getMaterial().getTexture();
                    target.draw (getFaces()[i]->getVertexArray(),states);
                }
            }
        }
    }
}

