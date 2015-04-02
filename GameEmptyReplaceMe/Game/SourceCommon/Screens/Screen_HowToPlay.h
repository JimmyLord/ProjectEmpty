//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Screen_HowToPlay_H__
#define __Screen_HowToPlay_H__

enum HowToPlayActions
{
    HTPA_None,
    HTPA_Back,
    HTPA_SwitchPage_Online,
    HTPA_SwitchPage_Infinite,
    HTPA_SwitchPage_Puzzle,
    HTPA_SwitchPage_Login,
    HTPA_SwitchPage_Scoring,
};

enum HowToPlayPageType
{
    HTPPT_Online,
    HTPPT_Infinite,
    HTPPT_Puzzle,
    HTPPT_Login,
    HTPPT_Scoring,
    HTPPT_NumPages,
};

class Screen_HowToPlay : public Screen_Base
{
public:
    bool m_NewBest;
    bool m_Saved;
    int m_TicksBeforeWeSave;

    HowToPlayPageType m_PageShowing;

public:
    Screen_HowToPlay();
    virtual ~Screen_HowToPlay();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);

    void DrawHelp_Online();
    void DrawHelp_Infinite();
    void DrawHelp_Puzzle();
    void DrawHelp_Login();
    void DrawHelp_Scoring();
};

#endif //__Screen_HowToPlay_H__
