#include "application.h"
#include <SFML/OpenGL.hpp>
#include "odfaeg/Window/window.hpp"
#include "odfaeg/Window/context.hpp"
#include <SFML/Window/WindowStyle.hpp>
#include "odfaeg/Graphics/sprite.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stddef.h>
#include <vector>
using namespace odfaeg::core;
using namespace odfaeg::math;
using namespace odfaeg::physic;
using namespace odfaeg::graphic;
using namespace odfaeg::window;
using namespace odfaeg::audio;
using namespace sorrok;
using namespace std;
unsigned int loadCubemap(vector<std::string> faces);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float speed = 10.f;
float sensivity = 0.1f;
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
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32, 0, 4, 3, 3));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);
    View view(800, 600, 80, 1, 1000);
    view.setCenter(Vec3f(0, 0, 3));
    view.setPerspective(-1, 1, -1, 1, 80, SCR_WIDTH / SCR_HEIGHT, 1, 1000);
    // load resources, initialize the OpenGL states, ...
    glEnable(GL_DEPTH_TEST);
    const std::string cubeMapsVS = R"(#version 330 core
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
    Shader shader;
    shader.loadFromMemory(cubeMapsVS, cubeMapsFS);
    Shader skyboxShader;
    skyboxShader.loadFromMemory(skyboxVS, skyboxFS);
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
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
    unsigned int cubemapTexture = loadCubemap(faces);
    glEnable(GL_TEXTURE_CUBE_MAP);
    Shader::bind(&shader);
    unsigned int index=0;
    shader.setParameter("texture1", index);
    Shader::bind(&skyboxShader);
    skyboxShader.setParameter("skybox", index);
    int oldX = sf::Mouse::getPosition(window).x;
    int oldY = sf::Mouse::getPosition(window).y;
    sf::Clock time;
    // run the main loop
    bool running = true;
    while (running)
    {
         // per-frame time logic
        // --------------------

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
            else if (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                int relX = (event.mouseMove.x - oldX) * sensivity;
                int relY = (event.mouseMove.y - oldY) * sensivity;
                int teta = view.getTeta() - relX;
                int phi = view.getPhi() - relY;
                view.rotate(teta, phi);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            view.move(view.getForward(), -speed * time.getElapsedTime().asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            view.move(view.getForward(), speed * time.getElapsedTime().asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            view.move(view.getLeft(), speed * time.getElapsedTime().asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            view.move(view.getLeft(), -speed * time.getElapsedTime().asSeconds());
        }
        // clear the buffers
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw...
        TransformMatrix model;
        Matrix4f modelMatrix = model.getMatrix().transpose();
        Matrix4f viewMatrix = view.getViewMatrix().getMatrix().transpose();
        Matrix4f projectionMatrix = view.getProjMatrix().getMatrix().transpose();
        Shader::bind(&shader);
        shader.setParameter("model", modelMatrix);
        shader.setParameter("view", viewMatrix);
        shader.setParameter("projection", projectionMatrix);
        shader.setParameter("cameraPos", view.getPosition().x, view.getPosition().y, view.getPosition().z);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        Shader::bind(&skyboxShader);
        skyboxShader.setParameter("view", viewMatrix);
        skyboxShader.setParameter("projection", projectionMatrix);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        // end the current frame (internally swaps the front and back buffers)
        window.display();
        oldX = sf::Mouse::getPosition(window).x;
        oldY = sf::Mouse::getPosition(window).y;
        time.restart();
    }

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



