#ifndef S3DIVE_UUID_H
#define S3DIVE_UUID_H

#include <cstdint>
#include <string>
#include <iostream>

namespace s3Dive {

    class UUID {
    public:
        UUID();
        explicit UUID(uint64_t value) noexcept;

        UUID(const UUID&) noexcept = default;
        UUID& operator=(const UUID&) noexcept = default;
        UUID(UUID&&) noexcept = default;
        UUID& operator=(UUID&&) noexcept = default;

        [[nodiscard]] constexpr uint64_t value() const noexcept { return m_UUID; }
        [[nodiscard]] constexpr bool operator==(const UUID& other) const noexcept { return m_UUID == other.m_UUID; }
        [[nodiscard]] constexpr bool operator!=(const UUID& other) const noexcept { return !(*this == other); }
        [[nodiscard]] constexpr bool operator<(const UUID& other) const noexcept { return m_UUID < other.m_UUID; }

        [[nodiscard]] std::string toString() const;

        friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);

    private:
        uint64_t m_UUID;
        static uint64_t generateUUID();
    };

} // namespace s3Dive

namespace std {
    template<>
    struct hash<s3Dive::UUID> {
        size_t operator()(const s3Dive::UUID& uuid) const noexcept {
            return hash<uint64_t>()(uuid.value());
        }
    };
}

#endif // S3DIVE_UUID_H
