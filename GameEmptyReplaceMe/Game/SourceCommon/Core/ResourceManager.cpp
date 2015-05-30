//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "GameAudio.h"

SpriteResourceDef g_SpriteResourceDefs[SL_NumSpritesLabels] =
{
    { "Data/Misc/WhiteSquare.png",   SpriteClass_Base, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, 1, 1, 0, 1, 0, 1, Justify_Center, }, //SL_WhiteSquare,
    { "Data/Misc/WhiteSquare.png",   SpriteClass_Base, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, 1, 1, 0, 1, 0, 1, Justify_Center, }, //SL_WhiteSquareResizable,
    { "Data/Misc/Button_Back.png",   SpriteClass_Base, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, 1, 1, 0, 1, 0, 1, Justify_Center, }, //SL_Button_Back
    { "Data/Misc/Button_Cancel.png", SpriteClass_Base, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, 1, 1, 0, 1, 0, 1, Justify_Center, }, //SL_Button_Cancel,
};

ResourceManager::ResourceManager()
//: m_ObjectPool_Clouds(10)
{
    for( int i=0; i<SL_NumSpritesLabels; i++ )
    {
        m_pSpriteTextures[i] = 0;
        m_pSprites[i] = 0;
    }

    for( int i=0; i<TL_NumTextureLabels; i++ )
    {
        m_pTextures[i] = 0;
    }

    for( int i=0; i<SS_NumSpriteSheets; i++ )
    {
        m_pSpriteSheets[i] = 0;
    }

#if USE_LOADWAVESFROMFILESYSTEM
    for( int i=0; i<GameAudio_NumSamples; i++ )
        m_pSoundFiles[i] = 0;
#endif

    m_pEffect_ParticleRisingGlow = 0;
    m_pEffect_ParticleBurst = 0;

    m_pFontText = 0;

    m_AllResourcesLoaded = false;
}

ResourceManager::~ResourceManager()
{
    LOGInfo( LOGTag, "Deleting ~ResourceManager\n" );

    for( int i=0; i<SL_NumSpritesLabels; i++ )
    {
        SAFE_RELEASE( m_pSpriteTextures[i] );
        SAFE_RELEASE( m_pSprites[i] );
    }

    for( int i=0; i<TL_NumTextureLabels; i++ )
    {
        SAFE_RELEASE( m_pTextures[i] );
    }

    for( int i=0; i<SS_NumSpriteSheets; i++ )
    {
        SAFE_DELETE( m_pSpriteSheets[i] );
    }

#if USE_LOADWAVESFROMFILESYSTEM
    for( int i=0; i<GameAudio_NumSamples; i++ )
        SAFE_DELETE( m_pSoundFiles[i] )
#endif

    SAFE_DELETE( m_pEffect_ParticleRisingGlow );
    SAFE_DELETE( m_pEffect_ParticleBurst );

    SAFE_DELETE( m_pFontText );
}

void ResourceManager::LoadAllTextures()
{
    checkGlError("start LoadAllTextures");
    
    // don't load unnecessary files until ready to render.
#if USE_LOADWAVESFROMFILESYSTEM
    if( m_pSoundFiles[0] == 0 && g_pGame->IsReadyToRender() )
    {
        char tempfilename[MAX_PATH];
        for( int i=0; i<GameAudio_NumSamples; i++ )
        {
            sprintf_s( tempfilename, MAX_PATH, "%s%s", GameAudioFilenames[i], GameAudioExtension );
            m_pSoundFiles[i] = RequestFile( tempfilename );
        }
    }
#endif

    for( int i=0; i<SL_NumSpritesLabels; i++ )
    {
        MyAssert( m_pSpriteTextures[i] == 0 );
        MyAssert( m_pSprites[i] == 0 );
    }

    for( int i=0; i<TL_NumTextureLabels; i++ )
    {
        MyAssert( m_pTextures[i] == 0 );
    }

    m_pEffect_ParticleRisingGlow = MyNew Effect_ParticleRisingGlow( g_pGame->m_pParticleRenderer );
    m_pEffect_ParticleBurst = MyNew Effect_ParticleBurst( g_pGame->m_pParticleRenderer );
    //m_pEffect_ParticleBurst = MyNew Effect_ParticleBurst( g_pGame->m_pParticleRenderer_Smoke );
    m_pEffect_ParticleBurst->m_InitialSpeedBoost = 1.0;
    m_pEffect_ParticleBurst->m_CenterVariation.x = 5;
    m_pEffect_ParticleBurst->m_CenterVariation.y = 5;
    m_pEffect_ParticleBurst->m_ScaleSpeed = 6;
    m_pEffect_ParticleBurst->m_Speed = 0;
    m_pEffect_ParticleBurst->m_SpeedVariation = 50;
    m_pEffect_ParticleBurst->m_Size = 12; //6;
    m_pEffect_ParticleBurst->m_SizeVariation = 10; //5;
    m_pEffect_ParticleBurst->m_ColorTransitionDelay = 0.2f;
    m_pEffect_ParticleBurst->m_TimeToLive = 0.4f;
    m_pEffect_ParticleBurst->m_TimeToLiveVariation = 0.1f;
    //m_pEffect_ParticleBurst->m_ColorTransitionSpeed = -1.0f;
    m_pEffect_ParticleBurst->m_Color1 = ColorByte(255,255,255,255);
    m_pEffect_ParticleBurst->m_Color2 = ColorByte(150,150,150,255);
    m_pEffect_ParticleBurst->m_UseColorsAsOptions = true;

    for( int i=0; i<SL_NumSpritesLabels; i++ )
    {
        m_pSpriteTextures[i] = g_pTextureManager->CreateTexture( g_SpriteResourceDefs[i].m_Filename, g_SpriteResourceDefs[i].m_MinFilter, g_SpriteResourceDefs[i].m_MagFilter, g_SpriteResourceDefs[i].m_WrapS, g_SpriteResourceDefs[i].m_WrapT );

        if( g_SpriteResourceDefs[i].m_SpriteClass == SpriteClass_Base )
            m_pSprites[i] = MyNew MySprite();
        else
            m_pSprites[i] = MyNew MySprite_XYZVertexColor();

        m_pSprites[i]->GetMaterial()->SetShader( g_pGame->m_pShader_Font );
        m_pSprites[i]->GetMaterial()->SetTextureColor( m_pSpriteTextures[i] );

        m_pSprites[i]->Create( g_SpriteResourceDefs[i].x, g_SpriteResourceDefs[i].y,
                               g_SpriteResourceDefs[i].su, g_SpriteResourceDefs[i].eu,
                               g_SpriteResourceDefs[i].sv, g_SpriteResourceDefs[i].ev,
                               g_SpriteResourceDefs[i].justify );
    }

    m_pTextures[TL_Particle_Glow] = g_pTextureManager->CreateTexture( "Data/Particles/SimpleGlow64.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
    g_pGame->m_pParticleRenderer->SetShaderAndTexture( g_pGame->m_pShader_PointSprite, g_pGame->m_pResources->m_pTextures[TL_Particle_Glow] );

    //m_pSpriteSheets[SS_BlocksSolid] = MyNew SpriteSheet_XYZVertexColor;
    //m_pSpriteSheets[SS_BlocksSolid]->SetScale( 60.0f / 150.0f );
    //m_pSpriteSheets[SS_BlocksSolid]->Load( "Data/Blocks/BlocksSolid" );

    //m_pSpriteSheets[SS_BlocksAlpha] = MyNew SpriteSheet_XYZVertexColor;
    //m_pSpriteSheets[SS_BlocksAlpha]->SetScale( 60.0f / 150.0f );
    //m_pSpriteSheets[SS_BlocksAlpha]->Load( "Data/Blocks/BlocksAlpha" );

    m_pFontText = g_FontManager.CreateFont( "Data/Fonts/System24.fnt" ); //Nevis60.fnt" );

    checkGlError("end LoadAllTextures");
}

void ResourceManager::Tick(double TimePassed)
{
    if( m_AllResourcesLoaded == false )
    {
        bool alltexturesloaded = true;
        for( int i=0; i<SL_NumSpritesLabels; i++ )
        {
            if( m_pSpriteTextures[i]->m_FullyLoaded == false )
            {
                alltexturesloaded = false;
            }
        }
        
        for( int i=0; i<TL_NumTextureLabels; i++ )
        {
            if( m_pTextures[i]->m_FullyLoaded == false )
            {
                alltexturesloaded = false;
            }
        }

        for( int i=0; i<SS_NumSpriteSheets; i++ )
        {
            if( m_pSpriteSheets[i] )
            {
                m_pSpriteSheets[i]->Tick( TimePassed );
                if( m_pSpriteSheets[i]->m_FullyLoaded == false )
                {
                    alltexturesloaded = false;
                }
            }
        }

#if USE_LOADWAVESFROMFILESYSTEM
        bool allsoundsloaded = true;
        for( int i=0; i<GameAudio_NumSamples; i++ )
        {
            if( m_pSoundFiles[i] && m_pSoundFiles[i]->m_FileReady == false )
            {
                allsoundsloaded = false;
            }
        }
#endif

        if( alltexturesloaded &&
#if USE_LOADWAVESFROMFILESYSTEM
            allsoundsloaded &&
#endif
            m_pFontText && m_pFontText->m_pTextureDef && m_pFontText->m_pTextureDef->m_FullyLoaded &&
            (g_pGame->m_pSaveGameFile == 0 || g_pGame->m_pSaveGameFile->QueryIsDone()) // make sure the saved game is fully loaded.
          )
        {
            // bit of a hack, but will work for this project, load all game audio once resources are loaded.
            //   game audio will load from resource files loaded above.
            g_pGame->m_pGameAudio->LoadAllGameAudio();

            LOGInfo( LOGTag, "m_AllResourcesLoaded = true\n" );
            m_AllResourcesLoaded = true;

            for( int i=0; i<SS_NumSpriteSheets; i++ )
            {
                if( m_pSpriteSheets[i] )
                {
                    m_pSpriteSheets[i]->Tick( TimePassed );

                    for( int s=0; s<m_pSpriteSheets[i]->m_NumSprites; s++ )
                    {
                        m_pSpriteSheets[i]->m_pSprites[s]->GetMaterial()->SetShader( g_pGame->m_pShader_TextureVertexColor );
                        m_pSpriteSheets[i]->m_pSprites[s]->GetMaterial()->SetTextureColor( m_pSpriteSheets[i]->m_pTextureDef );
                    }
                }
            }
        }
    }

    checkGlError("end ResourceManager::Tick");
}

void ResourceManager::Draw()
{
}
