//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_HowToPlay.h"
#include "../Core/ProfileManager.h"

Screen_HowToPlay::Screen_HowToPlay()
{
    m_MenuItemsNeeded = HTPPT_NumPages; // + 1;

    m_PageShowing = HTPPT_Online;
}

Screen_HowToPlay::~Screen_HowToPlay()
{
}

void Screen_HowToPlay::Init()
{
    Screen_Base::Init();

    for( int i=0; i<m_MenuItemsNeeded; i++ )
        CreateMenuButton( i );

    float fontheight = 30;

    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;

    for( int i=0; i<m_MenuItemsNeeded; i++ )
    {
        MenuButton* pButton = GetMenuButton(i);

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

        //pButton->m_pFont = g_pGame->m_pSystemFont;
        //pButton->m_Style = MBTS_SingleLine;
        pButton->m_FontHeight = fontheight;
        pButton->SetTextShadow( 3.0f, -3.0f );

        float yspace = fontheight * 2 * 2;
        pButton->SetPositionAndSize( 320.0f, 576.0f - i*yspace, g_pGame->m_GameWidth, 77 );
        pButton->m_InputWidth = g_pGame->m_GameWidth;
        pButton->m_InputHeight = 120;

        pButton->SetBGShadow( 0.0f, 6.0f );

        pButton->m_TextColor = GameMenuButtonColors[MBCT_SelectableText];
        pButton->m_BGColor = GameMenuButtonColors[MBCT_SelectableBG];

        pButton->m_DisabledBGColor = GameMenuButtonColors[MBCT_DisabledBG];
        pButton->m_DisabledTextColor = GameMenuButtonColors[MBCT_DisabledBG];
    }

    // set up the filter buttons
    {
        int optionsinarow = 3;

        float margin = 32;
        float width = (640.0f - margin*2) / optionsinarow;
        float bgwidth = width - 6.4f;
        float firstbuttonx = margin + width/2;
        float height = 67.0f;
        float y = 864.0f;
        float ysep = 77.0f;

        for( int i=0; i<HTPPT_NumPages; i++ )
        {
            MenuButton* pButton = GetMenuButton(i);
            pButton->m_FontHeight = 30;

            pButton->m_TextColor = ColorByte(255,255,255,255);

            pButton->SetPositionAndSize( firstbuttonx + width*(i%optionsinarow), y - ysep*(i/optionsinarow), bgwidth, height );
            pButton->m_InputWidth = width;
            pButton->m_InputHeight = height;
        }

        // special case to center the 4th and 5th buttons:
        GetMenuButton(3)->m_Position.x = firstbuttonx + width - width/2;
        GetMenuButton(4)->m_Position.x = firstbuttonx + width + width/2;

        sprintf_s( GetMenuButton(0)->m_Strings[0], MenuButton::MAX_STRING_LENGTH, "Online" );
        sprintf_s( GetMenuButton(1)->m_Strings[0], MenuButton::MAX_STRING_LENGTH, "Infinite" );
        sprintf_s( GetMenuButton(2)->m_Strings[0], MenuButton::MAX_STRING_LENGTH, "Puzzle" );
        sprintf_s( GetMenuButton(3)->m_Strings[0], MenuButton::MAX_STRING_LENGTH, "Login" );
        sprintf_s( GetMenuButton(4)->m_Strings[0], MenuButton::MAX_STRING_LENGTH, "Scoring" );

        GetMenuButton(0)->m_ButtonAction[0] = (int)HTPA_SwitchPage_Online;
        GetMenuButton(1)->m_ButtonAction[0] = (int)HTPA_SwitchPage_Infinite;
        GetMenuButton(2)->m_ButtonAction[0] = (int)HTPA_SwitchPage_Puzzle;
        GetMenuButton(3)->m_ButtonAction[0] = (int)HTPA_SwitchPage_Login;
        GetMenuButton(4)->m_ButtonAction[0] = (int)HTPA_SwitchPage_Scoring;
    }

    //// back is last button.
    //int backbuttonid = 4;

    //m_pMenuItems[backbuttonid]->m_PosY = 0.45f-(fontheight*2*(0))/g_pGame->m_GameHeight;
    //sprintf_s( m_pMenuItems[backbuttonid]->m_Strings[0], MAX_MENUBUTTON_STRING, "Back" );
    //m_pMenuItems[backbuttonid]->m_ButtonAction = (int)HTPA_Back;
}

void Screen_HowToPlay::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    if( g_pGame->m_pResources )
    {
        for( int i=0; i<m_MenuItemsNeeded; i++ )
            GetMenuButton(i)->SetFont( g_pGame->m_pSystemFont );
    }
}

void Screen_HowToPlay::Draw()
{
    Screen_Base::Draw();

    // half black
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 1, 0, 1, 0, 1, Justify_Left|Justify_Top );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0, 1, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,0,0,240) );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 30;
    //float lineincrement = fontheight/scrh;
    unsigned char justify = Justify_CenterX|Justify_CenterY;
    float y = 0;

    // title
    {
        y = 0.63f;
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,150,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.2f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
        RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, (y+0.03f)*scrh, justify, "Instructions" );
    
        fontheight = 20;
        if( m_PageShowing == HTPPT_Online )
            RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, (y-0.06f)*scrh, justify, "Online Mode" );
        else if( m_PageShowing == HTPPT_Infinite )
            RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, (y-0.06f)*scrh, justify, "Infinite Mode" );
        else if( m_PageShowing == HTPPT_Puzzle )
            RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, (y-0.06f)*scrh, justify, "Puzzle Mode" );
        else if( m_PageShowing == HTPPT_Login )
            RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, (y-0.06f)*scrh, justify, "Online Login" );
        else if( m_PageShowing == HTPPT_Scoring )
            RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, (y-0.06f)*scrh, justify, "Scoring" );
    }

    // draw the filter options
    {
        // bg quad for the filter buttons.
        y = 0.86f;
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(140,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.18f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        // set the bg color of the "filter" buttons.  this assumes the buttons are in slots 0 to HTPPT_NumPages;
        for( int i=0; i<HTPPT_NumPages; i++ )
        {
            if( m_PageShowing == i )
                GetMenuButton(i)->m_BGColor = ColorByte(70,0,70,255);
            else
                GetMenuButton(i)->m_BGColor = ColorByte(70,0,0,255);
        }
    }

    // draw the help screens.
    if( m_PageShowing == HTPPT_Online )
        DrawHelp_Online();
    else if( m_PageShowing == HTPPT_Infinite )
        DrawHelp_Infinite();
    else if( m_PageShowing == HTPPT_Puzzle )
        DrawHelp_Puzzle();
    else if( m_PageShowing == HTPPT_Login )
        DrawHelp_Login();
    else if( m_PageShowing == HTPPT_Scoring )
        DrawHelp_Scoring();

    // draw the buttons.
    int i=0;
    for( i=0; i<m_MenuItemsNeeded; i++ )
    {
        if( GetMenuItem(i) )
        {
            GetMenuItem(i)->Draw( &g_pGame->m_OrthoMatrixGameSize );
        }
    }

    // draw the back button
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->SetRST( Vector3(0,0,0), Vector3(64,64,0), Vector3(g_pGame->m_GameWidth-42,42,0) );
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->Draw( &g_pGame->m_OrthoMatrixGameSize );
}

void Screen_HowToPlay::DrawHelp_Online()
{
    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 35;
    unsigned char justify = Justify_Left|Justify_CenterY;

    float lineinc = fontheight;
    
    float x = 0.05f*scrw;
    float y = 0.48f*scrh;

    int lines = 0;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Each Online round lasts 5 minutes." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Try to find the highest score" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "with the 36 letters available." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Reset the board as often as you" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "want to make the best score possible." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "See the \"Scoring\" page for details" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "about how to maximize points." ); lines++;
}

void Screen_HowToPlay::DrawHelp_Infinite()
{
    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 35;
    unsigned char justify = Justify_Left|Justify_CenterY;

    float lineinc = fontheight;
    
    float x = 0.05f*scrw;
    float y = 0.48f*scrh;

    int lines = 0;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "In Infinite mode, letters will keep" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "falling from the top of the board." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Score points by making words." ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Short words will cause more balls" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "to fall, so try to make long ones." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Long words will cause silver" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "and gold bonus letters to appear." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "The game ends when the board fills up." ); lines++;
}

void Screen_HowToPlay::DrawHelp_Puzzle()
{
    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 35;
    unsigned char justify = Justify_Left|Justify_CenterY;

    float lineinc = fontheight;
    
    float x = 0.05f*scrw;
    float y = 0.48f*scrh;

    int lines = 0;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "The goal of Puzzle mode is to" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "use all the letters on the board." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Earn more stars by earning higher scores." ); lines++;
    //RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "" ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Long words and clumps of letters" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "give the most points." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "There's no penalty for going slow," ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "so take your time." ); lines++;
}

void Screen_HowToPlay::DrawHelp_Login()
{
    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 35;
    unsigned char justify = Justify_Left|Justify_CenterY;

    float lineinc = fontheight;
    
    float x = 0.05f*scrw;
    float y = 0.48f*scrh;

    int lines = 0;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "If you have an account on" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "FlatheadGames.com, just log in." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Otherwise, enter any username" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "and password to create an account." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Usernames and passwords need to" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "be at least 5 characters long." ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "At the moment, we don't take any" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "personal information, not even an e-mail" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "address, so don't lose your password." ); lines++;
}

void Screen_HowToPlay::DrawHelp_Scoring()
{
    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 35;
    unsigned char justify = Justify_Left|Justify_CenterY;

    float lineinc = fontheight;
    
    float x = 0.05f*scrw;
    float y = 0.48f*scrh;

    int lines = 0;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "Score is calculated as follows:" ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight/1.5f, x + 0.325f, y - lines*lineinc+lineinc/4, justify, "2" );
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "= Word Length" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "+ clump bonus for using touching letters" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "+ 10 for each gold letter used" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "+ 5 for each silver letter used" ); lines++;
    lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "And these 2 extras for Infinite Mode" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "+ 2 for each falling letter used" ); lines++;
    RenderTextQuick( pFont, fontheight, x, y - lines*lineinc, justify, "+ time bonus for making words quickly" ); lines++;
}

bool Screen_HowToPlay::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    //if( BasicMenuTouchFunc( action, id, x, y, pressure, size ) )
    //    return true;

    //LOGInfo( LOGTag, "OnTouch (%d %d)(%f,%f)(%f %f)\n", action, id, x, y, pressure, size);

    //if( actionmasked != 2 )
    //    LOGInfo( LOGTag, "OnTouch() id %d, actionmasked %d (%f, %f)\n", id, actionmasked, x, y );

    // flip y to make it start at bottom left.
    y = g_pGame->m_GameHeight - y;

    //static float totaldiffx = 0;

    bool newfingerdown = false;
    bool newfingerup = false;

    if( action == GCBA_Down ) // first finger down
    {
        g_Fingers[id].set( x, y, id );
        newfingerdown = true;

        //LOGInfo( LOGTag, "Finger Down %f %f", x, y );
        //float yperc = y / g_pGame->m_GameHeight;

        // check for the back button.
        if( y < 84 && x > g_pGame->m_GameWidth - 84 )
        {
            SetScreenOverlayToShow( ScreenOverlay_Destroy );
            return true;
        }

        int i=0;
        for( i=0; i<m_MenuItemsNeeded; i++ )
        {
            if( GetMenuItem(i) )
            {
                const char* actionstr = GetMenuItem(i)->TriggerOnCollision( id, x, y, false );
                int action = actionstr[0];

                if( action == HTPA_Back )
                {
                    SetScreenOverlayToShow( ScreenOverlay_Destroy );
                    return true;
                }
                if( action == HTPA_SwitchPage_Online )
                {
                    m_PageShowing = HTPPT_Online;
                    return true;
                }
                if( action == HTPA_SwitchPage_Infinite )
                {
                    m_PageShowing = HTPPT_Infinite;
                    return true;
                }
                if( action == HTPA_SwitchPage_Puzzle )
                {
                    m_PageShowing = HTPPT_Puzzle;
                    return true;
                }
                if( action == HTPA_SwitchPage_Login )
                {
                    m_PageShowing = HTPPT_Login;
                    return true;
                }
                if( action == HTPA_SwitchPage_Scoring )
                {
                    m_PageShowing = HTPPT_Scoring;
                    return true;
                }
            }
        }
    }

    if( action == GCBA_Up ) // last finger up... i.e. no fingers still down.
    {
        for( int i=0; i<10; i++ )
        {
            g_Fingers[i].reset();
        }
        newfingerup = true;

        //LOGInfo( LOGTag, "Finger Up" );
        //totaldiffx = 0;
    }

    if( action == GCBA_Held ) // any finger might have moved
    {
        int fingerindex = -1;

        for( int i=0; i<10; i++ )
        {
            if( g_Fingers[i].id == id )
                fingerindex = i;
        }

        if( fingerindex != -1 )
        {
            g_Fingers[fingerindex].set( x, y, id );

            int diffx = (int)(g_Fingers[fingerindex].currx) - (int)(g_Fingers[fingerindex].lastx);
            int diffy = (int)(g_Fingers[fingerindex].curry) - (int)(g_Fingers[fingerindex].lasty);

            if( diffx != 0 || diffy != 0 )
            {
                //totaldiffx += diffx;
                //LOGInfo( LOGTag, "move diffx = %f\n", totaldiffx );
            }
        }
    }

    if( newfingerdown )
    {   
    }

    if( newfingerup )
    {
    }

    return false;
}

bool Screen_HowToPlay::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
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
