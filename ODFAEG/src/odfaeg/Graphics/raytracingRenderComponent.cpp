#include "../../../include/odfaeg/Graphics/raytracingRenderComponent.hpp"
#include "glCheck.h"
#include "../../../include/odfaeg/Math/triangle.h"
namespace odfaeg {
    namespace graphic {
        RaytracingRenderComponent::RaytracingRenderComponent(RenderWindow& window, int layer, std::string expression, window::ContextSettings settings) : HeavyComponent(window, math::Vec3f(window.getView().getPosition().x, window.getView().getPosition().y, layer),
                          math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0),
                          math::Vec3f(window.getView().getSize().x + window.getView().getSize().x * 0.5f, window.getView().getPosition().y + window.getView().getSize().y * 0.5f, layer)),
            view(window.getView()),
            expression(expression),
            quad(math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, window.getSize().y * 0.5f)),
            layer(layer) {
                quad.move(math::Vec3f(-window.getView().getSize().x * 0.5f, -window.getView().getSize().y * 0.5f, 0));
                frameBuffer.create(window.getView().getSize().x, window.getView().getSize().y, settings);
                frameBufferSprite = Sprite(frameBuffer.getTexture(), math::Vec3f(0, 0, 0), math::Vec3f(window.getView().getSize().x, window.getView().getSize().y, 0), sf::IntRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
                frameBuffer.setView(view);
                sf::Vector3i resolution ((int) window.getSize().x, (int) window.getSize().y, window.getView().getSize().z);
                glCheck(glGenTextures(1, &frameBufferTex));
                glCheck(glBindTexture(GL_TEXTURE_2D, frameBufferTex));
                glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.getSize().x, window.getSize().y, 0, GL_RGBA, GL_FLOAT,
                 NULL));
                glCheck(glBindImageTexture(0, frameBufferTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));
                glCheck(glBindTexture(GL_TEXTURE_2D, 0));
                std::vector<GLfloat> texClearBuf(window.getView().getSize().x*window.getView().getSize().y*4, 0);
                glCheck(glGenBuffers(1, &clearBuf));
                glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf));
                glCheck(glBufferData(GL_PIXEL_UNPACK_BUFFER, texClearBuf.size() * sizeof(GLfloat),
                &texClearBuf[0], GL_STATIC_COPY));
                glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
                glCheck(glGenBuffers(1, &trianglesSSBO));
                glCheck(glGenBuffers(1, &lightsSSBO));
                external.setNativeHandle(frameBufferTex, window.getView().getSize().x, window.getView().getSize().y);
                core::FastDelegate<bool> signal (&RaytracingRenderComponent::needToUpdate, this);
                core::FastDelegate<void> slot (&RaytracingRenderComponent::drawNextFrame, this);
                core::Command cmd(signal, slot);
                getListener().connect("UPDATE", cmd);
                std::string quadVertexShader = R"(#version 460
                                                        layout (location = 0) in vec3 position;
                                                        layout (location = 1) in vec4 color;
                                                        layout (location = 2) in vec2 texCoords;
                                                        layout (location = 3) in vec3 normals;
                                                        uniform mat4 projectionMatrix;
                                                        uniform mat4 viewMatrix;
                                                        uniform mat4 worldMat;
                                                        uniform mat4 textureMatrix;
                                                        out vec2 fTexCoords;
                                                        out vec4 frontColor;
                                                        void main () {
                                                            gl_Position = projectionMatrix * viewMatrix * worldMat * vec4(position, 1.f);
                                                            fTexCoords = (textureMatrix * vec4(texCoords, 1.f, 1.f)).xy;
                                                            frontColor = color;
                                                        })";
                std::string quadFragmentShader = R"(#version 460
                                                        uniform sampler2D texture;
                                                        in vec4 frontColor;
                                                        in vec2 fTexCoords;
                                                        layout (location = 0) out vec4 fcolor;
                                                        void main() {
                                                            vec4 texel = texture2D(texture, fTexCoords.xy) * frontColor;
                                                            fcolor = texel;
                                                        }
                                                    )";
                std::string raytracingShaderCode = R"(#version 460
                                                      #extension GL_ARB_bindless_texture : enable
                                                      #define MAX_FRAGMENTS 20
                                                      struct Triangle {
                                                          vec3 position[3];
                                                          vec4 color[3];
                                                          vec2 texCoords[3];
                                                          vec3 normal;
                                                          uint textureIndex;
                                                          uint refractReflect;
                                                          float ratio;
                                                          mat4 transform;
                                                          mat4 textureMatrix;
                                                      };
                                                      struct Light {
                                                          vec3 center;
                                                          vec4 color;
                                                          float radius;
                                                      };
                                                      struct Pixel {
                                                          vec3 position;
                                                          vec4 color;
                                                          vec2 texCoords;
                                                      };
                                                      struct Ray {
                                                          vec3 origin;
                                                          vec3 dir;
                                                      };
                                                      layout(local_size_x = 1, local_size_y = 1) in;
                                                      layout(rgba32f, binding = 0) uniform image2D img_output;
                                                      layout(binding = 0) uniform ALL_TEXTURES {
                                                          sampler2D textures[200];
                                                      };
                                                      layout(binding = 0, std430) buffer trianglesBuffer {
                                                          Triangle triangles[];
                                                      };
                                                      layout(binding = 1, std430) buffer lightsBuffer {
                                                          Light lights[];
                                                      };
                                                      uniform uint nbLights;
                                                      uniform uint nbTriangles;
                                                      uniform vec3 cameraPos;
                                                      uniform vec3 resolution;
                                                      uniform mat4 viewMatrix;
                                                      uniform mat4 projMatrix;
                                                      uniform mat4 viewportMatrix;
                                                       vec4 castReflectionRay(Pixel currentPixel, uint triangle);
                                                      vec4 castRefractionRay(Pixel currentPixel, uint triangle);
                                                      bool intersects(Ray ray, Triangle triangle, vec3 intersection) {
                                                         vec3 r = ray.dir;
                                                         vec3 u = triangle.position[1] - triangle.position[0];
                                                         vec3 v = triangle.position[2] - triangle.position[0];
                                                         vec3 n = cross(u, v);
                                                         vec3 otr = ray.origin - triangle.position[0];
                                                         vec3 point;
                                                         if (dot(n, r) == 0)
                                                            return false;
                                                         point.x = dot(n, otr) / dot(n, r);
                                                         point.y = dot(cross(-otr, u), r) / dot(n, r);
                                                         point.z = dot(cross(v, otr), r) / dot(n, r);
                                                         vec3 p1 = triangle.position[0];
                                                         if (0 <= point.x
                                                               && 0 <= point.y && point.y <= 1
                                                               &&  0 <= point.z && point.z <= 1
                                                               && point.y + point.z <= 1) {
                                                               intersection.x = p1.x + u.x * point.z + v.x * point.y;
                                                               intersection.y = p1.y + u.y * point.z + v.y * point.y;
                                                               intersection.z = p1.z + u.z * point.z + v.z * point.y;
                                                               return true;
                                                         }
                                                         intersection = vec3(0, 0, 0);
                                                         return false;
                                                      }
                                                      bool intersects (Ray ray, Light light) {
                                                          vec3 omc = ray.origin - light.center;
                                                          float b = dot(ray.dir, omc);
                                                          float c = dot(omc, omc) - light.radius * light.radius;
                                                          float bsqmc = b * b - c;
                                                          if (bsqmc >= 0)
                                                            return true;
                                                          return false;
                                                      }
                                                      Pixel interpolate(Triangle triangle, vec2 p) {
                                                           Pixel pixel;
                                                           vec3 p1 = triangle.position[0];
                                                           vec3 p2 = triangle.position[1];
                                                           vec3 p3 = triangle.position[2];
                                                           mat2 m1 = mat2(p1.x - p3.x, p2.x - p3.x,
                                                                    p1.y - p3.y, p2.y - p3.y);
                                                           float u = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / determinant(m1);
                                                           float v = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / determinant(m1);
                                                           float w = 1 - u - v;
                                                           vec3 z = vec3 (p1.z, p2.z, p3.z);
                                                           float bz = z.x * u + z.y * v + z.z * w;
                                                           pixel.position = vec3(p, bz);
                                                           vec3 r = vec3(triangle.color[0].r / 255.f, triangle.color[1].r / 255.f, triangle.color[2].r / 255.f);
                                                           vec3 g = vec3(triangle.color[0].g / 255.f, triangle.color[1].g / 255.f, triangle.color[2].g / 255.f);
                                                           vec3 b = vec3(triangle.color[0].b / 255.f, triangle.color[1].b / 255.f, triangle.color[2].b / 255.f);
                                                           vec3 a = vec3(triangle.color[0].a / 255.f, triangle.color[1].a / 255.f, triangle.color[2].a / 255.f);
                                                           vec4 color = vec4 (r.x * u + r.y * v + r.z * w,
                                                                            g.x * u + g.y * v + g.z * w,
                                                                            b.x * u + b.y * v + b.z * w,
                                                                            a.x * u + a.y * v + a.z * w);
                                                           vec2 tc1 = (triangle.textureMatrix * vec4(triangle.texCoords[0], 1.f, 1.f)).xy;
                                                           vec2 tc2 = (triangle.textureMatrix * vec4(triangle.texCoords[1], 1.f, 1.f)).xy;
                                                           vec2 tc3 = (triangle.textureMatrix * vec4(triangle.texCoords[2], 1.f, 1.f)).xy;
                                                           pixel.texCoords = vec2(tc1.x * u + tc2.x * v + tc3.x * w,
                                                                                  tc1.y * u + tc2.y * v + tc3.y * w);
                                                           pixel.color = vec4(1, 0, 0 ,1);/*(triangle.textureIndex > 0) ? color * texture2D(textures[triangle.textureIndex], pixel.texCoords) : color;*/
                                                           return pixel;
                                                      }
                                                      vec4 computeLightAndShadow(Pixel currentPixel, vec3 n, uint triangle) {
                                                           vec4 lightColor = vec4(1, 1, 1, 1);
                                                           int count = 0;
                                                           bool isInShadow=false;
                                                           float maxAlpha = 0;
                                                           Pixel pixels[MAX_FRAGMENTS];
                                                           vec3 i = currentPixel.position - cameraPos;
                                                           vec3 dir = i + dot (i, n) * 2;
                                                           Ray toLight;
                                                           toLight.origin = currentPixel.position;
                                                           toLight.dir = dir;
                                                           vec4 visibility=vec4(1, 1, 1, 1);
                                                           for (uint l = 0; l < nbLights; l++) {
                                                                if (intersects(toLight, lights[l])) {
                                                                    vec3 vertexToLight = lights[l].center - currentPixel.position;
                                                                    float attenuation = 1.f - length(vertexToLight) / lights[l].radius;
                                                                    vec3 dirToLight = normalize(vertexToLight.xyz);
                                                                    float nDotl = dot (dirToLight, n);
                                                                    attenuation *= nDotl;
                                                                    lightColor *= lights[l].color * max(0.0, attenuation);
                                                                    Ray shadowRay;
                                                                    shadowRay.origin = lights[l].center;
                                                                    shadowRay.dir = -toLight.dir;
                                                                    vec3 shadowRayIntersection;
                                                                    for (uint i = 0; i < nbTriangles; i++) {
                                                                        if (i != triangle) {
                                                                            for (uint j = 0; j < 3; j++) {
                                                                                triangles[i].position[j] = (viewportMatrix * projMatrix * viewMatrix * triangles[i].transform * vec4(triangles[i].position[j], 1)).xyz;
                                                                            }
                                                                            if (intersects(shadowRay, triangles[i], shadowRayIntersection)) {
                                                                                pixels[count] = interpolate(triangles[i], shadowRayIntersection.xy);
                                                                                //count++;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                for (uint p = 0; p < count; p++) {
                                                                    if (pixels[p].position.z > currentPixel.position.z) {
                                                                        isInShadow = true;
                                                                        if (pixels[p].color.a > maxAlpha);
                                                                            maxAlpha = pixels[p].color.a;

                                                                    }
                                                                }
                                                           }
                                                           if (isInShadow) {
                                                               visibility = vec4(0.5, 0.5, 0.5, 1-maxAlpha);
                                                           }
                                                           return visibility * lightColor;
                                                      }
                                                      vec4 blendAlpha(Pixel depthPixels[MAX_FRAGMENTS], int count) {
                                                          //merge sort
                                                          int i, j1, j2, k;
                                                          int a, b, c;
                                                          int step = 1;
                                                          Pixel leftArray[MAX_FRAGMENTS/2]; //for merge sort
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
                                                                      leftArray[k] = depthPixels[a + k];
                                                                  j1 = 0;
                                                                  j2 = 0;
                                                                  for (k = a; k < c; k++)
                                                                  {
                                                                      if (b + j1 >= c || (j2 < step && leftArray[j2].position.z > depthPixels[b + j1].position.z))
                                                                          depthPixels[k] = leftArray[j2++];
                                                                      else
                                                                          depthPixels[k] = depthPixels[b + j1++];
                                                                  }
                                                                  ////////////////////////////////////////////////////////////////////////
                                                                  i += 2 * step;
                                                              }
                                                              step *= 2;
                                                          }
                                                          vec4 color = vec4(0, 0, 0, 0);
                                                          for( int i = count - 1; i >= 0; i--)
                                                          {
                                                            color.rgb = depthPixels[i].color.rgb * depthPixels[i].color.a + color.rgb * (1 - depthPixels[i].color.a);
                                                            color.a = depthPixels[i].color.a + color.a * (1 - depthPixels[i].color.a);
                                                          }
                                                          return color;
                                                      }
                                                      vec4 castRefractionRay(Pixel currentPixel, uint triangle) {
                                                          vec4 color;
                                                          vec3 I = currentPixel.position - cameraPos;
                                                          int count = 0;
                                                          Pixel depthPixels[MAX_FRAGMENTS];
                                                          for (unsigned int i = 0; i < nbTriangles; i++) {
                                                                if (i != triangle) {
                                                                    for (uint j = 0; j < 3; j++) {
                                                                        triangles[i].position[j] = (viewportMatrix * projMatrix * viewMatrix * triangles[i].transform * vec4(triangles[i].position[j], 1)).xyz;
                                                                    }
                                                                    vec3 n = triangles[i].normal;
                                                                    float k = 1.0 - triangles[i].ratio * triangles[i].ratio * (1.0 - dot(n, I) * dot(n, I));
                                                                    vec3 r;
                                                                    if (k < 0.0) {
                                                                        r = vec3(0, 0, 0);
                                                                    } else {
                                                                        r = triangles[i].ratio * I - (triangles[i].ratio * dot(n, I) + sqrt(k)) * n;
                                                                    }
                                                                    Ray ray;
                                                                    ray.origin = currentPixel.position;
                                                                    ray.dir = r;
                                                                    vec3 intersection;
                                                                    if (intersects(ray, triangles[i],intersection)) {
                                                                        vec3 intersection;
                                                                        vec4 reflectRefractColor = vec4(1, 1, 1, 1);
                                                                        vec4 lightShadowColor = vec4(1, 1, 1, 1);
                                                                        Pixel p;
                                                                        if (intersects(ray, triangles[i],intersection)) {
                                                                            p = interpolate(triangles[i], intersection.xy);
                                                                            lightShadowColor = computeLightAndShadow(p, triangles[i].normal, i);
                                                                            /*if (triangles[i].refractReflect == 1) {
                                                                                reflectRefractColor = castReflectionRay(p, i);
                                                                            }
                                                                            if (triangles[i].refractReflect == 2) {
                                                                                reflectRefractColor = castRefractionRay(p, i);
                                                                            }
                                                                            if (triangles[i].refractReflect == 3) {
                                                                                vec4 reflectColor = castReflectionRay(p, i);
                                                                                vec4 refractColor = castRefractionRay(p, i);
                                                                                color = reflectColor * refractColor;
                                                                            }*/
                                                                        }
                                                                        p.color *= lightShadowColor;
                                                                        depthPixels[count] = p;
                                                                        count++;
                                                                    }
                                                               }
                                                          }
                                                          color = blendAlpha(depthPixels, count);
                                                          return color;
                                                      }
                                                      vec4 castReflectionRay(Pixel currentPixel, uint triangle) {
                                                          vec4 color;
                                                          vec3 I = currentPixel.position - cameraPos;
                                                          int count = 0;
                                                          Pixel depthPixels[MAX_FRAGMENTS];

                                                          for (unsigned int i = 0; i < nbTriangles; i++) {
                                                                if (i != triangle) {
                                                                    for (uint j = 0; j < 3; j++) {
                                                                        triangles[i].position[j] = (viewportMatrix * projMatrix * viewMatrix * triangles[i].transform * vec4(triangles[i].position[j], 1)).xyz;
                                                                    }
                                                                    vec3 n = triangles[i].normal;
                                                                    vec3 r = I - 2.0 * dot(I, n) * n;
                                                                    Ray ray;
                                                                    ray.origin = currentPixel.position;
                                                                    ray.dir = r;
                                                                    vec3 intersection;
                                                                    vec4 lightShadowColor = vec4(1, 1, 1, 1);
                                                                    vec4 reflectRefractColor = vec4(1, 1, 1, 1);
                                                                    Pixel p;
                                                                    if (intersects(ray, triangles[i],intersection)) {
                                                                        p = interpolate(triangles[i], intersection.xy);
                                                                        //lightShadowColor = computeLightAndShadow(p, triangles[i].normal, i);
                                                                        /*if (triangles[i].refractReflect == 1) {
                                                                            reflectRefractColor = castReflectionRay(p, i);
                                                                        }
                                                                        if (triangles[i].refractReflect == 2) {
                                                                            reflectRefractColor = castRefractionRay(p, i);
                                                                        }
                                                                        if (triangles[i].refractReflect == 3) {
                                                                            vec4 reflectColor = castReflectionRay(p, i);
                                                                            vec4 refractColor = castRefractionRay(p, i);
                                                                            reflectRefractColor = reflectColor * refractColor;
                                                                        }*/
                                                                    }
                                                                    p.color *= lightShadowColor;
                                                                    //depthPixels[count] = p;
                                                                    count++;
                                                                }
                                                          }
                                                          color = blendAlpha(depthPixels, count);
                                                          return color;
                                                      }
                                                      void main() {
                                                          vec4 pixel = vec4(1.0, 0.0, 0.0, 1.0);
                                                          ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
                                                          Ray ray;
                                                          float max_x = resolution.x * 0.5;
                                                          float max_y = resolution.y * 0.5;
                                                          ivec2 dims = imageSize(img_output);
                                                          float x = (float(pixel_coords.x * 2 - dims.x) / dims.x);
                                                          float y = (float(pixel_coords.y * 2 - dims.y) / dims.y);
                                                          ray.origin = vec3(resolution.x * 0.5, resolution.y * 0.5, 0);
                                                          vec3 ext = vec3 (pixel_coords.xy, resolution.z * 0.5);
                                                          ray.dir = ray.origin - ext;
                                                          Pixel depthPixels[MAX_FRAGMENTS];
                                                          Pixel currentPixel;
                                                          int count = 0;
                                                          for (uint i = 0; i < nbTriangles; i++) {
                                                               for (uint j = 0; j < 3; j++) {
                                                                    vec4 t = projMatrix * viewMatrix * triangles[i].transform * vec4(triangles[i].position[j], 1);
                                                                    t /= t.w;
                                                                    triangles[i].position[j] = (viewportMatrix * t).xyz;
                                                               }
                                                               vec3 intersection;
                                                               if (intersects(ray, triangles[i], intersection)) {
                                                                   currentPixel = interpolate(triangles[i], pixel_coords);
                                                                   //vec4 shadowLightColor = computeLightAndShadow (currentPixel, triangles[i].normal, i);
                                                                   vec4 reflectRefractColor=vec4(1, 1, 1, 1);
                                                                   /*if (triangles[i].refractReflect == 1) {
                                                                       reflectRefractColor = castReflectionRay(currentPixel, i);
                                                                   }
                                                                   if (triangles[i].refractReflect == 2) {
                                                                       reflectRefractColor = castRefractionRay(currentPixel, i);
                                                                   }
                                                                   if (triangles[i].refractReflect == 3) {
                                                                       vec4 reflectColor = castReflectionRay(currentPixel, i);
                                                                       vec4 refractColor = castRefractionRay(currentPixel, i);
                                                                       reflectRefractColor = reflectColor * refractColor;
                                                                   }*/
                                                                   currentPixel.color = currentPixel.color /** reflectRefractColor * shadowLightColor*/;
                                                                   /*depthPixels[count] = currentPixel;
                                                                   if (count < MAX_FRAGMENTS)
                                                                       count++;*/
                                                                   vec4 color = blendAlpha (depthPixels, count);
                                                                   imageStore(img_output, pixel_coords, currentPixel.color);
                                                               }

                                                          }

                                                      }
                                                      )";
                    if (!rayComputeShader.loadFromMemory(raytracingShaderCode, Shader::Compute)) {
                        throw core::Erreur(60, "failed to load raytracing compute shader!", 3);
                    }
                    if (!quadShader.loadFromMemory(quadVertexShader, quadFragmentShader)) {
                        throw core::Erreur(61, "failed to load quad shader!", 3);
                    }
                    rayComputeShader.setParameter("resolution", resolution.x, resolution.y, resolution.z);
                    math::Matrix4f viewMatrix = window.getDefaultView().getViewMatrix().getMatrix().transpose();
                    math::Matrix4f projMatrix = window.getDefaultView().getProjMatrix().getMatrix().transpose();
                    quadShader.setParameter("viewMatrix", viewMatrix);
                    quadShader.setParameter("projectionMatrix", projMatrix);
                    quadShader.setParameter("worldMat", quad.getTransform().getMatrix().transpose());
                    quadShader.setParameter("texture", Shader::CurrentTexture);
                    quadShader.setParameter("textureMatrix", external.getTextureMatrix());
                    /*std::vector<Texture*> allTextures = Texture::getAllTextures();
                    for (unsigned int i = 0; i < allTextures.size(); i++) {

                        GLuint64 handle_texture = glGetTextureHandleARB(allTextures[i]->getNativeHandle());
                        glCheck(glMakeTextureHandleResidentARB(handle_texture));
                        glCheck(glProgramUniformHandleui64ARB(rayComputeShader.getHandle(), allTextures[i]->getNativeHandle(), handle_texture));
                    }*/
                    backgroundColor = sf::Color::Transparent;
                    /*glCheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, trianglesSSBO));
                    glCheck(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightsSSBO));*/
            }
            void RaytracingRenderComponent::setBackgroundColor(sf::Color color) {
                backgroundColor = color;
            }
            void RaytracingRenderComponent::clear() {
                frameBuffer.clear(sf::Color::Transparent);
                glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf));
                glCheck(glBindTexture(GL_TEXTURE_2D, frameBufferTex));
                glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, view.getSize().x, view.getSize().y, GL_RGBA,
                GL_FLOAT, NULL));
                glCheck(glBindTexture(GL_TEXTURE_2D, 0));
                glCheck(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
                frameBuffer.resetGLStates();
            }
            void RaytracingRenderComponent::drawNextFrame() {
                if (frameBuffer.getSettings().versionMajor >= 4 && frameBuffer.getSettings().versionMinor >= 6) {
                    /*math::Vec3f origin(getView().getSize().x * 0.5, getView().getSize().y * 0.5, 0);
                    for (unsigned int i = 0; i < getView().getSize().x; i++) {
                        for (unsigned int j = 0; j < getView().getSize().y; j++) {
                            math::Vec3f ext(i, j, getView().getSize().z*0.5);
                            math::Ray ray(origin, ext);
                            for (unsigned int t = 0; t < triangles.size(); t++) {
                                math::Vec3f p1 (triangles[t].positions[0].x, triangles[t].positions[0].y,triangles[t].positions[0].z);
                                math::Vec3f p2 (triangles[t].positions[1].x, triangles[t].positions[1].y,triangles[t].positions[1].z);
                                math::Vec3f p3 (triangles[t].positions[2].x, triangles[t].positions[2].y,triangles[t].positions[2].z);
                                p1 = triangles[i].transform * p1;
                                p2 = triangles[i].transform * p2;
                                p3 = triangles[i].transform * p3;
                                p1 = frameBuffer.mapCoordsToPixel(math::Vec3f(p1.x, getView().getSize().y - p1.y, p1.z));
                                p2 = frameBuffer.mapCoordsToPixel(math::Vec3f(p2.x, getView().getSize().y - p2.y, p2.z));
                                p3 = frameBuffer.mapCoordsToPixel(math::Vec3f(p3.x, getView().getSize().y - p3.y, p3.z));
                                math::Triangle trg(p1, p2, p3);

                                if (trg.intersects(ray))
                                    std::cout<<"intersects ? : "<<trg.intersects(ray)<<std::endl;
                            }
                        }
                    }*/
                    //std::cout<<"size : "<<triangles.size()<<std::endl;
                    glCheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesSSBO));

                    glCheck(glBufferData(GL_SHADER_STORAGE_BUFFER, triangles.size() * 248, NULL, GL_DYNAMIC_DRAW));
                    GLvoid* p = nullptr;
                    glCheck(p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
                    memcpy(p, &triangles[0], triangles.size() * 248);
                    glCheck(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
                    glCheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO));
                    glCheck(glBufferData(GL_SHADER_STORAGE_BUFFER, lights.size() * 32, NULL, GL_DYNAMIC_DRAW));
                    GLvoid* p2 = nullptr;
                    glCheck(p2 = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
                    memcpy(p2, &lights[0], lights.size() * 32);
                    glCheck(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
                    glCheck(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
                    math::Matrix4f viewMatrix = view.getViewMatrix().getMatrix().transpose();
                    math::Matrix4f projMatrix = view.getProjMatrix().getMatrix().transpose();
                    ViewportMatrix vpm;
                    vpm.setViewport(math::Vec3f(view.getViewport().getPosition().x, view.getViewport().getPosition().y, 0),
                    math::Vec3f(view.getViewport().getWidth(), view.getViewport().getHeight(), 1));
                    math::Matrix4f viewportMatrix = vpm.getMatrix().transpose();
                    rayComputeShader.setParameter("viewMatrix", viewMatrix);
                    rayComputeShader.setParameter("projMatrix", projMatrix);
                    rayComputeShader.setParameter("viewportMatrix", viewportMatrix);
                    rayComputeShader.setParameter("nbTriangles", triangles.size());
                    rayComputeShader.setParameter("nbLights", lights.size());
                    Shader::bind(&rayComputeShader);
                    glCheck(glDispatchCompute(view.getSize().x, view.getSize().y, 1));
                    // make sure writing to image has finished before read
                    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
                    RenderStates states;
                    states.shader = &quadShader;
                    states.transform = quad.getTransform();
                    states.texture = &external;
                    vb.clear();
                    vb.setPrimitiveType(sf::Quads);
                    Vertex v1 (sf::Vector3f(0, 0, quad.getSize().z));
                    Vertex v2 (sf::Vector3f(quad.getSize().x,0, quad.getSize().z));
                    Vertex v3 (sf::Vector3f(quad.getSize().x, quad.getSize().y, quad.getSize().z));
                    Vertex v4 (sf::Vector3f(0, quad.getSize().y, quad.getSize().z));
                    vb.append(v1);
                    vb.append(v2);
                    vb.append(v3);
                    vb.append(v4);
                    vb.update();
                    frameBuffer.drawVertexBuffer(vb, states);
                    frameBuffer.display();
                }
            }
            bool RaytracingRenderComponent::loadEntitiesOnComponent(std::vector<Entity*> vEntities) {
                triangles.clear();
                lights.clear();
                Light ambientLight;
                g2d::AmbientLight al = g2d::AmbientLight::getAmbientLight();
                ambientLight.center = sf::Vector3f(al.getLightCenter().x, al.getLightCenter().y, al.getLightCenter().z);
                ambientLight.radius = 10000;
                ambientLight.color = math::Vec3f(al.getColor().r/255.f, al.getColor().g/255.f, al.getColor().b/255.f, al.getColor().a/255.f);
                lights.push_back(ambientLight);
                unsigned int nbVertices =0;
                for (unsigned int e = 0; e < vEntities.size(); e++) {
                    if ( vEntities[e]->isLeaf()) {
                        if (!vEntities[e]->isLight()) {
                            for (unsigned int j = 0; j <  vEntities[e]->getNbFaces(); j++) {
                                Material material = vEntities[e]->getFace(j)->getMaterial();
                                VertexArray va = vEntities[e]->getFace(j)->getVertexArray();
                                unsigned int size = 0;
                                if (va.getPrimitiveType() == sf::PrimitiveType::Quads) {
                                    size = va.getVertexCount() / 4;
                                } else if (va.getPrimitiveType() == sf::PrimitiveType::Triangles) {
                                    size = va.getVertexCount() / 3;
                                } else if (va.getPrimitiveType() == sf::PrimitiveType::TriangleStrip || va.getPrimitiveType() == sf::PrimitiveType::TriangleFan) {
                                    size = va.getVertexCount() - 2;
                                }
                                for (unsigned int i = 0; i < size; i++) {
                                    if (va.getPrimitiveType() == sf::PrimitiveType::Quads) {
                                        for (unsigned int n = 0; n < 2; n++) {
                                            if (n == 0) {
                                                Triangle triangle;
                                                triangle.positions[0] = va[i*4].position;
                                                triangle.positions[1] = va[i*4+1].position;
                                                triangle.positions[2] = va[i*4+2].position;
                                                triangle.colours[0] = math::Vec3f(va[i*4].color.r / 255.f,va[i*4].color.g / 255.f,va[i*4].color.b / 255.f, va[i*4].color.a / 255.f);
                                                triangle.colours[1] = math::Vec3f(va[i*4+1].color.r / 255.f,va[i*4+1].color.g / 255.f,va[i*4+1].color.b / 255.f, va[i*4+1].color.a / 255.f);
                                                triangle.colours[2] = math::Vec3f(va[i*4+2].color.r / 255.f,va[i*4+2].color.g / 255.f,va[i*4+2].color.b / 255.f, va[i*4+2].color.a / 255.f);
                                                triangle.texCoords[0] = va[i*4].texCoords;
                                                triangle.texCoords[1] = va[i*4+1].texCoords;
                                                triangle.texCoords[2] = va[i*4+2].texCoords;
                                                /*for (unsigned int v = 0; v < 3; v++) {
                                                    std::cout<<va[i*4+v].position.x<<","<<va[i*4+v].position.y<<","<<va[i*4+v].position.z<<std::endl;
                                                }*/
                                                math::Vec3f v1(va[i*4].position.x, va[i*4].position.y, va[i*4].position.z);
                                                math::Vec3f v2(va[i*4+1].position.x, va[i*4+1].position.y, va[i*4+1].position.z);
                                                math::Vec3f v3(va[i*4+2].position.x, va[i*4+2].position.y, va[i*4+2].position.z);
                                                math::Vec3f d1 = v2 - v1;
                                                math::Vec3f d2 = v3 - v1;
                                                math::Vec3f n = d1.cross(d2).normalize();
                                                triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                                triangle.ratio = material.getRefractionFactor();
                                                math::Matrix4f m;
                                                triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                                triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                                triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                                if (!material.isReflectable() && !material.isRefractable())
                                                    triangle.refractReflect = 0;
                                                else if (material.isReflectable() && !material.isRefractable())
                                                    triangle.refractReflect = 1;
                                                else if (!material.isReflectable() && material.isRefractable())
                                                    triangle.refractReflect = 2;
                                                else
                                                    triangle.refractReflect = 3;
                                                triangles.push_back(triangle);
                                            } else {
                                                Triangle triangle;
                                                triangle.positions[0] = va[i*4+1].position;
                                                triangle.positions[1] = va[i*4+2].position;
                                                triangle.positions[2] = va[i*4+3].position;
                                                triangle.colours[0] = math::Vec3f(va[i*4+1].color.r / 255.f,va[i*4+1].color.g / 255.f,va[i*4+1].color.b / 255.f, va[i*4+1].color.a / 255.f);
                                                triangle.colours[1] = math::Vec3f(va[i*4+2].color.r / 255.f,va[i*4+2].color.g / 255.f,va[i*4+2].color.b / 255.f, va[i*4+2].color.a / 255.f);
                                                triangle.colours[2] = math::Vec3f(va[i*4+3].color.r / 255.f,va[i*4+3].color.g / 255.f,va[i*4+3].color.b / 255.f, va[i*4+3].color.a / 255.f);
                                                triangle.texCoords[0] = va[i*4+1].texCoords;
                                                triangle.texCoords[1] = va[i*4+2].texCoords;
                                                triangle.texCoords[2] = va[i*4+3].texCoords;
                                                /*for (unsigned int v = 0; v < 3; v++) {
                                                    std::cout<<va[i*4+v+1].position.x<<","<<va[i*4+v+1].position.y<<","<<va[i*4+v+1].position.z<<std::endl;
                                                }*/
                                                math::Vec3f v1(va[i*4+1].position.x, va[i*4+1].position.y, va[i*4+1].position.z);
                                                math::Vec3f v2(va[i*4+2].position.x, va[i*4+2].position.y, va[i*4+2].position.z);
                                                math::Vec3f v3(va[i*4+3].position.x, va[i*4+3].position.y, va[i*4+3].position.z);
                                                math::Vec3f d1 = v2 - v1;
                                                math::Vec3f d2 = v3 - v1;
                                                math::Vec3f n = d1.cross(d2).normalize();
                                                triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                                triangle.ratio = material.getRefractionFactor();
                                                math::Matrix4f m;
                                                triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                                triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                                triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                                if (!material.isReflectable() && !material.isRefractable())
                                                    triangle.refractReflect = 0;
                                                else if (material.isReflectable() && !material.isRefractable())
                                                    triangle.refractReflect = 1;
                                                else if (!material.isReflectable() && material.isRefractable())
                                                    triangle.refractReflect = 2;
                                                else
                                                    triangle.refractReflect = 3;
                                                triangles.push_back(triangle);
                                            }
                                        }
                                    } else if (va.getPrimitiveType() == sf::PrimitiveType::Triangles) {
                                        Triangle triangle;
                                        triangle.positions[0] = va[i*3].position;
                                        triangle.positions[1] = va[i*3+1].position;
                                        triangle.positions[2] = va[i*3+2].position;
                                        triangle.colours[0] = math::Vec3f(va[i*3].color.r / 255.f,va[i*3].color.g / 255.f,va[i*3].color.b / 255.f, va[i*3].color.a / 255.f);
                                        triangle.colours[1] = math::Vec3f(va[i*3+1].color.r / 255.f,va[i*3+1].color.g / 255.f,va[i*3+1].color.b / 255.f, va[i*3+1].color.a / 255.f);
                                        triangle.colours[2] = math::Vec3f(va[i*3+2].color.r / 255.f,va[i*3+2].color.g / 255.f,va[i*3+2].color.b / 255.f, va[i*3+2].color.a / 255.f);
                                        triangle.texCoords[0] = va[i*3].texCoords;
                                        triangle.texCoords[1] = va[i*3+1].texCoords;
                                        triangle.texCoords[2] = va[i*3+2].texCoords;

                                        math::Vec3f v1(va[i*3].position.x, va[i*3].position.y, va[i*3].position.z);
                                        math::Vec3f v2(va[i*3+1].position.x, va[i*3+1].position.y, va[i*3+1].position.z);
                                        math::Vec3f v3(va[i*3+2].position.x, va[i*3+2].position.y, va[i*3+2].position.z);
                                        math::Vec3f d1 = v2 - v1;
                                        math::Vec3f d2 = v3 - v1;
                                        math::Vec3f n = d1.cross(d2).normalize();
                                        triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                        triangle.ratio = material.getRefractionFactor();
                                        math::Matrix4f m;
                                        triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                        triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                        triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                        if (!material.isReflectable() && !material.isRefractable())
                                            triangle.refractReflect = 0;
                                        else if (material.isReflectable() && !material.isRefractable())
                                            triangle.refractReflect = 1;
                                        else if (!material.isReflectable() && material.isRefractable())
                                            triangle.refractReflect = 2;
                                        else
                                            triangle.refractReflect = 3;
                                        triangles.push_back(triangle);
                                    } else if (va.getPrimitiveType() == sf::PrimitiveType::TriangleStrip) {
                                        if (i == 0) {
                                            Triangle triangle;
                                            triangle.positions[0] = va[i*3].position;
                                            triangle.positions[1] = va[i*3+1].position;
                                            triangle.positions[2] = va[i*3+2].position;
                                            triangle.colours[0] = math::Vec3f(va[i*3].color.r / 255.f,va[i*3].color.g / 255.f,va[i*3].color.b / 255.f, va[i*3].color.a / 255.f);
                                            triangle.colours[1] = math::Vec3f(va[i*3+1].color.r / 255.f,va[i*3+1].color.g / 255.f,va[i*3+1].color.b / 255.f, va[i*3+1].color.a / 255.f);
                                            triangle.colours[2] = math::Vec3f(va[i*3+2].color.r / 255.f,va[i*3+2].color.g / 255.f,va[i*3+2].color.b / 255.f, va[i*3+2].color.a / 255.f);
                                            triangle.texCoords[0] = va[i*3].texCoords;
                                            triangle.texCoords[1] = va[i*3+1].texCoords;
                                            triangle.texCoords[2] = va[i*3+2].texCoords;

                                            math::Vec3f v1(va[i*3].position.x, va[i*3].position.y, va[i*3].position.z);
                                            math::Vec3f v2(va[i*3+1].position.x, va[i*3+1].position.y, va[i*3+1].position.z);
                                            math::Vec3f v3(va[i*3+2].position.x, va[i*3+2].position.y, va[i*3+2].position.z);
                                            math::Vec3f d1 = v2 - v1;
                                            math::Vec3f d2 = v3 - v1;
                                            math::Vec3f n = d1.cross(d2).normalize();
                                            triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                            triangle.ratio = material.getRefractionFactor();
                                            math::Matrix4f m;
                                            triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                            triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                            triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                            if (!material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 0;
                                            else if (material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 1;
                                            else if (!material.isReflectable() && material.isRefractable())
                                                triangle.refractReflect = 2;
                                            else
                                                triangle.refractReflect = 3;
                                            triangles.push_back(triangle);
                                        } else {
                                            Triangle triangle;
                                            triangle.positions[0] = va[i].position;
                                            triangle.positions[1] = va[i+1].position;
                                            triangle.positions[2] = va[i+2].position;
                                            triangle.colours[0] = math::Vec3f(va[i].color.r / 255.f,va[i].color.g / 255.f,va[i].color.b / 255.f, va[i].color.a / 255.f);
                                            triangle.colours[1] = math::Vec3f(va[i+1].color.r / 255.f,va[i+1].color.g / 255.f,va[i+1].color.b / 255.f, va[i+1].color.a / 255.f);
                                            triangle.colours[2] = math::Vec3f(va[i+2].color.r / 255.f,va[i+2].color.g / 255.f,va[i+2].color.b / 255.f, va[i+2].color.a / 255.f);
                                            triangle.texCoords[0] = va[i].texCoords;
                                            triangle.texCoords[1] = va[i+1].texCoords;
                                            triangle.texCoords[2] = va[i+2].texCoords;

                                            math::Vec3f v1(va[i].position.x, va[i].position.y, va[i].position.z);
                                            math::Vec3f v2(va[i+1].position.x, va[i+1].position.y, va[i+1].position.z);
                                            math::Vec3f v3(va[i+2].position.x, va[i+2].position.y, va[i+2].position.z);
                                            math::Vec3f d1 = v2 - v1;
                                            math::Vec3f d2 = v3 - v1;
                                            math::Vec3f n = d1.cross(d2).normalize();
                                            triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                            triangle.ratio = material.getRefractionFactor();
                                            math::Matrix4f m;
                                            triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                            triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                            triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                            if (!material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 0;
                                            else if (material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 1;
                                            else if (!material.isReflectable() && material.isRefractable())
                                                triangle.refractReflect = 2;
                                            else
                                                triangle.refractReflect = 3;
                                            triangles.push_back(triangle);
                                        }
                                    } else if (va.getPrimitiveType() == sf::TriangleFan) {
                                        if (i == 0) {
                                            Triangle triangle;
                                            triangle.positions[0] = va[i*3].position;
                                            triangle.positions[1] = va[i*3+1].position;
                                            triangle.positions[2] = va[i*3+2].position;
                                            triangle.colours[0] = math::Vec3f(va[i*3].color.r / 255.f,va[i*3].color.g / 255.f,va[i*3].color.b / 255.f, va[i*3].color.a / 255.f);
                                            triangle.colours[1] = math::Vec3f(va[i*3+1].color.r / 255.f,va[i*3+1].color.g / 255.f,va[i*3+1].color.b / 255.f, va[i*3+1].color.a / 255.f);
                                            triangle.colours[2] = math::Vec3f(va[i*3+2].color.r / 255.f,va[i*3+2].color.g / 255.f,va[i*3+2].color.b / 255.f, va[i*3+2].color.a / 255.f);
                                            triangle.texCoords[0] = va[i*3].texCoords;
                                            triangle.texCoords[1] = va[i*3+1].texCoords;
                                            triangle.texCoords[2] = va[i*3+2].texCoords;

                                            math::Vec3f v1(va[i*3].position.x, va[i*3].position.y, va[i*3].position.z);
                                            math::Vec3f v2(va[i*3+1].position.x, va[i*3+1].position.y, va[i*3+1].position.z);
                                            math::Vec3f v3(va[i*3+2].position.x, va[i*3+2].position.y, va[i*3+2].position.z);
                                            math::Vec3f d1 = v2 - v1;
                                            math::Vec3f d2 = v3 - v1;
                                            math::Vec3f n = d1.cross(d2).normalize();
                                            triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                            triangle.ratio = material.getRefractionFactor();
                                            math::Matrix4f m;
                                            triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                            triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                            triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                            if (!material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 0;
                                            else if (material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 1;
                                            else if (!material.isReflectable() && material.isRefractable())
                                                triangle.refractReflect = 2;
                                            else
                                                triangle.refractReflect = 3;
                                            triangles.push_back(triangle);
                                        } else {
                                            Triangle triangle;
                                            triangle.positions[0] = va[0].position;
                                            triangle.positions[1] = va[i+1].position;
                                            triangle.positions[2] = va[i+2].position;
                                            triangle.colours[0] = math::Vec3f(va[0].color.r / 255.f,va[0].color.g / 255.f,va[0].color.b / 255.f, va[0].color.a / 255.f);
                                            triangle.colours[1] = math::Vec3f(va[i+1].color.r / 255.f,va[i+1].color.g / 255.f,va[i+1].color.b / 255.f, va[i+1].color.a / 255.f);
                                            triangle.colours[2] = math::Vec3f(va[i+2].color.r / 255.f,va[i+2].color.g / 255.f,va[i+2].color.b / 255.f, va[i+2].color.a / 255.f);
                                            triangle.texCoords[0] = va[0].texCoords;
                                            triangle.texCoords[1] = va[i+1].texCoords;
                                            triangle.texCoords[2] = va[i+2].texCoords;

                                            math::Vec3f v1(va[0].position.x, va[0].position.y, va[0].position.z);
                                            math::Vec3f v2(va[i+1].position.x, va[i+1].position.y, va[i+1].position.z);
                                            math::Vec3f v3(va[i+2].position.x, va[i+2].position.y, va[i+2].position.z);
                                            math::Vec3f d1 = v2 - v1;
                                            math::Vec3f d2 = v3 - v1;
                                            math::Vec3f n = d1.cross(d2).normalize();
                                            triangle.normal = sf::Vector3f(n.x, n.y, n.z);
                                            triangle.ratio = material.getRefractionFactor();
                                            math::Matrix4f m;
                                            triangle.textureIndex = (material.getTexture() == nullptr) ? 0 : material.getTexture()->getNativeHandle();
                                            triangle.textureMatrix = (material.getTexture() == nullptr) ? m : material.getTexture()->getTextureMatrix();
                                            triangle.transform = vEntities[e]->getTransform().getMatrix().transpose();
                                            if (!material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 0;
                                            else if (material.isReflectable() && !material.isRefractable())
                                                triangle.refractReflect = 1;
                                            else if (!material.isReflectable() && material.isRefractable())
                                                triangle.refractReflect = 2;
                                            else
                                                triangle.refractReflect = 3;
                                            triangles.push_back(triangle);
                                        }
                                    } else {
                                        Light light;
                                        g2d::PonctualLight* pl = static_cast<g2d::PonctualLight*>(vEntities[e]);
                                        light.center = sf::Vector3f(pl->getCenter().x, pl->getCenter().y, pl->getCenter().z);
                                        light.radius = pl->getSize().y * 0.5f;
                                        light.color = math::Vec3f(pl->getColor().r / 255.f,pl->getColor().g/255.f,pl->getColor().b/255.f,pl->getColor().a/255.f);
                                        lights.push_back(light);
                                    }
                                }
                            }
                        }
                    }
                }
                update = true;
                return true;
            }
            void RaytracingRenderComponent::draw(RenderTarget& target, RenderStates states) {
                frameBufferSprite.setCenter(target.getView().getPosition());
                /*states.texture = &external;
                states.transform = quad.getTransform();
                View view = target.getView();
                target.setView(target.getDefaultView());*/
                target.draw(frameBufferSprite, states);
                //target.setView(view);
            }
            bool RaytracingRenderComponent::needToUpdate() {
                return update;
            }
            void RaytracingRenderComponent::setExpression (std::string expression) {
                this->expression = expression;
            }
            void RaytracingRenderComponent::draw(Drawable& drawable, RenderStates states) {
                //drawables.insert(std::make_pair(drawable, states));
            }
            void RaytracingRenderComponent::changeVisibleEntities(Entity* toRemove, Entity* toAdd, EntityManager* em) {
            }
            void RaytracingRenderComponent::setView(View view) {
                frameBuffer.setView(view);
                this->view = view;
            }
            std::vector<Entity*> RaytracingRenderComponent::getEntities() {
                return visibleEntities;
            }
            std::string RaytracingRenderComponent::getExpression() {
                return expression;
            }
            View& RaytracingRenderComponent::getView() {
                return view;
            }
            void RaytracingRenderComponent::updateParticleSystems() {
                /*for (unsigned int i = 0; i < visibleEntities.size(); i++) {
                    if (dynamic_cast<physic::ParticleSystem*>(visibleEntities[i]) != nullptr) {
                        static_cast<physic::ParticleSystem*>(visibleEntities[i])->update();
                    }
                }
                loadEntitiesOnComponent(visibleEntities);
                update = true;*/
            }
            void RaytracingRenderComponent::pushEvent(window::IEvent event, RenderWindow& rw) {
                if (event.type == window::IEvent::WINDOW_EVENT && event.window.type == window::IEvent::WINDOW_EVENT_RESIZED && &getWindow() == &rw && isAutoResized()) {
                    std::cout<<"recompute size"<<std::endl;
                    recomputeSize();
                    getListener().pushEvent(event);
                    getView().reset(physic::BoundingBox(getView().getViewport().getPosition().x, getView().getViewport().getPosition().y, getView().getViewport().getPosition().z, event.window.data1, event.window.data2, getView().getViewport().getDepth()));
                }
            }
            const Texture& RaytracingRenderComponent::getFrameBufferTexture() {
                return frameBuffer.getTexture();
            }
            RaytracingRenderComponent::~RaytracingRenderComponent() {
                glDeleteBuffers(1, &trianglesSSBO);
                glDeleteBuffers(1, &lightsSSBO);
                glDeleteBuffers(1, &clearBuf);
                glDeleteTextures(1, &frameBufferTex);
            }
    }
}
