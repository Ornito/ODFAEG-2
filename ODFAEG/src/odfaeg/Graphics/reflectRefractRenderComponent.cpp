#include "../../../include/odfaeg/Graphics/reflectRefractRenderComponent.hpp"
#include "glCheck.h"
namespace odfaeg {
    namespace graphic {
        ReflectRefractRenderComponent::ReflectRefractRenderComponent (RenderWindow& window, int layer, std::string expression, window::ContextSettings settings) :
            HeavyComponent(window, math::Vec3f(window.getView().getPosition().x, window.getView().getPosition().y, layer),
                          math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0),
                          math::Vec3f(window.getView().getSize().x + window.getView().getSize().x * 0.5f, window.getView().getPosition().y + window.getView().getSize().y * 0.5f, layer)),
            view(window.getView()),
            expression(expression),
            quad(math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, window.getSize().y * 0.5f)) {
            quad.move(math::Vec3f(-window.getView().getSize().x * 0.5f, 0/*-window.getView().getSize().y * 0.5f*/, 0));
            frameBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            frameBufferSprite = Sprite(frameBuffer.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            frameBuffer.setView(view);
            sf::Vector3i resolution ((int) window.getSize().x, (int) window.getSize().y, window.getView().getSize().z);
            reflectRefractBuffer.create (window.getView().getSize().x, window.getView().getSize().y, settings);
            settings.depthBits = 24;
            depthBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            reflectRefractTextBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            GLuint maxNodes = 20 * window.getView().getSize().x * window.getView().getSize().y;
            GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint);
            glCheck(glGenBuffers(1, &atomicBuffer));
            glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer));
            glCheck(glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW));
            glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));
            glCheck(glGenBuffers(1, &linkedListBuffer));
            glCheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, linkedListBuffer));
            glCheck(glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, NULL, GL_DYNAMIC_DRAW));
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
            core::FastDelegate<bool> signal (&ReflectRefractRenderComponent::needToUpdate, this);
            core::FastDelegate<void> slot (&ReflectRefractRenderComponent::drawNextFrame, this);
            core::Command cmd(signal, slot);
            getListener().connect("UPDATE", cmd);
            if (settings.versionMajor >= 4 && settings.versionMinor >= 3) {
                glGenBuffers(1, &vboWorldMatrices);
                const std::string vertexShader = R"(#version 460 core
                                                    layout (location = 0) in vec3 position;
                                                    layout (location = 1) in vec4 color;
                                                    layout (location = 2) in vec2 texCoords;
                                                    layout (location = 3) in mat4 worldMat;
                                                    uniform mat4 projectionMatrix;
                                                    uniform mat4 viewMatrix;
                                                    uniform mat4 textureMatrix;
                                                    out vec2 fTexCoords;
                                                    out vec4 frontColor;
                                                    void main() {
                                                        gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                        fTexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                        frontColor = color;
                                                    }
                                                    )";
                const std::string  normalVertexShader = R"(#version 460 core
                                                        layout (location = 0) in vec3 position;
                                                        layout (location = 1) in vec4 color;
                                                        layout (location = 2) in vec2 texCoords;
                                                        uniform mat4 textureMatrix;
                                                        uniform mat4 projectionMatrix;
                                                        uniform mat4 viewMatrix;
                                                        out vec2 fTexCoords;
                                                        out vec4 frontColor;
                                                        void main () {
                                                            gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.f);
                                                            fTexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                            frontColor = color;
                                                        })";
                const std::string  simpleVertexShader = R"(#version 460 core
                                                        layout (location = 0) in vec3 position;
                                                        layout (location = 1) in vec4 color;
                                                        layout (location = 2) in vec2 texCoords;
                                                        uniform mat4 projectionMatrix;
                                                        uniform mat4 viewMatrix;
                                                        uniform mat4 worldMat;
                                                        void main () {
                                                            gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                        })";
                const std::string perPixReflectRefractVertexShader = R"(#version 460 core
                                                                   layout (location = 0) in vec3 position;
                                                                   layout (location = 1) in vec4 color;
                                                                   layout (location = 2) in vec2 texCoords;
                                                                   layout (location = 3) in mat4 worldMat;
                                                                   uniform mat4 projectionMatrix;
                                                                   uniform mat4 viewMatrix;
                                                                   uniform mat4 depthBiasMatrix;
                                                                   uniform mat4 textureMatrix;
                                                                   out vec4 reflectRefractCoords;
                                                                   out vec4 frontColor;
                                                                   out vec2 fTexCoords;
                                                                   void main() {
                                                                       gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                                       fTexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                                       frontColor = color;
                                                                       reflectRefractCoords = depthBiasMatrix * viewMatrix * projectionMatrix * vec4(gl_Position.xyz, 1);
                                                                   }
                                                                  )";
                const std::string perPixReflectRefractVertexNormalShader = R"(#version 460 core
                                                                   layout (location = 0) in vec3 position;
                                                                   layout (location = 1) in vec4 color;
                                                                   layout (location = 2) in vec2 texCoords;
                                                                   uniform mat4 projectionMatrix;
                                                                   uniform mat4 viewMatrix;
                                                                   uniform mat4 depthBiasMatrix;
                                                                   uniform mat4 textureMatrix;
                                                                   out vec4 reflectRefractCoords;
                                                                   out vec4 frontColor;
                                                                   out vec2 fTexCoords;
                                                                   void main() {
                                                                       gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.f);
                                                                       fTexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                                       frontColor = color;
                                                                       reflectRefractCoords = depthBiasMatrix * viewMatrix * projectionMatrix * vec4(gl_Position.xyz, 1);
                                                                   }
                                                                  )";
                const std::string buildDepthBufferFragmentShader = R"(#version 460 core
                                                                          in vec4 frontColor;
                                                                          in vec2 fTexCoords;
                                                                          uniform sampler2D texture;
                                                                          uniform float haveTexture;
                                                                          layout (location = 0) out vec4 fColor;
                                                                          void main () {
                                                                              vec4 texel = texture2D(texture, fTexCoords);
                                                                              vec4 colors[2];
                                                                              colors[1] = texel * frontColor;
                                                                              colors[0] = frontColor;
                                                                              bool b = (haveTexture > 0.9);
                                                                              float current_alpha = colors[int(b)].a;
                                                                              float z = gl_FragCoord.z;
                                                                              fColor = vec4(0, 0, z, current_alpha);
                                                                          }
                                                                        )";
                const std::string buildReflectRefractFactorFragmentShader = R"(#version 460 core
                                                                               in vec4 frontColor;
                                                                               in vec2 fTexCoords;
                                                                               uniform sampler2D texture;
                                                                               uniform float haveTexture;
                                                                               uniform float reflectFactor;
                                                                               uniform float refractFactor;
                                                                               uniform float maxRefractionFactor;
                                                                               layout (location = 0) out vec4 fColor;
                                                                               void main() {
                                                                                   vec4 texel = texture2D (texture, fTexCoords);
                                                                                   vec4 colors[2];
                                                                                   colors[1] = texel * frontColor;
                                                                                   colors[0] = frontColor;
                                                                                   bool b = (haveTexture > 0.9);
                                                                                   float current_alpha = colors[int(b)].a;
                                                                                   fColor = vec4 (reflectFactor, refractFactor / maxRefractionFactor, 0, current_alpha);
                                                                               }
                                                                               )";
                const std::string fragmentShader = R"(#version 460 core
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
                                                      uniform float haveTexture;
                                                      uniform sampler2D texture;
                                                      in vec4 frontColor;
                                                      in vec2 fTexCoords;
                                                      layout (location = 0) out vec4 fcolor;
                                                      void main() {
                                                           uint nodeIdx = atomicCounterIncrement(nextNodeCounter);
                                                           vec4 texel = texture2D(texture, fTexCoords.xy);
                                                           vec4 color = (haveTexture > 0.9) ? frontColor * texel : frontColor;
                                                           if (nodeIdx < maxNodes) {
                                                                uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);
                                                                nodes[nodeIdx].color = color;
                                                                nodes[nodeIdx].depth = gl_FragCoord.z;
                                                                nodes[nodeIdx].next = prevHead;
                                                           }
                                                           fcolor = vec4(0, 0, 0, 0);
                                                      })";
                 const std::string fragmentShader2 =
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
                      for( int i = count - 1; i >= 0; i--)
                      {
                        color.rgb = frags[i].color.rgb * frags[i].color.a + color.rgb * (1 - frags[i].color.a);
                        color.a = frags[i].color.a + color.a * (1 - frags[i].color.a);
                      }
                      fcolor = color;
                   })";
                const std::string buildFramebufferShader = R"(#version 460 core
                                                                in vec4 reflectRefractCoords;
                                                                in vec4 frontColor;
                                                                in vec2 fTexCoords;
                                                                uniform float haveTexture;
                                                                uniform float maxRefraction;
                                                                uniform vec3 resolution;
                                                                uniform sampler2D texture;
                                                                uniform sampler2D depthBuffer;
                                                                uniform sampler2D reflectRefractFactorTexture;
                                                                uniform sampler2D reflectRefractTexture;
                                                                layout (location = 0) out vec4 fColor;
                                                                void main () {
                                                                    vec2 position = (gl_FragCoord.xy / resolution.xy);
                                                                    vec4 depth = texture2D(depthBuffer, position);
                                                                    vec4 reflectRefractInd = texture2D (reflectRefractFactorTexture, position);
                                                                    float offsetX = reflectRefractInd.y * maxRefraction;
                                                                    ivec2 offs = ivec2 (offsetX, 0);
                                                                    vec4 colorToReflect = textureOffset (reflectRefractTexture, position, offs);
                                                                    vec4 texel = texture2D(texture, fTexCoords);
                                                                    vec4 colors[2];
                                                                    colors[1] = texel * frontColor;
                                                                    colors[0] = frontColor;
                                                                    bool b = (haveTexture > 0.9);
                                                                    vec4 color = colors[int(b)];
                                                                    if (depth.z >= gl_FragCoord.z) {
                                                                        fColor = vec4 (color.rgb,1-depth.a) * (1 - reflectRefractInd.x) + vec4(colorToReflect.rgb,1-depth.a) * reflectRefractInd.x;
                                                                    } else {
                                                                        fColor = color * (1 - reflectRefractInd.x) + colorToReflect * reflectRefractInd.x;
                                                                    }
                                                                }
                                                              )";
                if (!sBuildDepthBuffer.loadFromMemory(vertexShader, buildDepthBufferFragmentShader)) {
                    throw core::Erreur(50, "Error, failed to load build depth buffer shader", 3);
                }
                if (!sBuildDepthBufferNormal.loadFromMemory(normalVertexShader, buildDepthBufferFragmentShader)) {
                    throw core::Erreur(51, "Error, failed to load build normal depth buffer shader", 3);
                }
                if (!sBuildReflectRefract.loadFromMemory(vertexShader, buildReflectRefractFactorFragmentShader)) {
                    throw core::Erreur(52, "Error, failed to load build reflect refract shader", 3);
                }
                if (!sBuildReflectRefractNormal.loadFromMemory(normalVertexShader, buildReflectRefractFactorFragmentShader)) {
                    throw core::Erreur(53, "Error, failed to load build reflect refract normal shader", 3);
                }
                if (!sLinkedList.loadFromMemory(vertexShader, fragmentShader)) {
                    throw core::Erreur(54, "Error, failed to load per pixel linked list shader", 3);
                }
                if (!sLinkedList2.loadFromMemory(normalVertexShader, fragmentShader)) {
                    throw core::Erreur(55, "Error, failed to load per pixel linked list normal shader", 3);
                }
                if (!sLinkedListP2.loadFromMemory(simpleVertexShader, fragmentShader2)) {
                    throw core::Erreur(56, "Error, failed to load per pixel linked list part 2 shader", 3);
                }
                if (!sReflectRefract.loadFromMemory(perPixReflectRefractVertexShader, buildFramebufferShader)) {
                    throw core::Erreur(57, "Error, failed to load reflect refract shader", 3);
                }
                if (!sReflectRefractNormal.loadFromMemory(perPixReflectRefractVertexNormalShader, buildFramebufferShader)) {
                    throw core::Erreur(58, "Error, failed to load reflect refract normal shader", 3);
                }
                sBuildDepthBuffer.setParameter("texture", Shader::CurrentTexture);
                sBuildDepthBufferNormal.setParameter("texture", Shader::CurrentTexture);
                sBuildReflectRefract.setParameter("texture", Shader::CurrentTexture);
                sBuildReflectRefract.setParameter("maxRefraction", Material::getMaxRefraction());
                sBuildDepthBufferNormal.setParameter("texture", Shader::CurrentTexture);
                sLinkedList.setParameter("maxNodes", maxNodes);
                sLinkedList.setParameter("texture", Shader::CurrentTexture);
                sLinkedList2.setParameter("maxNodes", maxNodes);
                sLinkedList2.setParameter("texture", Shader::CurrentTexture);
                sReflectRefract.setParameter("maxRefraction", Material::getMaxRefraction());
                sReflectRefract.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                sReflectRefract.setParameter("texture", Shader::CurrentTexture);
                sReflectRefract.setParameter("depthBuffer",depthBuffer.getTexture());
                sReflectRefract.setParameter("reflectRefractFactorTexture", reflectRefractBuffer.getTexture());
                sReflectRefract.setParameter("reflectRefractTexture", reflectRefractTextBuffer.getTexture());
                sReflectRefractNormal.setParameter("maxRefraction", Material::getMaxRefraction());
                sReflectRefractNormal.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                sReflectRefractNormal.setParameter("texture", Shader::CurrentTexture);
                sReflectRefractNormal.setParameter("depthTexture",depthBuffer.getTexture());
                sReflectRefractNormal.setParameter("reflectRefractFactorTexture", reflectRefractBuffer.getTexture());
                sReflectRefractNormal.setParameter("reflectRefractTexture", reflectRefractTextBuffer.getTexture());
                math::Matrix4f viewMatrix = view.getViewMatrix().getMatrix().transpose();
                math::Matrix4f projMatrix = view.getProjMatrix().getMatrix().transpose();
                sLinkedListP2.setParameter("viewMatrix", viewMatrix);
                sLinkedListP2.setParameter("projectionMatrix", projMatrix);
            }
            backgroundColor = sf::Color::Transparent;
            glCheck(glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicBuffer));
            glCheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, linkedListBuffer));
        }
        void ReflectRefractRenderComponent::drawNextFrame() {
            if (frameBuffer.getSettings().versionMajor >= 4 && frameBuffer.getSettings().versionMinor >= 3) {
                RenderStates currentStates;
                math::Matrix4f viewMatrix = view.getViewMatrix().getMatrix().transpose();
                math::Matrix4f projMatrix = view.getProjMatrix().getMatrix().transpose();
                sBuildDepthBuffer.setParameter("viewMatrix", viewMatrix);
                sBuildDepthBuffer.setParameter("projectionMatrix", projMatrix);
                sBuildDepthBufferNormal.setParameter("viewMatrix", viewMatrix);
                sBuildDepthBufferNormal.setParameter("projectionMatrix", projMatrix);
                sBuildReflectRefract.setParameter("viewMatrix", viewMatrix);
                sBuildReflectRefract.setParameter("projectionMatrix", projMatrix);
                for (unsigned int i = 0; i < m_instances.size(); i++) {
                    if (m_instances[i].getAllVertices().getVertexCount() > 0) {
                        if (m_instances[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_instances[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildDepthBuffer.setParameter("textureMatrix", texMatrix);
                            sBuildDepthBuffer.setParameter("haveTexture", 1.f);
                            sBuildReflectRefract.setParameter("textureMatrix", texMatrix);
                            sBuildReflectRefract.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildDepthBuffer.setParameter("haveTexture", 0.f);
                            sBuildReflectRefract.setParameter("haveTexture", 0.f);
                        }
                        sBuildReflectRefract.setParameter("reflectFactor", m_instances[i].getMaterial().getReflectionFactor());
                        sBuildReflectRefract.setParameter("refractFactor", m_instances[i].getMaterial().getRefractionFactor());
                        vb.clear();
                        vb.setPrimitiveType(m_instances[i].getVertexArrays()[0]->getPrimitiveType());
                        matrices.clear();
                        std::vector<TransformMatrix*> tm = m_instances[i].getTransforms();
                        for (unsigned int j = 0; j < tm.size(); j++) {
                            tm[j]->update();
                            std::array<float, 16> matrix = tm[j]->getMatrix().transpose().toGlMatrix();
                            for (unsigned int n = 0; n < 16; n++) {
                                matrices.push_back(matrix[n]);
                            }
                        }
                        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vboWorldMatrices));
                        glCheck(glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(float), &matrices[0], GL_DYNAMIC_DRAW));
                        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
                        if (m_instances[i].getVertexArrays().size() > 0) {
                            for (unsigned int j = 0; j < m_instances[i].getVertexArrays()[0]->getVertexCount(); j++) {
                                vb.append((*m_instances[i].getVertexArrays()[0])[j]);
                            }
                            vb.update();
                        }
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sBuildDepthBuffer;
                        currentStates.texture = m_instances[i].getMaterial().getTexture();
                        depthBuffer.drawInstanced(vb, m_instances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_instances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                        currentStates.shader = &sBuildReflectRefract;
                        reflectRefractBuffer.drawInstanced(vb, m_instances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_instances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                    }
                }
                for (unsigned int i = 0; i < m_normals.size(); i++) {
                    if (m_normals[i].getAllVertices().getVertexCount() > 0) {
                        if (m_normals[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_normals[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildDepthBufferNormal.setParameter("textureMatrix", texMatrix);
                            sBuildDepthBufferNormal.setParameter("haveTexture", 1.f);
                            sBuildReflectRefractNormal.setParameter("textureMatrix", texMatrix);
                            sBuildReflectRefractNormal.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildDepthBufferNormal.setParameter("haveTexture", 0.f);
                            sBuildReflectRefractNormal.setParameter("haveTexture", 0.f);
                        }
                        sBuildReflectRefractNormal.setParameter("reflectFactor", m_instances[i].getMaterial().getReflectionFactor());
                        sBuildReflectRefractNormal.setParameter("refractFactor", m_instances[i].getMaterial().getRefractionFactor());
                        vb.clear();
                        vb.setPrimitiveType(m_normals[i].getAllVertices().getPrimitiveType());
                        for (unsigned int j = 0; j < m_normals[i].getAllVertices().getVertexCount(); j++) {
                            vb.append(m_normals[i].getAllVertices()[j]);
                        }
                        vb.update();
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sBuildDepthBufferNormal;
                        currentStates.texture = m_instances[i].getMaterial().getTexture();
                        depthBuffer.drawVertexBuffer(vb, currentStates);
                        currentStates.shader = &sBuildReflectRefractNormal;
                        reflectRefractBuffer.drawVertexBuffer(vb, currentStates);
                    }
                }
                depthBuffer.display();
                reflectRefractBuffer.display();
                for (unsigned int i = 0; i < m_reflInstances.size(); i++) {
                    if (m_reflInstances[i].getAllVertices().getVertexCount() > 0) {
                        reflectRefractTextBuffer.setActive();
                        reflectRefractTextBuffer.clear(sf::Color::Transparent);
                        GLuint zero = 0;
                        glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer));
                        glCheck(glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero));
                        glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));
                        glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf));
                        glCheck(glBindTexture(GL_TEXTURE_2D, headPtrTex));
                        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, view.getSize().x, view.getSize().y, GL_RED_INTEGER,
                        GL_UNSIGNED_INT, NULL));
                        glCheck(glBindTexture(GL_TEXTURE_2D, 0));
                        reflectRefractTextBuffer.resetGLStates();
                        Entity* reflectEntity = m_reflInstances[i].getVertexArrays()[0]->getEntity()->getRootEntity();
                        View reflectView(reflectEntity->getSize().x, reflectEntity->getSize().y,0, 1000);
                        reflectView.setCenter(reflectEntity->getCenter());
                        math::Vec3f reflDir = reflectEntity->getRefractDir();
                        math::Vec3f target = reflectView.getPosition() + reflDir;
                        reflectView.lookAt(target.x, target.y, target.z);
                        std::vector<Entity*> visibleReflEntities = World::getEntitiesInRect(reflectView.getViewVolume(), expression);
                        rvBatcher.clear();
                        rvNormalBatcher.clear();
                        for (unsigned int j = 0; j < visibleReflEntities.size(); j++) {
                            if (visibleReflEntities[j]->isLeaf() && visibleReflEntities[j]->getDrawOnComponent()) {
                                for (unsigned int n = 0; n < visibleReflEntities[j]->getNbFaces(); n++) {
                                    if (visibleReflEntities[j]->getDrawMode() == Entity::INSTANCED) {
                                        rvBatcher.addFace(visibleReflEntities[j]->getFace(n));
                                    } else {
                                        rvNormalBatcher.addFace(visibleReflEntities[j]->getFace(n));
                                    }
                                }
                            }
                        }
                        m_rvInstances = rvBatcher.getInstances();
                        m_rvNormals = rvNormalBatcher.getInstances();
                        viewMatrix = reflectView.getViewMatrix().getMatrix().transpose();
                        projMatrix = reflectView.getProjMatrix().getMatrix().transpose();
                        sLinkedList.setParameter("viewMatrix", viewMatrix);
                        sLinkedList.setParameter("projectionMatrix", projMatrix);
                        for (unsigned int j = 0; j < m_rvInstances.size(); j++) {
                            if (m_rvInstances[j].getAllVertices().getVertexCount() > 0) {
                                if (m_rvInstances[j].getMaterial().getTexture() != nullptr) {
                                    math::Matrix4f texMatrix = m_rvInstances[j].getMaterial().getTexture()->getTextureMatrix();
                                    sLinkedList.setParameter("textureMatrix", texMatrix);
                                    sLinkedList.setParameter("haveTexture", 1.f);
                                } else {
                                    sLinkedList.setParameter("haveTexture", 0.f);
                                }
                                vb.clear();
                                vb.setPrimitiveType(m_rvInstances[j].getVertexArrays()[0]->getPrimitiveType());
                                matrices.clear();
                                std::vector<TransformMatrix*> tm = m_rvInstances[j].getTransforms();
                                for (unsigned int n = 0; n < tm.size(); n++) {
                                    tm[n]->update();
                                    std::array<float, 16> matrix = tm[n]->getMatrix().transpose().toGlMatrix();
                                    for (unsigned int k = 0; k < 16; k++) {
                                        matrices.push_back(matrix[k]);
                                    }
                                }
                                glCheck(glBindBuffer(GL_ARRAY_BUFFER, vboWorldMatrices));
                                glCheck(glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(float), &matrices[0], GL_DYNAMIC_DRAW));
                                glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
                                if (m_rvInstances[j].getVertexArrays().size() > 0) {
                                    for (unsigned int n = 0; n < m_rvInstances[i].getVertexArrays()[0]->getVertexCount(); n++) {
                                        vb.append((*m_rvInstances[j].getVertexArrays()[0])[n]);
                                    }
                                    vb.update();
                                }
                                currentStates.blendMode = sf::BlendNone;
                                currentStates.shader = &sLinkedList;
                                currentStates.texture = m_rvInstances[j].getMaterial().getTexture();
                                reflectRefractTextBuffer.drawInstanced(vb, m_rvInstances[j].getVertexArrays()[0]->getPrimitiveType(), 0, m_rvInstances[j].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                            }
                        }
                        sLinkedList2.setParameter("viewMatrix", viewMatrix);
                        sLinkedList2.setParameter("projectionMatrix", projMatrix);
                        for (unsigned int j = 0; j < m_rvNormals.size(); j++) {
                            if (m_rvNormals[j].getAllVertices().getVertexCount() > 0) {
                                if (m_rvNormals[j].getMaterial().getTexture() != nullptr) {
                                    math::Matrix4f texMatrix = m_rvNormals[j].getMaterial().getTexture()->getTextureMatrix();
                                    sLinkedList2.setParameter("textureMatrix", texMatrix);
                                    sLinkedList2.setParameter("haveTexture", 1.f);
                                } else {
                                    sLinkedList2.setParameter("haveTexture", 0.f);
                                }
                                vb.clear();
                                vb.setPrimitiveType(m_rvNormals[j].getAllVertices().getPrimitiveType());
                                for (unsigned int n = 0; n < m_rvNormals[j].getAllVertices().getVertexCount(); n++) {
                                    vb.append(m_rvNormals[j].getAllVertices()[n]);
                                }
                                vb.update();
                                currentStates.blendMode = sf::BlendNone;
                                currentStates.shader = &sLinkedList2;
                                currentStates.texture = m_rvNormals[j].getMaterial().getTexture();
                                reflectRefractTextBuffer.drawVertexBuffer(vb, currentStates);
                            }
                        }
                        glCheck(glFinish());
                        glCheck(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                        vb2.clear();
                        vb2.setPrimitiveType(sf::Quads);
                        Vertex v1 (sf::Vector3f(0, 0, 0));
                        Vertex v2 (sf::Vector3f(quad.getSize().x,0, 0));
                        Vertex v3 (sf::Vector3f(quad.getSize().x, quad.getSize().y, quad.getSize().z));
                        Vertex v4 (sf::Vector3f(0, quad.getSize().y, quad.getSize().z));
                        vb2.append(v1);
                        vb2.append(v2);
                        vb2.append(v3);
                        vb2.append(v4);
                        vb2.update();
                        viewMatrix = view.getViewMatrix().getMatrix().transpose();
                        projMatrix = view.getProjMatrix().getMatrix().transpose();
                        sReflectRefract.setParameter("viewMatrix", viewMatrix);
                        sReflectRefract.setParameter("projMatrix", projMatrix);
                        math::Matrix4f matrix = quad.getTransform().getMatrix().transpose();
                        sLinkedListP2.setParameter("worldMat", matrix);
                        currentStates.shader = &sLinkedListP2;
                        reflectRefractTextBuffer.drawVertexBuffer(vb2, currentStates);
                        glCheck(glFinish());
                        reflectRefractTextBuffer.display();
                        math::Matrix4f biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
                                              0.0f, 0.5f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 0.5f, 0.0f,
                                              0.5f, 0.5f, 0.5f, 1.f);
                        math::Matrix4f depthBiasMatrix = biasMatrix;
                        sReflectRefract.setParameter("depthBiasMatrix", depthBiasMatrix.transpose());
                        if (m_reflInstances[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_reflInstances[i].getMaterial().getTexture()->getTextureMatrix();
                            sReflectRefract.setParameter("textureMatrix", texMatrix);
                            sReflectRefract.setParameter("haveTexture", 1.f);
                        } else {
                            sReflectRefract.setParameter("haveTexture", 0.f);
                        }
                        vb.clear();
                        vb.setPrimitiveType(m_reflInstances[i].getVertexArrays()[0]->getPrimitiveType());
                        matrices.clear();
                        std::vector<TransformMatrix*> tm = m_reflInstances[i].getTransforms();
                        for (unsigned int j = 0; j < tm.size(); j++) {
                            tm[j]->update();
                            std::array<float, 16> matrix = tm[j]->getMatrix().transpose().toGlMatrix();
                            for (unsigned int n = 0; n < 16; n++) {
                                matrices.push_back(matrix[n]);
                            }
                        }
                        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vboWorldMatrices));
                        glCheck(glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(float), &matrices[0], GL_DYNAMIC_DRAW));
                        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
                        if (m_reflInstances[i].getVertexArrays().size() > 0) {
                            for (unsigned int j = 0; j < m_reflInstances[i].getVertexArrays()[0]->getVertexCount(); j++) {
                                vb.append((*m_rvInstances[i].getVertexArrays()[0])[j]);
                            }
                            vb.update();
                        }
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sReflectRefract;
                        currentStates.texture = m_reflInstances[i].getMaterial().getTexture();
                        reflectRefractTextBuffer.drawInstanced(vb, m_reflInstances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_reflInstances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                    }
                }
                for (unsigned int i = 0; i < m_reflNormals.size(); i++) {
                    if (m_reflNormals[i].getAllVertices().getVertexCount() > 0) {
                        reflectRefractTextBuffer.setActive();
                        reflectRefractTextBuffer.clear(sf::Color::Transparent);
                        GLuint zero = 0;
                        glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer));
                        glCheck(glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero));
                        glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));
                        glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf));
                        glCheck(glBindTexture(GL_TEXTURE_2D, headPtrTex));
                        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, view.getSize().x, view.getSize().y, GL_RED_INTEGER,
                        GL_UNSIGNED_INT, NULL));
                        glCheck(glBindTexture(GL_TEXTURE_2D, 0));
                        reflectRefractTextBuffer.resetGLStates();
                        Entity* reflectEntity = m_reflNormals[i].getVertexArrays()[0]->getEntity()->getRootEntity();
                        View reflectView(reflectEntity->getSize().x, reflectEntity->getSize().y,0, 1000);
                        reflectView.setCenter(reflectEntity->getCenter());
                        math::Vec3f reflDir = reflectEntity->getRefractDir();
                        math::Vec3f target = reflectView.getPosition() + reflDir;
                        reflectView.lookAt(target.x, target.y, target.z);
                        std::vector<Entity*> visibleReflEntities = World::getEntitiesInRect(reflectView.getViewVolume(), expression);
                        rvBatcher.clear();
                        rvNormalBatcher.clear();
                        for (unsigned int j = 0; j < visibleReflEntities.size(); j++) {
                            if (visibleReflEntities[j]->isLeaf() && visibleReflEntities[j]->getDrawOnComponent()) {
                                for (unsigned int n = 0; n < visibleReflEntities[j]->getNbFaces(); n++) {
                                    if (visibleReflEntities[j]->getDrawMode() == Entity::INSTANCED) {
                                        rvBatcher.addFace(visibleReflEntities[j]->getFace(n));
                                    } else {
                                        rvNormalBatcher.addFace(visibleReflEntities[j]->getFace(n));
                                    }
                                }
                            }
                        }
                        m_rvInstances = rvBatcher.getInstances();
                        m_rvNormals = rvNormalBatcher.getInstances();
                        viewMatrix = reflectView.getViewMatrix().getMatrix().transpose();
                        projMatrix = reflectView.getProjMatrix().getMatrix().transpose();
                        sLinkedList.setParameter("viewMatrix", viewMatrix);
                        sLinkedList.setParameter("projectionMatrix", projMatrix);
                        for (unsigned int j = 0; j < m_rvInstances.size(); j++) {
                            if (m_rvInstances[j].getAllVertices().getVertexCount() > 0) {
                                if (m_rvInstances[j].getMaterial().getTexture() != nullptr) {
                                    math::Matrix4f texMatrix = m_rvInstances[j].getMaterial().getTexture()->getTextureMatrix();
                                    sLinkedList.setParameter("textureMatrix", texMatrix);
                                    sLinkedList.setParameter("haveTexture", 1.f);
                                } else {
                                    sLinkedList.setParameter("haveTexture", 0.f);
                                }
                                vb.clear();
                                vb.setPrimitiveType(m_rvInstances[j].getVertexArrays()[0]->getPrimitiveType());
                                matrices.clear();
                                std::vector<TransformMatrix*> tm = m_rvInstances[j].getTransforms();
                                for (unsigned int n = 0; n < tm.size(); n++) {
                                    tm[n]->update();
                                    std::array<float, 16> matrix = tm[n]->getMatrix().transpose().toGlMatrix();
                                    for (unsigned int k = 0; k < 16; k++) {
                                        matrices.push_back(matrix[k]);
                                    }
                                }
                                glCheck(glBindBuffer(GL_ARRAY_BUFFER, vboWorldMatrices));
                                glCheck(glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(float), &matrices[0], GL_DYNAMIC_DRAW));
                                glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
                                if (m_rvInstances[j].getVertexArrays().size() > 0) {
                                    for (unsigned int n = 0; n < m_rvInstances[i].getVertexArrays()[0]->getVertexCount(); n++) {
                                        vb.append((*m_rvInstances[j].getVertexArrays()[0])[n]);
                                    }
                                    vb.update();
                                }
                                currentStates.blendMode = sf::BlendNone;
                                currentStates.shader = &sLinkedList;
                                currentStates.texture = m_rvInstances[j].getMaterial().getTexture();
                                reflectRefractTextBuffer.drawInstanced(vb, m_rvInstances[j].getVertexArrays()[0]->getPrimitiveType(), 0, m_rvInstances[j].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                            }
                        }
                        sLinkedList2.setParameter("viewMatrix", viewMatrix);
                        sLinkedList2.setParameter("projectionMatrix", projMatrix);
                        for (unsigned int j = 0; j < m_rvNormals.size(); j++) {
                            if (m_rvNormals[j].getAllVertices().getVertexCount() > 0) {
                                if (m_rvNormals[j].getMaterial().getTexture() != nullptr) {
                                    math::Matrix4f texMatrix = m_rvNormals[j].getMaterial().getTexture()->getTextureMatrix();
                                    sLinkedList2.setParameter("textureMatrix", texMatrix);
                                    sLinkedList2.setParameter("haveTexture", 1.f);
                                } else {
                                    sLinkedList2.setParameter("haveTexture", 0.f);
                                }
                                vb.clear();
                                vb.setPrimitiveType(m_rvNormals[j].getAllVertices().getPrimitiveType());
                                for (unsigned int n = 0; n < m_rvNormals[j].getAllVertices().getVertexCount(); n++) {
                                    vb.append(m_rvNormals[j].getAllVertices()[n]);
                                }
                                vb.update();
                                currentStates.blendMode = sf::BlendNone;
                                currentStates.shader = &sLinkedList2;
                                currentStates.texture = m_rvNormals[j].getMaterial().getTexture();
                                reflectRefractTextBuffer.drawVertexBuffer(vb, currentStates);
                            }
                        }
                        glCheck(glFinish());
                        glCheck(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                        vb2.clear();
                        vb2.setPrimitiveType(sf::Quads);
                        Vertex v1 (sf::Vector3f(0, 0, 0));
                        Vertex v2 (sf::Vector3f(quad.getSize().x,0, 0));
                        Vertex v3 (sf::Vector3f(quad.getSize().x, quad.getSize().y, quad.getSize().z));
                        Vertex v4 (sf::Vector3f(0, quad.getSize().y, quad.getSize().z));
                        vb2.append(v1);
                        vb2.append(v2);
                        vb2.append(v3);
                        vb2.append(v4);
                        vb2.update();
                        math::Matrix4f matrix = quad.getTransform().getMatrix().transpose();
                        sLinkedListP2.setParameter("worldMat", matrix);
                        currentStates.shader = &sLinkedListP2;
                        reflectRefractTextBuffer.drawVertexBuffer(vb2, currentStates);
                        glCheck(glFinish());
                        reflectRefractTextBuffer.display();
                        viewMatrix = view.getViewMatrix().getMatrix().transpose();
                        projMatrix = view.getProjMatrix().getMatrix().transpose();
                        sReflectRefractNormal.setParameter("viewMatrix", viewMatrix);
                        sReflectRefractNormal.setParameter("projMatrix", projMatrix);
                        math::Matrix4f biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
                                              0.0f, 0.5f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 0.5f, 0.0f,
                                              0.5f, 0.5f, 0.5f, 1.f);
                        math::Matrix4f depthBiasMatrix = biasMatrix;
                        sReflectRefractNormal.setParameter("depthBiasMatrix", depthBiasMatrix.transpose());
                        if (m_reflNormals[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_reflNormals[i].getMaterial().getTexture()->getTextureMatrix();
                            sReflectRefractNormal.setParameter("textureMatrix", texMatrix);
                            sReflectRefractNormal.setParameter("haveTexture", 1.f);
                        } else {
                            sReflectRefractNormal.setParameter("haveTexture", 0.f);
                        }
                        vb.clear();
                        vb.setPrimitiveType(m_reflNormals[i].getAllVertices().getPrimitiveType());
                        for (unsigned int n = 0; n < m_reflNormals[i].getAllVertices().getVertexCount(); n++) {
                            vb.append(m_reflNormals[i].getAllVertices()[n]);
                        }
                        vb.update();
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sReflectRefractNormal;
                        currentStates.texture = m_reflNormals[i].getMaterial().getTexture();
                        reflectRefractTextBuffer.drawVertexBuffer(vb, currentStates);
                    }
                }
            }
        }
        std::vector<Entity*> ReflectRefractRenderComponent::getEntities() {
            return visibleEntities;
        }
        bool ReflectRefractRenderComponent::loadEntitiesOnComponent(std::vector<Entity*> vEntities) {
            batcher.clear();
            normalBatcher.clear();
            reflBatcher.clear();
            reflNormalBatcher.clear();
            for (unsigned int i = 0; i < vEntities.size(); i++) {
                if ( vEntities[i]->isLeaf() && vEntities[i]->getDrawOnComponent()) {
                    for (unsigned int j = 0; j <  vEntities[i]->getNbFaces(); j++) {
                         if (vEntities[i]->getDrawMode() == Entity::INSTANCED) {
                            batcher.addFace( vEntities[i]->getFace(j));
                         } else {
                            normalBatcher.addFace(vEntities[i]->getFace(j));
                         }
                         if (vEntities[i]->isReflectable()) {
                            if (vEntities[i]->getDrawMode() == Entity::INSTANCED) {
                                reflBatcher.addFace( vEntities[i]->getFace(j));
                            } else {
                                reflNormalBatcher.addFace(vEntities[i]->getFace(j));
                            }
                        }
                    }
                }
            }
            update = true;
            return true;
        }
        bool ReflectRefractRenderComponent::needToUpdate() {
            return update;
        }
        void ReflectRefractRenderComponent::clear() {
            frameBuffer.clear(backgroundColor);
            reflectRefractBuffer.clear(sf::Color::Transparent);
            depthBuffer.clear(sf::Color::Transparent);
        }
        void ReflectRefractRenderComponent::setBackgroundColor (sf::Color color) {
            this->backgroundColor = color;
        }
        void ReflectRefractRenderComponent::setExpression (std::string expression) {
            this->expression = expression;
        }
        void ReflectRefractRenderComponent::draw (Drawable& drawable, RenderStates states) {

        }
        void ReflectRefractRenderComponent::draw (RenderTarget& target, RenderStates states) {
            frameBufferSprite.setCenter(target.getView().getPosition());
            target.draw(frameBufferSprite, states);
        }
        std::string ReflectRefractRenderComponent::getExpression() {
            return expression;
        }
        int ReflectRefractRenderComponent::getLayer() {
            return getPosition().z;
        }
        void ReflectRefractRenderComponent::changeVisibleEntities(Entity* toRemove, Entity* toAdd, EntityManager* em) {
        }
        void ReflectRefractRenderComponent::pushEvent(window::IEvent event, RenderWindow& rw) {
            if (event.type == window::IEvent::WINDOW_EVENT && event.window.type == window::IEvent::WINDOW_EVENT_RESIZED && &getWindow() == &rw && isAutoResized()) {
                std::cout<<"recompute size"<<std::endl;
                recomputeSize();
                getListener().pushEvent(event);
                getView().reset(physic::BoundingBox(getView().getViewport().getPosition().x, getView().getViewport().getPosition().y, getView().getViewport().getPosition().z, event.window.data1, event.window.data2, getView().getViewport().getDepth()));
            }
        }
        void ReflectRefractRenderComponent::updateParticleSystems() {

        }
        void ReflectRefractRenderComponent::setView (View view) {
            frameBuffer.setView(view);
            reflectRefractBuffer.setView(view);
            depthBuffer.setView(view);
            this->view = view;
        }
        View& ReflectRefractRenderComponent::getView() {
            return view;
        }
        ReflectRefractRenderComponent::~ReflectRefractRenderComponent() {
            glDeleteBuffers(1, &atomicBuffer);
            glDeleteBuffers(1, &linkedListBuffer);
            glDeleteBuffers(1, &clearBuf);
            glDeleteTextures(1, &headPtrTex);
        }
    }
}
