#pragma once

namespace TTSP
{
    struct DataCN
    {
        uint32_t sideKeyIdxs_; // Multi digits (in 10-based), 0=Scratch, 1=Key1, 2=Key2, ..., 7=Key7
        uint32_t position_;
        uint32_t length_;
        enum Type
        {
            CONTINUATION_ONLY = 0,
            HAS_START_NOTE,
            HAS_END_NOTE,
            HAS_BOTH_NOTE
        } type_;
    };

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
        OBJECTTYPE_NOTE,
        OBJECTTYPE_CN_START,
        OBJECTTYPE_CN_END,
        OBJECTTYPE_MEASURELINE
    };

    namespace
    {
        const uint32_t BaseMeasureSize = 384; // at 4/4

        const uint16_t BaseHiSpeed = 100; // 1 4/4 measure at once
        const uint16_t DefaultHiSpeed = 250;

        const size_t ObjectLaneIdxKeyBegin = 0;
        const size_t ObjectLaneIdxMeasureLine = 16;

        const size_t SideKeyCount = 8;
        const size_t SideKeyToObjectLaneIdxMap1P[SideKeyCount] = {0, 1, 2, 3, 4, 5, 6, 7};
        const size_t SideKeyToObjectLaneIdxMap2P[SideKeyCount] = {15, 8, 9, 10, 11, 12, 13, 14};

        const size_t KeyCount = 16;
        const size_t ObjectLaneCount = KeyCount + 1; // Key + measure line
    }

    struct Object final
    {
        ObjectType type_;
    };

    class Score final
    {
    public:
        typedef std::array<std::map<uint32_t, Object>, ObjectLaneCount> Objects;
        typedef std::map<uint32_t, uint16_t> BPMs;

    private:
        std::string title_;
        std::string artist_;
        std::string genre_;

        PlayMode playMode_;
        Objects objects_;
        BPMs bpms_;

    public:
        explicit Score(/* ... */);

    public:
        PlayMode playMode() const;
        boost::iterator_range<BPMs::const_iterator> bpms() const;
        boost::iterator_range<BPMs::const_iterator> bpms(uint32_t positionEnd) const;
        boost::iterator_range<Objects::value_type::const_iterator> objects(size_t objectLaneIdx, uint32_t positionBegin, uint32_t positionEnd) const;

    private:
        void load(std::vector<std::string> dataSP, std::vector<std::string> dataDP, std::vector<std::vector<DataCN>> dataCNSP, std::vector<std::vector<DataCN>> dataCNDP,
            std::vector<uint16_t> measureSizes, std::vector<std::vector<std::string>> bpms, PlayMode playMode);
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
        static uint32_t positionGapAtOnce(uint16_t hiSpeed);

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
        void position(uint32_t position);
        uint32_t position() const;

    public:
        void progress(std::chrono::nanoseconds interval);
    };
}
