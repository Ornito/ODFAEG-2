#include "application.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace odfaeg::core;
using namespace odfaeg::math;
using namespace odfaeg::physic;
using namespace odfaeg::graphic;
using namespace odfaeg::window;
using namespace odfaeg::audio;
using namespace sorrok;
using namespace std;
class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(std::string vertexCode, std::string fragmentCode)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    glm::mat4 lookAt (Camera_Movement direction) {
        if (direction == FORWARD) {
            return glm::lookAt(Position, Position + Front, Up);
        }
        if (direction == BACKWARD) {
            return glm::lookAt(Position, Position - Front, Up);
        }
        if (direction == LEFT) {
            return glm::lookAt(Position, Position - Right, Up);
        }
        if (direction == RIGHT) {
            return glm::lookAt(Position, Position + Right, Up);
        }
        if (direction == UP) {
            return glm::lookAt(Position, Position + Up, Up);
        }
        if (direction == DOWN) {
            return glm::lookAt(Position, Position - Up, Up);
        }
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
unsigned int loadCubemap(vector<std::string> faces);
Matrix4f glmToODFAEGMatrix(glm::mat4 mat);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera camera2;
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float speed = 10.f;
float sensivity = 0.1f;
float oldX = (float)SCR_WIDTH / 2.0;
float oldY = (float)SCR_HEIGHT / 2.0;

int main(int argc, char* argv[])
{
    /*EXPORT_CLASS_GUID(BoundingVolumeBoundingBox, BoundingVolume, BoundingBox)
    EXPORT_CLASS_GUID(EntityTile, Entity, Tile)
    EXPORT_CLASS_GUID(EntityTile, Entity, BigTile)
    EXPORT_CLASS_GUID(EntityWall, Entity, g2d::Wall)
    EXPORT_CLASS_GUID(EntityDecor, Entity, g2d::Decor)
    EXPORT_CLASS_GUID(EntityAnimation, Entity, Anim)
    EXPORT_CLASS_GUID(EntityHero, Entity, Hero)
    EXPORT_CLASS_GUID(EntityMesh, Entity, Mesh)
    MyAppli app(sf::VideoMode(800, 600), "Test odfaeg");
    return app.exec();*/
// create the window
    RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, ContextSettings(24, 0, 4, 4, 6));
    View view1 (800, 600, 80, 0.1, 1000);
    view1.move(0, 50, 0);
    window.setView(view1);
    View view3 (800, 800, 90, 0, 1000);
    View view2 (800, 800, 0, 100);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);
    // load resources, initialize the OpenGL states, ...
    // build and compile shaders
    // -------------------------
    const std::string cubeMapsVS = R"(#version 460
                                    layout (location = 0) in vec3 aPos;
                                    layout (location = 1) in vec4 color;
                                    layout (location = 2) in vec2 texCoords;
                                    layout (location = 3) in vec3 aNormal;
                                    out vec3 Normal;
                                    out vec3 Position;
                                    uniform mat4 model;
                                    uniform mat4 view;
                                    uniform mat4 projection;
                                    void main()
                                    {
                                        Normal = mat3(transpose(inverse(model))) * aNormal;
                                        Position = vec3(model * vec4(aPos, 1.0));
                                        gl_Position = projection * view * model * vec4(aPos, 1.0);
                                    }
                                )";
    const std::string cubeMapsFS = R"(#version 460
                                        out vec4 FragColor;
                                        in vec3 Normal;
                                        in vec3 Position;
                                        uniform vec3 cameraPos;
                                        uniform samplerCube skybox;
                                        void main()
                                        {
                                            vec3 I = normalize(Position - cameraPos);
                                            vec3 R = reflect(I, normalize(Normal));
                                            FragColor = vec4(texture(skybox, R).rgb, 1.0);
                                        }
                                   )";
    const std::string skyboxVS = R"(#version 460
                                    layout (location = 0) in vec3 position;
                                    layout (location = 1) in vec4 color;
                                    layout (location = 2) in vec2 texCoords;
                                    layout (location = 3) in vec3 normal;
                                    out vec2 TexCoords;
                                    out vec4 frontColor;
                                    uniform mat4 projection;
                                    uniform mat4 view;
                                    uniform mat4 model;
                                    uniform mat4 textureMatrix;
                                    void main()
                                    {
                                        TexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                        frontColor = color;
                                        gl_Position = projection * view * model * vec4(position, 1.0);;
                                    }
                                )";
    const std::string skyboxFS = R"(#version 460
                                    out vec4 FragColor;
                                    in vec2 TexCoords;
                                    in vec4 frontColor;
                                    uniform sampler2D skybox;
                                    void main()
                                    {
                                        FragColor = frontColor * texture2D(skybox, TexCoords);
                                    }
                                    )";
    const std::string perPixelLinkedListP2VS = R"(#version 460 core
                                                        layout (location = 0) in vec3 position;
                                                        uniform mat4 projection;
                                                        uniform mat4 view;
                                                        uniform mat4 model;
                                                        void main () {
                                                            gl_Position = projection * view * model * vec4(position, 1.f);
                                                        })";
    const std::string perPixelLinkedListP1FS = R"(#version 460 core
                                                      struct NodeType {
                                                          vec4 color;
                                                          float depth;
                                                          uint next;
                                                      };
                                                      layout(binding = 0, offset = 0) uniform atomic_uint nextNodeCounter;
                                                      layout(binding = 0, r32ui) uniform uimage2D headPointers;
                                                      layout(binding = 0, std430) buffer linkedLists {
                                                          NodeType nodes[];
                                                      };
                                                      uniform uint maxNodes;
                                                      uniform sampler2D skybox;
                                                      in vec2 TexCoords;
                                                      in vec4 frontColor;
                                                      layout (location = 0) out vec4 fcolor;
                                                      void main() {
                                                           uint nodeIdx = atomicCounterIncrement(nextNodeCounter);
                                                           vec4 color = frontColor * texture2D(skybox, TexCoords);
                                                           if (nodeIdx < maxNodes) {
                                                                uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);
                                                                nodes[nodeIdx].color = color;
                                                                nodes[nodeIdx].depth = gl_FragCoord.z;
                                                                nodes[nodeIdx].next = prevHead;
                                                           }
                                                           fcolor = vec4(0, 0, 0, 0);
                                                      })";
    const std::string perPixelLinkedListP2FS =
                   R"(
                   #version 460
                   #define MAX_FRAGMENTS 5
                   struct NodeType {
                      vec4 color;
                      float depth;
                      uint next;
                   };
                   layout(binding = 0, r32ui) uniform uimage2D headPointers;
                   layout(binding = 0, std430) buffer linkedLists {
                       NodeType nodes[];
                   };
                   layout(location = 0) out vec4 fcolor;
                   void main() {
                      NodeType frags[MAX_FRAGMENTS];
                      int count = 0;
                      uint n = imageLoad(headPointers, ivec2(gl_FragCoord.xy)).r;
                      while( n != 0xffffffffu && count < MAX_FRAGMENTS) {
                           frags[count] = nodes[n];
                           n = frags[count].next;
                           count++;
                      }
                      //merge sort
                      int i, j1, j2, k;
                      int a, b, c;
                      int step = 1;
                      NodeType leftArray[MAX_FRAGMENTS/2]; //for merge sort
                      while (step <= count)
                      {
                          i = 0;
                          while (i < count - step)
                          {
                              ////////////////////////////////////////////////////////////////////////
                              //merge(step, i, i + step, min(i + step + step, count));
                              a = i;
                              b = i + step;
                              c = (i + step + step) >= count ? count : (i + step + step);
                              for (k = 0; k < step; k++)
                                  leftArray[k] = frags[a + k];
                              j1 = 0;
                              j2 = 0;
                              for (k = a; k < c; k++)
                              {
                                  if (b + j1 >= c || (j2 < step && leftArray[j2].depth > frags[b + j1].depth))
                                      frags[k] = leftArray[j2++];
                                  else
                                      frags[k] = frags[b + j1++];
                              }
                              ////////////////////////////////////////////////////////////////////////
                              i += 2 * step;
                          }
                          step *= 2;
                      }
                      vec4 color = vec4(0, 0, 0, 0);
                      for( int i = count-1; i >= 0; i--)
                      {
                        color.rgb = frags[i].color.rgb * frags[i].color.a + color.rgb * (1 - frags[i].color.a);
                        color.a = frags[i].color.a + color.a * (1 - frags[i].color.a);
                      }
                      fcolor = color;
                   })";
    odfaeg::graphic::Shader shader, skyboxShader, perPixelLinkedListP1Shader, perPixelLinkedListP2Shader;
    shader.loadFromMemory(cubeMapsVS, cubeMapsFS);
    skyboxShader.loadFromMemory(skyboxVS, skyboxFS);
    perPixelLinkedListP1Shader.loadFromMemory(skyboxVS, perPixelLinkedListP1FS);
    perPixelLinkedListP2Shader.loadFromMemory(perPixelLinkedListP2VS, perPixelLinkedListP2FS);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    g3d::Cube skybox (Vec3f(-400, -1000, -300), 800, 2000, 600, sf::Color::White);
    /*skybox.move(Vec3f(0.f, 0.f, 50));
    skybox.rotate(45, Vec3f(0, 0, 1));
    skybox.setOrigin(Vec3f(0, 0, 0));
    skybox.scale(Vec3f(10, 10, 10));*/
    g3d::Cube cubeMap (Vec3f(-50, 10, -50), 100, 100, 100, sf::Color::White);
    RectangleShape fullScreenQuad(Vec3f(800, 600, 300));
    fullScreenQuad.move(Vec3f(-400, -400, 0));
    // load textures
    // -------------
    vector<std::string> faces
    {
        "tilesets/skybox/right.jpg",
        "tilesets/skybox/left.jpg",
        "tilesets/skybox/top.jpg",
        "tilesets/skybox/bottom.jpg",
        "tilesets/skybox/front.jpg",
        "tilesets/skybox/back.jpg"
    };
    Texture texs[6];
    for (unsigned int i = 0; i < 6; i++) {
        texs[i].loadFromFile(faces[i]);
        sf::IntRect texRect (0, 0, texs[i].getSize().x, texs[i].getSize().y);
        skybox.getFace(i)->getMaterial().clearTextures();
        skybox.getFace(i)->getMaterial().addTexture(&texs[i], sf::IntRect(0, 0, texs[i].getSize().x, texs[i].getSize().y));
        skybox.setTexCoords(i, texRect);
    }
    Matrix4f texMatrix = skybox.getFace(0)->getMaterial().getTexture()->getTextureMatrix();
    skyboxShader.setParameter("textureMatrix", texMatrix);
    perPixelLinkedListP1Shader.setParameter("textureMatrix", texMatrix);
    RenderTexture rtCubeMap;
    rtCubeMap.create(800, 800, ContextSettings(0, 0, 4, 4, 6), GL_TEXTURE_CUBE_MAP);
    for (unsigned int i = 0; i < 6; i++) {
        cubeMap.getFace(i)->getMaterial().clearTextures();
        cubeMap.getFace(i)->getMaterial().addTexture(&rtCubeMap.getTexture(), sf::IntRect(0, 0, 800, 800));
    }
    GLuint atomicBuffer, linkedListBuffer, headPtrTex, clearBuf;
    GLuint maxNodes = 5 * SCR_WIDTH * SCR_WIDTH;
    GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint);
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
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, SCR_WIDTH, SCR_WIDTH);
    glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::vector<GLuint> headPtrClearBuf(SCR_WIDTH*SCR_WIDTH, 0xffffffff);
    glGenBuffers(1, &clearBuf);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
    &headPtrClearBuf[0], GL_STATIC_COPY);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, linkedListBuffer);
   // shader configuration
    // --------------------
    shader.setParameter("skybox", rtCubeMap.getTexture());
    skyboxShader.setParameter("skybox", odfaeg::graphic::Shader::CurrentTexture);
    perPixelLinkedListP1Shader.setParameter("maxNodes", maxNodes);
    perPixelLinkedListP1Shader.setParameter("skybox", odfaeg::graphic::Shader::CurrentTexture);
    sf::Clock time;
    // run the main loop
    bool running = true;
    while (running)
    {
         // per-frame time logic
        // --------------------
        // per-frame time logic
        // --------------------
        float currentFrame = time.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // handle events
        IEvent event;
        while (window.pollEvent(event))
        {
            if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED)
            {
                // end the program
                running = false;
            }
            else if (event.type == IEvent::MOUSE_MOTION_EVENT) {
                int relX = (event.mouseMotion.x - oldX) * sensivity;
                int relY = (event.mouseMotion.y - oldY) * sensivity;
                //Rotate the view, (Polar coordinates) but you can also use the lookAt function to look at a point.
                int teta = view1.getTeta() - relX;
                int phi = view1.getPhi() - relY;
                view1.rotate(teta, phi);
                oldX = event.mouseMotion.x;
                oldY = event.mouseMotion.y;
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                //camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
            }
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Up)) {
            view1.move(view1.getForward(), -speed * deltaTime);
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Down)) {
            view1.move(view1.getForward(), speed * deltaTime);
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Right)) {
            view1.move(view1.getLeft(), speed * deltaTime);
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Left)) {
            view1.move(view1.getLeft(), -speed * deltaTime);
        }
        // clear the buffers
        // render
        // ------
        /*rtCubeMap.clear();
        for (unsigned int i = 0; i < 6; i++) {
            GLuint zero = 0;
            glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer);
            glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);
            glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
            glBindTexture(GL_TEXTURE_2D, headPtrTex);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_WIDTH, GL_RED_INTEGER,
                GL_UNSIGNED_INT, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            rtCubeMap.resetGLStates();
            rtCubeMap.selectCubemapFace(i);
            if (i == 0) {
                view3.lookAt(1, 0, 0);
            }
            if (i == 1) {
                view3.lookAt(-1, 0, 0);
            }
            if (i == 2) {
                view3.lookAt(0, 0, 1);
            }
            if (i == 3) {
                view3.lookAt(0, 0, -1);
            }
            if (i == 4) {
                view3.lookAt(0, 1, 0);
            }
            if (i == 5) {
                view3.lookAt(0, -1, 0);
            }
            RenderStates states;
            states.shader = &perPixelLinkedListP1Shader;
            Matrix4f viewMatrix = view3.getViewMatrix().getMatrix().transpose();
            Matrix4f projectionMatrix = view3.getProjMatrix().getMatrix().transpose();
            Matrix4f modelMatrix = skybox.getTransform().getMatrix().transpose();
            perPixelLinkedListP1Shader.setParameter("model", modelMatrix);
            perPixelLinkedListP1Shader.setParameter("view", viewMatrix);
            perPixelLinkedListP1Shader.setParameter("projection", projectionMatrix);
            // skybox cube
            rtCubeMap.setView(view3);
            rtCubeMap.draw(skybox, states);
            glFinish();
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            states.shader = &perPixelLinkedListP2Shader;
            rtCubeMap.setView(view2);
            modelMatrix = fullScreenQuad.getTransform().getMatrix().transpose();
            viewMatrix = view2.getViewMatrix().getMatrix().transpose();
            projectionMatrix = view2.getProjMatrix().getMatrix().transpose();
            perPixelLinkedListP2Shader.setParameter("model", modelMatrix);
            perPixelLinkedListP2Shader.setParameter("view", viewMatrix);
            perPixelLinkedListP2Shader.setParameter("projection", projectionMatrix);
            rtCubeMap.draw(fullScreenQuad, states);
            glFinish();
        }
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        rtCubeMap.display();*/
        window.clear();
        window.setView(view1);
        // draw...
        RenderStates states;
        states.shader = &shader;
        Matrix4f modelMatrix = cubeMap.getTransform().getMatrix().transpose();
        Matrix4f viewMatrix = view1.getViewMatrix().getMatrix().transpose();
        Matrix4f projectionMatrix = view1.getProjMatrix().getMatrix().transpose();
        /*shader.setParameter("model", modelMatrix);
        shader.setParameter("view", viewMatrix);
        shader.setParameter("projection", projectionMatrix);
        shader.setParameter("cameraPos", camera.Position.x,camera.Position.y,camera.Position.z);
        // cubes
        window.setEnableCubeMap(true);
        window.draw(cubeMap, states);
        window.setEnableCubeMap(false);*/
        // draw skybox as last
        states.shader = &skyboxShader;
        viewMatrix = Matrix4f(Matrix3f(view1.getViewMatrix().getMatrix())).transpose();
        modelMatrix = skybox.getTransform().getMatrix().transpose();
        skyboxShader.setParameter("model", modelMatrix);
        skyboxShader.setParameter("view", viewMatrix);
        skyboxShader.setParameter("projection", projectionMatrix);
        // skybox cube

        for (unsigned int i = 0; i < skybox.getNbFaces(); i++) {
            states.texture = skybox.getFace(i)->getMaterial().getTexture();
            VertexBuffer vb;
            vb.setPrimitiveType(skybox.getFace(i)->getVertexArray().getPrimitiveType());
            for (unsigned int j = 0; j < skybox.getFace(i)->getVertexArray().getVertexCount(); j++) {
                vb.append(skybox.getFace(i)->getVertexArray()[j]);
            }
            vb.update();
            window.drawVertexBuffer(vb, states);
        }
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // release resources...

    return 0;
}
Matrix4f glmToODFAEGMatrix(glm::mat4 mat) {
    Matrix4f odfaegMatrix(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                         mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                         mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                         mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    return odfaegMatrix;
}



