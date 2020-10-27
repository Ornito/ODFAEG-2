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
    RIGHT
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
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float speed = 100.f;
float sensivity = 10.f;

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
    MyAppli app(sf::VideoMode(800, 800), "Test odfaeg");
    return app.exec();
// create the window
    RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, ContextSettings(32, 0, 4, 3, 3));
    window.setEnableCubeMap(true);
    View view (800, 600, 80, 1, 1000);
    view.setCenter(Vec3f(0, 3, 0));
    window.setVerticalSyncEnabled(true);
    window.setView(view);

    // activate the window
    window.setActive(true);
    // load resources, initialize the OpenGL states, ...
    glEnable(GL_DEPTH_TEST);
    // build and compile shaders
    // -------------------------
    const std::string cubeMapsVS = R"(#version 330 core
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
    const std::string cubeMapsFS = R"(#version 330 core
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
    const std::string skyboxVS = R"(#version 330 core
                                    layout (location = 0) in vec3 aPos;
                                    layout (location = 1) in vec4 color;
                                    layout (location = 2) in vec2 texCoords;
                                    layout (location = 3) in vec3 aNormal;
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
    const std::string skyboxFS = R"(#version 330 core
                                    out vec4 FragColor;
                                    in vec3 TexCoords;
                                    uniform samplerCube skybox;
                                    void main()
                                    {
                                        FragColor = texture(skybox, TexCoords);
                                    }
                                    )";
    odfaeg::graphic::Shader shader, skyboxShader;
    shader.loadFromMemory(cubeMapsVS, cubeMapsFS);
    skyboxShader.loadFromMemory(skyboxVS, skyboxFS);
    g3d::Cube cubemap(Vec3f(-0.5f, -0.5f, -0.5f), 1, 1, 1, sf::Color::White);
    g3d::Cube skybox(Vec3f(-1.f, -1.f, -1.f), 2, 2, 2, sf::Color::White);
    Batcher cubemapBatcher, skyboxBatcher;
    cubemapBatcher.clear();
    skyboxBatcher.clear();
    for (unsigned int i = 0; i < cubemap.getNbFaces(); i++) {
        cubemapBatcher.addFace(cubemap.getFace(i));
    }
    for (unsigned int i = 0; i < skybox.getNbFaces(); i++) {
        skyboxBatcher.addFace(skybox.getFace(i));
    }
    std::vector<Instance> cubeMapInstances = cubemapBatcher.getInstances();
    std::vector<Instance> skyboxInstances = skyboxBatcher.getInstances();
    // load textures
    // -------------
    vector<std::string> faces
    {
        "tilesets/skybox/right.jpg",
        "tilesets/skybox/left.jpg",
        "tilesets/skybox/front.jpg",
        "tilesets/skybox/back.jpg",
        "tilesets/skybox/top.jpg",
        "tilesets/skybox/bottom.jpg"

    };
    std::vector<sf::Image> images;
    for (unsigned int i = 0; i < 6; i++) {
        sf::Image image;
        image.loadFromFile(faces[i]);
        images.push_back(image);
    }
    Texture cubeMapTex;
    int width = images[0].getSize().x;
    int height = images[0].getSize().y;
    cubeMapTex.createCubeMap(width, height, images);
   // shader configuration
    // --------------------
    shader.setParameter("skybox", cubeMapTex);
    skyboxShader.setParameter("skybox", cubeMapTex);
    VertexBuffer vb;
    for (unsigned int i = 0; i < cubeMapInstances.size(); i++) {
        if (cubeMapInstances[i].getAllVertices().getVertexCount() > 0) {
            vb.clear();
            vb.setPrimitiveType(cubeMapInstances[i].getAllVertices().getPrimitiveType());
            for (unsigned int j = 0; j < cubeMapInstances[i].getAllVertices().getVertexCount(); j++) {
                //std::cout<<"add vertex cube : "<<cubeMapInstances[i].getAllVertices()[j].position.x<<","<<cubeMapInstances[i].getAllVertices()[j].position.y<<","<<cubeMapInstances[i].getAllVertices()[j].position.z<<std::endl;
                vb.append(cubeMapInstances[i].getAllVertices()[j]);
            }
            vb.update();
        }
    }
    VertexBuffer vb2;
    for (unsigned int i = 0; i < skyboxInstances.size(); i++) {
        if (skyboxInstances[i].getAllVertices().getVertexCount() > 0) {
            vb2.clear();
            vb2.setPrimitiveType(skyboxInstances[i].getAllVertices().getPrimitiveType());
            for (unsigned int j = 0; j < skyboxInstances[i].getAllVertices().getVertexCount(); j++) {
                //std::cout<<"add vertex cube : "<<cubeMapInstances[i].getAllVertices()[j].position.x<<","<<cubeMapInstances[i].getAllVertices()[j].position.y<<","<<cubeMapInstances[i].getAllVertices()[j].position.z<<std::endl;
                vb2.append(skyboxInstances[i].getAllVertices()[j]);
            }
            vb2.update();
        }
    }
    int oldX = IMouse::getPosition(window).x;
    int oldY = IMouse::getPosition(window).y;
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
        // handle events
        IEvent event;
        while (window.pollEvent(event))
        {
            if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_CLOSED)
            {
                // end the program
                running = false;
            }
            else if (event.type == IEvent::WINDOW_EVENT && event.window.type == IEvent::WINDOW_EVENT_RESIZED)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
            else if (event.type == IEvent::MOUSE_MOTION_EVENT && IMouse::isButtonPressed(IMouse::Right)) {
                int relX = (event.mouseMotion.x - oldX) * sensivity;
                int relY = (event.mouseMotion.y - oldY) * sensivity;
                int teta = view.getTeta() - relX;
                int phi = view.getPhi() - relY;
                view.rotate(teta, phi);
            }
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Up)) {
            view.move(view.getForward(), -speed * time.getElapsedTime().asSeconds());
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Down)) {
            view.move(view.getForward(), speed * time.getElapsedTime().asSeconds());
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Right)) {
            view.move(view.getLeft(), speed * time.getElapsedTime().asSeconds());
        }
        if (IKeyboard::isKeyPressed(IKeyboard::Left)) {
            view.move(view.getLeft(), -speed * time.getElapsedTime().asSeconds());
        }
        // clear the buffers
        // render
        // ------
        window.clear(sf::Color(0.1f, 0.1f, 0.1f, 1.0f));
        // draw...
        TransformMatrix model;
        Matrix4f modelMatrix = model.getMatrix();
        Matrix4f viewMatrix = view.getViewMatrix().getMatrix();
        //std::cout<<"view matrix : "<<viewMatrix<<std::endl;
        Matrix4f projectionMatrix = view.getProjMatrix().getMatrix();
        shader.setParameter("model", modelMatrix);
        shader.setParameter("view", viewMatrix);
        shader.setParameter("projection", projectionMatrix);
        shader.setParameter("cameraPos", view.getPosition().x, view.getPosition().y, view.getPosition().z);
        // cubes
        RenderStates states;
        states.texture = &cubeMapTex;
        states.shader = &shader;
        window.drawVertexBuffer(vb, states);
        // draw skybox as last
        //glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        viewMatrix = Matrix4f(Matrix3f(viewMatrix));
        skyboxShader.setParameter("view", viewMatrix);
        skyboxShader.setParameter("projection", projectionMatrix);
        states.texture = &cubeMapTex;
        states.shader = &skyboxShader;
        window.drawVertexBuffer(vb2, states);


        //glDepthFunc(GL_GREATER); // set depth function back to default
        // end the current frame (internally swaps the front and back buffers)
        window.display();
        oldX = IMouse::getPosition(window).x;
        oldY = IMouse::getPosition(window).y;
        time.restart();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // release resources...

    return 0;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        sf::Image image;
        image.loadFromFile(faces[i]);
        width = image.getSize().x;
        height = image.getSize().y;
        if (image.getPixelsPtr())
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}



