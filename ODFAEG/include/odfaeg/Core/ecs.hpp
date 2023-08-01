#ifndef ODFAEG_ECS_HPP
#define ODFAEG_ECS_HPP
#include <vector>
#include <atomic>
#include <optional>
namespace odfaeg {
    namespace graphic {
        namespace ecs {
            template <typename T>
            struct atomwrapper
            {
              std::atomic<T> _a;

              atomwrapper()
                :_a()
              {}

              atomwrapper(const std::atomic<T> &a)
                :_a(a.load())
              {}

              atomwrapper(const atomwrapper &other)
                :_a(other._a.load())
              {}

              atomwrapper &operator=(const atomwrapper &other)
              {
                _a.store(other._a.load());
                return *this;
              }
              std::atomic<T>& get() {
                return _a;
              }
            };
            template <typename T>
            struct atomwrapper<T*>
            {
              std::atomic<T*> _a;

              atomwrapper()
                :_a()
              {}
              atomwrapper(T a)
                :_a(a)
              {}

              atomwrapper(const std::atomic<T> &a)
                :_a(a.load())
              {}

              atomwrapper(const atomwrapper &other)
                :_a(other._a.load())
              {}

              atomwrapper &operator=(const atomwrapper &other)
              {
                _a.store(other._a.load());
                return *this;
              }
              atomwrapper &operator=(T a)
              {
                _a.store(a);
                return *this;
              }
              std::atomic<T*>& get() {
                return _a;
              }
              void destroy() {
                T* ptr = _a.load();
                delete ptr;
              }
            };


            using EntityId = atomwrapper<std::size_t*>;
            class ComponentMapping {
                template <class>
                friend class World;
                friend class CloningSystem;
                friend class MergingSystem;
                friend class Grid;
                std::vector<std::vector<std::optional<size_t>>> componentMapping;
                std::vector<std::vector<EntityId>> childrenMapping;
                std::vector<std::optional<EntityId>> parentMapping;
                public :
                template <typename Component, typename DynamicTuple>
                auto addFlag(DynamicTuple& tuple) {
                    auto newTuple = tuple.template addType<Component>();
                    for (unsigned int i = 0; i < componentMapping.size(); i++) {
                        componentMapping[i].resize(newTuple.nbTypes());
                    }
                    return newTuple;
                }
                template <typename Component, typename DynamicTuple, typename Factory>
                auto addFlag(EntityId entity, DynamicTuple& tuple, Component component, Factory& factory) {
                    auto newTuple = tuple.add(component);
                    componentMapping.resize(factory.getNbEntities());
                    childrenMapping.resize(factory.getNbEntities());
                    parentMapping.resize(factory.getNbEntities());
                    for (unsigned int i = 0; i < componentMapping.size(); i++) {
                        componentMapping[i].resize(newTuple.nbTypes());
                    }

                    componentMapping[*entity.get().load()][newTuple.template getIndexOfTypeT<Component>()] = newTuple.template vectorSize<Component>()-1;
                    return newTuple;
                }

                template <typename DynamicTuple, typename Component, typename Factory>
                void addAgregate(EntityId entityId, DynamicTuple& tuple, Component component, Factory& factory) {
                    tuple.add(component);
                    componentMapping.resize(factory.getNbEntities());
                    childrenMapping.resize(factory.getNbEntities());
                    parentMapping.resize(factory.getNbEntities());
                    for (unsigned int i = 0; i < componentMapping.size(); i++) {
                        componentMapping[i].resize(tuple.nbTypes());
                    }
                    componentMapping[*entityId.get().load()][tuple.template getIndexOfTypeT<Component>()] = tuple.template vectorSize<Component>()-1;
                }
                template <typename Factory>
                void addChild(EntityId parentId, EntityId child, Factory& factory) {

                    for (unsigned int i = 0; i < childrenMapping.size(); i++) {
                        childrenMapping[*parentId.get().load()].resize(factory.getNbEntities());
                    }

                    childrenMapping[*parentId.get().load()].push_back(child);
                    parentMapping.resize(factory.getNbEntities());
                    parentMapping[*child.get().load()] = parentId;
                }
                EntityId getRoot(EntityId entityId) {
                    EntityId root = entityId;
                    while (parentMapping[*root.get().load()].has_value()) {
                        root = parentMapping[*root.get().load()].value();
                    }
                    return std::move(root);
                }
                void removeMapping(EntityId entityId) {
                    bool found = false;
                    std::vector<std::vector<std::optional<size_t>>>::iterator it;
                    unsigned int i = 0;
                    for (it = componentMapping.begin(); it != componentMapping.end() && !found;) {
                        if (*entityId.get().load() == i) {
                            found =true;
                            it = componentMapping.erase(it);
                        } else {
                            it++;
                            i++;
                        }
                    }
                    if (found) {
                        std::vector<std::vector<EntityId>>::iterator it2;
                        std::vector<EntityId>::iterator it3;
                        for (it2 = childrenMapping.begin(); it2 != childrenMapping.end(); it2++) {
                            for (it3 = it2->begin(); it3 != it2->end();) {
                                if (*entityId.get().load() == *it3->get().load()) {
                                    it3 = it2->erase(it3);
                                } else {
                                    it3++;
                                }
                            }
                        }
                        std::vector<std::optional<EntityId>>::iterator it4;
                        for (it4 = parentMapping.begin(); it4 != parentMapping.end(); it4++) {
                            if (*entityId.get().load() == *it4->value().get().load()) {
                                it4 = parentMapping.erase(it4);
                            } else {
                                it4++;
                            }
                        }
                    }
                }
                template <typename T, typename DynamicTuple>
                T* getAgregate(DynamicTuple& tuple, EntityId entityId) {
                    if (componentMapping[*entityId.get().load()][tuple.template getIndexOfTypeT<T>()].has_value())
                        return tuple.template get<T>(componentMapping[*entityId.get().load()][tuple.template getIndexOfTypeT<T>()].value());
                    return nullptr;
                }
                template <typename... Signature, typename DynamicTuple, typename System, typename... Params>
                void apply(DynamicTuple& tuple, System& system, std::vector<EntityId>& entities, std::tuple<Params...>& params, bool reverse=false) {
                  EntityId* tmpRootEntityId;
                  EntityId* tmpParentEntityId;
                  EntityId* tmpClonedParentEntityId;
                  bool first = true;
                  auto additionnal_params = std::make_tuple(tmpClonedParentEntityId, tmpRootEntityId, tmpParentEntityId, first);
                  auto cated_params = std::tuple_cat(params, additionnal_params);
                  if (reverse) {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        std::vector<EntityId> children = childrenMapping[*entities[i].get().load()];
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[*children[0].get().load()].has_value()) {
                                EntityId parentId = parentMapping[*children[0].get().load()];
                                children = childrenMapping[*parentId.get().load()];
                            } else {
                                children.clear();
                            }
                        }
                        for (unsigned i = 0; i < addedChildren.size(); i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], tuple, system, cated_params, std::index_sequence_for<Signature...>());
                        }
                        this->template apply_impl<Signature...>(entities[i], tuple, system, cated_params, std::index_sequence_for<Signature...>());
                      }
                  } else {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        this->template apply_impl<Signature...>(entities[i], tuple, system, cated_params, std::index_sequence_for<Signature...>());
                        std::vector<EntityId> children = childrenMapping[*entities[i].get().load()];
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[*children[0].get().load()].has_value()) {
                                EntityId parentId = parentMapping[*children[0].get().load()];
                                children = childrenMapping[*parentId.get().load()];
                            } else {
                                children.clear();
                            }
                        }
                        for (int i = addedChildren.size()-1; i >=0; i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], tuple, system, cated_params, std::index_sequence_for<Signature...>());
                        }
                      }
                  }
                }

                template <typename... Signature, typename DynamicTuple, typename System, size_t... I, typename... Params>
                void apply_impl(EntityId entityId, DynamicTuple& tuple, System& system, std::tuple<Params...>& params, std::index_sequence<I...>) {
                    auto tp = std::make_tuple(getAgregate<std::tuple_element_t<I, std::tuple<Signature...>>>(tuple, entityId)...);
                    system(tp, entityId, params);
                }
                template <typename... Signature, typename DynamicTuple, typename System, typename... Params, class R>
                void apply(DynamicTuple& tuple, System& system, std::vector<EntityId>& entities, std::tuple<Params...>& params, std::vector<R>& ret, bool reverse=false) {
                  EntityId* tmpRootEntityId;
                  EntityId* tmpParentEntityId;
                  EntityId* tmpClonedParentEntityId;
                  bool first = true;
                  auto additionnal_params = std::make_tuple(tmpClonedParentEntityId, tmpRootEntityId, tmpParentEntityId, first);
                  auto cated_params = std::tuple_cat(params, additionnal_params);
                  if (reverse) {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        std::vector<EntityId> children = childrenMapping[*entities[i].get().load()];
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[*children[0].get().load()].has_value()) {
                                EntityId parentId = parentMapping[*children[0].get().load()];
                                children = childrenMapping[*parentId.get().load()];
                            } else {
                                children.clear();
                            }
                        }
                        for (unsigned i = 0; i < addedChildren.size(); i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], tuple, system, cated_params, std::index_sequence_for<Signature...>(), ret);
                        }
                        this->template apply_impl<Signature...>(entities[i], tuple, system, cated_params, std::index_sequence_for<Signature...>(), ret);
                      }
                  } else {
                      for (unsigned int i = 0; i < entities.size(); i++) {
                        this->template apply_impl<Signature...>(entities[i], tuple, system, cated_params, std::index_sequence_for<Signature...>(), ret);
                        std::vector<EntityId> children = childrenMapping[*entities[i].get().load()];
                        std::vector<EntityId> addedChildren;
                        while (children.size() > 0) {
                            for (unsigned int i = 0; i < children.size(); i++) {
                                addedChildren.push_back(children[i]);
                            }
                            if (parentMapping[*children[0].get().load()].has_value()) {
                                EntityId parentId = parentMapping[*children[0].get().load()];
                                children = childrenMapping[*parentId.get().load()];
                            } else {
                                children.clear();
                            }
                        }
                        for (int i = addedChildren.size()-1; i >=0; i++) {
                            this->template apply_impl<Signature...>(addedChildren[i], tuple, system, cated_params, std::index_sequence_for<Signature...>(), ret);
                        }
                      }
                  }
                }
                template <typename... Signature, typename DynamicTuple, typename System, size_t... I, typename... Params, typename R>
                void apply_impl(EntityId entityId, DynamicTuple& tuple, System& system, std::tuple<Params...>& params, std::index_sequence<I...>, std::vector<R>& rets) {
                    auto tp = std::make_tuple(getAgregate<std::tuple_element_t<I, std::tuple<Signature...>>>(tuple, entityId)...);
                    rets.push_back(system(tp, entityId, params));
                }

            };
            struct EntityFactory {
                size_t nbEntities=0;
                std::vector<EntityId> ids;
                EntityId createEntity() {

                    EntityId id;
                    id.get().store(new size_t(nbEntities));
                    ids.push_back(id);
                    nbEntities++;
                    return id;
                }
                size_t getNbEntities() {
                    return nbEntities;
                }
                ~EntityFactory() {
                    for (unsigned int i = 0; i < ids.size(); i++) {
                        ids[i].destroy();
                    }
                }
                private :
                void destroyEntity(EntityId id) {
                    const auto itToFind =
                        std::find_if(ids.begin(), ids.end(),
                                     [&](auto& p) { return p.get().load() == id.get().load(); });
                    const bool found = (itToFind != ids.end());
                    if (found) {
                        for (auto it = itToFind; it != ids.end(); it++) {
                            *it->get().load() =  *it->get().load() - 1;
                        }
                        itToFind->destroy();
                        ids.erase(itToFind);

                        nbEntities--;
                    }
                }
            };
            template <typename T>
            atomwrapper<bool>& getProducerStatus() {
                static atomwrapper<bool> task;
                return task;
            }
            template <typename T>
            void setProducerStatus(bool status) {
                 getProducerStatus<T>().get().store(status);
            }

            template <typename T>
            bool isProduced() {
                 return getProducerStatus<T>().get().load();
            }
            template <typename T>
            atomwrapper<bool>& getConsumerStatus() {
                static atomwrapper<bool> task(true);
                return task;
            }
            template <typename T>
            void setConsumerStatus(bool status) {
                 getConsumerStatus<T>().get().store(status);
            }
            template <typename T>
            bool isConsumed() {
                 return getConsumerStatus<T>().get().load();
            }
        }
    }
}
#endif
