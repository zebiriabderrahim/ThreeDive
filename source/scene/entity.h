//
// Created by ABDERRAHIM ZEBIRI on 2024-07-22.
//

#ifndef THREEDIVE_ENTITY_H
#define THREEDIVE_ENTITY_H

#include "scene.h"
#include <entt/entity/registry.hpp>
#include "../core/uuid.h"

namespace s3Dive {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);

        template<typename T, typename... Args> T& addComponent(Args&&... args);
        template<typename T> void removeComponent();
        template<typename T>T& getComponent();
        template<typename T> const T& getComponent() const;
        template<typename T> [[nodiscard]] bool hasComponent() const;
        std::optional<UUID> getUUID() const;

        explicit operator bool() const { return entityHandle != entt::null && scene != nullptr; }
        explicit operator entt::entity() const { return entityHandle; }
        friend bool operator==(const Entity& lhs, const Entity& rhs) {
            return lhs.entityHandle == rhs.entityHandle && lhs.scene == rhs.scene;
        }
        friend bool operator!=(const Entity& lhs, const Entity& rhs) { return !(lhs == rhs);}

    private:
        entt::entity entityHandle{entt::null };
        Scene* scene = nullptr;
    };

} // namespace s3Dive


#endif // THREEDIVE_ENTITY_H

