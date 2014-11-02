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
#include "Achievements.h"

Achievement g_Achievements[ACHID_NumAchs] = 
{
    { ACHID_FirstWord,          "FirstWord",         "First Word",       "Create your first word",           "Created your first word", },
    { ACHID_First8LetterWord,   "First8LetterWord",  "8 Letters!!",      "Make an 8 letter word",            "You made an 8 letter word", },
    { ACHID_InfiniteLevel2,     "InfiniteLevel2",    "Level 2",          "Reach level 2 in Infinite Mode",   "Reached level 2 in Infinite Mode", },
    { ACHID_InfiniteLevel5,     "InfiniteLevel5",    "Level 5",          "Reach level 5 in Infinite Mode",   "Reached level 5 in Infinite Mode", },
    { ACHID_InfiniteLevel10,    "InfiniteLevel10",   "Level 10",         "Reach level 10 in Infinite Mode",  "Reached level 10 in Infinite Mode", },
    { ACHID_InfiniteLevel15,    "InfiniteLevel15",   "Level 15",         "Reach level 15 in Infinite Mode",  "Reached level 15 in Infinite Mode", },
    { ACHID_InfiniteLevel20,    "InfiniteLevel20",   "Level 20",         "Reach level 20 in Infinite Mode",  "Reached level 20 in Infinite Mode", },
    { ACHID_PuzzleLevel1,       "PuzzleLevel1",      "First Puzzle",     "Finish 1st puzzle",                "Finished 1st puzzle", },
    { ACHID_PuzzleLevel10,      "PuzzleLevel9",      "Puzzle 10",        "Finish 10th puzzle",               "Finished 10th puzzle", },
    { ACHID_PuzzleLevel20,      "PuzzleLevel18",     "Puzzle 20",        "Finish 20th puzzle",               "Finished 20th puzzle", },
    { ACHID_PuzzleLevel30,      "PuzzleLevel27",     "Puzzle 30",        "Finish 30th puzzle",               "Finished 30th puzzle", },
    { ACHID_PuzzleLevel40,      "PuzzleLevel36",     "Puzzle 40",        "Finish 40th puzzle",               "Finished 40th puzzle", },
    { ACHID_PuzzleLevel50,      "PuzzleLevel43",     "Puzzle 50",        "Finish 50th puzzle",               "Finished 50th puzzle", },
    { ACHID_PuzzleAll1Star,     "PuzzleAll1Star",    "Puzzle 1 Star",    "Earn 1 star in each puzzle",       "Earned 1 star in each puzzle", },
    { ACHID_PuzzleAll2Stars,    "PuzzleAll2Stars",   "Puzzle 2 Stars",   "Earn 2 stars in each puzzle",      "Earned 2 stars in each puzzle", },
    { ACHID_PuzzleAll3Stars,    "PuzzleAll3Stars",   "Puzzle 3 Stars",   "Earn 3 stars in each puzzle",      "Earned 3 stars in each puzzle", },
    { ACHID_17,                 "ACHID_17",          "Name 17",          "Some long-ass achievement description 17", "", },
    { ACHID_18,                 "ACHID_18",          "Name 18",          "Some long-ass achievement description 18", "", },
    { ACHID_19,                 "ACHID_19",          "Name 19",          "Some long-ass achievement description 19", "", },
    { ACHID_20,                 "ACHID_20",          "Name 20",          "Some long-ass achievement description 20", "", },
    { ACHID_21,                 "ACHID_21",          "Name 21",          "Some long-ass achievement description 21", "", },
    { ACHID_22,                 "ACHID_22",          "Name 22",          "Some long-ass achievement description 22", "", },
    { ACHID_23,                 "ACHID_23",          "Name 23",          "Some long-ass achievement description 23", "", },
    { ACHID_24,                 "ACHID_24",          "Name 24",          "Some long-ass achievement description 24", "", },
    { ACHID_25,                 "ACHID_25",          "Name 25",          "Some long-ass achievement description 25", "", },
    { ACHID_26,                 "ACHID_26",          "Name 26",          "Some long-ass achievement description 26", "", },
    { ACHID_27,                 "ACHID_27",          "Name 27",          "Some long-ass achievement description 27", "", },
    { ACHID_28,                 "ACHID_28",          "Name 28",          "Some long-ass achievement description 28", "", },
    { ACHID_29,                 "ACHID_29",          "Name 29",          "Some long-ass achievement description 29", "", },
    { ACHID_30,                 "ACHID_30",          "Name 30",          "Some long-ass achievement description 30", "", },
};

AchievementChunk::AchievementChunk()
: m_BGColor(90, 90, 90, 255)
, m_Color(255, 255, 255, 255)
{
    Reset();
}

AchievementChunk::~AchievementChunk()
{
}

void AchievementChunk::Reset()
{
    m_InUse = false;

    m_String[0] = 0;
    m_X = -10000;
    m_Y = -10000;
    m_Size = 0;

    m_Color = ColorByte(255, 255, 255, 255);
        
    m_TimeAlive = 0;
    m_TimeToLive = 1;
}

void AchievementChunk::Setup( float x, float y, AchievementID achid )
{
    Reset();

    m_InUse = true;

    m_X = x;
    m_Y = y;

    strcpy_s( m_String, 32, g_Achievements[achid].name );

    m_TimeToLive = 2.0f;
}

void AchievementChunk::Tick(double TimePassed)
{
    if( m_InUse == false )
        return;

    float timepassed = (float)TimePassed;
    m_TimeAlive += timepassed;

    if( m_TimeAlive > m_TimeToLive )
    {
        Reset();
        //g_pGravityBallsGame->m_ObjectPool_ScoreChunks.MakeObjectInactive( this );
    }
}

void AchievementChunk::Draw()
{
    if( m_InUse == false || g_pGame == 0 )
        return;

    float posx = (m_X) / g_pGame->m_GameWidth;
    float posy = (m_Y) / g_pGame->m_GameHeight;

    FontDefinition* pFont = g_pGame->m_pResources->m_pFontText;
    int scrw = (int)g_pGame->m_GameWidth;
    int scrh = (int)g_pGame->m_GameHeight;
    float fontheight = 40;
    unsigned char justification = Justify_CenterX|Justify_CenterY;

    float timetoenter = 0.4f;
    float timetoexit = 0.4f;

    if( m_TimeAlive < timetoenter )
    {
        float enterperc = m_TimeAlive / timetoenter;
        MyClamp( enterperc, 0.0f, 1.0f );

        posx = posx - 1.0f*(1-enterperc);
    }
    else if( m_TimeAlive > m_TimeToLive - timetoexit )
    {
        float timeintoexit = m_TimeAlive - (m_TimeToLive - timetoexit);
        float exitperc = timeintoexit / timetoexit;
        MyClamp( exitperc, 0.0f, 1.0f );

        posx = posx + 1.0f*exitperc;
    }

    //g_pGame->m_pResources->m_pSprites[SL_Playfield]->Create( 0.5f, 0.1f, 0, 1, 0, 1, Justify_CenterX|Justify_CenterY );
    //g_pGame->m_pResources->m_pSprites[SL_Playfield]->SetPosition( posx, posy, 0 );
    //g_pGame->m_pResources->m_pSprites[SL_Playfield]->SetTint( m_BGColor );
    //g_pGame->m_pResources->m_pSprites[SL_Playfield]->Draw( &g_pGame->m_OrthoMatrix );

    RenderTextQuickWithColor( pFont, fontheight, posx*scrw, (posy+0.02f)*scrh, justification, m_Color, "Achievement!" );

    RenderTextQuickWithColor( pFont, fontheight, posx*scrw, (posy-0.02f)*scrh, justification, m_Color, m_String );

    //g_pGame->m_pResources->m_pSprites[SL_Playfield]->SetTint( ColorByte(255,255,255,255) );
}
