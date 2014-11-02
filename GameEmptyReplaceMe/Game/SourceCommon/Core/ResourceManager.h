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

#ifndef __ResourceManager_H__
#define __ResourceManager_H__

#include "GameAudio.h"

class BaseObject;
class MyMesh;
class Effect_ParticleRisingGlow;
class Effect_ParticleBurst;
class SpriteSheet;

enum SpriteClass
{
    SpriteClass_Base,
    SpriteClass_XYZVertexColor,
};

struct SpriteResourceDef
{
    const char* m_Filename;
    SpriteClass m_SpriteClass;
    int m_MinFilter;
    int m_MagFilter;
    int m_WrapS;
    int m_WrapT;
    float x;
    float y;
    float su;
    float eu;
    float sv;
    float ev;
    unsigned char justify;
};

enum SpriteLabel
{
    SL_WhiteSquare,
    SL_WhiteSquareResizable,
    SL_Button_Back,
    SL_Button_Cancel,

    SL_NumSpritesLabels,
};

extern SpriteResourceDef g_SpriteResourceDefs[SL_NumSpritesLabels];

enum TextureLabels
{
    TL_Particle_Glow,
    TL_NumTextureLabels,
};

enum SpriteSheets
{
    SS_BlocksSolid,
    SS_BlocksAlpha,
    SS_NumSpriteSheets,
};

class ResourceManager
{
public:
    bool m_AllResourcesLoaded;

    TextureDefinition* m_pSpriteTextures[SL_NumSpritesLabels];
    TextureDefinition* m_pTextures[TL_NumTextureLabels];
    MySprite* m_pSprites[SL_NumSpritesLabels];
#if USE_LOADWAVESFROMFILESYSTEM
    MyFileObject* m_pSoundFiles[GameAudio_NumSamples];
#endif

    Effect_ParticleRisingGlow* m_pEffect_ParticleRisingGlow;
    Effect_ParticleBurst* m_pEffect_ParticleBurst;

    SpriteSheet* m_pSpriteSheets[SS_NumSpriteSheets];

    FontDefinition* m_pFontText;

public:
    ResourceManager();
    ~ResourceManager();

    void LoadAllTextures();
    void Tick(double TimePassed);
    void Draw();
};

#endif //__ResourceManager_H__
