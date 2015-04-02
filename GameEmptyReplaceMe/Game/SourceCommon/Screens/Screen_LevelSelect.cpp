//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_LevelSelect.h"
#include "../Core/ProfileManager.h"

Screen_LevelSelect::Screen_LevelSelect()
{
    LOGInfo( LOGTag, "[Flow] - Showing Screen_LevelSelect\n" );

    m_Page = 0;
    m_Width = 3;
    m_Height = 3;

    m_PuzzleButtonFingerWentDownOn = -1;
    m_InitialFinger = -1;
    m_SwitchingPages = false;

    m_ScrollAmount = 0;
    m_Momentum = 0;
    m_PageDirectionToSnapTo = 0;

    m_ArrivedFromPause = false;
}

Screen_LevelSelect::~Screen_LevelSelect()
{
}

void Screen_LevelSelect::Init()
{
    Screen_Base::Init();

    int NumPerPage = m_Width * m_Height;
    int PageOffset = m_Page * NumPerPage;
    int NumPages = 6; //g_pGravityBallsGame->m_pWordPuzzles->m_NumPuzzles / NumPerPage;

    m_Page = 0; //g_pGravityBallsGame->m_CurrentPuzzleIndex / NumPerPage;
}

void Screen_LevelSelect::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    int scrw = (int)g_pGame->m_GameWidth;

    int NumPerPage = m_Width * m_Height;
    int NumPages = 6; //g_pGravityBallsGame->m_pWordPuzzles->m_NumPuzzles / NumPerPage;

    if( m_Page == 0 && m_ScrollAmount > 0 )
    {
        m_ScrollAmount = 0;
        m_Momentum = 0;
        m_PageDirectionToSnapTo = 0;
        m_SwitchingPages = false;
    }
    
    if( m_Page == NumPages && m_ScrollAmount < 0 )
    {
        m_ScrollAmount = 0;
        m_Momentum = 0;
        m_PageDirectionToSnapTo = 0;
        m_SwitchingPages = false;
    }

    while( m_ScrollAmount <= -scrw )
    {
        m_ScrollAmount += scrw;
        m_Page++;
    }
    while( m_ScrollAmount >= scrw )
    {
        m_ScrollAmount -= scrw;
        m_Page--;
    }

    if( m_SwitchingPages && m_InitialFinger == -1 )
    {
        float prevscrollamount = m_ScrollAmount;
        m_ScrollAmount += m_PageDirectionToSnapTo * scrw*3 * (float)TimePassed;

        if( (prevscrollamount > 0 && m_ScrollAmount < 0) || (prevscrollamount < 0 && m_ScrollAmount > 0) )
        {
            m_ScrollAmount = 0;
            m_SwitchingPages = false;
        }

        if( fabs(m_ScrollAmount) >= scrw )
        {
            m_Page += m_PageDirectionToSnapTo*-1;
            m_ScrollAmount = 0;
            m_SwitchingPages = false;
        }
    }

    MyClamp( m_Page, 0, NumPages );
}

void Screen_LevelSelect::Draw()
{
    Screen_Base::Draw();

    int NumPerPage = m_Width * m_Height;
    int NumPages = 6; //g_pGravityBallsGame->m_pWordPuzzles->m_NumPuzzles / NumPerPage;

    if( m_Page == 0 && m_ScrollAmount > 0 )
    {
        m_ScrollAmount = 0;
        m_Momentum = 0;
        m_PageDirectionToSnapTo = 0;
    }
    
    if( m_Page == NumPages && m_ScrollAmount < 0 )
    {
        m_ScrollAmount = 0;
        m_Momentum = 0;
        m_PageDirectionToSnapTo = 0;
    }

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
    RenderTextQuick( pFont, fontheight*4, 0.5f*scrw, 0.8f*scrh, justify, "Level select" );

    // draw the back button
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->SetRST( Vector3(0,0,0), Vector3(64,64,0), Vector3(g_pGame->m_GameWidth-42,42,0) );
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->Draw( &g_pGame->m_OrthoMatrixGameSize );

    RenderTextQuick( pFont, fontheight, 0.5f*scrw, 0.096f*scrh, justify, "Page %d of %d", m_Page+1, NumPages+1 );

    int blockwidth = scrw / m_Width - 25; //135;
    int blockheight = scrh / m_Height - 130; //140;

    for( int p=0; p<2; p++ )
    {
        int pagetoshow = m_Page;
        int pagescrollamount = (int)m_ScrollAmount;

        if( p == 1 )
        {
            if( m_ScrollAmount < 0 )
            {
                pagescrollamount += scrw;
                pagetoshow++;
            }
            if( m_ScrollAmount > 0 )
            {
                pagescrollamount -= scrw;
                pagetoshow--;
            }
        }

        int PageOffset = pagetoshow * NumPerPage;
        int pageoffsetx = (scrw - blockwidth*m_Width) / 2 + pagescrollamount;

        for( int i=0; i<NumPerPage; i++ )
        {
            int ix = i%m_Width;
            int iy = i/m_Width;

            int px = (pageoffsetx + ix*blockwidth);
            int py = (670 - iy*blockheight);

            int puzzleindex = PageOffset + i;
            //if( puzzleindex >= g_pGravityBallsGame->m_pWordPuzzles->m_NumPuzzles )
            //    break;

            //g_pGravityBallsGame->m_pResources->m_pSpriteStar_Full->Create( 32.0f/g_pGame->m_GameWidth, 32.0f/g_pGame->m_GameHeight, 0, 0.5f, 0, 1, Justify_CenterX|Justify_CenterY );

            //int score = g_pGravityBallsGame->m_pWordPuzzles->m_pPuzzles[puzzleindex].bestscore;
            //int stars = g_pGravityBallsGame->m_pWordPuzzles->GetNumStarsEarned( puzzleindex, score );
            //char* levelname = g_pGravityBallsGame->m_pWordPuzzles->m_pPuzzles[puzzleindex].m_Name;

            //g_pGravityBallsGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,0,150,255) );
            //g_pGravityBallsGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( (blockwidth - 20)/g_pGame->m_GameWidth, (blockheight - 20)/g_pGame->m_GameHeight, 0, 1, 0, 1, Justify_Left|Justify_Top );
            //g_pGravityBallsGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( (px+10)/g_pGame->m_GameWidth, (py-10)/g_pGame->m_GameHeight, 0 );
            //g_pGravityBallsGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

            //RenderTextQuick( pFont, fontheight, (px+blockwidth/2), (py-40), justify, "Puzzle %d", puzzleindex+1 );
            //RenderTextQuick( pFont, fontheight, (px+blockwidth/2), (py-90), justify, levelname );
            ////RenderTextQuick( pFont, fontheight, (px+blockwidth/2), (py-70), justify, "%d", puzzleindex+1 );

            //float stary = (py-140)/g_pGame->m_GameHeight;

            //if( g_pGravityBallsGame->m_pWordPuzzles->m_pPuzzles[puzzleindex].bestscore > 0 )
            //{
            //    int s;
            //    for( s=0; s<stars; s++ )
            //    {
            //        g_pGravityBallsGame->m_pResources->m_pSpriteStar_Full->SetPosition( ((px+blockwidth/2)-32+s*32)/g_pGame->m_GameWidth, stary, 0 );
            //        g_pGravityBallsGame->m_pResources->m_pSpriteStar_Full->Draw( &g_pGame->m_OrthoMatrix );
            //    }
            //    for( ; s<3; s++ )
            //    {
            //        g_pGravityBallsGame->m_pResources->m_pSpriteStar_Empty->SetPosition( ((px+blockwidth/2)-32+s*32)/g_pGame->m_GameWidth, stary, 0 );
            //        g_pGravityBallsGame->m_pResources->m_pSpriteStar_Empty->Draw( &g_pGame->m_OrthoMatrix );
            //    }
            //}

            //if( puzzleindex >= 1 && g_pGame->m_pProfileManager->m_CurrentProfile->m_PuzzleMode_Scores[puzzleindex-1] == 0 )
            //{
            //    RenderTextQuick( pFont, fontheight, (px+blockwidth/2), stary*scrh, justify, "Locked" );
            //}
        }
    }
}

bool Screen_LevelSelect::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    if( BasicMenuTouchFunc( action, id, x, y, pressure, size ) )
        return true;

    return false;
}

bool Screen_LevelSelect::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    if( id == GCBI_Back && action == GCBA_Down )
    {
        //if( m_ArrivedFromPause )
        //{
        //    g_pGravityBallsGame->SetScreenOverlayToShow( GBScreen_Pause );
        //    return true;
        //}
        //else
        {
            g_pGame->m_GameTypeToSwitchToNextTick = GAMETYPE_Selector;
            return true;
        }
    }

    return false;
}
