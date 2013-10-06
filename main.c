#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "utils.h"
#include "global.h"
#include "config.h"
#include "properties.h"
#include "board.h"
#include "players.h"
#include "GUI.h"
#include "cpl.h"
#include "print_board.h"


int main () {
	srand (time (NULL));
	int i,d;
	char *config;
	init_GUI ();
	config=enter_config ();
	load_config (config);
	load_properties (config);
	load_board (config);
	enter_players ();
	print_board (config);
	show_owners ();
//	p[0].money=-5540;
	for (i=0;i<players_num;i++) {
		print_token (config,0,i);
		display_money (i);
	}
	/*
	for (i=0;i<28;i++) {
		for (d=0;d<players_num;d++) {
			p[d].prop[i][0]=0;
		}
//		p[0].prop[i][0]=1;
//		p[0].prop[i][1]=4;
	}
	*/
	display_houses (config);
	SDL_Flip(screen);
	current=0;
	previous=-1;
	while (1) {
		if (current==previous) {
			printf ("WINNER!!!");
			quit ();
		}
		display_money (current);
		turn (current,config);
		previous=current;
		do {
			current++;
			if(current==players_num)
				current=0;
		} while (p[current].bankrupt);
		display_money (previous);
		
			
	}
	sleep (3);
	quit ();
	while (1) {
	}
}

