#ifndef CAPENGINE_DIALOGSTATE_H
#define CAPENGINE_DIALOGSTATE_H

#include "widgetstate.h"

#include <functional>

namespace CapEngine
{
namespace UI
{
//! WidgetState class for a dialog box
class DialogState : public WidgetState
{
public:
  using callback_t = std::function<void(bool)>;

  static std::shared_ptr<DialogState> create(const std::string &msg,
                                             callback_t callback);
  virtual ~DialogState() override = default;

  bool onLoad() override;
  bool onDestroy() override;
  void update(double ms) override;

private:
  DialogState(const std::string &msg, callback_t callback);

  // deleted
  DialogState(const DialogState &) = delete;
  DialogState &operator=(const DialogState &) = delete;

  std::string m_msg;     //<! The message to be displayed.
  callback_t m_callback; //<! The callback to be called with the result.
  bool m_result = false; //<! The result.
  bool m_done = false;   //<! Flag indicating if the dialog is done.

  const int m_kWidth = 300;
  const int m_kHeight = 200;
  const std::string m_kTitle = "";
  const std::string m_kOkButtonText = "Ok";
  const std::string m_kCancelButtonText = "Cancel";
  boost::signals2::scoped_connection m_okSignalConnection;
  boost::signals2::scoped_connection m_cancelSignalConnection;
};

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_DIALOGSTATE_H
