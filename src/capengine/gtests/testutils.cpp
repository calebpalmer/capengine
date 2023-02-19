#include "testutils.h"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <string>

namespace CapEngine {

namespace Testing {

TempFile::TempFile() {
  const std::filesystem::path tempDir = std::filesystem::temp_directory_path();

  while (true) {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const std::string fileName = to_string(uuid);
    m_filePath = tempDir / fileName;

    if (!std::filesystem::exists(m_filePath))
      break;
  }
}

TempFile::~TempFile() {
  if (std::filesystem::exists(m_filePath)) {
    std::filesystem::remove(m_filePath);
  }
}

const std::filesystem::path &TempFile::getFilePath() const { return m_filePath; }
} // namespace Testing
} // namespace CapEngine
