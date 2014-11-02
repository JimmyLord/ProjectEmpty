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
#include "ProfileManager.h"

void PlayerProfile::AwardAchievement( AchievementID ach )
{
    if( m_AchievementsEarned[ach] == false )
    {
        m_AchievementsEarned[ach] = true;
        g_pGameServiceManager->AddAchToList( m_Username, m_Password, LeaderboardGameID, ach, 100.0f, g_Achievements[ach].achidstr, GameServiceProtocol_All );
#if MYFW_BLACKBERRY10
        g_pBBM->AddStringToBBMProfile( g_Achievements[ach].achidstr, "Achievement: %s\n%s", g_Achievements[ach].name, g_Achievements[ach].descriptionEarned );
#endif
    }
}

void PlayerProfile::DisplayEarnedAchievements()
{
    if( g_pGame->m_pAchievementChunk->m_InUse == true )
        return;

    for( int i=0; i<ACHID_NumAchs; i++ )
    {
        if( m_AchievementsEarned[i] == 1 && m_AchievementsDisplayed[i] == false )
        {
            m_AchievementsDisplayed[i] = true;

            g_pGame->m_pAchievementChunk->Setup( g_pGame->m_GameWidth/2, 720.0f, (AchievementID)i );
            return;
        }
    }
}

void PlayerProfile::SubmitOldAchievementsAndScores()
{
    // loop through unsubmitted scores and achievements and submit them if necessary.
    for( int i=0; i<ACHID_NumAchs; i++ )
    {
        for( int p=0; p<GameServiceProtocol_NumProtocols; p++ )
        {
            if( p == GameServiceProtocol_MyServer )
                continue;
#if !MYFW_IOS
            if( p == GameServiceProtocol_GameCenter )
                continue;
#endif
            
            if( m_AchievementsSubmitted[p][i] == false && m_AchievementsEarned[i] == (int)true )
            {
                g_pGameServiceManager->AddAchToList( m_Username, m_Password, LeaderboardGameID, i, 100.0f, g_Achievements[i].achidstr, (GameServiceProtocol)p );
                break;
            }
        }
    }
        
    for( int i=0; i<Leaderboard_NumBoards; i++ )
    {
        int score = 0;
        
        if( i == Leaderboard_Main )
        {
            score = 1;
        }
        
        if( score > 0 )
        {
            for( int p=0; p<GameServiceProtocol_NumProtocols; p++ )
            {
#if !MYFW_IOS
                if( p == GameServiceProtocol_GameCenter )
                    continue;
#endif
                
                if( m_HighScoresSubmitted[p][i] == false )
                {
                    g_pGameServiceManager->AddScoreToList(
                            m_Username, m_Password, LeaderboardGameID,
                            i, score, LeaderboardBoardStringIDs[i], false, (GameServiceProtocol)p );
                }
            }
        }
    }
}

ProfileManager::ProfileManager()
{
    for( int i=0; i<MAX_PROFILES; i++ )
    {
        m_pProfiles[i] = MyNew PlayerProfile;
    }

    m_CurrentProfileIndex = 0;

    m_CurrentProfile = m_pProfiles[m_CurrentProfileIndex];
}

ProfileManager::~ProfileManager()
{
    for( int i=0; i<MAX_PROFILES; i++ )
        SAFE_DELETE( m_pProfiles[i] );
}

void ProfileManager::LoadSavedProfilesFromString(char* saveddata)
{
    cJSON* root = cJSON_Parse( saveddata );

    if( root == 0 )
        return;
    
    cJSON* profiles = cJSON_GetObjectItem( root, "Profiles" );

    if( profiles == 0 )
        return;

    for( int i=0; i<MAX_PROFILES; i++ )
    {
        cJSON* profile = cJSON_GetArrayItem( profiles, i );

        if( profile == 0 )
            return;

        cJSON* obj;
        cJSON* jsonarray;

        obj = cJSON_GetObjectItem( profile, "m_LanguageSelected" ); if( obj ) m_pProfiles[i]->m_LanguageSelected = obj->valueint;
        obj = cJSON_GetObjectItem( profile, "m_UserIndex" ); if( obj ) m_pProfiles[i]->m_UserIndex = obj->valueint;
        obj = cJSON_GetObjectItem( profile, "m_Username" ); if( obj ) strcpy_s( m_pProfiles[i]->m_Username, MAX_USERPASS_LENGTH, obj->valuestring );
        obj = cJSON_GetObjectItem( profile, "m_Password" ); if( obj ) strcpy_s( m_pProfiles[i]->m_Password, MAX_USERPASS_LENGTH, obj->valuestring );

        jsonarray = cJSON_GetObjectItem( profile, "Stats" );
        if( jsonarray )
        {
            for( int j=0; j<Num_PlayerStats; j++ )
            {
                obj = cJSON_GetArrayItem( jsonarray, j ); if( obj ) m_pProfiles[i]->m_PlayerStatsAllTime[j] = obj->valueint;
            }
        }

        jsonarray = cJSON_GetObjectItem( profile, "Achs" );
        if( jsonarray )
        {
            for( int j=0; j<ACHID_NumAchs; j++ )
            {
                obj = cJSON_GetArrayItem( jsonarray, j ); if( obj ) m_pProfiles[i]->m_AchievementsEarned[j] = obj->valueint;
                if( m_pProfiles[i]->m_AchievementsEarned[j] == 1 ) 
                    m_pProfiles[i]->m_AchievementsDisplayed[j] = true;
            }
        }
    }

    cJSON_Delete( root );
}

char* ProfileManager::CreateSaveString()
{
	cJSON* root;

	root = cJSON_CreateObject();

    cJSON* profilesarray = cJSON_CreateArray();
    cJSON_AddItemToObject( root, "Profiles", profilesarray );

    for( int i=0; i<MAX_PROFILES; i++ )
    {
        cJSON* profile = cJSON_CreateObject();
        cJSON_AddItemToArray( profilesarray, profile );

        cJSON* jsonarray;

        cJSON_AddItemToObject( profile, "m_LanguageSelected", cJSON_CreateNumber( m_pProfiles[i]->m_LanguageSelected ) );
        cJSON_AddItemToObject( profile, "m_UserIndex", cJSON_CreateNumber( m_pProfiles[i]->m_UserIndex ) );
        cJSON_AddItemToObject( profile, "m_Username", cJSON_CreateString( m_pProfiles[i]->m_Username ) );
        cJSON_AddItemToObject( profile, "m_Password", cJSON_CreateString( m_pProfiles[i]->m_Password ) );

        jsonarray = cJSON_CreateDoubleArray( m_pProfiles[i]->m_PlayerStatsAllTime, Num_PlayerStats );
        cJSON_AddItemToObject( profile, "Stats", jsonarray );

        jsonarray = cJSON_CreateIntArray( m_pProfiles[i]->m_AchievementsEarned, ACHID_NumAchs );
        cJSON_AddItemToObject( profile, "Achs", jsonarray );
    }

    char* savestring = cJSON_Print( root );
    cJSON_Delete(root);

    char* RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations;
    int len = strlen( savestring );
    RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations = MyNew char[len+1];
    strcpy_s( RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations, len+1, savestring );
    free( savestring );

    return RidiculousReallocatedStringSoICanUseMyNewDeleteOverridesWithoutChangingThecJSONCodeToUseThemForAllItsSmallAllocations;
}

void ProfileManager::ClearStats()
{
    for( int p=0; p<MAX_PROFILES; p++ )
    {
        for( int i=0; i<Num_PlayerStats; i++ )
        {
            m_pProfiles[p]->ClearStats();
        }
    }
}
