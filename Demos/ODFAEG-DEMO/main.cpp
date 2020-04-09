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
class Renderer {
    public :
    Renderer() : m_defaultView(), m_view(), m_cache() {
        m_cache.glStatesSet = false;

    }
    void initialize(unsigned int width, unsigned int height) {
        m_defaultView = View (static_cast<float>(width), static_cast<float>(height), -static_cast<float>(width) - 100, static_cast<float>(height)+100);
        m_defaultView.reset(BoundingBox(0, 0, -static_cast<float>(height) - 100,static_cast<float>(width), static_cast<float>(height),static_cast<float>(height)+100));
        m_view = m_defaultView;
        m_cache.glStatesSet = false;
        m_cache.viewChanged = true;
    }
    void applyCurrentView() {
        BoundingBox viewport = getView().getViewport();
        glViewport(viewport.getPosition().x, viewport.getPosition().y, viewport.getWidth(), viewport.getHeight());
        // Set the projection matrix
        glMatrixMode(GL_PROJECTION);
        //float* projMatrix = getView().getProjMatrix().getGlMatrix();
        glLoadMatrixf(getView().getProjMatrix().getMatrix().transpose().toGlMatrix().data());
        //delete projMatrix;
        //float* viewMatrix = getView().getViewMatrix().getGlMatrix();
        glMultMatrixf(getView().getViewMatrix().getMatrix().transpose().toGlMatrix().data());
        //delete viewMatrix;

        // Go back to model-view mode
        getView().updated();
        glCheck(glMatrixMode(GL_MODELVIEW));
    }
    void setView(View view) {
        m_view = view;
        m_cache.viewChanged = true;
    }
    View& getView() {
        return m_view;
    }
    sf::Uint32 factorToGlConstant(sf::BlendMode::Factor blendFactor)
    {
        switch (blendFactor)
        {
            default:
            case sf::BlendMode::Zero:             return GL_ZERO;
            case sf::BlendMode::One:              return GL_ONE;
            case sf::BlendMode::SrcColor:         return GL_SRC_COLOR;
            case sf::BlendMode::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
            case sf::BlendMode::DstColor:         return GL_DST_COLOR;
            case sf::BlendMode::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
            case sf::BlendMode::SrcAlpha:         return GL_SRC_ALPHA;
            case sf::BlendMode::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case sf::BlendMode::DstAlpha:         return GL_DST_ALPHA;
            case sf::BlendMode::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
        }
    }


    // Convert an sf::BlendMode::BlendEquation constant to the corresponding OpenGL constant.
    sf::Uint32 equationToGlConstant(sf::BlendMode::Equation blendEquation)
    {
        switch (blendEquation)
        {
            default:
            case sf::BlendMode::Add:             return GL_FUNC_ADD_EXT;
            case sf::BlendMode::Subtract:        return GL_FUNC_SUBTRACT_EXT;
        }
    }
    void applyBlendMode(const sf::BlendMode mode) {
            // Apply the blend mode, falling back to the non-separate versions if necessary

            if (glBlendFuncSeparateEXT) {
                glCheck(glBlendFuncSeparate(
                factorToGlConstant(mode.colorSrcFactor), factorToGlConstant(mode.colorDstFactor),
                factorToGlConstant(mode.alphaSrcFactor), factorToGlConstant(mode.alphaDstFactor)));

            } else {
                    glCheck(glBlendFunc(
                    factorToGlConstant(mode.colorSrcFactor),
                    factorToGlConstant(mode.colorDstFactor)));
            }

            if (glBlendFuncSeparateEXT)
            {
                glCheck(glBlendEquationSeparate(
                    equationToGlConstant(mode.colorEquation),
                    equationToGlConstant(mode.alphaEquation)));
            } else {
                glCheck(glBlendEquation(equationToGlConstant(mode.colorEquation)));
            }

            m_cache.lastBlendMode = mode;
    }
    void resetGLStates() {
        if (GL_ARB_multitexture) {
            glClientActiveTexture(GL_TEXTURE0);
            glActiveTexture(GL_TEXTURE0);
        }
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.f);
        glDepthFunc(GL_GREATER);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glClearDepth(0);
        glDepthMask(GL_TRUE);
        glDisable(GL_SCISSOR_TEST);
        m_cache.glStatesSet = true;
        applyBlendMode(sf::BlendAlpha);
        TransformMatrix tm;
        applyTransform(tm);
        applyTexture(nullptr);
        glUseProgram(0);
        m_cache.useVertexCache = false;
        setView(getView());
    }
    void applyTexture(const Texture* texture)
    {
        Texture::bind(texture, Texture::Pixels);
        m_cache.lastTextureId = texture ? texture->getNativeHandle() : 0;

    }
    void applyTransform(TransformMatrix& tm) {
        glLoadIdentity();
        glMultMatrixf(tm.getMatrix().transpose().toGlMatrix().data());
    }
    void draw(const Vertex* vertices, unsigned int vertexCount, sf::PrimitiveType type, RenderStates states) {
        if (!m_cache.glStatesSet)
            resetGLStates();
        // Apply the view
        /*if (m_cache.viewChanged)
            applyCurrentView();*/

        if (states.blendMode != m_cache.lastBlendMode)
            applyBlendMode(states.blendMode);

        // Apply the texture
        sf::Uint64 textureId = states.texture ? states.texture->getNativeHandle() : 0;
        if (textureId != m_cache.lastTextureId)
            applyTexture(states.texture);
        bool useVertexCache = (vertexCount <= StatesCache::VertexCacheSize);
        if (useVertexCache)
        {

            // Pre-transform the vertices and store them into the vertex cache
            for (unsigned int i = 0; i < vertexCount; ++i)
            {

                Vertex& vertex = m_cache.vertexCache[i];
                Vec3f pos (vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
                Vec3f finalpos = states.transform.transform(pos);

                vertex.position = sf::Vector3f(finalpos.x, finalpos.y, finalpos.z);
                vertex.color = vertices[i].color;
                vertex.texCoords = vertices[i].texCoords;
            }
            // Since vertices are transformed, we must use an identity transform to render them
            states.transform.reset3D();
            applyTransform(states.transform);
        }
        else
        {
            TransformMatrix tm = states.transform;
            applyTransform(tm);
        }
        if (useVertexCache)
        {
            // ... and if we already used it previously, we don't need to set the pointers again
            if (!m_cache.useVertexCache)
                vertices = m_cache.vertexCache;
            else
                vertices = nullptr;
        }
        if (vertices) {
            const char* data = reinterpret_cast<const char*>(vertices);
            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(Vertex), data + 0 );
            glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 12);
            glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 16);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        // Find the OpenGL primitive type
        static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                           GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
        GLenum mode = modes[type];
        // Draw the primitives
        glDrawArrays(mode, 0, vertexCount);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    private :
        View m_defaultView, m_view;
        struct StatesCache
        {
            enum {VertexCacheSize = 4};
            bool      glStatesSet; ///< Are our internal GL states set yet?
            bool      viewChanged; ///< Has the current view changed since last draw?
            sf::BlendMode lastBlendMode; ///< Cached blending mode
            sf::Uint64    lastTextureId; ///< Cached texture
            bool      useVertexCache; ///< Did we previously use the vertex cache?
            Vertex vertexCache[VertexCacheSize]; ///< Pre-transformed vertices cache
            VertexBuffer* lastVboBuffer;
        };
        StatesCache m_cache;
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
    RenderWindow window(sf::VideoMode(800, 600), "test", sf::Style::Default, ContextSettings(0, 0, 4, 3, 0));
    Texture tex;
    tex.loadFromFile("tilesets/herbe.png");
    window.getView().move(400, 300, 0);
    PerPixelLinkedListRenderComponent ppll1(window, 0, "E_TILE", ContextSettings(0, 0, 4, 4, 6));
    PerPixelLinkedListRenderComponent ppll2(window, 1, "E_TILE", ContextSettings(0, 0, 4, 4, 6));
    Tile tile1 (&tex, Vec3f(0, 0, 0), Vec3f(100, 50, 0), sf::IntRect(0, 0, 100, 50));
    Tile tile2 (&tex, Vec3f(50, 25, 0), Vec3f(100, 50, 0), sf::IntRect(0, 0, 100, 50));
    std::vector<Entity*> e1 = {&tile1};
    std::vector<Entity*> e2 = {&tile2};
    ppll1.loadEntitiesOnComponent(e1);
    ppll2.loadEntitiesOnComponent(e2);
    while(window.isOpen()) {
        IEvent event;
        while (window.pollEvent(event)) {
            if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED)
                window.close();
        }
        window.clear();
        ppll1.clear();
        ppll2.clear();
        ppll1.drawNextFrame();
        ppll2.drawNextFrame();
        window.draw(ppll1);
        window.draw(ppll2);
        window.display();
    }
    return 0;
    /*sf::Window window(sf::VideoMode(800, 600), "test", sf::Style::Default, sf::ContextSettings(0, 0, 4, 3, 0));
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    }
    Vertex verticesFBO1[] = {Vertex(sf::Vector3f(-1.f, -1.f, 0.f),
                                    sf::Color(255, 0, 0, 128),
                                    sf::Vector2f(0, 0)),
                             Vertex(sf::Vector3f(-0.90f, -1.f, 0.f),
                                    sf::Color(255, 0, 0, 128),
                                    sf::Vector2f(0, 0)),
                             Vertex(sf::Vector3f(-0.90f, -0.90f, 0.f),
                                    sf::Color(255, 0, 0, 128),
                                    sf::Vector2f(0, 0)),
                             Vertex(sf::Vector3f(-1.f, -0.90f, 0.f),
                                    sf::Color(255, 0, 0, 128),
                                    sf::Vector2f(0, 0))};
    Vertex verticesFBO2[] = {Vertex(sf::Vector3f(-0.95f, -0.95f, 0),
                                    sf::Color(0, 255, 0, 128),
                                    sf::Vector2f(0, 0)),
                             Vertex(sf::Vector3f(-0.85f, -0.95f, 0.f),
                                    sf::Color(0, 255, 0, 128),
                                    sf::Vector2f(0, 0)),
                             Vertex(sf::Vector3f(-0.85f, -0.85f, 0.f),
                                    sf::Color(0, 255, 0, 128),
                                    sf::Vector2f(0, 0)),
                             Vertex(sf::Vector3f(-0.95f, -0.85f, 0.f),
                                    sf::Color(0, 255, 0, 128),
                                    sf::Vector2f(0, 0))};
    Vertex fullScreenQuad[] = {Vertex(sf::Vector3f(-1.f, -1.f, 0.f),
                                      sf::Color(255, 255, 255, 255),
                                      sf::Vector2f(0, 0)),
                               Vertex(sf::Vector3f(-1.f, 1.f, 0.f),
                                      sf::Color(255, 255, 255, 255),
                                      sf::Vector2f(800, 0)),
                               Vertex(sf::Vector3f(1.f, 1.f, 0.f),
                                      sf::Color(255, 255, 255, 255),
                                      sf::Vector2f(800, 600)),
                               Vertex(sf::Vector3f(1.f, -1.f, 0.f),
                                      sf::Color(255, 255, 255, 255),
                                      sf::Vector2f(0, 600))};

    RenderTexture rt1;
    rt1.create(800, 600, ContextSettings(0, 0, 4, 3, 0));

    RenderTexture rt2;
    rt2.create(800, 600, ContextSettings(0, 0, 4, 3, 0));
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    Renderer r1, r2, r3;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        rt1.setActive(true);
        glClearColor(1.f, 1.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        RenderStates states;
        states.blendMode = sf::BlendAlpha;
        r1.initialize(800, 600);
        r1.draw(verticesFBO1, 4, sf::Quads, states);
        rt1.display();
        rt2.setActive(true);
        glClearColor(1.f, 1.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        r2.initialize(800, 600);
        r2.draw(verticesFBO2, 4, sf::Quads, states);
        rt2.display();
        window.setActive(true);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        states.texture = &rt1.getTexture();
        View view = r3.getView();
        r3.initialize(800, 600);
        view.move(400, 300, 0);
        r3.setView(view);
        r3.draw(fullScreenQuad, 4, sf::Quads, states);
        states.texture = &rt2.getTexture();
        r3.draw(fullScreenQuad, 4, sf::Quads, states);
        window.display();
    }
    return 0;*/
}




