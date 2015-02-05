
#include "BGL.h"

inline BGLRect
BGLRectMake(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	BGLRect rect = {x, y, w, h};
	return (rect);
}

bool
operator==(BGLRect a, BGLRect b)
{
	return(a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h);
}

bool 
BGLRectOverlap(BGLRect a, BGLRect b)
{
	if(a.x > b.x + b.w) return false;
	if(a.x + a.w < b.x) return false;

	if(a.y > b.y + b.h) return false;
	if(a.y + a.h < b.y) return false;
	return true;
}

void InitializeFrame(float left, float right, float bottom, float top, float near, float far)
{
	Frame.id = 0; // The screen
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &Frame.vao);
	glBindVertexArray(Frame.vao);

	BGLProjection = glm::ortho(left, right, bottom, top, near, far);

	BGLCamera = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
}


BGLLayer BGLLayerManager::layers[BGL_MAX_LAYERS];
uint32_t BGLLayerManager::layerSz = 0;
LayerVertex BGLLayerManager::quad[6];
GLuint BGLLayerManager::vbo = 0;

GLint BGLLayerManager::LayerPosition0;
GLint BGLLayerManager::LayerPosition1;
GLint BGLLayerManager::LayerPosition2;
GLint BGLLayerManager::LayerPosition3;
GLint BGLLayerManager::LayerPosition4;


void BGLLayerManager::InitLayers(uint32_t width, uint32_t height)
{

	layerSz = BGL_MAX_LAYERS;

	quad[0].x = -1.0f; quad[0].y =  1.0f; quad[0].u =  0.0f; quad[0].v =  1.0f;
	quad[1].x = -1.0f; quad[1].y = -1.0f; quad[1].u =  0.0f; quad[1].v =  0.0f;
	quad[2].x =  1.0f; quad[2].y =  1.0f; quad[2].u =  1.0f; quad[2].v =  1.0f;
	quad[3].x =  1.0f; quad[3].y =  1.0f; quad[3].u =  1.0f; quad[3].v =  1.0f;
	quad[4].x = -1.0f; quad[4].y = -1.0f; quad[4].u =  0.0f; quad[4].v =  0.0f;
	quad[5].x =  1.0f; quad[5].y = -1.0f; quad[5].u =  1.0f; quad[5].v =  0.0f;

	glGenBuffers(1, &vbo);
	if(vbo == 0)
	{
		std::cout << "ERROR: Could not generate the vbo for the framebuffers" << std::endl;
	}

	BGLShaderHandler::Load("finalFrame", "shaders/layer.vertex", "shaders/layer.fragment");
	BGLShaderHandler::Enable("finalFrame");
	BGLShader shader = BGLShaderHandler::Get("finalFrame");

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LayerPosition0 = glGetUniformLocation(shader.id, "layer0");
	LayerPosition1 = glGetUniformLocation(shader.id, "layer1");

	for(int32_t i = 0; i < BGL_MAX_LAYERS; ++i)
	{
		BGLLayer layer = {};

		glGenFramebuffers(1, &layer.frameId);
		if(layer.frameId == 0)
		{
			std::cout << "WARNING: Could not create opengl framebuffer." << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, layer.frameId);


		glGenTextures(1, &layer.texId);
		if(layer.texId == 0)
		{
			std::cout << "WARNING: Could not create opengl writable texture." << std::endl;
		}

		layer.width = width;
		layer.height = height;

		glBindTexture(GL_TEXTURE_2D, layer.texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, layer.width, layer.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, layer.texId, 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: Framebuffer is invalid." << std::endl;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		layers[i] = layer;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void BGLLayerManager::UseLayer(uint32_t layer)
{
	if(layer > layerSz)
	{
		std::cout << "Cannot bind layer that does not exist" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, layers[layer].frameId);
}

void BGLLayerManager::RenderLayers()
{

	BGLShaderHandler::Enable("finalFrame");
	BGLShader shader = BGLShaderHandler::Get("finalFrame");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	LayerPosition0 = glGetUniformLocation(shader.id, "layer0");
	LayerPosition1 = glGetUniformLocation(shader.id, "layer1");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, layers[1].texId);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, layers[0].texId);

	glUniform1d(LayerPosition0, 0);
	glUniform1d(LayerPosition1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LayerVertex), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(LayerVertex), (void *)(sizeof(GLfloat)*2));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void BGLLayerManager::Cleanup()
{
	for(uint32_t i = 0; i < layerSz; ++i)
	{
		glDeleteFramebuffers(1, &(layers[i].frameId));
	}
}


void  SetCamera(glm::vec2 const offset)
{
	BGLCamera = glm::translate(glm::mat4(), glm::vec3(-offset, 0.0f));
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

	textures[tag] = bglTexture;
}


void TextureHandler::Bind(std::string tag, int32_t textureSlot)
{
	BGLTexture texture = textures[tag];

	if(texture.isBound && textureSlot == texture.textureUnit)
	{
		return;
	}

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

	std::cout << "Compiled [" << vertex << ", " << fragment << "]: " << shader.id << std::endl;

}


void ShaderHandler::Enable(std::string tag)
{

	if(shaders[tag].id == 0)
	{
		std::cout << "Could not bind shader with given tag: " << tag << std::endl;
		return;
	}

	glUseProgram(shaders[tag].id);
	shaders[tag].isBound = true;
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


bool BGLShaderHandler::IsBound(std::string tag)
{
	return (tag == boundShader);
}

BGLShader BGLShaderHandler::Get(std::string tag)
{
	return (shaders[tag]);
}



GLuint BGLSprite::BGLSpriteVBO = 0;

void BGLSprite::Render()
{
	// TODO(brett): make sure the sprite is bound
#pragma message(__LOC__ "This should be set before the render AND NOT done every frame")
	BGLShader shader = BGLShaderHandler::Get("sprite");
	BGLShaderHandler::Enable("sprite");

	//BGLTexture texture = TextureHandler::Get(diffuseTextureTag);
	//BGLTexture normal = TextureHandler::Get(normalTextureTag);

	// NOTE(brett): this is a bit redundant... perhaps the texture will have the tag (noted in BGL.h)
	TextureHandler::Bind(diffuseTextureTag, 0);
	//TextureHandler::Bind(normalTextureTag, 1); // This throws asn exception when passing an empty string as a normal

	GLuint projectionLocation = glGetUniformLocation(shader.id, "projection");
	GLuint modelLocation = glGetUniformLocation(shader.id, "model");
	GLuint cameraLocation = glGetUniformLocation(shader.id, "camera");
	GLuint textureLocation = glGetUniformLocation(shader.id, "textureSampler");
	GLuint normalLocation = glGetUniformLocation(shader.id, "normalSampler");

	// binding this texture to texture unit 0
	glUniform1d(textureLocation, 0);
	//glUniform1d(normalLocation, 1);

	// TODO(brett): no lights yet, that will have to be done in the batching
	// NOTE(brett): maybe the lights are static parts of the sprite that can be turned
	// on and off?

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (GLfloat *)&BGLProjection[0]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, (GLfloat *)&model);
	glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, (GLfloat *)&BGLCamera);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteVertex), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BGLSpriteVertex), (void *)(sizeof(GLfloat)*2));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// NOTE(brett): Do I reset the transform after every draw?
}

void BGLSprite::SetAnimationFrame(uint32_t frameIndex)
{
    uint32_t newFrame = frameIndex % totalFrames;
    currentFrame = newFrame;

    // TODO(brett): move this code out int the animation frame functions
	float uvFrameX = (frames[currentFrame].x / (float)textureWidth);
	float uvFrameY = (frames[currentFrame].y / (float)textureHeight);

	float uvFrameW = uvFrameX + (frames[currentFrame].w / (float)textureWidth);
	float uvFrameH = uvFrameY + (frames[currentFrame].h / (float)textureHeight);

	this->quad[0].pos.x = viewRect.x;
	this->quad[0].pos.y = viewRect.y;
	this->quad[0].uv.x = uvFrameX;
	this->quad[0].uv.y = uvFrameY;

	this->quad[1].pos.x = viewRect.x;
	this->quad[1].pos.y = viewRect.y+viewRect.h;
	this->quad[1].uv.x = uvFrameX;
	this->quad[1].uv.y = uvFrameH;

	this->quad[2].pos.x = viewRect.x+viewRect.w;
	this->quad[2].pos.y = viewRect.y;
	this->quad[2].uv.x = uvFrameW;
	this->quad[2].uv.y = uvFrameY;

	this->quad[3].pos.x = viewRect.x+viewRect.w;
	this->quad[3].pos.y = viewRect.y;
	this->quad[3].uv.x = uvFrameW;
	this->quad[3].uv.y = uvFrameY;

	this->quad[4].pos.x = viewRect.x;
	this->quad[4].pos.y = viewRect.y+viewRect.h;
	this->quad[4].uv.x = uvFrameX;
	this->quad[4].uv.y = uvFrameH;

	this->quad[5].pos.x = viewRect.x+viewRect.w;
	this->quad[5].pos.y = viewRect.y+viewRect.h;
	this->quad[5].uv.x = uvFrameW;
	this->quad[5].uv.y = uvFrameH;

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(this->quad), &this->quad[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void BGLSprite::AdvanceAnimationFrame()
{
	SetAnimationFrame(currentFrame+1);
}

void BGLSprite::SetRelativeAnimationFrame(uint32_t frameIndex)
{
	SetAnimationFrame(currentFrame + frameIndex);
}


BGLSprite BGLSprite::Create(std::string diffuseTag, std::string normalTag, float w, float h, int32_t order, uint32_t frameCount, BGLRect *frames)
{
	BGLSprite sprite = {};

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

	sprite.viewRect = BGLRectMake(-w/2.0f, -h/2.0f, w, h);
	sprite.order = order;
	sprite.totalFrames = frameCount;
	sprite.currentFrame = 0;
	

	// copy the frames 
	for(uint32_t i = 0; i < frameCount; ++i)
	{
		//sprite.frames[i] = BGLRectMake(frames[i].x, frames[i].y, frames[i].w, frames[i].h);
		sprite.frames[i] = frames[i];
	}


	// TODO(brett): move this code out int the animation frame functions
	float uvFrameX = (sprite.frames[sprite.currentFrame].x / (float)sprite.textureWidth);
	float uvFrameY = (sprite.frames[sprite.currentFrame].y / (float)sprite.textureHeight);

	float uvFrameW = uvFrameX + (sprite.frames[sprite.currentFrame].w / (float)sprite.textureWidth);
	float uvFrameH = uvFrameY + (sprite.frames[sprite.currentFrame].h / (float)sprite.textureHeight);

	sprite.quad[0].pos.x = sprite.viewRect.x;
	sprite.quad[0].pos.y = sprite.viewRect.y;
	sprite.quad[0].uv.x = uvFrameX;
	sprite.quad[0].uv.y = uvFrameY;

	sprite.quad[1].pos.x = sprite.viewRect.x;
	sprite.quad[1].pos.y = sprite.viewRect.y+sprite.viewRect.h;
	sprite.quad[1].uv.x = uvFrameX;
	sprite.quad[1].uv.y = uvFrameH;

	sprite.quad[2].pos.x = sprite.viewRect.x+sprite.viewRect.w;
	sprite.quad[2].pos.y = sprite.viewRect.y;
	sprite.quad[2].uv.x = uvFrameW;
	sprite.quad[2].uv.y = uvFrameY;

	sprite.quad[3].pos.x = sprite.viewRect.x+sprite.viewRect.w;
	sprite.quad[3].pos.y = sprite.viewRect.y;
	sprite.quad[3].uv.x = uvFrameW;
	sprite.quad[3].uv.y = uvFrameY;

	sprite.quad[4].pos.x = sprite.viewRect.x;
	sprite.quad[4].pos.y = sprite.viewRect.y+sprite.viewRect.h;
	sprite.quad[4].uv.x = uvFrameX;
	sprite.quad[4].uv.y = uvFrameH;

	sprite.quad[5].pos.x = sprite.viewRect.x+sprite.viewRect.w;
	sprite.quad[5].pos.y = sprite.viewRect.y+sprite.viewRect.h;
	sprite.quad[5].uv.x = uvFrameW;
	sprite.quad[5].uv.y = uvFrameH;

	glm::vec2 f;
	glGenBuffers(1, &sprite.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(sprite.quad),
				&sprite.quad[0],
				GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return sprite;
}


GLfloat BGLSpriteBatch::verts[8] = {
	-0.5f,  0.5f,
	-0.5f, -0.5f,
	 0.5f,  0.5f, 
	 0.5f, -0.5f
};

