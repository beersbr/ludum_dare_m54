#include "behaviors.h"

// register all the behaviors
BehaviorRegistery<PlayerBehavior> PlayerBehavior::registration("player");
BehaviorRegistery<PlayerBulletBehavior> PlayerBulletBehavior::registration("bullet");
BehaviorRegistery<SimpleEnemyBehavior> SimpleEnemyBehavior::registration("simple-enemy");
BehaviorRegistery<FlybyEnemyBehavior> FlybyEnemyBehavior::registration("straight-enemy");
BehaviorRegistery<ExplosionBehavior> ExplosionBehavior::registration("explosion");