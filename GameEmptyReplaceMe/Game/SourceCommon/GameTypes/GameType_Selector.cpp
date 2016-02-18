//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "GameType_Selector.h"
#include "../Core/ProfileManager.h"
#include "../Core/ResourceManager.h"
#include "../../../SharedGameCode/Menus/MenuButton.h"
#include "../../../SharedGameCode/Menus/MenuSprite.h"
#include "../../../SharedGameCode/Menus/MenuText.h"

void SetExitOnBackButton(bool exit)
{
#if MYFW_WINDOWS
    extern bool g_EscapeButtonWillQuit;
    g_EscapeButtonWillQuit = exit;
#endif

#if MYFW_ANDROID
    jclass cls = g_pJavaEnvironment->GetObjectClass( g_pMainActivity );
    LOGInfo( LOGTag, "SetExitOnBackButton - cls %p", cls );

    jmethodID SetExitOnBackButton = g_pJavaEnvironment->GetMethodID( cls, "SetExitOnBackButton", "(Z)V" );

    LOGInfo( LOGTag, "SetExitOnBackButton - SetExitOnBackButton %p", SetExitOnBackButton );

    if( SetExitOnBackButton == 0 )
        return;

    g_pJavaEnvironment->CallVoidMethod( g_pMainActivity, SetExitOnBackButton, exit );
#endif
}

GameType_Selector::GameType_Selector()
{
    sprintf_s( m_ScreenName, MAX_SCREENNAME_LENGTH, "GameType_Selector" );
    m_ScreenOverlayIndex = ScreenOverlay_GameType_Selector;

    m_MenuItemsNeeded = MainMenuItems_Max;
    m_TimeToOpen = 0.5f;
    m_TimeToClose = 1.0f;

    SetExitOnBackButton( true );
}

GameType_Selector::~GameType_Selector()
{
    //SetExitOnBackButton( false );
}

void GameType_Selector::Init()
{
    LOGInfo( LOGTag, "GameType_Selector::Init() buttons = %d\n", m_MenuItemsNeeded );

    float scrw = g_pGame->m_GameWidth;
    float scrh = g_pGame->m_GameHeight;

	float devw = g_pGame->m_DeviceWidth * (g_pGame->m_GameWidth / g_pGame->m_ScreenWidth);
    float devh = g_pGame->m_DeviceHeight * (g_pGame->m_GameHeight / g_pGame->m_ScreenHeight);

    MenuSprite* pMenuSprite;
    MenuText* pMenuText;

    {
        CreateMenuSprite( MainMenuItems_None_BackgroundItem );
        CreateMenuSprite( MainMenuItems_None_MenuTitleBG );
        CreateMenuText( MainMenuItems_None_MenuTitleText );
        CreateMenuSprite( MainMenuItems_None_FlatheadGamesBG );
        CreateMenuText( MainMenuItems_None_FlatheadGamesText );

        pMenuSprite = GetMenuSprite( MainMenuItems_None_BackgroundItem );
        pMenuSprite->SetPositionAndSize( scrw/2, scrh/2, devw, devh );
        //pMenuSprite->m_HasShadow = false;

        pMenuSprite = GetMenuSprite( MainMenuItems_None_MenuTitleBG );
        pMenuSprite->SetPositionAndSize( scrw/2, scrh*0.8f, devw, scrh*0.2f );
        pMenuSprite->m_BGColor = ColorByte(0,150,0,200);

        pMenuText = GetMenuText( MainMenuItems_None_MenuTitleText );
        pMenuText->SetPositionAndSize( scrw/2, scrh*0.8f, scrw, scrh );
        pMenuText->m_FontHeight = 120;
        pMenuText->SetString( "EmptyReplaceMe" );
        pMenuText->SetFont( g_pGame->m_pSystemFont );

        pMenuSprite = GetMenuSprite( MainMenuItems_None_FlatheadGamesBG );
        pMenuSprite->SetPositionAndSize( scrw/2, scrh*0.1f, devw, scrh*0.15f );
        pMenuSprite->m_BGColor = ColorByte(0,0,150,200);

        pMenuText = GetMenuText( MainMenuItems_None_FlatheadGamesText );
        pMenuText->SetPositionAndSize( scrw/2, scrh*0.1f, devw, scrh );
        pMenuText->m_FontHeight = 60;
        pMenuText->SetString( "Flathead Games" );
        pMenuText->SetFont( g_pGame->m_pSystemFont );
    }
    
    pMenuSprite = GetMenuSprite( MainMenuItems_None_BackgroundItem );

    pMenuSprite->m_UseTweenIn = true;
    pMenuSprite->m_TweenIn.AddUnsignedChar( &pMenuSprite->m_BGColor.a, 0, 255, 0.5f, MTT_SineEaseOut, 0*0.1f );

    pMenuSprite->m_UseTweenOut = true;
    pMenuSprite->m_TweenOut.AddUnsignedChar( &pMenuSprite->m_BGColor.a, 255, 0, 0.5f, MTT_Linear, 0*0.1f );

    for( int i=MainMenuItems_None_MenuTitleBG; i<=MainMenuItems_None_MenuTitleText; i++ )
    {
        GetMenuItem(i)->m_UseTweenIn = true;
        GetMenuItem(i)->m_TweenIn.AddFloat( &GetMenuItem(i)->m_Position.y, GetMenuItem(i)->m_Position.y + 1000, GetMenuItem(i)->m_Position.y, 0.5f, MTT_SineEaseOut, i*0.1f );

        GetMenuItem(i)->m_UseTweenOut = true;
        GetMenuItem(i)->m_TweenOut.AddFloat( &GetMenuItem(i)->m_Position.y, GetMenuItem(i)->m_Position.y, GetMenuItem(i)->m_Position.y + 1000, 0.5f, MTT_SineEaseOut, i*0.1f );

        GetMenuItem(i)->m_Position.y += 1000;
    }

    for( int i=MainMenuItems_None_FlatheadGamesBG; i<=MainMenuItems_None_FlatheadGamesText; i++ )
    {
        GetMenuItem(i)->m_UseTweenIn = true;
        GetMenuItem(i)->m_TweenIn.AddFloat( &GetMenuItem(i)->m_Position.y, GetMenuItem(i)->m_Position.y - 1000, GetMenuItem(i)->m_Position.y, 0.5f, MTT_SineEaseOut, i*0.1f );

        GetMenuItem(i)->m_UseTweenOut = true;
        GetMenuItem(i)->m_TweenOut.AddFloat( &GetMenuItem(i)->m_Position.y, GetMenuItem(i)->m_Position.y, GetMenuItem(i)->m_Position.y - 1000, 0.5f, MTT_SineEaseOut, i*0.1f );

        GetMenuItem(i)->m_Position.y -= 1000;
    }

    for( int i=MainMenuItems_FirstButton; i<=MainMenuItems_LastButton; i++ )
    {
        MenuButton* pButton = CreateMenuButton( i );

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

        pButton->SetFont( g_pGame->m_pSystemFont );
        pButton->m_FontHeight = 30;
        pButton->SetTextShadow( 3.0f, -3.0f );

        float xpos = 350;
        //if( i == MainMenuItems_NewDebugGame )
        //    xpos = 650;

        pButton->SetPositionAndSize( xpos, 400 - (i-MainMenuItems_FirstButton)*100.0f, 200, 50 );
        pButton->m_InputHeight = 50;

        pButton->SetBGShadow( 6.0f, -6.0f );

        pButton->m_TextColor = GameMenuButtonColors[MBCT_SelectableText];
        pButton->m_BGColor = GameMenuButtonColors[MBCT_SelectableBG];

        pButton->m_ButtonAction[0] = (char)i;
    }

    for( int i=MainMenuItems_FirstButton; i<=MainMenuItems_Options; i++ )
    {
        MenuButton* pButton = GetMenuButton(i);

        pButton->m_UseTweenIn = true;
        pButton->m_TweenIn.AddFloat( &pButton->m_Position.x, pButton->m_Position.x - 1000, pButton->m_Position.x, 2.5f, MTT_ElasticEaseOut, i*0.1f );
        //pButton->m_TweenIn.AddFloat( &pButton->m_PosY, pButton->m_PosY - 1000, pButton->m_PosY, 0.5f, MTT_SineEaseOut, i*0.1f );

        pButton->m_UseTweenOut = true;
        pButton->m_TweenOut.AddFloat( &pButton->m_Position.x, pButton->m_Position.x, pButton->m_Position.x - 1000, 1.5f, MTT_ElasticEaseIn, (i-MainMenuItems_FirstButton)*0.1f );

        pButton->m_Position.x -= 1000;
    }

    GetMenuButton( MainMenuItems_MainGame )->SetString( "New Game" );
    GetMenuButton( MainMenuItems_Achievements )->SetString( "Achievements" );
    GetMenuButton( MainMenuItems_Stats )->SetString( "Stats" );
    GetMenuButton( MainMenuItems_Options )->SetString( "Options" );

    {
        MenuButton* pButton = GetMenuButton( MainMenuItems_Stats );
        pButton->SetString( "Stats" );
        pButton->m_BGColor = ColorByte(255,120,120,255);
        pButton->SetPositionAndSize( scrw-50, 300, 100, 60 );
        pButton->m_UseTweenIn = true;
        pButton->m_TweenIn.AddFloat( &pButton->m_Position.x, pButton->m_Position.x + 1000, pButton->m_Position.x, 0.5f, MTT_SineEaseOut, 1.0f );
        pButton->m_UseTweenOut = true;
        pButton->m_TweenOut.AddFloat( &pButton->m_Position.x, pButton->m_Position.x, pButton->m_Position.x + 1000, 1.5f, MTT_ElasticEaseIn, 0 );

        pButton->m_Position.x += 1000;
    }

    //GetMenuButton( MainMenuItems_NewDebugGame )->SetVisibleAndEnabled( false );
    //GetMenuButton( MainMenuItems_Achievements )->SetVisibleAndEnabled( false );
    //GetMenuButton( MainMenuItems_Options )->SetVisibleAndEnabled( false );
    //GetMenuButton( MainMenuItems_Stats )->SetVisibleAndEnabled( false );

    Screen_Base::Init();
}

void GameType_Selector::Tick(double TimePassed)
{
    //LOGInfo( LOGTag, "GameType_Selector::Tick()\n" );

    Screen_Base::Tick( TimePassed );

    // should disable exiting on back only because there are some ticks while this screen is closing(and not on top).
    if( g_pScreenManager->GetTopScreen() == this )
        SetExitOnBackButton( true );
    else
        SetExitOnBackButton( false );
}

void GameType_Selector::Draw()
{
    int i=0;
    for( i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( GetMenuItem(i) )
        {
            //if( i== 4 )
            {
                GetMenuItem(i)->Draw( &g_pGame->m_OrthoMatrixGameSize );
                //return;
            }
        }
    }

//    float scrw = g_pGame->m_GameWidth;
//    float scrh = g_pGame->m_GameHeight;
//
//    int justify = Justify_CenterX|Justify_CenterY;
//    FontDefinition* pFont = g_pGame->m_pSystemFont;
//
//    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( scrw * 1.2f, 120, 0, 1, 0, 1, justify );
//    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( scrw/2, 58, 0 );
//    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,0,150,255) );
//    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrixGameSize );
//
//    RenderTextQuick( pFont, 40, scrw/2, 58+20*2, justify, "Pre-pre-pre-pre-pre-pre-alpha version 0.00001" );
//    RenderTextQuick( pFont, 40, scrw/2, 58-20*0, justify, "Visit FlatheadGames.com" );
//    RenderTextQuick( pFont, 40, scrw/2, 58-20*2, justify, "for more info" );

    //MenuSprite* pMenuSprite;
    //pMenuSprite = (MenuSprite*)m_pMenuItems[MainMenuItems_None_BackgroundItem];
    //RenderTextQuick( pFont, 40, scrw/2, 58+20*4, justify, "Alpha - %d", (int)pMenuSprite->m_BGColor.a );

    // since this draw screen overlays, render it last
    //Screen_Base::Draw();
}

bool GameType_Selector::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    if( BasicMenuTouchFunc( action, id, x, y, pressure, size ) )
        return true;

    return false;
}

bool GameType_Selector::OnMenuAction(int action)
{
    if( action == MainMenuItems_MainGame )
    {
        //g_pGame->m_pGameOptions->Reset();

        g_pGame->m_GameTypeToSwitchToNextTick = GAMETYPE_EmptyReplaceMe;
        return true;
    }
    if( action == MainMenuItems_Achievements )
    {
        SetScreenOverlayToShow( ScreenOverlay_Achievements );
        return true;
    }
    if( action == MainMenuItems_Options )
    {
        SetScreenOverlayToShow( ScreenOverlay_Options );
        return true;
    }
    if( action == MainMenuItems_Stats )
    {
        SetScreenOverlayToShow( ScreenOverlay_Stats );
        return true;
    }

    //LaunchURL( "http://www.flatheadgames.com" );

    return false;
}

bool GameType_Selector::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    if( id == GCBI_Back && action == GCBA_Down )
    {
        //TODO: find a way to kill the app.
        return true;
    }

    return false;
}
