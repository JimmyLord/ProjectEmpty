//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "GameEmptyReplaceMe.h"

#include "ResourceManager.h"
#include "../GameTypes/GameType_Selector.h"
#include "../GameTypes/GameType_EmptyReplaceMe.h"
#include "Screens/Screen_Loading.h"
#include "../../../SharedGameCode/Core/Game_GameServiceManager.h"
#include "../../../SharedGameCode/Leaderboards/LeaderboardStorage.h"

#include "ProfileManager.h"

GameEmptyReplaceMe* g_pGameEmptyReplaceMe = 0;
GameEmptyReplaceMe* g_pGame = 0; // duplicate of above for simplicity.

ColorByte GameMenuButtonColors[4] =
{
    ColorByte(150,150,150,255), //255,81,205,255),  // MBCT_SelectableBG
    ColorByte(255,255,255,255),    // MBCT_SelectableText

    ColorByte(100,100,100,255), //0,150,150,255),   // MBCT_DisabledBG
    ColorByte(200,200,200,200), // MBCT_DisabledText
};

GameEmptyReplaceMe::GameEmptyReplaceMe()
{
    g_pGameEmptyReplaceMe = this;
    g_pGame = this;

    m_pScreenOverlay = 0;
    m_ScreenToShow = ScreenOverlay_DontChange;

    m_TimeBeforeWeStartAdView = 0;

    m_pShaderFile_Simple = 0;
    m_pShaderFile_Font = 0;
    m_pShaderFile_Texture = 0;
    m_pShaderFile_TextureVertexColor = 0;
    m_pShaderFile_PointSprite = 0;

    m_pShader_Simple = 0;
    m_pShader_Font = 0;
    m_pShader_Texture = 0;
    m_pShader_TextureVertexColor = 0;
    m_pShader_PointSprite = 0;

    m_pParticleRenderer = 0;

    m_pSystemFont = 0;

    m_GameWidth = 640.0f;
    m_GameHeight = 960.0f;

    m_GameFullWidth = 0;
    m_GameFullHeight = 0;

    InitializeManagers();

    h_WaitATickForLoadScreen = true;
    m_pResources = MyNew ResourceManager;

    m_pGameAudio = MyNew GameAudio;
    m_pCurrentGame = 0;
    m_pGameOptions = MyNew GameOptions;
    
    m_pProfileManager = MyNew ProfileManager();
    m_pSaveGameFile = CreatePlatformSpecificSaveFile();
    m_pCurrentStringBeingSaved = 0;

    m_GameTypeToSwitchToNextTick = GAMETYPE_None;
    m_SaveGameNextTick = false;

    m_pAchievementChunk = MyNew AchievementChunk;
    m_ObjectPool_ScoreChunks.AllocateObjects( 10 );
    for( unsigned int i=0; i<m_ObjectPool_ScoreChunks.Length(); i++ )
    {
        ScoreChunk* pObj = MyNew ScoreChunk;
        m_ObjectPool_ScoreChunks.AddInactiveObject( pObj );
    }

    m_OrthoLeft = 0;
    m_OrthoRight = 0;
    m_OrthoBottom = 0;
    m_OrthoTop = 0;

    m_Rand.seed( rand() );

    //m_pHighScoreStorage = 0;
    m_pLeaderboardStorage = 0;
}

GameEmptyReplaceMe::~GameEmptyReplaceMe()
{
    SAFE_DELETE( m_pScreenOverlay );

    SAFE_RELEASE( m_pShaderFile_Simple );
    SAFE_RELEASE( m_pShaderFile_Font );
    SAFE_RELEASE( m_pShaderFile_Texture );
    SAFE_RELEASE( m_pShaderFile_TextureVertexColor );
    SAFE_RELEASE( m_pShaderFile_PointSprite );

    SAFE_RELEASE( m_pShader_Simple );
    SAFE_RELEASE( m_pShader_Font );
    SAFE_RELEASE( m_pShader_Texture );
    SAFE_RELEASE( m_pShader_TextureVertexColor );
    SAFE_RELEASE( m_pShader_PointSprite );

    SAFE_DELETE( m_pParticleRenderer );

    SAFE_RELEASE( m_pSystemFont );
    SAFE_DELETE( m_pResources );
    
    SAFE_DELETE( m_pGameAudio );

    //SAFE_DELETE( m_pCurrentGame ); // deleted my screenmanager.
    SAFE_DELETE( m_pGameOptions );

    SAFE_DELETE( m_pProfileManager );
    SAFE_DELETE( m_pSaveGameFile );
    //TODO: prevent shutdown while saving.
    MyAssert( m_pCurrentStringBeingSaved == 0 ); // if it's not then there's probably a save in progress.
    SAFE_DELETE( m_pCurrentStringBeingSaved );

    SAFE_DELETE( m_pAchievementChunk );
    m_ObjectPool_ScoreChunks.DeleteAllObjectsInPool();

    //SAFE_DELETE( m_pHighScoreStorage );
    SAFE_DELETE( m_pLeaderboardStorage );

    SAFE_DELETE( g_pScreenManager );
    SAFE_DELETE( g_pScreenOverlayManager );

    SAFE_DELETE( g_pRTQGlobals );

    g_pGameEmptyReplaceMe = 0;
    g_pGame = 0;
}

void GameEmptyReplaceMe::InitializeManagers()
{
    GameCore::InitializeManagers();

    if( g_pRTQGlobals == 0 )
        g_pRTQGlobals = MyNew RenderTextQuickGlobals;
}

void GameEmptyReplaceMe::OneTimeInit()
{
    if( m_OneTimeInitWasCalled )
        return;
    LOGInfo( LOGTag, "GameEmptyReplaceMe::OneTimeInit()\n" );

    GameCore::OneTimeInit();

    if( m_pSaveGameFile )
        m_pSaveGameFile->ReadString( "", "SavedData.dat" );
    //m_pSaveGameFile->WriteString( "", "SavedData.dat", "It's working!!" );

#if MYFW_NACL
    //g_pGame->m_pSoundPlayer->PlaySound( GameAudio_Music, true );
    //m_pGameAudio->Play( GameAudioCue_Music );
#endif

    srand( (unsigned int)(MyTime_GetSystemTime()*10000000) );

    g_pTextureManager->SetMaxTexturesToLoadInOneTick( 1 );

    m_pShaderFile_Simple =             RequestFile( "Data/Shaders/Shader_Simple.glsl" );
    m_pShaderFile_Font =               RequestFile( "Data/Shaders/Shader_Font.glsl" );
    m_pShaderFile_Texture =            RequestFile( "Data/Shaders/Shader_Texture.glsl" );
    m_pShaderFile_TextureVertexColor = RequestFile( "Data/Shaders/Shader_TextureVertexColor.glsl" );
    m_pShaderFile_PointSprite =        RequestFile( "Data/Shaders/Shader_PointSprite.glsl" );

    m_pShader_Simple =             MyNew ShaderGroup( m_pShaderFile_Simple );
    m_pShader_Font =               MyNew ShaderGroup( m_pShaderFile_Font );
    m_pShader_Texture =            MyNew ShaderGroup( m_pShaderFile_Texture );
    m_pShader_TextureVertexColor = MyNew ShaderGroup( m_pShaderFile_TextureVertexColor );
    m_pShader_PointSprite =        MyNew ShaderGroup( m_pShaderFile_PointSprite );

    m_pParticleRenderer = MyNew ParticleRenderer( false );
    if( m_pParticleRenderer )
    {
        m_pParticleRenderer->m_ScalePosToScreenSize = false;
        m_pParticleRenderer->AllocateVertices( 2000, "Particles" );
        m_pParticleRenderer->m_ScreenOffsetX = m_ScreenOffsetX;
        m_pParticleRenderer->m_ScreenOffsetY = m_ScreenOffsetY;
        m_pParticleRenderer->m_GameWidth = m_GameWidth;
        m_pParticleRenderer->m_GameHeight = m_GameHeight;
        m_pParticleRenderer->m_DeviceWidth = m_DeviceWidth;
        m_pParticleRenderer->m_DeviceHeight = m_DeviceHeight;
    }

    m_pResources->LoadAllTextures();
    //m_pGameAudio->LoadAllGameAudio();

    if( m_pSystemFont == 0 )
        m_pSystemFont = g_pFontManager->CreateFont( "Data/Fonts/System24.fnt" );

    //m_pHighScoreStorage = MyNew HighScoreStorage();
    //m_pHighScoreStorage->Init();

    m_pLeaderboardStorage = MyNew LeaderboardStorage();
    m_pLeaderboardStorage->Init();

    g_pScreenManager = MyNew ScreenManager;
    g_pScreenOverlayManager = MyNew ScreenOverlayManager;
    g_pScreenOverlayManager->LoadLayouts();

    Screen_Base* loadingscreen = MyNew Screen_Loading();
    loadingscreen->Init();
    //SetScreenOverlayToShow( ScreenOverlay_Loading );

    g_pScreenManager->PushScreen( loadingscreen );

    g_pGameServiceManager->AllocateEntries( 100 * GameServiceProtocol_NumProtocols );
}

bool GameEmptyReplaceMe::IsReadyToRender()
{
    if( g_pGame->m_pResources == 0 )
        return false;
    
    //if( g_pGame->m_pResources->m_pTextures[SL_Logo]->m_TextureID == 0 )
    //    return false;
    
    return true;
}

void GameEmptyReplaceMe::FinishedLoading()
{
    m_pCurrentGame = MyNew GameType_Selector();

    //Screen_Background* pBG = MyNew Screen_Background();
    //pBG->Init();
    //g_pScreenManager->PushScreen( pBG );
    g_pScreenManager->PushScreen( m_pCurrentGame );

    m_pCurrentGame->Init();
}

void GameEmptyReplaceMe::OnSurfaceCreated()
{
    GameCore::OnSurfaceCreated();
}

void GameEmptyReplaceMe::OnSurfaceLost()
{
    GameCore::OnSurfaceLost();
}

static int h_numframes = 0;
static double h_totaltime = 0;

void GameEmptyReplaceMe::OnSurfaceChanged(unsigned int startx, unsigned int starty, unsigned int width, unsigned int height)
{
    GameCore::OnSurfaceChanged( startx, starty, width, height );

    if( height == 0 || width == 0 )
        return;

    if( width >= height )
    {
        m_GameWidth = 960.0f;
        m_GameHeight = 640.0f;
    }
    else
    {
        m_GameWidth = 640.0f;
        m_GameHeight = 960.0f;
    }

    LOGInfo( LOGTag, "[OnSurfaceChanged]===========================================\n" );

    LOGInfo( LOGTag, "[OnSurfaceChanged] OnSurfaceChanged(%d, %d)\n", width, height );

    m_DeviceWidth = (float)width;
    m_DeviceHeight = (float)height;

    float deviceratio = m_DeviceWidth / m_DeviceHeight;
    float gameratio = m_GameWidth / m_GameHeight;

    LOGInfo( LOGTag, "[OnSurfaceChanged] device ratio(%f)\n", deviceratio );

    if( deviceratio < gameratio )
    {
        m_ScreenWidth = m_DeviceWidth;
        m_ScreenHeight = m_DeviceWidth / gameratio;
    }
    else
    {
        m_ScreenWidth = m_DeviceHeight * gameratio;
        m_ScreenHeight = m_DeviceHeight;
    }

    m_ScreenOffsetX = 0;
    m_ScreenOffsetY = 0;

    if( m_ScreenWidth < m_DeviceWidth )
        m_ScreenOffsetX = (m_DeviceWidth - m_ScreenWidth)/2;
 
    if( m_ScreenHeight < m_DeviceHeight )
        m_ScreenOffsetY = (m_DeviceHeight - m_ScreenHeight)/2;

    if( m_pParticleRenderer )
    {
        m_pParticleRenderer->m_ScreenOffsetX = m_ScreenOffsetX;
        m_pParticleRenderer->m_ScreenOffsetY = m_ScreenOffsetY;
        m_pParticleRenderer->m_GameWidth = m_GameWidth;
        m_pParticleRenderer->m_GameHeight = m_GameHeight;
        m_pParticleRenderer->m_DeviceWidth = m_DeviceWidth;
        m_pParticleRenderer->m_DeviceHeight = m_DeviceHeight;
    }

    m_GameFullWidth = g_pGame->m_DeviceWidth * (g_pGame->m_GameWidth / g_pGame->m_ScreenWidth) + 0.5f;
    m_GameFullHeight = g_pGame->m_DeviceHeight * (g_pGame->m_GameHeight / g_pGame->m_ScreenHeight) + 0.5f;

#if !MYFW_IOS
    LOGInfo( LOGTag, "[OnSurfaceChanged] Window Size(%0.1f, %0.1f)\n", m_DeviceWidth, m_DeviceHeight );
    LOGInfo( LOGTag, "[OnSurfaceChanged] Screen Size(%0.1f, %0.1f)\n", m_ScreenWidth, m_ScreenHeight );
    LOGInfo( LOGTag, "[OnSurfaceChanged] Screen Offset(%0.1f, %0.1f)\n", m_ScreenOffsetX, m_ScreenOffsetY );
#endif

    //m_ProjMatrix.SetFrustum( -deviceratio, deviceratio, -1, 1, 1, 10000 );

    //// Set Camera Position.
    //m_ViewMatrix.SetIdentity();
    //m_CamX = -32;
    //m_CamY = -17;
    //m_CamZ = -25;
    //m_ViewMatrix.TranslatePostRotation( m_CamX, m_CamY, m_CamZ );

    float ortholeft = 0 - m_ScreenOffsetX/m_ScreenWidth;
    float orthoright = 1 + m_ScreenOffsetX/m_ScreenWidth;
    float orthobottom = 0 - m_ScreenOffsetY/m_ScreenHeight;
    float orthotop = 1 + m_ScreenOffsetY/m_ScreenHeight;

    ////const GLdouble vvFovDegs = 45.0;
    //GLdouble vvFovRads = PI/4.0f; //ConvertDegsToRads(vvFovDegs);

    //float offsetx = m_ScreenOffsetX/m_ScreenWidth;
    //float offsety = m_ScreenOffsetY/m_ScreenHeight;

    //float vvWidth = orthoright - ortholeft;
    //float vvHeight = orthotop - orthobottom;
    //float vvDepth = 1 - -1;
    //float vvNear = 1;//(vvHeight / 2.0f) / (float)tan(vvFovRads / 2.0f);
    //float vvFar = vvNear + 2; //vvDepth;

    //m_ProjMatrix.SetFrustum( ortholeft, // - offsetx/2 + vvWidth/2,
    //    orthoright, // - offsetx/2 + vvWidth/2,
    //    orthobottom, // - offsety/2 + vvHeight/2,
    //    orthotop, // - offsety/2 + vvHeight/2,
    //    vvNear, vvFar );

    //m_ViewMatrix.SetIdentity();
    //float transx = 0.5f; //vvWidth/2; // - m_ScreenOffsetX/m_ScreenWidth;
    //float transy = 0.5f; //vvHeight/2; // - m_ScreenOffsetY/m_ScreenHeight;
    //float transz = -vvNear - 1; //0.1f; //-(vvNear + (vvDepth / 2.0f));
    //m_ViewMatrix.TranslatePostRotation( transx, transy, transz );
    ////m_ViewMatrix.TranslatePostRotation( (ortholeft - orthoright)/2.0f, (orthobottom - orthotop)/2.0f, -2 );

    m_OrthoMatrix.CreateOrtho( ortholeft, orthoright, orthobottom, orthotop, 1, -1 ); // 0, 0 is bottom left.

    float scrw = g_pGame->m_GameWidth;
    float scrh = g_pGame->m_GameHeight;
    m_OrthoMatrixGameSize.CreateOrtho( ortholeft*scrw, orthoright*scrw, orthobottom*scrh, orthotop*scrh, 1, -1 ); // 0, 0 is bottom left.

    m_OrthoLeft = ortholeft*scrw;
    m_OrthoRight = orthoright*scrw;
    m_OrthoBottom = orthobottom*scrh;
    m_OrthoTop = orthotop*scrh;

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    LOGInfo( LOGTag, "[OnSurfaceChanged]===========================================\n" );

    m_TimeBeforeWeStartAdView = 5;

    if( g_pScreenManager )
        g_pScreenManager->OnResized();
}

double GameEmptyReplaceMe::Tick(double TimePassed)
{
    //LOGInfo( LOGTag, "GameEmptyReplaceMe::Tick()\n" );
    
    GameCore::Tick( TimePassed );

#if MYFW_ANDROID
    m_pSoundPlayer->Tick( TimePassed );
#endif

    //GetMemoryUsageCount();

    double lasttime = m_TimeBeforeWeStartAdView;
    m_TimeBeforeWeStartAdView -= TimePassed;
    if( lasttime >= 0 && m_TimeBeforeWeStartAdView < 0 )
    {
        //StartAdView();
    }

    StatsFunction_AddTime( TimePassed );

    if( m_pSaveGameFile )
    {
        m_pSaveGameFile->Tick();
        if( m_pSaveGameFile->QueryIsDone() )
        {
            if( m_pSaveGameFile->m_SaveFileOp == SFO_Read )
            {
                char* savegamestring = m_pSaveGameFile->QueryString();

                if( savegamestring != 0 )
                {
                    LOGInfo( LOGTag, "m_pSaveGameFile read complete: %s\n", savegamestring );

                    // TODO: process the string.
                    m_pProfileManager->LoadSavedProfilesFromString( savegamestring );

                    //SetMusicVolume( (float)m_pProfileManager->m_CurrentProfile->m_MusicEnabled );
                }
                else
                {
                    SetMusicVolume( 1 );
                    LOGInfo( LOGTag, "m_pSaveGameFile read complete: no data loaded\n" );
                }

                m_pSaveGameFile->Reset();
            }
            else if( m_pSaveGameFile->m_SaveFileOp == SFO_Write )
            {
                MyAssert( m_pCurrentStringBeingSaved );
                SAFE_DELETE_ARRAY( m_pCurrentStringBeingSaved )

                LOGInfo( LOGTag, "m_pSaveGameFile write complete.\n" );

                m_pSaveGameFile->Reset();
            }
        }
    }

    h_totaltime += TimePassed;
    h_numframes++;
    if( h_totaltime > 1 )
    {
        m_DebugFPS = h_numframes;
        //LOGInfo( LOGTag, "FPS: %d\n", m_DebugFPS );
        //LOGInfo( LOGTag, "fps - totaltime(%d) - frames(%d)", totaltime, numframes/2 );
        h_numframes = 0;
        h_totaltime = 0;
    }

    if( m_GameTypeToSwitchToNextTick != GAMETYPE_None )
    {
        SwitchGameType( m_GameTypeToSwitchToNextTick );
        m_GameTypeToSwitchToNextTick = GAMETYPE_None;
    }

    if( m_SaveGameNextTick && m_pSaveGameFile->QueryIsDone() )
    {
        m_pCurrentStringBeingSaved = m_pProfileManager->CreateSaveString();
        m_pSaveGameFile->WriteString( "", "SavedData.dat", m_pCurrentStringBeingSaved );

        m_SaveGameNextTick = false;
    }

    if( m_HasFocus )
    {
        g_pScreenManager->Tick( TimePassed );

        //if( m_pResources )
        //    m_pResources->Tick( TimePassed );

        if( m_pAchievementChunk )
            m_pAchievementChunk->Tick( TimePassed );

        g_pGame->m_pProfileManager->m_CurrentProfile->DisplayEarnedAchievements();

        for( unsigned int i=0; i<m_ObjectPool_ScoreChunks.m_ActiveObjects.Count(); i++ )
        {
            m_ObjectPool_ScoreChunks.m_ActiveObjects[i]->Tick( TimePassed );
        }
    }

    if( g_pScreenOverlayManager->m_NeedsTick )
        g_pScreenOverlayManager->Tick();

    //m_pHighScoreStorage->Tick( TimePassed );
    m_pLeaderboardStorage->Tick( TimePassed );

    //LOGInfo( LOGTag, "~GameEmptyReplaceMe::Tick()\n" );

    return 0;
}

void GameEmptyReplaceMe::OnDrawFrame(unsigned int canvasid)
{
    if( IsReadyToRender() == false )
        return;

    GameCore::OnDrawFrame( canvasid );

    // clear the viewport to dark gray.
    //glClearColor( 0.1f, 0.1f, 0.8f, 1.0f );
    //glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glClearColor( 0, 0, 0, 1 );
    checkGlError( "glClearColor" );

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    checkGlError( "glClear" );

    if( m_pResources == 0 || m_pResources->m_AllResourcesLoaded == false )
    {
        FontDefinition* pFont = g_pGame->m_pSystemFont;

        if( pFont && pFont->m_pBMFont && pFont->m_pTextureDef && pFont->m_pTextureDef->m_TextureID )
        {
            int scrw = (int)g_pGame->m_GameWidth;
            int scrh = (int)g_pGame->m_GameHeight;
            RenderTextQuick( pFont, 40, 320.0f, 960.0f, Justify_CenterX|Justify_CenterY, "LOADING" );
        }

        return;
    }

    // Calculate the view proj matrix
    m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;

    g_pScreenManager->Draw();

    if( m_pAchievementChunk )
        m_pAchievementChunk->Draw();

    // draw all the small text chunks over the playfield.
    for( unsigned int i=0; i<m_ObjectPool_ScoreChunks.m_ActiveObjects.Count(); i++ )
    {
        m_ObjectPool_ScoreChunks.m_ActiveObjects[i]->Draw();
    }

    FontDefinition* pFont = g_pGame->m_pSystemFont;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    RenderTextQuick( pFont, 40, g_pGame->m_OrthoRight, g_pGame->m_OrthoTop, Justify_Top|Justify_Right, "%d", m_DebugFPS );
}

bool GameEmptyReplaceMe::OnTouch(int action, int id, float x, float y, float pressure, float size)
{
    if( id >= MAX_FINGERS )
        return false;

    x = (x - m_ScreenOffsetX - m_WindowStartX) / m_ScreenWidth * m_GameWidth;
    y = (y - m_ScreenOffsetY - m_WindowStartY) / m_ScreenHeight * m_GameHeight;

    GameCore::OnTouch( action, id, x, y, pressure, size );
    //LOGInfo( LOGTag, "OnTouch (%d %d)(%f,%f)(%f %f)\n", action, id, x, y, pressure, size);

    return g_pScreenManager->OnTouch( action, id, x, y, pressure, size );
}

bool GameEmptyReplaceMe::OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id)
{
    GameCore::OnButtons( action, id );

    return g_pScreenManager->OnButtons( action, id );
}

bool GameEmptyReplaceMe::OnKeys(GameCoreButtonActions action, int keycode, int unicodechar)
{
    GameCore::OnKeys( action, keycode, unicodechar );

    return g_pScreenManager->OnKeys( action, keycode, unicodechar );
}

void GameEmptyReplaceMe::SwitchGameType(GameType newtype)
{
    g_pScreenManager->CloseAllScreens();

    bool initialized = false;

    if( newtype == GAMETYPE_Selector )
    {
        m_pCurrentGame = MyNew GameType_Selector;
        g_pScreenManager->PushScreen( m_pCurrentGame );
    }
    else if( newtype == GAMETYPE_EmptyReplaceMe )
    {
        m_pGameOptions->m_Something = 0;
        m_pCurrentGame = MyNew GameType_EmptyReplaceMe;
        g_pScreenManager->PushScreen( m_pCurrentGame );
    }

    MyAssert( m_pCurrentGame );

    if( initialized == false )
        m_pCurrentGame->Init();

    m_pCurrentGame->OnDisplay();
}

void GameEmptyReplaceMe::SetScreenOverlayToShow(ScreenOverlays screen)
{
    m_ScreenToShow = screen;
}

void GameEmptyReplaceMe::KickOffASave()
{
    m_SaveGameNextTick = true;
}
