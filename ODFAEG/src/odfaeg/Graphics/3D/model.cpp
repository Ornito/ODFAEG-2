#include "../../../../include/odfaeg/Graphics/3D/model.hpp"
namespace odfaeg {
    namespace graphic {
        namespace g3d {
            Model::Model ()  {
                float maxF = std::numeric_limits<float>::max();
                float minF = std::numeric_limits<float>::min();
                min = math::Vec3f(maxF, maxF, maxF);
                max = math::Vec3f(minF, minF, minF);
            }
            Entity* Model::loadModel(std::string path) {
                Mesh* emesh = new Mesh(math::Vec3f(0, 0, 0), math::Vec3f(0, 0, 0),"E_MESH");
                Assimp::Importer importer;
                const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
                if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
                {
                    std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                    return emesh;
                }
                directory = path.substr(0, path.find_last_of('/'));
                processNode(scene->mRootNode, scene, emesh);
                return emesh;
            }
            void Model::processNode(aiNode *node, const aiScene *scene, Mesh* emesh)
            {

                // process all the node's meshes (if any)
                for(unsigned int i = 0; i < node->mNumMeshes; i++)
                {
                    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                    processMesh(mesh, scene, emesh);
                }
                // then do the same for each of its children
                for(unsigned int i = 0; i < node->mNumChildren; i++)
                {
                    processNode(node->mChildren[i], scene, emesh);
                }
            }
            void Model::processMesh(aiMesh *mesh, const aiScene *scene, Mesh* emesh) {

                Material mat;
                mat.clearTextures();
                if(mesh->mMaterialIndex >= 0) {
                    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                    std::vector<const Texture*> diffuseMaps = loadMaterialTextures(material,
                                                        aiTextureType_DIFFUSE, "texture_diffuse");
                    std::vector<const Texture*> specularMaps = loadMaterialTextures(material,
                                                        aiTextureType_SPECULAR, "texture_specular");
                    for (unsigned int i = 0; i < diffuseMaps.size(); i++) {
                        mat.addTexture(diffuseMaps[i], sf::IntRect(0, 0, 0, 0));
                    }
                    for (unsigned int i = 0; i < specularMaps.size(); i++) {
                        mat.addTexture(specularMaps[i], sf::IntRect(0, 0, 0, 0));
                    }
                }
                std::vector<math::Vec3f> verts;
                Face* f = new Face(sf::Triangles, emesh->getTransform());
                f->setMaterial(mat);
                f->getVertexArray().setEntity(emesh);
                for(unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    aiFace face = mesh->mFaces[i];
                    for(unsigned int j = 0; j < face.mNumIndices; j++) {
                        Vertex vertex;
                        vertex.position.x = mesh->mVertices[face.mIndices[j]].x;
                        vertex.position.y = mesh->mVertices[face.mIndices[j]].y;
                        vertex.position.z = mesh->mVertices[face.mIndices[j]].z;
                        vertex.texCoords.x = mesh->mTextureCoords[0][face.mIndices[j]].x * mat.getTexture()->getSize().x;
                        vertex.texCoords.y = mesh->mTextureCoords[0][face.mIndices[j]].y * mat.getTexture()->getSize().y;
                        f->append(vertex,face.mIndices[j]);
                        verts.push_back(math::Vec3f(vertex.position.x, vertex.position.y, vertex.position.z));
                    }
                }
                emesh->addFace(f);
                std::array<std::array<float, 2>, 3> exts = math::Computer::getExtends(verts);
                emesh->setSize(math::Vec3f(exts[0][1] - exts[0][0], exts[1][1] - exts[1][0], exts[2][1] - exts[2][0]));
                if (exts[0][0] < min.x)
                    min.x = exts[0][0];
                if (exts[0][1] > max.x)
                    max.x = exts[0][1];
                if (exts[1][0] < min.y)
                    min.y = exts[1][0];
                if (exts[1][1] > max.y)
                    max.y = exts[1][1];
                if (exts[2][0] < min.z)
                    min.z = exts[2][0];
                if (exts[2][1] > max.z)
                    max.z = exts[2][1];
            }
            std::vector<const Texture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
            {
                std::vector<const Texture*> textures;
                for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
                {
                    aiString str;
                    mat->GetTexture(type, i, &str);
                    bool skip = false;
                    std::string path = directory + "/" + std::string (str.C_Str());
                    std::cout<<"path : "<<path<<std::endl;
                    for(unsigned int j = 0; j < tm.getPaths().size(); j++)
                    {
                        if(tm.getPaths()[j] == path)
                        {
                            textures.push_back(tm.getResourceByAlias(path));
                            skip = true;
                            break;
                        }
                    }
                    if(!skip)
                    {   // if texture hasn't been loaded already, load it
                        tm.fromFileWithAlias(path, path);
                        const Texture* texture = tm.getResourceByAlias(path);
                        /*sf::Image img = texture->copyToImage();
                                for (unsigned int i = 0; i < img.getSize().x; i++) {
                                    for (unsigned int j = 0; j < img.getSize().y; j++) {
                                        if (img.getPixel(i, j).r > 0 || img.getPixel(i, j).g > 0 || img.getPixel(i, j).b > 0)
                                        std::cout<<"pixel : "<<(int) img.getPixel(i, j).r<<" , "<<(int) img.getPixel(i, j).g<<" , "<<(int) img.getPixel(i, j).b<<std::endl;
                                    }
                                }*/
                        textures.push_back(texture);
                    }
                }
                return textures;
            }

        }
    }
}
