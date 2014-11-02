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

#ifndef __Screen_Stats_H__
#define __Screen_Stats_H__

#include "../Core/Stats.h"

enum StatsFilterTime
{
    SFT_AllTime,
    SFT_Daily,
    STF_NumOptions,
};

enum StatsFilterGameType
{
    SFGT_All,
    SFGT_Online,
    SFGT_Infinite,
    SFGT_Puzzle,
    STGT_NumOptions,
};

extern int StatsToShowForEachFilter[STGT_NumOptions][Num_PlayerStats];

class Screen_Stats : public Screen_Base
{
protected:
    float m_ScrollAmount;
    float m_Momentum;

    int m_InitialFinger;

    StatsFilterTime m_FilterTime;
    StatsFilterGameType m_FilterGameType;

public:
    Screen_Stats();
    virtual ~Screen_Stats();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);
};

#endif //__Screen_Stats_H__
