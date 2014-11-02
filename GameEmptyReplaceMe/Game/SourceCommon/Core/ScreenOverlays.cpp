//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "../GameTypes/GameType_Selector.h"
#include "../GameTypes/GameType_EmptyReplaceMe.h"

#include "../Screens/Screen_Loading.h"
#include "../Screens/Screen_HowToPlay.h"
#include "../Screens/Screen_Pause.h"
#include "../Screens/Screen_Achievements.h"
#include "../Screens/Screen_Stats.h"

ScreenOverlayManager* g_pScreenOverlayManager;

const char* ScreenOverlayNames[ScreenOverlay_NumScreens] =
{
    "DontChange",               //ScreenOverlay_DontChange,
    "Destroy",                  //ScreenOverlay_Destroy,
    "RemoveDontDestroy",        //ScreenOverlay_RemoveDontDestroy,

    "GameType_Selector",        //ScreenOverlay_GameType_Selector,
    "GameType_EmptyReplaceMe",  //ScreenOverlay_GameType_EmptyReplaceMe,

    "Loading",                  //ScreenOverlay_Loading,
    "HowToPlay_Online",         //ScreenOverlay_HowToPlay_Online,
    "HowToPlay_Infinite",       //ScreenOverlay_HowToPlay_Infinite,
    "HowToPlay_Puzzle",         //ScreenOverlay_HowToPlay_Puzzle,
    "Options",                  //ScreenOverlay_Options,
    "Achievements",             //ScreenOverlay_Achievements,
    "Stats",                    //ScreenOverlay_Stats,
    "Login",                    //ScreenOverlay_Login,
};

ScreenOverlayManager::ScreenOverlayManager()
{
    m_NeedsTick = false;

    for( int i=0; i<ScreenOverlay_NumScreens; i++ )
    {
        m_pScreens[i] = 0;
    }

    m_pLayoutsFile = 0;

    for( int screen=0; screen<ScreenOverlay_NumScreens; screen++ )
    {
        for( int layout=0; layout<LayoutType_NumLayouts; layout++ )
        {
            m_LayoutJSONStrings[screen][layout] = 0;
        }
    }
}

ScreenOverlayManager::~ScreenOverlayManager()
{
    for( int i=0; i<ScreenOverlay_NumScreens; i++ )
    {
        SAFE_DELETE( m_pScreens[i] );
    }

    g_pFileManager->FreeFile( m_pLayoutsFile );

    for( int screen=0; screen<ScreenOverlay_NumScreens; screen++ )
    {
        for( int layout=0; layout<LayoutType_NumLayouts; layout++ )
        {
            SAFE_DELETE_ARRAY( m_LayoutJSONStrings[screen][layout] );
        }
    }
}

void ScreenOverlayManager::Tick()
{
    m_NeedsTick = false;

    if( m_pLayoutsFile )
    {
        if( m_pLayoutsFile->m_FileReady == false )
        {
            m_NeedsTick = true;
        }
        else
        {
            cJSON* layouts = cJSON_Parse( m_pLayoutsFile->m_pBuffer );
            assert( layouts );

            if( layouts )
            {
                for( int i=0; i<ScreenOverlay_NumScreens; i++ )
                {
                    cJSON* screen = cJSON_GetObjectItem( layouts, ScreenOverlayNames[i] );
                    if( screen )
                    {
                        cJSON* obj = cJSON_GetObjectItem( screen, "Wide" );
                        if( obj )
                        {
                            int len = strlen( obj->valuestring );
                            m_LayoutJSONStrings[i][LayoutType_Wide] = MyNew char[len+1];
                            strcpy_s( m_LayoutJSONStrings[i][LayoutType_Wide], len+1, obj->valuestring );
                        }

                        obj = cJSON_GetObjectItem( screen, "Tall" );
                        if( obj )
                        {
                            int len = strlen( obj->valuestring );
                            m_LayoutJSONStrings[i][LayoutType_Tall] = MyNew char[len+1];
                            strcpy_s( m_LayoutJSONStrings[i][LayoutType_Tall], len+1, obj->valuestring );
                        }

                        obj = cJSON_GetObjectItem( screen, "Square" );
                        if( obj )
                        {
                            int len = strlen( obj->valuestring );
                            m_LayoutJSONStrings[i][LayoutType_Square] = MyNew char[len+1];
                            strcpy_s( m_LayoutJSONStrings[i][LayoutType_Square], len+1, obj->valuestring );
                        }
                    }
                }

                cJSON_Delete( layouts );
            }
        }
    }
}

void ScreenOverlayManager::LoadLayouts()
{
    m_pLayoutsFile = RequestFile( "Data/Layouts/allscreens.layout" );

    m_NeedsTick = true;
}

void ScreenOverlayManager::SaveLayouts()
{
#if MYFW_WINDOWS && MYFW_USING_WX
    cJSON* layouts = cJSON_CreateObject();

    for( int i=0; i<ScreenOverlay_NumScreens; i++ )
    {
        if( m_LayoutJSONStrings[i][LayoutType_Wide] != 0 ||
            m_LayoutJSONStrings[i][LayoutType_Tall] != 0 ||
            m_LayoutJSONStrings[i][LayoutType_Square] != 0 )
        {
            cJSON* screenlayout = cJSON_CreateObject();
            cJSON_AddItemToObject( layouts, ScreenOverlayNames[i], screenlayout );

            if( m_LayoutJSONStrings[i][LayoutType_Wide] )
                cJSON_AddStringToObject( screenlayout, "Wide", m_LayoutJSONStrings[i][LayoutType_Wide] );
            if( m_LayoutJSONStrings[i][LayoutType_Tall] )
                cJSON_AddStringToObject( screenlayout, "Tall", m_LayoutJSONStrings[i][LayoutType_Tall] );
            if( m_LayoutJSONStrings[i][LayoutType_Square] )
                cJSON_AddStringToObject( screenlayout, "Square", m_LayoutJSONStrings[i][LayoutType_Square] );
        }
    }

    char* layoutstring = cJSON_Print( layouts );
    cJSON_Delete( layouts );

    char filename[MAX_PATH];
    sprintf_s( filename, MAX_PATH, "Data/Layouts/allscreens.layout" );
    FILE* file;
    fopen_s( &file, filename, "wb" );
    fprintf( file, layoutstring );
    fclose( file );

    free( layoutstring );
#endif
}

Screen_Base* ScreenOverlayManager::CreateScreen(ScreenOverlays screentomake, Screen_Base* pParentScreen, void* pPtr1, void* pPtr2)
{
    Screen_Base* newscreen = 0;

    switch( screentomake )
    {
    case ScreenOverlay_DontChange:
    case ScreenOverlay_Destroy:
    case ScreenOverlay_RemoveDontDestroy:
        assert( false );
        break;

    case ScreenOverlay_GameType_Selector:
        newscreen = MyNew GameType_Selector();
        break;

    case ScreenOverlay_GameType_EmptyReplaceMe:
        newscreen = MyNew GameType_EmptyReplaceMe();
        break;

    case ScreenOverlay_Loading:
        newscreen = MyNew Screen_Loading();
        break;

    case ScreenOverlay_HowToPlay_Online:
        newscreen = MyNew Screen_HowToPlay();
        ((Screen_HowToPlay*)newscreen)->m_PageShowing = HTPPT_Online;
        break;

    case ScreenOverlay_HowToPlay_Infinite:
        newscreen = MyNew Screen_HowToPlay();
        ((Screen_HowToPlay*)newscreen)->m_PageShowing = HTPPT_Infinite;
        break;

    case ScreenOverlay_HowToPlay_Puzzle:
        newscreen = MyNew Screen_HowToPlay();
        ((Screen_HowToPlay*)newscreen)->m_PageShowing = HTPPT_Puzzle;
        break;

    case ScreenOverlay_Options:
        newscreen = MyNew Screen_Pause();
        break;

    case ScreenOverlay_Achievements:
        newscreen = MyNew Screen_Achievements();
        break;

    case ScreenOverlay_Stats:
        newscreen = MyNew Screen_Stats();
        break;
    }

    if( newscreen )
        newscreen->SetParentScreen( pParentScreen );

    return newscreen;
};
