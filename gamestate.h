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
  virtual bool onLoad() = 0;
  virtual bool onDestroy() = 0;
  virtual bool onPause() = 0;
  virtual bool onResume() = 0;

private:
};

} // namespace CapEngine
#endif // GAMESTATE_H
