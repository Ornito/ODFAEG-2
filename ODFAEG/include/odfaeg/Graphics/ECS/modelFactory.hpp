#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP
#include "components.hpp"
#include "../rectangleShape.h"
#include <GLFW/glfw3.h>
#include <set>
namespace odfaeg {
    namespace graphic {
        namespace ecs {
        class ModelFactory {
            struct uint64_to_uint128 {
                uint64_t handle;
                uint64_t padding;
            };
            struct Samplers {
                std::vector<uint64_to_uint128> texs;
            };
            #ifndef VULKAN
            template <typename World, typename DynamicArray, typename Factory>
            static auto createPerPixelLinkedListBindlessRenderComponent(World& world, Factory factory, EntityId rendererId, DynamicArray& da, RenderWindow& window, int layer, std::string expression) {
                PerPixelLinkedListBindlessPass1RenderComponent* perPixelLinkedListBindlessPass1 = new PerPixelLinkedListBindlessPass1RenderComponent(window);
                perPixelLinkedListBindlessPass1->expression = expression;
                GLuint maxNodes = 20 * window.getView().getSize().x * window.getView().getSize().y;
                GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint);
                sf::Vector3i resolution = sf::Vector3i((int) window.getSize().x, (int) window.getSize().y, window.getView().getSize().z);
                GLuint atomicBuffer, linkedListBuffer, headPtrTex, clearBuf;
                glCheck(glGenBuffers(1, &atomicBuffer));
                glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer));
                glCheck(glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW));
                glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));
                glCheck(glGenBuffers(1, &linkedListBuffer));
                glCheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, linkedListBuffer));
                glCheck(glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, nullptr, GL_DYNAMIC_DRAW));
                glCheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
                glCheck(glGenTextures(1, &headPtrTex));
                glCheck(glBindTexture(GL_TEXTURE_2D, headPtrTex));
                glCheck(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, window.getView().getSize().x, window.getView().getSize().y));
                glCheck(glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI));
                glCheck(glBindTexture(GL_TEXTURE_2D, 0));
                std::vector<GLuint> headPtrClearBuf(window.getView().getSize().x*window.getView().getSize().y, 0xffffffff);
                glCheck(glGenBuffers(1, &clearBuf));
                glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf));
                glCheck(glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
                &headPtrClearBuf[0], GL_STATIC_COPY));
                glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
                perPixelLinkedListBindlessPass1->atomicBuffer = atomicBuffer;
                perPixelLinkedListBindlessPass1->linkedListBuffer = linkedListBuffer;
                perPixelLinkedListBindlessPass1->headPtrTex = headPtrTex;
                perPixelLinkedListBindlessPass1->clearBuf = clearBuf;
                perPixelLinkedListBindlessPass1->layer = layer;
                const std::string  vertexShader = R"(#version 460
                                                        layout (location = 0) in vec3 position;
                                                        layout (location = 1) in vec4 color;
                                                        layout (location = 2) in vec2 texCoords;
                                                        layout (location = 3) in vec3 normals;
                                                        layout (location = 4) in uint textureIndex;
                                                        uniform mat4 textureMatrix[)"+core::conversionUIntString(Texture::getAllTextures().size())+R"(];
                                                        uniform mat4 projectionMatrix;
                                                        uniform mat4 viewMatrix;
                                                        uniform vec3 resolution;
                                                        out vec2 fTexCoords;
                                                        out vec4 frontColor;
                                                        out uint texIndex;
                                                        void main () {
                                                            gl_Position = projectionMatrix * viewMatrix * vec4(position.xyz, 1.f);
                                                            fTexCoords = (textureIndex != 0) ? (textureMatrix[textureIndex-1] * vec4(texCoords, 1.f, 1.f)).xy : texCoords;
                                                            frontColor = color;
                                                            texIndex = textureIndex;
                                                        })";
                const std::string fragmentShader = R"(#version 460
                                                  #extension GL_ARB_bindless_texture : enable
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
                                                  layout(std140, binding = 0) uniform ALL_TEXTURES {
                                                      sampler2D textures[)"+core::conversionUIntString(Texture::getAllTextures().size())+R"(];
                                                  };
                                                  uniform uint maxNodes;
                                                  uniform float haveTexture;
                                                  uniform sampler2D currentTex;
                                                  in vec4 frontColor;
                                                  in vec2 fTexCoords;
                                                  in flat uint texIndex;
                                                  layout (location = 0) out vec4 fcolor;
                                                  void main() {
                                                       uint nodeIdx = atomicCounterIncrement(nextNodeCounter);
                                                       vec4 color = (texIndex != 0) ? frontColor * texture(textures[texIndex-1], fTexCoords.xy) : frontColor;
                                                       if (nodeIdx < maxNodes) {
                                                            uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);
                                                            nodes[nodeIdx].color = color;
                                                            nodes[nodeIdx].depth = gl_FragCoord.z;
                                                            nodes[nodeIdx].next = prevHead;
                                                       }
                                                       fcolor = vec4(0, 0, 0, 0);
                                                  })";
                 Shader& perPixelLinkedList = perPixelLinkedListBindlessPass1->perPixelLinkedListShader;
                 if (!perPixelLinkedList.loadFromMemory(vertexShader, fragmentShader)) {
                     throw core::Erreur(54, "Failed to load per pixel linked list shader");
                 }
                 std::vector<Texture*> allTextures = Texture::getAllTextures();
                 Samplers allSamplers{};
                 std::vector<math::Matrix4f> textureMatrices;
                 for (unsigned int i = 0; i < allTextures.size(); i++) {
                     textureMatrices.push_back(allTextures[i]->getTextureMatrix());
                     GLuint64 handle_texture = allTextures[i]->getTextureHandle();
                     allTextures[i]->makeTextureResident(handle_texture);
                     uint64_to_uint128 texHandle;
                     texHandle.handle = handle_texture;
                     allSamplers.texs.push_back(texHandle);
                 }

                 perPixelLinkedList.setParameter("textureMatrix", textureMatrices);
                 perPixelLinkedList.setParameter("maxNodes", maxNodes);
                 GLuint ubo;
                 glCheck(glGenBuffers(1, &ubo));
                 unsigned int ubid;
                 glCheck(ubid = glGetUniformBlockIndex(perPixelLinkedList.getHandle(), "ALL_TEXTURES"));
                 glCheck(glUniformBlockBinding(perPixelLinkedList.getHandle(),    ubid, 0));
                 glCheck(ubid = glGetUniformBlockIndex(perPixelLinkedList.getHandle(), "ALL_TEXTURES"));
                 glCheck(glUniformBlockBinding(perPixelLinkedList.getHandle(),    ubid, 0));
                 glCheck(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
                 glCheck(glBufferData(GL_UNIFORM_BUFFER, sizeof(Samplers),allSamplers.texs.data(), GL_STATIC_DRAW));
                 glCheck(glBindBuffer(GL_UNIFORM_BUFFER, 0));

                 glCheck(glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo));
                 glCheck(glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicBuffer));
                 glCheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, linkedListBuffer));

                 for (unsigned int i = 0; i < Batcher::nbPrimitiveTypes; i++) {
                     perPixelLinkedListBindlessPass1->vbBindlessTex[i].setPrimitiveType(static_cast<sf::PrimitiveType>(i));
                 }
                 window.setActive(false);
                 auto da2 = world.addRendererFlag(da, rendererId, perPixelLinkedListBindlessPass1, factory);
                 return createPerPixelLinkedListBindlessRenderComponentPass2(world, factory, rendererId, da2, window, layer, expression);
             }
             template <typename World, typename DynamicArray, typename Factory>
             static auto createPerPixelLinkedListBindlessRenderComponentPass2(World& world, Factory factory, EntityId rendererId, DynamicArray& da, RenderWindow& window, int layer, std::string expression) {
                 RectangleShape quad(math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, window.getSize().y * 0.5f));
                 quad.move(math::Vec3f(-window.getView().getSize().x * 0.5f, -window.getView().getSize().y * 0.5f, 0));
                 PerPixelLinkedListBindlessPass2RenderComponent* perPixelLinkedListBindlessPass2 = new PerPixelLinkedListBindlessPass2RenderComponent(window);
                 const std::string  vertexShader = R"(#version 460
                                                    layout (location = 0) in vec3 position;
                                                    layout (location = 1) in vec4 color;
                                                    layout (location = 2) in vec2 texCoords;
                                                    layout (location = 3) in vec3 normals;
                                                    uniform mat4 projectionMatrix;
                                                    uniform mat4 viewMatrix;
                                                    uniform mat4 worldMat;
                                                    void main () {
                                                        gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                    })";
                 const std::string fragmentShader =
                 R"(
                 #version 460
                 #define MAX_FRAGMENTS 20
                 struct NodeType {
                    vec4 color;
                    float depth;
                    uint next;
                 };
                 layout(binding = 0, r32ui) uniform uimage2D headPointers;
                 layout(binding = 0, std430) buffer linkedLists {
                     NodeType nodes[];
                 };
                 uniform vec4 bg_color;
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
                    //Insertion sort.
                    for (int i = 0; i < count - 1; i++) {
                      for (int j = i + 1; j > 0; j--) {
                          if (frags[j - 1].depth > frags[j].depth) {
                             NodeType tmp = frags[j - 1];
                             frags[j - 1] = frags[j];
                             frags[j] = tmp;
                          }
                       }
                    }
                    vec4 color = bg_color;
                    for( int i = 0; i < count; i++)
                    {
                      color.rgb = frags[i].color.rgb * frags[i].color.a + color.rgb * (1 - frags[i].color.a);
                      color.a = frags[i].color.a + color.a * (1 - frags[i].color.a);
                    }
                    fcolor = color;
                 })";
                 Shader& perPixelLinkedList = perPixelLinkedListBindlessPass2->perPixelLinkedListShader;
                 if (!perPixelLinkedList.loadFromMemory(vertexShader, fragmentShader)) {
                     throw core::Erreur(56, "Failed to load per pixel linked list 2 shader");
                 }
                 math::Matrix4f viewMatrix = window.getDefaultView().getViewMatrix().getMatrix().transpose();
                 math::Matrix4f projMatrix = window.getDefaultView().getProjMatrix().getMatrix().transpose();
                 perPixelLinkedList.setParameter("viewMatrix", viewMatrix);
                 perPixelLinkedList.setParameter("projectionMatrix", projMatrix);
                 math::Matrix4f matrix = quad.getTransform().getMatrix().transpose();
                 perPixelLinkedList.setParameter("worldMat", matrix);
                 perPixelLinkedList.setParameter("bg_color", 0, 0, 0, 0);
                 perPixelLinkedListBindlessPass2->fullScreenQuad = quad;
                 EntityId childId = factory.createEntity();
                 auto da3 = world.addSubRendererFlag(da, rendererId, childId, perPixelLinkedListBindlessPass2, factory);
                 return da3;
                }
                #endif
                template <typename World, typename DynamicArray, typename Factory>
                static auto createTileModel(World& world, DynamicArray& da, Factory& factory, EntityId entity, const Texture *image, math::Vec3f position, math::Vec3f size, sf::IntRect subRect,  sf::Color color = sf::Color::White, Entity* parent = nullptr) {
                    TransformComponent tc;
                    TransformMatrix tm;
                    tc.localBounds = physic::BoundingBox(0, 0, 0, size.x, size.y, size.z);
                    tc.position = position;
                    tc.origin = size * 0.5f;
                    tc.center = position + tc.origin;
                    tm.setOrigin(tc.origin);
                    tm.setRotation(math::Vec3f::zAxis, 0);
                    tm.setTranslation(tc.center);
                    math::Vec3f scale(1.f, 1.f, 1.f);
                    tm.setScale(scale);
                    tc.globalBounds = tc.localBounds.transform(tm);
                    tc.transformMatrix = tm;
                    auto da2 = world.addEntityComponentFlag(da, entity, tc, factory);
                    MeshComponent mesh;
                    VertexArray va(sf::Quads, 4);
                    Vertex v1(sf::Vector3f(0, 0, 0), color);
                    Vertex v2(sf::Vector3f(size.x, 0, 0), color);
                    Vertex v3(sf::Vector3f(size.x, size.y, 0), color);
                    Vertex v4(sf::Vector3f(0, size.y, 0), color);
                    v1.texCoords = sf::Vector2f(subRect.left, subRect.top);
                    v2.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top);
                    v3.texCoords = sf::Vector2f(subRect.left + subRect.width, subRect.top + subRect.height);
                    v4.texCoords = sf::Vector2f(subRect.left, subRect.top + subRect.height);
                    //std::cout<<"tex coords : "<<v2.texCoords.x<<" "<<v2.texCoords.y<<std::endl;
                    //v1.color = v2.color = v3.color = v4.color = color;


                    va[0] = v1;
                    va[1] = v2;
                    va[2] = v3;
                    va[3] = v4;
                    Material material;
                    material.addTexture(nullptr, sf::IntRect(0, 0, 0, 0));
                    Face face(va, material, tm);
                    mesh.faces.push_back(face);
                    auto da3 = world.addEntityComponentFlag(da2, entity, mesh, factory);
                    ClonableComponent clonable;
                    auto da4 = world.addEntityComponentFlag(da3, entity, clonable, factory);
                    EntityInfoComponent entityInfo;
                    entityInfo.groupName = "E_TILE";
                    auto da5 = world.addEntityComponentFlag(da4, entity, entityInfo, factory);
                    return da5;
                }
                template <typename World, typename DynamicArray, typename Factory>
                static auto createBigTileModel(World& world, DynamicArray& da, Factory& factory, EntityId entity, math::Vec3f position) {
                    TransformMatrix tm;
                    TransformComponent tc;
                    tc.localBounds = physic::BoundingBox(0, 0, 0, 0, 0, 0);
                    tc.position = position;
                    tc.origin = tc.size * 0.5f;
                    tc.center = position + tc.origin;
                    tm.setOrigin(tc.origin);
                    tm.setRotation(math::Vec3f::zAxis, 0);
                    tm.setTranslation(tc.center);
                    math::Vec3f scale(1.f, 1.f, 1.f);
                    tm.setScale(scale);
                    tc.globalBounds = tc.localBounds.transform(tm);
                    tc.transformMatrix = tm;
                    auto da1 = world.addEntityComponentFlag(da, entity, tc, factory);
                    return da1;
                }
                template <typename World, typename DynamicArray, typename Factory>
                static auto createGridSceneModel(World& world, DynamicArray& da, Factory& factory, EntityId sceneId, std::string name, unsigned int gridWidth, unsigned int gridHeight, unsigned int gridDepth) {
                    Grid grid(gridWidth, gridHeight, gridDepth);
                    SceneGridComponent sceneGrid;
                    sceneGrid.partitions = grid;
                    auto da2 = world.addSceneFlag(da, sceneId, sceneGrid, factory);
                    return da2;
                }
            };

        }
    }
}

#endif
