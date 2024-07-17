//
// Created by ABDERRAHIM ZEBIRI on 2024-07-03.
//

#ifndef THREEDIVE_SCENE_NODE_H
#define THREEDIVE_SCENE_NODE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../core/memory_and_binding.h"

namespace s3Dive {

    class SceneNode : public std::enable_shared_from_this<SceneNode> {
    public:

        explicit SceneNode(std::string name = "SceneNode");

        virtual ~SceneNode() = default;

        SceneNode(const SceneNode &) = delete;
        SceneNode &operator=(const SceneNode &) = delete;
        SceneNode(SceneNode &&) = default;
        SceneNode &operator=(SceneNode &&) = default;

        void addChild(SharedPtr<SceneNode> child);
        void removeChild(const SharedPtr<SceneNode> &child);

        WeakPtr<SceneNode> getParent() const;
        const std::vector<SharedPtr<SceneNode>> &getChildren() const;

        void setLocalPosition(const glm::vec3 &position);
        void setLocalRotation(const glm::quat &rotation);
        void setLocalScale(const glm::vec3 &scale);

        glm::vec3 getLocalPosition() const;
        glm::quat getLocalRotation() const;
        glm::vec3 getLocalScale() const;

        void setName(std::string name);
        const std::string &getName() const;

        void setVisible(bool visible);
        bool isVisible() const;

        virtual void update(float deltaTime);
        virtual void render();

    private:
        std::string name_;
        bool m_isVisible;

        WeakPtr<SceneNode> parent_;
        std::vector<SharedPtr<SceneNode>> children_;

        glm::vec3 localPosition_;
        glm::quat localRotation_;
        glm::vec3 localScale_;
    };


} // s3Dive

#endif //THREEDIVE_SCENE_NODE_H
