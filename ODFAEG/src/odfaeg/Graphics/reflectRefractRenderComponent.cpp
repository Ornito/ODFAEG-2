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
            ppll(window, layer, expression, settings) {
            frameBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            frameBufferSprite = Sprite(frameBuffer.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            reflectRefractTextSprite = Sprite(ppll.getFrameBufferTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            reflectRefractSprite = Sprite(reflectRefractBuffer.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            sf::Vector3i resolution ((int) window.getSize().x, (int) window.getSize().y, window.getView().getSize().z);
            reflectRefractBuffer.create (window.getView().getSize().x, window.getView().getSize().y, settings);
            settings.depthBits = 24;
            depthBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
            depthBufferSprite = Sprite(depthBuffer.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
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
                                                                    fColor = colorToReflect * reflectRefractInd.x;
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
                sReflectRefract.setParameter("maxRefraction", Material::getMaxRefraction());
                sReflectRefract.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                sReflectRefract.setParameter("texture", Shader::CurrentTexture);
                sReflectRefract.setParameter("depthBuffer",depthBuffer.getTexture());
                sReflectRefract.setParameter("reflectRefractFactorTexture", reflectRefractBuffer.getTexture());
                sReflectRefract.setParameter("reflectRefractTexture", ppll.getFrameBufferTexture());
                sReflectRefractNormal.setParameter("maxRefraction", Material::getMaxRefraction());
                sReflectRefractNormal.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                sReflectRefractNormal.setParameter("texture", Shader::CurrentTexture);
                sReflectRefractNormal.setParameter("depthTexture",depthBuffer.getTexture());
                sReflectRefractNormal.setParameter("reflectRefractFactorTexture", reflectRefractBuffer.getTexture());
                sReflectRefractNormal.setParameter("reflectRefractTexture", ppll.getFrameBufferTexture());
            }
            backgroundColor = sf::Color::Transparent;
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
                sBuildReflectRefractNormal.setParameter("viewMatrix", viewMatrix);
                sBuildReflectRefractNormal.setParameter("projectionMatrix", projMatrix);
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
                for (unsigned int i = 0; i < m_reflInstances.size(); i++) {
                    if (m_reflInstances[i].getAllVertices().getVertexCount() > 0) {
                        if (m_reflInstances[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_reflInstances[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildReflectRefract.setParameter("textureMatrix", texMatrix);
                            sBuildReflectRefract.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildReflectRefract.setParameter("haveTexture", 0.f);
                        }
                        sBuildReflectRefract.setParameter("reflectFactor", m_reflInstances[i].getMaterial().getReflectionFactor());
                        sBuildReflectRefract.setParameter("refractFactor", m_reflInstances[i].getMaterial().getRefractionFactor());
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
                        currentStates.texture = m_reflInstances[i].getMaterial().getTexture();
                        currentStates.shader = &sBuildReflectRefract;
                        reflectRefractBuffer.drawInstanced(vb, m_reflInstances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_reflInstances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
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
                for (unsigned int i = 0; i < m_reflNormals.size(); i++) {
                    if (m_reflNormals[i].getAllVertices().getVertexCount() > 0) {
                        if (m_reflNormals[i].getMaterial().getTexture() != nullptr) {
                            math::Matrix4f texMatrix = m_reflNormals[i].getMaterial().getTexture()->getTextureMatrix();
                            sBuildReflectRefractNormal.setParameter("textureMatrix", texMatrix);
                            sBuildReflectRefractNormal.setParameter("haveTexture", 1.f);
                        } else {
                            sBuildReflectRefractNormal.setParameter("haveTexture", 0.f);
                        }
                        sBuildReflectRefractNormal.setParameter("reflectFactor", m_reflNormals[i].getMaterial().getReflectionFactor());
                        sBuildReflectRefractNormal.setParameter("refractFactor", m_reflNormals[i].getMaterial().getRefractionFactor());
                        vb.clear();
                        vb.setPrimitiveType(m_reflNormals[i].getAllVertices().getPrimitiveType());
                        for (unsigned int j = 0; j < m_reflNormals[i].getAllVertices().getVertexCount(); j++) {
                            vb.append(m_reflNormals[i].getAllVertices()[j]);
                        }
                        vb.update();
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.texture = m_reflNormals[i].getMaterial().getTexture();
                        currentStates.shader = &sBuildReflectRefractNormal;
                        reflectRefractBuffer.drawVertexBuffer(vb, currentStates);
                    }
                }

                depthBuffer.display();
                reflectRefractBuffer.display();
                for (unsigned int i = 0; i < m_reflInstances.size(); i++) {
                    if (m_reflInstances[i].getAllVertices().getVertexCount() > 0) {
                        Entity* reflectEntity = m_reflInstances[i].getVertexArrays()[0]->getEntity()->getRootEntity();
                        View reflectView(reflectEntity->getSize().x, reflectEntity->getSize().y,0, 1000);
                        reflectView.setCenter(reflectEntity->getCenter());
                        math::Vec3f reflDir = reflectEntity->getRefractDir();
                        math::Vec3f target = reflectView.getPosition() + reflDir;
                        reflectView.lookAt(target.x, target.y, target.z);
                        std::vector<Entity*> visibleReflEntities = World::getEntitiesInRect(view.getViewVolume(), expression);
                        std::vector<Entity*> copy;
                        for (unsigned int j = 0; j < visibleReflEntities.size(); j++)  {
                            if (visibleReflEntities[j]->getRootEntity() != reflectEntity) {
                                copy.push_back(visibleReflEntities[j]);
                            }
                        }
                        ppll.loadEntitiesOnComponent(copy);
                        ppll.setView(view);
                        ppll.clear();
                        ppll.drawNextFrame();
                        viewMatrix = view.getViewMatrix().getMatrix().transpose();
                        projMatrix = view.getProjMatrix().getMatrix().transpose();
                        sReflectRefract.setParameter("viewMatrix", viewMatrix);
                        sReflectRefract.setParameter("projectionMatrix", projMatrix);
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
                                vb.append((*m_reflInstances[i].getVertexArrays()[0])[j]);
                            }
                            vb.update();
                        }
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.texture = m_reflInstances[i].getMaterial().getTexture();
                        currentStates.shader = &sReflectRefract;
                        frameBuffer.drawInstanced(vb, m_reflInstances[i].getVertexArrays()[0]->getPrimitiveType(), 0, m_reflInstances[i].getVertexArrays()[0]->getVertexCount(), tm.size(), currentStates, vboWorldMatrices);
                    }
                }
                for (unsigned int i = 0; i < m_reflNormals.size(); i++) {
                    if (m_reflNormals[i].getAllVertices().getVertexCount() > 0) {
                        Entity* reflectEntity = m_reflNormals[i].getVertexArrays()[0]->getEntity()->getRootEntity();
                        View reflectView(reflectEntity->getSize().x, reflectEntity->getSize().y,0, 1000);
                        reflectView.setCenter(reflectEntity->getCenter());
                        math::Vec3f reflDir = reflectEntity->getRefractDir();
                        math::Vec3f target = reflectView.getPosition() + reflDir;
                        reflectView.lookAt(target.x, target.y, target.z);
                        std::vector<Entity*> visibleReflEntities = World::getEntitiesInRect(view.getViewVolume(), expression);
                        std::vector<Entity*> copy;
                        for (unsigned int j = 0; j < visibleReflEntities.size(); j++)  {
                            if (visibleReflEntities[j]->getRootEntity() != reflectEntity) {
                                copy.push_back(visibleReflEntities[j]);
                            }
                        }
                        ppll.loadEntitiesOnComponent(copy);
                        ppll.setView(view);
                        ppll.clear();
                        ppll.drawNextFrame();
                        viewMatrix = view.getViewMatrix().getMatrix().transpose();
                        projMatrix = view.getProjMatrix().getMatrix().transpose();
                        sReflectRefractNormal.setParameter("viewMatrix", viewMatrix);
                        sReflectRefractNormal.setParameter("projectionMatrix", projMatrix);
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
                        for (unsigned int j = 0; j < m_reflNormals[i].getAllVertices().getVertexCount(); j++) {
                            vb.append(m_reflNormals[i].getAllVertices()[j]);
                        }
                        vb.update();
                        currentStates.blendMode = sf::BlendNone;
                        currentStates.shader = &sReflectRefractNormal;
                        currentStates.texture = m_reflNormals[i].getMaterial().getTexture();
                        frameBuffer.drawVertexBuffer(vb, currentStates);
                    }
                }
                frameBuffer.display();
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
            glDeleteBuffers(1, &vboWorldMatrices);
        }
    }
}
