#include "components.h"

#include <string>

namespace CapEngine
{

Component::Metadata const &Component::metadata() const { return m_metadata; }

void Component::setMetadata(const std::string &in_key,
                            const MetadataType &in_value)
{
    m_metadata[in_key] = in_value;
}

} // namespace CapEngine
