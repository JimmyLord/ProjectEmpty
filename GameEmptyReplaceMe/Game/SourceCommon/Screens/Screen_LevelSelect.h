//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Screen_LevelSelect_H__
#define __Screen_LevelSelect_H__

class Screen_LevelSelect : public Screen_Base
{
protected:
    int m_Page;
    int m_Width;
    int m_Height;

    int m_PuzzleButtonFingerWentDownOn;
    int m_InitialFinger;
    bool m_SwitchingPages;

    float m_ScrollAmount;
    float m_Momentum;
    int m_PageDirectionToSnapTo;

public:
    bool m_ArrivedFromPause;

public:
    Screen_LevelSelect();
    virtual ~Screen_LevelSelect();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);
};

#endif //__Screen_LevelSelect_H__
