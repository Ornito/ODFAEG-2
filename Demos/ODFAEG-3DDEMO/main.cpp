#include "application.h"
using namespace odfaeg::math;
using namespace odfaeg::physic;
using namespace odfaeg::core;
using namespace odfaeg::graphic;
using namespace odfaeg::window;
int main (int argv, char* argc[]) {
    MyAppli appli(Vec2f(800, 600), "test");
    return appli.exec();
    // crée la fenêtre
    /*RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, ContextSettings(0, 0, 4, 4, 6));
    window.setVerticalSyncEnabled(true);

    // activation de la fenêtre
    window.setActive(true);
    //Initialisation de glew.
    glewInit();
    // chargement des ressources, initialisation des états OpenGL, ...
    //Création du vertex array.
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    //Vue.
    View view(800, 600, 80, 0.1f, 1000);
    window.setView(view);
    //Pour générer la heightmap.
    RenderComponentManager rcm(window);
    Map theMap(&rcm, "Map test", 100, 100, 0);
    std::vector<Tile*> tGround;
    std::vector<Tile*> tWall;
    Texture text;
    text.setRepeated(true);
    text.loadFromFile("tilesets/Terre2.jpg");
    text.setSmooth(true);
    tGround.push_back(new Tile(&text, Vec3f(0, 0, 0), Vec3f(50, 50, 0),sf::IntRect(0, 0, 100*20, 100*20)));
    BoundingBox mapZone (-500, -500, 0, 1000, 1000, 50);
    theMap.generate_map(tGround, tWall, Vec2f(50, 50), mapZone, true);
    theMap.checkVisibleEntities();
    std::vector<Entity*> visibleEntities = theMap.getVisibleEntities("E_BIGTILE");
    std::vector<Vertex> vertices;
    //Transformation des sommets visible de la heightmap.
    for (unsigned int i = 0; i < visibleEntities.size(); i++) {
        for (unsigned int j = 0; j < visibleEntities[i]->getNbFaces(); j++) {
            for (unsigned int n = 0; n < visibleEntities[i]->getFace(j)->getVertexArray().getVertexCount(); n++) {
                 TransformMatrix tm = visibleEntities[i]->getTransform();
                 Vec3f t = tm.transform(Vec3f(visibleEntities[i]->getFace(j)->getVertexArray()[n].position.x, visibleEntities[i]->getFace(j)->getVertexArray()[n].position.y, visibleEntities[i]->getFace(j)->getVertexArray()[n].position.z));
                 Vertex v (sf::Vector3f(t.x, t.y, t.z), visibleEntities[i]->getFace(j)->getVertexArray()[n].color, visibleEntities[i]->getFace(j)->getVertexArray()[n].texCoords);
                 vertices.push_back(v);
            }
        }
    }
    //VBO de la heightmap.
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Quad plein écran.
    std::vector<Vertex> vertices2;
    vertices2.push_back(Vertex(sf::Vector3f(-400, -300, 0)));
    vertices2.push_back(Vertex(sf::Vector3f(400, -300, 0)));
    vertices2.push_back(Vertex(sf::Vector3f(400, 300, 0)));
    vertices2.push_back(Vertex(sf::Vector3f(-400, 300, 0)));
    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(Vertex), &vertices2[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Création de la linked list.
    GLuint maxNodes = 20 * window.getView().getSize().x * window.getView().getSize().y;
    GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint);
    GLuint atomicBuffer, linkedListBuffer, headPtrTex, clearBuf;
    glGenBuffers(1, &atomicBuffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    glGenBuffers(1, &linkedListBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, linkedListBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glGenTextures(1, &headPtrTex);
    glBindTexture(GL_TEXTURE_2D, headPtrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, window.getView().getSize().x, window.getView().getSize().y);
    glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::vector<GLuint> headPtrClearBuf(window.getView().getSize().x * window.getView().getSize().y, 0xffffffff);
    std::vector<GLuint> linkedListClearBuffer(maxNodes * nodeSize, 0);
    glGenBuffers(1, &clearBuf);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
    &headPtrClearBuf[0], GL_STATIC_COPY);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, linkedListBuffer);
    // la boucle principale
    bool running = true;
    while (running)
    {
        // gestion des évènements
        IEvent event;
        while (window.pollEvent(event))
        {
            if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED)
            {
                // on stoppe le programme
                running = false;
            }
            else if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_RESIZED)
            {
                // on ajuste le viewport lorsque la fenêtre est redimensionnée
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
        }

        // effacement les tampons de couleur/profondeur
        window.clear();

        // dessin...

        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;*/
}
