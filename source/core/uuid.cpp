#include "uuid.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace s3Dive {

    UUID::UUID() : m_UUID(generateUUID()) {}

    UUID::UUID(uint64_t value) noexcept : m_UUID(value) {}

    uint64_t UUID::generateUUID() {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> dis;

        return dis(gen);
    }

    std::string UUID::toString() const {
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << m_UUID;
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
        return os << uuid.toString();
    }

} // namespace s3Dive
