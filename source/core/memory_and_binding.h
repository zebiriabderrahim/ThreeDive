//
// Created by ABDERRAHIM ZEBIRI on 2024-07-03.
//

#ifndef THREEDIVE_MEMORY_AND_BINDING_H
#define THREEDIVE_MEMORY_AND_BINDING_H

#include <memory>
#include <functional>
#include <type_traits>


namespace s3Dive {

    // Smart pointer aliases
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    // Smart pointer creation functions
    template<typename T, typename... Args>
    [[nodiscard]] constexpr UniquePtr<T> MakeUnique(Args &&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    [[nodiscard]] constexpr SharedPtr<T> MakeShared(Args &&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    // Method binding for non-const member functions
    template<typename T, typename F, typename Ret = std::invoke_result_t<F, T*>>
    auto BindMethod(T* instance, F&& method)
    {
        return [instance, boundMethod = std::forward<F>(method)](auto&&... args) -> Ret {
            return std::invoke(boundMethod, instance, std::forward<decltype(args)>(args)...);
        };
    }

    // Method binding for const member functions
    template<typename T, typename F, typename Ret = std::invoke_result_t<F, const T*>>
    auto BindMethod(const T* instance, F&& method)
    {
        return [instance, boundMethod = std::forward<F>(method)](auto&&... args) -> Ret {
            return std::invoke(boundMethod, instance, std::forward<decltype(args)>(args)...);
        };
    }
} // s3Dive
#endif //THREEDIVE_MEMORY_AND_BINDING_H
