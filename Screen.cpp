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

        auto calcObjectY = [height, positionGap, bottomY](const int32_t screenObjectPosition)
        {
            return bottomY - (screenObjectPosition * height / static_cast<int32_t>(positionGap));
        };

        {
            class Clipper
            {
                SDL2pp::Renderer &renderer_;
                Clipper &operator =(const Clipper &) = delete;
                Clipper &operator =(Clipper &&) = delete;
            public:
                Clipper(SDL2pp::Renderer &renderer, const SDL2pp::Rect &rect)
                    : renderer_(renderer)
                {
                    renderer.SetClipRect(rect);
                }
                ~Clipper()
                {
                    renderer_.SetClipRect();
                }
            } clipper(renderer, SDL2pp::Rect(SDL2pp::Point(x, y), SDL2pp::Point(width, height)));

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
            auto drawNote = [&calcObjectY, &renderer, y](
                const SDL_Color noteColor, const int noteX, const int noteWidth,
                const int32_t screenNotePosition)
            {
                const int noteY = calcObjectY(screenNotePosition) - NoteHeight + 1;
                renderer.SetDrawColor(LIST_RGBA(noteColor));
                renderer.FillRect(SDL2pp::Rect(SDL2pp::Point(noteX, noteY), SDL2pp::Point(noteWidth, NoteHeight)));
            };

            auto drawCN = [&calcObjectY, &renderer, y, height, position, positionGap, bottomY](
                const LaneType laneType, const int noteX, const int noteWidth,
                const uint32_t cnStartPosition, const uint32_t cnEndPosition,
                bool noteCNStartExistsInScreen)
            {
                int32_t screenCNStartPosition = static_cast<int32_t>(cnStartPosition) - position;
                if(!noteCNStartExistsInScreen)
                {
                    screenCNStartPosition = 0;
                }

                int32_t screenCNEndPosition = static_cast<int32_t>(cnEndPosition) - position;
                if(screenCNEndPosition < 0)
                {
                    return;
                }

                SDL_Color noteCNColor;
                SDL_Color noteCNAltColor;
                uint16_t noteCNMargin;
                switch(laneType)
                {
                case LANETYPE_SCRATCH:
                    noteCNColor = NoteScratchCNColor;
                    noteCNAltColor = NoteScratchCNAltColor;
                    noteCNMargin = NoteScratchCNMargin;
                    break;

                case LANETYPE_WHITE:
                    noteCNColor = NoteWhiteCNColor;
                    noteCNAltColor = NoteWhiteCNAltColor;
                    noteCNMargin = NoteWhiteCNMargin;
                    break;

                case LANETYPE_BLACK:
                    noteCNColor = NoteBlackCNColor;
                    noteCNAltColor = NoteBlackCNAltColor;
                    noteCNMargin = NoteBlackCNMargin;
                    break;

                default:
                    return; // TODO: error
                }

                const int barBottomY = calcObjectY(screenCNStartPosition) - (noteCNStartExistsInScreen ? NoteHeight : 0) + 1;
                const int barTopY = calcObjectY(screenCNEndPosition);

                renderer.SetDrawColor(LIST_RGBA(noteCNColor));
                renderer.FillRect(SDL2pp::Rect(SDL2pp::Point(noteX + noteCNMargin, barTopY), SDL2pp::Point(noteWidth - 2 * noteCNMargin, barBottomY - barTopY)));

                renderer.SetDrawColor(LIST_RGBA(noteCNAltColor));
                renderer.DrawLine(SDL2pp::Point(noteX + noteCNMargin + 1, barTopY), SDL2pp::Point(noteX + noteCNMargin + 1, barBottomY - 1));
                renderer.DrawLine(SDL2pp::Point(noteX + noteWidth - 2 * noteCNMargin - 1, barTopY), SDL2pp::Point(noteX + noteWidth - 2 * noteCNMargin - 1, barBottomY - 1));
            };

            for(size_t i = 0; i < KeyCount; ++ i)
            {
                const size_t laneIdx = KeyObjectIdxToLaneIdxMap[i];
                if(lanePos[laneIdx] < 0)
                {
                    break;
                }

                const LaneType laneType = LaneTypes[laneIdx];
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

                boost::optional<uint32_t> noteCNStartPosition;

                for(const auto &objectPair : score->objects(ObjectLaneIdxKeyBegin + i, position, position + positionGap))
                {
                    if(objectPair.second.type_ != OBJECTTYPE_NOTE &&
                        objectPair.second.type_ != OBJECTTYPE_CN_START &&
                        objectPair.second.type_ != OBJECTTYPE_CN_END)
                    {
                        continue;
                    }

                    const uint32_t notePosition = objectPair.first;
                    const int32_t screenNotePosition = static_cast<int32_t>(notePosition) - position;

                    if(objectPair.second.type_ == OBJECTTYPE_CN_START)
                    {
                        noteCNStartPosition = notePosition;
                    }

                    // To draw bottom-blinded note
                    /*if(notePosition < position)
                    {
                        continue;
                    }
                    else */if(notePosition >= position + positionGap)
                    {
                        continue;
                    }

                    if(objectPair.second.type_ == OBJECTTYPE_CN_START)
                    {
                        noteCNStartPosition = notePosition;
                    }
                    else if(objectPair.second.type_ == OBJECTTYPE_CN_END)
                    {
                        if(noteCNStartPosition)
                        {
                            drawCN(laneType, noteX, noteWidth, *noteCNStartPosition, notePosition, true);
                            noteCNStartPosition = boost::none;
                        }
                        else
                        {
                            drawCN(laneType, noteX, noteWidth, 0, notePosition, false);
                        }
                    }

                    drawNote(noteColor, noteX, noteWidth, screenNotePosition);
                }

                if(noteCNStartPosition)
                {
                    drawCN(laneType, noteX, noteWidth, *noteCNStartPosition, position + positionGap, true);
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
