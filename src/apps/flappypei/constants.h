namespace FlappyPei {

// window
static constexpr int kLogicalWindowWidth = 1600;
static constexpr int kLogicalWindowHeight = 900;

// player
static constexpr int kSpriteWidth = 48;
static constexpr int kSpriteHeight = 32;

// cats
static constexpr int kCatWidth = 32;

// physics
static constexpr float kGravity = 900.0f;  // Gravity constant
static constexpr float kJumpVelocity = -300.0f;  // jump velocity
static constexpr int kStartingCatVelocity = -150;
static constexpr int kStartingCatInterval = 50;

// gameplay
static constexpr double kGameWaitTime = 3000.0;  // Time to wait before starting the game
static constexpr int kStartingGapSize = 300;
static constexpr int kStartingIntervalSize = 300;

// other
static constexpr char const* kDefaultFont = "tahoma.ttf";
static constexpr int kBannerFontSize = 72;

}  // namespace FlappyPei
