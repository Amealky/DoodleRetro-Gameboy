#include <stdio.h>
#include <gb/gb.h>
#include <rand.h>

#include "graphics.h"

typedef struct Element{
	int etat;
	int PosX, PosY;
	UINT8 t;
} Element;

Element player;
Element platform[10];

UINT8 loose = 0;

int i;
UINT8 onJump;
UINT8 oldPosY;
UINT8 random;

UINT8 highscore = 0;

UINT8 z;
UINT8 e;

UINT8 blink;

void JumpSound(void)
{
    NR10_REG = 0x34U; // 4th bit indicates if freq increases or decreases
              // bits 5th-7th indicate sweep delay
    NR11_REG = 0x80U;
    NR12_REG = 0xF0U;
    NR13_REG = 0x0AU; //lowest 8 frequency bits
    NR14_REG = 0xC6U; // first 3 bits are the 3 highest bits of freq
    NR51_REG |= 0x11;
}

void DeathSound()
{
	NR41_REG = 0x00;//0x1FU;
    NR42_REG = 0xE1;//0xF7U;
    NR43_REG = 0x22;//0x24U;
    NR44_REG = 0xC3;//0x80U;
    NR51_REG = 0x88U;
}

/********************************************************************
 * ================================================================ *
*********************************************************************/

/***
 * INIT FUNCTIONS
*/
void initGamesDatas();

/***
 * SCREEN FUNCTIONS
*/
void showBootScreen();
void showStartScreen();
void setupGameScreen();
void showGameScreen();
void showGameOverScreen();

/***
 * DRAW FUNCTIONS
*/
void drawPlatform();

/***
 * CHORE FUNCTIONS
*/
void checkPlayer();
void jumpPlayer();


/********************************************************************
 * ================================================================ *
*********************************************************************/

/***
 * INIT FUNCTIONS IMPL
*/
void initGameDatas() {
	player.etat = 1;
	player.t = 0;
	onJump = 0;
}


/***
 * SCREEN FUNCTIONS IMPL
*/
void showBootScreen() {
	DISPLAY_OFF;

	set_bkg_data(0, 15,logoBkg);	
	set_bkg_tiles(0, 0, 20, 20, logoDev);
	
	SHOW_BKG;
	DISPLAY_ON;

		
	delay(2000);
	wait_vbl_done();
}

void showStartScreen()
{
	DISPLAY_OFF;
	set_bkg_data(0, 20, doodleTitleBkg);
	set_bkg_tiles(0,0,20,20, doodleTitleTiles);
	DISPLAY_ON;

	do{
	//Wait the player to push start
	}while(joypad() != J_START);

}

void setupGameScreen() {
	loose = 0;

	set_bkg_data(0, 2, doodleWalls);
	set_bkg_tiles(0, 0, 20, 35, doodleGameBkg);

	
	set_sprite_data(player.t, 1, playerSprite);
	set_sprite_tile(player.t, player.t);

	
	for(i = 0; i < 10; i++) {
		platform[i].t = i+1;
		platform[i].etat = 1;
		set_sprite_data(platform[i].t, 1, platformSprite);
		set_sprite_tile(platform[i].t, platform[i].t);
		
		do {
			random = rand() % 140;
			platform[i].PosY = random;

			if(platform[i].PosY <= 0) {
				platform[i].PosY *= -1;
			}
		
		} while(platform[i].PosY >= 140);
		
		do {
			random = rand() % 18;
			platform[i].PosX = 8 * random;
			
			if(platform[i].PosX <= 8) {
				platform[i].PosX *= 8;
			}
		
		} while(platform[i].PosX <= 8);
		move_sprite(platform[i].t, platform[i].PosX, platform[i].PosY);

	}

	
	player.PosY = 100;
	player.PosX = 80;
	move_sprite(player.t, player.PosX, player.PosY);

	
	platform[0].PosY = 120;
	platform[0].PosX = 80;
	move_sprite(platform[0].t, platform[0].PosX, platform[0].PosY);
}

void showGameScreen() {
	SHOW_SPRITES;

	while(loose == 0) {	
		drawPlatform();
		
		if(joypad()==J_RIGHT) {
			player.PosX +=1;
			set_sprite_prop(0, !S_FLIPX);
		}

		if(joypad()==J_LEFT) {
			player.PosX-=1;
			set_sprite_prop(0, S_FLIPX);
		}

		checkPlayer();
		jumpPlayer();

		move_sprite(player.t, player.PosX, player.PosY);
		delay(5);
	}
	
}

void showGameOverScreen()
{

	DeathSound();
	DISPLAY_OFF;
	move_bkg(0, 0);
	set_bkg_data(0, 16, gameOverBkg);
	
	set_bkg_tiles(0, 0, 20, 20, gameOverTiles);

	player.PosX = 240;
	move_sprite(player.t, player.PosX, player.PosY);

	DISPLAY_ON;
	waitpad(J_START);
}


/***
 * DRAW FUNCTIONS IMPL
*/
void drawPlatform()
{
	for(i = 0; i < 10; i++)
	{
		if(platform[i].etat == 0)
		{
			
			platform[i].t = i+1;
			platform[i].etat = 1;
			set_sprite_data(platform[i].t, 1, platformSprite);
			set_sprite_tile(platform[i].t, platform[i].t);

			platform[i].PosY = 0; 

			do{
				random = rand() % 18;
				platform[i].PosX = 8 * random;
				if(platform[i].PosX <= 8) {
					platform[i].PosX *= 8;
				}

					
			}while(platform[i].PosX <= 8);
		
			move_sprite(platform[i].t, platform[i].PosX, platform[i].PosY);
				
		}
		if(platform[i].PosY >= MAXWNDPOSY)
		{
			platform[i].etat = 0;
		}
	}

}


/***
 * CHORE FUNCTIONS IMPL
*/
void checkPlayer()
{
	if(onJump == 0)
	{
		for(i = 0; i < 10; i++)
		{
			if(player.PosY+8 >= platform[i].PosY && player.PosY+8 <= platform[i].PosY+3)
			{
				if((player.PosX >= platform[i].PosX-4 && player.PosX <= platform[i].PosX+4))
				{
					onJump = 1;
					JumpSound();
					oldPosY = player.PosY;
				}
			}
		}
	}

	if(player.PosX >= MAXWNDPOSX-14)
	{
		player.PosX = MAXWNDPOSX-14;
		player.PosY = player.PosY;
		move_sprite(player.t, player.PosX, player.PosY);
	}
	if(player.PosX <= 14)
	{
		player.PosX = 14;
		player.PosY = player.PosY;
		move_sprite(player.t, player.PosX, player.PosY);
	}

	if(player.PosY < MAXWNDPOSY / 2)
	{
		for(i = 0; i < 10; i++)
		{
			platform[i].PosY++;  
			move_sprite(platform[i].t, platform[i].PosX, platform[i].PosY);

		}
		z--;
		move_bkg(0, z);
	}
	if(player.PosY >= MAXWNDPOSY)
	{
		loose = 1;
	}
}

void jumpPlayer()
{
	if(onJump == 1)
	{
		player.PosY--;
		if(player.PosY <= oldPosY-56)
		{
			onJump = 0;
		}
	}
	else
	{
		player.PosY+=2;
		if(player.PosY >= 150)
		{
			player.PosY = 0;
		}
	}

}


int main(){

	SPRITES_8x8;
  	NR52_REG = 0xF8U;
    NR51_REG = 0x00U;
    NR50_REG = 0x77U;


	initGameDatas();

	showBootScreen();

	while(1) {

		showStartScreen();

		setupGameScreen();

		showGameScreen();

		showGameOverScreen();
	}

	return 0;

}