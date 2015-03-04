#pragma once

namespace TTSP
{
    enum PlayMode
    {
        PLAYMODE_INVALID,
        PLAYMODE_SP,
        PLAYMODE_DP,
    };

    enum PlaySide
    {
        PLAYSIDE_INVALID,
        PLAYSIDE_1P,
        PLAYSIDE_2P,
        PLAYSIDE_DP,
    };

    enum ObjectType
    {
        OBJECTTYPE_KEY,
        OBJECTTYPE_MEASURELINE
    };

    namespace
    {
        const uint16_t DefaultHiSpeed = 250;

        const size_t ObjectIdxMeasureLine = 16;

        const size_t KeyCount = 16;
        const size_t ObjectCount = KeyCount + 1; // Key + measure line
    }

    struct Object final
    {
        ObjectType type_;
    };

    class Score final
    {
    public:
        typedef std::array<std::map<uint32_t, Object>, ObjectCount> Objects;

    private:
        PlayMode playMode_;
        Objects objects_;
        std::map<uint32_t, uint32_t> bpms_;

    public:
        explicit Score(/* ... */);

    public:
        PlayMode playMode() const;
        std::vector<std::pair<uint32_t, uint32_t>> bpms(uint32_t positionBegin, uint32_t positionEnd) const;
        boost::iterator_range<Objects::value_type::const_iterator> objects(size_t objectIdx, uint32_t positionBegin, uint32_t positionEnd) const;

    private:
        void load(std::vector<std::string> dataSp, std::vector<std::string> dataDp, PlayMode playMode);
    };

    class ScorePlayback final
    {
    private:
        std::shared_ptr<Score> score_;
        PlaySide playSide_;
        uint16_t hiSpeed_;

        std::chrono::nanoseconds timePosition_;

        std::chrono::nanoseconds intervalToProgress_;

    public:
        ScorePlayback();

    public:
        void score(const std::shared_ptr<Score> &score);
        const Score *score() const;
        void playSide(PlaySide playSide);
        PlaySide playSide() const;
        void hiSpeed(uint16_t hiSpeed);
        uint16_t hiSpeed() const;
        void position(std::chrono::nanoseconds timePosition);
        uint32_t position() const;

    public:
        void progress(std::chrono::nanoseconds interval);
    };
}
