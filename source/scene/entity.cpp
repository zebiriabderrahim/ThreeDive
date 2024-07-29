//
// Created by ABDERRAHIM ZEBIRI on 2024-07-22.
//

#ifndef THREEDIVE_ENTITY_INL
#define THREEDIVE_ENTITY_INL

#include "entity.h"

namespace s3Dive {

    inline Entity::Entity(entt::entity handle, Scene* scene)
            : entityHandle(handle), scene(scene) {}

    template<typename T, typename... Args>
    T& Entity::addComponent(Args&&... args) {
        return scene->addComponent<T>(getUUID(), std::forward<Args>(args)...);
    }

    template<typename T>
    void Entity::removeComponent() {
        scene->removeComponent<T>(getUUID());
    }

    template<typename T>
    T& Entity::getComponent() {
        return scene->getComponent<T>(getUUID());
    }

    template<typename T>
    const T& Entity::getComponent() const {
        return scene->getComponent<T>(getUUID());
    }

    template<typename T>
    bool Entity::hasComponent() const {
        return scene->hasComponent<T>(getUUID());
    }


    std::optional<UUID> Entity::getUUID() const {
        if (!scene) {
            return std::nullopt;
        }
        return scene->getEntityUUID(entityHandle);
    }



} // namespace s3Dive

#endif // THREEDIVE_ENTITY_INL
