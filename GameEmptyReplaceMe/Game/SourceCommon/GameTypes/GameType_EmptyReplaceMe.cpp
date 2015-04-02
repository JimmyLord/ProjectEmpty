//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "GameType_EmptyReplaceMe.h"

#include "../Core/ProfileManager.h"
#include "../Core/GameEmptyReplaceMe.h"
#include "../Core/ResourceManager.h"

GameType_EmptyReplaceMe::GameType_EmptyReplaceMe()
{
    sprintf_s( m_ScreenName, MAX_SCREENNAME_LENGTH, "GameType_EmptyReplaceMe" );
    m_ScreenOverlayIndex = ScreenOverlay_GameType_EmptyReplaceMe;

    m_MenuItemsNeeded = 1;
    m_TimeToOpen = 0.5f;
}

GameType_EmptyReplaceMe::~GameType_EmptyReplaceMe()
{
}

void GameType_EmptyReplaceMe::Init()
{
    LOGInfo( LOGTag, "GameType_EmptyReplaceMe::Init() buttons = %d\n", m_MenuItemsNeeded );

    MenuItemDefinition MenuItems[IGBA_Max] =
    {
        //name,        type,       sprite,                   bgcolor,                     font,    maxletters, str1, str2, str3,                buttonaction
        { "Quit",      MIT_Button, SL_WhiteSquare, ColorByte(150,150,150,255), g_pGame->m_pSystemFont,   4, "Quit", 0, 0,                         IGBA_Quit }, //
    };

    CreateMenuItems( IGBA_Max, MenuItems );

    Screen_Base::Init();
}

void GameType_EmptyReplaceMe::OnDisplay()
{
    Screen_Base::OnDisplay();
}

void GameType_EmptyReplaceMe::UpdateMenuItems(bool initialsetup)
{
    Screen_Base::UpdateMenuItems( initialsetup );
}

void GameType_EmptyReplaceMe::Tick(double TimePassed)
{
    //LOGInfo( LOGTag, "GameType_EmptyReplaceMe::Tick()\n" );

    Screen_Base::Tick( TimePassed );

    m_Camera.SetPos( 0.0f, 0.0f );
    m_Camera.SetZoom( 1.0f );
    m_Camera.ResetMatrix();

    if( 0 )
    {
        int mode = Leaderboard_Main;

        g_pGameServiceManager->AddScoreToList( g_pGame->m_pProfileManager->m_CurrentProfile->m_Username,
                    g_pGame->m_pProfileManager->m_CurrentProfile->m_Password,
                    LeaderboardGameID,
                    mode,
                    0, // score
                    LeaderboardBoardStringIDs[mode],
                    false, // overwrite = false
                    GameServiceProtocol_All );
    }
}

void GameType_EmptyReplaceMe::Draw()
{
    Screen_Base::Draw();

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    float scrw = g_pGame->m_GameWidth;
    float scrh = g_pGame->m_GameHeight;
    float fontheight = 30;
    float lineheight = fontheight / g_pGame->m_GameHeight;
    unsigned char justify = Justify_CenterX|Justify_CenterY;

    //RenderTextQuick( pFont, 120, 0.5f*scrw, 0.8f*scrh, scrw, scrh, justify, "AToW" );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 16, 16, 0, 1, 0, 1, Justify_Center );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( scrw/2, scrh/2, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(255,0,0,255) );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &m_Camera.m_matViewProj );

    // draw the menu items.
    int i=0;
    for( i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( GetMenuItem(i) )
        {
            GetMenuItem(i)->Draw();
        }
    }
}

bool GameType_EmptyReplaceMe::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    if( BasicMenuTouchFunc( action, id, x, y, pressure, size ) )
        return true;

    return false;
}

bool GameType_EmptyReplaceMe::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    if( id == GCBI_Back && action == GCBA_Down )
    {
        // destroy this screen and go back to main menu.
        OnMenuAction( IGBA_Quit );
        return true;
    }

    return false;
}

bool GameType_EmptyReplaceMe::OnMenuAction(int action)
{
    if( action == IGBA_Quit )
    {
        g_pGame->m_GameTypeToSwitchToNextTick = GAMETYPE_Selector;
        return true;
    }

    return false;
}
