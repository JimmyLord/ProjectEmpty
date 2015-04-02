//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __ProfileManager_H__
#define __ProfileManager_H__

#include "Stats.h"
#include "Achievements.h"
#include "LeaderboardsConfig.h"

#define MAX_PROFILES    3

#define INFINITEMODE_HIGHSCORESTOTRACK  10
#define MAX_PUZZLE_LEVELS      1000

#define MAX_USERPASS_LENGTH 15

class PlayerProfile
{
public:
    int m_LanguageSelected;

    int m_UserIndex; // currently used to indicate if the player is logged in. - 0 is no user logged in.

    char m_Username[MAX_USERPASS_LENGTH+1]; // 15 chars and a 0.
    char m_Password[MAX_USERPASS_LENGTH+1]; // 15 chars and a 0.

    double m_PlayerStatsAllTime[Num_PlayerStats];
    double m_PlayerStatsDaily[Num_PlayerStats];

    int m_AchievementsEarned[ACHID_NumAchs];
    bool m_AchievementsDisplayed[ACHID_NumAchs]; // not for saving, just to queue up visuals.

    int m_AchievementsSubmitted[GameServiceProtocol_NumProtocols][ACHID_NumAchs]; // used as bools, but using ints for easier json saving.
    int m_HighScoresSubmitted[GameServiceProtocol_NumProtocols][Leaderboard_NumBoards]; // used as bools, but using ints for easier json saving.

public:
    PlayerProfile()
    {
        m_LanguageSelected = 0;
        m_UserIndex = -1;
        m_Username[0] = 0;
        m_Password[0] = 0;

        ClearStats();
    }

    void ClearStats()
    {
        for( int i=0; i<Num_PlayerStats; i++ )
        {
            m_PlayerStatsAllTime[i] = 0;
            m_PlayerStatsDaily[i] = 0;
        }

        for( int i=0; i<ACHID_NumAchs; i++ )
        {
            m_AchievementsEarned[i] = 0;
            m_AchievementsDisplayed[i] = false;
        }

        ClearGameServiceStatus();
    }

    void ClearGameServiceStatus()
    {
        for( int p=0; p<GameServiceProtocol_NumProtocols; p++ )
        {
            for( int i=0; i<ACHID_NumAchs; i++ )
            {
                m_AchievementsSubmitted[p][i] = false;
            }
            for( int i=0; i<Leaderboard_NumBoards; i++ )
            {
                m_HighScoresSubmitted[p][i] = false;
            }
        }
    }

    void AwardAchievement( AchievementID ach );
    void DisplayEarnedAchievements();
    void SubmitOldAchievementsAndScores();
};

class ProfileManager
{
public:
    PlayerProfile* m_pProfiles[MAX_PROFILES];
    int m_CurrentProfileIndex;
    PlayerProfile* m_CurrentProfile;

public:
    ProfileManager();
    ~ProfileManager();

    void LoadSavedProfilesFromString(char* saveddata);

    int CalculateLengthOfSaveSring();
    char* CreateSaveString(); // this string needs to be deleted when save is complete.
    void ClearStats();
};

#endif //__ProfileManager_H__
