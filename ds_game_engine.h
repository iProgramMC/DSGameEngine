#ifndef DS_GAME_ENGINE_INCL
#define DS_GAME_ENGINE_INCL

#define WHITE 0xFFFF
#define BLACK 0x0000
#define SKY 0xEEEF
#define BLUE 0xFC00
#define MAGENTA 0xFC1F
#define CYAN 0xFFE0


#include <nds.h>

/*
int round(float f){
	float decpt = f - (int)f;
	if(decpt < 0.5f){
		return (int)f;
	}else{
		return (int)f+1;
	}
}*/

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
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = colour;
			}
		}
	}
}

void plot_pixel (int x, int y, short colour){
	
			if((x) >= 0 && (x) < 256 && (y) >= 0 && (y) < 192){
				screenbuffer_maindisplay[y*screen_width+x] = colour;
			}
}

void draw_line (int x1, int y1, int x2, int y2, short col){
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			plot_pixel(x, y, col);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				plot_pixel(x, y, col);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			plot_pixel(x, y, col);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				plot_pixel(x, y, col);
			}
		}
}

void draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, short colour){
	draw_line (x1, y1, x2, y2, colour);
	draw_line (x1, y1, x3, y3, colour);
	draw_line (x3, y3, x2, y2, colour);
}

void fill_triangle (int x1, int y1, int x2, int y2, int x3, int y3, short col){
	/* Ripped from OneLoneCoder/ConsoleGameEngine */
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			plot_pixel(x, y, col);
			draw_line(x, y, x3, y3, col);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				plot_pixel(x, y, col);
			draw_line(x, y, x3, y3, col);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			plot_pixel(x, y, col);
			draw_line(x, y, x3, y3, col);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				plot_pixel(x, y, col);
				draw_line(x, y, x3, y3, col);
			}
		}
}

void draw_image_sizable(short* image, int w, int h, int dx, int dy, int dw, int dh){
	/*float pixel_width = dw / w;
	float pixel_height= dh / h;
	
	for(int y = 0; y < dh; y++){
		for(int x = 0; x < dw; x++){
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				int src_y = round(y * pixel_height);
				int src_x = round(x * pixel_width);
				screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[(src_y*w+src_x)];
			}
		}
	}*/
	
	int w2 = dw, w1 = w, h2 = dh, h1 = h;
	
    // EDIT: added +1 to account for an early rounding problem
    int x_ratio = (int)((w1<<16)/w2) +1;
    int y_ratio = (int)((h1<<16)/h2) +1;
	
    int x2, y2 ;
    for (int i=0;i<h2;i++) {
        for (int j=0;j<w2;j++) {
            x2 = ((j*x_ratio)>>16) ;
            y2 = ((i*y_ratio)>>16) ;
			if((j+dx) >= 0 && (j+dx) < 256 && (i+dy) >= 0 && (i+dy) < 192){
				screenbuffer_maindisplay[(i+dy)*screen_width+(j+dx)] = image[(y2*w1)+x2] ;
			}
        }                
    }            
}

void draw_image(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				if(image[y*w+x] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[y*w+x];}
			}
		}
	}
}

void draw_image_wsr(short* image, int w, int h, int dx, int dy, int sx, int sy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				if(image[(sy+y)*w+(sx+x)] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[(sy+y)*w+(sx+x)];}
			}
		}
	}
}

void draw_image_flipped_v(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				if(image[(h-y-1)*w+x] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[(h-y-1)*w+x];}
			}
		}
	}
}
void draw_image_flipped_hv(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				if(image[(h-y)*w+(w-x-1)] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[(h-y)*w+(w-x-1)];}
			}
		}
	}
}
void draw_image_flipped_h(short* image, int w, int h, int dx, int dy){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			if((x+dx) >= 0 && (x+dx) < 256 && (y+dy) >= 0 && (y+dy) < 192){
				if(image[y*w+(w-x-1)] == -993){}else{screenbuffer_maindisplay[(dy+y)*screen_width+(x+dx)] = image[y*w+(w-x-1)];}
			}
		}
	}
}

void OnUserCreate();
void OnUserUpdate();

void make_btm_screen() {consoleSelect(&bottomScreen);}
void printchar(char x, char y, char c) { printf("\x1b[%d;%dH%c", y, x, c); }

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


 
/*int dx = x2 - x1;
	int dy = y2 - y1;
		
	if (dx >= dy){
		float y = 0.0f;
		if(x1 < x2){
			for (int x = x1; x <= x2; x++) {
				y = y1 + dy * (x - x1) / dx;
				plot_pixel(x, y, colour);
			}
		}else{
			for (int x = x2; x <= x1; x++) {
				y = y1 + dy * (x - x1) / dx;
				plot_pixel(x, y, colour);
			}
		}
	}else{
		float x = 0.0f;
		if(dx == 0){
			if(y1 < y2){
				for (int y = y1; y <= y2; y++) {
					plot_pixel(x1, y, colour);
				}
			}else{
				for (int y = y2; y <= y1; y++) {
					plot_pixel(x2, y, colour);
				}
			}
		}else{
			if(y1 < y2){
				for (int y = y1; y <= y2; y++) {
					x = x1 + dx * (y - y1) / dy;
					plot_pixel(x, y, colour);
				}
			}else{
				for (int y = y2; y <= y1; y++) {
					x = x1 + dx * (y - y1) / dy;
					plot_pixel(x, y, colour);
				}
			}
		}
	}*/
#endif
