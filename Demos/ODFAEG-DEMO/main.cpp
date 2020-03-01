#include "application.h"
#include <SFML/OpenGL.hpp>
#include "odfaeg/Window/window.hpp"
#include "odfaeg/Window/context.hpp"
#include <SFML/Window/WindowStyle.hpp>
#include "odfaeg/Graphics/sprite.h"
#include <stdio.h>

#include <stddef.h>
using namespace odfaeg::core;
using namespace odfaeg::math;
using namespace odfaeg::physic;
using namespace odfaeg::graphic;
using namespace odfaeg::window;
using namespace odfaeg::audio;
using namespace sorrok;
class BigInt {
    public :
        BigInt() {
        }
        BigInt(std::string str) {
            fromStr(str);
        }
        static BigInt genRandom(unsigned int nbBits) {
            BigInt n;
            for (unsigned int i = 0; i < nbBits-1; i++) {
                bool bit = (int) Math::random(2);
                n.bits.push_back(bit);
            }
            n.bits.push_back(1);
            return n;
        }
        BigInt operator+= (BigInt& b) {
            (*this) = (*this) + b;
            return (*this);
        }
        BigInt operator+ (BigInt& b) {
            if (isNull())
                return b;
            if (b.isNull())
                return *this;
            BigInt s;
            if (bits.size() != b.bits.size()) {
                if (bits.size() > b.bits.size()) {
                    b.addZeros(bits.size() - b.bits.size());
                } else {
                    addZeros(b.bits.size() - bits.size());
                }
            }
            bool r = 0;
            for (unsigned int i = 0; i < bits.size(); i++) {
                bool bit = addBits(bits[i], b.bits[i], r);
                s.bits.push_back(bit);
            }
            s.bits.push_back(r);
            removeZeros();
            b.removeZeros();
            s.removeZeros();
            return s;
        }
        BigInt operator- (BigInt& b) {
            BigInt s, c;
            if (bits.size() > b.bits.size()) {
                b.addZeros(bits.size() - b.bits.size());
            }
            c = b.complAt1();
            c.removeZeros();
            c++;
            s = (*this) + c;
            s.bits.pop_back();
            s.removeZeros();
            b.removeZeros();
            return s;
        }
        BigInt operator++(int i) {
            BigInt one;
            one.bits.push_back(1);
            (*this) += one;
            return *this;
        }
        BigInt operator* (BigInt &b) {
            BigInt s;
            for (unsigned int i = 0; i < b.bits.size(); i++) {
                BigInt m;
                for (unsigned int j = 0; j < bits.size(); j++) {
                    bool bit = bits[j] & b.bits[i];
                    m.bits.push_back(bit);
                }
                m >>= i;
                s += m;
            }
            return s;
        }
        BigInt operator/ (BigInt &b) {
            BigInt r, q;
            for (int i = bits.size() - 1; i >= 0; i--) {
                r.bits.push_front(bits[i]);
                bool bit = !(r < b);
                q.bits.push_front(bit);
                r = (!bit) ? r : r - b;
            }
            q.removeZeros();
            return q;
        }
        BigInt operator% (BigInt &b) {
            BigInt r, q;
            for (int i = bits.size() - 1; i >= 0; i--) {
                r.bits.push_front(bits[i]);
                bool bit = !(r < b);
                q.bits.push_front(bit);
                r = (!bit) ? r : r - b;
            }
            q.removeZeros();
            return r;
        }
        BigInt operator>>= (unsigned int shiftLeft) {
            for (unsigned int i = 0; i < shiftLeft; i++) {
                bits.push_front(0);
            }
            return (*this);
        }
        bool isNull () {
            return bits.size() == 0;
        }
        bool operator< (BigInt& b) {
            if (bits.size() < b.bits.size())
                return true;
            if (bits.size() > b.bits.size())
                return false;
            for (int i = bits.size() - 1; i >= 0; i--) {
                if (bits[i] < b.bits[i])
                    return true;
                if (bits[i] > b.bits[i])
                    return false;
            }
            return false;
        }
        void fromStr(std::string str) {
            unsigned int n = 0;
            for (unsigned int i = 0; i < str.length(); i++) {
                unsigned int digit = conversionStringInt(str.substr(i, 1)) * Math::power(10, str.length() - i - 1);
                n += digit;
            }
            int p = 0;
            while (n > Math::power(2, p))
                p++;
            while (p != -1) {
                bool bit = n / (unsigned int) Math::power(2, p);
                bits.push_front(bit);
                n -= bit * (unsigned int) Math::power(2, p);
                p--;
            }
            removeZeros();
        }
        std::string getStr() {
            std::string s;
            unsigned int n=0;
            for (unsigned int i = 0; i < bits.size(); i++) {
                unsigned int digit = bits[i] * Math::power(2, i);
                n += digit;
            }
            return conversionIntString(n);
        }
    private :
        BigInt complAt1 () {
            BigInt c;
            for (unsigned int i = 0; i < bits.size(); i++) {
                c.bits.push_back(!bits[i]);
            }
            return c;
        }
        void addZeros(unsigned int nb) {
            for (unsigned int i = 0; i < nb; i++) {
                bits.push_back(0);
            }
        }
        void removeZeros() {
            while(!bits.empty() && !bits.back())
                bits.pop_back();
        }
        bool addBits(bool a, bool b, bool& r) {
            bool s = (a ^ b ^ r);
            r = (a & b) | (a & r) | (b & r);
            return s;
        }
        friend std::ostream& operator<< (std::ostream& out, const BigInt& bi);
        std::deque<bool> bits;
};
std::ostream& operator<< ( std::ostream& out, const BigInt& bi) {
    if ( bi.bits.size()== 0 ) return ( out << 0 );
    for(int i = bi.bits.size() - 1; i >= 0; i--) {
        out<<(bi.bits[i]);
    }
    return out;
}
class TestAppli : public Application {
    public :
    TestAppli(sf::VideoMode mode, std::string title) : Application(mode, title, sf::Style::Default, ContextSettings(24, 0, 8, 3, 0)) {
    }
    void onLoad() {
        TextureManager<> tm;
        tm.fromFileWithAlias("tilesets/herbe.png", "GRASS");
        tm.fromFileWithAlias("tilesets/murs.png", "WALLS");
        cache.addResourceManager(tm, "TextureManager");
    }
    void onInit() {
        TextureManager<> &tm = cache.resourceManager<Texture, std::string>("TextureManager");
        Map* theMap = new Map(&getRenderComponentManager(), "Map test", 100, 50, 0);
        BaseChangementMatrix bm;
        bm.set2DIsoMatrix();
        theMap->setBaseChangementMatrix(bm);
        World::addEntityManager(theMap);
        World::setCurrentEntityManager("Map test");
        EntitiesUpdater* eu = new EntitiesUpdater();
        World::addWorker(eu);
        std::vector<Tile*> tiles;
        std::vector<Tile*> walls;
        tiles.push_back(new Tile(tm.getResourceByAlias("GRASS"), Vec3f(0, 0, 0), Vec3f(120, 60, 0),sf::IntRect(0, 0, 100, 50)));
        walls.push_back(new Tile(tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 0, 100, 100)));
        walls.push_back(new Tile(tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 100, 100, 100)));
        walls.push_back(new Tile(tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 200, 100, 100)));
        walls.push_back(new Tile(tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 300, 100, 100)));
        walls.push_back(new Tile(tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 400, 100, 100)));
        walls.push_back(new Tile(tm.getResourceByAlias("WALLS"), Vec3f(0, 0, 0), Vec3f(100, 100, 0), sf::IntRect(100, 500, 100, 100)));
        BoundingBox mapZone(0, 0, 0, 1500, 1000, 0);
        World::generate_map(tiles, walls, Vec2f(100, 50), mapZone, false);
        PerPixelLinkedListRenderComponent *frc = new PerPixelLinkedListRenderComponent(getRenderWindow(),2, "E_BIGTILE", ContextSettings(0, 0, 4, 3, 0));
        getRenderComponentManager().addComponent(frc);
        World::update();
    }
    void onRender(RenderComponentManager *rcm) {
        // draw everything here...
        World::drawOnComponents("E_BIGTILE", 0);
    }
    void onDisplay(RenderWindow* window) {
    }
    void onUpdate (RenderWindow* window, IEvent& event) {
        // check all the window's events that were triggered since the last iteration of the loop
        if (window == &getRenderWindow() && event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED) {
            stop();
        }
    }
    void onExec() {
    }
    private :
    ResourceCache<> cache;
};
int main(int argc, char* argv[])
{
    EXPORT_CLASS_GUID(BoundingVolumeBoundingBox, BoundingVolume, BoundingBox)
    EXPORT_CLASS_GUID(EntityTile, Entity, Tile)
    EXPORT_CLASS_GUID(EntityTile, Entity, BigTile)
    EXPORT_CLASS_GUID(EntityWall, Entity, g2d::Wall)
    EXPORT_CLASS_GUID(EntityDecor, Entity, g2d::Decor)
    EXPORT_CLASS_GUID(EntityAnimation, Entity, Anim)
    EXPORT_CLASS_GUID(EntityHero, Entity, Hero)
    EXPORT_CLASS_GUID(EntityMesh, Entity, Mesh)
    MyAppli app(sf::VideoMode(800, 600), "Test odfaeg");
    return app.exec();
    sf::Window window(sf::VideoMode(800, 600), "Test instanced rendering", sf::Style::Default, sf::ContextSettings(24, 8, 8, 4, 6));
    glEnable(GL_DEPTH_TEST);
    const std::string vertexShader =
                R"(#version 330 core
                    layout (location = 0) in vec3 position;
                    layout (location = 1) in vec4 color;
                    layout (location = 2) in vec2 texCoords;
                    layout (location = 3) in mat4 worldMat;
                    /*uniform mat4 projectionMatrix;
                    uniform mat4 viewMatrix;
                    uniform mat4 textureMatrix;*/
                    out vec2 fTexCoords;
                    out vec4 frontColor;
                    void main() {
                        gl_Position = worldMat * vec4(position, 1.f);
                        //fTexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                        frontColor = color;
                    }
                )";
                const std::string fragmentShader =
                R"(#version 330 core
                uniform sampler2D texture;
                in vec4 frontColor;
                in vec2 fTexCoords;
                void main() {
                    gl_FragColor = /*texture2D (texture, fTexCoords.xy) **/ frontColor;
                })";
    sf::Shader shader;
    if (!shader.loadFromMemory(vertexShader, fragmentShader)) {
        std::cerr<<"Failed to load shaders"<<std::endl;
        return -1;
    }
    TransformMatrix tm;
    std::vector<float> matrices;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            tm.setTranslation(Vec3f((float)x / 10.0f + offset, (float)y / 10.0f + offset, 0));
            tm.update();
            std::array<float, 16> matrix = tm.getMatrix().transpose().toGlMatrix();
            for (unsigned int n = 0; n < 16; n++) {
                matrices.push_back(matrix[n]);
            }
        }
    }
    // store instance data in an array buffer
    // --------------------------------------
    unsigned int instanceVBO;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr<<"failed to initialize glew!"<<std::endl;
        return -1;
    }
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * matrices.size(), &matrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Vertex v1(sf::Vector3f(-0.05f,  0.05f, 0.f), sf::Color(255, 0, 0, 255), sf::Vector2f(0, 0));
    Vertex v2(sf::Vector3f(0.05f, -0.05f, 0.f),  sf::Color(0, 255, 0, 255), sf::Vector2f(0, 0));
    Vertex v3(sf::Vector3f(-0.05f, -0.05f, 0.f),  sf::Color(0, 0, 255, 255), sf::Vector2f(0, 0));
    Vertex v4(sf::Vector3f(-0.05f,  0.05f, 0.f),  sf::Color(255, 0, 0, 255), sf::Vector2f(0, 0));
    Vertex v5(sf::Vector3f(0.05f, -0.05f, 0.f),  sf::Color(0, 255, 0, 255), sf::Vector2f(0, 0));
    Vertex v6(sf::Vector3f(0.05f,  0.05f, 0.f),  sf::Color(0, 255, 255, 255), sf::Vector2f(0, 0));
    std::vector<Vertex> vertices;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);

    printf( "sizeof( Vertex )             = %zu\n", sizeof( Vertex ) );

    printf( "offsetof( Vertex, pos      ) = %zu\n", offsetof( Vertex, position      ) );

    printf( "offsetof( Vertex, color    ) = %zu\n", offsetof( Vertex, color    ) );

    printf( "offsetof( Vertex, texcoord ) = %zu\n", offsetof( Vertex, texCoords ) );

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
    glCheck(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(Vertex), (void*) 12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*) 16);
    // also set instance data
    for (unsigned int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3+i);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
        glVertexAttribPointer(3+i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(3+i, 1); // tell OpenGL this is an instanced vertex attribute.
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader.getNativeHandle());
        glBindVertexArray(quadVBO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
        glBindVertexArray(0);
        window.display();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    return 0;
}




