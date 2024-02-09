#ifndef CAPENGINETESTING_TESTUTILS_H
#define CAPENGINETESTING_TESTUTILS_H

#include <filesystem>
#include <string>

#include <gtest/gtest.h>

namespace CapEngine::testing {

class TempFile final {
public:
  TempFile();
  ~TempFile();
  const std::filesystem::path &getFilePath() const;

private:
  std::filesystem::path m_filePath;
};

std::filesystem::path getTestFilePath();

} // namespace CapEngine::testing

#endif /* CAPENGINETESTING_TESTUTILS_H */
