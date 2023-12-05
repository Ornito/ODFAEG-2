#ifndef ODFAEG_ECS_HPP
#define ODFAEG_ECS_HPP
#include <vector>
#include "../Graphics/ECS/components.hpp"
#include <optional>
#include "entityFactory.hpp"
namespace odfaeg {
    namespace graphic {
        namespace ecs {


            template <typename T>
            std::vector<std::optional<T>>& get(){
                static std::vector<std::optional<T>> tvec;
                return tvec;
            }
            template <typename T>
            void addComponent(EntityId entity, T component, EntityFactory& factory) {
                get<T>().resize(factory.getNbEntities());
                get<T>()[entity] = component;
            }
            template <typename T>
            T* getComponent(EntityId entity) {
                if (entity >= get<T>().size() || !get<T>()[entity].has_value())
                    return nullptr;
                else
                    return &get<T>()[entity].value();
            }
            template <typename T>
            void removeComponent(EntityId entity, EntityFactory& factory) {
                typename std::vector<T>::iterator it;
                unsigned int i;
                for (i = 0, it = get<T>().begin(); it != get<T>().end();i++) {
                    if (i == entity)
                        it = get<T>().erase(it);
                    else
                        it++;
                }
                get<T>().resize(factory.getNbEntities());
            }
            class ComponentMapping {

                std::vector<std::vector<EntityId>> childrenMapping;
                std::vector<std::optional<EntityId>> parentMapping;
                public :
                void updateSize(EntityFactory& factory) {
                    childrenMapping.resize(factory.getNbEntities());
                    parentMapping.resize(factory.getNbEntities());
                }
                void addChild(EntityId parentId, EntityId child, EntityFactory& factory) {
                    childrenMapping.resize(factory.getNbEntities());
                    parentMapping.resize(factory.getNbEntities());
                    childrenMapping[parentId].push_back(child);
                    parentMapping[child] = parentId;
                }
                EntityId getRoot(EntityId entityId) {
                    EntityId root = entityId;
                    if (entityId >= parentMapping.size())
                        return root;
                    while (parentMapping[root].has_value()) {
                        root = parentMapping[root].value();
                    }
                    return root;
                }
                std::vector<EntityId> getChildren(EntityId entity) {
                    if (entity >= childrenMapping.size())
                        return std::vector<EntityId>();
                    return childrenMapping[entity];
                }
                EntityId getParent(EntityId entity) {
                    if(parentMapping[entity].has_value());
                       return parentMapping[entity].value();
                    return -1;
                }
                void setParent(EntityId entity, EntityId parent) {
                    parentMapping[entity] = parent;
                }
                bool remove(EntityId entity) {
                    for (unsigned int i = 0; i < childrenMapping[entity].size(); i++) {
                        remove(childrenMapping[entity][i]);
                    }
                    childrenMapping[entity].clear();
                    std::vector<std::optional<EntityId>>::iterator it;
                    for (it = parentMapping.begin(); it != parentMapping.end();) {
                        if (it->has_value() && it->value() == entity)
                            it = parentMapping.erase(it);
                        else
                            it++;
                    }
                }
                template <typename... Signature>
                EntityId clone(EntityFactory& factory, EntityId toClone, EntityId parent = -1) {
                    EntityId clonedId = factory.createEntity(getComponent<EntityInfoComponent>(toClone)->groupName);
                    clone_impl<0, Signature...>(toClone, clonedId, factory);
                    if (parent != -1) {
                        childrenMapping.resize(factory.getNbEntities());
                        parentMapping.resize(factory.getNbEntities());
                        childrenMapping[parent].push_back(clonedId);
                        parentMapping[clonedId] = parent;
                    }
                    for (unsigned int i = 0; i < getChildren(toClone).size(); i++) {
                        clone(factory, childrenMapping[toClone][i], clonedId);
                    }
                    return clonedId;
                }
                template <size_t I, typename... Components, class = typename std::enable_if_t<(sizeof...(Components) != 0 && I < sizeof...(Components)-1)>>
                void clone_impl(EntityId toCloneId, EntityId clonedId, EntityFactory& factory) {
                    //si l'entité possède le composant en question on le clône.
                    if (getComponent<std::tuple_element_t<I, std::tuple<Components...>>>(toCloneId)) {
                        addComponent(clonedId, *getComponent<std::tuple_element_t<I, std::tuple<Components...>>>(toCloneId), factory);
                    }
                    clone_impl<I+1, Components...>(toCloneId, clonedId, factory);
                }
                template <size_t I, typename... Components, class... D, class = typename std::enable_if_t<(sizeof...(Components) != 0 && I == sizeof...(Components)-1)>>
                void clone_impl(EntityId toCloneId, EntityId clonedId, EntityFactory& factory) {
                    if (getComponent<std::tuple_element_t<I, std::tuple<Components...>>>(toCloneId)) {
                        addComponent(clonedId, *getComponent<std::tuple_element_t<I, std::tuple<Components...>>>(toCloneId), factory);
                    }
                }
                template <size_t I, typename... Components, class... D, class...E, class = typename std::enable_if_t<(sizeof...(Components) == 0)>>
                void clone_impl(EntityId tocloneId, EntityId clonedId, EntityFactory& factory) {
                }
                template <typename... Signature>
                EntityId merge(EntityFactory& factory, EntityId toMerge1, EntityId toMerge2, std::string groupName, EntityId parent=-1) {
                    EntityId merged = factory.createEntity(groupName);
                    EntityId merged1, merged2;
                    if (parent == -1) {
                        merged1 = factory.createEntity(getComponent<EntityInfoComponent>(toMerge1)->groupName);
                        merged2 = factory.createEntity(getComponent<EntityInfoComponent>(toMerge2)->groupName);
                        merge_node<0, Signature...>(merged, toMerge1, factory);
                        merge_node<0, Signature...>(merged, toMerge2, factory);
                        childrenMapping.resize(factory.getNbEntities());
                        parentMapping.resize(factory.getNbEntities());
                        childrenMapping[merged].push_back(merged1);
                        parentMapping[merged1] = merged;
                        childrenMapping[merged].push_back(merged2);
                        parentMapping[merged2] = merged;
                    }
                    if (parent != -1 && toMerge1 != -1) {
                        childrenMapping.resize(factory.getNbEntities());
                        parentMapping.resize(factory.getNbEntities());
                        childrenMapping[parent].push_back(merged);
                        parentMapping[merged] = parent;
                        merge_node<0, Signature...>(merged, toMerge1, factory);
                    }
                    if (parent != -1 && toMerge2 != -1) {
                        childrenMapping.resize(factory.getNbEntities());
                        parentMapping.resize(factory.getNbEntities());
                        childrenMapping[parent].push_back(merged);
                        parentMapping[merged] = parent;
                        merge_node<0, Signature...>(merged, toMerge2, factory);
                    }
                    for (unsigned int i = 0; i < childrenMapping[toMerge1].size(); i++) {
                        merge(factory, childrenMapping[toMerge1][i], -1, getComponent<EntityInfoComponent>(childrenMapping[toMerge1][i])->groupName, (parent == -1) ? merged1 : merged);
                    }
                    for (unsigned int i = 0; i < childrenMapping[toMerge2].size(); i++) {
                        merge(factory, -1, childrenMapping[toMerge2][i], getComponent<EntityInfoComponent>(childrenMapping[toMerge2][i])->groupName, (parent == -1) ? merged2 : merged);
                    }
                    return merged;
                }
                template <size_t I, typename... Signature, class = typename std::enable_if_t<(sizeof...(Signature) != 0 && I < sizeof...(Signature)-1)>>
                void merge_node(EntityId entityId, EntityId toMergeId, EntityFactory& factory) {
                    //si l'entité possède le composant en question on le clône.
                    if (getComponent<std::tuple_element_t<I, std::tuple<Signature...>>>(toMergeId)) {
                        addComponent(entityId, *getComponent<std::tuple_element_t<I, std::tuple<Signature...>>>(toMergeId), factory);
                    }
                    merge_node<I+1, Signature...>(entityId, toMergeId, factory);
                }
                template < size_t I, typename... Signature, class... D, class = typename std::enable_if_t<(sizeof...(Signature) != 0 && I == sizeof...(Signature)-1)>>
                void merge_node(EntityId entityId, EntityId toMergeId, EntityFactory& factory) {
                    if (getComponent<std::tuple_element_t<I, std::tuple<Signature...>>>(toMergeId)) {
                        addComponent(entityId, *getComponent<std::tuple_element_t<I, std::tuple<Signature...>>>(toMergeId), factory);
                    }
                }
                template <size_t I, typename... Signature, class... D, class...E, class = typename std::enable_if_t<(sizeof...(Signature) == 0)>>
                void merge_node(EntityId tocloneId, EntityId clonedId, EntityFactory& factory) {
                }
                template <typename... Signature, typename System, typename... Params>
                void apply(System& system, std::vector<EntityId>& entities, std::tuple<Params...>& params, bool reverse=false) {
                  if (reverse) {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        std::vector<EntityId> children = getChildren(entities[i]);
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[children[0]].has_value()) {
                                EntityId parentId = parentMapping[children[0]].value();
                                children = childrenMapping[parentId];
                            } else {
                                children.clear();
                            }
                        }
                        for (unsigned i = 0; i < addedChildren.size(); i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], system, params, std::index_sequence_for<Signature...>());
                        }
                        this->template apply_impl<Signature...>(entities[i], system, params, std::index_sequence_for<Signature...>());
                      }
                  } else {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        this->template apply_impl<Signature...>(entities[i], system, params, std::index_sequence_for<Signature...>());
                        std::vector<EntityId> children = getChildren(entities[i]);
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[children[0]].has_value()) {
                                EntityId parentId = parentMapping[children[0]].value();
                                children = childrenMapping[parentId];
                            } else {
                                children.clear();
                            }
                        }
                        for (int i = addedChildren.size()-1; i >=0; i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], system, params, std::index_sequence_for<Signature...>());
                        }
                      }
                    }
                }

                template <typename... Signature, typename System, size_t... I, typename... Params>
                void apply_impl(EntityId entityId, System& system, std::tuple<Params...>& params, std::index_sequence<I...>) {
                    system.template operator()<Signature...>(entityId, params);
                }
                template <typename... Signature, typename System, typename... Params, class R>
                void apply(System& system, std::vector<EntityId>& entities, std::tuple<Params...>& params, std::vector<R>& ret, bool reverse=false) {
                  if (reverse) {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        std::vector<EntityId> children = getChildren(entities[i]);
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[children[0]].has_value()) {
                                EntityId parentId = parentMapping[children[0]];
                                children = childrenMapping[parentId];
                            } else {
                                children.clear();
                            }
                        }
                        for (unsigned i = 0; i < addedChildren.size(); i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], system, params, std::index_sequence_for<Signature...>(), ret);
                        }
                        this->template apply_impl<Signature...>(entities[i], system, params, std::index_sequence_for<Signature...>(), ret);
                      }
                  } else {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        this->template apply_impl<Signature...>(entities[i], system, params, std::index_sequence_for<Signature...>(), ret);
                        std::vector<EntityId> children = getChildren(entities[i]);
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[children[0]].has_value()) {
                                EntityId parentId = parentMapping[children[0]];
                                children = childrenMapping[parentId];
                            } else {
                                children.clear();
                            }
                        }
                        for (int i = addedChildren.size()-1; i >=0; i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], system, params, std::index_sequence_for<Signature...>(), ret);
                        }
                      }
                  }
                }
                template <typename... Signature, typename DynamicTuple, typename System, size_t... I, typename... Params, typename R>
                void apply_impl(EntityId entityId, DynamicTuple& tuple, System& system, std::tuple<Params...>& params, std::index_sequence<I...>, std::vector<R>& rets) {

                    rets.push_back(system.template operator()<Signature...>(entityId, params));
                }

            };

        }
    }
}
#endif
