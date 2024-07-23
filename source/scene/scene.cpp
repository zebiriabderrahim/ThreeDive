//
// Created by ABDERRAHIM ZEBIRI on 2024-07-20.
//

#include <memory>
#include "SceneGridSystem.h"

#include "Scene.h"

namespace s3Dive {
    Scene::Scene() : gridSystem_(std::make_unique<SceneGridSystem>()) {}

    entt::entity Scene::createEntity() {
        UUID uuid {};
        entt::entity entity = registry_.create();
        addEntity(uuid, entity);
        return entity;
    }

    void Scene::destroyEntity(const UUID& uuid) {
        auto it = entitiesMap_.find(uuid);
        if (it != entitiesMap_.end()) {
            registry_.destroy(it->second);
            entitiesMap_.erase(it);
        }
    }

    entt::entity Scene::getEntity(const UUID& uuid) const {
        if (auto it = entitiesMap_.find(uuid); it != entitiesMap_.end()) {
            return it->second;
        }
        return entt::null;
    }

    void Scene::clear() {
        registry_.clear();
        entitiesMap_.clear();
    }

    void Scene::addEntity(const UUID& uuid, entt::entity entity) {
        entitiesMap_[uuid] = entity;
    }

    void Scene::onUpdate(float deltaTime) {
        // Implement your update logic here
        // You can iterate over systems or specific components
    }

    std::optional<UUID> Scene::getEntityUUID(entt::entity entity) const {
        for (const auto& [uuid, e] : entitiesMap_) {
            if (e == entity) {
                return uuid;
            }
        }
        return std::nullopt;
    }

} // namespace s3Dive
