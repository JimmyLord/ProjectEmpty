//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_Pause.h"
#include "Screen_LevelSelect.h"
#include "Screen_Pause.h"

Screen_Pause::Screen_Pause()
{
    m_MenuItemsNeeded = 6;

    LOGInfo( LOGTag, "[Flow] - Showing Screen_Pause\n" );

    //g_pGame->m_GamePaused = true;
}

Screen_Pause::~Screen_Pause()
{
}

void Screen_Pause::Init()
{
    Screen_Base::Init();

    for( int i=0; i<m_MenuItemsNeeded; i++ )
        CreateMenuButton( i );

    float fontheight = 30;

    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        MenuButton* pButton = GetMenuButton( i );

        MySprite* pWhiteSquare = g_pGame->m_pResources->m_pSprites[SL_WhiteSquare];

        MaterialDefinition* pMatGray = g_pMaterialManager->LoadMaterial( "Data/Materials/Gray.mymaterial" );
        MaterialDefinition* pMatDarkGray = g_pMaterialManager->LoadMaterial( "Data/Materials/DarkGray.mymaterial" );
        MaterialDefinition* pMatShadow = g_pMaterialManager->LoadMaterial( "Data/Materials/Shadow.mymaterial" );
        pButton->SetMaterial( MenuButton::Material_BG, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGDisabled, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGOverlay, pMatGray );
        pButton->SetMaterial( MenuButton::Material_BGPressed, pMatDarkGray );
        pButton->SetMaterial( MenuButton::Material_Shadow, pMatShadow );
        pMatGray->Release();
        pMatDarkGray->Release();
        pMatShadow->Release();

        float yspace = fontheight * 2 * 2;

        //pButton->m_pFont = g_pGame->m_pResources->m_pGBResources->m_pFontArial48;
        //pButton->m_Style = MBTS_SingleLine;
        pButton->m_FontHeight = fontheight;
        pButton->SetTextShadow( 3.0f, -3.0f );

        pButton->SetPositionAndSize( 320.0f, 576.0f - i*yspace, g_pGame->m_GameWidth, 77 );
        pButton->m_InputWidth = g_pGame->m_GameWidth;
        pButton->m_InputHeight = yspace;

        pButton->SetBGShadow( 0.0f, 6.0f );

        pButton->m_TextColor = GameMenuButtonColors[MBCT_SelectableText];
        pButton->m_BGColor = GameMenuButtonColors[MBCT_SelectableBG];

        pButton->m_DisabledBGColor = GameMenuButtonColors[MBCT_DisabledBG];
        pButton->m_DisabledTextColor = GameMenuButtonColors[MBCT_DisabledBG];
    }

    int i = 0;

    if( 0 ) //g_pGame->m_GBGameType != GBType_Options )
    {
        GetMenuButton( i )->SetString( "Resume" );
        GetMenuButton( i )->m_ButtonAction[0] = (int)PMA_Resume;
        i++;
    }

    GetMenuButton( i )->SetString( "Achievements" );
    GetMenuButton( i )->m_ButtonAction[0] = (int)PMA_Achievements;
    i++;

    GetMenuButton( i )->SetString( "Stats" );
    GetMenuButton( i )->m_ButtonAction[0] = (int)PMA_Stats;
    i++;

    if( 0 ) //g_pGame->m_GBGameType == GBType_WordPuzzle && g_pGame->m_OnlineMode == false )
    {
        GetMenuButton( i )->SetString( "Level select" );
        GetMenuButton( i )->m_ButtonAction[0] = (int)PMA_LevelSelect;
        i++;
    }

    GetMenuButton( i )->SetString( "How to Play" );
    GetMenuButton( i )->m_ButtonAction[0] = (int)PMA_HowToPlay;
    i++;

    GetMenuButton( i )->SetString( "Back to main menu" );
    GetMenuButton( i )->m_ButtonAction[0] = (int)PMA_MainMenu;
    i++;
}

void Screen_Pause::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    if( g_pGame->m_pResources )
    {
        for( int i=0; i<m_MenuItemsNeeded; i++ )
            GetMenuButton( i )->SetFont( g_pGame->m_pSystemFont );
    }
}

void Screen_Pause::Draw()
{
    Screen_Base::Draw();

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 1, 0, 1, 0, 1, Justify_Left|Justify_Top );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0, 1, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,0,0,240) );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 30;
    unsigned char justify = Justify_CenterX|Justify_CenterY;

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,150,0,255) );

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.2f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, 0.8f, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
    //if( g_pGame->m_GBGameType == GBType_Options )
    //{
    //    RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, 0.8f*scrh, justify, "OPTIONS" );        
    //}
    //else
    {
        RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, 0.8f*scrh, justify, "PAUSE" );        
    }

    int i=0;
    for( i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( GetMenuItem(i) )
        {
            GetMenuItem(i)->Draw( &g_pGame->m_OrthoMatrixGameSize );
        }
    }
}

bool Screen_Pause::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    if( BasicMenuTouchFunc( action, id, x, y, pressure, size ) )
        return true;

    return false;
}

bool Screen_Pause::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    if( id == GCBI_Back && action == GCBA_Down )
    {
        SetScreenOverlayToShow( ScreenOverlay_Destroy );
        return true;
    }

    return false;
}

bool Screen_Pause::OnMenuAction(int action)
{
    if( action == PMA_Resume ) // resume.
    {
        SetScreenOverlayToShow( ScreenOverlay_Destroy );
        return true;
    }
    if( action == PMA_Achievements )
    {
        SetScreenOverlayToShow( ScreenOverlay_Achievements );
        return true;
    }
    if( action == PMA_Stats )
    {
        SetScreenOverlayToShow( ScreenOverlay_Stats );
        return true;
    }
    if( action == PMA_LevelSelect )
    {
        //g_pGame->m_GamePaused = false;
        //SetScreenOverlayToShow( GBScreen_LevelSelect );
        //((Screen_LevelSelect*)g_pGame->m_pNextScreenOverlay)->m_ArrivedFromPause = true;
        return true;
    }
    if( action == PMA_HowToPlay )
    {
        //if( g_pGame->m_OnlineMode == true )
            SetScreenOverlayToShow( ScreenOverlay_HowToPlay_Online );
        //else if( g_pGame->m_GBGameType == GBType_WordPuzzle )
        //    SetScreenOverlayToShow( ScreenOverlay_HowToPlay_Puzzle );
        //else //if( g_pGame->m_GBGameType == GBType_WordPuzzle )
        //    SetScreenOverlayToShow( ScreenOverlay_HowToPlay_Infinite );
        return true;
    }
    if( action == PMA_MainMenu )
    {
        //g_pGame->m_GamePaused = false;
        SetScreenOverlayToShow( ScreenOverlay_Destroy );
        return true;
    }

    return false;
}