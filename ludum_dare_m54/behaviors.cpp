#include "behaviors.h"

// register all the behaviors
BehaviorRegistery<CosineEnemyBehavior> CosineEnemyBehavior::registration("cosine_enemy_behavior");
BehaviorRegistery<PlayerBehavior> PlayerBehavior::registration("player");