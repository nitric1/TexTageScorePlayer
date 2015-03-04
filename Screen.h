#pragma once

#include "Score.h"

namespace TTSP
{
    class Screen final
    {
    private:
        std::atomic<uint32_t> frames_;

    public:
        Screen();

    public:
        void render(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback, std::chrono::nanoseconds frameInterval);
        bool drawScorePlayback(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback);
        bool drawLanes(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback, int16_t x, int16_t y, uint16_t height, uint16_t &width);
        bool drawNotes(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback, int16_t x, int16_t y, uint16_t width, uint16_t height);

    public:
        uint32_t getFrameCount() const;
        uint32_t pullFrameCount();

    private:
        void incFrameCount();
    };
}
