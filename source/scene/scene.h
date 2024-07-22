//
// Created by ABDERRAHIM ZEBIRI on 2024-07-20.
//
#ifndef THREEDIVE_SCENE_H
#define THREEDIVE_SCENE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <entt/entt.hpp>
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

        void onUpdate(float deltaTime);

        entt::registry& getRegistry() { return registry_; }
        const EntityMap& getEntities() const { return entitiesMap_; }

        entt::entity createEntity();
        void destroyEntity(const UUID& uuid);
        entt::entity getEntity(const UUID& uuid) const;

        template<typename T, typename... Args>
        T& addComponent(const UUID& uuid, Args&&... args) {
            auto entity = getEntity(uuid);
            return registry_.emplace<T>(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        void removeComponent(const UUID& uuid) {
            auto entity = getEntity(uuid);
            registry_.remove<T>(entity);
        }

        template<typename T>
        T& getComponent(const UUID& uuid) {
            auto entity = getEntity(uuid);
            return registry_.get<T>(entity);
        }

        template<typename T>
        bool hasComponent(const UUID& uuid) const {
            auto entity = getEntity(uuid);
            return registry_.all_of<T>(entity);
        }

        void forEach(const std::function<void(entt::entity)>& func);

        void clear();

    private:
        entt::registry registry_;
        EntityMap entitiesMap_;

        UUID gridUUID_; // Store the UUID of the grid entity
        std::unique_ptr<SceneGridSystem> gridSystem_; // Unique pointer to the SceneGridSystem

        void addEntity(const UUID& uuid, entt::entity entity);
    };

} // namespace s3Dive

#endif //THREEDIVE_SCENE_H
