#include "Common.h"

#include "Screen.h"

namespace TTSP
{
    namespace
    {
        const char *const WindowTitle = "TexTageScorePlayer";

        const int ScreenWidth = 800;
        const int ScreenHeight = 600;
    }

    int run(int argc, char **argv)
    {
        try
        {
            SDL2pp::SDL sdl(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

            SDL2pp::Window window(WindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
            SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            renderer.SetLogicalSize(ScreenWidth, ScreenHeight);

            Screen screen;
            std::shared_ptr<Score> score;
            ScorePlayback scorePlayback;

            SDL_Event e;
            bool quit = false;

            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point prev;

            std::chrono::steady_clock::time_point prevFrameMeasurePoint = now;

            while(!quit)
            {
                while(SDL_PollEvent(&e))
                {
                    switch(e.type)
                    {
                    case SDL_QUIT:
                        {
                            quit = true;
                        }
                        break;
                    }
                }

                prev = now;
                now = std::chrono::steady_clock::now();
                const std::chrono::nanoseconds interval = now - prev;

                scorePlayback.progress(interval);

                screen.render(renderer, &scorePlayback, interval);

                // FPS measure
                const std::chrono::milliseconds frameMeasureInterval = std::chrono::duration_cast<std::chrono::milliseconds>(now - prevFrameMeasurePoint);
                if(frameMeasureInterval.count() >= 5000)
                {
                    const uint64_t fpsBy100 = screen.pullFrameCount() * 100ull * 1000ull / static_cast<uint64_t>(frameMeasureInterval.count());

                    char buf[50];
                    sprintf_s(buf, "%s (%llu.%02llu fps)", WindowTitle, fpsBy100 / 100, fpsBy100 % 100);

                    window.SetTitle(buf);

                    prevFrameMeasurePoint = now;
                }
            }
        }
        catch(const SDL2pp::Exception &)
        {
            return 1;
        }
        catch(const std::exception &)
        {
            return 1;
        }

        return 0;
    }
}

extern "C" int main(int argc, char **argv)
{
    return TTSP::run(argc, argv);
}
