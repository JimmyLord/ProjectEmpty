//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"
#include "ProfileManager.h"

char g_PlayerStatStrings[Num_PlayerStats][128] = 
{
    "All Modes: Total Time",  //PS_TimeInApp,                      // Stats_AddTime          // coded
    "All Modes: Time spent in game",  //PS_TimeInGame,                      // Stats_AddTime          // coded
    "All Modes: Time spent in menus", //PS_TimeInMenus,                     // Stats_AddTime          // coded
    "All Modes: Time spent paused", //PS_TimePaused,                      // Stats_AddTime          // coded

    //Num_PlayerStats,
};

StatTypes g_PlayerStatTypes[Num_PlayerStats] =
{
    StatType_Time, //PS_TimeInApp,                       // Stats_AddTime          // coded
    StatType_Time, //PS_TimeInGame,                      // Stats_AddTime          // coded
    StatType_Time, //PS_TimeInMenus,                     // Stats_AddTime          // coded
    StatType_Time, //PS_TimePaused,                      // Stats_AddTime          // coded

    //StatType_Number, //Num_PlayerStats,
};

//PS_TimeInApp,                       // Stats_AddTime      // coded
//PS_TimeInGame,                      // Stats_AddTime      // coded
//PS_TimeInMenus,                     // Stats_AddTime      // coded
//PS_TimePaused,                      // Stats_AddTime      // coded
void StatsFunction_AddTime(double time)
{
    g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsAllTime[PS_TimeInApp] += time;
    g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsDaily[PS_TimeInApp] += time;

    if( 1 )//g_pGravityBallsGame == 0 )
    {
        g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsAllTime[PS_TimeInMenus] += time;
        g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsDaily[PS_TimeInMenus] += time;
    }
    else
    {
        if( 1 )//g_pGravityBallsGame->m_GamePaused )
        {
            g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsAllTime[PS_TimePaused] += time;
            g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsDaily[PS_TimePaused] += time;
        }
        else
        {
            g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsAllTime[PS_TimeInGame] += time;
            g_pGame->m_pProfileManager->m_CurrentProfile->m_PlayerStatsDaily[PS_TimeInGame] += time;
        }
    }
}
