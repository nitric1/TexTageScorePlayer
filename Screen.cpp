#include "Common.h"

#include "Screen.h"

namespace TTSP
{
    Screen::Screen()
        : frames_(0)
    {
    }

    namespace
    {
        enum LaneType
        {
            LANETYPE_SPLITTER,
            LANETYPE_SCRATCH,
            LANETYPE_WHITE,
            LANETYPE_BLACK,
        };

        const size_t KeyObjectIdxToLaneIdxMap[KeyCount] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16};

        // 1P S1234567 / Splitter / 1234567S 2P
        const size_t LaneCount = 17;
        const size_t LaneIdx1PS = 0; // 1P Scratch
        const size_t LaneIdx2PS = 16;
        const size_t LaneIdxSplitter = 8;

        const LaneType LaneTypes[LaneCount] =
        {
            LANETYPE_SCRATCH, LANETYPE_WHITE, LANETYPE_BLACK, LANETYPE_WHITE, LANETYPE_BLACK, LANETYPE_WHITE, LANETYPE_BLACK, LANETYPE_WHITE,
            LANETYPE_SPLITTER,
            LANETYPE_WHITE, LANETYPE_BLACK, LANETYPE_WHITE, LANETYPE_BLACK, LANETYPE_WHITE, LANETYPE_BLACK, LANETYPE_WHITE, LANETYPE_SCRATCH
        };
        const uint16_t LaneWidths[LaneCount] = {36, 13, 13, 13, 13, 13, 13, 13, 32, 13, 13, 13, 13, 13, 13, 13, 36};
        const int16_t LanePos1P[LaneCount] = {0, 37, 51, 65, 79, 93, 107, 121, 135, -1, -1, -1, -1, -1, -1, -1, -1};
        const int16_t LanePos2P[LaneCount] = {131, 33, 47, 61, 75, 89, 103, 117, 0, -1, -1, -1, -1, -1, -1, -1, -1};
        const int16_t LanePosDP[LaneCount] = {0, 37, 51, 65, 79, 93, 107, 121, 135, 168, 182, 196, 210, 224, 238, 252, 266};

        const SDL_Color LaneInnerLineColor = {64, 64, 64, 255};
        const SDL_Color LaneOuterLineColor = {255, 255, 255, 255};
        const SDL_Color LaneSplitterColor = {128, 128, 128, 255};
        // const SDL_Color LaneSubMeasureLineColor = {128, 128, 128, 255};

        const uint16_t NoteHeight = 4;

        const SDL_Color NoteWhiteColor = {192, 192, 192, 255};
        const SDL_Color NoteBlackColor = {0, 128, 255, 255}; // "black key"
        const SDL_Color NoteScratchColor = {255, 0, 0, 255};

        const uint16_t NoteWhiteCNMargin = 1;
        const uint16_t NoteBlackCNMargin = 1;
        const uint16_t NoteScratchCNMargin = 2;

        const SDL_Color NoteWhiteCNColor = {167, 167, 167, 255};
        const SDL_Color NoteWhiteCNAltColor = NoteBlackColor;
        const SDL_Color NoteBlackCNColor = {0, 103, 230, 255};
        const SDL_Color NoteBlackCNAltColor = NoteWhiteColor;
        const SDL_Color NoteScratchCNColor = {230, 0, 0, 255};
        const SDL_Color NoteScratchCNAltColor = NoteWhiteColor;

        const int16_t ScoreX = 50;
        const int16_t ScoreY = 100;
        const uint16_t ScoreHeight = 400;
    }

    void Screen::render(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback, const std::chrono::nanoseconds /*frameInterval*/)
    {
        renderer.SetDrawColor(0, 0, 0, 255);
        renderer.Clear();

        if(scorePlayback)
        {
            drawScorePlayback(renderer, scorePlayback);
        }

        renderer.Present();

        incFrameCount();
    }

    bool Screen::drawScorePlayback(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback)
    {
        assert(scorePlayback);

        // TODO: print title, level, ...

        uint16_t scoreWidth = 0;

        if(!drawLanes(renderer, scorePlayback, ScoreX, ScoreY, ScoreHeight, scoreWidth))
        {
            return false;
        }
        if(!drawNotes(renderer, scorePlayback, ScoreX, ScoreY, scoreWidth, ScoreHeight))
        {
            return false;
        }

        return true;
    }

#define LIST_RGBA(x) (x).r, (x).g, (x).b, (x).a

    bool Screen::drawLanes(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback,
        const int16_t x, const int16_t y, const uint16_t height, uint16_t &width)
    {
        assert(scorePlayback);

        const int16_t *lanePos = nullptr;

        switch(scorePlayback->playSide())
        {
        case PLAYSIDE_1P:
            lanePos = LanePos1P;
            break;

        case PLAYSIDE_2P:
            lanePos = LanePos2P;
            break;

        case PLAYSIDE_DP:
        default: // TODO: temporary
            lanePos = LanePosDP;
            break;
        }

        if(!lanePos)
        {
            return false;
        }

        renderer.SetDrawColor(LIST_RGBA(LaneInnerLineColor));
        const size_t rightmostLaneIdx = std::max_element(lanePos, lanePos + LaneCount) - lanePos;

        int16_t leftmostX = 0, rightmostX = 0;

        for(size_t i = 0; i < LaneCount; ++ i)
        {
            if(lanePos[i] < 0)
            {
                break;
            }

            // left
            if(lanePos[i] == 0)
            {
                leftmostX = static_cast<int>(x) + lanePos[i];

                renderer.SetDrawColor(LIST_RGBA(LaneOuterLineColor));
            }
            renderer.DrawLine(
                static_cast<int>(x) + lanePos[i], y,
                static_cast<int>(x) + lanePos[i], static_cast<int>(y) + height - 1);
            if(lanePos[i] == 0)
            {
                renderer.SetDrawColor(LIST_RGBA(LaneInnerLineColor));
            }

            // right
            if(i == rightmostLaneIdx)
            {
                rightmostX = static_cast<int>(x) + lanePos[i] + LaneWidths[i] + 1;

                renderer.SetDrawColor(LIST_RGBA(LaneOuterLineColor));
            }
            renderer.DrawLine(
                static_cast<int>(x) + lanePos[i] + LaneWidths[i] + 1, y,
                static_cast<int>(x) + lanePos[i] + LaneWidths[i] + 1, static_cast<int>(y) + height - 1);
            if(i == rightmostLaneIdx)
            {
                renderer.SetDrawColor(LIST_RGBA(LaneInnerLineColor));
            }

            // fill
            if(i == LaneIdxSplitter)
            {
                renderer.SetDrawColor(LIST_RGBA(LaneSplitterColor));

                renderer.FillRect(SDL2pp::Rect(SDL2pp::Point(static_cast<int>(x) + lanePos[i] + 1, y), SDL2pp::Point(LaneWidths[i], height)));

                renderer.SetDrawColor(LIST_RGBA(LaneInnerLineColor));
            }
        }

        width = rightmostX - leftmostX;

        return true;
    }

    bool Screen::drawNotes(SDL2pp::Renderer &renderer, const ScorePlayback *scorePlayback,
        const int16_t x, const int16_t y, const uint16_t width, const uint16_t height)
    {
        assert(scorePlayback);

        const Score *score = scorePlayback->score();
        if(!score)
        {
            return true;
        }

        const int16_t *lanePos = nullptr;

        switch(scorePlayback->playSide())
        {
        case PLAYSIDE_1P:
            lanePos = LanePos1P;
            break;

        case PLAYSIDE_2P:
            lanePos = LanePos2P;
            break;

        case PLAYSIDE_DP:
            lanePos = LanePosDP;
            break;
        }

        if(!lanePos)
        {
            return false;
        }

        const uint32_t position = static_cast<uint32_t>(scorePlayback->position());
        const uint16_t hiSpeed = scorePlayback->hiSpeed();
        const uint32_t positionGap = ScorePlayback::positionGapAtOnce(hiSpeed);

        const int bottomY = static_cast<int>(y) + height - 1;

        auto calcObjectY = [height, positionGap, bottomY](const uint32_t screenObjectPosition)
        {
            return bottomY - (screenObjectPosition * height / positionGap);
        };

        // measure line
        for(const auto &objectPair : score->objects(ObjectLaneIdxMeasureLine, position, position + positionGap))
        {
            const uint32_t screenObjectPosition = objectPair.first - position;
            const int objectY = calcObjectY(screenObjectPosition);

            renderer.SetDrawColor(LIST_RGBA(LaneOuterLineColor));
            renderer.DrawLine(
                static_cast<int>(x), objectY,
                static_cast<int>(x) + width, objectY);
        }

        // notes
        for(size_t i = 0; i < KeyCount; ++ i)
        {
            const size_t laneIdx = KeyObjectIdxToLaneIdxMap[i];
            if(lanePos[laneIdx] < 0)
            {
                break;
            }

            for(const auto &objectPair : score->objects(ObjectLaneIdxKeyBegin + i, position, position + positionGap))
            {
                if(objectPair.second.type_ != OBJECTTYPE_NOTE)
                {
                    continue;
                }

                const uint32_t notePosition = objectPair.first;
                const uint32_t noteCNEndPosition = notePosition; // TODO: CN support
                if(notePosition < position && noteCNEndPosition < position)
                {
                    continue; // really?
                }
                else if(notePosition >= position + positionGap && noteCNEndPosition >= position + positionGap)
                {
                    continue;
                }

                const LaneType laneType = LaneTypes[laneIdx];

                const uint32_t screenNotePosition = notePosition - position;
                const uint32_t screenNoteCNEndPosition = noteCNEndPosition - position;

                const int noteX = static_cast<int>(x) + lanePos[laneIdx] + 1;
                const int noteWidth = LaneWidths[laneIdx];

                SDL_Color noteColor;
                switch(laneType)
                {
                case LANETYPE_SCRATCH:
                    noteColor = NoteScratchColor;
                    break;

                case LANETYPE_WHITE:
                    noteColor = NoteWhiteColor;
                    break;

                case LANETYPE_BLACK:
                    noteColor = NoteBlackColor;
                    break;

                default:
                    return false; // TODO: error
                }

                if(position <= notePosition && notePosition < position + positionGap) // start note
                {
                    const int noteY = calcObjectY(screenNotePosition) - NoteHeight + 1;
                    renderer.SetDrawColor(LIST_RGBA(noteColor));
                    renderer.FillRect(SDL2pp::Rect(SDL2pp::Point(noteX, noteY), SDL2pp::Point(noteWidth, NoteHeight)));
                }

                if(notePosition < noteCNEndPosition) // draw CN bar
                {
                    uint16_t noteCNMargin;
                    switch(laneType)
                    {
                    case LANETYPE_SCRATCH:
                        noteCNMargin = NoteScratchCNMargin;
                        break;

                    case LANETYPE_WHITE:
                        noteCNMargin = NoteWhiteCNMargin;
                        break;

                    case LANETYPE_BLACK:
                        noteCNMargin = NoteBlackCNMargin;
                        break;

                    default:
                        return false; // TODO: error
                    }

                    // TODO: draw CN bar

                    if(position <= noteCNEndPosition && noteCNEndPosition < position + positionGap) // end note
                    {
                        const int noteY = calcObjectY(screenNotePosition) - NoteHeight + 1;
                        renderer.SetDrawColor(LIST_RGBA(noteColor));
                        renderer.FillRect(SDL2pp::Rect(SDL2pp::Point(noteX, noteY), SDL2pp::Point(noteWidth, NoteHeight)));
                    }
                }
            }
        }

        return true;
    }

#undef LIST_RGBA

    void Screen::incFrameCount()
    {
        ++ frames_;
    }

    uint32_t Screen::getFrameCount() const
    {
        return frames_;
    }

    uint32_t Screen::pullFrameCount()
    {
        return frames_.exchange(0);
    }
}
