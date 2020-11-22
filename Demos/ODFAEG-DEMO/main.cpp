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
// create the window
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(24, 0, 4, 4, 6));
    glewInit();
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);
    // load resources, initialize the OpenGL states, ...
    glEnable(GL_DEPTH_TEST);
    // build and compile shaders
    // -------------------------
    const std::string refractCubeVS = R"(#version 460
                                         layout (location = 0) in vec3 aPos;
                                         layout (location = 1) in vec2 aTexCoords;
                                         uniform mat4 model;
                                         uniform mat4 view;
                                         uniform mat4 projection;
                                         out vec2 texCoords;
                                         void main() {
                                             gl_Position = projection * view * model * vec4(aPos, 1.0);
                                             texCoords = aTexCoords;
                                         }
                                         )";
    const std::string refractCubeFS = R"(#version 460
                                         uniform sampler2D texture;
                                         in vec2 texCoords;
                                         out vec4 FragColor;
                                         void main() {
                                            FragColor = texture2D (texture, texCoords);
                                         })";
    const std::string cubeMapsVS = R"(#version 460
                                    layout (location = 0) in vec3 aPos;
                                    layout (location = 1) in vec3 aNormal;
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
                                            R = normalize(R);
                                            FragColor = vec4(texture(skybox, R).rgb, 1.0);
                                        }
                                   )";
    const std::string skyboxVS = R"(#version 460
                                    layout (location = 0) in vec3 aPos;
                                    out vec3 TexCoords;
                                    uniform mat4 projection;
                                    uniform mat4 view;
                                    void main()
                                    {
                                        TexCoords = aPos;
                                        vec4 pos = projection * view * vec4(aPos, 1.0);
                                        gl_Position = pos.xyww;
                                    }
                                )";
    const std::string skyboxFS = R"(#version 460
                                    out vec4 FragColor;
                                    in vec3 TexCoords;
                                    uniform samplerCube skybox;
                                    void main()
                                    {
                                        FragColor = texture(skybox, TexCoords);
                                    }
                                    )";
    odfaeg::graphic::Shader shader, skyboxShader, refractCubeShader;
    shader.loadFromMemory(cubeMapsVS, cubeMapsFS);
    skyboxShader.loadFromMemory(skyboxVS, skyboxFS);
    refractCubeShader.loadFromMemory(refractCubeVS, refractCubeFS);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    float fullScreenQuadVertices2[] = {
        // positions        //TexCoords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
    std::vector<sf::Image> images;
    for (unsigned int i = 0; i < 6; i++) {
        sf::Image image;
        image.loadFromFile(faces[i]);
        images.push_back(image);
    }
    int width = images[0].getSize().x;
    int height = images[0].getSize().y;
    odfaeg::graphic::Texture cubeMapTex, cubeMapFBOTex, refractTex;
    cubeMapTex.createCubeMap(width, height, images);
    cubeMapFBOTex.createCubeMap(SCR_WIDTH, SCR_WIDTH);
    refractTex.create(SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_TEXTURE_CUBE_MAP);
    /*sf::Context context(sf::ContextSettings(0, 0, 4, 4, 6), SCR_WIDTH, SCR_WIDTH);
    context.setActive(true);
    unsigned int skyboxVAOFBO;
    glGenVertexArrays(1, &skyboxVAOFBO);
    glBindVertexArray(skyboxVAOFBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);*/


    GLuint frameBufferID;
    glGenFramebuffers(1, &frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeMapFBOTex.getNativeHandle(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        std::cerr << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
        return false;
    }
    /*sf::Context context2(sf::ContextSettings(0, 0, 4, 4, 6), SCR_WIDTH, SCR_WIDTH);
    context2.setActive(true);*/
    unsigned int fsQuadVAOFBO, fsQuadVBOFBO;
    glGenVertexArrays(1, &fsQuadVAOFBO);
    glBindVertexArray(fsQuadVAOFBO);
    glGenBuffers(1, &fsQuadVBOFBO);
    glBindBuffer(GL_ARRAY_BUFFER, fsQuadVBOFBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fullScreenQuadVertices2), &fullScreenQuadVertices2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    GLuint frameBufferID2;
    glGenFramebuffers(1, &frameBufferID2);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID2);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractTex.getNativeHandle(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        std::cerr << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
        return false;
    }
   // shader configuration
    // --------------------
    shader.setParameter("skybox", cubeMapFBOTex);
    skyboxShader.setParameter("skybox", cubeMapTex);
    refractCubeShader.setParameter("texture", refractTex);
    int oldX = sf::Mouse::getPosition(window).x;
    int oldY = sf::Mouse::getPosition(window).y;
    sf::Clock time;
    // run the main loop
    bool running = true;
    glEnable(GL_TEXTURE_CUBE_MAP);
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
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (firstMouse)
                {
                    lastX = event.mouseMove.x;
                    lastY = event.mouseMove.y;
                    firstMouse = false;
                }

                float xoffset = event.mouseMove.x - lastX;
                float yoffset = lastY - event.mouseMove.y; // reversed since y-coordinates go from bottom to top

                lastX = event.mouseMove.x;
                lastY = event.mouseMove.y;

                camera.ProcessMouseMovement(xoffset, yoffset);
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        // clear the buffers
        // render
        // ------
        window.setActive(true);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*context2.setActive(true);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        context.setActive(true);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
        for (unsigned int i = 0; i < 6; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMapFBOTex.getNativeHandle(), 0);
            glViewport(0, 0, SCR_WIDTH, SCR_WIDTH);
            glm::mat4 view;
            if (i == 0) {
                view = camera2.lookAt(FORWARD);
            }
            if (i == 1) {
                view = camera2.lookAt(BACKWARD);
            }
            if (i == 2) {
                view = camera2.lookAt(UP);
            }
            if (i == 3) {
                view = camera2.lookAt(DOWN);
            }
            if (i == 4) {
                view = camera2.lookAt(RIGHT);
            }
            if (i == 5) {
                view = camera2.lookAt(LEFT);
            }
            glm::mat4 projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            odfaeg::graphic::Shader::bind(&skyboxShader);
            Matrix4f viewMatrix = glmToODFAEGMatrix(view);
            Matrix4f projectionMatrix = glmToODFAEGMatrix(projection);
            skyboxShader.setParameter("view", viewMatrix);
            skyboxShader.setParameter("projection", projectionMatrix);
            // skybox cube

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            odfaeg::graphic::Texture::bind(&cubeMapTex, odfaeg::graphic::Texture::Normalized);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        //window.setActive(true);
        //context2.setActive(true);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        // draw...
        odfaeg::graphic::Shader::bind(&shader);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        Matrix4f modelMatrix = glmToODFAEGMatrix(model);
        Matrix4f viewMatrix = glmToODFAEGMatrix(view);
        Matrix4f projectionMatrix = glmToODFAEGMatrix(projection);
        shader.setParameter("model", modelMatrix);
        shader.setParameter("view", viewMatrix);
        shader.setParameter("projection", projectionMatrix);
        shader.setParameter("cameraPos", camera.Position.x,camera.Position.y,camera.Position.z);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        odfaeg::graphic::Texture::bind(&cubeMapFBOTex, odfaeg::graphic::Texture::Normalized);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID2);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // refract cube.
        model = glm::mat4(1.0f);
        view = camera2.GetViewMatrix();
        projection = glm::ortho(-1, 1, -1, 1);
        modelMatrix = glmToODFAEGMatrix(model);
        viewMatrix = glmToODFAEGMatrix(view);
        projectionMatrix = glmToODFAEGMatrix(projection);
        refractCubeShader.setParameter("model", modelMatrix);
        refractCubeShader.setParameter("view", viewMatrix);
        refractCubeShader.setParameter("projection", projectionMatrix);
        odfaeg::graphic::Shader::bind(&refractCubeShader);
        glBindVertexArray(fsQuadVAOFBO);
        glActiveTexture(GL_TEXTURE0);
        odfaeg::graphic::Texture::bind(&refractTex, odfaeg::graphic::Texture::Normalized);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        //Draw skybox.
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        odfaeg::graphic::Shader::bind(&skyboxShader);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        viewMatrix = glmToODFAEGMatrix(view);
        skyboxShader.setParameter("view", viewMatrix);
        skyboxShader.setParameter("projection", projectionMatrix);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        odfaeg::graphic::Texture::bind(&cubeMapTex, odfaeg::graphic::Texture::Normalized);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        window.display();
        oldX = sf::Mouse::getPosition(window).x;
        oldY = sf::Mouse::getPosition(window).y;
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    //glDeleteVertexArrays(1, &skyboxVAOFBO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVAO);
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



