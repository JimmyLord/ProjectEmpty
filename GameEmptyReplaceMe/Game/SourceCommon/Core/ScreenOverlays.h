//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __ScreenOverlays_H__
#define __ScreenOverlays_H__

class Screen_Base;

enum ScreenOverlays
{
    ScreenOverlay_DontChange,
    ScreenOverlay_Destroy,
    ScreenOverlay_RemoveDontDestroy,

    ScreenOverlay_GameType_Selector,
    ScreenOverlay_GameType_EmptyReplaceMe,

    ScreenOverlay_Loading,
    ScreenOverlay_HowToPlay_Online,
    ScreenOverlay_HowToPlay_Infinite,
    ScreenOverlay_HowToPlay_Puzzle,
    ScreenOverlay_Options,
    ScreenOverlay_Achievements,
    ScreenOverlay_Stats,
    ScreenOverlay_Login,

    ScreenOverlay_NumScreens,
};

enum LayoutTypes
{
    LayoutType_Wide,
    LayoutType_Tall,
    LayoutType_Square,
    LayoutType_NumLayouts,
};

class ScreenOverlayManager
{
public:
    Screen_Base* m_pScreens[ScreenOverlay_NumScreens];
    MyFileObject* m_pLayoutsFile;
    char* m_LayoutJSONStrings[ScreenOverlay_NumScreens][LayoutType_NumLayouts];

    bool m_NeedsTick;

public:
    ScreenOverlayManager();
    ~ScreenOverlayManager();

    void Tick();

    void LoadLayouts();
    void SaveLayouts();
    Screen_Base* CreateScreen(ScreenOverlays screentomake, Screen_Base* pParentScreen, void* pPtr1, void* pPtr2);
};

extern ScreenOverlayManager* g_pScreenOverlayManager;

#endif // __ScreenOverlays_H__
