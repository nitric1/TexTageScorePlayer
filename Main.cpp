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

    int run(int /*argc*/, char **/*argv*/)
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

            score.reset(new Score());
            scorePlayback.score(score);

            SDL_Event e;
            bool quit = false;

            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point prev;

            std::chrono::steady_clock::time_point prevFrameMeasurePoint = now;

            uint64_t fpsBy100 = 0;

            bool playing = false;

            auto updateTitle = [&window, &fpsBy100, &scorePlayback]()
            {
                window.SetTitle(fmt::format("{}: HS {} ({}.{:02} fps)", WindowTitle, scorePlayback.hiSpeed(), fpsBy100 / 100, fpsBy100 % 100));
            };

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

                    case SDL_KEYDOWN:
                        switch(e.key.keysym.sym)
                        {
                        case SDLK_UP:
                            scorePlayback.hiSpeed(scorePlayback.hiSpeed() + 10);
                            updateTitle();
                            break;

                        case SDLK_DOWN:
                            if(scorePlayback.hiSpeed() > 10)
                            {
                                scorePlayback.hiSpeed(scorePlayback.hiSpeed() - 10);
                                updateTitle();
                            }
                            break;

                        case SDLK_RIGHT:
                            {
                                const uint32_t position = scorePlayback.position();
                                scorePlayback.position(position + ScorePlayback::positionGapAtOnce(scorePlayback.hiSpeed()));
                            }
                            break;

                        case SDLK_LEFT:
                            {
                                const uint32_t position = scorePlayback.position();
                                const uint32_t gap = ScorePlayback::positionGapAtOnce(scorePlayback.hiSpeed());
                                if(position >= gap)
                                {
                                    scorePlayback.position(position - gap);
                                }
                                else
                                {
                                    scorePlayback.position(0);
                                }
                            }
                            break;

                        case SDLK_SPACE:
                            playing = !playing;
                            break;
                        }
                        break;
                    }
                }

                prev = now;
                now = std::chrono::steady_clock::now();
                const std::chrono::nanoseconds interval = now - prev;

                if(playing)
                {
                    scorePlayback.progress(interval);
                }

                screen.render(renderer, &scorePlayback, interval);

                // FPS measure
                static const std::chrono::milliseconds FrameMeasureInterval(5000);
                const std::chrono::milliseconds currentFrameMeasureInterval = std::chrono::duration_cast<std::chrono::milliseconds>(now - prevFrameMeasurePoint);
                if(currentFrameMeasureInterval >= FrameMeasureInterval)
                {
                    fpsBy100 = screen.pullFrameCount() * 100ull /* percent */ * 1000ull /* millisec */ / static_cast<uint64_t>(currentFrameMeasureInterval.count());
                    updateTitle();

                    prevFrameMeasurePoint = now;
                }
            }
        }
        catch(const SDL2pp::Exception &e)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", fmt::format("SDL2pp Exception: {}", e.what()).c_str(), nullptr);
            return 1;
        }
        catch(const std::exception &e)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", fmt::format("Exception: {}", e.what()).c_str(), nullptr);
            return 1;
        }

        return 0;
    }
}

extern "C" int main(int argc, char **argv)
{
    return TTSP::run(argc, argv);
}
