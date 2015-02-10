#include "behaviors.h"

// register all the behaviors
BehaviorRegistery<PlayerBehavior> PlayerBehavior::registration("player");
BehaviorRegistery<PlayerBulletBehavior> PlayerBulletBehavior::registration("bullet");