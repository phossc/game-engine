#include <engine/core/component_uuid.hpp>

#include <iomanip>
#include <sstream>

namespace engine::core {

std::string Component_uuid::str() const {
    std::ostringstream ostr;
    ostr << std::hex << std::noshowbase << std::nouppercase << std::internal
         << std::setfill('0');
    ostr << std::setw(8) << (upper_half_ >> 32);
    ostr << std::setw(1) << '-';
    ostr << std::setw(4) << (upper_half_ << 32 >> 48);
    ostr << std::setw(1) << '-';
    ostr << std::setw(4) << (upper_half_ << 48 >> 48);
    ostr << std::setw(1) << '-';
    ostr << std::setw(4) << (lower_half_ >> 48);
    ostr << std::setw(1) << '-';
    ostr << std::setw(12) << (lower_half_ << 16 >> 16);
    return ostr.str();
}

} // namespace engine::core