
#include "BGL.h"

BGLRect BGLRectMake(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	BGLRect rect = {x, y, w, h};
	return (rect);
}

bool operator==(BGLRect a, BGLRect b)
{
	return(a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h);
}


void
InitializeFrame(float left, float right, float bottom, float top, float near, float far)
{
	Frame.id = 0; // The screen
	
	glGenVertexArrays(1, &Frame.vao);
	glBindVertexArray(Frame.vao);

	Projection = glm::ortho(left, right, bottom, top, near, far);
}


std::unordered_map<std::string, BGLTexture> TextureHandler::textures;
std::unordered_map<std::string, bool> TextureHandler::boundTextures;

void TextureHandler::Load(std::string tag, std::string filename)
{
	SDL_Surface *image = IMG_Load(filename.c_str());
	if(!image)
	{
		std::cout << "Image was not loaded: " << filename << std::endl;
		return;
	}

	BGLTexture bglTexture = {};
	bglTexture.width = image->w;
	bglTexture.height = image->h;

	glGenTextures(1, &bglTexture.id);
	if(bglTexture.id == 0)
	{
		std::cout << "Could not create a texture on current GL context" << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, bglTexture.id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				bglTexture.width,
				bglTexture.height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);
}


void TextureHandler::Bind(std::string tag, int32_t textureSlot)
{
	BGLTexture texture = textures[tag];

	if(texture.isBound && textureSlot == texture.textureUnit)
		return;

	if(texture.id == 0)
	{
		std::cout << "Could not bind texture with given tag: " << tag << std::endl;
		return;
	}


	texture.isBound = true;
	texture.textureUnit = textureSlot;
	glActiveTexture(GL_TEXTURE0+textureSlot);
	glBindTexture(GL_TEXTURE_2D, texture.id);
}

void TextureHandler::Unbind(std::string tag)
{
	BGLTexture texture = textures[tag];

	if(texture.id == 0)
	{
		std::cout << "Could not bind texture with given tag: " << tag << std::endl;
		return;
	}

	glActiveTexture(GL_TEXTURE0+texture.textureUnit);
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.isBound = false;
	texture.textureUnit = 0;
}

BGLTexture TextureHandler::Get(std::string tag)
{
	BGLTexture result = textures[tag];

	if(result.id == 0)
	{
		std::cout << "Warning: either the texture wasnt created or it doesnt exist" << std::endl;
		std::cout << "\tGiven tag: " << tag << std::endl;
	}

	return (result);
}


std::unordered_map<std::string, BGLShader> ShaderHandler::shaders;
std::string ShaderHandler::boundShader;

void ShaderHandler::Load(std::string tag, std::string vertex, std::string fragment)
{
	FILE* vertexfd = fopen(vertex.c_str(), "r");
	FILE* fragmentfd = fopen(fragment.c_str(), "r");

	if(!vertexfd || !fragmentfd)
	{
		std::cout << "Could not load given shader files: " << std::endl;
		std::cout << vertex << std::endl;
		std::cout << fragment << std::endl;
		return;
	}

	fseek(vertexfd, 0, SEEK_END);
	int64_t vertexSz = ftell(vertexfd);
	fseek(vertexfd, 0, SEEK_SET);

	fseek(fragmentfd, 0, SEEK_END);
	int64_t fragmentSz = ftell(fragmentfd);
	fseek(fragmentfd, 0, SEEK_SET);

	char *vertexFileBuffer = (char *)malloc(sizeof(char)*vertexSz);
	char *fragmentFileBuffer = (char *)malloc(sizeof(char)*fragmentSz);

	memset(vertexFileBuffer, 0, vertexSz);
	memset(fragmentFileBuffer, 0, fragmentSz);

	fread(vertexFileBuffer, vertexSz, sizeof(char), vertexfd);
	fread(fragmentFileBuffer, fragmentSz, sizeof(char), fragmentfd);

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	if(!vertexId || !fragmentId)
	{
		std::cout << "Could not create the shader programs to compile final shader" << std::endl;
		std::cout << vertex << std::endl;
		std::cout << fragment << std::endl;
		return;
	}

	glShaderSource(vertexId, 1, (const GLchar **)&vertexFileBuffer, (GLint *)&vertexSz);
	glShaderSource(fragmentId, 1, (const GLchar **)&fragmentFileBuffer, (GLint *)&fragmentSz);

	free(vertexFileBuffer);
	free(fragmentFileBuffer);

	glCompileShader(vertexId);
	glCompileShader(fragmentId);

	GLint compiled;

	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE)
	{
		GLint logSz;
		glGetShaderiv(vertexId, GL_INFO_LOG_LENGTH, &logSz);

		char *log = (char *)malloc(sizeof(char) * logSz);
		memset(log, 0, logSz);
		glGetShaderInfoLog(vertexId, logSz, 0, log);

		std::cout << "Vertex Shader Compilation: " << std::endl;
		std::cout << log << std::endl;

		free(log);
	}

	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE)
	{
		GLint logSz;
		glGetShaderiv(fragmentId, GL_INFO_LOG_LENGTH, &logSz);

		char *log = (char *)malloc(sizeof(char) * logSz);
		memset(log, 0, logSz);
		glGetShaderInfoLog(fragmentId, logSz, 0, log);

		std::cout << "Fragment Shader Compilation: " << std::endl;
		std::cout << log << std::endl;

		free(log);
	}


	GLuint shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexId);
	glAttachShader(shaderId, fragmentId);
	glLinkProgram(shaderId);

	GLint linked;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &linked);
	if(linked == GL_FALSE)
	{
		GLint logSz;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &logSz);

		char *log = (char *)malloc(sizeof(char) * logSz);
		memset(log, 0, logSz);
		glGetShaderInfoLog(shaderId, logSz, 0, log);

		std::cout << "Linker Status: " << std::endl;
		std::cout << log << std::endl;
		return;
	}

	glDeleteProgram(vertexId);
	glDeleteProgram(fragmentId);

	BGLShader shader = {};
	shader.id = shaderId;

	shaders[tag] = shader;
}


void ShaderHandler::Enable(std::string tag)
{
	BGLShader shader = shaders[tag];
	if(shader.id == 0)
	{
		std::cout << "Could not bind shader with given tag: " << tag << std::endl;
		return;
	}

	glUseProgram(shader.id);
	shader.isBound = true;
	boundShader = tag;
}


void ShaderHandler::Disable(std::string tag)
{
	BGLShader shader = shaders[tag];
	if(shader.id == 0)
	{
		std::cout << "Could not unbind shader with given tag: " << tag << std::endl;
		return;
	}

	shader.isBound = false;
	glUseProgram(0);
	boundShader = "";
}


bool ShaderHandler::IsBound(std::string tag)
{
	return (tag == boundShader);
}

BGLShader ShaderHandler::Get(std::string tag)
{
	return (shaders[tag]);
}


void Sprite::Render()
{
	// TODO(brett): make sure the sprite is bound
#pragma message(__LOC__ "This should be set before the render AND NOT done every frame")
	BGLShader shader = ShaderHandler::Get("sprite");

	//BGLTexture texture = TextureHandler::Get(diffuseTextureTag);
	//BGLTexture normal = TextureHandler::Get(normalTextureTag);

	// NOTE(brett): this is a bit redundant... perhaps the texture will have the tag (noted in BGL.h)
	TextureHandler::Bind(diffuseTextureTag, 0);
	TextureHandler::Bind(normalTextureTag, 1);

	GLuint projectionLocation = glGetUniformLocation(shader.id, "projection");
	GLuint modelLocation = glGetUniformLocation(shader.id, "modeltransform");
	GLuint textureLocation = glGetUniformLocation(shader.id, "textureSampler");
	GLuint normalLocation = glGetUniformLocation(shader.id, "normalSampler");

	// binding this texture to texture unit 0
	glUniform1d(textureLocation, 0);
	glUniform1d(normalLocation, 1);

	// TODO(brett): no lights yet, that will have to be done in the batching
	// NOTE(brett): maybe the lights are static parts of the sprite that can be turned
	// on and off?

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (GLfloat *)&Projection[0]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, (GLfloat *)&modelTransform);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteVertex), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteVertex), (void *)(sizeof(GLfloat)*2));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// NOTE(brett): Do I reset the transform after every draw?
}



Sprite Sprite::Create(std::string diffuseTag, std::string normalTag, float w, float h, int32_t order, uint32_t frameCount, BGLRect *frames)
{
	Sprite sprite = {};

	if(frameCount > SPRITE_MAX_FRAMES)
	{
		std::cout << "Warning: Too many frames in the animation. Max frames: " << SPRITE_MAX_FRAMES << std::endl;
		frameCount = SPRITE_MAX_FRAMES;
	}

	if(frameCount == 0)
	{
		std::cout << "Error: Frame count cannot be zero!" << std::endl;
		frameCount = 1;
	}


	sprite.diffuseTextureTag = diffuseTag;
	sprite.normalTextureTag = normalTag;

	BGLTexture texture = TextureHandler::Get(sprite.diffuseTextureTag);
	sprite.textureWidth = texture.width;
	sprite.textureHeight = texture.height;

	sprite.viewRect = BGLRectMake(0, 0, w, h);
	sprite.order = order;
	sprite.totalFrames = frameCount;
	sprite.currentFrame = 0;
	
	for(int i = 0; i < frameCount; ++i)
	{
		//sprite.frames[i] = BGLRectMake(frames[i].x, frames[i].y, frames[i].w, frames[i].h);
		sprite.frames[i] = frames[i];
	}


	// TODO(brett): move this code out int the animation frame functions
	float uvFrameX = (sprite.frames[0].x / (float)sprite.textureWidth);
	float uvFrameY = (sprite.frames[0].w / (float)sprite.textureHeight);

	float uvFrameW = uvFrameX + (sprite.frames[0].w / (float)sprite.textureWidth);
	float uvFrameH = uvFrameY + (sprite.frames[0].h / (float)sprite.textureHeight);

	sprite.quad[0].pos.x = 0.0f;
	sprite.quad[0].pos.y = 0.0f;
	sprite.quad[0].uv.x = uvFrameX;
	sprite.quad[0].uv.y = uvFrameY;

	sprite.quad[1].pos.x = 0.0f;
	sprite.quad[1].pos.y = h;
	sprite.quad[1].uv.x = uvFrameX;
	sprite.quad[1].uv.y = uvFrameH;

	sprite.quad[2].pos.x = w;
	sprite.quad[2].pos.y = 0.0f;
	sprite.quad[2].uv.x = uvFrameW;
	sprite.quad[2].uv.y = uvFrameY;

	sprite.quad[3].pos.x = w;
	sprite.quad[3].pos.y = 0.0f;
	sprite.quad[3].uv.x = uvFrameW;
	sprite.quad[3].uv.y = uvFrameY;

	sprite.quad[4].pos.x = 0.0f;
	sprite.quad[4].pos.y = h;
	sprite.quad[4].uv.x = uvFrameX;
	sprite.quad[4].uv.y = uvFrameH;

	sprite.quad[5].pos.x = w;
	sprite.quad[5].pos.y = h;
	sprite.quad[5].uv.x = uvFrameW;
	sprite.quad[5].uv.y = uvFrameH;


	glGenBuffers(1, &sprite.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(sprite.quad),
				&sprite.quad[0],
				GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return sprite;
}
