#ifndef Macros_h__
#define Macros_h__

#include "Box2D/Box2D.h"

#define ABOUT_SCENE				1
#define MENU_SCENE				2
#define HELLOWORLD_SCENE		3
#define LEVELSELECT_SCENE		4
#define GAME_SCENE				5
#define SHOP_SCENCE				6

#define AUDIO_BACKGROND         "audio/background.wav"
#define AUDIO_CLICK				"audio/click.wav"
#define AUDIO_CUTTING			"audio/cutting.wav"
#define AUDIO_FAIL				"audio/fail.wav"
#define AUDIO_GAME				"audio/game.wav"
#define AUDIO_SUCCESS			"audio/success.wav"
#define AUDIO_WATER				"audio/water.wav"



#define MAX_PACKAGE_NUM			3
#define TOTAL_LEVEL_NUM			32
#define MAX_STAR_NUM			3

#define PTM_RATIO				32

enum CutType
{
	enableCut,
	unableCut
};

enum ElementType{
	eHero,
	eBall,
	eWood,
	eBomb,
	eSurface,
	eStar
};
#endif // Macros_h__