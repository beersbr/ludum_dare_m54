#ifndef __SCENE_H__
#define __SCENE_H__

enum SceneState {
	CLOSING = 0,
	STARTING,
	PLAYING,
	PAUSED,
	SEGUE,
	SCENE_STATE_COUNT
};

class Scene
{
public:

	virtual void Initialize() = 0;
	virtual void Playing(float dt) = 0;
	virtual void Paused(float dt) = 0;
	virtual void Segue(float dt) = 0;
	virtual void Closing(float dt) = 0;

	void SetState(SceneState s)
	{

	}

public:
	SceneState state;
};

#endif