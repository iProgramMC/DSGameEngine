/*---------------------------------------------------------------------------------

	Simple console print demo which prints to bottom screen
	and also has a frame buffer for the top screen to which
	you can draw stuff
	-- iProgramInCpp - Graphics Engine
	-- dovoto - Console bottom screen print

---------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "block.h"
#include "brick.h"
#include "coin.h"
#include "dirt.h"
#include "door.h"
#include "ground.h"
#include "mariojump.h"
#include "mariostill.h"
#include "question.h"
#include "usedblock.h"


#include "ds_game_engine.h"


char sLevel[] = "................................................................"
	"................................................................"
    ".......ooooo...................................................."
	"........ooo....................................................."
    ".......................########................................."
    ".....BB?BBBB?BB.......###..............#.#......................"
    "....................###................#.#......................"
    "...................####........................................."
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.##############.....########"
    "...................................#.#...............###........"
    "........................############.#............###..........."
    "........................#............#.........###.............."
    "........................#.############......###................."
    "........................#................###...................."
    "........................#################......................."
    "................................................................";
int nLevelWidth;
int nLevelHeight;

const char TileSize = 8; //8px wide

short block_image[256];
short brick_image[256];
short coin_image[256];
short dirt_image[256];
short door_image[256];
short ground_image[256];
short question_image[256];
short mariojump_image[384];
short mariostill_image[384];
short usedblock_image[256];

bool bPlayerOnGround = false;

int nDirModX = 0, nDirModY = 0;

float fCameraPosX = 0.0f, fCameraPosY = 0.0f;
float fPlayerPosX = 0.0f, fPlayerPosY = 0.0f;
float fPlayerVelX = 0.0f, fPlayerVelY = 0.0f;

unsigned short rng;

short sky_colour = 0xEEEF;

uint32 global_timer = 0;

char ticks = 0;

unsigned short rng_function(unsigned short input){
	if(input == 0x560A) input=0;
	unsigned short S0 = (unsigned char)input<<8;
	S0 = S0 ^ input;
	input = ((S0 & 0xFF) << 8) | ((S0 & 0xFF00) >> 8);
	S0 = ((unsigned char)S0 << 1) ^ input;
	short S1 = (S0 >> 1) ^ 0xFF80;
	if((S0 & 1) == 0){
		if(S1 == 0xAA55){input = 0;}else{input=S1 ^ 0x1FF4;}
	}else{
		input = S1 ^ 0x1FF4;
	}
	return (unsigned short)input;
}

void rng_update() {
	rng = rng_function(global_timer);
}

char GetTile(int x, int y){
	if(x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight){
		return sLevel[y*nLevelWidth+x];
	}else{
		return ' ';
	}
}
	
void SetTile(char c, int x, int y){
	if(x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight){
		sLevel[y*nLevelWidth+x] = c;
	}
}

void make_btm_screen() {consoleSelect(&bottomScreen);}
void printchar(char x, char y, char c) { printf("\x1b[%d;%dH%c", y, x, c); }

void OnUserCreate()
{
	nLevelWidth = 64;
	nLevelHeight = 16;
	
	decompress(blockBitmap, &block_image,  LZ77);
	decompress(brickBitmap, &brick_image,  LZ77);
	decompress(coinBitmap, &coin_image,  LZ77);
	decompress(dirtBitmap, &dirt_image,  LZ77);
	decompress(doorBitmap, &door_image,  LZ77);
	decompress(groundBitmap, &ground_image,  LZ77);
	decompress(questionBitmap, &question_image,  LZ77);
	decompress(usedblockBitmap, &usedblock_image,  LZ77);
	decompress(mariostillBitmap, &mariostill_image,  LZ77);
	decompress(mariojumpBitmap, &mariojump_image,  LZ77);
	iprintf("\n\n\tGraphics Engine\n\tCoded by iProgramInCpp\n\tMore modern-like\n\tDo whatever you want!");
}

void OnUserUpdate()
{
		clear_screenbuffer(sky_colour);
		
		
		//fPlayerVelX = 0.0f;
		//fPlayerVelY = 0.0f;
		
		//Handle 
		if(get_key_state(KEY_UP)){
			fPlayerVelY = -6.0f;
		}
		if(get_key_state(KEY_DOWN)){
			fPlayerVelY = 6.0f;
		}
		if(get_key_state(KEY_LEFT)){
			nDirModY = 1;
			fPlayerVelX += (bPlayerOnGround ?-6.0f :-4.0f) * 0.01664f;
		}
		if(get_key_state(KEY_RIGHT)){
			nDirModY = 0;
			fPlayerVelX += (bPlayerOnGround ? 6.0f : 4.0f) * 0.01664f;
		}
		if(get_key_down_state(KEY_B)){
			if(bPlayerOnGround){
				fPlayerVelY = -12.0f;
				nDirModX = 1;
			}
		}
		
		fPlayerVelY += 20.0f * 0.01664f;
		
		if(bPlayerOnGround){
			fPlayerVelX += -3.0f * fPlayerVelX * 0.01664f;
			if(fabs(fPlayerVelX) < 0.01f){
				fPlayerVelX = 0.0f;
			}
		}
		
		if(fPlayerVelX > 10.0f) fPlayerVelX = 10.0f;
		if(fPlayerVelX < -10.0f) fPlayerVelX = -10.0f;
		if(fPlayerVelY > 100.0f) fPlayerVelY = 100.0f;
		if(fPlayerVelY < -100.0f) fPlayerVelY = -100.0f;
		
		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * 0.01664f;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * 0.01664f;
		
		if(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == 'o')
			SetTile('.', fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f);
		if(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == 'o')
			SetTile('.', fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f);
		if(GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == 'o')
			SetTile('.', fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f);
		if(GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == 'o')
			SetTile('.', fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f);
			
			
		//Collision
		if(fPlayerVelX <= 0){
			if(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != '.' ||
			   GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != '.'){
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				fPlayerVelX = 0;
			}
		}else{
			if(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != '.' ||
			   GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != '.'){
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;
			}
		}
		
		bPlayerOnGround = false;
		if(fPlayerVelY <= 0){
			if(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != '.' ||
			   GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != '.'){
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}else{
			if(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY+1) != '.' ||
			   GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY+1) != '.'){
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true;
				nDirModX = 0;
			}
		}
		//nDirModX = !bPlayerOnGround;
		
		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;
		
		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;
		
		//Draw Level
		int nTileWidth = 16;
		int nTileHeight = 16;
		int nVisibleTilesX = SCREEN_WIDTH / nTileWidth;
		int nVisibleTilesY = SCREEN_HEIGHT/nTileHeight;
		
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;
		
		if(fOffsetX <= 0.0f) fOffsetX = 0.0f;
		if(fOffsetY <= 0.0f) fOffsetY = 0.0f;
		if(fOffsetX > (float)(nLevelWidth - nVisibleTilesX)) fOffsetX = nLevelWidth - nVisibleTilesX;
		if(fOffsetY > (float)(nLevelHeight- nVisibleTilesY)) fOffsetY = nLevelHeight- nVisibleTilesY;
		
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;
		
						
		for(int x = -1; x < nVisibleTilesX+1; x++){
			for(int y = -1; y < nVisibleTilesY+1; y++){
				char sTileID = GetTile(x + fOffsetX, y+fOffsetY);
				
				switch(sTileID){
					case '.':
						break;
					case '#':
						draw_image(block_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case 'o':
						draw_image(coin_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case '?':
						draw_image(question_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case 'B':
						draw_image(brick_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case 'G':
						draw_image(ground_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case 'D':
						draw_image(dirt_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case 'E':
						draw_image(door_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					case 'U':
						draw_image(usedblock_image, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
					default:
						fill_rectangle(0x0000, nTileWidth, nTileHeight, x*nTileWidth-fTileOffsetX,y*nTileHeight-fTileOffsetY);
						break;
				}
			}
		}
		
		if(nDirModX){
			if(nDirModY){
				draw_image_flipped_h(mariojump_image, nTileWidth, 24, (fPlayerPosX - fOffsetX)*nTileWidth, (fPlayerPosY - fOffsetY)*nTileHeight-8);
			}else{
				draw_image(mariojump_image, nTileWidth, 24, (fPlayerPosX - fOffsetX)*nTileWidth, (fPlayerPosY - fOffsetY)*nTileHeight-8);
			}
		}else{
			if(nDirModY){
				draw_image_flipped_h(mariostill_image, nTileWidth, 24, (fPlayerPosX - fOffsetX)*nTileWidth, (fPlayerPosY - fOffsetY)*nTileHeight-8);
			}else{
				draw_image(mariostill_image, nTileWidth, 24, (fPlayerPosX - fOffsetX)*nTileWidth, (fPlayerPosY - fOffsetY)*nTileHeight-8);
			}
		}
						
		push_frame();
}
