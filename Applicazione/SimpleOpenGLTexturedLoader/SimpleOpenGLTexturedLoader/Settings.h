#pragma once

/* DEFAULT LIBRARIES ********************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

/* ASSIMP FILES ********************************/
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include "GL/glut.h"
#include <IL/il.h>

/* MUSIC ***************************************/
#include "irrklang/irrKlang.h"

/* UTILITIES ********************************/
#include <string.h>
#include <map>

/* CUSTOM OBJECTS ********************************/
#include "Mouse.h"
#include "Keyboard.h"
#include "Render.h"
#include "StateMachine.h"
#include "Ship.h"
#include "Rock.h"
#include "Animations.h"
#include "Sounds.h"
#include "Bullet.h"
#include "Enemy.h"
#include "File.h"
#include "Score.h"
#include "PowerUp.h"

/* DEFINE ********************************/
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)
#define TRUE                1
#define FALSE               0
#define SW 1920
#define SH 1080

/* SCENES INDEX */
#define MENU 0
#define LEVEL_1 1
#define VICTORY_1 2
#define LEVEL_2 3
#define VICTORY_2 4
#define LEVEL_3 5
#define VICTORY_3 6
#define RULES 7
#define COMMANDS 8
#define GAMEOVER -1

/* COSTANTS */
#define LEVEL_2_ISLAND_OFFSET 80.f
#define LEVEL_3_ISLAND_OFFSET 150.f

using namespace std;
using namespace irrklang;

#pragma comment(lib, "irrklang/irrKlang.lib")