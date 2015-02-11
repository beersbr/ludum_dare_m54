#include "Player.h"


Player::Player(void) : Entity()
{
	moveSpeed = 350.0;
	shootBulletsInterval = 0.3f;
	shootBulletsElapsed = 0.0f;

	animationFrameInterval = 0.1f;
	currentFrameTime = 0.0f;
}


Player::~Player(void)
{
}

void Player::Update(float dt)
{

}

void Player::Update(BGLInputState *state, float dt)
{
	// NOTE(brett): this is getting removed next commit
	//float dx = 0.0f,
	//	  dy = 0.0f;


	//currentFrameTime += dt;
	//shootBulletsElapsed += dt;

	//if(state->controllerSz > 0)
	//{
	//	dx = state->controllers[0].LX * moveSpeed * dt;
	//	dy = state->controllers[0].LY * moveSpeed * dt;
	//}

	//if(state->keyboard.keys[SDLK_w].down)
	//{
	//	dy = moveSpeed * dt * -1.0;
	//}
	//if(state->keyboard.keys[SDLK_s].down)
	//{
	//	dy = moveSpeed * dt * 1.0;
	//}
	//if(state->keyboard.keys[SDLK_a].down)
	//{
	//	dx = moveSpeed * dt * -1.0;
	//}
	//if(state->keyboard.keys[SDLK_d].down)
	//{
	//	dx = moveSpeed * dt * 1.0;
	//}


	//pos.x += dx;
	//pos.y += dy;

	//if(state->controllerSz > 0)
	//{
	//	// NOTE(brett): This can be done better if we know the current game render rate. 
	//	// If we know the render rate we know if we are leaking time in frames
	//	if((state->controllers[0].X.down) && shootBulletsElapsed > shootBulletsInterval)
	//	{
	//		// TODO(brett): this is annoying so I disabled it until I can get a different sound lib in 
	//		BGLAudio::PlayAudio("laser");
	//		// NOTE(brett): this works better than the sdl version. Though we just get a filename. it is going to 
	//		// be used until fmod gets added
	//		//PlaySoundA("sounds/laser.wav", NULL, SND_FILENAME|SND_ASYNC);
	//		shootBulletsElapsed = 0.0f;
	//		//createPlayerBullet(glm::vec2(pos.x + 40, pos.y), glm::vec2(20, 20));
	//	}
	//}
	//if (state->keyboard.keys[SDLK_j].down)
	//{
	//	if(shootBulletsElapsed > shootBulletsInterval)
	//	{
	//		BGLAudio::PlayAudio("laser");
	//		// NOTE(brett): this works better than the sdl version. Though we just get a filename. it is going to 
	//		// be used until fmod gets added
	//		//PlaySoundA("sounds/laser.wav", NULL, SND_FILENAME|SND_ASYNC);
	//		shootBulletsElapsed = 0.0f;
	//		//createPlayerBullet(glm::vec2(pos.x + 40, pos.y), glm::vec2(20, 20));
	//	}
	//}

	// //fuck it, change the frame ASAP!
 //   if(currentFrameTime > animationFrameInterval)
 //   {
 //       sprite.AdvanceAnimationFrame();
	//	currentFrameTime = 0.0f;
 //   }
}

void Player::Render()
{
	//sprite.model = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, 0.0f));
	//sprite.Render();
}