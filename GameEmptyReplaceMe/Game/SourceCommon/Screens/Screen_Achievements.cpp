//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_Achievements.h"
#include "../Core/Achievements.h"
#include "../Core/ProfileManager.h"

Screen_Achievements::Screen_Achievements()
{
    LOGInfo( LOGTag, "[Flow] - Showing Screen_Achievements\n" );

    m_ScrollAmount = 0;
    m_Momentum = 0;

    m_InitialFinger = -1;
}

Screen_Achievements::~Screen_Achievements()
{
}

void Screen_Achievements::Init()
{
    Screen_Base::Init();
}

void Screen_Achievements::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    if( m_InitialFinger == -1 && m_Momentum != 0 )
    {
        m_Momentum *= 0.99f;
        
        m_ScrollAmount += m_Momentum * (float)TimePassed * 30;
    }

    //LOGInfo( LOGTag, "m_ScrollAmount: %f\n", m_ScrollAmount );

    MyClamp( m_ScrollAmount, 0.0f, ACHID_NumAchs*100 - 500.0f );
}

void Screen_Achievements::Draw()
{
    Screen_Base::Draw();

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 1, 0, 1, 0, 1, Justify_Left|Justify_Top );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0, 1, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,0,0,240) );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 20;
    unsigned char justify = Justify_CenterX|Justify_CenterY;

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,150,0,255) );

    float y = 0.8f + m_ScrollAmount/g_pGame->m_GameHeight;

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.2f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
    RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, y*scrh, justify, "ACHIEVEMENTS" );

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,70,0,255) );
    //pFont = g_pGame->m_pSystemFont;
    //pFont = g_pGame->m_pResources->m_pFontArial48;

    float sep = 100;

    for( int i=0; i<ACHID_NumAchs; i++ )
    {
        y = 0.6f - ((sep*i) - m_ScrollAmount)/g_pGame->m_GameHeight;

        if( g_pGame->m_pProfileManager->m_CurrentProfile->m_AchievementsEarned[i] == 1 )
        {
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,70,0,255) );
        }
        else
        {
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        }

        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.08f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
        
        fontheight = 24;
        y = 0.6f - ((sep*i - 15) - m_ScrollAmount)/g_pGame->m_GameHeight;
        RenderTextQuick( pFont, fontheight*2, 0.1f*scrw, y*scrh, Justify_Left|Justify_CenterY, g_Achievements[i].name );

        fontheight = 16;
        y = 0.6f - ((sep*i + 20) - m_ScrollAmount)/g_pGame->m_GameHeight;
        if( g_pGame->m_pProfileManager->m_CurrentProfile->m_AchievementsEarned[i] == 1 )
        {
            RenderTextQuick( pFont, fontheight*2, 0.1f*scrw, y*scrh, Justify_Left|Justify_CenterY, g_Achievements[i].descriptionEarned );
        }
        else
        {
            RenderTextQuick( pFont, fontheight*2, 0.1f*scrw, y*scrh, Justify_Left|Justify_CenterY, g_Achievements[i].descriptionUnearned );
        }
    }

    // draw the back button
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->SetRST( Vector3(0,0,0), Vector3(64,64,0), Vector3(g_pGame->m_GameWidth-42,42,0) );
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->Draw( &g_pGame->m_OrthoMatrixGameSize );
}

bool Screen_Achievements::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    //if( BasicMenuTouchFunc( action, id, x, y, pressure, size ) )
    //    return true;

    // flip y to make it start at bottom left.
    y = g_pGame->m_GameHeight - y;

    bool newfingerdown = false;
    bool newfingerup = false;

    if( action == GCBA_Down ) // first finger down
    {
        g_Fingers[id].set( x, y, id );
        newfingerdown = true;

        m_InitialFinger = id;
        m_Momentum = 0;

        //LOGInfo( LOGTag, "Finger Down %f %f", x, y );
        if( y < 84 && x > g_pGame->m_GameWidth - 84 )
        {
            SetScreenOverlayToShow( ScreenOverlay_Destroy );
        }
        else
        {
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

        m_InitialFinger = -1;
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

            //int diffx = (int)(g_Fingers[fingerindex].currx) - (int)(g_Fingers[fingerindex].lastx);
            int diffy = (int)(g_Fingers[fingerindex].curry) - (int)(g_Fingers[fingerindex].lasty);

            if( diffy != 0 )
            {
                if( id == m_InitialFinger )
                {
                    m_ScrollAmount += diffy;
                    m_Momentum = (float)diffy;

                    //LOGInfo( LOGTag, "move diffx = %d\n", diffx );
                }
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

bool Screen_Achievements::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    if( id == GCBI_Back && action == GCBA_Down )
    {
        SetScreenOverlayToShow( ScreenOverlay_Destroy );
        //return true;
    }

    return false;
}
