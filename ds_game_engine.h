#ifndef DS_GAME_ENGINE_INCL
#define DS_GAME_ENGINE_INCL

#include <nds.h>

short screenbuffer_maindisplay[49152];
short screen_width = 256;
short screen_height = 192;

uint32 input_keys = 0;
uint32 input_keysUp = 0;
uint32 input_keysDown = 0;
uint32 input_keysHeld = 0;

touchPosition touch;
PrintConsole bottomScreen;

bool get_key_state(uint32 button){
	uint32 s = input_keys & button;
	if(s){return true;}else{return false;}
}
bool get_key_down_state(uint32 button){
	uint32 s = input_keysDown & button;
	if(s){return true;}else{return false;}
}
bool get_key_up_state(uint32 button){
	uint32 s = input_keysUp & button;
	if(s){return true;}else{return false;}
}
bool get_key_held_state(uint32 button){
	uint32 s = input_keysHeld & button;
	if(s){return true;}else{return false;}
}

void push_frame(){
	dmaCopy(&screenbuffer_maindisplay, BG_GFX, 49152*2);
}

void clear_screenbuffer(short colour){
	for(int x = 0; x < 49152; x+=1){
		screenbuffer_maindisplay[x] = colour;
	}
}

void fill_rectangle(short colour, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if(!(x<0||x>255||y<0||y>191)){
				if(image[y*w+x] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = colour;}
			}
		}
	}
}

void draw_image(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if(!(x<0||x>255||y<0||y>191)){
				if(image[y*w+x] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[y*w+x];}
			}
		}
	}
}
void draw_image_flipped_v(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if(!(x<0||x>255||y<0||y>191)){
				if(image[(h-y-1)*w+x] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[(h-y-1)*w+x];}
			}
		}
	}
}
void draw_image_flipped_hv(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if(!(x<0||x>255||y<0||y>191)){
				if(image[(h-y)*w+(w-x-1)] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[(h-y)*w+(w-x-1)];}
			}
		}
	}
}
void draw_image_flipped_h(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if(!(x<0||x>255||y<0||y>191)){
				if(image[y*w+(w-x-1)] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[y*w+(w-x-1)];}
			}
		}
	}
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	
	
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	
	//consoleInit(&topScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	consoleInit(&bottomScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	make_btm_screen();
	
	OnUserCreate();
	
	clear_screenbuffer(0xEEEE);

	while(1) {
		touchRead(&touch);
		scanKeys();
		
		input_keys = keysCurrent();
		input_keysUp = keysUp();
		input_keysDown = keysDown();
		input_keysHeld = keysHeld();
		
		OnUserUpdate();
		swiWaitForVBlank();
	}

	return 0;
}


 

#endif
