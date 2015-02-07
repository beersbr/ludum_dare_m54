#pragma once

/**

BGLE :: Brett's openGL Engine (boggle)

A set of functionality for handling opengl and sprites. 

TODO(brett): add the sounds component to this.
TODO(brett): split definitions and code up for creating the lib

Version 0.02

*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <gl\gl.h>

#include "language.h"

// The max number of frames a sprite can have
#define SPRITE_MAX_FRAMES 64
#define BGL_MAX_LAYERS 5
#define BGL_BATCH_SPRITE_MAX 100000

typedef struct
{
	GLfloat x, y;
	GLfloat w, h;
} BGLRect;

// NOTE(brett): perhaps the texture can hold a tag as this structure is
// pretty closely bound to the texturehandler
typedef struct
{
	GLuint id;
	int32_t width;
	int32_t height;

	bool isBound;
	uint8_t textureUnit;

} BGLTexture; 

typedef struct
{
	GLuint id;
	bool isBound;
} BGLShader;

typedef struct
{
	GLuint id;
	GLuint vao;
} BGLFrame;

typedef struct 
{
	glm::vec2 pos;
	glm::vec2 uv;
} BGLSpriteVertex;


typedef struct
{
	GLfloat x, y;
	GLfloat u, v;
} LayerVertex;


global glm::mat4 BGLProjection;
global glm::mat4 BGLCamera;
global BGLFrame Frame = {};

// Creates a BGL rect object
BGLRect BGLRectMake(GLfloat x, GLfloat y, GLfloat w, GLfloat h);

// A quick boolean check to tell if two rectangles overlap in any way
bool BGLRectOverlap(BGLRect a, BGLRect b);

// Check if two rectanles equal each other
bool operator==(BGLRect a, BGLRect b);


// Inializes the current frame. This function will set up the screen space to be shown.
// TODO(brett): make this actually set up the render frame for post effects
void InitializeFrame(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);


typedef struct
{
	GLuint frameId;
	GLuint texId;

	uint32_t width;
	uint32_t height;
} BGLLayer;


// This is the layer manager. It takes control of rendering the layers
// in the way they should be and giving access to the programmer to 
// decide which layers to be drawn to.
// Layers are from 0 to BGL_MAX_LAYERS.
// REMARK: 
// Layer 0 is the closest layer to the screen. Layer 1 sits BEHIND layer 0 etc.
class BGLLayerManager
{
public: 
	// TODO(brett): get the frame manager set up so we can run shaders on the entire frame vs each sprite
	static void InitLayers(uint32_t width, uint32_t height);
	static void UseLayer(uint32_t layer);
	static void RenderLayers();
	static void CleanLayer(uint32_t layer);
	
	static void Cleanup();

private:
	static BGLLayer layers[BGL_MAX_LAYERS];
	static uint32_t layerSz;
	static LayerVertex quad[6];
	static GLuint vbo;
	static GLint LayerPosition0;
	static GLint LayerPosition1;
	static GLint LayerPosition2;
	static GLint LayerPosition3;
	static GLint LayerPosition4;
};


// This will set the camera which is the viewspace for the screen.
// The camera shows the same number of pixes that the view was set up with. It
// currently just acts as an offset.
void SetCamera(glm::vec2 const offset);

class TextureHandler
{
public:
	/**
	Put the image on the video card
	*/
	static void Load(std::string tag, std::string filename);

	/**
	Bind the image to the texture2d mount with the given slot, default slot is ACTIVE_TEXTURE0
	*/
	static void Bind(std::string tag, int32_t textureSlot = 0);

	/**
	UNBind the image to the texture2d mount with the given slot, default slot is ACTIVE_TEXTURE0
	*/
	static void Unbind(std::string tag);


	/**
	Get the actual BGLTexture at the given tag
	*/
	static BGLTexture Get(std::string tag);

private:
	static std::unordered_map<std::string, BGLTexture> textures;
	static std::unordered_map<std::string, bool> boundTextures;
};

class ShaderHandler
{
public:
	static void Load(std::string tag, std::string vertex, std::string fragment);
	static void Enable(std::string tag);
	static void Disable(std::string tag);
	static bool IsBound(std::string tag);

	static BGLShader Get(std::string tag);

private:
	static std::unordered_map<std::string, BGLShader> shaders;
	static std::string boundShader;
};

// TODO(brett): Need to make sure things are named consistently 
typedef ShaderHandler BGLShaderHandler;


class BGLSprite
{
public:

	
	// 0, 0 is top right hand corner of sprite
	// diffuseTag and normalTag are to have the same coordinates
	static BGLSprite Create(std::string diffuseTag, std::string normalTag, float w, float h, int32_t order, uint32_t frameCount, BGLRect *frames);

	// TODO(brett): for batching if it is needed... NOT finished
	static void BeginRender();
	void BatchRender();
	static void EndRender();

	// TODO(brett): might get some speed boost if the buffer is changed from data to an 
	// index buffer. The uv could then have their own buffer... 
	void Render();

	// TODO(brett): Get these finished after we have sprites on the screen
	void Trasnform(glm::vec2 position);
	void Rotate(float angle);
	void Scale(glm::vec2 scale);

	void ResetTransform();

	void SetAnimationFrame(uint32_t frameIndex); // set the curretn frame MOD max frames;
	void SetRelativeAnimationFrame(uint32_t frameIndex); // move relative to the current frame and set the currentframe
	void AdvanceAnimationFrame();

public:
	BGLSpriteVertex quad[6];
	
	glm::vec3 transform;
	float rotation;
	glm::vec2 scale;
	
	// this is the model transform for moving/rotating/scaling
	glm::mat4 model;

	// this is for ordering the background withe foreground. 
	int32_t order; 

	BGLRect viewRect;
	BGLRect frames[SPRITE_MAX_FRAMES]; // max of 64 frames per animation
	BGLRect frameRect;
	uint32_t totalFrames;
	uint32_t currentFrame;

	int32_t textureWidth;
	int32_t textureHeight;

	std::string diffuseTextureTag;
	std::string normalTextureTag;

	// TODO(brett): this is for a newer version of BGL in preperation for batching
	BGLShader  *shader;
	BGLTexture *diffuseTexture;
	BGLTexture *normalTexture;

	GLuint vbo;

	uint64_t spritesSz;
	uint64_t spriteOffset;
	static GLuint BGLSpriteVBO;
};


typedef struct 
{
	GLfloat x, y;
	GLfloat scaleX, scaleY;
	GLfloat rotatex, rotatey, rotatez;
	GLfloat tx, ty, tw, th;
	GLint	textureIndex;

} BGLSpriteBatchVertex;

class BGLSpriteBatch
{
public:
	GLuint VAO;
	GLuint GeometryVBO;
	GLuint SpriteVBO;

	glm::mat4 view;

	BGLSpriteBatchVertex spriteArray[BGL_BATCH_SPRITE_MAX];

	// the number of sprites that have been put into the buffer
	uint32_t spritesIndex;

	BGLShader shader;
	

public:

	BGLSpriteBatch()
	{
		shader = shader;
		spritesIndex = 0;

		glGenVertexArrays(1, &VAO);
		if(!VAO)
		{
			std::cout << "ERROR: Could not create opengl VAO for spriteBatch" << std::endl;
		}

		glBindVertexArray(VAO);
		glGenBuffers(1, &GeometryVBO);
		if(!GeometryVBO)
		{
			std::cout << "ERROR: Could not create opengl VBO for spriteBatch" << std::endl;
		}


		glBindBuffer(GL_ARRAY_BUFFER, GeometryVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(sizeof(GLfloat)*2));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &SpriteVBO);
		if(!SpriteVBO)
		{
			std::cout << "ERROR: Could not create opengl VBO for spriteBatch" << std::endl;
		}
		glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(BGLSpriteBatchVertex)*BGL_BATCH_SPRITE_MAX, 0, GL_STREAM_DRAW);

		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteBatchVertex), (void *)0);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteBatchVertex), (void *)(sizeof(GLfloat)*2));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteBatchVertex), (void *)(sizeof(GLfloat)*4));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteBatchVertex), (void *)(sizeof(GLfloat)*7));
		glVertexAttribPointer(6, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(BGLSpriteBatchVertex), (void *)(sizeof(GLfloat)*11));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glVertexAttribDivisor(0, 0);
		//glVertexAttribDivisor(1, 0);

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(Frame.vao);
	}


	void BeginBatch()
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);

		glUseProgram(shader.id);

		BGLTexture t = TextureHandler::Get("spritesheet");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.id);
	}

	void DrawSprite(BGLSprite sprite, glm::vec2 posCenter, glm::vec2 scale, glm::vec3 rotation)
	{
		if(spritesIndex == BGL_BATCH_SPRITE_MAX)
			return;

		BGLSpriteBatchVertex vert = { posCenter.x, posCenter.y,
									  scale.x, scale.y,
									  rotation.x, rotation.y, rotation.z,
									  sprite.frameRect.x, sprite.frameRect.y, 
									  sprite.frameRect.w, sprite.frameRect.h,
									  0 };

		spriteArray[spritesIndex] = vert;

		spritesIndex += 1;

	}

	void DrawSprite(BGLSprite *sprite, glm::vec2 posCenter, glm::vec2 scale, glm::vec3 rotation)
	{
		if(spritesIndex == BGL_BATCH_SPRITE_MAX)
			return;

		BGLSpriteBatchVertex vert = { posCenter.x, posCenter.y,
									  scale.x, scale.y,
									  rotation.x, rotation.y, rotation.z,
									  sprite->frameRect.x, sprite->frameRect.y, 
									  sprite->frameRect.w, sprite->frameRect.h,
									  0 };

		spriteArray[spritesIndex] = vert;

		spritesIndex += 1;
	}

	void RenderBatch(glm::mat4 projection, glm::mat4 view)
	{
		// TODO(brett): Get the shader and texture tracking working properly so we dont have to set
		// this evertytime we draw
		glUseProgram(shader.id);

		GLint projectionLocation = glGetUniformLocation(shader.id, "projection");
		GLint viewLocation = glGetUniformLocation(shader.id, "view");
		GLint textureLocation = glGetUniformLocation(shader.id, "textureSampler");

		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (GLfloat *)&projection[0]);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (GLfloat *)&view[0]);
		glUniform1d(textureLocation, 0);

		glBufferSubData(GL_ARRAY_BUFFER, 
						0,
						spritesIndex * sizeof(BGLSpriteBatchVertex),
						(void *)&spriteArray[0]);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, spritesIndex);

		glBindVertexArray(Frame.vao);
	}


	void ClearBatch()
	{
		spritesIndex = 0;
	}


private:

	static GLfloat verts[24];
};