#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace CapEngine
{

class GameState
{
public:
  virtual ~GameState(){};
  virtual void render() = 0;
  virtual void update(double ms) = 0;
  virtual bool onLoad() { return true; }
  virtual bool onDestroy() { return true; }
  virtual bool onPause();
  virtual bool onResume();
  virtual bool enabled() const;
  virtual void setEnabled(bool in_enabled);

protected:
  bool m_enabled = true;
};

} // namespace CapEngine
#endif // GAMESTATE_H
