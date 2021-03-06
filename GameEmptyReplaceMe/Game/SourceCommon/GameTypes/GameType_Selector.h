//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __GameType_Selector_H__
#define __GameType_Selector_H__

#include "../../../SharedGameCode/Screens/Screen_Base.h"

enum MainMenuItems
{
    MainMenuItems_None_BackgroundItem,
    MainMenuItems_None_MenuTitleBG,
    MainMenuItems_None_MenuTitleText,
    MainMenuItems_None_FlatheadGamesBG,
    MainMenuItems_None_FlatheadGamesText,

    MainMenuItems_FirstButton,
        MainMenuItems_MainGame = MainMenuItems_FirstButton,
        MainMenuItems_Achievements,
        MainMenuItems_Options,
        MainMenuItems_Stats,
    MainMenuItems_LastButton = MainMenuItems_Stats,

    MainMenuItems_Max,
};

class GameType_Selector : public Screen_Base
{
public:

public:
    GameType_Selector();
    virtual ~GameType_Selector();

    virtual void Init();
    virtual void Tick(double TimePassed);
    virtual void Draw();

    virtual bool OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual bool OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);

    bool OnMenuAction(int action);
};

#endif //__GameType_Selector_H__
