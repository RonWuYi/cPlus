#include "Header.h"

bool LTexture::loadFromFile(SDL_Renderer* gRenderer, std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface==NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
			SDL_PIXELFORMAT_RGBA8888,
			0);

		if (formattedSurface==NULL)
		{
			printf("Unable to convert loaded surface to display format! %s\n", SDL_GetError());
		}
		else
		{
			newTexture = SDL_CreateTexture(gRenderer,
				SDL_PIXELFORMAT_RGBA8888,
				SDL_TEXTUREACCESS_STREAMING,
				formattedSurface->w,
				formattedSurface->h);
			
			if (newTexture==NULL)
			{
				printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

				SDL_LockTexture(newTexture,
					&formattedSurface->clip_rect,
					&mPixels,
					&mPitch);
				
				memcpy(mPixels,
					formattedSurface->pixels,
					formattedSurface->pitch * formattedSurface->h);

				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;

				Uint32* pixels = (Uint32*)mPixels;
				int pixelCount = (mPitch / 4) * mHeight;
				//Map colors
				Uint32 colorKey = SDL_MapRGB(formattedSurface->format,
					0,
					0xFF,
					0xFF);
				Uint32 transparent = SDL_MapRGBA(formattedSurface->format,
					0x00,
					0xFF,
					0xFF,
					0x00);

				for (int i = 0; i < pixelCount; i++)
				{
					if (pixels[i]==colorKey)
					{
						pixels[i] = transparent;
					}
				}

				SDL_UnlockTexture(newTexture);
				mPixels = NULL;
			}
			SDL_FreeSurface(formattedSurface);
		}
		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

bool LTexture::createBlank(SDL_Renderer* gRenderer, int width, int height)
{
	mTexture = SDL_CreateTexture(gRenderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (mTexture == NULL)
	{
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
	}

	else
	{
		mWidth = width;
		mHeight = height;
	}

	return mTexture != NULL;
}

void LTexture::copyPixels(void* pixels)
{
	//Texture is locked
	if (mPixels != NULL)
	{
		//Copy to locked pixels
		memcpy(mPixels, pixels, mPitch * mHeight);
	}
}

int LTexture::getPitch()
{
	return mPitch;
}

Uint32 LTexture::getPixel32(unsigned int x, unsigned int y)
{
	Uint32* pixels = (Uint32*)mPixels;
	return pixels[(y*(mPitch/4)) + x];
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center=NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip!=NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::lockTexture()
{
	bool success = true;

	if (mPixels!=NULL)
	{
		printf("Texture is already locked!\n");
		success = false;
	}
	else
	{
		if (SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0)
		{
			printf("Unable to lock texture! %s\n", SDL_GetError());
			success = false;
		}
	}
	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if (mPixels == NULL)
	{
		printf("Texture is not locked!\n");
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture(mTexture);
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* LTexture::getPixels()
{
	return mPixels;
}


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

inline bool DataStream::loadMedia()
{
	bool success = true;

	for (int i = 0; i < 4; ++i)
	{
		char path[64] = "";
		/*sprintf(path, "pic/foo_walk_%d.png", i);*/
		sprintf_s(path, "pic/foo_walk_%d.png", i);

		SDL_Surface* loadedSurface = IMG_Load(path);

		if (loadedSurface==NULL)
		{
			printf("Unable to load %s! SDL_image error: %s\n", path, IMG_GetError());
			success = false;
		}
		else
		{
			mImages[i] = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		}
		SDL_FreeSurface(loadedSurface);
	}

	return success;
}

void DataStream::free()
{
	for (int i = 0; i < 4; i++)
	{
		SDL_FreeSurface(mImages[i]);
	}
}

void* DataStream::getBuffer()
{
	--mDelayFrames;
	if (mDelayFrames==0)
	{
		++mCurrentImage;
		mDelayFrames = 4;
	}

	if (mCurrentImage==4)
	{
		mCurrentImage = 0;
	}
	return mImages[mCurrentImage]->pixels;
}


DataStream::DataStream()
{
	mImages[0] = NULL;
	mImages[1] = NULL;
	mImages[2] = NULL;
	mImages[3] = NULL;

	mCurrentImage = 0;
	mDelayFrames = 4;
}

bool init(SDL_Window* gWindow, SDL_Renderer* gRenderer)
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Seed random
		srand(SDL_GetTicks());

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
		if (gRenderer==NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			int imgFlags = IMG_INIT_PNG;

			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}
		}
	}
	return success;
}

bool loadMedia(LTexture* gStreamingTexture, DataStream* gDataStream, SDL_Renderer* gRenderer)
{
	bool success = true;

	if (!gStreamingTexture->createBlank(gRenderer, 64, 205))
	{
		printf("Failed to create streaming texture!\n");
		success = false;
	}

	if (!gDataStream->loadMedia())
	{
		printf("Unable to load data stream!\n");
		success = false;
	}
	return success;
}

void close(LTexture* gStreamingTexture, DataStream* gDataStream, SDL_Window* gWindow, SDL_Renderer* gRenderer)
{
	gStreamingTexture->free();
	gDataStream->free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
