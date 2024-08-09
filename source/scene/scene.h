#ifndef THREEDIVE_SCENE_H
#define THREEDIVE_SCENE_H

#include <unordered_map>
#include <optional>
#include <entt/entity/registry.hpp>
#include "../core/uuid.h"

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

        [[nodiscard]] entt::registry& getRegistry() noexcept { return registry_; }
        [[nodiscard]] const entt::registry& getRegistry() const noexcept { return registry_; }
        [[nodiscard]] const EntityMap& getEntities() const noexcept { return entitiesMap_; }

        [[nodiscard]] entt::entity createEntity();
        void destroyEntity(const UUID& uuid);

        [[nodiscard]] entt::entity getEntity(const UUID& uuid) const noexcept;
        [[nodiscard]] std::optional<UUID> getEntityUUID(entt::entity entity) const;

        template<typename T, typename... Args>
        T& addComponent(const UUID& uuid, Args&&... args) {
            auto entity = getEntity(uuid);
            if (entity == entt::null) {
                throw std::runtime_error("Entity not found");
            }
            if (hasComponent<T>(uuid)) {
                return registry_.get<T>(entity);
            }
            return registry_.emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        void removeComponent(const UUID& uuid) {
            auto entity = getEntity(uuid);
            if (entity != entt::null) {
                registry_.remove<T>(entity);
            }
        }

        template<typename T>
        [[nodiscard]] T& getComponent(const UUID& uuid) {
            auto entity = getEntity(uuid);
            if (entity == entt::null) {
                throw std::runtime_error("Entity not found");
            }
            return registry_.get<T>(entity);
        }

        template<typename T>
        [[nodiscard]] const T& getComponent(const UUID& uuid) const {
            auto entity = getEntity(uuid);
            if (entity == entt::null) {
                throw std::runtime_error("Entity not found");
            }
            return registry_.get<T>(entity);
        }

        template<typename T>
        [[nodiscard]] bool hasComponent(const UUID& uuid) const noexcept {
            auto entity = getEntity(uuid);
            return entity != entt::null && registry_.all_of<T>(entity);
        }

        template<typename... Components>
        [[nodiscard]] auto view() {
            return registry_.view<Components...>();
        }

        template<typename... Components>
        [[nodiscard]] auto view() const {
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