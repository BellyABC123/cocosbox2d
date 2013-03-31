#ifndef _TEST_RESOURCE_H_
#define _TEST_RESOURCE_H_

static const char s_pPathGrossini[]       = "Images/grossini.png";
static const char s_pPathSister1[]        = "Images/grossinis_sister1.png";
static const char s_pPathSister2[]        = "Images/grossinis_sister2.png";
static const char s_pPathB1[]             = "Images/b1.png";
static const char s_pPathB2[]             = "Images/b2.png";
static const char s_pPathR1[]             = "Images/r1.png";
static const char s_pPathR2[]             = "Images/r2.png";
static const char s_pPathF1[]             = "Images/f1.png";
static const char s_pPathF2[]             = "Images/f2.png";
static const char s_pPathBlock[]          = "Images/blocks.png";
static const char s_back[]                = "Images/background.png";
static const char s_back1[]               = "Images/background1.png";
static const char s_back2[]               = "Images/background2.png";
static const char s_back3[]               = "Images/background3.png";
static const char s_stars1[]              = "Images/stars.png";
static const char s_stars2[]              = "Images/stars2.png";
static const char s_fire[]                = "Images/fire.png";
static const char s_snow[]                = "Images/snow.png";
static const char s_streak[]              = "Images/streak.png";
static const char s_PlayNormal[]          = "Images/btn-play-normal.png";
static const char s_PlaySelect[]          = "Images/btn-play-selected.png";
static const char s_AboutNormal[]         = "Images/btn-about-normal.png";
static const char s_AboutSelect[]         = "Images/btn-about-selected.png";
static const char s_HighNormal[]          = "Images/btn-highscores-normal.png";
static const char s_HighSelect[]          = "Images/btn-highscores-selected.png";
static const char s_Ball[]                = "Images/ball.png";
static const char s_Paddle[]              = "Images/paddle.png";
static const char s_pPathClose[]          = "Images/close.png";
static const char s_MenuItem[]            = "Images/menuitemsprite.png";
static const char s_SendScore[]           = "Images/SendScoreButton.png";
static const char s_PressSendScore[]      = "Images/SendScoreButtonPressed.png";
static const char s_Power[]               = "Images/powered.png";
static const char s_AtlasTest[]           = "Images/atlastest.png";

// tilemaps resource
static const char s_TilesPng[]            = "TileMaps/tiles.png";
static const char s_LevelMapTga[]         = "TileMaps/levelmap.tga";

static const float PTM_RATIO = 32.0;

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define EFFECT_FILE_1        "pz1.ogg"
	#define EFFECT_FILE_2        "pz2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
    #define EFFECT_FILE_1        "effect1.raw"
#else
    #define EFFECT_FILE_1        "effect1.wav"
#endif // CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #define MUSIC_FILE        "music.mid"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    #define MUSIC_FILE        "background.ogg"
#else
    #define MUSIC_FILE        "background.mp3"
#endif // CC_PLATFORM_WIN32
#endif
