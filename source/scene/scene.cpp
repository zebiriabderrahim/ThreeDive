//
// Created by ABDERRAHIM ZEBIRI on 2024-07-20.
//

#include "scene.h"

namespace s3Dive {

    entt::entity Scene::createEntity() {
        UUID uuid{};
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
        auto it = entitiesMap_.find(uuid);
        return (it != entitiesMap_.end()) ? it->second : entt::null;
    }

    std::optional<UUID> Scene::getEntityUUID(entt::entity entity) const {
        for (const auto& [uuid, e] : entitiesMap_) {
            if (e == entity) {
                return uuid;
            }
        }
        return std::nullopt;
    }

    void Scene::clear() {
        registry_.clear();
        entitiesMap_.clear();
    }

    void Scene::addEntity(const UUID& uuid, entt::entity entity) {
        entitiesMap_[uuid] = entity;
    }

} // namespace s3Dive
