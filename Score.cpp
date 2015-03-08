#include "Common.h"

#include "Score.h"

namespace TTSP
{
    Score::Score(/* ... */)
        : playMode_(PLAYMODE_INVALID)
    {
        // TODO: temporary
        title_ = "GOLDEN CROSS";
        artist_ = "dj REMO-CON VS dj TAKA";
        genre_ = "TECHNO";

        std::vector<std::string> dataSP =
        {
            "", "", "01", "#4Dw", "#R1Oevev_",
            "#R1Oevev", "#R1Oevev", "#OOPOSQdd", "#R1OGuet_", "#OOKONOYuYo9ICg8RCw",
            "#R1Oeveq6FQ9IFo", "#X90riLUyaXAAAAl3F8", "#r2r4R1R5oDDDDoHGHGq33_", "#r2r4R1R5oDDDDoHGHGq33", "#r2r4R1R5oDDDDoHGHGq33",
            "#r2r4XIzILIzILXoHo+oHo+", "#r2r4R1R5oDDDDoHGHGq33_", "#r2r4R1R5oDDDDoHGHGq33", "#XITITIqIPXonomo3ooq3G3D44EA", "#XcL9aKcCaXAuA1l3F1",
            "#XLULiLULiX4u414u41", "#XLULiLULiX4u414u41", "#XLULiLULiX4u414u41", "#XLULiLULiX4u414u41qHH", "#XLULiLULiX4u414u41_",
            "#XLULiLULiX4u414u41", "#XLULiLULiX4u414u41", "#r3R1Xi0C6i0C6ov2v2", "#r7R1XlTlalTFaX4mAG4mAG_", "#XNTNaNTNaXgmg+gmg+O/A/A",
            "#XNTNaNTNaXgmg+gmg+O/A/A", "#XNTNKNTNKXgmgegmgdO/H/+7FQ", "#r3r7R4oRSRSo2u21ONANAB/_", "#r3r7R4oRSRSo2u21ONANAB/-OAw", "#r3r7R4oKKKKouuuu-r",
            "#p1r3r7R4ouuuuQCS-Ogw", "#OQEC4_", "#OIDC4", "#OIDC4", "#XoFAoQCAQ",
            "#XIAweGQAAO4AHA_Eg", "#XIAweGQAAO4AHA_Eg", "#XIAweGQAAO4AHA_Eg", "#XoFwuW6AQO4AAH", "#r2R1R3XVG98Fe9+X4AAGHgAA",
            "#r4R3R7XNGNZCOlOXQAAGFYAA", "#r4R1R3XWF+dCdm9X4AAHG4AA", "#R1R7XdUdUUaFUXwGAYF0AYqGGqHH", "#r4R1R3XVG9aFWF+X4AAGHYAA_Eg", "#r1R2R7XMDszETErXoAAFGoAA_Eg",
            "#R1XrrrrrWgAX4A4HHo4A8KFA8KFg_Eg", "#XIHA4QCAQ7AA_", "#bm6FA6Fg", "#OzAEA3B4_AwB4", "#r7R1XeTVaeTVaXomg0oug0_",
            "#r7R1XeTVaeTVaXomg0oug0", "#r7R1XeTVaeTVaXomg0oug0", "#R1XeTVKeTVZXomgcoug1q+47CQ_FQ", "#XUMTLUMTLXweolweolO9+9+_", "#XUMTLUMRaXweolweE1X4o4w4vA4",
            "#r7XKcTiKcKaX4u41Au405Fo-OCQ", "#r3r5R1X6E6CCE66oGAmA_", "#r4r7R1XoTgQqagQb2-OgQ", "#r4r7R1XoTgQqagQb2_FQ", "#r4r7R1XoTgQqagTb2_FQ",
            "#r2r4r7R1XrDgLrDgLb2", "#P6r4r7R1XATgVCagVoFAFA-OgQ", "#P6r4r7R1XATgVCagToFAFF_FQ", "#P6r2r4r7R1obDbDoFFFFbk-qU", "#r1r6Xiiaqaid7otktEoHHHFbE-q4",
            "#r7R1XTlE1TlE1", "#r7R1XTlF0TlE1_FQ", "#P1XcuG+cuG+_", "#P1XjzD7jzF+ovuvA", "01"
        };

        std::vector<uint16_t> measureSizes(dataSP.size());
        measureSizes.back() = 24;

        std::vector<std::vector<std::string>> bpms = {{}, {"1500"}};

        load(std::move(dataSP), {}, {}, {}, std::move(measureSizes), std::move(bpms), PLAYMODE_SP);
    }

    PlayMode Score::playMode() const
    {
        return playMode_;
    }

    uint16_t Score::firstBpm() const
    {
        if(bpms_.empty())
        {
            return 0;
        }

        return bpms_.begin()->second;
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

    boost::iterator_range<Score::Objects::value_type::const_iterator> Score::objects(const size_t objectLaneIdx, const uint32_t positionBegin, const uint32_t positionEnd) const
    {
        auto begin = objects_.at(objectLaneIdx).lower_bound(positionBegin);
        auto end = objects_.at(objectLaneIdx).lower_bound(positionEnd);

        return {begin, end};
    }

    namespace
    {
        typedef boost::archive::iterators::detail::to_6_bit<char> FromBase64CharFn;
    }

    void Score::load(std::vector<std::string> dataSP, std::vector<std::string> dataDP, std::vector<std::vector<DataCN>> dataCNSP, std::vector<std::vector<DataCN>> dataCNDP,
        std::vector<uint16_t> measureSizes, std::vector<std::vector<std::string>> bpms, const PlayMode playMode)
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

        D: flag
          ...

        E: 한 column의 measure 수

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
          ln = [384, ...]; // measure size, 4/4박 기준 384
          sp = [...]; // SP or DP 1P side 보면
          dp = [...]; // DP 2P side 보면
          c1 = [[0,32,62,3], ...]; // sp의 CN ([key#, start pos, length, type]; pos와 length는 4/4박 기준 128; type & 1 == 1 then 시작 노트가 있음; type & 2 == 2 then 종료 노트가 있음; type == undefined then type = 3)
          c2 = [...]; // dp의 CN
          tc = ["2000", ...]; // BPM 변화 (앞 3글자가 bpm, 나머지 글자가 128진 기준 위치)

          sd = [[], sp, dp];
          cn = [[], c1, c2];
        */

        if(playMode != PLAYMODE_SP && playMode != PLAYMODE_DP)
        {
            throw(std::invalid_argument("Invalid playMode; playMode must be either PLAYMODE_SP or PLAYMODE_DP."));
        }

        size_t measures = std::max(dataSP.size(), dataCNSP.size());

        if(playMode == PLAYMODE_DP)
        {
            measures = std::max(measures, std::max(dataDP.size(), dataCNDP.size()));
        }

        dataSP.resize(measures);
        dataCNSP.resize(measures);

        measureSizes.resize(measures);
        bpms.resize(measures);

        if(playMode == PLAYMODE_DP)
        {
            dataDP.resize(measures);
            dataCNDP.resize(measures);
        }

        for(uint16_t &measureSize : measureSizes)
        {
            if(measureSize == 0)
            {
                measureSize = BaseMeasureSize;
            }
        }

        uint32_t measurePos = 0;
        for(size_t measureNum = 1; measureNum < measures; ++ measureNum)
        {
            static const uint32_t BaseMeasureSizeCN = 128;
            static const uint32_t BaseMeasureSizeBPM = 128;

            const uint32_t measureSize = measureSizes[measureNum];

            // measure line
            objects_[ObjectLaneIdxMeasureLine].emplace(measurePos, Object{OBJECTTYPE_MEASURELINE});

            // notes
            const std::string &spStr = dataSP[measureNum];
            if(!spStr.empty())
            {
                size_t strPos = 0;
                try
                {
                    if(spStr[0] == '#')
                    {
                        static const FromBase64CharFn fromBase64Char;

                        enum
                        {
                            SUBSTRMODE_DEFAULT,
                            SUBSTRMODE_COLLAPSED,
                            SUBSTRMODE_FINAL,
                        } subStrMode = SUBSTRMODE_DEFAULT;

                        for(++ strPos; strPos < spStr.size(); ++ strPos)
                        {
                            uint32_t start = 0, gap = 0;
                            enum ParseType
                            {
                                PARSETYPE_REPEAT_SINGLE_KEY, // raw = key | ""
                                PARSETYPE_BASE64, // raw = base64(u3 keys[]) | base64(u6 key0OrKey1BitFlag[])
                                PARSETYPE_FREE_POSITION, // raw = base64(u6 keysBitFlag{0,1}) base64(u12 pos[])
                            } parseType;

                            std::string rawStr;
                            const uint32_t measureLen = (subStrMode == SUBSTRMODE_DEFAULT ? 3 : 1) * measureSize / 6;
                            switch(spStr[strPos])
                            {
                                // repeat single key; half note to 8th note; v2t=0
#define SET_SUBSTR(s, g) \
    do \
    { \
        start = (s); gap = (g); parseType = PARSETYPE_REPEAT_SINGLE_KEY; \
        if(subStrMode == SUBSTRMODE_DEFAULT) { rawStr.assign(1, spStr.at(++ strPos)); } \
    } \
    while(false, false)

                            case 'C': SET_SUBSTR(0, 192); break;
                            case 'c': SET_SUBSTR(96, 192); break;
                            case 'R': SET_SUBSTR(0, 96); break;
                            case 'r': SET_SUBSTR(48, 96); break;
                            case 'P': SET_SUBSTR(0, 48); break;
                            case 'p': SET_SUBSTR(24, 48); break;

#undef SET_SUBSTR

#define SET_SUBSTR(s, g) \
    do \
    { \
        start = (s); gap = (g); parseType = PARSETYPE_BASE64; \
        const size_t len = (measureLen + gap - 1) / gap; \
        spStr.at(strPos + len); \
        rawStr.assign(spStr, strPos + 1, len); \
        strPos += len; \
    } \
    while(false, false)

                                // half note to 32nd note; v2t=1
                            case 'B': SET_SUBSTR(0, 192); break;
                            case 'b': SET_SUBSTR(96, 192); break;
                            case 'Q': SET_SUBSTR(0, 96); break;
                            case 'q': SET_SUBSTR(48, 96); break;
                            case 'O': SET_SUBSTR(0, 48); break;
                            case 'o': SET_SUBSTR(24, 48); break;
                            case 'X': SET_SUBSTR(0, 24); break;
                            case 'x': SET_SUBSTR(12, 24); break;
                            case 'Z': SET_SUBSTR(0, 12); break;

                                // 6th note to 24th note; v2t=1
                            case 'S': SET_SUBSTR(0, 64); break;
                            case 's': SET_SUBSTR(32, 64); break;
                            case 'T': SET_SUBSTR(0, 32); break;
                            case 't': SET_SUBSTR(16, 32); break;
                            case 'U': SET_SUBSTR(0, 16); break;

#undef SET_SUBSTR

                                // constant (including self); v2t=2; v2p not used
                            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
                                // assert(subStrMode == SUBSTRMODE_DEFAULT); ?

                                spStr.at(strPos + 2);

                                parseType = PARSETYPE_FREE_POSITION;
                                rawStr.assign(spStr, strPos, 3);
                                strPos += 2; // +1 in for statement above
                                break;

                                // multi keys (including key #1)
                            case '9':
                                // assert(subStrMode == SUBSTRMODE_DEFAULT); ?

                                spStr.at(strPos + 3);

                                rawStr = "1";
                                rawStr.append(spStr, strPos + 2, 2);
                                // don't break

                                // multi keys (excluding key #1)
                            case '8':
                                parseType = PARSETYPE_FREE_POSITION;

                                {
                                    const int8_t keysToRepeat = fromBase64Char(spStr.at(strPos + 1));
                                    for(size_t key = 2; key <= 7; ++ key)
                                    {
                                        if(keysToRepeat & (1 << (key - 2)))
                                        {
                                            rawStr += fmt::format("{}", key);
                                            rawStr.append(spStr, strPos + 2, 2);
                                        }
                                    }
                                }

                                strPos += 3;
                                break;

                                // modifier
                            case '-':
                                subStrMode = SUBSTRMODE_COLLAPSED;
                                continue; // not break

                                // finalizer
                            case '_':
                                subStrMode = SUBSTRMODE_FINAL;
                                parseType = PARSETYPE_FREE_POSITION;
                                if(strPos >= spStr.size() - 1)
                                {
                                    rawStr = "AA";
                                }
                                else
                                {
                                    rawStr = spStr.substr(strPos + 1);
                                }
                                break;

                                // error
                            default:
                                throw(std::invalid_argument(fmt::format("unknown data character: {} at character index {}, measure {} ({})", spStr[strPos], strPos, measureNum, spStr)));
                            }

                            // assert(subStrMode != SUBSTRMODE_FINAL || parseType == PARSETYPE_FREE_POSITION); ?

                            // convert rawStr to keyStr
                            std::string keyStr; // list of keys
                            if(parseType == PARSETYPE_REPEAT_SINGLE_KEY)
                            {
                                keyStr.assign((measureSize - start + gap - 1) / gap, (subStrMode == SUBSTRMODE_DEFAULT) ? rawStr[0] : '1');
                            }
                            else if(parseType == PARSETYPE_BASE64) // split each 6-bit-digit into 2 keys
                            {
                                if(subStrMode == SUBSTRMODE_DEFAULT)
                                {
                                    for(const char ch : rawStr)
                                    {
                                        const int bin = fromBase64Char(ch);
                                        // assert(bin >= 0);
                                        keyStr += fmt::format("{}{}", (bin & 0x38) >> 3, bin & 0x07);
                                    }
                                }
                                else // key #1 only
                                {
                                    for(const char ch : rawStr)
                                    {
                                        const int bin = fromBase64Char(ch);
                                        // assert(bin >= 0);
                                        for(int i = 5; i >= 0; -- i)
                                        {
                                            keyStr += fmt::format("{}", (bin >> i) & 0x01);
                                        }
                                    }
                                }
                            }

                            {
                                size_t i = 0;
                                if(parseType != PARSETYPE_FREE_POSITION)
                                {
                                    for(uint32_t posInMeasure = start; posInMeasure < measureSize; posInMeasure += gap, ++ i)
                                    {
                                        const char keyCh = keyStr.at(i);
                                        if(keyCh != '0')
                                        {
                                            const size_t key = keyCh - '0';
                                            objects_[ObjectLaneIdxKeyBegin + key].emplace(measurePos + posInMeasure, Object{OBJECTTYPE_NOTE});
                                        }
                                    }
                                }
                                else
                                {
                                    for(; i < rawStr.size(); i += 2)
                                    {
                                        size_t key = 0;
                                        if(subStrMode == SUBSTRMODE_DEFAULT)
                                        {
                                            key = rawStr[i] - '0';
                                            ++ i;
                                        }

                                        const uint32_t posInMeasure = (static_cast<uint32_t>(fromBase64Char(rawStr.at(i))) << 6) | fromBase64Char(rawStr.at(i + 1));
                                        objects_[ObjectLaneIdxKeyBegin + key].emplace(measurePos + posInMeasure, Object{OBJECTTYPE_NOTE});
                                    }
                                }
                            }

                            if(subStrMode == SUBSTRMODE_FINAL)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        uint16_t totalBeats = 0;
                        if(spStr[0] == 'x')
                        {
                            spStr.at(3);

                            size_t lastIdx;
                            totalBeats = static_cast<uint16_t>(std::stoul(spStr.substr(1, 3), &lastIdx, 16));

                            if(lastIdx < 3)
                            {
                                strPos += lastIdx + 1;
                                throw(std::invalid_argument(fmt::format("invalid data character: {} at character index {}, measure {} ({}) (not a hexadecimal digit)", spStr[strPos], strPos, measureNum, spStr)));
                            }

                            strPos = 4;
                        }
                        else
                        {
                            totalBeats = static_cast<uint16_t>(spStr.size());
                        }

                        for(uint16_t currentBeat = 0; strPos < spStr.size(); strPos += 2, currentBeat += 2)
                        {
                            while(spStr.at(strPos) == '@')
                            {
                                size_t lastIdx;
                                currentBeat += static_cast<uint16_t>(std::stoul(spStr.substr(strPos + 1, 2), &lastIdx, 16) * 2);
                                if(lastIdx < 2)
                                {
                                    strPos += lastIdx + 1;
                                    throw(std::invalid_argument(fmt::format("invalid data character: {} at character index {}, measure {} ({}) (not a hexadecimal digit)", spStr[strPos], strPos, measureNum, spStr)));
                                }
                                strPos += 3;
                            }

                            size_t lastIdx;
                            const uint16_t keysBitFlag = static_cast<uint16_t>(std::stoul(spStr.substr(strPos, 2), &lastIdx, 16));
                            if(lastIdx < 2)
                            {
                                strPos += lastIdx;
                                throw(std::invalid_argument(fmt::format("invalid data character: {} at character index {}, measure {} ({}) (not a hexadecimal digit)", spStr[strPos], strPos, measureNum, spStr)));
                            }

                            for(size_t key = 0; key <= 7; ++ key)
                            {
                                if(keysBitFlag >> key == 0)
                                {
                                    break;
                                }

                                if(((keysBitFlag >> key) & 0x01) == 0x01)
                                {
                                    const uint32_t posInMeasure = measureSize * currentBeat / totalBeats;
                                    objects_[ObjectLaneIdxKeyBegin + key].emplace(measurePos + posInMeasure, Object{OBJECTTYPE_NOTE});
                                }
                            }
                        }
                    }
                }
                catch(const std::out_of_range &)
                {
                    throw(std::invalid_argument(fmt::format("invalid data character: {} at character index {}, measure {} ({}) (insufficient data length)", spStr[strPos], strPos, measureNum, spStr)));
                }
                catch(const boost::archive::iterators::dataflow_exception &)
                {
                    throw(std::invalid_argument(fmt::format("invalid data characters at measure {} ({}) (not base64 character)", measureNum, spStr)));
                }
            }

            // TODO: dataDP

            // CNs
            // TODO: dataCNSP, dataCNDP

            for(const std::string &bpm : bpms[measureNum])
            {
                if(bpm.size() < 4)
                {
                    throw(std::invalid_argument(fmt::format("invalid bpm string at measure {} ({}) (insufficient bpm string length)", measureNum, bpm)));
                }

                size_t lastIdx = 0;
                const uint16_t bpmNum = static_cast<uint16_t>(std::stoul(bpm.substr(0, 3), &lastIdx));
                if(lastIdx < 3)
                {
                    throw(std::invalid_argument(fmt::format("invalid bpm character: {} at character index {}, measure {} ({}) (not a digit)", bpm[lastIdx], lastIdx, measureNum, bpm)));
                }

                const uint32_t posInMeasure = std::stoul(bpm.substr(3));
                bpms_.emplace(posInMeasure * BaseMeasureSize / BaseMeasureSizeBPM, bpmNum);
            }

            measurePos += measureSize;
        }

        playMode_ = playMode;

        /*
        genre="HARD RENAISSANCE";title="JOMANDA";
        artist="DJ YOSHITAKA";bpm="90～300";measure=100;gap--;ln[100]=24;
        tc[1]=["1950"];tc[35]=["1600"];tc[36]=["1400","12064"];tc[37]=[" 900"];
        tc[38]=[" 9516","10024","11032","12064"];tc[39]=["1300","14064"];
        tc[40]=["1500","17064"];tc[41]=["1800","19016","21096"];tc[42]=["22064","24096"];
        tc[43]=["2500"];tc[44]=["2700"];tc[45]=["3000"];tc[50]=["1950"];

        if(k){notes=1300;
        c1[45]=[[0,0,128,1]];c1[46]=c1[47]=c1[48]=[[0,0,128,0]];c1[49]=[[0,0,64,2]];

        sp=[,,,"#X4rLoejI+bt_","#Xgzz4KqImQAt",,"#XgzzonPI3OAAEg","231092201210a223","#r1OdbrNbFbH-ORQ","#O0OMOQEmb/","#R1OlkppOA4E8","#XYiigNNQmb/_","#XQddIiiohQA/","#XYqqoMMYVb/_","#r3XIGeAh5gGbH","#R1X4ddYmegv_","#R1Xwqqonegu","#R1XgddYmDwq_","#O4EAA3BI_AABICQ","#OqlLW_","#Onvli","#OyfVU","#OTKLv","#O03lf","#OmdX1","#Ozmle","#O+jRZ","#r2O+usf_","#r2Ovums","#r1O7mmj","#r1OUdl3","#r2Osucv","#r2Odlf1","#OhelX","#Oeldi-Xkkg","#BJBe5As7BY4B84Ds2EY5E8-C","#QIKBf5Ao7BQ4Bw2CU6Dk4EE5FA-C","#Xzy7mNTo/_","#Xzy7mNToS","#Xzy7mNTo/_","#Xzy7mNToS","#X8y9U5e4i_","#X8y9U5e4i","#Xzy9Mym4A","#Xzy9Mym4A","28",,,,,"#XoWC4eDqg-Xkkg","#R1X4dDwVE9c_","#R1XwlD4mCui","#R1X4mCocC1Y","#R1X4UConDwQ4FQ","#R1XonDonC0d_","#R1X4cCoeCvr","#R1XwcC4nDwl","#R1X4VDwnFwQ5FQ","#XgN6wdCsabH_","#XwL6oOCjRbH","#R7XoUBYMC0Y","#R7XoUBoeBgY","#R1XwVD4mCsW","#R1X4dE4eCmi","#R1XoeC4fE4c_FQ","#XwUBoXAgAbG-OUg","#OIRRZOYvmv7AA","#OxapTOEs8u","#OKZRZO80Gs7CQ","#OpThUOHuDu","#XLmVIrmV4O4HAA_","#R1XwgogylGY7FQ","#XIcN4PEIIQwjbG8oFQ","#XgNEIfMyoqfA-Qc","01","#XwmF4VC0e_","#X4NC4dDn0","#XQdBgdCrw","#X4cCwnDwQ","#XwdEwWEzN","#X4mD4VD3i","#XwdC4eCoM","#X4VCwfDwg-Xkkg","#R1XwArwmD8W-OoA","#R1X4dy4mDuj","#R1Xwd6gdC0Y","#R1Xod8wnDwg","#R7XYVagNC0c_","#R7XoMawdBes","#XYUKoNCwUO444/_FQ","#XoMCwfAQAbG-OUg","#OIZRZOonlm-OlQ","#r4Oxa5fb1","#OLZRZO03ms-OlQ","#R5OBcRcq+m","#XTl6IcuUIqGH_","#XAIQYUOUIOF1FH","#XYV6geLgIOoGA9_FQ","#XoWBoeCpg7CQ-Qc","29"];
        sp[5]=sp[3];

        if(a){notes=1918;c1=[];
        sp=[,,,"#XIUsQOUocO9FdG-OgQ","#XIWmQcqYeX4oAowEwo-OSg","#XIV1QfMwPO+GlF-OgQ","#XQNlYPZYVXwgAwonog-OQg","#XILLQMMQfO8k11-OgQ","#XITLIWOQfOlllm-qs","#XYNNYVUIdOuWgeb2","#XIVNIVdIUO8k2eb/","#R1Xo0collYOO4wA3-OgQ","#r1XgvvYcsYmOoEww-XIIg","#R1XomegvnYPO4gAu-OgQ","#R5XgLDIHPQmq2YqHg-OQg","#XQLLIUcQsO+32/_","#XYRRYTKYjO3t+/_","#XQLTIUrgjOu3o/-C","#9ZAA9iBI4CQ_CQ","#OZTLWO1lAA_","#ORTjiOn3FA","#OKcVUOwHAA","#OTKKf","#OZblfO13AA","#ORLU1OmlHA","#OLjleOwGAA","#O+jRZ","#r2OhcsfO+uAA_","#r2OMLhsOvuGA","#r2OMrujO4GAA","#r2OsNl3","#R1OacqyOvuEH","#R1OacazO1mFF7Dw","#XiiaahpZxO+v3n","#XgxgoZogQO4HA4-Xkkg","#BiBrB03As5BY1B85Ds1EY4E8-C","#QoNByB73Ao4BQ2Bw5CU7Dk4EE2FA-C","#XNdVdcMQkO+414_","#XNdVdeMQbO+49o",,,"#XNdVdcMQiO+414_","#XNdVdeMQdO+49o_","#r3R7XN0VGMuVE-C",,"#OKbkt8RAA_","#OKcu6","#OTclp","#OTl3T_Bg","#OlwAA_DA","#XYUCoUBZQXoGA4FAwo-Xkko","#r2R1Xwd7wlE1cbH_","#r3R1Xwl6wlCmsbH","#r2R1Xwl7wlD1YbH","#r3R1Xom6omEoQbH","#XIedQNLWMOlGglO4HA4_","#XQONQOTuLOdEs8OwHAA","#XQLLQOMQNO1EozO4GA4","#R2XYNDonZoYO0GA0","#r2R1Xwd8wcD0uQ4v_","#r3R1XYmq4mEX0Q4t","#r2R1Xod8wmD1gQ4v","#r3R1Xol6wlEwQQw3","#r2R1XolcwnDseOwHow_","#r3R1XomqwmEWsO4HAo","#R1XoU7QlTwNO7FA86CQ_FQ","#XYNDIfAQAO8Ggw-OUg","330458a3086314c3","#OpiplODrz3OG4E4","530c28c304532817","#OpUZcOEei3OHoF4","#r2R1X0ud4rlcw_","#r7R1XgoQYrlUwbgbw","#R1XoVcQnVYIOEGA17FQ_FQ","#XwPeIeNcoOEHgw-Qc","01","#XQOVgdEzuO0HAA_","#XQNKIdUv1O8HAA","#XIcCIdDsoOgDAA","#XwcCwnFwQ","#XINUYWE1mOzGAA","#XQWLQdl3sO8HAA","#XIdCYuCocOwHAA","#XIiDIiDIIX4GA4GAwg5FQ-Xkko","#XIALQOTMaXYAQoY4usOwvg4bw-Qw","#XIdLgNWLlXogQwYYV3O33ng","#r7R1R6XQdiYVjsQqEG","#r7R1XgaroVjgYOswgw","#XIVOQOUMaXogYogYusOwno4_","#R1XoeUYlUacX4go4wA8u","#R1XYWdQnUYNO9mo0OAHA4_FQ","#XwLEIXAYAOFGYo7Aw-OUg","532834c3285334a3","#OpcilOCt7+qww-c","a35834c32c63541b","#OhlZcOF2itq44-c","#XLlKYTNUIOX0s1OoGHGOwHAA-OlQ","#XYIgYTNcIOEt0+q+4-OIw","#XYUrIdLQIO1FnkO4GAu7FQ-OIw","#XwOTIWLTIODlcvOE/ow-Qc","45"];
        sp[39]=sp[37];sp[40]=sp[38];sp[44]=sp[43];}

        if(l){notes=595;
        sp=[,,,"#O7dSl","#O7cJU","#O7dSl","#O7cJU","#OoOgPb1","#OoMEPb1","#OwNgObu","#O4NYMbu","#OqUJd","#OzcSv","#OqUJd","#OzcSv","#OwMoNbj","#O4MEPb1","#OoLgObe","#8KAA8KBI_CQ","#OweCH_","#OFEFa","#OweFE","#ODELv","#OwWDH","#OGEHr","#OIVFG","#q9i","#O4fEG_","#OFDFi","#O4fGE","#OCDM3","#OoNCG","#OEDH0","#OQNFG","#XIHAIHAIAOYDAY","2109","2109","#OIoBob/","#OIoBob/","#OIgBgb2","#OIgBgb2","#OIoBoQfH","#OIoBoQfH","#QMMQee","#QMMQee",,,,,,"#OYDAYOoFAo","#X4BAwBA84","#XoCAwFAMI","#XYFAQFAwYO4EAA","#XICAIGAgIO4FAA","#XwCAoCAzw","#XgCA4BAdY","#XQBAgFA4QOwHAA","#XgDAgGAYIOwHAA","#XoCAoBA9Y","#XwCA4FAUw","#XoDIgGAIYbH","#XoEIwHAIYbF","#XwCAwDArI","#X4EAwFALo","#XwDI4EAIQbH","#XwDAoHAAA_Eg","#OYIUHQuA","#OoYoU","#OwfGi","#OvNZU","#O2B2DqEH","#OBhtC7FQ","#XgEAYHAIoOwHAA","#XAGA4FAgQ","01","#OxdAA","#O6mFA","#OYGAA",,"#OxdAA","#O6mDA","#OoHAA","#OIBAI8oBI8oDY","#XAAQoBAro_","#XwEA4FAUQ","#XwFIgEAIYbH","#XwFIgHAIYbF","#XwCAwEAqo","#X4DAwEANI","#X4DIwEAIQbG","#XoCAwHAAA_Eg","#OQIUHQvA","#OoYwL","#OonGZ","#OvWhd","#OSESBqHG","#ODpkB7FQ","#XQCAIFAYwOgFAA","#XAFA4GAYI","21"];}

        }else{notes=1317;
        c1[45]=[[0,0,128,1]];c1[46]=c1[47]=c1[48]=[[0,0,128,0]];c1[49]=[[0,0,64,2]];

        sp=[,,,"#bb-OgQ","#QAbQA/_Aw","#bb-OgQ","#bDbH8iDw_Aw","#XIdYwigo4","#XoZYgWQo4","#XgdYwWDwg","#XofYgywoI","#OANb2_","#OVSrk","#OqWbt","#OMJpk","#XYrIwUwY4","#XwrIYUwQo","#XgdYwiEQo","#7AA4BI_CQ","#OKlMW","#OfmVi","#OxfVU","#OLMTv","#OUvAA","#OKlGA","#OYEAA",,"#O0esf","#Oudms","#O7uul","#ORdc3","#r3","#r3","#r2","#-Xkkg","#3AA4As6BY2B8_DA","#5AA2Ao4BQ7Bw3CU_DA","#X4y8O6m4t","#X4y8O6m4b","#OAoBo_","#OIoBo","#X40QN4Uw4","#OAoBo_","#O6l6m","#QNNBb",,,,,,"#XoWC4NBxg","#QDbbt_","#R3bt","#X4eCoNCwQ","#XoMBoeDoQ","#R1bt","#R1bt","#XwUC4fE4Y","#XwdCwnBog","#OAOSjb/_","#OqVZ6b4","#OrekZb/","#OtIbQb+","#XoMCwdBgQ","#XwdDwVC4o","#XgLCweEwY","#XoMCwPAAA_Eg","#OgXmv6AA","#Oqirm","#O83Vj","#OvWhm","#R3b/","#R3bt","#R3b/_FQ","#1Aw-Qc","01","#XAQCAIBDF","#XAgCAYEGF","#XAgCAIDDA","#XAgDAoGAA","#XAYEAQEFG","#XAgDAoFHE","#XAYCAgCAo","#X4dBwfCwg","08010828","#R3bt","#R3bt","#R3bt","#XoUD4mC0Q","#X4eC4dC9Y","#XwUDwmDwo","#X4dCwfAAA_Eg","#QpA-qc","#OFIFI","#QpA-qc","#OFIFI","#XKmDoKmD4","#XAgwgKmD4","#XwUDoVBgw","#X4eE4WCgY","01"];

        dp=[,,,"#X4dYgUQow","#XodYwigIo","#X4dYgUQow","#XodYgWQY4","#bt-OgQ","#bBbF_Aw","#bbbt","#bkb2","#X4d4g0Qo4","#XwUwYrIgw","#XoLog0Qo4","#Xgd4Y0QIo","#OZNJk","#OUJq2","#OqVbt","#6CQ_AABI",sp[23],"#OKmGA",sp[25],,"#Oz3mf","#OmdX1","#Ohele","#OXVMK","#r5","#r5","#r6","#r6","#Orml3","#Omlf1","#Ojmdm","#OnedW","#6DA5Ds3EY7E8_","#QAj7Dk6EE2FA_","#OA4D4_","#OY4D4",sp[38],sp[37],"#OA4D4_","#X40QN4Uwo",sp[44],"#O6l5e","01",,,"#_Bg",,sp[34],"#X4VCwNE4Y","#XoUC4eDog",sp[52],sp[52],"#XwUCoMBwY","#XoUC4eDog",sp[55],sp[55],"#XwUCoMCwY","#X4mD4mD4g","#XoWCgMDwY","#XoMDoXDwg","#OqVbkb/","#Ozdh6b4","#Orebob/_FQ","#bDbH-qw","#-OlQ","#OBoBo","#-OlQ","#OBoBo","#XwmDowmD4","#XAgogwdE4","#XoLEweCo4","#XAMD4mCog",,"#XwFA4FAwg","#XoDAwEAYQ","#XgDAICAoQ","#XgDAgGAoY","#XwFAwEAoY","#XgCAwFAYg","#XwDA4GA4A",sp[34],"#XoAiwdC0Q","#X4dC4mD9Y","#XgNCgdB0Q","#XofEwnDwo","#R1bb","#R1bb","#R1bb_FQ","#bEbG-qw","#OincN","#OiZic","#Ormdi","#Onmau","#QfA-qc","#OD4D7","#OA4D7","#3Aw-Qc","28"];

        if(a){notes=1810;c1=[];
        sp=[,,,"#QBJbt-OgQ","#OABpp3EI_Aw","#QBJbb-OgQ","#OABBo7Dw3EI_Aw","#XArroyig3_","#XYhhoddYn","#XoUUQrGY8","#XwVVQdeg5","#OZLJYOs+8o_FQ","#OILRSOAH11_Aw","#OCTJYOE+1o-OgQ","#OIRRSOAl93_Aw","#XAddYiyg7_","#XoiigVNQm","#XQNNIAFQi_DA","#8oAA1CQ_BI","#OClMW_",sp[20],sp[21],sp[22],sp[23],sp[24],sp[25],,sp[27],sp[28],sp[29],sp[30],"#R1OrmDD","#R1O7lDD6Dw","#p3P5","#OAFUA-XkAg",sp[35],sp[36],"#X8y8O6m4t","#XYYgddIgb","#X8y8O6m4t","#XYYgddIgb","#X8y8O6m4A","#OLZZZO4oHo","#X+i9Zqm4A","#OMuN3Bc","#Obkt26AA",,"#OKcu4","#OAAAc_Bg","#Ou4AA","#XAiAApAAA-Okw","#X4dDwVE9M","#XwNB4mB+j","#X4mBoUC0I","#XoUCoeDoQ","#X4eCofD0V","#X4WC4eB+j","#XwUBweEwd","#X4VDwnB4g","#r2QF9b/_","#r2R7bt","#r7R1bt","#r7R1bt","#ORLJqOEuAA","#r4OaXWAbo","#OTdJoOoHAA_FQ","#XAMCoWAAA_Eg","#OZThZOonGH","#Oqiqm","#OadUpO43GD","#OnWhu","#r5R1bb","#r5R1bb","#R1OGeGY_FQ","#qJI-Qc",,"#XwVD4UD0W","#X4dBwVC9Z","#XgVBgUDyg","#X4VDwnCwY","#OKlAA","#OTmHA","#OICAH4Fo","#XQABgAGQIOAAAl-oSA","#OARZKOAmAA_","#OycVEOEvHA","#ONqb2OwGAA","#OkI2Ibv","#XoUD4mD0W","#X4MBoUBsR","#XwUDwmCwc","#Q4CbG-qw","#ONYQIQ1A-qc","#OBd5eOGoG4","#OOYQIQ+A-qc","#OBd5cOHoF4","#XylDIylDIqHG","#XQgoQLmV44CQ","#XoUBwWDog7FQ","#XwVBoWCpg7CQ","01"];

        dp=[,,,"#XwiigNNQ7","#XwMMwiigf","#XwiigNMQv","#XwddQnfgf","#R1Olutw_FQ","#Qt29ECQ-qs","#O8NMOQEm","#OPNJJOoo99","#XA0UwLrIi_","#Xgd9YiKgv","#Xo0UwLrI8","#X4d9YiKgv","#R1O1ekdbu","#OHOMPQD7b3_","#OFOMOQDjbu_","#4BI-OkA",dp[19],"#OKlGA",dp[21],,dp[23],dp[24],dp[25],dp[26],"#r7OAdAA_","#r7ORcGA","#r6OQDAA","#r6",dp[31],dp[32],dp[33],"#OdQAf_CQDY",dp[35],dp[36],"#XAowvvYwt_","#X8y8O6m4b","#XoowvvYwt","#X8y8O6m4b",sp[42],sp[41],"#ON3MuBj",sp[43],"01","#OSbkt","#1FQ","#OTl3A","#_DA","#XoAD4ACyo-oSA","#r6QBJbb_","#r6R1bb","#r7R1bb","#r7R1bb","#ORUJiOF3AA","#OZKdBOEuHA","#OiUSiOwHAA","#OJYtgb/","#XwdCwMB0W","#X4eE4WD9Z","#XwdCgMC0I","#X4mE4fDwg","#XoUCwMC9e","#XgeE4eD+j","#XwVC4fE4M","#QwDbH-qw","#b2-OlQ","#OBbpdOF4D4","#b2-OlQ","#OBbhjOE4D4","#XUuD4UuB4","#XAgwgKlD4","#XwWD4nCow","#X4NC4eCqg","01","#OGjAA_","#O9iDA","#OoEAA",,"#XwUDwND8V","#X4mD4MC0a","#XoMC4vE4A","#XAzAAfAAA-Okw","#XoAA4eC9e7AA_Bg","#X4VCwNC0Z","#XoUBoVD8Q","#XwdCwnDwg","#r4R1bb",sp[71],sp[73],"#XAMCofAAA_Eg","#ORTZjOonEF","#Oqiql","#ORTSpOzuvE","#OnViu","#OMYw45AA-qc","#OBd5ZOGoEF","#O8JRYOAeEo_FQ","#qn36Aw-Qc","28"];}

        if(l){notes=598;c1=[];
        c2[45]=[[0,0,128,1]];c2[46]=c2[47]=c2[48]=[[0,0,128,0]];c2[49]=[[0,0,64,2]];

        sp=[,,,"#OEgtA","#OGwkw","#ODYSA","#ODYkw","#oFAEAqYQ","#XAdAAUAAA","#ODYtA","#qaI","#QDr_","#QtA-QM","#Qrr","#OoACQ_CQ","#OEgbA","#OFoJA","#qsR","#OQGAA4BI","#bo","#qiD","#bg","#ODAmA","#OAoDA","#qEF","#QMA","#qGj","#O4oEA","#OAGAr","#OAQFE","#ODALA","#OAQEA","#qrG","#OQADA","#OgEAgOwGAw","0120","0110","#bbbt_","#bbbt","#bSbk","#bSbk","#bJbt","#bJbt","#bTbl","#blb3",,,,,,"#XYDAYDAAA","#oDAYA5FQ","#OAAAb4BI","#qCE2DY","#OAAA05BI","#oFAoA3FQ","#OAAAk2BI","#3BI5FQ","#OAAAp3BI","#XAQCAQCAA","#XAYDAYDAA","#oGAgA4CQ","#OAAAi5BI","#XAIBAIBAA","#XAYDAYDAA","#4BI5FQ","#oDAwA4CQ","#Qrg","#OAoAm","#qAp","#OYgDv","#oGAGAqgg","#q/Y5EI","#OgAAW4BI","#OAAA9",,"#OcwAA","#OoAHA","20",,"#OUAAA","#OYAFA","40","#OQCAQOgEAg","#3DY4FQ_Bg","#OAAA25BI","#oFAYAqDE","#qEF2BI","#oDAYA4FQ","#OAAAb5BI","#1BI2FQ","#4BI_Eg","#QDo","#OAgAU","#OwAAF","#O4wD4","#oGAGAqgg","#qtQ4EI","#OYAAN3BI","#OAAA0","08"];

        dp=[,,,"#OoEAf","#OgDAG","#OgFAf","#OwFAG","#Ogbom","#OoaE3","#OoEAU","#Owgo0","#OIIIJ","#OBBpo","#OIIIJ","#OBBBIOAAFo","#OgGAd","#OgCAN","#QTm","#-XkgA","#OoFDE","#OAAFY","#OwGFD","#OADAW","#OgEAG","#OFAHY","#qFu","#4Aw","#qFH_","#qwo","#OgEAA","#OAGAf","#OoFAH","#OAAGg","#OANAG","#8FBI8FDY","0801","1001","#OIADA","#OIADA","#9IAA3Dw","#9IAA3Dw","#7Dw_","#7Dw_","5028","140a",,,,,,"#XYEAwFAYQ","#OgEAo3E4","#XoAAwEAFA","#QYC1BI","#OgEAA5DY","#OYDAg2E4","#XoAA4GAFA","#OgGAw6DY","#OoGAA7DY","#XoDAoCAgQ","#XwEA4GAoY","#OIAAi","#OoEAA6DY","#XoDAoDAwQ","#XgCAoEAYI","#OoGAw6DY","#7AA_Eg","#qAe","#Bk","#OYdAA","#OGEQA","#OYGgH","#OAogG","#5CQ5DY","#oCAoA6CQ","01","#5CQ_","#OCmAA","#5CQ",,"#OAfAA","#OE3AA","#5CQ","#8UBI8UDY","#OYGAg5AA2E4","#XoAA4GAFA","#QgG","#Q4E6DY","#OoFAw4E4","#X4AAwCADA","#OgFAo5DY","#OwFAA7DY","#OgADG","#Bl","#OAWFA","#OHGgD","#OoGgH","#OAoYG","#5CQ5DY","#oCAoA6CQ","01"];}
        }

        hd();w("<table><tr><td valign=top>");im(8);b(1,2);w("<\/td>");
        for(x=3;x<measure;x+=4){w("<td valign=bottom>");
        if(x==96)b(100);if(x==75){b(75);x-=3;}else b(x,x+3);w("<\/td>");}ft();
        */

        /*
        genre="ONLY ONE FINALE";title="SA.YO.NA.RA. SUPER STAR";
        artist="SUPER STAR 満-MITSURU-";bpm="85～170";measure=72;gap--;
        ln[40]=ln[41]=ln[42]=ln[43]=ln[44]=ln[45]=ln[46]=ln[47]=288;
        tc[2]=["1700"];tc[40]=[" 850"];tc[49]=["1700"];

        if(k){notes=656;
        c1[47]=[[0,64,32,1]];c1[48]=[[0,0,56,2]];c1[67]=[[0,0,128,1]];c1[68]=[[0,0,126,2]];

        sp=[,,"#OKcDT5AA","#OKcDTOwAA9","#OKdDT","#OKajTbF","#OKlEUOwHAw","#OKl0UQvF","#OKdEU6AA","#OLzWA7AA_Eg","#R1QrD_","#R1QzDbH9IFQ","#R1bb","#R1OAYgZbF","#R1OwfAYbG_","#R1O4YoZb2_","#R1OocBZ-C","#XLuUvLmUv","#XwZZoZgZo_","#X4iiwioiw","#XoZwZoZgZ","#X4wioAAiw","#X4ZZoZgZo_","#Xwi4iwioi","#X4ZZwZoZw","#r1QNBbF-O1Q","#R1OwdEd_","#R1OwdEZ5FQ","#R1Qzzb9","#R1OsaAZ4FQ","#R1OwYGebF_","#R1O4Y4Zb1_","#R1OgcBZOAoFH-C","#OIpBJbF-Okg","52","20","08","#6DA","04","8101","#r2OIdfBu_","#OLgQQ+o","#r2OIdfBm","#OLqi7AA","#r2OIdfBu","#OKgoQ+4","#r2OIfeB1","83",,"#XYAE3QogYBu-C","#XQDAQAIIUXoGAoAAoA-XkgA","#R1XwAAQgiEG_","#R1OAHY55Co","#R1XoACmgQYI4FQ-C","#XYCAIAII0XoGA4AAYA-XkgA","#R1X4rCEAIYM6FQ_","#XIrCEAIIIO4AAd-qw","#XXfgxpg0Q_","#X9Y0Q5xqg","#O9MrLb0","#XogIoKmLvbw","#XOWY6yo0Q_","#XrI0Q5xqg","#X4gIYoQ+ibw","#OKLMJOow43","#XLqmLqmd4","#XUxdUzvUw",,,,,"01","#_Aw"];

        if(a){notes=868;
        sp=[,,sp[2],sp[3],sp[4],sp[5],sp[6],sp[7],sp[8],"#UICAgCAoDA0WiO4AHA-Qs","#r7R1QrD_","#r7R1QzDbF9EFQ","#r7R1bb","#r7R1OAYgZbF2EI","#r5R1OwfAYbG_","#r4R1O4YoZb3_","#r7XNFNdNNUUQ3/-C","#XLuUvLmUv-cOwA","#XwZZoZgZob/_","#Xwiioioiwb/","#XoiIiwiIib9","#XwoZgADMIOA4AG","#XQZZgZoZgqGG8oAA_","#XoiwioZgZQ4+","#XoZZgZoLLO4GHE6FQ","#r1r7QNBbF3Fo-r_AA","#R1XwiYoCgao_","#R1XwiYoCgaI5FQ","#R1Xwrgrwioib/","#R1XogeQAFaI4FQ","#R1XwiYiEyYiOAAAu_","#R1X4iYi8EYKOAwA0_","#R1Xo6gYCIzLOAwEH-C","#r6OIhBJbE3Fo-Xggg","a2","40","10","#5DA","04",sp[39],"4b04102a5084","#OLPXQkoQ+A","5204102a5084","#OMKiQdA7AA","8a04102a5084","#OKMsQj4Q+A","#r2OIfeBt6AA","93",,"#XYQM3QogYBu-C","#XQjAQAIIUXoGAoAgoA-XkgA","#r7R1XwAAQgiEG_","#r4R1XAAA9YF4I","#XI4KmIQLLOoYgu-C","#XoiAIAII0X4GA4AgYA-XkgA","#R1X4rC8AIYMqA+_","#XIrC0AIIPO4AGd-XIoA","#XTlx+ripi_","#XmrX0diKd","#X+j1aKidc","#X0vqydULPOAAA1","#XamM9czNab/_","#XmrP0esadbo","#X+mrsUZ0a","#XLild03Lm","#R1X7qmrqmrv_","#XyldyjvU3-Qs","28",,,,"01",sp[72]];}

        if(l){notes=248;
        sp=[,,"08","#OoAA7",,"#QAK","#OYFA4","#QsR","04","#3AA_Eg","08","#OoAA7",,"#QAL","#OoHAo","#QjT","#OQBAA","#OMVen","#OoDBD","#OoDBD","#Qvr","#OaBAD","#OoDBD","#Qur","#O4GFG","#-O1Q","#OoDBD","#OoDBD","#Qur","#OaBAD","#OoAEU","#Q3r","#OQiBD","#-Okg","80","08","20","#6DA","10","#-C","#OYDv","#Q9Y","#OQBd","20","#OQCm","#Qz4","#OwEV","80",,"#OYA0R7B4","#OoCAB7BI","#XYAAQgBDF","#OAGg4","#OQAfL6B4","#OoHAF6BI","#X4ECEAAAo","#X4ECEAAY4","#QBJbt_","#R1OAoAp","#QLLb/","#OIYIZb/","#OUwrI","#OWf0Q","#O5xpg","#OPXfg","#OStb2","#OLoUw",,,,,"01","#_Aw"];}

        }else{notes=754;
        c2[47]=[[0,64,32,1]];c2[48]=[[0,0,56,2]];c1[67]=[[0,0,128,1]];c1[68]=[[0,0,126,2]];

        sp=[,,"#OcuFl","#OcuFl","#OcvGm","#OcsFd","#OgGA4","#Q1j","10","2001","#R1Q7D","#R1OAYAZ","#R1bb","#R1OAYAZ","#r3OwHAw","#OphZh","#r5OYCAA","#oYgowQKc","#XArrArArA_","#XAiiAiAiA","#r3oBBBB","#oAQAQbk","#OoDBD","#Qur","#O4GFG","#OJpAA-qM","#R1Q7D","#R1OAYAZ","#R1bb","#R1OAYAZ","#r7OoAEc","#Ov3vn","#r7OYjBD","#r41CQ-Qk","22",,"10",,"04","01","#r2QMg7AA","#OLow","#OKii","#OLqi","#OLrr","#OKgo","#OU00","82",,"#qAj8FAA_DA","#OQCBIOwEAA_BI","#R1Q7D","#R1OAYAZ","#XAACmgQYg_","#XACAAAAY06BI-OkA","#X4rCEAAA0","#XIACEAIII_Aw","#XUA0ArArI","#XArIGgFZA","#XGiArICAY","#XEAoBdADv","#XUA0ArArI","#XArIEQFZA","#owbCDqll","#XICAYEAdA","#OTjlg6E4","#XLCgdGAvA",,,,,,"#_Aw"];

        dp=[,,"20","#OwAA9",,"#QAc","#OKlEU","#OKlEU","#OKdEU","#OKiAA_Eg","#OrDDD","#r3OwAA9","#r3","#ODDjr",sp[10],"#R1O4YAZ","#OIIJbQ74","#oEFGHqTl",sp[22],sp[22],"#Qvr","#OaBAD",sp[18],"#r4oCCCC","#XAZZAZAZA","#OAABJbF-O0A","#r7OoDBD","#r7OoDBD","#Ov3vf","#r7OaBAD",sp[10],"#R1O4YAZ","#R1O4Y5Z","#OIoDbOAAFt_CQ","80","20",,"#5DA",,"8001","#OoF3","#Q+o","#OgDl","20","#OoF3","#Q+4","#OwEV","80",,"#XAADvoAgA_","#XADAAAAoU7BI-OkA","#XwAAQgiEG","#OAHY45Co","#OIIAJQrD_DA","#OYBBIOoHAA_BI","#R1O4YBZ","#O9AAd3BI_CQ","#oFFCCqfg_","#XmAHoGgAd","#X4A0ABYUD","#XomF4AUwA","#oFFCCqfg_","#XmAHoGgAd","#oGAwoQ8n","#XBYUDomAv","#ocsuH5FQ","#XAICAYmAv","0a",,,,"01",];

        if(a){notes=1192;
        sp=[,,"#OKYAQ","#OKYDT","#OKAAQ","#OKCDT","#OKlEU","#OKlEU","#OKAAQ","#UICAAAAAHAUym7AA_DA",sp[10],"#R1QzDbH9IFQ",sp[12],"#R1OAYgZbF","#R1OwfAYbG","#R1O4YoZb2","#R1XoAYgAIdN","#XLmU3AQAA4Dw_Eg","#r4R1Q9F","#r4R1OAoAp","#r4R1bt","#r4R1OAoAp2EI","#X4ZZoZgZo","#Xwi4iwioi","#X4ZZwZoZw","#OJoBoObADA-qU","#R1OwdEd","#R1OwdEZ5FQ","#XUzoUzoUx","#XUzoUzpUz","#R1OwYGebF","#XUz4Uxoym","#XIAIYIIddO4g9Abw","#qhJ4CQ3Fo-Qk","82",,"10",,"04","01","#OKaa","#OLmW","#OKaa",sp[43],"#OIBeOoEo","421288","#OV3r","82",,"#r4QLLB/","#XIhAIAIIU7AA7BI8kCQ","#R1XwAAQgiEG","#R1OAHY55Co","#R1XoACmgQYQ4FQ","#XYCAIAII0XoGA4AAYA","#XQgYgAgBhO+GGG","#OqaEbqk4q/A5Fo_DA","#r4R7QLD","#r4R7OAYAf","#X0R9iMyvL","#XynMxdUzv","#r4R1QzD","#XeiP0ViLv","#XY4YmY4Ym","#XLynMzvKm","#ObbJJO//tt","#XUxdUzvAA7AA_Eg",,,,,,sp[72]];

        dp=[,,"#OoEDD","#OwEA9","#OAdDD","#QDl","#OwHAw","#Qsa","#OwdEE","#OAirA-Qk","#OKcDTO+GGG","#r6OKcDT","#r6OKcDT","#r6OKaBs3EI","#OalEM7AA","#OcuDL7AA","#p3P5","#XAAAQLmU34CQ-O4A","#XwZZoZgZo","#X4iiwioiw","#XoZwZoZgZ","#X4wioAAiw",sp[18],sp[19],"#R1XAgogAIrL4Dw","#OABAJ3CQ5Fo-XoCA","#XUzoUzoUx7AA","#XUzoUzoUz","#R1Qzzb9","#R1OsaAZ4FQ","#X0VI0XYWi","#R1O4Y4Zb1","#p3P5","#OIoEMbF_CQ","01","20",,"#4DA",,dp[39],"#OIBfOgEo7AA","824228","#OIBeOYEo","420208",sp[40],"#OKjr","#OICaOwHu","82",,"#XYAE3QogYBu","#XQDAQAYojOoFAA6BI","#OUcEcO3HHH","#r4r6QLD1EI","#XQgYgQIddOvHsA6Dw","#qhl9gAA9gBI8kCQ3Fo","#R1X4rCEAIYM6FQ","#XIrCEAIIIO4AAd","#XTlz+jqzK","#XmrPsWiLv",sp[20],sp[21],"#XTlz+jq0V",sp[19],"#X+mrrOi+j","#X4Y404D4Y7FQ","#XLqmLqmLv","#XLLLLNNU3","0a",,,,"01",];}

        if(l){notes=402;
        sp=[,,"#OIFAY","#OIFAY","#OIFAY","#QLD","#OIFAY","#OIFAY","#OIFAY","#QMA","#bb_","#bb","#bb","#bb","#OoHAo","#QjT","#OQBAA","#QKc","#QBJ_","#R1","#R1","#R1",sp[22],sp[23],"#O4GAE","#-qU","#QDL","#OIYIZ","#QLL","#OIYId","#O4AEU","#Q3r","#OQiBD","#-OEg","20",,"10",,"04","01","#QDY_","#QLo","#QLY","#QNY","#QLY","#QLo","#QLY","02",,"#OYAFE4B4","#6AA8FCQ","#QBJ_","#OIIIJ","#OQAZU5B4","#OAHBI6BI","#oEUAA5FQ","#O4AAd","#OUADA","#OCAFY","#OAiAd","#QCc","#OUwAA","#QcS","#qjR","#Qcu","#ObAkA","#OLAfA",,,,,,sp[72]];

        dp=[,,"08","#OoAA7",,"#QAK","#OYFA4","#QsR","04","#3AA_Eg","08","#OoAA7",,"#QAL","#Q7D","#Q7D","#Q77","#ql3",dp[18],dp[19],dp[20],dp[21],sp[18],"#R1","#OIIDA","#-OxA",sp[22],sp[22],sp[23],"#OaBAA","#Q9t","#Q9t","#Q49","#qF/_","01","20",,"#6DA",,"#_DA","#OYDv","#Q9Y","#OQBd","20","#OIBd","#Qr4",dp[46],"80",,"#QAj6Co_","#8oBI1FQ","#XYAAQgBDF","#OAGg4","#OAAAb-C","#OoHAE6BI","#QwD","#XAECEAIIA","#OAvoo_","#Og1YA","#O0ALA","#OUFGH","#OAArI","#qug","#Qsa","#ql3","#OASAk","#OAdAW",,,,,"01",];}
        }

        hd();w("<table><tr>");for(x=2;x<measure;x+=4){w("<td valign=bottom>");
        if(x==42){b(x,x+4);x++;}else b(x,x+3);w("<\/td>");}ft();
        */

        /*
        genre="ANTHEM CORE";title="perditus†paradisus";
        artist="iconoclasm";bpm="172";measure=91;gap--;ln[91]=24;

        if(k){notes=1345;
        c1[2]=[[2,0,128,1],[7,0,126]];c1[3]=[[2,0,128,0],[6,0,128,1]];
        c1[4]=[[2,0,62,2],[1,64,64,1],[6,0,126,2]];c1[5]=[[1,0,128,0],[5,0,128,1]];
        c1[6]=[[1,0,62,2],[2,64,64,1],[5,0,126,2]];c1[7]=[[2,0,128,0],[4,0,62],[7,64,62]];
        c1[8]=[[2,0,128,0],[6,0,62],[5,64,64,1]];c1[9]=[[2,0,126,2],[5,0,62,2],[4,64,62]];
        c1[10]=[[1,0,126],[5,0,62],[6,64,54]];c1[11]=[[3,0,126],[6,0,62],[5,64,62]];
        c1[12]=[[2,0,62],[1,64,62],[6,0,126]];c1[13]=[[2,0,126],[7,0,62],[6,64,62]];
        c1[14]=[[3,0,126],[5,0,126]];c1[15]=[[4,0,126],[7,0,126]];c1[16]=[[2,0,126],[6,0,126]];
        c1[17]=[[3,0,126],[7,0,126]];c1[18]=[[2,0,62],[1,64,62],[6,0,62],[4,64],[5,96]];
        c1[19]=[[2,0,126],[6,0,62],[7,64,62]];c1[20]=[[3,0,128,1],[5,0,14]];
        c1[21]=[[3,0,126,2],[6,0,126]];

        sp=[,,"00","00","00","00","00","00","00","00","80","00","00","00","02","00","00","00","02","10","#XI9999999","00","#XoQ4gdB0Q_DA","#XLmUvdLmU-C","#XvidLmRdi-Qs","#X7lyi1Zsa-C","#X7N0RsajR-Qs","#ooo4oUYCmYCmoCmYEU-C","#o4o4oUoGiYCmoGmYCm-C_Eg","#o6ooYUo0AYGiYEUICm-C","#oQWyAUIDmoEA4EA1jR-C_Eg","#XcuTll3Kc_","#X+ssa1jjR","#XcuTll3Kc","#X+ssa1jjR","#XcuKcu6cu","#X+ssa1jjR","#XcuTll3Kc","#o0mmAU4FAYFA4FA+sa","#XLo6mLozv-coRA","#X0Q9Z0Q9e-coRA","#XhYxdUw7m-coRA","#X9Y9Z9Y0Z-coRA","#XUw5dUw6m-coRA","#X9YOi9YNZ-coRA","#XUwqmLo6m-coRA","#X+scrLu+i","#obQdAUIBAoGAIAnIDv7AA4CE2D0_","#XIoNNNiIo","#XNNIoNiIo","#XeVelWi9d_Dw","#XNNIoNiIo_","#XIoNNNiIo","#XffY4f0Y4","#XQwQwWWUw","#XNNIoNdNd","#XQwQwWWQi","#otAAAUYDAYFAYFA+sa","#oCeAHU1jAIEArrrrrA","#X1jQoKccu_","#X+sTLTlcu","#X9jQgKcTl","#X0Wr91jRd","#Xim7DKcTloAA//","#Xym7DKcTloAA//","#Xyl9KcTrloAA//","#oAAjAU0VZ0VZwAArqm","#R1T9+mrsU_","#XIAIAIAIOO4wokTF4mDoQ_FQ","#R1T9+mrsU_","#xAAAAAuA3UIo4IgwKcATlAQ+A_EgFQ","#UoeiYOioeiYOi_","#UoeiYOioeiYIU",,"#UoeiYOiZirxTl","#6AA7AM_",,"#U4gwIAAZ+rU1j2B45Cc","#URsdi1Zi+rysa","#oWAewUIEA+saIEA4sa_","#oeAVwUIFA+saIDA4sa","#oWgfYUIEA4Tu4FAoKm","#orAAAU4EAKcvwHr0VZ","#obCAGotAAAUQCA9cAKcudcAO2AAA_","#obDAGotAAAUIBA0VAKcudcAO/AAA","#obBAFo2AAAUQCAsaAU1fWTAO/AAA","#oSBoAo2AAAUIBAsaA4DQ1jRO/AAA","#XTT81LogoX11AAAAAASAA56EQ2Ew_","#XKKLPiogIXss9wAAAASAA66EQ3Ew_","#XTTrMKZ6cX++Aw09Fw_","#XKcqjuyI4X/H4/AAgAUAAAAAAAgYFYV6FA_","83"];
        sp[73]=sp[71];

        if(a){notes=2065;
        c1[2]=[[1,0,128,1],[4,0,128,1],[7,0,126]];c1[3]=[[1,0,126,2],[4,0,128,0],[6,0,128,1]];
        c1[4]=[[2,0,128,1],[4,0,62,2],[3,64,64,1],[6,0,126,2]];
        c1[5]=[[2,0,126,2],[3,0,128,0],[5,0,128,1]];
        c1[6]=[[1,0,62],[4,64,64,1],[3,0,62,2],[5,0,126,2]];
        c1[7]=[[2,0,126],[4,0,128,0],[6,0,62],[7,64,62]];
        c1[8]=[[1,0,128,1],[4,0,128,0],[6,0,62],[5,64,64,1]];
        c1[9]=[[1,0,126,2],[4,0,126,2],[5,0,62,2],[3,64,62]];c1[10]=[[2,0,126],[5,0,62],[6,64,54]];
        c1[11]=[[1,0,126],[3,0,126],[6,0,62],[5,64,62]];
        c1[12]=[[2,0,128,1],[4,0,62],[3,64,62],[6,0,126]];
        c1[13]=[[1,0,126],[2,0,94,2],[7,0,62],[5,64,62]];c1[14]=[[2,0,126],[4,0,128,1],[6,0,126]];
        c1[15]=[[3,0,126],[4,0,128,0],[7,0,126]];c1[16]=[[2,0,126],[4,0,62,2],[5,64,62],[6,0,126]];
        c1[17]=[[3,0,126],[4,0,126],[7,0,126]];
        c1[18]=[[1,0,126],[5,0,62],[3,64,62],[6,0,62],[4,64],[5,96]];
        c1[19]=[[2,0,126],[4,0,126],[6,0,62],[7,64,62]];c1[20]=[[1,0,128,1],[3,0,128,1],[5,0,14]];
        c1[21]=[[1,0,126,2],[3,0,126,2],[4,0,126]];

        sp=[,,"00","00","00","00","00","00","00","#7FQ","80","00","00","00","80","00","00","#1FQ","00","80","#XA9999999","#O44//","#XwQ4gdB0Q_DA","#XLV0XNTmlOsGfHB+-C","#XPVjVNjNjOeEfXQw2-Qs","#XLVzTNjsaOUFnHB+-C","#XMWsaOTMKOvHlrOwA4+-Qs","#XNQuYPQ+QUYHcAHlYEuAFlB2-Qo","#XPQdQPQcQUYGsAHlYGsAF3B2-Qs","#XOjVgPQKYUYvAAHaoGsAHlBu-Qo","#R1XaYcWuMYIUwGl4FA4CA+sa5Dw-Qs","#r2R1Xcucul3cub9_","#R1R7XGUsaGUrUquu","#R1XcWcWlXacOF9Gu","#R1X+Usa+TjUOF+F3","#R1XcWacuacWOF+H1","#R1R7XGUsaGTjUqut","#R1XcXadlXacOFuGu","#XOUMWKMKPX4oYoceogUAAAwAA4FA+oa","#R1oUVqvoGAEAOrkeWb0b/_","#R1orilioFAHAO0bzrbvb4","#R1okebWoGAFAOdlUcb1b/","#R1R7osqyqoGAEAODjFTb0","#R1olVcWoHAGAOec1kbub/","#R1R7oqqrtoEAEAODTCbbk","#R1oVU0eoGAGAOkbvUbsb2","#R1X+scuqKeKXAA4AEe4g5Dw","#R1XdVaodVwQU4DAwHAAGnADv4CE3D06FQ_","#XIWNVNTIWXYoYeYmgob/","#r2R1XddYudLgOXAG4AAm4o","#XeXcTUMaaX4owo9V337Dw_Dw","#r2R1R3XFc4sF04soFAFA_","#r2R1R3XAs9cF04soAFFA","#r2R1R5XHuY+HsY+oHAGA","#r4R1R3XA3o3Geq37EI","#r2R1R3XFd4uFl9loGAGG","#r2R1R4XA141Gd40oAAGF","#XNVIWIWKIXYeYoYoYgUAAA4AAAAA+oa","#X3aLTIIIPXAG9moYoYUFgAAAA7or7oA","#R1X1TgQaccWXAm4o3u4o_","#R1XeUaajicWX4o4o314o","#R1X+UYQacTlXAo4o3u4w","#R1X1WsUuUacXAY44AusuOAAH4","#R1XrVzTqcTlXAg4AH3v3","#R1X0WrTqcTlXAo4AH3v3","#XLVMTNULVXwgwwnfvnb+","#XIYYgKKKIXwAoAs0oYUFgR+oaAAA7ou","#XKiIQKCPoTd+mrsU8oAA_","#R1X6iwQqCnOTF4mDoQ8MFQ_FQ",,"#R1ZYQgQwAQAYAKDgAMGU4o4Agw8uA9yAqA/_EgFQ","#R1R7UYUaAdjAUaAdjTowGAwG_","#R1UoVjwsaoVj4TlT+A4GAo","#R1Uwdj4mUwdj4mUT4wFAwF_","#R1U4VjwsalxlyZuqAc6Ag7CA7Dw","#XKmrLUv0U3AA7AM_","#XLv0UcvzN",sp[77],"#XQgIoQIPIUBofsQZ0eTysaTAAwHoA","#oWAewUIEA+saIEA4sa5AA_",sp[80],sp[81],"#orAAAU4EAKcvwFjRsaUAAAAAAAH1j+s","#XLTIiIgImXd1wAoAYAUwAA9YACYu9YA_","#XLTIaIgImXl9wAgAYAU4AA9gACYu9YA","#XLTIVIoIdXm+oAQAQAU4AA8YAEwd+QA","#XKaITNIIIXe+oA4YoYU4AA8YAAAQ+gU","#R1XTTiajrgIX1191AA4oSAA75EQ2Ew_","#XLTLXKMQIXV1VwgYooUwAAwAAAF+EYAb4_","#P1XccrUaaicX++4w099w_","#oUT1Aov3AAUIBAIBALiZiZUUYDAoEAoGA93FO4/AA7AY_","83"];
        sp[69]=sp[67];}

        if(l){notes=658;c1=[];
        c1[2]=[[7,0,126]];c1[3]=[[5,0,128,1]];c1[4]=[[5,0,126,2]];c1[5]=[[3,0,128,1]];
        c1[6]=[[3,0,126,2]];c1[7]=[[1,0,62],[6,64,62]];c1[8]=[[4,0,62],[2,64,64,1]];
        c1[9]=[[2,0,62,2],[5,64,62]];c1[10]=[[3,0,62],[7,64,54]];c1[11]=[[7,0,62],[3,64,62]];
        c1[12]=[[5,0,126]];c1[13]=[[7,0,62],[6,64,62]];c1[14]=[[4,0,126]];c1[15]=[[6,0,126]];
        c1[16]=[[3,0,126]];c1[17]=[[7,0,126]];c1[18]=[[5,0,62],[1,64],[3,96]];
        c1[19]=[[5,0,62],[7,64,62]];c1[21]=[[1,0,126]];

        sp=[,,"00","00","00","00","00","00","00","00","00","00","00","00","00","00","00","00","00","00","#XYrrrrrrr","00","#XAAAAfB0Q_DA","#oYgowQKc-C","#o4owgQri-Qs","#Q9r-C","#oEDCAQ+s-Qs","#oYg4wQKs-C","#owo4oQjr-Qs","#o4wwgQsi-C","#OLu9Z-Qs","#XU3AALvAA_","#O0a9a","#XLvAAKmAA","#O9irR","#XU3AAVAeA","#O9a0a","#XLvAAU3AA","#O0d+i","#oFAEAQPO_","#oBAFAQrX","#oEADAQeV","#oEACAQ+8","#oCADAQsN","#oEACAQ+s","#oFAFAQPX","#O1aL45EI","#ONNKE_","#ONNKE","#ONNKE","#OddWf_Dw","#R1qNl5BI_","#R1qpl5B4","#R3qf37BI","#R2q2W6DY","#R1qNb5AY","#R2q2U6EI","#Obdd+5AY","#OrRAA_DA","#O9ILm3BI_","#O+QLU4BI","#O9IUd3BI","#Oyv0V","#XYo8EGGGG","#XIY9FHHHH","#XIgzDHHHH","#OsR0Z","#T5+OpsM_","#OwogVTBwNBgA","#T5+OpsM_","#OwoTvTBwNAAA-OAw","#T0VZ0Xr_","#T0VZ0VZ","#T9eirOi_","#T9eirOi","81",,,"#O9i1a","#XIm4AId4A_","#XIUwAId4A","#XIm4A4U4A","#O9I1i3BI","#X55YAQYgo_","#X55gAIYgo","#XyyoAIQYg","#XppYA4wgQ","#XNNYA4wgQ_","#XPPoAwogQ_","#XppAWQmg4","#XpFIpGQYo","83"];}

        }else{notes=1341;
        c1[2]=[[1,0,128,1]];c1[3]=[[1,0,128,0]];c1[4]=[[1,0,62,2],[3,64,64,1]];c1[5]=[[3,0,128,0]];
        c1[6]=[[3,0,62,2],[2,64,64,1]];c1[7]=c1[8]=[[2,0,128,0]];c1[9]=c1[21]=[[2,0,126,2]];
        c1[10]=c1[15]=c1[19]=c2[14]=[[4,0,126]];c1[11]=c2[2]=c2[17]=[[7,0,126]];
        c1[12]=[[5,0,62],[3,64,62]];c1[13]=c2[21]=[[1,0,126]];c1[14]=[[2,0,126]];
        c1[16]=c2[12]=[[5,0,126]];c1[17]=c2[15]=[[6,0,126]];c1[18]=[[4,0,62],[2,64,62]];
        c1[20]=[[2,0,128,1]];c2[3]=[[6,0,128,1]];c2[4]=[[6,0,126,2]];c2[5]=[[5,0,128,1]];
        c2[6]=[[5,0,126,2]];c2[7]=[[4,0,62],[6,64,62]];c2[8]=[[4,0,62],[3,64,64,1]];
        c2[9]=[[3,0,62,2],[2,64,62]];c2[10]=[[1,0,62],[5,64,54]];c2[11]=[[5,0,62],[4,64,62]];
        c2[13]=[[7,0,62],[6,64,62]];c2[16]=[[3,0,126]];c2[18]=[[6,0,62],[2,64],[4,96]];
        c2[19]=[[5,0,62],[7,64,62]];c2[20]=[[3,0,14]];

        sp=[,,"00","00","00","00","00","00","00","00","80","00","00","00","00","00","00","00","00","80","#B/","#O44//","#XAQAgdBAA","#XAAAALvmA_","#XAd+gAD+g-C","#XAAAy9ZAB_","#X0QAD9YAA_Eg","#UACmADvACmABA_","#UAGiABdACmABA_","#UAEAAHrAFZACm2BI_","#X7oEwAA4A5Ew3FA_DA","#XAATlAAKc_","#XAAsaAAjR","#XAATlAAKc","#XAAsaAAjR","#XAAKcADlA","#XAAsaAAjR","#XAATlAAKc","#XAADlwAAA5Fg3Fw","#XKgCmDoCm","#oopgpqbT","#oQNgWqj0","#ooy4pqcb","#ogNQfqzl","#owpgpqTT","#ogNgfqz1","#XAAUz4AGi","#OBoII3DY5D07EA_","#R11CQ2EI","#R11Aw2EI","#XYQYgWiAI","#OBIIIb/2EI_","#R1OA5A42EI","#R3OD4A44EI","#R2OA4C44E4","#R1OB4A7","#R2OA4C82Fo","#ObYY9sHAD1Fw","#tGAAbbYbQ4BI","#XAAAAKcoA_","#XAAAATlwA","#XAAAAKcAA","#XEQrGAAsY","#XCmAALvU3","#XDvAAKmLv","#XCmAIdULv","#UAFZAGiAAALoA","#QBJ_","#O4wokTF4mDoQ","#QBJ_","#U4o4wgwLvAAAA-OAw","#T9GgrEQ","#TrEQZEQ","#UAGiAHrAGiAFZ_","#UAGiAHrAA0RdA","01",,"#UAgAYAAA9AUwA4Cc","#ULAUArA0ArAGi","#XKcAAcuAA","#XTlAAcuAA","#XKcAATlAA","#UAAALqmAAAAGi","#otBAAUIBA4AAUwAd4A","#otCAAUIBAgAAm4Ad4A","#o2AAAUQCAoAALoAUwA","#XMMwDAAoA3Ew1FA","#XddwALoAoS4A7","#XUUNAAwAI5Ew3FA","#XmmAddUUN","#XKco9YAgATAAABwR","01"];

        dp=[,,"00","00","00","00","00","00","00","00","00","00","00","00","02","00","00","00","02","00","#XArrrrrrr","00","#Oo4Ay4E4_DA","#XLvUwAAAU_DA","#X3AACmYAB_Eg","#X+ipAAA0Q_DA","#XAHrIAC0R-C","#oow4wQcs1Fg3Fw","#o4g4wQqs1Fg3Fw","#o4wogQsa2Ag","#oAEhAUAAd4AAwCAAHr_",sp[79],"#X+sAA1jAA",sp[80],"#X+sAA1jAA","#Xl3AAcoAl","#X1jAAsaAA","#XcuAAl3AA","#X1jQAEu4I5Ew3FA","#oGAEAQGM_","#oFADAQ9T","#oFAGAQNW","#oDAEAQ78","#oGAFAQWV","#oDABAQz5","#oGAGAQWm","#X+iAABdYA","#XLDCwAFAY7AA4CE7EQ5Fg7Fw","#oAtoAqol","#otAoAqFl","#o11AZbH_Dw","#XdFAoNgAo","#XIoFFNgAo","#XPHA4PwA4","#XIwAwOGAw","#XNFAoNdFF","#XIwAwOGAA","#XNFAoIowA4Ew2FA","#oAfAGUrIAAFAoooooA","#XsaIAAABd","#X+sYAAACm","#X9jQAAATl","#XwFA41jAE","#oAb//QXA","#oAJttQlA","#oAb//QfA","#oAASAU0QArIAgAAACm","#T9+mrsU","#R1_FQ","#T9+mrsU","#OIIA12D84EU1Ew3FA7Fc",sp[73],"#UAGiAHrAGiAAd",sp[71],"#UoYAgQA9ZAAAU","#6AA7AM",,"#U4AwAAAZAiAHr5B4","#UA0ArAiArA0QA","#UAAA0VZAAAwVZ4E4_","#UAAA0VZAAA4ei5E4","#oAYAgUAAAIqmAAAQzv","#orAAAUwEAAAAwGirIA","#X77IAAYAm5EA7EQ","#X77gAAIAU3EA5EQ","#XyyYBAQAd4EA6EQ","#X88QA9YAw1EA4Fg2Fw","#XrrQBAAoA7EQ3Ew","#XM04fAAIA6AA5EA3EQ","#XiiArr007","#XtFotEgI4X/H4/GwAA3Ew5FA4Fw","42"];

        if(a){notes=2109;c1=[];c2=[];
        c1[2]=[[1,0,128,1],[5,0,128,1]];c2[2]=[[7,0,126]];c1[3]=[[1,0,126,2],[5,0,128,0]];
        c2[3]=[[6,0,128,1]];c1[4]=[[3,0,128,1],[5,0,62,2]];c2[4]=[[2,64,64,1],[6,0,126,2]];
        c1[5]=[[3,0,126,2]];c2[5]=[[2,0,128,0],[5,0,128,1]];c1[6]=[[1,0,62],[5,64,64,1]];
        c2[6]=[[2,0,62,2],[5,0,126,2]];c1[7]=[[2,0,126],[5,0,128,0]];c2[7]=[[4,0,62],[7,64,62]];
        c1[8]=[[1,0,128,1],[5,0,128,0]];c2[8]=[[6,0,62],[5,64,64,1]];
        c1[9]=[[1,0,126,2],[5,0,126,2],[3,64,62]];c2[9]=[[5,0,62,2]];
        c2[10]=[[2,0,126],[4,0,62],[6,64,54]];c1[11]=[[1,0,126],[3,0,126],[6,0,62],[5,64,62]];
        c2[12]=[[1,0,128,1],[4,0,62],[2,64,62],[6,0,126]];c1[13]=[[1,0,126],[7,0,62],[6,64,62]];
        c2[13]=[[1,0,94,2]];c2[14]=[[3,0,126],[7,0,128,1],[5,0,126]];c1[15]=[[3,0,126],[7,0,126]];
        c2[15]=[[7,0,128,0]];c1[16]=[[7,64,62]];c2[16]=[[1,0,126],[3,0,126],[7,0,62,2]];
        c2[17]=[[2,0,126],[4,0,126],[6,0,126]];c1[18]=[[1,0,126],[3,0,62],[5,0,62]];
        c2[18]=[[1,64,62],[3,64],[5,96]];c2[19]=[[2,0,126],[4,0,126],[6,0,62],[7,64,62]];
        c1[20]=[[1,0,128,1],[3,0,128,1]];c1[21]=[[1,0,126,2],[3,0,126,2]];c2[21]=[[3,0,126]];

        sp=[,,"00","00","00","00","00","00","00","00","#O8EEE","00","#r4","00","#O8EEE","00","#r4","#r61FQ","#O4AEE","#O0EEE","a080","#O44//","#OqEAA1EI_DA","01208a20","0120c28a","01209220","#OAoIJQAlQA/_","01209220","0120c2a0","01208a20","#UAAAIAAIBA+saQFo7DA_","#XKm9iLv0a","#r7R1bt","#Xc3sRUu9i","#r7R1bt","#XUuiZUvc3","#r7R1bt","#XKmriLvUu","#XI4I4sajRbo","#XAUIUIUIUb/_","#R1XAU4UAU4A","#XKm1iMv7m","#X9iRd+jer","#r2R1oEEEEb/",,"#XLv+il35d","#X+scxU3sR","#R3X9d4oFy4o","#R3XAo9dFy4o","#R3XFd4oFy4o","#XYoYoiiNNb4","#XIeIGIGIGQbb","#XIGIeIGIGQbb","#XImIGIGIGQkk","#XIGIGIeIGQbb","#XIWIGIGIWQSS","#R1R4oGGGE6Fo","#R1XFFAvAvdN7BI3FQ","#oCeAHU+jAIEAiiiiiA","#X1jQAKlcu","#X+sYANWIoQAb","#R1Xmmgwmd4w","#XIoIo+saNQbA","#XddYoKcTl","#XQwQwTlcu","#XNNIgpUrl","#oAAjAU0XArPAwHArqm","#XIiIQKCPr8qAA","#XIiIQKCPL5FQ","#TF+mrsU_","#UYoYQgQLvAAAA-OAw","#R1Td+mrum7AA","#R1T8+eque","#UAOi4eioOi4ei_","#UoOi4eiO5mIBAOAAC/","01",,,"#UA0ei9Zi+jyrI","#XClAALmAA_","#XLuAAKdAA","#oWAQAUIEAADuAEAAKm","#UAAATlwAFjR1j","#R1XUU4wQA4w4DQ5Dg","#R1XUUo0YAAw4DQ6Dg","#R7XKKArIQAQ4DQ6Dg","#XccwIIIIIO44Ft4Bw2CA","#R1XccyiDm9I3FQ","#XNlM3MLvIOgYyA","#XEULQc3Klbo_","#oUTAAUYDAoEA+saNjR7AA","01"];
        sp[44]=sp[43];

        dp=[,,"00","00","00","00","00","00","00","#7FQ_Eg","80","#r4","00","#r4","02","#r4","00","00","#O9FAA","00","#XI9999999","#r6","#XAA4AdA0Q","#XKmc3dLmU7AA","#XP0dimZUr5AA","#XPidRmZdi5AA","#XOZlRdiLU4AA","#oogwgUICm4BdQDvIGi3AA","#o4o44UIGiICmYGioBd5AA","#oxo4oUQiAYGioGiICm7AA","#XPQ5d1jIIOYAAvbH4BA6BQ","#r7QBJbb_","#X+jsR9i0a","#r7R1bb","#X0asZ+jsa","#r7R1bb","#X1jsR+i1i","#r7R1bb","#oieVAUwDAIEA4EA+sa","#XKn+jKmzm","#X9aRm9e0a","#r6R1oEEEEb/","#r6R1oEEEEb/","#XTv+jKmyd","#X+jZU9aWj","#r6R1oEEEEb/","#R1XA040AIoIqAt","#oAQFAUAGAIGAIAnIDvbo4CE2D0_","#XI0I0I0I0","#r6XIEIEIMIM","#X9XsW7Oiv7Dw","#r7XAEIgErIg_","#r7XAgMEErIg","#r7XDDIYDiIY","#r7XAgIgEEKg","#r7XDDIYDrLD","#r7XAgIgEcIg","#UoFAIHAoHA1jROHoAA","#XHG9UIIIIOAAtt","#XAdIYNUIoQDa_","#XIoNdTlcuQbA","#X+sYAKcTl","#X0Vi9NdIoQAb","#R1XiczDGGGG","#R1Xqm8EHHHH","#R1Xyd6CHHHH","#R1UAHrAGiABAYBAqAb",sp[69],"#OYQIkTFYiDIQ6Fo",sp[67],"#R1XAiAQCI4I3D85EU2Ew4FA6FQ",sp[73],"#UoOi4eioOi4Ym","#R1T9+mrum","#OIIIjT8+eqoU1Ew5Fw","#6AA7AM",,"#U4gwYAAZ+jU1j4B45Cc","#OQABJ1AQ","#UIAA1jRAAA4sa5AA6E4","#UAAA1jRAAA4sa6E4","#oAgHYQH92CA","#orAAAU4EAAAGgFjR1j","#okBAFUIGAeiAAEuMLAQ3H","#okAAFUwGAehAABdWUAb/","#o2AAEU4HAMQAABeOTAbq","#okFgAUoFAIDAwD41jRbQ","#X11c0QYgAS4A56EQ2Ew","#XssyvAYgIS4Ay5EQ3Ew","#P1X99A0099m","#XbEgbCRQoX/H4/FoAA3Ew4FA6Fw","82"];}

        if(l){notes=665;c1=[];c2=[];
        c2[2]=[[5,0,126]];c1[3]=[[5,0,128,1]];c1[4]=[[5,0,126,2]];c2[5]=[[3,0,128,1]];
        c2[6]=[[3,0,126,2]];c1[7]=[[4,0,62]];c2[7]=[[6,64,62]];c1[8]=[[4,0,62]];c2[8]=[[5,64,64,1]];
        c2[9]=[[2,0,62,2]];c1[9]=[[3,64,62]];c2[10]=[[3,0,62]];c1[10]=[[5,64,54]];c2[11]=[[5,0,62]];
        c1[11]=[[4,64,62]];c2[12]=[[5,0,126]];c1[13]=[[7,0,62]];c2[13]=[[6,64,62]];c1[14]=[[3,0,126]];
        c2[15]=[[6,0,126]];c1[16]=[[5,0,126]];c2[17]=[[7,0,126]];c1[18]=[[3,0,62],[5,96]];
        c2[18]=[[4,64]];c2[19]=[[6,0,62]];c1[19]=[[7,64,62]];c2[21]=[[4,0,126]];

        sp=[,,,"00","00",,,"00","00","00","00","00",,"00","00",,"00",,"00","00","#oDbbb",,,"#oAAowQAc_","#oAAogQAa_","#Q9A","#oFEDA6AA","#oAgAobT_","#oAgAwbU_","#ooAAgQYC","#OQAA74AY","#3DA5DY_","#OAA0R","#ooAwABc","#OAA9Z","#OQAdA4AY","#OAA0R","#ooAwABc","#X4oYAGECB","280a","1428","0a14","a028","#oEADAb1","#oCABAbj","#oEAEAb2","#ODvAA6EI","#OAILI_","#OIJII","#OJIII","#OIIUA","#QBJ_","#OIJII","#ObYYY","#OQQSQ","#OJIII","#OQQSQ","#QbY","#O0QDD","#OAALv","#OAAKm","#OAALU","#OEwAB","#XYo4AAHAH","#oAkww","#XYo4AAHAH","#OAArI","#TACmADv_","#OAQAJ4CA6Cg","#TACmADv_","#bQ4CA6Cg-OAw","#T0FYiDI","#T0FYiDI","#TA4GAoE_","#TAwFAiA","01",,,,"#OLAUA","#OdAmA","#OmAog6Fg","#OArAA6FA4Fg","#XFFAAQgAA","#XFFAAYgAA","#XCCAAYoAA","#XDDAAAAwg","#otAAA1DA","#Ot4Gi","#okFu3","#XADoAEwAA","01"];

        dp=[,,"00",,,"00","00","00","00","#_Eg","00","00","00","00",,"00",,"00","00","00","#Odttt","00","#_DA","#oYgAAQKA","#o4oAAQrA","#QAr_","#QFj_","#oYAwABM","#ooA4ABd","#oAwwAQEg_","#OAd7A_","#oEAGAqQg","#O9ZAA","#oFAGAqYg","#O+iAA","#OBAAm3BI","#O9iAA","#oFAHAqYo","#OAE0R","#oFAEAb+_","#oDAFAbv","#oCAEAbm","#oDACAbs","2814",sp[39],sp[40],"#OgAew6AA","#XIADoAmAv","#oAtAAqol","#otAAAqFl","#OFFAU","#qtl","#oAtAAqol","#o/AAAqH3","#oAA2Aq2G","#otAAAqFb","#oAA2Aq2E","#OFFFi","#OADoo","#O0QAA","#O9YAA","#OrIAA","#OwH0Q","#oAb44","#XQgwAAGAG","#oAb44","#O0ZAA","#T9YA0QA","#T9YA0QA","#T9YA0QA","#qAfT9YAIFA",sp[73],"#TA4GAoD","#T9GgrEQ","#T0FYiAd","80",,,"#OAABJ",sp[45],"#oDAFAbv","#XAEwEwdAA2Fw","#O0ArI","#O/oAU","#O/YAl","#O2QAL","#OtgZA","#OtYGi","#obAAA1DA","#OkFu3","#XrAArAAQg","40"];}
        }

        hd();w("<table><tr>");for(x=2;x<measure;x+=4){w("<td valign=bottom>");
        if(x==87)b(91);if(x==22){b(x);x-=3;}else b(x,x+3);w("<\/td>");}ft();
        */
    }

    uint32_t ScorePlayback::positionGapAtOnce(const uint16_t hiSpeed)
    {
        return BaseMeasureSize * BaseHiSpeed / static_cast<uint32_t>(hiSpeed);
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

    void ScorePlayback::position(const uint32_t position)
    {
        // TODO: temporary; position to timePosition_
        timePosition_ = std::chrono::nanoseconds(position * 1000000);
    }

    uint32_t ScorePlayback::position() const
    {
        if(!score_)
        {
            return 0;
        }

        // BaseMeasureSize: 4 beats
        // 120 BPM: 120 Beats / 1 minutes

        double timePositionMinutes = static_cast<double>(timePosition_.count()) / std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::minutes(1)).count();

        return static_cast<uint32_t>((score_->firstBpm() * BaseMeasureSize / 4) * timePositionMinutes);
    }

    void ScorePlayback::progress(const std::chrono::nanoseconds interval)
    {
        if(!score_)
        {
            return;
        }

        timePosition_ += interval;
        if(timePosition_.count() < 0)
        {
            timePosition_ = std::chrono::nanoseconds::zero();
        }
    }
}
