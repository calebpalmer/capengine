#ifndef CAPENGINETESTING_TESTUTILS_H
#define CAPENGINETESTING_TESTUTILS_H

#include <filesystem>

namespace CapEngine {
namespace Testing {

class TempFile final {
public:
  TempFile();
  ~TempFile();
  const std::filesystem::path &getFilePath() const;

private:
  std::filesystem::path m_filePath;
};

} // namespace Testing
} // namespace CapEngine

#endif /* CAPENGINETESTING_TESTUTILS_H */
