#include "Common.h"

#include "Score.h"

namespace TTSP
{
    Score::Score()
        : playMode_(PLAYMODE_INVALID)
    {
    }

    PlayMode Score::playMode() const
    {
        return playMode_;
    }

    std::vector<std::pair<uint32_t, uint32_t>> Score::bpms(const uint32_t positionBegin, const uint32_t positionEnd) const
    {
        auto begin = bpms_.lower_bound(positionBegin);
        if(begin != bpms_.begin())
        {
            -- begin;
        }

        auto end = bpms_.lower_bound(positionEnd);

        std::vector<std::pair<uint32_t, uint32_t>> bpmsInRange;
        std::copy(begin, end, std::back_inserter(bpmsInRange));

        return bpmsInRange;
    }

    boost::iterator_range<Score::Objects::value_type::const_iterator> Score::objects(const size_t objectIdx, const uint32_t positionBegin, const uint32_t positionEnd) const
    {
        auto begin = objects_.at(objectIdx).lower_bound(positionBegin);
        auto end = objects_.at(objectIdx).lower_bound(positionEnd);

        return {begin, end};
    }

    void Score::load(std::vector<std::string> dataSp, std::vector<std::string> dataDp, const PlayMode playMode)
    {
        /*
        URL parameter

        변수 초기값
          // 보면 선택 변수
          k=1 // 0일 경우 DP 보면 source, 1일 경우 SP 보면 source
          a=0 // 어나더
          kuro=0 // a+흑어나더
          l=0 // 노멀
          hps=0 // ??
          g=0 // 비기너

          // 화면 조정 변수
          key=7 // 출력 키 수
          sides[1]=2 // play side
          sides[2]=2 // ??
          pty=0 // ?? "BEGINNER" 출력?
          prt=0 // print

          // 보면 조정 변수
          os=0 // only side
          flp=0
          m=0 // merge
          db=0 // double battle

        some.html?ABCDE(R0NNNNNNN0NNNNNNN)?(=NN)?
        A: 플레이사이드
          1: SP 1P; sides[1]=1
          2: SP 2P (default)
          D: DP; k=0; key=14
          L: DP 1P side only; k=0; key=14; os=1; sides[1]=1
          R: DP 2P side only; k=0; key=14; os=2
          F: DP flip; flp=1; k=0; key=14
          M: DP -> SP화 (1P side + 2P side merge); m=1; k=0; key=14
          B: Double Battle; db=1; key=14

        B: 난이도 (소문자일 경우 print mode; prt=1)
          X: 흑어나더; a=1; kuro=1
          A: 어나더; a=1
          L: ??; l=1
          N: 노멀; l=1; hps=1
          H: 하이퍼; hps=1
          B: 비기너 (10th 이하); l=1; g=1
          G, R: 비기너; l=1; hps=1; g=1
          P: ??; l=1; hps=1; pty=1; g=1

        C: 레벨 (16진수)
          1-C: 통상 레벨
          D: 7+ (붉은 글씨)
          E: 8+ (붉은 글씨)

        DE: flag

        R...: 랜덤 키배치
          랜덤 방식: ABCDEFG일 때, 각 자리에 source 레인의 숫자를 지정
            (즉, 1234567 -> 1726354일 경우, 1234567 계단은 1357642가 됨.
             랜덤 후의 레인 기준으로 보자면(target<-source) 1<-1, 2<-7, 3<-2, 4<-6, 5<-3, 6<-5, 7<-4 가 되며,
             랜덤 전의 레인 기준으로 보자면(source->target) 1->1, 2->3, 3->5, 4->7, 5->6, 6->4, 7->2 가 됨)

          먼저 나오는 0NNNNNNN에 SP 랜덤 또는 DP의 1P side 랜덤 지정 (DP only side 모드 포함)
          나중 나오는 0NNNNNNN에 DP 2P side 랜덤 지정 (flip일 경우 flip 후의 2P side)

        =NN: 하이스피드
        */

        /*
        Data part

        변수 초기값
          ln = [384, ...]; // measure size, 4/4박 기준
          sp = [...]; // SP or DP 1P side 보면
          dp = [...]; // DP 2P side 보면
          c1 = [...]; // sp의 CN
          c2 = [...]; // dp의 CN

          sd = [[], sp, dp];
          cn = [[], c1, c2];
        */

        if(playMode != PLAYMODE_SP && playMode != PLAYMODE_DP)
        {
            throw(std::invalid_argument("Invalid playMode; playMode must be either PLAYMODE_SP or PLAYMODE_DP."));
        }

        if(playMode == PLAYMODE_DP)
        {
            if(dataSp.size() < dataDp.size())
            {
                dataSp.resize(dataDp.size());
            }
            else if(dataSp.size() > dataDp.size())
            {
                dataDp.resize(dataSp.size());
            }
        }
    }

    ScorePlayback::ScorePlayback()
        : playSide_(PLAYSIDE_INVALID)
        , hiSpeed_(DefaultHiSpeed)
    {
    }

    void ScorePlayback::score(const std::shared_ptr<Score> &score)
    {
        score_ = score;

        if(score)
        {
            switch(score->playMode())
            {
            case PLAYMODE_SP:
                playSide(PLAYSIDE_1P);
                break;

            case PLAYMODE_DP:
                playSide(PLAYSIDE_DP);
                break;
            }
        }

        timePosition_ = std::chrono::nanoseconds::zero();
    }

    const Score *ScorePlayback::score() const
    {
        return score_.get();
    }

    void ScorePlayback::playSide(const PlaySide playSide)
    {
        if(score_)
        {
            if(playSide == PLAYSIDE_INVALID)
            {
                throw(std::invalid_argument("Invalid playSide; playSide must not be PLAYSIDE_INVALID if the score is set."));
            }
        }
        else
        {
            if(playSide != PLAYSIDE_INVALID)
            {
                throw(std::invalid_argument("Invalid playSide; playSide must be PLAYSIDE_INVALID if the score is not set."));
            }
        }

        playSide_ = playSide;
    }

    PlaySide ScorePlayback::playSide() const
    {
        return playSide_;
    }

    void ScorePlayback::hiSpeed(const uint16_t hiSpeed)
    {
        if(hiSpeed == 0)
        {
            throw(std::invalid_argument("Invalid hiSpeed; hiSpeed must be greater than zero."));
        }

        hiSpeed_ = hiSpeed;
    }

    uint16_t ScorePlayback::hiSpeed() const
    {
        return hiSpeed_;
    }

    void ScorePlayback::position(const std::chrono::nanoseconds timePosition)
    {
        if(timePosition.count() < 0)
        {
            throw(std::invalid_argument("Invalid position; position must be greater than zero."));
        }
        else if(false) // TODO: max position
        {
            // TODO: adjust; not throw
        }

        timePosition_ = timePosition;
    }

    uint32_t ScorePlayback::position() const
    {
        // TODO: timePosition_ to position
        return 0;
    }

    void ScorePlayback::progress(const std::chrono::nanoseconds interval)
    {
        if(!score_)
        {
            return;
        }

        timePosition_ += interval;
    }
}
