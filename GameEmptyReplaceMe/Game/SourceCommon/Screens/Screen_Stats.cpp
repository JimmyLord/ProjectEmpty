//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_Stats.h"
#include "../Core/ProfileManager.h"

int StatsToShowForEachFilter[STGT_NumOptions][Num_PlayerStats] =
{
    // SFGT_All
    {
        PS_TimeInGame,
        PS_TimeInMenus,
        PS_TimePaused,
        Num_PlayerStats, //END
    },
    // SFGT_Online
    {
        PS_TimeInGame,
        PS_TimeInMenus,
        PS_TimePaused,
        Num_PlayerStats, //END
    },
    // SFGT_Infinite
    {
        PS_TimeInGame,
        PS_TimeInMenus,
        PS_TimePaused,
        Num_PlayerStats, //END
    },
    // SFGT_Puzzle
    {
        PS_TimeInGame,
        PS_TimeInMenus,
        PS_TimePaused,
        Num_PlayerStats, //END
    },
};

Screen_Stats::Screen_Stats()
{
    LOGInfo( LOGTag, "[Flow] - Showing Screen_Stats\n" );

    m_ScrollAmount = 0;
    m_Momentum = 0;

    m_InitialFinger = -1;

    m_FilterTime = SFT_AllTime;
    m_FilterGameType = SFGT_All;
}

Screen_Stats::~Screen_Stats()
{
}

void Screen_Stats::Init()
{
    Screen_Base::Init();
}

void Screen_Stats::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    if( m_InitialFinger == -1 && m_Momentum != 0 )
    {
        m_Momentum *= 0.99f;
        
        m_ScrollAmount += m_Momentum * (float)TimePassed * 30;
    }

    int numstats = 0;
    for( int i=0; i<Num_PlayerStats; i++ )
    {
        if( StatsToShowForEachFilter[m_FilterGameType][i] == Num_PlayerStats )
            break;

        numstats++;
    }

    //LOGInfo( LOGTag, "m_ScrollAmount: %f\n", m_ScrollAmount );

    MyClamp( m_ScrollAmount, 0.0f, numstats*50 - 200.0f );
}

void Screen_Stats::Draw()
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
    float fontheight = 20;
    unsigned char justify = Justify_CenterX|Justify_CenterY;

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,150,0,255) );

    float x = 0.5f*scrw;
    float y = 0.63f*scrh + m_ScrollAmount;

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.2f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
    RenderTextQuick( pFont, fontheight*4, x, y, justify, "Stats" );
    if( m_FilterTime == SFT_AllTime )
        RenderTextQuick( pFont, fontheight*3, x, y+0.06f*scrh, justify, "All time" );
    else
        RenderTextQuick( pFont, fontheight*3, x, y+0.06f*scrh, justify, "Session" );

    if( m_FilterGameType == SFGT_All )
        RenderTextQuick( pFont, fontheight*3, x, y-0.06f*scrh, justify, "Everything" );
    else if( m_FilterGameType == SFGT_Infinite )
        RenderTextQuick( pFont, fontheight*3, x, y-0.06f*scrh, justify, "Infinite Mode" );
    else if( m_FilterGameType == SFGT_Puzzle )
        RenderTextQuick( pFont, fontheight*3, x, y-0.06f*scrh, justify, "Puzzle Mode" );
    else if( m_FilterGameType == SFGT_Online )
        RenderTextQuick( pFont, fontheight*3, x, y-0.06f*scrh, justify, "Online Mode" );

    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,70,0,255) );
    //pFont = g_pGame->m_pSystemFont;
    //pFont = g_pGame->m_pResources->m_pFontArial48;

    float sep = 50;
    
    fontheight = 16;

    // Draw the stats.
    for( int i=0; i<Num_PlayerStats; i++ )
    {
        if( StatsToShowForEachFilter[m_FilterGameType][i] == Num_PlayerStats )
            break;

        y = 0.48f - ((sep*i) - m_ScrollAmount)/g_pGame->m_GameHeight;

        if( y < -0.1f || y > 1.1f )
            continue;

        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.04f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );
        
        int stattoshow = StatsToShowForEachFilter[m_FilterGameType][i];

        RenderTextQuick( pFont, fontheight*2, 0.02f*scrw, y*scrh, Justify_Left|Justify_CenterY, g_PlayerStatStrings[stattoshow] );

        double value;
        if( m_FilterTime == SFT_AllTime )
            value = g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsAllTime[stattoshow];
        else
            value = g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsDaily[stattoshow];

        if( g_PlayerStatTypes[stattoshow] == StatType_Number )
        {
            RenderTextQuick( pFont, fontheight*2, 0.98f*scrw, y*scrh, Justify_Right|Justify_CenterY, "%0.0f", value );
        }
        else if( g_PlayerStatTypes[stattoshow] == StatType_Time )
        {
            int valueint = (int)value;
            int days = valueint / (60*60*24);
            int hours = (valueint % (60*60*24)) / (60*60);
            int minutes = (valueint % (60*60)) / 60;
            int seconds = (valueint % 60);
            if( days >= 1 )
                RenderTextQuick( pFont, fontheight*2, 0.98f*scrw, y*scrh, Justify_Right|Justify_CenterY, "%d:%02d:%02d:%02d", days, hours, minutes, seconds );
            else if( hours >= 1 )
                RenderTextQuick( pFont, fontheight*2, 0.98f*scrw, y*scrh, Justify_Right|Justify_CenterY, "%d:%02d:%02d", hours, minutes, seconds );
            else if( minutes >= 1 )
                RenderTextQuick( pFont, fontheight*2, 0.98f*scrw, y*scrh, Justify_Right|Justify_CenterY, "%d:%02d", minutes, seconds );
            else
                RenderTextQuick( pFont, fontheight*2, 0.98f*scrw, y*scrh, Justify_Right|Justify_CenterY, "%d", seconds );
        }
    }

    // draw the filter options
    fontheight = 20;

    y = 0.86f;
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(140,0,0,255) );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1, 0.18f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, y, 0 );
    g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

    // Time filter
    {
        float margin = 0.1f;
        float width = (1 - margin*2) / STF_NumOptions;
        float bgwidth = width-0.01f;
        float firstbuttonx = margin + width/2;
        float height = 0.07f;
        y = 0.9f;

        // SFT_AllTime
        if( m_FilterTime == SFT_AllTime )
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,70,255) );
        else
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( bgwidth, height, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( firstbuttonx + width*SFT_AllTime, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        RenderTextQuick( pFont, fontheight*2, (firstbuttonx + width*SFT_AllTime)*scrw, y*scrh, Justify_CenterX|Justify_CenterY, "All Time" );

        // SFT_Daily
        if( m_FilterTime == SFT_Daily )
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,70,255) );
        else
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( bgwidth, height, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( firstbuttonx + width*SFT_Daily, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        RenderTextQuick( pFont, fontheight*2, (firstbuttonx + width*SFT_Daily)*scrw, y*scrh, Justify_CenterX|Justify_CenterY, "Session" );
    }

    // GameType filter
    {
        float margin = 0.05f;
        float width = (1 - margin*2) / STGT_NumOptions;
        float bgwidth = width-0.01f;
        float firstbuttonx = margin + width/2;
        float height = 0.07f;
        y = 0.82f;

        // SFGT_All
        if( m_FilterGameType == SFGT_All )
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,70,255) );
        else
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( bgwidth, height, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( firstbuttonx + width*SFGT_All, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        RenderTextQuick( pFont, fontheight*2, (firstbuttonx + width*SFGT_All)*scrw, y*scrh, Justify_CenterX|Justify_CenterY, "All" );

        // SFGT_Online
        if( m_FilterGameType == SFGT_Online )
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,70,255) );
        else
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( bgwidth, height, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( firstbuttonx + width*SFGT_Online, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        RenderTextQuick( pFont, fontheight*2, (firstbuttonx + width*SFGT_Online)*scrw, y*scrh, Justify_CenterX|Justify_CenterY, "Online" );

        // SFGT_Infinite
        if( m_FilterGameType == SFGT_Infinite )
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,70,255) );
        else
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( bgwidth, 0.07f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( firstbuttonx + width*SFGT_Infinite, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        RenderTextQuick( pFont, fontheight*2, (firstbuttonx + width*SFGT_Infinite)*scrw, y*scrh, Justify_CenterX|Justify_CenterY, "Infinite" );

        // SFGT_Puzzle
        if( m_FilterGameType == SFGT_Puzzle )
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,70,255) );
        else
            g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(70,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( bgwidth, 0.07f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( firstbuttonx + width*SFGT_Puzzle, y, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        RenderTextQuick( pFont, fontheight*2, (firstbuttonx + width*SFGT_Puzzle)*scrw, y*scrh, Justify_CenterX|Justify_CenterY, "Puzzle" );
    }

    // draw the back button
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->SetRST( Vector3(0,0,0), Vector3(64,64,0), Vector3(g_pGame->m_GameWidth-42,42,0) );
    g_pGame->m_pResources->m_pSprites[SL_Button_Back]->Draw( &g_pGame->m_OrthoMatrixGameSize );

#if _DEBUG
    // draw the clear button
    g_pGame->m_pResources->m_pSprites[SL_Button_Cancel]->SetRST( Vector3(0,0,0), Vector3(64,64,0), Vector3(42,42,0) );
    g_pGame->m_pResources->m_pSprites[SL_Button_Cancel]->Draw( &g_pGame->m_OrthoMatrixGameSize );
#endif
}

bool Screen_Stats::OnTouch(int action, int id, float x, float y, float pressure, float size)
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
            
        m_InitialFinger = id;
        m_Momentum = 0;

        //LOGInfo( LOGTag, "Finger Down %f %f", x, y );
        if( y < 84 && x > g_pGame->m_GameWidth - 84 ) // back
        {
            SetScreenOverlayToShow( ScreenOverlay_Destroy );

            m_InitialFinger = -1;
        }
#if _DEBUG
        else if( y < 84 && x < 84 ) // back
        {
            g_pGame->m_pProfileManager->ClearStats();
        }
#endif
        else
        {
            int scrw = (int)g_pGame->m_GameWidth;
            int scrh = (int)g_pGame->m_GameHeight;

            {
                float margin = 0.1f;
                float width = (1 - margin*2) / STF_NumOptions;
                //float bgwidth = width-0.01f;
                float firstbuttonx = margin + width/2;
                float height = 0.07f;
                float buttony = 0.9f;

                for( int i=0; i<STF_NumOptions; i++ )
                {
                    if( y/scrh > buttony - height/2 && y/scrh < buttony + height/2 &&
                        x/scrw > firstbuttonx+width*i - width/2 && x/scrw < firstbuttonx+width*i + width/2 )
                    {
                        m_FilterTime = (StatsFilterTime)i;
                        m_InitialFinger = -1;
                    }
                }
            }

            {
                float margin = 0.05f;
                float width = (1 - margin*2) / STGT_NumOptions;
                //float bgwidth = width-0.01f;
                float firstbuttonx = margin + width/2;
                float height = 0.07f;
                float buttony = 0.82f;

                for( int i=0; i<STGT_NumOptions; i++ )
                {
                    if( y/scrh > buttony - height/2 && y/scrh < buttony + height/2 &&
                        x/scrw > firstbuttonx+width*i - width/2 && x/scrw < firstbuttonx+width*i + width/2 )
                    {
                        m_FilterGameType = (StatsFilterGameType)i;
                        m_InitialFinger = -1;
                    }
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

bool Screen_Stats::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
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
