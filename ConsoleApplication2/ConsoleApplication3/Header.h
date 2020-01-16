#pragma once
#ifndef __HEADER__
#define __HEADER__

#include <iostream>
#include <string>
#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public:
	LTexture();
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer* gRenderer, std::string path);

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	bool createBlank(SDL_Renderer* gRenderer, int width, int height);

	//Deallocate texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle,
		SDL_Point* center, SDL_RendererFlip flip);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Pixel manipulators
	bool lockTexture();
	bool unlockTexture();
	void* getPixels();
	void copyPixels(void* pixels);
	int getPitch();
	Uint32 getPixel32(unsigned int x, unsigned int y);

private:
	//The actual hardware texture
	SDL_Texture* mTexture;
	void* mPixels;
	int mPitch;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class DataStream
{
public:
	DataStream();
	//~DataStream();

	bool loadMedia();

	void free();

	void* getBuffer();

private:
	//Internal data
	SDL_Surface* mImages[4];
	int mCurrentImage;
	int mDelayFrames;
};

//DataStream::~DataStream()
//{
//}

//Starts up SDL and creates window
bool init(SDL_Window* gWindow, SDL_Renderer* gRenderer);

bool loadMedia(LTexture* gStreamingTexture, DataStream* gDataStream, SDL_Renderer* gRenderer);

//Frees media and shuts down SDL
void close(LTexture* gStreamingTexture, DataStream* gDataStream, SDL_Window* gWindow, SDL_Renderer* gRenderer);

#endif // !__HEADER__

