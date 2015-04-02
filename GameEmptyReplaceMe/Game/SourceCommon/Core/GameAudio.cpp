//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#if MYFW_ANDROID
    const char* GameAudioExtension = ".ogg";
#else
    const char* GameAudioExtension = ".wav";
#endif

char GameAudioFilenames[GameAudio_NumSamples][64] = 
{
    "Data/Audio/shoot",
    "Data/Audio/explode",
};

GameAudio::GameAudio()
{
    for( int i=0; i<GameAudio_NumSamples; i++ )
        m_AudioIds[i] = -1;
}

GameAudio::~GameAudio()
{
}

void GameAudio::LoadAllGameAudio()
{
    //assert( g_pGameCore->m_pSoundPlayer != 0 );
    if( g_pGameCore->m_pSoundPlayer == 0 )
    {
        LOGError( LOGTag, "g_pGameCore->m_pSoundPlayer NOT INITIALIZED!" );
        return;
    }

#if USE_LOADWAVESFROMFILESYSTEM
    for( int i=0; i<GameAudio_NumSamples; i++ )
    {
        if( g_pGame->m_pResources->m_pSoundFiles[i] &&
            g_pGame->m_pResources->m_pSoundFiles[i]->m_FileReady )
        {
            //g_WaveLoader.LoadFromMemoryIntoOpenALBuffer( g_pGame->m_pResources->m_pGBResources->m_pSoundFiles[i]->m_pBuffer );
            m_AudioIds[i] = g_pGameCore->m_pSoundPlayer->LoadSound(
                    g_pGame->m_pResources->m_pSoundFiles[i]->m_pBuffer,
                    g_pGame->m_pResources->m_pSoundFiles[i]->m_FileLength );
        }
    }
#else
    for( int i=0; i<GameAudio_NumSamples; i++ )
        m_AudioIds[i] = g_pGameCore->m_pSoundPlayer->LoadSound( GameAudioFilenames[i], GameAudioExtension );
#endif
}

void GameAudio::Play(GameAudioCues type)
{
    if( g_pGame->m_pSoundPlayer == 0 )
        return;

    int r = 0;
    GameAudioSamples basesampleindex = GameAudio_NumSamples;

    switch( type )
    {
    case GameAudioCue_None:
        return;
        break;
            
    case GameAudioCue_Shoot:
        r = 0;
        basesampleindex = GameAudio_Shoot;
        break;

    case GameAudioCue_Explode:
        r = 0;
        basesampleindex = GameAudio_Explode;
        break;
    };

    assert( basesampleindex + r < GameAudio_NumSamples );
    if( basesampleindex + r < GameAudio_NumSamples )
        g_pGame->m_pSoundPlayer->PlaySound( m_AudioIds[basesampleindex + r] );
}
