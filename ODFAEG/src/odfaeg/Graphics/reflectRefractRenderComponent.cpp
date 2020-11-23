#include "../../../include/odfaeg/Graphics/reflectRefractRenderComponent.hpp"
#include "glCheck.h"
namespace odfaeg {
    namespace graphic {
        ReflectRefractRenderComponent::ReflectRefractRenderComponent (RenderWindow& window, int layer, std::string expression, window::ContextSettings settings) :
            HeavyComponent(window, math::Vec3f(window.getView().getPosition().x, window.getView().getPosition().y, layer),
                          math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0),
                          math::Vec3f(window.getView().getSize().x + window.getView().getSize().x * 0.5f, window.getView().getPosition().y + window.getView().getSize().y * 0.5f, layer)),
            view(window.getView()),
            expression(expression)
            {
            if (window.getView().getSize().x > window.getView().getSize().y) {
                squareSize = window.getView().getSize().x;
            } else {
                squareSize = window.getView().getSize().y;
            }
            quad = RectangleShape(math::Vec3f(squareSize, squareSize, squareSize * 0.5f));
            quad.move(math::Vec3f(-squareSize * 0.5f, -squareSize * 0.5f, 0));
            dirs[0] = math::Vec3f(1, 0, 0);
            dirs[1] = math::Vec3f(-1, 0, 0);
            dirs[2] = math::Vec3f(0, 1, 0);
            dirs[3] = math::Vec3f(0, -1, 0);
            dirs[4] = math::Vec3f(0, 0, 1);
            dirs[5] = math::Vec3f(0, 0, -1);
            settings.depthBits = 24;
            depthBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            depthBufferSprite = Sprite(depthBuffer.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            rDepthBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            sf::Vector3i resolution ((int) window.getSize().x, (int) window.getSize().y, window.getView().getSize().z);
            settings.depthBits = 0;
            reflectRefractTex.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            reflectRefractTex.setEnableCubeMap(true);
            reflectRefractTexSprite = Sprite(reflectRefractTex.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            environmentMap.create(squareSize, squareSize, settings, GL_TEXTURE_CUBE_MAP);
            GLuint maxNodes = 5 * squareSize * squareSize;
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
            glCheck(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, squareSize, squareSize));
            glCheck(glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI));
            glCheck(glBindTexture(GL_TEXTURE_2D, 0));
            std::vector<GLuint> headPtrClearBuf(squareSize*squareSize, 0xffffffff);
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
                const std::string linkedListVertexShader = R"(#version 460
                                                    layout (location = 0) in vec3 position;
                                                    layout (location = 1) in vec4 color;
                                                    layout (location = 2) in vec2 texCoords;
                                                    layout (location = 3) in vec3 normals;
                                                    layout (location = 4) in mat4 worldMat;
                                                    uniform mat4 projectionMatrix;
                                                    uniform mat4 viewMatrix;
                                                    uniform mat4 textureMatrix;
                                                    out vec4 vColor;
                                                    out vec2 vTexCoord;
                                                    void main() {
                                                        gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                        vTexCoord = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                        vColor = color;
                                                    }
                                                    )";
                const std::string  linkedListNormalVertexShader = R"(#version 460
                                                        layout (location = 0) in vec3 position;
                                                        layout (location = 1) in vec4 color;
                                                        layout (location = 2) in vec2 texCoords;
                                                        layout (location = 3) in vec3 normals;
                                                        uniform mat4 textureMatrix;
                                                        uniform mat4 projectionMatrix;
                                                        uniform mat4 viewMatrix;
                                                        out vec4 vColor;
                                                        out vec2 vTexCoord;
                                                        void main () {
                                                            gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.f);
                                                            vTexCoord = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                            vColor = color;
                                                        })";
                const std::string vertexShader = R"(#version 460
                                                    layout (location = 0) in vec3 position;
                                                    layout (location = 1) in vec4 color;
                                                    layout (location = 2) in vec2 texCoords;
                                                    layout (location = 3) in vec3 normals;
                                                    layout (location = 4) in mat4 worldMat;
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
                const std::string  normalVertexShader = R"(#version 460
                                                        layout (location = 0) in vec3 position;
                                                        layout (location = 1) in vec4 color;
                                                        layout (location = 2) in vec2 texCoords;
                                                        layout (location = 3) in vec3 normals;
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
                const std::string  linkedListVertexShader2 = R"(#version 460
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
                const std::string geometryShader = R"(#version 460
                                                      #extension GL_EXT_geometry_shader4 : enable
                                                      layout (triangles) in;
                                                      layout (triangle_strip, max_vertices = 4) out;
                                                      uniform int layer;
                                                      in vec4 vColor[];
                                                      in vec2 vTexCoord[];
                                                      out vec4 frontColor;
                                                      out vec2 fTexCoords;
                                                      void main() {
                                                        /*gl_Layer = layer;*/
                                                        gl_Position = gl_in[0].gl_Position;
                                                        frontColor = vColor[0];
                                                        fTexCoords = vTexCoord[0];
                                                        EmitVertex();
                                                        gl_Position = gl_in[1].gl_Position;
                                                        frontColor = vColor[1];
                                                        fTexCoords = vTexCoord[1];
                                                        EmitVertex();
                                                        gl_Position = gl_in[2].gl_Position;
                                                        frontColor = vColor[2];
                                                        fTexCoords = vTexCoord[2];
                                                        EmitVertex();
                                                        EndPrimitive();
                                                      }
                                                      )";
                const std::string perPixReflectRefractVertexShader = R"(#version 460
                                                                   layout (location = 0) in vec3 position;
                                                                   layout (location = 1) in vec4 color;
                                                                   layout (location = 2) in vec2 texCoords;
                                                                   layout (location = 3) in vec3 normals;
                                                                   layout (location = 4) in mat4 worldMat;
                                                                   uniform mat4 projectionMatrix;
                                                                   uniform mat4 viewMatrix;
                                                                   out vec3 normal;
                                                                   out vec3 pos;
                                                                   out vec4 frontColor;
                                                                   void main() {
                                                                       normal = mat3(transpose(inverse(worldMat))) * normals;
                                                                       pos = vec3(worldMat * vec4(position, 1.0));
                                                                       gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                                       frontColor = color;
                                                                   }
                                                                  )";
                const std::string perPixReflectRefractVertexNormalShader = R"(#version 460
                                                                   layout (location = 0) in vec3 position;
                                                                   layout (location = 1) in vec4 color;
                                                                   layout (location = 2) in vec2 texCoords;
                                                                   layout (location = 3) in vec3 normals;
                                                                   uniform mat4 projectionMatrix;
                                                                   uniform mat4 viewMatrix;
                                                                   out vec4 frontColor;
                                                                   out vec2 fTexCoords;
                                                                   out vec3 normal;
                                                                   out vec3 pos;
                                                                   void main() {
                                                                       gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.f);
                                                                       frontColor = color;
                                                                       normal = normals;
                                                                       pos = position;
                                                                   }
                                                                  )";
                const std::string buildDepthBufferFragmentShader = R"(#version 460
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
                const std::string buildFramebufferShader = R"(#version 460
                                                                in vec4 frontColor;
                                                                in vec3 normal;
                                                                in vec3 pos;
                                                                const vec2 size = vec2(2.0,0.0);
                                                                const ivec3 off = ivec3(-1,0,1);
                                                                uniform vec3 cameraPos;
                                                                uniform samplerCube sceneBox;
                                                                uniform sampler2D depthBuffer;
                                                                uniform sampler2D rDepthBuffer;
                                                                uniform vec3 resolution;
                                                                layout (location = 0) out vec4 fColor;
                                                                void main () {
                                                                    vec2 position = (gl_FragCoord.xy / resolution.xy);
                                                                    vec4 depth = texture2D(depthBuffer, position);
                                                                    float s01 = textureOffset(rDepthBuffer, position, off.xy).z;
                                                                    float s21 = textureOffset(rDepthBuffer, position, off.zy).z;
                                                                    float s10 = textureOffset(rDepthBuffer, position, off.yx).z;
                                                                    float s12 = textureOffset(rDepthBuffer, position, off.yz).z;
                                                                    vec3 va = normalize (vec3(size.xy, s21 - s01));
                                                                    vec3 vb = normalize (vec3(size.yx, s12 - s10));
                                                                    vec3 n = cross(va, vb);
                                                                    float ratio = 1.00 / 1.33;
                                                                    vec3 i = (pos - cameraPos);
                                                                    vec3 r = refract (i, normalize(n), ratio);
                                                                    if (depth.z > 0) {
                                                                        fColor = texture(sceneBox, r) * (1 - depth.a);
                                                                    } else {
                                                                        fColor = texture(sceneBox, r);
                                                                    }
                                                                }
                                                              )";
                const std::string fragmentShader = R"(#version 460
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
                      for( int i = count - 1; i >= 0; i--)
                      {
                        color.rgb = frags[i].color.rgb * frags[i].color.a + color.rgb * (1 - frags[i].color.a);
                        color.a = frags[i].color.a + color.a * (1 - frags[i].color.a);
                      }
                      fcolor = color;
                   })";
                if (!sBuildDepthBuffer.loadFromMemory(vertexShader, buildDepthBufferFragmentShader)) {
                    throw core::Erreur(50, "Error, failed to load build depth buffer shader", 3);
                }
                if (!sBuildDepthBufferNormal.loadFromMemory(normalVertexShader, buildDepthBufferFragmentShader)) {
                    throw core::Erreur(51, "Error, failed to load build normal depth buffer shader", 3);
                }
                if (!sReflectRefract.loadFromMemory(perPixReflectRefractVertexShader, buildFramebufferShader)) {
                    throw core::Erreur(57, "Error, failed to load reflect refract shader", 3);
                }
                if (!sReflectRefractNormal.loadFromMemory(perPixReflectRefractVertexNormalShader, buildFramebufferShader)) {
                    throw core::Erreur(58, "Error, failed to load reflect refract normal shader", 3);
                }
                if (!sLinkedList.loadFromMemory(vertexShader, fragmentShader/*, geometryShader*/)) {
                    throw core::Erreur(58, "Error, failed to load per pixel linked list shader", 3);
                }
                if (!sLinkedListNormal.loadFromMemory(normalVertexShader, fragmentShader/*, geometryShader*/)) {
                    throw core::Erreur(58, "Error, failed to load per pixel linked list normal shader", 3);
                }
                if (!sLinkedList2.loadFromMemory(linkedListVertexShader2, fragmentShader2)) {
                    throw core::Erreur(58, "Error, failed to load per pixel linked list 2 shader", 3);
                }
                sBuildDepthBuffer.setParameter("texture", Shader::CurrentTexture);
                sBuildDepthBufferNormal.setParameter("texture", Shader::CurrentTexture);
                sReflectRefract.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                sReflectRefract.setParameter("depthBuffer", depthBuffer.getTexture());
                sReflectRefract.setParameter("sceneBox", Shader::CurrentTexture);
                sReflectRefract.setParameter("rDepthBuffer", rDepthBuffer.getTexture());
                sReflectRefractNormal.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                sReflectRefractNormal.setParameter("sceneBox", Shader::CurrentTexture);
                sReflectRefractNormal.setParameter("depthBuffer", depthBuffer.getTexture());
                sReflectRefractNormal.setParameter("rDepthBuffer", rDepthBuffer.getTexture());
                sLinkedList.setParameter("maxNodes", maxNodes);
                sLinkedList.setParameter("texture", Shader::CurrentTexture);
                sLinkedListNormal.setParameter("maxNodes", maxNodes);
                sLinkedListNormal.setParameter("texture", Shader::CurrentTexture);
                View defaultView = window.getDefaultView();
                defaultView.setPerspective(-squareSize * 0.5f, squareSize * 0.5f, -squareSize * 0.5f, squareSize * 0.5f, 0, 1000);
                math::Matrix4f viewMatrix = defaultView.getViewMatrix().getMatrix().transpose();
                math::Matrix4f projMatrix = defaultView.getProjMatrix().getMatrix().transpose();
                sLinkedList2.setParameter("viewMatrix", viewMatrix);
                sLinkedList2.setParameter("projectionMatrix", projMatrix);
            }
            backgroundColor = sf::Color::Transparent;
            glCheck(glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicBuffer));
            glCheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, linkedListBuffer));
        }
        void ReflectRefractRenderComponent::drawNextFrame() {
            if (reflectRefractTex.getSettings().versionMajor >= 4 && reflectRefractTex.getSettings().versionMinor >= 3) {
                RenderStates currentStates;
                math::Matrix4f viewMatrix = view.getViewMatrix().getMatrix().transpose();
                math::Matrix4f projMatrix = view.getProjMatrix().getMatrix().transpose();
                sBuildDepthBuffer.setParameter("viewMatrix", viewMatrix);
                sBuildDepthBuffer.setParameter("projectionMatrix", projMatrix);
                sBuildDepthBufferNormal.setParameter("viewMatrix", viewMatrix);
                sBuildDepthBufferNormal.setParameter("projectionMatrix", projMatrix);
                for (unsigned int i = 0; i < m_instances.size(); i++) {
                    if (m_instances[i].getAllVertices().getVertexCount() > 0) {
                        if (m_instances[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_instances[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildDepthBuffer.setParameter("textureMatrix", texMatrix);
                            sBuildDepthBuffer.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildDepthBuffer.setParameter("haveTexture", 0.f);
                        }
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
                    }
                }
                for (unsigned int i = 0; i < m_normals.size(); i++) {
                    if (m_normals[i].getAllVertices().getVertexCount() > 0) {
                        if (m_normals[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_normals[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildDepthBufferNormal.setParameter("textureMatrix", texMatrix);
                            sBuildDepthBufferNormal.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildDepthBufferNormal.setParameter("haveTexture", 0.f);
                        }
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
                    }
                }
                depthBuffer.display();
                for (unsigned int i = 0; i < m_reflInstances.size(); i++) {
                    if (m_reflInstances[i].getAllVertices().getVertexCount() > 0) {
                        if (m_reflInstances[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_reflInstances[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildDepthBuffer.setParameter("textureMatrix", texMatrix);
                            sBuildDepthBuffer.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildDepthBuffer.setParameter("haveTexture", 0.f);
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
                                vb.append((*m_reflInstances[i].getVertexArrays()[0])[j]);
                            }
                            vb.update();
                        }
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sBuildDepthBuffer;
                        currentStates.texture = m_reflInstances[i].getMaterial().getTexture();
                        rDepthBuffer.drawInstanced(vb, m_reflInstances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_reflInstances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                    }
                }
                for (unsigned int i = 0; i < m_reflNormals.size(); i++) {
                    if (m_reflNormals[i].getAllVertices().getVertexCount() > 0) {
                        if (m_reflNormals[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_reflNormals[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildDepthBufferNormal.setParameter("textureMatrix", texMatrix);
                            sBuildDepthBufferNormal.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildDepthBufferNormal.setParameter("haveTexture", 0.f);
                        }
                        vb.clear();
                        vb.setPrimitiveType(m_reflNormals[i].getAllVertices().getPrimitiveType());
                        for (unsigned int j = 0; j < m_reflNormals[i].getAllVertices().getVertexCount(); j++) {
                            vb.append(m_reflNormals[i].getAllVertices()[j]);
                        }
                        vb.update();
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sBuildDepthBufferNormal;
                        currentStates.texture = m_reflNormals[i].getMaterial().getTexture();
                        rDepthBuffer.drawVertexBuffer(vb, currentStates);
                    }
                }
                rDepthBuffer.display();
                View reflectView;
                if (view.isOrtho()) {
                    reflectView = View (squareSize, squareSize, view.getViewport().getPosition().z, view.getViewport().getSize().z);
                } else {
                    reflectView = View (squareSize, squareSize, 90, view.getViewport().getPosition().z, view.getViewport().getSize().z);
                }
                reflectView.setCenter(view.getPosition());
                for (unsigned int m = 0; m < 6; m++) {
                    math::Vec3f target = reflectView.getPosition() + dirs[m];
                    reflectView.lookAt(target.x, target.y, target.z);
                    std::vector<Entity*> visibleReflEntities = World::getEntitiesInRect(reflectView.getViewVolume(), expression);
                    std::vector<Entity*> vEntities;
                    for (unsigned int j = 0; j < visibleReflEntities.size(); j++)  {
                        if (!visibleReflEntities[j]->isReflectable()) {
                            vEntities.push_back(visibleReflEntities[j]);
                        }
                    }
                    rvBatcher.clear();
                    normalRvBatcher.clear();
                    for (unsigned int j = 0; j < vEntities.size(); j++) {
                        for (unsigned int n = 0; n < vEntities[j]->getNbFaces(); n++) {
                            if (vEntities[j]->getDrawMode() == Entity::INSTANCED) {
                                rvBatcher.addFace(vEntities[j]->getFace(n));
                            } else {
                                normalRvBatcher.addFace(vEntities[j]->getFace(n));
                            }
                        }
                    }
                    std::vector<Instance> rvInstances = rvBatcher.getInstances();
                    std::vector<Instance> rvNormals = normalRvBatcher.getInstances();
                    environmentMap.setView(reflectView);
                    environmentMap.setActive();
                    GLuint zero = 0;
                    glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicBuffer));
                    glCheck(glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero));
                    glCheck(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));
                    glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf));
                    glCheck(glBindTexture(GL_TEXTURE_2D, headPtrTex));
                    glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, squareSize, squareSize, GL_RED_INTEGER,
                    GL_UNSIGNED_INT, NULL));
                    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
                    environmentMap.resetGLStates();
                    environmentMap.selectCubemapFace(m);
                    viewMatrix = reflectView.getViewMatrix().getMatrix().transpose();
                    projMatrix = reflectView.getProjMatrix().getMatrix().transpose();
                    sLinkedList.setParameter("viewMatrix", viewMatrix);
                    sLinkedList.setParameter("projectionMatrix", projMatrix);
                    for (unsigned int i = 0; i < rvInstances.size(); i++) {
                        if (rvInstances[i].getAllVertices().getVertexCount() > 0) {
                            matrices.clear();
                            std::vector<TransformMatrix*> tm = rvInstances[i].getTransforms();
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
                            vb.clear();
                            vb.setPrimitiveType(rvInstances[i].getVertexArrays()[0]->getPrimitiveType());
                            if (rvInstances[i].getVertexArrays().size() > 0) {
                                for (unsigned int j = 0; j < rvInstances[i].getVertexArrays()[0]->getVertexCount(); j++) {
                                    vb.append((*rvInstances[i].getVertexArrays()[0])[j]);
                                }
                                vb.update();
                            }
                            currentStates.blendMode = sf::BlendNone;
                            currentStates.shader = &sLinkedList;
                            currentStates.texture =rvInstances[i].getMaterial().getTexture();
                            if (rvInstances[i].getMaterial().getTexture() != nullptr) {
                                math::Matrix4f texMatrix = rvInstances[i].getMaterial().getTexture()->getTextureMatrix();
                                sLinkedList.setParameter("textureMatrix", texMatrix);
                                sLinkedList.setParameter("haveTexture", 1.f);
                            } else {
                                sLinkedList.setParameter("haveTexture", 0.f);
                            }
                            environmentMap.drawInstanced(vb, rvInstances[i].getVertexArrays()[0]->getPrimitiveType(), 0, rvInstances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                        }
                    }
                    viewMatrix = reflectView.getViewMatrix().getMatrix().transpose();
                    projMatrix = reflectView.getProjMatrix().getMatrix().transpose();
                    sLinkedListNormal.setParameter("viewMatrix", viewMatrix);
                    sLinkedListNormal.setParameter("projectionMatrix", projMatrix);
                    for (unsigned int i = 0; i < rvNormals.size(); i++) {
                       if (rvNormals[i].getAllVertices().getVertexCount() > 0) {
                            if (rvNormals[i].getMaterial().getTexture() == nullptr) {
                                sLinkedListNormal.setParameter("haveTexture", 0.f);
                            } else {
                                math::Matrix4f texMatrix = rvNormals[i].getMaterial().getTexture()->getTextureMatrix();
                                sLinkedListNormal.setParameter("textureMatrix", texMatrix);
                                sLinkedListNormal.setParameter("haveTexture", 1.f);
                            }
                            currentStates.blendMode = sf::BlendNone;
                            currentStates.shader = &sLinkedListNormal;
                            currentStates.texture = rvNormals[i].getMaterial().getTexture();
                            vb.clear();
                            vb.setPrimitiveType(rvNormals[i].getAllVertices().getPrimitiveType());
                            for (unsigned int j = 0; j < rvNormals[i].getAllVertices().getVertexCount(); j++) {
                                vb.append(rvNormals[i].getAllVertices()[j]);
                            }
                            vb.update();
                            environmentMap.drawVertexBuffer(vb, currentStates);
                        }
                    }
                    glCheck(glFinish());
                    glCheck(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                    vb2.clear();
                    vb2.setPrimitiveType(sf::Quads);
                    Vertex v1 (sf::Vector3f(0, 0, quad.getSize().z));
                    Vertex v2 (sf::Vector3f(quad.getSize().x,0, quad.getSize().z));
                    Vertex v3 (sf::Vector3f(quad.getSize().x, quad.getSize().y, quad.getSize().z));
                    Vertex v4 (sf::Vector3f(0, quad.getSize().y, quad.getSize().z));
                    vb2.append(v1);
                    vb2.append(v2);
                    vb2.append(v3);
                    vb2.append(v4);
                    vb2.update();
                    math::Matrix4f matrix = quad.getTransform().getMatrix().transpose();
                    sLinkedList2.setParameter("worldMat", matrix);
                    currentStates.shader = &sLinkedList2;
                    currentStates.texture = nullptr;
                    environmentMap.drawVertexBuffer(vb2, currentStates);
                    glCheck(glFinish());
                    glCheck(glMemoryBarrier(GL_ALL_BARRIER_BITS));
                }
                environmentMap.display();
                viewMatrix = view.getViewMatrix().getMatrix().transpose();
                projMatrix = view.getProjMatrix().getMatrix().transpose();
                sReflectRefract.setParameter("viewMatrix", viewMatrix);
                sReflectRefract.setParameter("projectionMatrix", projMatrix);
                sReflectRefract.setParameter("cameraPos", view.getPosition().x, view.getPosition().y, view.getPosition().z);
                for (unsigned int i = 0; i < m_reflInstances.size(); i++) {
                    if (m_reflInstances[i].getAllVertices().getVertexCount() > 0) {
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
                                vb.append((*m_reflInstances[i].getVertexArrays()[0])[j]);
                            }
                            vb.update();
                        }
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sReflectRefract;
                        currentStates.texture = &environmentMap.getTexture();
                        reflectRefractTex.drawInstanced(vb, m_reflInstances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_reflInstances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                    }
                }
                viewMatrix = view.getViewMatrix().getMatrix().transpose();
                projMatrix = view.getProjMatrix().getMatrix().transpose();
                sReflectRefractNormal.setParameter("viewMatrix", viewMatrix);
                sReflectRefractNormal.setParameter("projectionMatrix", projMatrix);
                sReflectRefractNormal.setParameter("cameraPos", view.getPosition().x, view.getPosition().y, view.getPosition().z);
                for (unsigned int i = 0; i < m_reflNormals.size(); i++) {
                    if (m_reflNormals[i].getAllVertices().getVertexCount() > 0) {
                        vb.clear();
                        vb.setPrimitiveType(m_reflNormals[i].getAllVertices().getPrimitiveType());
                        for (unsigned int j = 0; j < m_reflNormals[i].getAllVertices().getVertexCount(); j++) {
                            vb.append(m_reflNormals[i].getAllVertices()[j]);
                        }
                        vb.update();
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sReflectRefractNormal;
                        currentStates.texture = &environmentMap.getTexture();
                        reflectRefractTex.drawVertexBuffer(vb, currentStates);
                    }
                }
                reflectRefractTex.display();
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
                        if (vEntities[i]->isReflectable()) {
                            if (vEntities[i]->getDrawMode() == Entity::INSTANCED) {
                                reflBatcher.addFace( vEntities[i]->getFace(j));
                            } else {
                                reflNormalBatcher.addFace(vEntities[i]->getFace(j));
                            }
                        } else {
                            if (vEntities[i]->getDrawMode() == Entity::INSTANCED) {
                                batcher.addFace( vEntities[i]->getFace(j));
                            } else {
                                normalBatcher.addFace(vEntities[i]->getFace(j));
                            }
                        }
                    }
                }
            }
            m_instances = batcher.getInstances();
            m_normals = normalBatcher.getInstances();
            m_reflInstances = reflBatcher.getInstances();
            m_reflNormals = reflNormalBatcher.getInstances();
            visibleEntities = vEntities;
            update = true;
            return true;
        }
        bool ReflectRefractRenderComponent::needToUpdate() {
            return update;
        }
        void ReflectRefractRenderComponent::clear() {
            depthBuffer.clear(sf::Color::Transparent);
            environmentMap.clear(sf::Color::Transparent);
            reflectRefractTex.clear(sf::Color::Transparent);
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
            reflectRefractTexSprite.setCenter(target.getView().getPosition());
            target.draw(reflectRefractTexSprite, states);
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
            depthBuffer.setView(view);
            reflectRefractTex.setView(view);
            this->view = view;
        }
        View& ReflectRefractRenderComponent::getView() {
            return view;
        }
        ReflectRefractRenderComponent::~ReflectRefractRenderComponent() {
            glDeleteBuffers(1, &vboWorldMatrices);
            glDeleteBuffers(1, &atomicBuffer);
            glDeleteBuffers(1, &linkedListBuffer);
            glDeleteBuffers(1, &clearBuf);
            glDeleteTextures(1, &headPtrTex);
        }
    }
}
