#include "Header.h"

#ifdef _WIN32
#undef main
#endif // _WIN32

//using namespace std;
int main(int argc, char * args[]) {
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Scene textures
	LTexture *gStreamingTexture = new LTexture;

	//Animation stream
	DataStream *gDataStream = new DataStream;

	if (!init(gWindow, gRenderer))
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia(gStreamingTexture, gDataStream, gRenderer))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type==SDL_QUIT)
					{
						quit = true;
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				gStreamingTexture->lockTexture();
				gStreamingTexture->copyPixels(gDataStream->getBuffer());
				gStreamingTexture->unlockTexture();

				gStreamingTexture->render(gRenderer, (SCREEN_WIDTH - gStreamingTexture->getWidth()) / 2,
										(SCREEN_HEIGHT - gStreamingTexture->getHeight()) / 2, NULL, 0.0, NULL, SDL_FLIP_NONE);

				SDL_RenderPresent(gRenderer);
			}

		}
	}
	close(gStreamingTexture, gDataStream, gWindow, gRenderer);
	delete gStreamingTexture;
	delete gDataStream;
	return 0;
}