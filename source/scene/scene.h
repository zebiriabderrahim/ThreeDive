#ifndef THREEDIVE_SCENE_H
#define THREEDIVE_SCENE_H

#include <unordered_map>
#include <functional>
#include <optional>
#include <entt/entity/registry.hpp>
#include "../core/uuid.h"
#include "../camera/camera_controller.h"
#include "../platform/openGLRender/gl_shader_program.h"

namespace s3Dive {

    class Scene {
    public:
        using EntityMap = std::unordered_map<UUID, entt::entity>;

        Scene() = default;
        ~Scene() = default;

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) noexcept = default;
        Scene& operator=(Scene&&) noexcept = default;


        entt::registry& getRegistry() { return registry_; }
        const entt::registry& getRegistry() const { return registry_; }
        const EntityMap& getEntities() const { return entitiesMap_; }

        entt::entity createEntity();
        void destroyEntity(const UUID& uuid);
        std::optional<entt::entity> getEntity(const UUID& uuid) const;
        std::optional<UUID> getEntityUUID(entt::entity entity) const;

        template<typename T, typename... Args>
        T& addComponent(const UUID& uuid, Args&&... args) {
            auto entity = getEntity(uuid);
            if (!entity) {
                throw std::runtime_error("Entity not found");
            }
            return registry_.emplace<T>(*entity, std::forward<Args>(args)...);
        }

        template<typename T>
        void removeComponent(const UUID& uuid) {
            auto entity = getEntity(uuid);
            if (!entity) {
                throw std::runtime_error("Entity not found");
            }
            registry_.remove<T>(*entity);
        }

        template<typename T>
        T& getComponent(const UUID& uuid) {
            auto entity = getEntity(uuid);
            if (!entity) {
                throw std::runtime_error("Entity not found");
            }
            return registry_.get<T>(*entity);
        }

        template<typename T>
        const T& getComponent(const UUID& uuid) const {
            auto entity = getEntity(uuid);
            if (!entity) {
                throw std::runtime_error("Entity not found");
            }
            return registry_.get<T>(*entity);
        }

        template<typename T>
        bool hasComponent(const UUID& uuid) const {
            auto entity = getEntity(uuid);
            if (!entity) {
                return false;
            }
            return registry_.all_of<T>(*entity);
        }

        template<typename... Components>
        auto view() {
            return registry_.view<Components...>();
        }

        template<typename... Components>
        auto view() const {
            return registry_.view<Components...>();
        }

        void clear();

    private:
        entt::registry registry_;
        EntityMap entitiesMap_;

        void addEntity(const UUID& uuid, entt::entity entity);
    };

} // namespace s3Dive

#endif //THREEDIVE_SCENE_H