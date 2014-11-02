//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Achievements_H__
#define __Achievements_H__

enum AchievementID
{
    ACHID_FirstWord,            // done
    ACHID_First8LetterWord,     // done
    ACHID_InfiniteLevel2,       // done
    ACHID_InfiniteLevel5,       // done
    ACHID_InfiniteLevel10,      // done
    ACHID_InfiniteLevel15,      // done
    ACHID_InfiniteLevel20,      // done
    ACHID_PuzzleLevel1,         // done
    ACHID_PuzzleLevel10,        // done
    ACHID_PuzzleLevel20,        // done
    ACHID_PuzzleLevel30,        // done
    ACHID_PuzzleLevel40,        // done
    ACHID_PuzzleLevel50,        // done
    ACHID_PuzzleAll1Star,       // done
    ACHID_PuzzleAll2Stars,      // done
    ACHID_PuzzleAll3Stars,      // done
    ACHID_17,
    ACHID_18,
    ACHID_19,
    ACHID_20,
    ACHID_21,
    ACHID_22,
    ACHID_23,
    ACHID_24,
    ACHID_25,
    ACHID_26,
    ACHID_27,
    ACHID_28,
    ACHID_29,
    ACHID_30,
    ACHID_NumAchs,
};

struct Achievement
{
    AchievementID achid;
    char achidstr[32];
    char name[32];
    char descriptionUnearned[256];
    char descriptionEarned[256];
};

extern Achievement g_Achievements[ACHID_NumAchs];

class AchievementChunk
{
protected:
    float m_TimeAlive;
    float m_TimeToLive;

public:
    bool m_InUse;

    char m_String[32];
    float m_X;
    float m_Y;
    ColorByte m_BGColor;
    ColorByte m_Color;
    float m_Size;

    AchievementChunk();
    ~AchievementChunk();

    void Reset();
    void Setup( float x, float y, AchievementID achid );

    void Tick(double TimePassed);
    void Draw();
};

#endif //__Achievements_H__
