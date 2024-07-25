//
// Created by ABDERRAHIM ZEBIRI on 2024-07-20.
//

#include "sceneGridSystem.h"

#include "scene.h"
#include "entity.h"
#include "components.h"

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

    std::optional<entt::entity> Scene::getEntity(const UUID& uuid) const {
        if (auto it = entitiesMap_.find(uuid); it != entitiesMap_.end()) {
            return it->second;
        }
        return std::nullopt;
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
