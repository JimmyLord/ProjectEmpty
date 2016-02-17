//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Screen_Loading.h"
#include "Screen_LevelSelect.h"

Screen_Loading::Screen_Loading()
{
    m_ScreenID = 1111;
    m_MenuItemsNeeded = 0;
    m_TimeToClose = 0;

    LOGInfo( LOGTag, "[Flow] - Showing Screen_Loading\n" );
}

Screen_Loading::~Screen_Loading()
{
}

void Screen_Loading::Init()
{
    Screen_Base::Init();
}

void Screen_Loading::Tick(double TimePassed)
{
    Screen_Base::Tick( TimePassed );

    if( this->GetState() != SS_Running )
        return;

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    if( pFont && pFont->m_pBMFont && pFont->m_pTextureDef && pFont->m_pTextureDef->m_TextureID )
    {
        //if( h_WaitATickForLoadScreen )
        //{
        //    h_WaitATickForLoadScreen = false;
        //    return;
        //}

        // start loading the biggest chunk of resources when the system font is loaded... allowing us to print "loading".
        if( g_pGame->m_pResources == 0 )
        {
            return;
        }
        else
        {
            g_pGame->m_pResources->Tick( TimePassed );

            if( g_pGame->m_pResources->m_AllResourcesLoaded )
            {
                SetScreenOverlayToShow( ScreenOverlay_Destroy );
                g_pGame->FinishedLoading();
            }
        }
    }
}

void Screen_Loading::Draw()
{
    Screen_Base::Draw();

    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;

    if( g_pGame->m_pResources == 0 || g_pGame->m_pResources == 0 )
        return;

    //if( g_pGame->m_pResources->m_pSpriteTextures[SL_Logo]->m_FullyLoaded == false )
    //    return;

    if( g_pGame->m_pResources && g_pGame->m_pResources )
    {
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Create( 1.125f, 1.134f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetPosition( 0.5f, 0.5f, 0 );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->SetTint( ColorByte(0,0,0,255) );
        g_pGame->m_pResources->m_pSprites[SL_WhiteSquareResizable]->Draw( &g_pGame->m_OrthoMatrix );

        // grid
//#if MYFW_IOS
//        g_pGame->m_pResources->m_pSprites[SL_BG_Menu]->Create( 1, 1, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
//#else
//        g_pGame->m_pResources->m_pSprites[SL_BG_Menu]->Create( 1.125f, 1.134f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
//#endif
//        g_pGame->m_pResources->m_pSprites[SL_BG_Menu]->SetPosition( 0.5f, 0.5f, 0 );
//        g_pGame->m_pResources->m_pSprites[SL_BG_Menu]->SetTint( ColorByte(255,255,255,255) );
//        g_pGame->m_pResources->m_pSprites[SL_BG_Menu]->Draw( &g_pGame->m_OrthoMatrix );
//        
//        g_pGame->m_pResources->m_pSprites[SL_Logo]->Create( 0.9f, (0.9f*scrw/2)/scrh, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
//        g_pGame->m_pResources->m_pSprites[SL_Logo]->SetPosition( 0.5f, 0.82f, 0 );
//        g_pGame->m_pResources->m_pSprites[SL_Logo]->SetTint( ColorByte(255,255,255,255) );
//        g_pGame->m_pResources->m_pSprites[SL_Logo]->Draw( &g_pGame->m_OrthoMatrix );
    }

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    if( g_pGame->m_pSystemFont )
    {
        int scrw = (int)g_pGame->m_GameWidth;
        int scrh = (int)g_pGame->m_GameHeight;
        float fontheight = 60;
        unsigned char justify = Justify_CenterX|Justify_CenterY;

        RenderTextQuickWithColor( pFont, fontheight, (0.5f+0.007f)*scrw, (0.5f-0.007f)*scrh, justify, ColorByte(0,0,0,128), "LOADING" );
        RenderTextQuick( pFont, fontheight, 0.5f*scrw, 0.5f*scrh, justify, "LOADING" );        
    }

    if( g_pGame->m_pResources && g_pGame->m_pResources )
    {
        unsigned char justify = Justify_CenterX|Justify_CenterY;
        
        float fontheight = 60;
        //float lineheight = fontheight / scrh;
        FontDefinition* pFont = g_pGame->m_pSystemFont; //g_pGame->m_pResources->m_pFontFlathead;
        RenderTextQuickWithColor( pFont, fontheight, (0.5f+0.007f)*scrw, (0.08f-0.007f)*scrh, justify, ColorByte(0,0,0,128), "Flathead Games" );
        RenderTextQuick( pFont, fontheight, 0.5f*scrw, 0.08f*scrh, justify, "Flathead Games" );
    }
}

bool Screen_Loading::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( Screen_Base::OnTouch( action, id, x, y, pressure, size ) )
        return true;

    // always return true, don't let any input past this screen.
    return true;
}

bool Screen_Loading::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    if( Screen_Base::OnButtons( action, id ) )
        return true;

    // always return true, don't let any input past this screen.
    return true;
}
