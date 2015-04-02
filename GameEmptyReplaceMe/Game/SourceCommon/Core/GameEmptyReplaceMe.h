//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __GameEmptyReplaceMe_H__
#define __GameEmptyReplaceMe_H__

class GameCore;
class GameEmptyReplaceMe;
class BMFont;

class ResourceManager;
class GameAudio;
class Screen_Base;
class ProfileManager;

class GameBoard;
class SlidingTile;
struct Point;
class AchievementChunk;
class ScoreChunk;

class ParticleRenderer;

//class HighScoreStorage;
class LeaderboardStorage;

struct GameOptions
{
    int m_Something;
};

enum GameType
{
    GAMETYPE_None,
    GAMETYPE_Selector,
    GAMETYPE_EmptyReplaceMe,
};

extern GameEmptyReplaceMe* g_pGameEmptyReplaceMe;
extern GameEmptyReplaceMe* g_pGame;

extern ColorByte GameMenuButtonColors[4];

class GameEmptyReplaceMe : public GameCore
{
public:
    Screen_Base* m_pScreenOverlay;
    ScreenOverlays m_ScreenToShow;

    MyMatrix m_ViewProjMatrix;
    MyMatrix m_WorldMatrix;
    MyMatrix m_ViewMatrix;
    MyMatrix m_ProjMatrix;
    MyMatrix m_OrthoMatrix;
    MyMatrix m_OrthoMatrixGameSize;

    MyFileObject* m_pShaderFile_Simple;
    MyFileObject* m_pShaderFile_Font;
    MyFileObject* m_pShaderFile_Texture;
    MyFileObject* m_pShaderFile_TextureVertexColor;
    MyFileObject* m_pShaderFile_PointSprite;

    ShaderGroup* m_pShader_Simple;
    ShaderGroup* m_pShader_Font;
    ShaderGroup* m_pShader_Texture;
    ShaderGroup* m_pShader_TextureVertexColor;
    ShaderGroup* m_pShader_PointSprite;

    ParticleRenderer* m_pParticleRenderer;

    MTRand_int32 m_Rand;
    MTRand_int32 m_NetworkRand;

    Screen_Base* m_pCurrentGame;
    GameOptions* m_pGameOptions;
    GameType m_GameTypeToSwitchToNextTick;
    bool m_SaveGameNextTick;

    AchievementChunk* m_pAchievementChunk;
    MyActivePool<ScoreChunk*> m_ObjectPool_ScoreChunks;

    double m_TimeBeforeWeStartAdView;

    float m_CamX;
    float m_CamY;
    float m_CamZ;

    int m_DebugFPS;

    float m_GameWidth;
    float m_GameHeight;

    float m_GameFullWidth;
    float m_GameFullHeight;

    float m_DeviceWidth;
    float m_DeviceHeight;

    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_ScreenOffsetX;
    float m_ScreenOffsetY;

    float m_OrthoLeft;
    float m_OrthoRight;
    float m_OrthoBottom;
    float m_OrthoTop;

    FontDefinition* m_pSystemFont;

    bool h_WaitATickForLoadScreen;
    ResourceManager* m_pResources;

    GameAudio* m_pGameAudio;

    ProfileManager* m_pProfileManager;
    MySaveFileObject* m_pSaveGameFile;
    char* m_pCurrentStringBeingSaved;

    //HighScoreStorage* m_pHighScoreStorage;
    LeaderboardStorage* m_pLeaderboardStorage;

    void KickOffASave();

protected:

public:
    GameEmptyReplaceMe();
    virtual ~GameEmptyReplaceMe();

    virtual void InitializeManagers();

    virtual void OneTimeInit();
    virtual bool IsReadyToRender();

    virtual double Tick(double TimePassed);
    virtual void OnSurfaceCreated();
    virtual void OnSurfaceLost();
    virtual void OnSurfaceChanged(unsigned int startx, unsigned int starty, unsigned int width, unsigned int height);
    virtual void OnDrawFrame();

    virtual void OnTouch(int action, int id, float x, float y, float pressure, float size);
    virtual void OnButtons(GameCoreButtonActions action, GameCoreButtonIDs id);
    virtual void OnKeyDown(int keycode, int unicodechar);

    void SetScreenOverlayToShow(ScreenOverlays screen);

    void FinishedLoading();

protected:
    void SwitchGameType(GameType newtype);
};

#endif //__GameEmptyReplaceMe_H__
