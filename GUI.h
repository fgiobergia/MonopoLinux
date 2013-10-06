#define	WIDTH	900
#define	HEIGHT	598

SDL_Event a;
SDL_Surface *screen;
TTF_Font *verdana;

void quit (void) {
	SDL_Quit ();
	TTF_Quit ();
	exit (0);
}

void pixel (int y, int x, int c) {
	unsigned int *p;
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);
//	c=SDL_MapRGB(screen->format,pixel[0],pixel[1],pixel[2]);
	p=(unsigned int*)screen->pixels+(y*screen->pitch/4)+x;
	*p=c;
	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);
}

void _clear (void) {
	int i,d;
	for (i=0;i<499;i++) {
		for (d=598;d<900;d++) {
			pixel (i,d,0xFFFFFF);
		}
	}
	SDL_Flip (screen);
}

int display (int y, int x, char *string, int c) {
	int r;
	SDL_Color color;
	SDL_Rect dst;
	SDL_Surface *txt;
	r=-1;
	if (strlen(string)) {
		color.r=(c&0xFF0000)>>16;
		color.g=(c&0x00FF00)>>8;
		color.b=(c&0x0000FF);
		txt = TTF_RenderText_Solid (verdana,string,color);
		dst.x = x;
		dst.y = y;
		dst.w = txt->w;
		dst.h = txt->h;
//		if (txt->w<400) {
		SDL_BlitSurface(txt,NULL,screen,&dst);
		r = txt->w;
//			return 1;
//		}
//		else {
//			return 0;
//		}
	}
	SDL_Flip(screen);
	return r;
}

void input (int y, int x, int height, int width, int c) {
	int i,d;
	for (d=0;d<width;d++) {
		pixel (y,x+d,c);
		pixel (y+height,x+d,c);
	}
	for (i=0;i<=height;i++) {
		pixel (y+i,x,c);
		pixel (y+i,x+width,c);
	}
}

void rectangle (int y1, int x1, int y2, int x2, int c) {
	int i,d;
	for (i=y1;i<y2;i++) 
		for (d=x1;d<x2;d++)
			pixel (i,d,c);
}

// controlla che tutti i file richiesti
// esistano davvero
int check_config (char *s) {
	FILE *fp;
	int i;
	char str[1024];
	char list[13][15]={	{"board.ppm"},
				{"board.txt"},
				{"chance.ppm"},
				{"chest.ppm"},
				{"config.txt"},
				{"electric.ppm"},
				{"income.ppm"},
				{"prop.ppm"},
				{"properties.txt"},
				{"station.ppm"},
				{"super.ppm"},
				{"water.ppm"},
				{"token.ppm"}
			  };
	for (i=0;i<13;i++) {
		sprintf (str,"%s/%s",s,list[i]);
		fp = fopen (str,"r");
		if (fp==NULL) {
			display (75,10,"Invalid config :(",0xFF0000);
			return 0;
		}
		fclose (fp);
	}
	return 1;
}

char *enter_config () {
	int i,d,len,w;
	char *str;
	w=0;
	str = malloc (1);
	len=1;
	SDL_Flip(screen);
	for (i=0;i<HEIGHT;i++) {
		for (d=0;d<WIDTH;d++) {
			pixel (i,d,0xD4FCE4);
		}
	}
	display (10,10,"Enter the name (or path) of the config directory:",0x000000);
	rectangle (26,11,44,890,0xDEDEDE);
	rectangle (51,11,68,65,0xDEDEDE);
	input (25,10,18,880,0);
	input (50,10,18,55,0);
	display (52,22,"Enter",0x000000);
	for (;;) {
		while (SDL_PollEvent (&a)) {
			switch (a.type) {
				case SDL_KEYDOWN:
					// carattere da ' ' a '~'
					if (a.key.keysym.unicode>0x1f && a.key.keysym.unicode<0x80 && w<865) {
						str=realloc(str,++len);
						str[len-2]=a.key.keysym.unicode;
						str[len-1]=0;
					}
					// Backspace
					if (a.key.keysym.unicode==0x08) {
						if (len>1) {
							str[strlen(str)-1]=0;
							len--;
						}
					}
					if (a.key.keysym.unicode==0x0d) {
						if (check_config (str))
							return str;
					}
					rectangle (26,11,44,890,0xDEDEDE);
					input (25,10,18,880,0);
					w = display (27,12,str,0x000000);
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (a.button.button==1 && (a.button.x>10 && a.button.x<65) && (a.button.y>50 && a.button.y<68)) {
						if (check_config (str)) 
							return str;
					}
					break;
				case SDL_QUIT:
					quit ();
					break;
				default:
					break;
			}
		}
	}
}

// #D4FCE4

int enter_color (int old) {
	int i,d;
	int r,g,b;
	r=(old&0xFF0000)>>16;
	g=(old&0x00FF00)>>8;
	b=(old&0x0000FF);
//	SDL_Event x;
//	SDL_Surface *s;
//	SDL_Init (SDL_INIT_VIDEO);
//	SDL_putenv ("SDL_VIDEO_CENTERED=center");
//	SDL_WM_SetCaption ("Choose a color",NULL);
//	s=SDL_SetVideoMode (286,296,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	rectangle (0,0,HEIGHT,WIDTH,0xD4FCE4);
	rectangle (151,307,447,307+286,0xD4FCE4);
	input (150,306,296,286,0);
	rectangle (256+151,0+307,296+151,286+307,0xD4FCE4);
	input (256+151,0+306,39,286,0);
	input (150,306,257,257,0);
	// i: red, d: green
	for (i=0;i<256;i++) {
		for (d=0;d<256;d++)
			pixel (i+151,d+307,(i<<16)|(d<<8)|0x7F);
	}
	for (i=0;i<38;i++) {
		for (d=0;d<285;d++) {
			pixel (i+151+257,d+307,(r<<16)|(g<<8)|(b));
		}
	}
	for (i=0;i<256;i++) {
		for (d=0;d<28;d++) {
			pixel (151+i,308+256+d,(r<<16)|(g<<8)|i);
		}
	}
	SDL_Flip (screen);
	display (151+256+12,307+55,"Click here to choose this color",~((r<<16)|(g<<8)|(b)));
	for (;;) {
	while (SDL_PollEvent (&a)) {
		switch (a.type) {
			case SDL_QUIT:
				quit ();
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (a.button.button==1) {
					// L'utente ha clickato nel quadrato
					// dove sono definiti R e G. Viene mostrata
					// nella colonna a destra la sfumatura di blu
					if ((a.button.x>307&&a.button.x<307+256) && (a.button.y>151 && a.button.y<256+151)) {
						r=a.button.y-151;
						g=a.button.x-307;
						b=0x7F;
						for (i=0;i<256;i++) {
							for (d=0;d<28;d++) {
								pixel (151+i,308+256+d,(r<<16)|(g<<8)|i);
							}
						}
					}
					if ((a.button.x>307+256 && a.button.x<900-307) && (a.button.y>151 && a.button.y<151+256)) {
						b=a.button.y-151;
					}
					if ((a.button.x>307 && a.button.x<307+286) && (a.button.y>151+256 && a.button.y<151+256+40)) {
						return (r<<16)|(g<<8)|(b);
					}
					for (i=0;i<38;i++) {
						for (d=0;d<285;d++) {
							pixel (i+151+257,d+307,(r<<16)|(g<<8)|(b));
						}
					}
					SDL_Flip (screen);
					display (151+256+12,307+55,"Click here to choose this color",~((r<<16)|(g<<8)|(b)));
				}
				break;
			default:
				break;
		}		
		
		if (a.type==SDL_QUIT) {
//			SDL_Quit ();
			return 0;
		}
	}
	}
}

void us_enter_players (int *list, int *ai, int *cols) {
	int i,d,k,j;
	char str[128];
	// Viene pulito lo schermo
	rectangle (0,0,HEIGHT,WIDTH,0xD4FCE4);
	for (i=0;i<HEIGHT;i++) {
		pixel (i,0,0);
		pixel (i,WIDTH/2,0);
		pixel (i,WIDTH-1,0);
	}
	for (d=0;d<WIDTH;d++) {
		pixel (0,d,0);
		pixel (HEIGHT/4,d,0);
		pixel (HEIGHT/2,d,0);
		pixel (3*HEIGHT/4,d,0);
		pixel (HEIGHT-1,d,0);
	}
	// colonne
	for (i=0;i<2;i++) {
		// righe
		for (d=0;d<4;d++) {
			// Mostra la checkbox per scegliere
			// se l'utente dev'essere selezionato
			// o meno
			input (40+150*d,40+i*450,15,15,0);
			if (list[i*4+d]) {
				rectangle (40+3+150*d,40+3+i*450,40+3+150*d+10,40+3+10+i*450,0);
				rectangle (11+150*d,376+i*450,75+150*d,375+i*450+65,cols[i*4+d]);
			}
			else {
				rectangle (d*150+41,i*450+41,d*150+40+15,i*450+40+15,0xD4FCE4);
				for (j=0;j<64;j++) {
					for (k=0;k<64;k++) {
						pixel (11+150*d+j,376+i*450+k,((j+k)%2)?0:0xD4FCE4);
					}
				}
			}
			sprintf (str,"Enable Player %d",i*4+d+1);
			display (40+150*d,65+i*450,str,0);
			// Mostra le scelte per impostare il
			// giocatore come AI o come Human
			
			// AI
			if (ai[i*4+d]) {
				rectangle (95+150*d+3,40+3+i*450,105+150*d+3,50+3+i*450,0);
			}
			else {
				rectangle (95+150*d+3,40+3+i*450+80,105+150*d+3,50+3+i*450+80,0);
			}
			input (95+150*d,40+i*450,15,15,0);
			display (95+150*d,65+i*450,"AI",0);
			input (95+150*d,120+i*450,15,15,0);
			display (95+150*d,145+i*450,"Human",0);
			// Mostra il riquadro per cambiare colore
			// della pedina
			input (10+150*d,375+i*450,65,65,0);
		}
	}
	for (i=0,d=0,k=0;i<8;i++) {	
		d+=list[i];
		k+=(!ai[i] && list[i]);
	}
	input (577,409,20,80,0);
	if (d>1 && k) {
		rectangle (578,410,597,489,0xFFCC33);
	}
	else {
		rectangle (578,410,597,489,0xEDEDED);
	}
	display (580,430,"START",0);
		
	SDL_Flip (screen);
}

void print_dot (int y, int x) {
	int i,d;
	for (i=1;i<=3;i++) {
		pixel (y,x+i+1,0);
		pixel (y+4,x+i+1,0);
	}
	for (i=1;i<=3;i++) {
		for (d=1;d<=5;d++) {
			pixel (y+i,x+d,0);
		}
	}
}

int show_dice (int y, int x) {
	int n;
	n=(rand () % 6)+1;
//	n=3;
	rectangle (y,x,y+45,x+45,0xFFFFFF);
	input (y,x,45,45,0);
	switch (n) {
		case 1:
			print_dot (y+20,x+20);
			break;
		case 2:
			print_dot (y+33,x+7);
			print_dot (y+7,x+33);
			break;
		case 3:
			print_dot (y+33,x+7);
			print_dot (y+7,x+33);
			print_dot (y+20,x+20);			
			break;
		case 4:
			print_dot (y+33,x+7);
			print_dot (y+7,x+33);
			print_dot (y+33,x+33);
			print_dot (y+7,x+7);
			break;
		case 5:
			print_dot (y+33,x+7);
			print_dot (y+7,x+33);
			print_dot (y+33,x+33);
			print_dot (y+7,x+7);
			print_dot (y+20,x+20);
			break;
		case 6:
			print_dot (y+33,x+7);
			print_dot (y+7,x+33);
			print_dot (y+33,x+33);
			print_dot (y+7,x+7);
			print_dot (y+20,x+7);
			print_dot (y+20,x+33);
			break;			
		default:
			break;
	}
	return n;
}
	

// Lancia i dadi per ogni giocatore
// per determinare l'ordine d'inizio
// TODO: trovare un nome migliore per questa funzione xD
int randomly_choose_order (int *list, int *ai, int *cols)  {
	unsigned char props;
	int i,d,j,k,p,tmp;
	int res[8];
	int list2[8],ai2[8],cols2[8];
	memset (res,0,sizeof(res));
	for (i=0,p=0;i<2;i++) {
		for (d=0;d<4;d++) {
			if (list[i*4+d]) {
//				rectangle (85+d*150,320+i*450,140+d*150,375+i*450,0xFFFFFF);
//				input (85+d*150,320+i*450,55,55);
				for (k=0;k<2;k++) {
					for (j=0;j<7;j++) {
						show_dice (90+d*150,325+i*450+k*60);
						SDL_Flip (screen);
//						usleep (100000);	DA TOGLIERE QUESTO COMMENTOOOOO
					}
					res[i*4+d]+=show_dice (90+d*150,325+i*450+k*60);
//					SDL_Flip (screen);		ANCHE QUESTOOOO
				}
				list2[p]=res[i*4+d];
				ai2[p]=ai[i*4+d];
				cols2[p]=cols[i*4+d];
				p++;
			}
		}
	}
	for (i=1;i<p;i++) {
		for (d=0;d<i;d++) {
			if (list2[d]<list2[i]) {
				tmp=list2[d];
				list2[d]=list2[i];
				list2[i]=tmp;
				
				tmp=ai2[d];
				ai2[d]=ai2[i];
				ai2[i]=tmp;
				
				tmp=cols2[d];
				cols2[d]=cols2[i];
				cols2[i]=tmp;
			}
		}
	}
	players_num = p;
	props=(prop_values_sub)<<7 | prop_at_beginning;
	return new_players (p,cols2,ai2,money_at_start,props);
}

void enter_players () {
	int i,d,j,k;
	int list[8];
	int ai[8];
	int cols[8];
	// 1  -> enabled
	// 0  -> disabled
	for (i=0;i<8;i++)
		list[i]=1;
	// 0 -> human
	// 1 -> ai
	memset (ai,0,sizeof(ai));
	cols[0]=0xFF0000; // rosso
	cols[1]=0xFF8000; // arancione
	cols[2]=0xFFFF00; // giallo
	cols[3]=0x00FF00; // verde
	cols[4]=0x007FFF; // azzurro
	cols[5]=0x4B0082; // indaco
	cols[6]=0x8F00FF; // viola
	cols[7]=0x8B4513; // marrone
//	enter_color (0,0,0);
	us_enter_players (list,ai,cols);
	for (;;) {
		while (SDL_PollEvent(&a)) {
			switch (a.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (a.button.button==1) {
						j=a.button.y/150;
						k=a.button.x/450;

						// è stato premuto un button per disattivare
						// l'utente
						if ((a.button.y>j*150+40 && a.button.y<j*150+55) && (a.button.x>k*450+40 && a.button.x<k*450+55)) {	
							// Se l'utente è attivato, viene disattivato
							if (list[k*4+j]) {
								rectangle (j*150+41,k*450+41,j*150+40+15,k*450+40+15,0xD4FCE4);
								for (i=0;i<64;i++) {
									for (d=0;d<64;d++) {
										pixel (11+150*j+i,376+k*450+d,((i+d)%2)?0:0xD4FCE4);
									}
								}
								list[k*4+j]=0;
							}
							// se l'utente è disattivato, viene attivato
							else {
								rectangle (40+3+150*j,40+3+k*450,40+3+150*j+10,40+3+10+k*450,0);
								rectangle (11+150*j,376+k*450,75+150*j,375+k*450+65,cols[k*4+j]);
								list[k*4+j]=1;
							}
//							SDL_Flip (screen);
						}
						// Viene premuto il pulsante per impostare
						// il giocatore come AI
						if ((a.button.y>j*150+95 && a.button.y<j*150+95+15) && (a.button.x>k*450+40 && a.button.x<k*450+55) && (list[k*4+j])) {
							ai[k*4+j]=1;
							rectangle (95+150*j+3,40+3+k*450,105+150*j+3,50+3+k*450,0);
							rectangle (95+150*j+3,40+3+k*450+80,105+150*j+3,50+3+k*450+80,0xD4FCE4);
//							SDL_Flip (screen);
						}
						// Il giocatore viene impostato come Human
						if ((a.button.y>j*150+95 && a.button.y<j*150+95+15) && (a.button.x>k*450+120 && a.button.x<k*450+135) && (list[k*4+j])) {
							ai[k*4+j]=0;
							rectangle (95+150*j+3,40+3+k*450,105+150*j+3,50+3+k*450,0xD4FCE4);
							rectangle (95+150*j+3,40+3+k*450+80,105+150*j+3,50+3+k*450+80,0);
//							SDL_Flip (screen);
						}
						// e' stato clickato il pulsante per 
						// cambiare colore
						if ((a.button.y>j*150+10 && a.button.y<j*150+75) && (a.button.x>k*450+375 && a.button.x<k*450+440) && (list[k*4+j])) {
							cols[k*4+j]=enter_color (cols[k*4+j]);
							us_enter_players (list,ai,cols);
						}
						for (i=0,d=0,k=0;i<8;i++) {	
							d+=list[i];
							k+=(!ai[i] && list[i]);
						}
						if ((a.button.y>598-20) && (a.button.x>410 && a.button.x<490) && (d>1 && k)) {	
							rectangle (577,409,597,490,0xD4FCE4);
							for (i=0;i<20;i++) {
								pixel (i+577,450,0);
							}
							SDL_Flip (screen);
							randomly_choose_order (list,ai,cols);
							sleep (1);
							return ;
						}
						input (577,409,20,80,0);
						if (d>1 && k) {
							rectangle (578,410,597,489,0xFFCC33);
						}
						else {
							rectangle (578,410,597,489,0xEDEDED);
						}
						SDL_Flip (screen);
						display (580,430,"START",0);
					}
					break;
				case SDL_QUIT:
					quit ();
					break;
				default:
					break;
			}
		}
	}	
}

void display_money (int id) {
	int y,x,i,d;
	char str[1024];
	y=id%4;
	x=id/4;
	rectangle (498+y*25,598+x*150,498+y*25+25,598+x*150+150,(id==current)?0xFFCC33:0xFFFFFF);
	input (498+y*25+5,598+5+x*150,15,15,0);
	rectangle (498+y*25+6,598+6+x*150,498+y*25+6+14,598+6+x*150+14,p[id].token);
	sprintf (str,"Player %d ~ %d $",id+1,p[id].money);
	display (498+y*25+5,598+5+x*150+25,str,0);
	// Viene mostrato che il giocatore
	// è in prigione
	if (p[id].inprison>0) {
		for (i=0;i<14;i+=2) {
			for (d=0;d<14;d++) {
				pixel (498+y*25+6+d,598+6+x*150+i,0x888888);
			}
		}
	}
		
}

void update_owner (int pos, int owner) {
	int col,k,l;
	// se owner è -1, allora
	// il terreno torna alla
	// banca -> colore D4FCE4
	if (owner==-1) {
		col=0xD4FCE4;
	}
	else {
		col=p[owner].token;
	}		
	if (pos<10) {
		for (k=0;k<5;k++) {
			for (l=0;l<(40+k*2);l++) {
				pixel (522+k-8,76+(9-pos)*49+(4-k)+3+l,col);
			}
		}
	}
	else if (pos<20) {
		for (k=0;k<5;k++) {
			for (l=0;l<(40+(4-k)*2);l++) {
				pixel (76+52+49*(9-(pos-9))+l+k,3+76+k,col);
			}
		}
	}
	else if (pos<30) {
		for (k=0;k<5;k++) {
			for (l=0;l<(40+(4-k)*2);l++) {
				pixel (79+k,76+((pos-19))*49+k+3+l-49*2,col);
			}
		}
	}
	else {
		for (k=0;k<5;k++) {
			for (l=0;l<(40+k*2);l++) {
				pixel (76+3+49*((pos-29))+l+(4-k)-49*2,3+76+435+k,col);
			}
		}
	}
}

void show_owners () {
	int i,d,j,k,l;
	for (i=0;i<players_num;i++) {
		for (d=0;d<28;d++) {
			// Se il giocatore possiede
			// quel terreno, viene ricercato
			// in che posizione del tabellone
			if (p[i].prop[d][0]) {
				for (j=0;j<40;j++) {
					if (b[j].type=='P' && b[j].v==d) {
						update_owner (j,i);
						// Trovato
						// Ora viene visualizzato
						// il rettangolo

					}
				}
			}
		}
	}
	SDL_Flip(screen);
}

// Questa funzione aggiorna una proprietà ad o da "ipotecata"
void update_mortgage (char *dir, int pos, int m) {
	// se m==1 allora la proprietà è stata
	// ipotecata, viene colorata la casella
	// di rosso.
	int x,y,i,d,j,k;
	unsigned int c[76][48],tmp[3];
	char line[1024];
	FILE *fp;
	if (m==1) {
		for (i=0;i<76;i++) {
			for (d=0;d<48;d++) {
				c[i][d]=0xB7410E;
			}
		}
	}
	else {
		switch (t[b[pos].v].type) {
			case 'P':
				sprintf (line,"%s/prop.ppm",dir);
				break;
			case 'S':
				sprintf (line,"%s/station.ppm",dir);
				break;
			case 'U':
				switch (t[b[pos].v].col) {
					case 0:
						sprintf (line,"%s/electric.ppm",dir);
						break;
					case 1:
						sprintf (line,"%s/water.ppm",dir);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		fp=fopen (line,"r");
		if (fp==NULL)
			return ;
		while (1) {
			fgets (line,sizeof(line),fp);
			if (sscanf (line,"%d %d",&x,&y)==2) {
				fgets (line,sizeof(line),fp);
				break;
			}
			if (feof (fp)) {
				return ;
			}
		}
		// legge le coordinate, per "sicurezza"
		if (x!=48 || y!=76)
			return ;
		for (i=0;i<y;i++) {
			for (d=0;d<x;d++) {
				fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
				// se il pixel è bianco, lo colora del colore
				// del terreno desiderato
				if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255 && t[b[pos].v].type=='P') {
					tmp[0]=((t[b[pos].v].col)&0xFF0000)>>16;
					tmp[1]=((t[b[pos].v].col)&0x00FF00)>>8;
					tmp[2]=((t[b[pos].v].col)&0x0000FF);
				}
				c[i][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
			}
		}
		fclose (fp);
	}
	if (pos<10) {
		for (i=520,j=0;j<76;i++,j++) {
			for (d=471-(49*(pos-1)),k=0;k<48;d++,k++) {
				pixel (i,d,c[j][k]);
			}
		}
	}
	// dalla 10 alla 20 le caselle
	// sono stampate girandole di 90°
	else if (pos<20) {
		for (j=0,d=77;j<76;j++,d--) {
			for (k=0,i=(471-(49*(pos-11)));k<48;k++,i++) {
				pixel (i,d,c[j][k]);
			}
		}
	}
	// dalla 20 alla 30 le caselle
	// son girate di 180°
	else if (pos<30) {
		for (j=0,i=77;j<76;j++,i--) {
			for (k=0,d=(126+(49*(pos-21)));k<48;k++,d--) {
				pixel (i,d,c[j][k]);
			}
		}
	}
	// dalla 30 alla 40 son girate
	// 270°
	else {
		for (j=0,d=520;j<76;j++,d++) {
			for (k=0,i=(126+(49*(pos-31)));k<48;k++,i--) {
				pixel (i,d,c[j][k]);
			}
		}
	}
}

// Viene visualizzata la pedina di un giocatore
void print_token (char *dir, int newpos, int id) {
	// Come prima cosa, viene cancellata 
	// la posizione precedente
	// Per fare ciò, viene caricata l'immagine della casella
	// dove si trovava precedentemente il giocatore, quindi
	// viene presa la posizione dove si trovava la pedina del giocatore
	// e viene sostituita, a questo punto si disegna la pedina
	// nel posto nuovo.
	unsigned int c[13][22],s[76][48];
	unsigned int tmpb[598][598],tmp[3];
	FILE *fp;
	int i,d,y,x;
	char line[1024];
	// La pedina si trova su uno dei quattro angoli
	// Viene quindi aperto il tabellone e viene
	// "ritagliato" il pezzo da uno degli angoli
	if (!(p[id].pos%10)) {
		sprintf (line,"%s/board.ppm",dir);
		fp=fopen (line,"r");
		if (fp==NULL)
			return ;
		while (1) {
			fgets (line,sizeof(line),fp);
			if (sscanf (line,"%d %d",&x,&y)==2) {
				fgets (line,sizeof(line),fp);
				break;
			}
			if (feof (fp)) {
				return ;
			}
		}
		// controllo coordinate
		if (x!=598 || y!=598)
			return ;
		// carica lo "scheletro" del tabellone
		for (i=0;i<y;i++) {
			for (d=0;d<x;d++) {
				fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
				tmpb[i][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
//				pixel (screen,i,d,board[i][d]);
			}
		}
		switch (p[id].pos) {
			// Via
			case 0:
				y=520;
				x=520;
				break;
			case 10:
				y=520;
				x=0;
				break;
			case 20:
				y=0;
				x=0;
				break;
			case 30:
				x=520;
				y=0;
				break;
			default:
				break;
		}
		// LOL non commento perchè tanto
		// sicuramente mi ricorderò tutto
		// perfettamente senza problemi un
		// mese a questa parte. 13/04/2012 22.14
		
		/* mer 13 giugno 2012, h. 23.41: WTF? */
		for (i=0;i<13;i++) {
			for (d=0;d<22;d++) {
				pixel (y+24+i+(id%4)*13,x+32+d+(id/4)*22,tmpb[y+24+i+(id%4)*13][x+32+d+(id/4)*22]);
			}
		}
	}
	// La pedina si trova in un altro posto,
	// quindi viene aperto il file relativo
	// al posto in cui si trova, si prende il
	// contenuto e si ritaglia la parte interessata
	// la si gira nella posizione giusta e la
	// si incolla
	else {
		switch (b[p[id].pos].type) {
			case 'P':
				switch (t[b[p[id].pos].v].type) {
					case 'P':
						sprintf (line,"%s/prop.ppm",dir);
						break;
					case 'S':
						sprintf (line,"%s/station.ppm",dir);
						break;
					case 'U':
						switch (t[b[p[id].pos].v].col) {
							case 0:
								sprintf (line,"%s/electric.ppm",dir);
								break;
							case 1:
								sprintf (line,"%s/water.ppm",dir);
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
				break;
			case 'C':
				sprintf (line,"%s/chest.ppm",dir);
				break;
			case 'I':
				sprintf (line,"%s/income.ppm",dir);
				break;
			case 'H':
				sprintf (line,"%s/chance.ppm",dir);
				break;
			case 'L':
				sprintf (line,"%s/super.ppm",dir);
				break;
			default:
				break;
		}
		fp = fopen (line,"r");
		if (fp==NULL) {
			return ;
		}
		while (1) {
			fgets (line,sizeof(line),fp);
			if (sscanf (line,"%d %d",&x,&y)==2) {
				fgets (line,sizeof(line),fp);
				break;
			}
			if (feof (fp)) {
				return ;
			}
		}
		// legge le coordinate, per "sicurezza"
		if (x!=48 || y!=76)
			return ;
		for (i=0;i<y;i++) {
			for (d=0;d<x;d++) {
				fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
				// se il pixel è bianco, lo colora del colore
				// del terreno desiderato
				if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255 && t[b[p[id].pos].v].type=='P') {
					tmp[0]=((t[b[p[id].pos].v].col)&0xFF0000)>>16;
					tmp[1]=((t[b[p[id].pos].v].col)&0x00FF00)>>8;
					tmp[2]=((t[b[p[id].pos].v].col)&0x0000FF);
				}
				s[i][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
			}
		}
		if (b[p[id].pos].type=='P') {
			for (x=0;x<players_num;x++) {
				if (p[x].prop[b[p[id].pos].v][0]==-1) {
//					if (d!=28) {
						for (i=0;i<76;i++) {
							for (d=0;d<48;d++) {
								s[i][d]=0xB7410E;
							}
						}
//					}
				}
			}
		}
		for (i=0;i<13;i++) {
			for (d=0;d<22;d++) {
				if (p[id].pos<10) {
					y=520+24+i+(id%4)*13;
					x=79+49*(9-p[id].pos)+d+(id/4)*22;
//					pixel (520+24+i+(id%4)*13,79+49*(9-p[id].pos)+d+(id/4)*22,s[24+i+(id%4)*13][d+(id/4)*22]);
				}
				else if (p[id].pos<20) {
					y=79+49*(19-p[id].pos)+d+(id/4)*22;
					x=1+(3-(id%4))*13+(13-i);
//					pixel (79+49*(19-p[id].pos)+d+(id/4)*22,2+(3-(id%4))*13+i,s[24+i+(id%4)*13][d+(id/4)*22]);
				}
				else if (p[id].pos<30) {
					y=1+(13-i)+13*(3-(id%4));
					x=82+49*(p[id].pos-21)+(22-d)+(1-(id/4))*22;
//					pixel (1+(13-i)+13*(3-(id%4)),82+49*(p[id].pos-21)+(22-d)+(1-(id/4))*22,s[24+i+(id%4)*13][d+(id/4)*22]);
				}
				else {
					y=82+49*(p[id].pos-31)+(22-d)+(1-(id/4))*22;
					x=544+i+(id%4)*13;
//					pixel (83+49*(p[id].pos-31)+d+(1-(id/4))*22,544+i+(id%4)*13,s[24+i+(id%4)*13][d+(id/4)*22]);
				}
				pixel (y,x,s[24+i+(id%4)*13][d+(id/4)*22]);
			}
		}
	}
	// Ora che è stata cancellata la pedina dalla
	// posizione precedente, viene mostrata la pedina
	//nella nuova posizione.
	fclose (fp);
	p[id].pos=newpos;
	// se l'utente ha passato il via
	// viene usato %40, e gli vengono
	// dati i soldi del Via
	if (p[id].pos>39) {
		p[id].pos%=40;
		p[id].money+=money_per_lap;
		display_money (id);
	}
	sprintf (line,"%s/token.ppm",dir);
	fp=fopen (line,"r");
	if (fp==NULL) {
		return ;
	}
	while (1) {
		fgets (line,sizeof(line),fp);
		if (sscanf (line,"%d %d",&x,&y)==2) {
			fgets (line,sizeof(line),fp);
			break;
		}
		if (feof (fp)) {
			return ;
		}
	}
	// legge le coordinate, per "sicurezza"
	if (x!=22 || y!=13) {
		return ;
	}
	for (i=0;i<y;i++) {
		for (d=0;d<x;d++) {
			fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
			// se il pixel è bianco, lo colora del colore
			// del terreno desiderato
			if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255) {
				tmp[0]=(p[id].token&0xFF0000)>>16;
				tmp[1]=(p[id].token&0x00FF00)>>8;
				tmp[2]=(p[id].token&0x0000FF);
			}
			c[i][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
		}
	}
	// Il giocatore non si trova su uno dei quattro angoli
	if (p[id].pos%10) {
		if (p[id].pos<10) {
			y=520+24+(id%4)*13;
			x=79+49*(9-p[id].pos)+(id/4)*22;
		}
		else if (p[id].pos<20) {
			y=79+49*(19-p[id].pos)+(id/4)*22;
			x=2+(3-(id%4))*13;

		}
		else if (p[id].pos<30) {
			y=2+13*(3-(id%4));
			x=82+49*(p[id].pos-21)+(1-(id/4))*22;
		}
		else {
			y=83+49*(p[id].pos-31)+(1-(id/4))*22;
			x=544+(id%4)*13;
		}
		for (i=0;i<13;i++) {
			for (d=0;d<22;d++) {
				// Il colore 0xFF00FF è usato
				// come "trasparente"
				if (c[i][d]!=0xFF00FF) {
					pixel (y+((!(p[id].pos/10) || p[id].pos/10==2)?i:d),x+((!(p[id].pos/10) || p[id].pos/10==2)?d:i),c[i][d]);
				}
			}
		}
	}
	else {
		switch (p[id].pos) {
			case 0:
				y=520;
				x=520;
				break;
			case 10:
				y=520;
				x=0;
				break;
			case 20:
				y=0;
				x=0;
				break;
			case 30:
				x=520;
				y=0;
				break;
			default:
				break;
		}
		for (i=0;i<13;i++) {
			for (d=0;d<22;d++) {
				if (c[i][d]!=0xFF00FF) {
					pixel (y+24+i+(id%4)*13,x+32+d+(id/4)*22,c[i][d]);
				}
			}
		}
	}
	fclose (fp);
	SDL_Flip (screen);
}

int **scale (int m[76][48]) {
	int **n;
	float x_r = 48/20;
	float y_r = 76/30;
	int i,d,x,y;
	n=malloc(30*sizeof(int*));
	for (i=0;i<30;i++) {
		n[i]=malloc(20*sizeof(int));
		for (d=0;d<20;d++) {
			n[i][d]=m[(int)round(i*y_r)][(int)round(d*x_r)];
		}
	}
	return n;
}

// Mostra la scheda di informazioni su una proprietà
void display_property (char *dir,int pid,int plus) {
	int w,h,x,y,i,d;
	int c[76][48],**n;
	unsigned int tmp[3];
	char line[1024];
	FILE *fp;
	h=200;
	w=170;
	input ((498-h)/2+plus,598+(302-w)/2,h,w,0);
	rectangle ((498-h)/2+1+plus,598+(302-w)/2+1,(498-h)/2+h+plus,598+(302-w)/2+w,0xFFFFFF);
	switch (t[pid].type) {
		case 'P':
			sprintf (line,"%s/prop.ppm",dir);
			break;
		case 'S':
			sprintf (line,"%s/station.ppm",dir);
			break;
		case 'U':
			switch (t[pid].col) {
				case 0:
					sprintf (line,"%s/electric.ppm",dir);
					break;
				case 1:
					sprintf (line,"%s/water.ppm",dir);
					break;
				default:
					break;
			}
		default:
			break;
	}
	fp=fopen (line,"r");
	if (fp==NULL) {
		return ;
	}
	while (1) {
		fgets (line,sizeof(line),fp);
		if (sscanf (line,"%d %d",&x,&y)==2) {
			fgets (line,sizeof(line),fp);
			break;
		}
		if (feof (fp)) {
			return ;
		}
	}
	// legge le coordinate, per "sicurezza"
	if (x!=48 || y!=76)
		return ;
	for (i=0;i<y;i++) {
		for (d=0;d<x;d++) {
			fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
			// se il pixel è bianco, lo colora del colore
			// del terreno desiderato
			if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255 && t[pid].type=='P') {
				tmp[0]=((t[pid].col)&0xFF0000)>>16;
				tmp[1]=((t[pid].col)&0x00FF00)>>8;
				tmp[2]=((t[pid].col)&0x0000FF);
			}
			c[i][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
			if (c[i][d]==0xD4FCE4) {
				c[i][d]=0xFFFFFF;
			}
		}
	}
	fclose (fp);
	input ((498-h)/2+plus,598+(302-w)/2,31,21,0);
	for (i=0;i<w;i++) {
		pixel ((498-h)/2+31+plus,598+(302-w)/2+i,0);
	}
	n=scale(c);
	// Ora che si ha l'"icona" del terreno, questa viene messa
	// nell'angolo in alto a sinistra
	for (i=0;i<30;i++) {
		for (d=0;d<20;d++) {
			pixel ((498-h)/2+i+1+plus,(302-w)/2+599+d,n[i][d]);
		}
	}
	// Viene mostrato il nome della proprietà,
	// accorciandolo con "..." qualora il nome
	// risulti troppo lungo
	for (i=0;i<strlen(t[pid].name);i++) {
		memset (line,0,sizeof(line));
		strncpy (line,t[pid].name,i+1);
		if (display ((498-h)/2+9+plus,(302-w)/2+599+22,line,0)>w-40) {
			rectangle ((498-h)/2+2+plus,(302-w)/2+599+22,180,(302-w)/2+599+22+147+plus,0xFFFFFF);
			SDL_Flip (screen);
			strncpy (line,t[pid].name,i);
			strcat (line,"...");
			display ((498-h)/2+9+plus,(302-w)/2+599+22,line,0);
			break;
		}
	}
	/* Vengono ora mostrate le informazioni riguardanti
	 * la proprietà in questione
	 */
	switch (t[pid].type) {
		case 'P':
			// effetto "bold": viene stampata la scritta
			// e poi viene scritta la stessa, spostata di
			// un pixel a destra, così si ha l'effetto "bold"
			
			// PREZZO
			sprintf (line,"Price: ");
			display ((498-h)/2+33+plus+10,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+plus+10,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[6]*2);
			display ((498-h)/2+33+10+plus,(302-w)/2+599+111,line,0);
			
			// PEDAGGIO
			sprintf (line,"Rent: ");
			display ((498-h)/2+33+13+10+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+13+10+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[0]);
			display ((498-h)/2+33+13+10+plus,(302-w)/2+599+111,line,0);
	
			// 1 - 4 CASE
			for (i=1;i<5;i++) {
				sprintf (line,"With %d hous%s: ",i,(i==1)?"e":"es");
				display ((498-h)/2+33+13+i*13+10+13+plus,(302-w)/2+599+3,line,0);
				display ((498-h)/2+33+13+i*13+10+13+plus,(302-w)/2+599+4,line,0);
				sprintf (line,"%d",t[pid].values[i]);
				display ((498-h)/2+33+13+i*13+10+13+plus,(302-w)/2+599+111,line,0);

			}
			// HOTEL
			sprintf (line,"With HOTEL: ");
			display ((498-h)/2+33+78+10+13+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+78+10+13+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[5]);
			display ((498-h)/2+33+78+10+13+plus,(302-w)/2+599+111,line,0);
			
			
			// MORTGAGE
			sprintf (line,"Mortgage value: ");
			display ((498-h)/2+33+104+10+13+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+104+10+13+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[6]);
			display ((498-h)/2+33+104+10+13+plus,(302-w)/2+599+111,line,0);
			
			// ONE HOUSE
			sprintf (line,"One house costs: ");
			display ((498-h)/2+33+117+10+13+plus,(302-w)/2+599+3,line,0);
//			sprintf (line,"One house costs:",t[pid].values[7]);
			display ((498-h)/2+33+117+10+13+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[7]);
			display ((498-h)/2+33+117+10+13+plus,(302-w)/2+599+111,line,0);
			
			for (i=0;i<11;i++) {
				if (i!=2 && i!=8) {
					display ((498-h)/2+33+10+i*13+plus,(302-w)/2+599+155,"$",0);
				}
			}
			break;
		case 'S':
			// PREZZO
			sprintf (line,"Price: ");
			display ((498-h)/2+33+11+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+11+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[4]*2);
			display ((498-h)/2+33+11+plus,(302-w)/2+599+111,line,0);
			display ((498-h)/2+33+11+plus,(302-w)/2+599+155,"$",0);

			// RENT
			sprintf (line,"Rent: ");
			display ((498-h)/2+33+42+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+42+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[0]);
			display ((498-h)/2+33+42+plus,(302-w)/2+599+111,line,0);
			display ((498-h)/2+33+42+plus,(302-w)/2+599+155,"$",0);
			
			for (i=2;i<5;i++) {
				sprintf (line,"With %d stations: ",i);
				display ((498-h)/2+33+21+i*21+plus,(302-w)/2+599+3,line,0);
				display ((498-h)/2+33+21+i*21+plus,(302-w)/2+599+4,line,0);
				sprintf (line,"%d",t[pid].values[i-1]);
				display ((498-h)/2+33+21+i*21+plus,(302-w)/2+599+111,line,0);
				display ((498-h)/2+33+21+i*21+plus,(302-w)/2+599+155,"$",0);
			}
			
			// MORTGAGE
			sprintf (line,"Mortgage value: ");
			display ((498-h)/2+33+21*5+31+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+21*5+31+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[4]);
			display ((498-h)/2+33+21*5+31+plus,(302-w)/2+599+111,line,0);
			display ((498-h)/2+33+21*5+31+plus,(302-w)/2+599+155,"$",0);
			
			break;
		case 'U':
			// PREZZO
			sprintf (line,"Price: ");
			display ((498-h)/2+33+11+5+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+11+5+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[2]*2);
			display ((498-h)/2+33+11+5+plus,(302-w)/2+599+111,line,0);
			display ((498-h)/2+33+11+5+plus,(302-w)/2+599+155,"$",0);
			
			// FRASE D'EFFETTO 1
			sprintf (line,"If one \"Utility\" is owned rent");
			display ((498-h)/2+33+35+5+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"is %d times amount shown",t[pid].values[0]);
			display ((498-h)/2+33+46+5+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"on the dice.");
			display ((498-h)/2+33+57+5+plus,(302-w)/2+599+4,line,0);
			
			// FRASE D'EFFETTO 2
			sprintf (line,"If both utilities are owned");
			display ((498-h)/2+33+35+50+5+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"rent is %d times amount",t[pid].values[1]);
			display ((498-h)/2+33+46+50+5+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"shown on dice.");
			display ((498-h)/2+33+57+50+5+plus,(302-w)/2+599+4,line,0);
			
			// MORTGAGE
			sprintf (line,"Mortgage value: ");
			display ((498-h)/2+33+57+55+5+20+plus,(302-w)/2+599+3,line,0);
			display ((498-h)/2+33+57+55+5+20+plus,(302-w)/2+599+4,line,0);
			sprintf (line,"%d",t[pid].values[2]);
			display ((498-h)/2+33+57+55+5+20+plus,(302-w)/2+599+111,line,0);
			display ((498-h)/2+33+57+55+5+20+plus,(302-w)/2+599+155,"$",0);
			
			break;
		default:
			break;
	}
	
	SDL_Flip (screen);
}

void display_houses (char *dir) {
	// Scorre tutti i giocatori e tutte le proprietà
	int i,d,x,j,k,y,f;
	unsigned int tmp[3];
	FILE *fp;
	int c[76][48];
	char line[1024];
	int h;
	for (d=0;d<players_num;d++) {
		for (i=0;i<28;i++) {
			// Se il giocatore possiede
			// una determinata proprietà
			// e questa proprietà è un terreno
			// vengono stampate le case che
			// possiede su quel terreno
			if (p[d].prop[i][0] && t[i].type=='P') {
				sprintf (line,"%s/prop.ppm",dir);
				fp = fopen (line,"r");
				if (fp==NULL) {
					return ;
				}
				while (1) {
					fgets (line,sizeof(line),fp);
					if (sscanf (line,"%d %d",&x,&y)==2) {
						fgets (line,sizeof(line),fp);
						break;
					}
					if (feof (fp)) {
						return ;
					}
				}
				// legge le coordinate, per "sicurezza"
				if (x!=48 || y!=76)
					return ;
				for (j=0;j<y;j++) {
					for (k=0;k<x;k++) {
						fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
						// se il pixel è bianco, lo colora del colore
						// del terreno desiderato
						if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255) {
							tmp[0]=((t[i].col)&0xFF0000)>>16;
							tmp[1]=((t[i].col)&0x00FF00)>>8;
							tmp[2]=((t[i].col)&0x0000FF);
						}
						c[j][k]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
					}
				}
				fclose (fp);
				// ora abbiamo tutta la casella nella matrice c.
				// lavoreremo solo con la parte superiore, inserendo
				// le case e gli hotel necessari.
				
				// Ci sono delle case (messe nei bit meno significativi)
				if (p[d].prop[i][1]&0x0000FFFF) {
					h=((p[d].prop[i][1]&0x0000FFFF)>8)?8:p[d].prop[i][1]&0x0000FFFF;
					for (f=0;f<h;f++) {
						j=f/4;
						k=f%4;
						// vengono prima disegnati i contorni della casa
						// con un verde scuro:
						for (x=0;x<13;x++) {
							c[j*9][k*12+x]=0x008000;
							c[j*9+((j)?8:9)][k*12+x]=0x008000;
						}
						for (y=0;y<9;y++) {
							c[j*9+y][k*12]=0x008000;
							c[j*9+y][k*12+((k==3)?11:12)]=0x008000;
						}
						for (y=0;y<((j)?7:8);y++) {
							for (x=0;x<((k==3)?10:11);x++) {
								c[j*9+1+y][k*12+1+x]=0x009900;
							}
						}
					}
				}
				// se invece ci sono degli hotel, vengono
				// stampati
				else if (p[d].prop[i][1]&0xFFFF0000) {
					h=((p[d].prop[i][1]&0xFFFF0000)>0x00040000)?4:(p[d].prop[i][1]&0xFFFF0000)>>16;
					for (f=0;f<h;f++) {
						j=f/2;
						k=f%2;
						for (x=0;x<((k)?24:25);x++) {
							c[j*9][k*24+x]=0x92000A;
							c[j*9+((j)?8:9)][k*24+x]=0x92000A;
						}
						for (y=0;y<9;y++) {
							c[j*9+y][k*24]=0x92000A;
							c[j*9+y][k*24+((k)?23:24)]=0x92000A;
						}
						for (y=0;y<((j)?7:8);y++) {
							for (x=0;x<((k)?22:23);x++) {
								c[j*9+1+y][k*24+1+x]=0xFF2400;
							}
						}
					}
				}
				for (h=0;h<40;h++) {
					if (b[h].type=='P' && b[h].v==i) {
						break;
					}
				}
				if (h==40) {
					break;
				}
				if (h<10) {
					for (y=520,j=0;j<18;y++,j++) {
						for (x=471-(49*(h-1)),k=0;k<48;x++,k++) {
							pixel (y,x,c[j][k]);
						}
					}
				}
				// dalla 10 alla 20 le caselle
				// sono stampate girandole di 90°
				else if (h<20) {
					for (j=0,x=77;j<18;j++,x--) {
						for (k=0,y=(471-(49*(h-11)));k<48;k++,y++) {
							pixel (y,x,c[j][k]);
						}
					}
				}
				// dalla 20 alla 30 le caselle
				// son girate di 180°
				else if (h<30) {
					for (j=0,y=77;j<18;j++,y--) {
						for (k=0,x=(126+(49*(h-21)));k<48;k++,x--) {
							pixel (y,x,c[j][k]);
						}
					}
				}
				// dalla 30 alla 40 son girate
				// 270°
				else {
					for (j=0,x=520;j<18;j++,x++) {
						for (k=0,y=(126+(49*(h-31)));k<48;k++,y--) {
							pixel (y,x,c[j][k]);
						}
					}
				}	
			}
		}
	}
	SDL_Flip(screen);
	return ;
}

void _trade (char *dir, int oth, int pl, int trade[2][31]) {
	int i;
	for (i=0;i<28;i++) {
		if (trade[0][i]) {
			// viene tolta la proprietà all'altro
			// giocatore (più tutte le case, da
			// aggiornare) e viene dato il terreno
			// all'altro giocatore
			p[pl].prop[i][0]=p[oth].prop[i][0];
			p[oth].prop[i][0]=0;
			p[oth].prop[i][1]=0;
			p[pl].prop[i][1]=0;
		}
		else if (trade[1][i]) {
			p[oth].prop[i][0]=p[pl].prop[i][0];
			p[pl].prop[i][0]=0;
			p[pl].prop[i][1]=0;
			p[oth].prop[i][1]=0;
		}
	}
	// Viene effettuato lo scambio
	// di soldi tra i due giocatori
	if (p[oth].money>0) {
		if (trade[0][28]>p[oth].money) {
			if (p[pl].money<0) {
				if (p[oth].money>abs(p[pl].money)) {
					p[p[pl].oweto].money+=abs(p[pl].money);
					display_money (p[pl].oweto);
				}
				else {
					p[p[pl].oweto].money+=p[oth].money;
					display_money (p[pl].oweto);					
				}
			}
			p[pl].money+=p[oth].money;
			p[oth].money=0;
		}
		else {
			if (p[pl].money<0) {
				if (trade[0][28]>abs(p[pl].money)) {
					p[p[pl].oweto].money+=abs(p[pl].money);
					display_money (p[pl].oweto);
				}
				else {
					p[p[pl].oweto].money+=trade[0][28];
					display_money (p[pl].oweto);
				}
			}
			p[pl].money+=trade[0][28];
			p[oth].money-=trade[0][28];
		}
	}
	
	if (p[pl].money>0) {
		if (trade[1][28]>p[pl].money) {
			if (p[oth].money<0) {
				if (p[pl].money>abs(p[oth].money)) {
					p[p[oth].oweto].money+=abs(p[oth].money);
					display_money (p[oth].oweto);
				}
				else {
					p[p[oth].oweto].money+=p[pl].money;
					display_money (p[oth].oweto);
				}
			}
			p[oth].money+=p[pl].money;
			p[pl].money=0;
		}
		else {
			if (p[oth].money<0) {
				if (trade[1][28]>abs(p[oth].money)) {
					p[p[oth].oweto].money+=abs(p[oth].money);
					display_money (p[oth].oweto);
				}
				else {
					p[p[oth].oweto].money+=trade[1][28];
					display_money (p[oth].oweto);
				}
			}
			p[oth].money+=trade[1][28];
			p[pl].money-=trade[1][28];
		}
	}
	
	// Viene effettuato lo scambio di
	// buoni per uscire di prigione
	trade[0][29]=(!trade[0][29])?0:(trade[0][29]<3)?1:2;
	trade[1][29]=(!trade[1][29])?0:(trade[1][29]<3)?1:2;
	p[pl].outpr+=trade[0][29];
	p[oth].outpr-=trade[0][29];
	if (p[pl].outpr>2) {
		p[pl].outpr=2;
	}
	p[oth].outpr+=trade[1][29];
	p[pl].outpr-=trade[1][29];
	if (p[oth].outpr>2) {
		p[pl].outpr=2;
	}
	
	// Aggiornamento "grafico" :)	
	display_houses (dir);
	display_money (oth);
	display_money (pl);
	show_owners ();
	return ;
}

int turn (int id, char *dir) {
	// s contiene la schermata che dev'essere mostrata.
	// a seconda della schermata mostrata, viene gestito il tutto in modo diverso.
	// (es vengono letti in modo diverso gli input, ecc)
	// prev invece contiene la schermata mostrata al ciclo prima
	int s=1,prev=0;
	// 1: schermata principale
	// 2: schermata di scambio, mostra l'elenco dei giocatori
	// 3: schermata di scambio, mostra le proprietà e le fa selezionare
	// 4: schermata con l'elenco delle proprietà (per ipotecare o costuire)
	// 5: schermata per costruire
	
	// dice contiene i valori ottenuti dal lancio dei dadi
	// rolled il numero di volte che si ha tirato (dopo 3 doppi, in prigione)
	int dice[2]={-1,-1},rolled=0,clicked=-1;
	char msg[1024];
	int i,d,j,k,x,y,r[2],c2;
	int n[76][48],**c,tmp[3];
	char money[16]; // quando l'utente vuole modificare 
			// l'importo di soldi da dare all'altro
			// giocatore, viene preso il valore
			// e messo dentro "money", in modo da poterlo
			// visualizzare e, in seguito, metterlo con
			// un atoi () dentro la variabile int.
	int chm; // chm normalmente è a 0, se viene messo a 1 si sta
		 // modificando l'importo di soldi che il giocatore in alto
		 // da' a quello in basso, se messo a 2, invece, il contrario
	int mid; // indice per "money"
	// 0: contiene quello che dà l'altro utente al giocatore corrente
	// 1: contiene quello che il giocatore corrente da all'altro giocatore
	// 0->27: proprietà
	// 28: soldi
	// 29: get out of jail: 0 -> nessuno 1 -> quello alto 2 -> quello in basso 3 -> tutti e due (serve per una "questione grafica")
	// 30: approvazione da parte dei giocatori (0->non approva, 1->approva)
	int trade[2][31];
	// prevh contiene le costruzioni presenti
	// prima di iniziare ad aggiungerne e/o
	// toglierne
	int prevh[11];
	FILE *fp;
	switch (p[id].ai) {
		case 0:
			while (1) {
				// Turno del giocatore ID				
				// il giocatore è appena uscito 
				// di prigione
				if (p[id].inprison==-2 && p[id].money>=0) {
					p[id].inprison=0;
					print_token (dir,10+dice[0]+dice[1],id);
					SDL_Flip (screen);
				}
				if (p[id].inprison==-1) {
					// il giocatore paga i soldi che deve
					// e viene liberato.
					// a questo punto tira i dadi
					p[id].money-=bail;
					p[id].oweto=-1;
					display_money (id);
					SDL_Flip(screen);
					p[id].inprison=-2; // viene messo a -2, così
						   // quando viene riaggiornato 
						   // il tutto, il giocatore
						   // avvanza di quello che deve
					continue;
				}
				switch (s) {
					case 1:		
						 // schermata principale
						// se la schermata precedente
						// è diversa da questa, viene
						// aggiornata, altrimenti no
						/*
						 * Primo pulsante
						 */
						if (p[id].money<0) {
							// Il giocatore è in debito. Il primo pulsante sarà "BANKRUPT"
							sprintf (msg,"BANKRUPT");
						}
						// se dice[0]==dice[1], il giocatore può tirare i dadi
						else if (dice[0]==dice[1]) {
							sprintf (msg,"ROLL DICE");
						}
						// se il giocatore non può fare nulla delle cose precedenti,
						// viene mostrato il pulsante per finire il turno
						else {
							sprintf (msg,"END TURN");
						}
						input (10,598+10,20,131,0);
						rectangle (11,598+11,30,698+10+31,0xFFCC33);
						display (13,598+27+15,msg,0);
						/*
						 * Secondo pulsante
						 */
 						input (40,598+10,20,131,0);
						rectangle (41,598+11,60,698+10+31,0xFFCC33);
						display (43,598+40+15,"TRADE",0);
						/*
						 * terzo pulsante
						 */
						input (70,598+10,20,131,0);
						rectangle (71,598+11,90,698+10+31,0xFFCC33);
						display (73,598+13,"MANAGE PROPERTIES",0);
						// Se il giocatore ha gia tirato e si trova su una
						// proprietà, viene verificato se quest'ultima
						// è già stata acquistata o meno, in modo da mostrare
						// o meno il pulsante BUY
						if (rolled && b[p[id].pos].type=='P') {
							for (i=0;i<players_num;i++) {
								// La proprietà appartiene a qualcun'altro
								// quindi non viene mostrato il tasto BUY
								if (p[i].prop[b[p[id].pos].v][0]) {
									break;
								}
							}
							// Viene mostrato il pulsante BUY
							// perchè il terreno non appartiene
							// a nessuno
							
							/* LOL, controlla se ci sono anche abbastanza soldi */
							if (i==players_num && p[id].money>=t[b[p[id].pos].v].values[(t[b[p[id].pos].v].type=='P')?6:(t[b[p[id].pos].v].type=='S')?4:2]*2) {
								/*
								 * quarto pulsante
								 */
								input (100,598+10,20,131,0);
								rectangle (101,598+11,120,698+10+31,0xFFCC33);
								display (103,598+64,"BUY",0);
							}
						}
						else if (p[id].inprison && p[id].inprison!=-2 && !rolled) {
							input (100,598+10,20,131,0);
							rectangle (101,598+11,120,698+10+31,0xFFCC33);
							display (103,598+44,"POST BAIL",0);
							if (p[id].outpr) {
								input (130,598+10,20,131,0);
								rectangle (131,598+11,150,698+10+31,0xFFCC33);
								display (133, 598+45,"USE CARD",0);
							}
						}
						else {
							rectangle (100,598+10,121,698+10+32,0xFFFFFF);
							rectangle (130,598+10,151,698+10+32,0xFFFFFF);
						}
						SDL_Flip (screen);
						// A seconda di dove l'utente clicka, viene
						// eseguita l'azione relativa
//						for (;;) {
						while (SDL_PollEvent(&a)) {
							switch (a.type) {
								case SDL_KEYDOWN:
									// Se l'utente preme ESC, torna al
									// menu principale
									if (a.key.keysym.unicode==0x1b) {
										s=1;
										clicked=-1;
										_clear ();
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									if (a.button.button==1) {
										// L'utente ha clickato una delle proprietà
										// della fila in alto, che viene quindi
										// mostrata
										if (a.button.y<76 && (a.button.x>76 && a.button.x<522)) {
											if (b[(a.button.x-77)/49+21].type=='P') {
												if (clicked!=b[(a.button.x-77)/49+21].v) {
													display_property (dir,b[(a.button.x-77)/49+21].v,20);
													clicked=b[(a.button.x-77)/49+21].v;
												}
											}
										}
										if ((a.button.y>76 && a.button.y<522) && (a.button.x<76)) {
											if (b[19-(a.button.y-77)/49].type=='P') {
												if (clicked!=b[19-(a.button.y-77)/49].v) {
													display_property (dir,b[19-(a.button.y-77)/49].v,20);
													clicked=b[19-(a.button.y-77)/49].v;
												}
											}
										}
										if (a.button.y>522 && (a.button.x>76 && a.button.x<522)) {
											if (b[9-(a.button.x-77)/49].type=='P') {
												if (clicked!=b[9-(a.button.x-77)/49].v) {
													display_property (dir,b[9-(a.button.x-77)/49].v,20);
													clicked=b[9-(a.button.x-77)/49].v;
												}
											}
										}
										if ((a.button.y>76 && a.button.y<522) && (a.button.x>522 && a.button.x<598)) {
											if (b[(a.button.y-77)/49+31].type=='P') {
												if (clicked!=b[(a.button.y-77)/49+31].v) {
													display_property (dir,b[(a.button.y-77)/49+31].v,20);
													clicked=b[(a.button.y-77)/49+31].v;
												}
											}
										}
										// l'utente ha clickato nella colonna
										// compresa tra 10 e 141 (dove ci sono
										// i pulsanti)
										if (a.button.x>598+10 && a.button.x<598+141) {
											if (a.button.y>10 && a.button.y<30) {
												// viene fatta la scelta per i vari
												// pulsanti
												if (p[id].money<0) {
													// il giocatore è andato in bancarotta
													p[id].bankrupt=1;
													// Se il giocatore deve soldi alla
													// banca, tutte le proprietà vengono
													// "liberate"
													if (p[id].oweto==-1) {
														for (i=0;i<28;i++) {
															if (p[id].prop[i][0]) {	
																for (d=0;d<40;d++) {
																	if (b[d].type=='P' && b[d].v==i) {
																		update_owner(d,-1);
																		update_mortgage(dir,d,0);
																	}
																}
															}
															p[id].prop[i][0]=0;
															p[id].prop[i][1]=0;
														}
													}
													// Se altrimenti il giocatore deve dei
													// soldi a un altro giocatore, viene dato
													// tutto al giocatore
													else {
														for (i=0;i<28;i++) {
															if (p[id].prop[i][0]) {
																p[id].prop[p[id].oweto][0]=p[id].prop[i][0];
																p[id].prop[p[id].oweto][1]=0;
																for (d=0;d<40;d++) {
																	if (b[d].type=='P' && b[d].v==i) {
																		update_owner(d,p[id].oweto);
																	}
																}
															}
															p[id].prop[i][0]=0;
															p[id].prop[i][1]=0;
														}
														// vengono passati anche gli
														// "esci di prigione"
														p[p[id].oweto].outpr+=p[id].outpr;
														if (p[p[id].oweto].outpr>2) {
															// massimo 2 "esci gratis di
															// prigione" consentiti
															p[p[id].oweto].outpr=2;
														}
													}
													SDL_Flip (screen);
													// Addio.
													return ;
												}
												// Se l'utente può ancora tirare,
												// tira
												else if (dice[0]==dice[1]) {
													for (k=0;k<2;k++) {
														for (j=0;j<7;j++) {
															show_dice (10,790+k*55);
															SDL_Flip (screen);
															usleep (100000);
														}
														dice[k]=show_dice (10,790+k*55);
														SDL_Flip(screen);
													}
													usleep (500000);
													// Vengono cancellati i dadi
													rectangle (9,789,70,899,0xFFFFFF);
													if (p[id].inprison>0) {
														// se il giocatore è in prigione
														// e tira un doppio, esce, ma non
														// fa nessun altro tiro.
														if (dice[0]==dice[1]) {
															p[id].inprison=0;
															print_token (dir,10+dice[0]+dice[1],id);
															// i dadi hanno valori diversi
															// cosi il giocatore non può
															// ritirare ;)
															dice[0]=-1;
															dice[1]=-2;
														}
														// se altrimenti non tira doppio
														// viene scalato un turno dal suo
														// tempo in prigione
														else {
															p[id].inprison--;
															// se sono finiti i turni in prigione,
															// inprison viene messo a -1 così
															// che viene poi pagata dall'
															// utente la cauzione
															if (!p[id].inprison) {
																p[id].inprison--;
															}
														}
													}
													// se il giocatore non è in prigione,
													// allora questa parte viene eseguita
													else {
														rolled++;
														// se è il terzo lancio doppio,
														// il giocatore va in prigione
														if (rolled==3 && dice[0]==dice[1]) {
															print_token (dir,10,id);
															p[id].inprison=jail_turns;
															display_money(id);
															// IN JAILLLL XD
															_clear ();
															return ;
														}
														// altrimenti si sposta solo il 
														// giocatore al posto giusto
														else {
															print_token (dir,p[id].pos+dice[0]+dice[1],id);
															label:
															// Il giocatore è arrivato alla
															// posizione p[id].pos+dice[0]+dice[1].
															// Ora si vede cosa fare:
															switch (b[p[id].pos].type) {
																// Il giocatore è 
																// atterrato sul via
																// quindi riceve più soldi 
																// se le impostazioni son giuste
																case 'S':
																	p[id].money+=money_per_lap*(land_on_start-1);
																	display_money (id);
																	break;
																// Il giocatore è su
																// una proprietà.
																// se non è di nessuno
																// o è del giocatore
																// non succede nulla;
																// se invece è di un altro
																// giocatore e non è ipotecata,
																// viene fatto pagare.
																case 'P':
																	for (i=0;i<players_num;i++) {
																		if (p[i].prop[b[p[id].pos].v][0]==1) {
																			break;
																		}
																	}
																	if (i!=players_num && i!=id) {
																		// La proprietà è di uno degli
																		// altri giocatori e non è 
																		// ipotecata, quindi si paga
																		switch (t[b[p[id].pos].v].type) {
																			case 'P':
																				// viene verificato se si hanno 
																				// case o alberghi
																				if (p[i].prop[b[p[id].pos].v][1]&0xFFFF0000) {
																					// Viene fatto pagare proporzionalmente a quanti
																					// alberghi ci sono: viene creata una funzione
																					// (retta) passante per, per x=0, per il prezzo
																					// di 4case, per x=1 invece passa per il prezzo
																					// di 1hotel: questa retta, quindi, per valori
																					// interi di x, indicherà il prezzo da pagare
																					// con 1->4 hotel.
																					// (p-q)*x+q 
																					// p: prezzo 1 hotel, q: prezzo di 1 casa, x: numero alberghi
																					p[id].money-=abs((t[b[p[id].pos].v].values[5]-t[b[p[id].pos].v].values[4])*((p[i].prop[b[p[id].pos].v][1]&0xFFFF0000)>>16)+t[b[p[id].pos].v].values[4]);
																					j=abs((t[b[p[id].pos].v].values[5]-t[b[p[id].pos].v].values[4])*((p[i].prop[b[p[id].pos].v][1]&0xFFFF0000)>>16)+t[b[p[id].pos].v].values[4]);
																				}
																				else if (p[i].prop[b[p[id].pos].v][1]) {
																					// Se ci sono meno di 5 case, viene fatto
																					// pagare il prezzo per quel numero di case,
																					// altrimenti si usa il metodo per far pagare
																					// usato sopra
																					if (p[i].prop[b[p[id].pos].v][1]<5) {
																						p[id].money-=t[b[p[id].pos].v].values[p[i].prop[b[p[id].pos].v][1]];
																						j=t[b[p[id].pos].v].values[p[i].prop[b[p[id].pos].v][1]];
																					}
																					else {
																						// Ci son più di 4 case, si usa un
																						// ciocco figo:
																						// Si prende la retta usata prima per
																						// calcolare il prezzo di più hotel
																						// e si collocano i prezzi delle case
																						// tra 0 e 1: più precisamente, secondo
																						// questo calcolo:
																						// (p-q)*(1/max_houses-3)*(houses-4)+q
																						// p: prezzo di 1 hotel, q: prezzo di 4 case, max_houses: ovvio, houses: case presenti
																						p[id].money-=abs((t[b[p[id].pos].v].values[5]-t[b[p[id].pos].v].values[4])*(1.0/(max_houses-3))*(p[i].prop[b[p[id].pos].v][1]-4)+t[b[p[id].pos].v].values[4]);
																						j=abs((t[b[p[id].pos].v].values[5]-t[b[p[id].pos].v].values[4])*(1.0/(max_houses-3))*(p[i].prop[b[p[id].pos].v][1]-4)+t[b[p[id].pos].v].values[4]);
																					}
																				}
																				else {
																					// Non ci sono case/alberghi.
																					// Viene fatto pagare il pedaggio;
																					// se poi l'utente ha tutti i terreni,
																					// viene fatto ripagare
																					p[id].money-=t[b[p[id].pos].v].values[0];
																					for (d=0;d<28;d++) {
																						if (t[b[p[id].pos].v].col==t[d].col) {
																							if (p[i].prop[d][0]!=1) {
																								break;
																							}
																						}
																					}
																					if (d==28) {
																						// l'altro giocatore ha 
																						// tutti i terreni, quindi
																						// viene fatto pagare il doppio
																						p[id].money-=t[b[p[id].pos].v].values[0];
																						j=t[b[p[id].pos].v].values[0];
																					}
																				}
																				break;
																			case 'S':
																				// Viene contato il numero di 
																				// stazioni in possesso dell'altro
																				// giocatore, e viene fatto pagare
																				// di conseguenza.
																				for (d=0,k=0;d<28;d++) {
																					if (p[i].prop[d][0]==1 && t[d].type=='S') {
																						k++;
																					}
																				}
																				if (k<5) {
																					p[id].money-=t[b[p[id].pos].v].values[k-1];
																					j=t[b[p[id].pos].v].values[k-1];
																				}
																				else {
																					// Classico ciocco degli ultimi due valori
																					p[id].money-=abs((t[b[p[id].pos].v].values[3]-t[b[p[id].pos].v].values[2])*(k-3)+t[b[p[id].pos].v].values[2]);
																					j=abs((t[b[p[id].pos].v].values[3]-t[b[p[id].pos].v].values[2])*(k-3)+t[b[p[id].pos].v].values[2]);
																				}
																				break;
																			case 'U':
																				for (d=0,k=0;d<28;d++) {
																					if (p[i].prop[d][0]==1 && t[d].type=='U') {
																						k++;
																					}
																				}
																				// classico
																				p[id].money-=abs(((t[b[p[id].pos].v].values[1]-t[b[p[id].pos].v].values[0])*(k-1)+t[b[p[id].pos].v].values[0])*(dice[0]+dice[1]));
																				j=abs(((t[b[p[id].pos].v].values[1]-t[b[p[id].pos].v].values[0])*(k-1)+t[b[p[id].pos].v].values[0])*(dice[0]+dice[1]));
																				break;
																			default:
																				break;
																		}
																		// Se il giocatore è riuscito a
																		// pagare tutti i soldi dovuti,
																		// questi vengono passati all'altro.
																		// Altrimenti, l'altro riceve i soldi
																		// posseduti; gli altri li prenderà
																		// se e quando possibile
																		if (p[id].money>=0) {
																			p[i].money+=j;
																		}
																		else {
																			p[i].money+=(j+p[id].money);
																			p[id].oweto=i;
																		}
																		display_money (id);
																		display_money (i);
																	}
																	break;
																// Community Chest
																case 'C':
																	k=load_card (dir,"chest",(rand()%community_chest)+1,id);
																	if (k==10) {
																		// L'utente è stato spostato, quindi viene caricata
																		// la nuova casella
																		
																		// Che il Dio dei programmatori possa perdonarmi
																		// per aver usato una goto, ma mi sembra
																		// la soluzione più veloce senza ricopiare
																		// chili di codice.
																		goto label;
																	}
																	else if (k==5) {
																		_clear ();
																		return ;
																	}
																		
																	break;
																// Income Tax
																case 'I':
																	p[id].money-=income_tax;
																	p[id].oweto=-1;
																	display_money (id);
																	break;
																// Chance
																case 'H':
																	k=load_card (dir,"chance",(rand()%chance)+1,id);
																	if (k==10) {
																		// Utente spostato, ricarica
																		goto label;
																	}
																	else if (k==5) {
																		// prigione
																		_clear ();
																		return ;
																	}
																	break;
																// Sulla prigione e sul parcheggio
																// gratuito non succede niente
																case 'J':
																case 'F':
																	break;
																// "Vai in Prigione", l'utente
																// viene messo in prigione
																case 'G':
																	print_token (dir,10,id);
																	p[id].inprison=jail_turns;
																	display_money (id);
																	_clear ();
																	return ;
																case 'L':
																	p[id].money-=luxury_tax;
																	p[id].oweto=-1;
																	display_money (id);
																	break;
																default:
																	break;
															}
																
														}
													}
													SDL_Flip(screen);
												}
												else {
													// Finisce il turno
													_clear();
													return ;
												}
											}
											// scambio
											else if (a.button.y>40 && a.button.y<60) {
												s=2;
												_clear ();
											}
											// gestione proprietà
											else if (a.button.y>70 && a.button.y<90) {
												s=4;
												clicked=-1;
												c2=-1;
												_clear ();
											}
											else if (a.button.y>100 && a.button.y<120) {
												if (rolled && b[p[id].pos].type=='P') {
													for (i=0;i<players_num;i++) {
														if (p[i].prop[b[p[id].pos].v][0]) {
															break;
														}
													}
													if (i==players_num && p[id].money>=t[b[p[id].pos].v].values[(t[b[p[id].pos].v].type=='P')?6:(t[b[p[id].pos].v].type=='S')?4:2]*2) {
														// L'utente ha abbastanza soldi per
														// comprare la proprietà, quindi gli
														// viene venduta
														p[id].money-=t[b[p[id].pos].v].values[(t[b[p[id].pos].v].type=='P')?6:(t[b[p[id].pos].v].type=='S')?4:2]*2;
														p[id].prop[b[p[id].pos].v][0]=1;
														p[id].prop[b[p[id].pos].v][1]=0;
														rectangle (100,598+10,121,698+10+32,0xFFFFFF);
														display_money (id);
														show_owners ();
													}
												}
												else if (p[id].inprison>0 && !rolled) {
													// Viene pagata la cauzione
													if (p[id].money>=bail) {
														p[id].money-=bail;
														p[id].inprison=0;
														display_money (id);
													}
												}
											}
											else if (a.button.y>130 && a.button.y<150) {
												if (p[id].outpr && !rolled) {
													p[id].outpr--;
													p[id].inprison=0;
												}
											}
										}
									}
									break;
								case SDL_QUIT:
									quit ();
									break;
								default:
									break;
							}
						}
						break;
					case 2:
						// Viene mostrato l'elenco dei giocatori
						// per consentire l'effettuazione dello scambio
						for (i=0,x=0;i<players_num;i++) {
							if (!p[i].bankrupt) {
								k=x/4;
								j=x%4;
								rectangle (189+j*25,598+k*151,189+(j+1)*25,598+(k+1)*151,p[i].token);
								if (i==id) {
									sprintf (msg,"YOU");
								}
								else {
									sprintf (msg,"Player %d",i+1);
								}
								display (189+j*25+4,598+k*157+10,msg,~p[i].token);
								x++;
							}
						}
						SDL_Flip (screen);
						while (SDL_PollEvent(&a)) {
							switch (a.type) {
								case SDL_KEYDOWN:
									// Se l'utente preme ESC, torna al
									// menu principale
									if (a.key.keysym.unicode==0x1b) {
										s=1;
										_clear ();
										clicked=-1;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									if (a.button.button==1) {
										if (a.button.x>598 && a.button.y>189 && a.button.y<289) {
											k=((a.button.x-598)/151)*4+(a.button.y-189)/25; // k contiene l'id con cui scambiare
											if (k!=id && k<players_num) {
												// L'utente ha selezionato 
												// l'altro utente con cui scambiare
												// Viene quindi mostrata
												// la schermata di scambio
												s=3;
												_clear ();
												memset (trade,0,sizeof(trade));
												clicked=-1;
												chm=0;
											}
										}
									}
									break;
								case SDL_QUIT:
									quit ();
									break;
								default:
									break;
							}
						}
						break;
					// Scambio "vero e proprio"
					case 3:
//						display_property (dir,0);	// TEMP
						// l'altro utente viene mostrato sopra
						// r contiene il numero di proprietà a cui è "arrivato"
						// 0: avversario in alto, 1 giocatore in basso
						for (i=0,r[0]=0,r[1]=0;i<28;i++) {
							// se l'utente possiede la proprietà,
							// viene mostrata
							if (p[k].prop[i][0] || p[id].prop[i][0]) {
								switch (t[i].type) {
									case 'P':
										sprintf (msg,"%s/prop.ppm",dir);
										break;
									case 'S':
										sprintf (msg,"%s/station.ppm",dir);
										break;
									case 'U':
										switch (t[i].col) {
											case 0:
												sprintf (msg,"%s/electric.ppm",dir);
												break;
											case 1:
												sprintf (msg,"%s/water.ppm",dir);
												break;
											default:
												break;
										}
									default:
										break;
								}
								fp=fopen (msg,"r");
								if (fp==NULL) {
									return ;
								}
								while (1) {
									fgets (msg,sizeof(msg),fp);
									if (sscanf (msg,"%d %d",&x,&y)==2) {
										fgets (msg,sizeof(msg),fp);
										break;
									}
									if (feof (fp)) {
										return ;
									}
								}
								if (x!=48 || y!=76)
									return ;
								for (j=0;j<y;j++) {
									for (d=0;d<x;d++) {
										fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
										// se il pixel è bianco, lo colora del colore
										// del terreno desiderato
										if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255 && t[i].type=='P') {
											tmp[0]=((t[i].col)&0xFF0000)>>16;
											tmp[1]=((t[i].col)&0x00FF00)>>8;
											tmp[2]=((t[i].col)&0x0000FF);
										}
										n[j][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
									}
								}
								fclose (fp);
								c=scale(n);
//								i -> 0-28
								for (j=0;j<30;j++) {
									for (d=0;d<20;d++) {		
										if (p[k].prop[i][0]) {
											pixel (4+34*(r[0]/10)+j,598+9+29*(r[0]%10)+d,c[j][d]);
										}
										else {
											pixel (396+4+34*(r[1]/10)+j,598+9+29*(r[1]%10)+d,c[j][d]);
										}
									}
								}
								// Se è stato selezionato, viene messo
								// un bordo verde carino attorno al 
								// riquadro
								if(p[k].prop[i][0]) {
									if (trade[0][i]) {
										input (4+34*(r[0]/10),598+9+29*(r[0]%10),29,19,0x00FF00);
									}
									r[0]++;
								}
								else {
									if (trade[1][i]) {
										input (396+4+34*(r[1]/10),598+9+29*(r[1]%10),29,19,0x00FF00);
									}
									r[1]++;
								}
							}
						}
						
						// Viene mostrato il pulsante di 
						// approvazione per il giocatore 
						// di sopra
						if (!trade[0][30]) {
							rectangle (106,626,132,891,p[k].token);
							sprintf (msg,"Player %d approves",k+1);
							display (111,626+80,msg,~p[k].token);
						}
						else {
							rectangle (106,626,132,891,0xEDEDED);
							display (111,626+104,"Approved",0x121212);
						}
						
						if (!trade[1][30]) {
							rectangle (369,626,395,891,p[id].token);
							sprintf (msg,"Player %d approves",id+1);
							display (374,626+80,msg,~p[id].token);
						}
						else {
							rectangle (369,626,395,891,0xEDEDED);
							display (374,626+104,"Approved",0x121212);
						}
//						input (106,598+28,25,246,0);
						
						// Se il giocatore 1 ha degli "esci gratis di prigione",
						// vengono mostrati (MAX 2)
						if (p[k].outpr>0) {
							input (106,598+9,10,10,(trade[0][29]==1 || trade[0][29]==3)?0x00FF00:0);
							if (p[k].outpr==2) {
								input (121,598+9,10,10,(trade[0][29]>1)?0x00FF00:0);
							}
						}
						
						// Stessa cosa di sopra per il giocatore 2 (quello sotto)
						if (p[id].outpr>0) {
							input (369,598+9,10,10,(trade[1][29]==1 || trade[1][29]==3)?0x00FF00:0);
							if (p[id].outpr==2) {
								input (384,598+9,10,10,(trade[1][29]>1)?0x00FF00:0);
							}
						}
						
						/* vengono mostrate le input per i soldi */
						input (81,840,20,52,0);
						if (chm==1) {
							rectangle (82,841,101,892,0xFFCC33);
							if (strlen(money)>7) {
								display (83,842,money+(strlen(money)-7),0);
							}
							else {
								display (83,842,money,0);
							}
						}
						else {
							rectangle (82,841,101,892,0xEDEDED);
							sprintf (msg,"%d",trade[0][28]);
							if (strlen(msg)>7) {
								display (83,842,msg+(strlen(msg)-7),0);
							}
							else {
								display (83,842,msg,0);
							}
						}
						input (477,840,20,52,0);
						if (chm==2) {
							rectangle (478,841,497,892,0xFFCC33);
							if (strlen(money)>7) {
								display (480,842,money+(strlen(money)-7),0);
							}
							else {
								display (480,842,money,0);
							}
						}
						else {
							rectangle (478,841,497,892,0xEDEDED);
							sprintf (msg,"%d",trade[1][28]);
							if (strlen(msg)>7) {
								display (480,842,msg+(strlen(msg)-7),0);
							}
							else {
								display (480,842,msg,0);
							}
						}
						SDL_Flip (screen);
						while (SDL_PollEvent(&a)) {
							switch (a.type) {
								case SDL_MOUSEBUTTONDOWN:
									// Se è stata selezionata una proprietà
									// viene, la prima volta, evidenziata da un
									// bordo rosso, la seconda volta viene 
									// invece presa
									if (a.button.button==1) {
										// L'utente ha clickato "sopra"
										if (a.button.y<102) {
											if (a.button.x>598) {
												tmp[0]=(a.button.x-598-9)/29+(a.button.y/34)*10+1;
												for (d=0,tmp[1]=0;d<28;d++) {
													tmp[1]+=abs(p[k].prop[d][0]);
												}
												// se la proprietà cliccata è "valida"
												if ((tmp[0]<=tmp[1]) && (a.button.y>(a.button.y/34)*34+3 && a.button.x>598+((a.button.x-598)/29)*29+8)) {
													for (d=0;d<28;d++) {
														tmp[0]-=abs(p[k].prop[d][0]);
														if (!tmp[0]) {
															break;
														}
													}
//													rectangle (
													tmp[0]=(a.button.x-598-9)/29+(a.button.y/34)*10+1;
													// +128 se si tratta del giocatore
													// avversario (in alto), altrimenti
													// "+0"
													if (clicked!=tmp[0]+128) {
														display_property (dir,d,0);
														SDL_Flip(screen);
														// viene evidenziato di rosso il bordo
														// della tessera selezionata
														input ((a.button.y/34)*34+3,598+((a.button.x-598)/29)*29+8,31,21,0xFF0000);
														// viene invece tolto il bordo rosso
														// se c'è un altra scheda selezionata
														// in precedenza
														if (clicked!=-1) {
															// La scheda selezionata prima
															// si trova in alto
															if (clicked>128) {
																input (3+((clicked-129)/10)*34,598+8+((clicked-129)%10)*29,31,21,0xFFFFFF);
															}
															else {
																input (399+((clicked-1)/10)*34,598+8+((clicked-1)%10)*29,31,21,0xFFFFFF);
															}
														}
														clicked=tmp[0]+128;
													}
													else {
														// L'utente ha clickato in alto,
														// quindi si tratta della proprietà
														// dell'altro giocatore
														for (j=0,d=0;j<28;j++) {
															if (p[k].prop[j][0]) {
																d++;
																if (d==(clicked-128)) {
																	break;
																}
															}
														}
														// Se la proprietà è già stata selezionata,
														// viene deselezionata
														if (trade[0][j]) {
															trade[0][j]=0;
														}
														// Se invece era deselezionata, viene
														// selezionata
														else {
															trade[0][j]=1;
														}
														trade[0][30]=0;
														trade[1][30]=0;
													}
												}
											}
											if (a.button.y>81 && a.button.x>840 && a.button.x<892) {
												if (chm) {
													trade[chm-1][28]=atoi(money);
												}
												chm=3;
											}
										}
										else if (a.button.y>398 && a.button.y<498) {
											if (a.button.x>598) {
												tmp[0]=(a.button.x-598-9)/29+((a.button.y-398)/34)*10+1;
												for (d=0,tmp[1]=0;d<28;d++) {
													tmp[1]+=abs(p[id].prop[d][0]);
												}
												// se la proprietà cliccata è "valida"
												if ((tmp[0]<=tmp[1]) && ((a.button.y-398)>((a.button.y-398)/34)*34+3 && a.button.x>598+((a.button.x-598)/29)*29+8)) {
													for (d=0;d<28;d++) {
														tmp[0]-=abs(p[id].prop[d][0]);
														if (!tmp[0]) {
															break;
														}
													}
//													rectangle (
													tmp[0]=(a.button.x-598-9)/29+((a.button.y-398)/34)*10+1;
													// +128 se si tratta del giocatore
													// avversario (in alto), altrimenti
													// "+0"
													if (clicked!=tmp[0]) {
														display_property (dir,d,0);
														SDL_Flip(screen);
														// viene evidenziato di rosso il bordo
														// della tessera selezionata
														input (((a.button.y-398)/34)*34+3+396,598+((a.button.x-598)/29)*29+8,31,21,0xFF0000);
														// viene invece tolto il bordo rosso
														// se c'è un altra scheda selezionata
														// in precedenza
														if (clicked!=-1) {
															// La scheda selezionata prima
															// si trova in alto
															if (clicked>128) {
																input (3+((clicked-129)/10)*34,598+8+((clicked-129)%10)*29,31,21,0xFFFFFF);
															}
															else {
																input (399+((clicked-1)/10)*34,598+8+((clicked-1)%10)*29,31,21,0xFFFFFF);
															}
														}
														clicked=tmp[0];
													}
													else {
														// L'utente ha clickato sotto,
														// quindi si tratta della proprietà
														// propria
														for (j=0,d=0;j<28;j++) {
															if (p[id].prop[j][0]) {
																d++;
																if (d==clicked) {
																	break;
																}
															}
														}
														// Se la proprietà è già stata selezionata,
														// viene deselezionata
														if (trade[1][j]) {
															trade[1][j]=0;
														}
														// Se invece era deselezionata, viene
														// selezionata
														else {
															trade[1][j]=1;
														}
														trade[0][30]=0;
														trade[1][30]=0;
													}
												}
											}
											if (a.button.y>477 && a.button.x>840 && a.button.x<892) {
												if (chm) {
													trade[chm-1][28]=atoi(money);
												}
												chm=4;
											}
										}
										// chm viene messo a un valore +2 rispetto
										// al suo normale.
										// In questo modo, se l'utente clicka a caso,
										// chm, avendo già il suo valore giusto,
										// viene riconosciuto e messo a 0;
										// altrimenti se il click è stato fatto all'
										// interno della casella, chm viene messo al
										// suo valore esatto.
										if (chm && chm<3) {
											// mettere il valore corretto in trade[chm-1][28]!
											trade[chm-1][28]=atoi(money);
											chm=0;
										}
										else {
											chm-=2;
											mid=0;
											memset (money,0,sizeof(money));
											sprintf (money,"%d",trade[chm-1][28]);
											if (trade[chm-1][28]) {
												mid=strlen(money);
											}
											
										}
										if (a.button.x>607 && a.button.x<617) {
											// premuto il tasto per il primo "esci gratis"
											if (a.button.y>106 && a.button.y<116) {
												// se l'utente ha abbastanza 
												// outpr, allora viene aggiunto
												if (p[k].outpr>0) {
													switch (trade[0][29]) {
														case 0:
															trade[0][29]=1;
															break;
														case 1:
															trade[0][29]=0;
															break;
														case 2:
															trade[0][29]=3;
															break;
														case 3:
															trade[0][29]=2;
															break;
														default:
															break;
													}
													trade[0][30]=0;
													trade[1][30]=0;
												}
											}
											if (a.button.y>121 && a.button.y<131) {
												// Secondo "esci gratis" dell'utente di sopra
												if (p[k].outpr==2) {
													switch (trade[0][29]) {
														case 0:
															trade[0][29]=2;
															break;
														case 1:
															trade[0][29]=3;
															break;
														case 2:
															trade[0][29]=0;
															break;
														case 3:
															trade[0][29]=1;
															break;
														default:
															break;
													}
													trade[0][30]=0;
													trade[1][30]=0;
												}
											}
											// primo pulsante del secondo giocatore
											if (a.button.y>369 && a.button.y<379) {
												if (p[id].outpr>0) {
													switch (trade[1][29]) {
														case 0:
															trade[1][29]=1;
															break;
														case 1:
															trade[1][29]=0;
															break;
														case 2:
															trade[1][29]=3;
															break;
														case 3:
															trade[1][29]=2;
															break;
														default:
															break;
													}
													trade[0][30]=0;
													trade[1][30]=0;
												}
											}
											// secondo "esci gratis" del secondo giocatore
											if (a.button.y>384 && a.button.y<394) {
												if (p[id].outpr==2) {
													switch (trade[1][29]) {
														case 0:
															trade[1][29]=2;
															break;
														case 1:
															trade[1][29]=3;
															break;
														case 2:
															trade[1][29]=0;
															break;
														case 3:
															trade[1][29]=1;
															break;
														default:
															break;
													}
													trade[0][30]=0;
													trade[1][30]=0;													
												}
											}
										}
										// Pulsante "approved" in alto"
										if (a.button.x>626 && a.button.x<891) {
											if (a.button.y>106 && a.button.y<131) {
												// Se è stato premuto, intanto si verifica
												// che il giocatore in alto sia un umano;
												// in caso contrario non si lascia premere
												// il pulsante
												if (!p[k].ai) {
													trade[0][30]=1;
													if (trade[1][30]==1) {
														_trade (dir,k,id,trade);
														_clear ();
														s=1;
														clicked=-1;
														// SCAMBIO CONCLUSO
														// CON ENTRAMBI FAVOREVOLI
													}
													else if (p[id].ai) {
														// Il giocatore di sotto è
														// un AI, quindi qua viene chiamata
														// la funzione che decide se fare
														// o no lo scambio.
													}
												}
											}
											if (a.button.y>369 && a.button.y<394) {
												if (!p[id].ai) {
													trade[1][30]=1;
													if (trade[0][30]==1) {
														_trade (dir,k,id,trade);
														_clear ();
														s=1;
														clicked=-1;
														// SCAMBIO CONCLUSO
														// CON ENTRAMBI FAVOREVOLI
													}
													else if (p[k].ai) {
														// Il giocatore di sopra
														// è un AI, quindi gli viene fatta
														// fare la scelta qua
													}
												}
											}
										}		
											
									}
									break;
								case SDL_KEYDOWN:
									// Se l'utente preme ESC, torna al
									// menu principale
									if (a.key.keysym.unicode==0x1b) {
										s=1;
										clicked=-1;
										_clear ();
									}
									
									// La tastiera viene utilizzata solo se si sta
									// inserendo il valore di soldi desiderati nello scambio
									if (chm) {
										// 0->9
										if (a.key.keysym.unicode>=48 && a.key.keysym.unicode<='9') {
											money[mid]=a.key.keysym.unicode;
											if (mid<15) {
												money[++mid]=0;
												trade[0][30]=0;
												trade[1][30]=0;
											}
										}
										// backspace
										else if (a.key.keysym.unicode==0x08) {
											if (mid>0) {
												money[--mid]=0;
												trade[0][30]=0;
												trade[1][30]=0;
											}
										}
									}
									break;
								case SDL_QUIT:
									quit ();
									break;
								default:
									break;
							}
						}
						break;
					case 4:
						for (i=0,r[0]=0;i<28;i++) {
							// se l'utente possiede la proprietà,
							// viene mostrata
							if (p[id].prop[i][0]) {
								switch (t[i].type) {
									case 'P':
										sprintf (msg,"%s/prop.ppm",dir);
										break;
									case 'S':
										sprintf (msg,"%s/station.ppm",dir);
										break;
									case 'U':
										switch (t[i].col) {
											case 0:
												sprintf (msg,"%s/electric.ppm",dir);
												break;
											case 1:
												sprintf (msg,"%s/water.ppm",dir);
												break;
											default:
												break;
										}
									default:
										break;
								}
								fp=fopen (msg,"r");
								if (fp==NULL) {
									return ;
								}
								while (1) {
									fgets (msg,sizeof(msg),fp);
									if (sscanf (msg,"%d %d",&x,&y)==2) {
										fgets (msg,sizeof(msg),fp);
										break;
									}
									if (feof (fp)) {
										return ;
									}
								}
								if (x!=48 || y!=76)
									return ;
								for (j=0;j<y;j++) {
									for (d=0;d<x;d++) {
										fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
										// se il pixel è bianco, lo colora del colore
										// del terreno desiderato
										if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255 && t[i].type=='P') {
											tmp[0]=((t[i].col)&0xFF0000)>>16;
											tmp[1]=((t[i].col)&0x00FF00)>>8;
											tmp[2]=((t[i].col)&0x0000FF);
										}
										n[j][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
									}
								}
								fclose (fp);
								c=scale(n);
//								i -> 0-28
								for (j=0;j<30;j++) {
									for (d=0;d<20;d++) {
										pixel (4+34*(r[0]/10)+j,598+9+29*(r[0]%10)+d,c[j][d]);
									}
								}
								r[0]++;
							}
						}
						SDL_Flip (screen);
						while (SDL_PollEvent(&a)) {
							switch (a.type) {
								case SDL_KEYDOWN:
									if (a.key.keysym.unicode==0x1B) {
										s=1;
										_clear ();
										clicked=-1;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									if (a.button.button==1) {
										// Pulsante per costruire
										if ((a.button.y>390 && a.button.y<410) && (a.button.x>683 && a.button.x<814)) {
											if (t[c2].type=='P') {
												for (i=0,k=1;i<28;i++) {
													if (t[c2].col==t[i].col) {
														if (p[id].prop[i][0]!=1) {
															k=0;
														}
													}
												}
												// consente la costruzione solo
												// se il giocatore possiede tutti
												// i terreni ed essi non sono ipotecati
												if (k) {
													s=5;
													clicked=-1;
													_clear ();
													// vengono salvate le
													// case che c'erano prima
													for (i=0,j=0;(i<28 || j==10);i++) {
														if (t[c2].col==t[i].col) {
															prevh[j++]=p[id].prop[i][1];
														}
													}
													// vengono salvati anche i soldi
													// avuti in precedenza, se ci
													// fosse la necessità di ripristinare
													prevh[10]=p[id].money;
												}
											}
												
										}
										// Pulsante per ipotecare/disipotecare
										if ((a.button.y>360 && a.button.y<380) && (a.button.x>683 && a.button.x<814)) {
											if (c2!=-1) {
												// La proprietà dev'essere ipotecata
												if (p[id].prop[c2][0]==1 && !p[id].prop[c2][1]) {
													if (p[id].money<0) {
														// L'ipoteca da più soldi di quanti
														// si è in debito
														if (t[c2].values[(t[c2].type=='P')?6:(t[c2].type=='S')?4:2]>abs(p[id].money)) {
															if (p[id].oweto>0) {
																p[p[id].oweto].money+=abs(p[id].money);
																display_money (p[id].oweto);
															}
														}
														else {
															if (p[id].oweto>0) {
																p[p[id].oweto].money+=t[c2].values[(t[c2].type=='P')?6:(t[c2].type=='S')?4:2];
																display_money (p[id].oweto);
															}
														}
													}	
													p[id].money+=t[c2].values[(t[c2].type=='P')?6:(t[c2].type=='S')?4:2];
													p[id].prop[c2][0]=-1;
													for (i=0;i<40;i++) {
														if (b[i].v==c2) {
															break;
														}
													}
													if (i==40) {
														break;
													}
													// vengono aggiornate alcune parti
													// l'SDL_Flip() viene poi fatto nelle 
													// funzioni sotto
													input (360,683,20,131,0);
													rectangle (361,684,380,814,0xFFCC33);
													display (363,708,"UNMORTGAGE",0);
													rectangle (390,683,411,815,0xFFFFFF);
													update_mortgage (dir,i,1);
													display_money (id);
													for (d=0;d<players_num;d++) {
														if (p[d].pos==i) {
															print_token (dir,i,d);
														}
													}
												}
												// La proprietà dev'essere disipotecata
												else if (p[id].prop[c2][0]==-1 && p[id].money>=t[c2].values[(t[c2].type=='P')?6:(t[c2].type=='S')?4:2]) {
													p[id].money-=t[c2].values[(t[c2].type=='P')?6:(t[c2].type=='S')?4:2];
													p[id].prop[c2][0]=1;
													for (i=0;i<40;i++) {
														if (b[i].v==c2) {
															break;
														}
													}
													if (i==40) {
														break;
													}
													update_mortgage (dir,i,0);
													input (360,683,20,131,0);
													rectangle (361,684,380,814,0xFFCC33);
													display (363,716,"MORTGAGE",0);
													for (d=0;d<players_num;d++) {
														if (p[d].pos==i)  {
															print_token (dir,i,d);
														}
													}
													if (t[c2].type=='P') {
														for (i=0,k=1;i<28;i++) {
															if (t[c2].col==t[i].col) {
																if (p[id].prop[i][0]!=1) {
																	k=0;
																}
															}
														}
														if (k) {
															input (390,683,20,131,0);
															rectangle (391,684,410,814,0xFFCC33);
															display (393,735,"BUILD",0);
														}
														else {
															rectangle (390,683,411,815,0xFFFFFF);
														}
													}
													else {
														rectangle (390,683,411,815,0xFFFFFF);
													}
													display_money (id);
												}
											}
										}
										if (a.button.y<102) {
											if (a.button.x>598) {
												tmp[0]=(a.button.x-598-9)/29+(a.button.y/34)*10+1;
												for (d=0,tmp[1]=0;d<28;d++) {
													tmp[1]+=abs(p[id].prop[d][0]);
												}
												// se la proprietà cliccata è "valida"
												if ((tmp[0]<=tmp[1]) && (a.button.y>(a.button.y/34)*34+3 && a.button.x>598+((a.button.x-598)/29)*29+8)) {
													for (d=0;d<28;d++) {
														tmp[0]-=abs(p[id].prop[d][0]);
														if (!tmp[0]) {
															break;
														}
													}
//													rectangle (
													tmp[0]=(a.button.x-598-9)/29+(a.button.y/34)*10+1;
													// +128 se si tratta del giocatore
													// avversario (in alto), altrimenti
													// "+0"
													if (clicked!=tmp[0]) {
														display_property (dir,d,0);
														// Se la proprietà non è ipotecata
														// e non ci sono case o alberghi su
														// di essa,la fa ipotecare
														if (p[id].prop[d][0]==1 && !p[id].prop[d][1]) {
															input (360,683,20,131,0);
															rectangle (361,684,380,814,0xFFCC33);
															display (363,716,"MORTGAGE",0);
														}
														// altrimenti, se è già ipotecata
														// ma l'utente ha i soldi per togliere
														// l'ipoteca, la fa togliere.
														else if (p[id].prop[d][0]==-1 && p[id].money>=t[d].values[(t[d].type=='P')?6:(t[d].type=='S')?4:2]) {
															input (360,683,20,131,0);
															rectangle (361,684,380,814,0xFFCC33);
															display (363,708,"UNMORTGAGE",0);
														}
														// altrimenti boh niente :)
														else {
															rectangle (360,683,381,815,0xFFFFFF);
														}
														
														// Se l'utente vuole costruire case,
														// gli viene fatto fare, ovviamente solo
														// se ha l'intero monopolio
														if (t[d].type=='P') {
															for (i=0,k=1;i<28;i++) {
																if (t[d].col==t[i].col) {
																	if (p[id].prop[i][0]!=1) {
																		k=0;
																	}
																}
															}
															// L'utente ha tutto,
															// può costruire :)
															if (k) {
																input (390,683,20,131,0);
																rectangle (391,684,410,814,0xFFCC33);
																display (393,735,"BUILD",0);
															}
															else {
																rectangle (390,683,411,815,0xFFFFFF);
															}
														}
														else {
															rectangle (390,683,411,815,0xFFFFFF);
														}
														
															
														SDL_Flip(screen);
														// viene evidenziato di rosso il bordo
														// della tessera selezionata
														input ((a.button.y/34)*34+3,598+((a.button.x-598)/29)*29+8,31,21,0xFF0000);
														// viene invece tolto il bordo rosso
														// se c'è un altra scheda selezionata
														// in precedenza
														if (clicked!=-1) {
															input (3+((clicked-1)/10)*34,598+8+((clicked-1)%10)*29,31,21,0xFFFFFF);
														}
														clicked=tmp[0];
														c2=d;
													}
												}
											}
										}
									}
									break;
								case SDL_QUIT:
									quit ();
									break;
								default:
									break;
							}
						}
						break;
					case 5:
						// c2 contiene una delle proprietà da mostrare;
						// vengono mostrate in alto anche le altre.
						for (i=0,r[0]=0;(i<28 || r[0]==10);i++) {
							// La proprietà i ha lo stesso colore
							// di c2
							if (t[c2].col==t[i].col && t[i].type=='P') {
								sprintf (msg,"%s/prop.ppm",dir);
								fp=fopen (msg,"r");
								if (fp==NULL) {
									return ;
								}
								while (1) {
									fgets (msg,sizeof(msg),fp);
									if (sscanf (msg,"%d %d",&x,&y)==2) {
										fgets (msg,sizeof(msg),fp);
										break;
									}
									if (feof (fp)) {
										return ;
									}
								}
								if (x!=48 || y!=76)
									return ;
								for (j=0;j<y;j++) {
									for (d=0;d<x;d++) {
										fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
										// se il pixel è bianco, lo colora del colore
										// del terreno desiderato
										if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255) {
											tmp[0]=((t[i].col)&0xFF0000)>>16;
											tmp[1]=((t[i].col)&0x00FF00)>>8;
											tmp[2]=((t[i].col)&0x0000FF);
										}
										n[j][d]=(tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
									}
								}
								fclose (fp);
								c=scale(n);
								for (j=0;j<30;j++) {
									for (d=0;d<20;d++) {
										pixel (4+j,598+9+29*(r[0]%10)+d,c[j][d]);
									}
								}
								// Vengono mostrati tutti i
								// pulsanti + e - sotto ogni
								// proprietà
								rectangle (44,598+9+29*(r[0]%10),64,598+9+29*(r[0]%10)+20,0xADDFAD);
								display (46,598+9+29*(r[0]%10)+5,"+",0);
								rectangle (64,598+9+29*(r[0]%10),84,598+9+29*(r[0]%10)+20,0xCC8899);
								display (61,598+9+29*(r[0]%10)+6,"_",0);
								r[0]++;
							}
						}
						// Pulsante per resettare al default :)
						input (360,683,20,131,0);
						rectangle (361,684,380,814,0xFFCC33);
						display (363,730,"RESET",0);
						// Pulsante "DONE"
						input (390,683,20,131,0);
						rectangle (391,684,410,814,0xFFCC33);
						display (393,732,"DONE",0);
						SDL_Flip (screen);
						while (SDL_PollEvent(&a)) {
							switch (a.type) {
								case SDL_QUIT:
									quit ();
									break;
								case SDL_KEYDOWN:
									if (a.key.keysym.unicode==0x1B) {
										s=1;
										_clear ();
										clicked=-1;
									}
									break;
								case SDL_MOUSEBUTTONDOWN:
									if (a.button.button==1) {	
										if (a.button.y<84) {
											if (a.button.x>598) {
												tmp[0]=(a.button.x-598-9)/29+1;
/*												for (d=0,tmp[1]=0;d<28;d++) {
													tmp[1]+=abs(p[id].prop[d][0]);
												}*/
												// se la proprietà cliccata è "valida"
												if ((tmp[0]<=r[0]) && (a.button.y>3 && a.button.x>598+((a.button.x-598)/29)*29+8)) {
													for (d=0;d<28;d++) {
														tmp[0]-=(t[d].col==t[c2].col);
														if (!tmp[0]) {
															break;
														}
													}
//													rectangle (
													tmp[0]=(a.button.x-598-9)/29+1;
													// L'utente ha clickato sulla 
													// proprietà per analizzarla
													if (a.button.y<34) {
														if (clicked!=tmp[0]) {
															display_property (dir,d,0);
															SDL_Flip(screen);
															// viene evidenziato di rosso il bordo
															// della tessera selezionata
															input (3,598+((a.button.x-598)/29)*29+8,31,21,0xFF0000);
															// viene invece tolto il bordo rosso
															// se c'è un altra scheda selezionata
															// in precedenza
															if (clicked!=-1) {
																input (3,598+8+((clicked-1)%10)*29,31,21,0xFFFFFF);
															}
															clicked=tmp[0];
														}
													}
													// L'utente vuole mettere una casa in più
													else if (a.button.y>44 && a.button.y<64) {
														// viene fatta una prima
														// divisione: se bisogna
														// aggiungere una casa o 
														// un albergo
														if (!(p[id].prop[d][1]&0xFFFF0000) && (p[id].prop[d][1]&0x0000FFFF)<max_houses) {
															// Bisogna mettere una casa!
															// viene prima controllato che il
															// resto sia in ordine.
															for (i=0,j=0;i<28;i++) {
																// Stesso colore
																if (t[c2].col==t[i].col) {
																	if (d!=i) {
																		// Bisogna aggiungere una casa,
																		// in quanto in 'i' c'è una differenza
																		// di più di 1 con l'altro
																		if ((p[id].prop[d][1]>p[id].prop[i][1]) && !(p[id].prop[i][1]&0xFFFF0000)) {
																			// Se ci sono abbastanza soldi, viene costruita
																			// la casa
																			if (prevh[j]>=p[id].prop[i][1]+1) {
																				if (p[id].money>=t[i].values[7]/2) {
																					p[id].money-=t[i].values[7]/2;
																					p[id].prop[i][1]++;
																				}
																				// a metà prezzo (c'erano già prima)
																			}
																			else {
																				// prezzo pieno
																				if (p[id].money>=t[i].values[7]) {
																					p[id].money-=t[i].values[7];
																					p[id].prop[i][1]++;
																				}
																			}
																		}
																	}
																	else {
																		k=j;
																	}
																	j++;
																}
															}
															// Ora le altre case sono allo
															// stesso livello, quindi si può
															// costruire una casa in più =) (se ha 
															// ancora soldi!)
															if (prevh[k]>=p[id].prop[d][1]+1) {
																if (p[id].money>=t[d].values[7]/2) {
																	p[id].money-=t[d].values[7]/2;
																	p[id].prop[d][1]++;
																}
																// a metà prezzo (c'erano già prima)
															}
															else {
																// prezzo pieno
																if (p[id].money>=t[d].values[7]) {
																	p[id].money-=t[d].values[7];
																	p[id].prop[d][1]++;
																}
															}
															/*
															if (p[id].money>=t[d].values[7]) {
																p[id].money-=t[d].values[7];
																p[id].prop[d][1]++;
															}
															*/
														}
														else {
															// Bisogna mettere degli alberghi ^^
															// (c'è da fare la divisione, se è
															// il primo albergo o se ce ne sono
															// già altri)
															if (p[id].prop[d][1]&0xFFFF0000) {
																// ci sono già altri hotel
																// viene controllato se ce n'è
																// solo uno o più
																if ((p[id].prop[d][1]&0xFFFF0000)==0x00010000) {
																	// ce n'è solo 1
																	// quindi ci sono già, almeno,
																	// max_houses sugli altri territori;
																	// bisogna assicurarsi che ci sia almeno
																	// 1 hotel per casa
																	for (i=0,j=0;i<28;i++) {
																		if (t[c2].col==t[i].col) {
																			if (d!=i) {
																				if (!(p[id].prop[i][1]&0xFFFF0000)) {
																					// Su questa proprietà non
																					// c'è nemmeno un albergo,
																					// quindi viene messo
																					// se max_hotels è 0, non si possono
																					// costruire hotel
																					if (max_hotels) {
																						if (prevh[j]>=p[id].prop[i][1]+0x00010000) {
																							if (p[id].money>=t[i].values[7]/2) {
																								p[id].money-=t[i].values[7]/2;
																								p[id].prop[i][1]=0x00010000;
																							}
																							// a metà prezzo (c'erano già prima)
																						}
																						else {
																							// prezzo pieno
																							if (p[id].money>=t[i].values[7]) {
																								p[id].money-=t[i].values[7];
																								p[id].prop[i][1]=0x00010000;
																							}
																						}
																					}
																				}
																			}
																			else {
																				k=j;
																			}
																			j++;
																		}
																	}
																	// Ora che su tutte le proprietà c'è
																	// un albergo, viene messa anche su
																	// quella su cui si voleva costruire
																	if (p[id].money>=(max_houses+1)*t[d].values[7] && max_hotels>=2) {
																		if (p[id].money>=t[d].values[7] && max_hotels) {
																			if (prevh[k]>=p[id].prop[d][1]+0x00010000) {
																				if (p[id].money>=((max_houses+1)*t[d].values[7])/2) {
																					p[id].money-=((max_houses+1)*t[d].values[7])/2;
																					p[id].prop[d][1]=0x00020000;
																				}
																				// a metà prezzo (c'erano già prima)
																			}
																			else {
																				// prezzo pieno
																				if (p[id].money>=((max_houses+1)*t[d].values[7])) {
																					p[id].money-=((max_houses+1)*t[d].values[7]);
																					p[id].prop[d][1]=0x00020000;
																				}
																			}
																		}
																		//p[id].money-=(max_houses+1)*t[d].values[7];
																		//p[id].prop[d][1]=0x00020000;
																	}
																}
																else {
																	// C'è più di un hotel, quindi
																	// vengono aggiunti altri hotel
																	// facendoli pagare (max_houses+1)*costo
																	for (i=0,j=0;i<28;i++) {
																		if (t[c2].col==t[i].col) {
																			if (d!=i) {
																				// Bisogna aggiungere una casa,
																				// in quanto in 'i' c'è una differenza
																				// di più di 1 con l'altro
																				if (((p[id].prop[d][1]&0xFFFF0000)>>16)>((p[id].prop[i][1]&0xFFFF0000)>>16)) {
																					// Limite degli hotel
																					if (((p[id].prop[i][1]&0xFFFF0000)>>16)+1<=max_hotels) {
																					//	if (p[id].money>=t[i].values[7] && max_hotels) {
																						if (prevh[j]>=p[id].prop[i][1]+0x00010000) {
																							if (p[id].money>=((max_houses+1)*t[i].values[7])/2) {
																								p[id].money-=((max_houses+1)*t[i].values[7])/2;
																								p[id].prop[i][1]+=0x00010000;
																							}
																							// a metà prezzo (c'erano già prima)
																						}
																						else {
																							// prezzo pieno
																							if (p[id].money>=((max_houses+1)*t[i].values[7])) {
																								p[id].money-=((max_houses+1)*t[i].values[7]);
																								p[id].prop[i][1]+=0x00010000;
																							}
																						}
																					}
																				}
																			}
																			else {
																				k=j;
																			}
																			j++;
																		}
																	}
																	// Se ci sono ancora soldi, adesso viene
																	// costruito un altro albergo
																	if (((p[id].prop[d][1]&0xFFFF0000)>>16)+1<=max_hotels) {
																		if (prevh[k]>=p[id].prop[d][1]+0x00010000) {
																			if (p[id].money>=((max_houses+1)*t[d].values[7])/2) {
																				p[id].money-=((max_houses+1)*t[d].values[7])/2;
																				p[id].prop[d][1]+=0x00010000;
																			}
																			// a metà prezzo (c'erano già prima)
																		}
																		else {
																			// prezzo pieno
																			if (p[id].money>=((max_houses+1)*t[d].values[7])) {
																				p[id].money-=((max_houses+1)*t[d].values[7]);
																				p[id].prop[d][1]+=0x00010000;
																			}
																		}
																		//p[id].money-=(max_houses+1)*t[d].values[7];
																		//p[id].prop[d][1]+=0x00010000;
																	}
																}
															}
															else {
																// Non ci sono ancora alberghi,
																// quindi viene prima verificato
																// che su tutte le case ci siano
																// max_houses, quindi viene 
																// messo l'hotel
																for (i=0,j=0;i<28;i++) {
																	if (t[c2].col==t[i].col) {
																		if (d!=i) {
																			// controlla che ci siano max_houses
																			// oppure già un hotel
																			if ((p[id].prop[i][1]<max_houses) && !(p[id].prop[i][1]&0xFFFF0000)) {
																				// Non ci sono abbastanza case, vengono quindi messe
																				// le case necessarie
																				if (prevh[j]>=p[id].prop[i][1]+1) {
																					if (p[id].money>=t[i].values[7]/2) {
																						p[id].money-=t[i].values[7]/2;
																						p[id].prop[i][1]++;
																					}
																				}
																				else {
																					if (p[id].money>=t[i].values[7]) {
																						p[id].money-=t[i].values[7];
																						p[id].prop[i][1]++;
																					}
																				}
																			}
																		}
																		else {
																			k=j;
																		}
																		j++;
																	}
																}
																// se ci sono ancora soldi, mette l'albergo
																if (max_hotels) {
																	if (prevh[k]>=p[id].prop[d][1]+0x00010000) {
																		if (p[id].money>=t[d].values[7]/2) {
																			p[id].money-=t[d].values[7]/2;
																			p[id].prop[d][1]=0x00010000;
																		}
																	}
																	else {
																		if (p[id].money>=t[d].values[7] && max_hotels) {
																			p[id].money-=t[d].values[7];
																			p[id].prop[d][1]=0x00010000;
																		}
																	}
																}
															}
														}
														display_houses (dir);
														display_money (id);
													}
													else if (a.button.y>64) {
														// Vengono tolte case..
														// ora inizia il casino :O
														
														// viene prima determinato se
														// bisogna togliere case o
														// alberghi
														
														if (!(p[id].prop[d][1]&0xFFFF0000)) {
															// case
															// prima cosa, si tolgono
															// altre case dagli altri posti
															// se necessario
															for (i=0,j=0;i<28;i++) {
																if (t[c2].col==t[i].col) {
																	if (d!=i) {
																		// Se ci sono più case dell'altro, vengono
																		// tolte
																		if (p[id].prop[d][1]<p[id].prop[i][1]) {
																			if (prevh[j]>p[id].prop[i][1]-1) {
																				if (p[id].money<0) {
																					if ((int)(t[i].values[7]/2)>abs(p[id].money)) {
																						if (p[id].oweto>0) {
																							p[p[id].oweto].money+=abs(p[id].money);
																							display_money (p[id].oweto);
																						}
																					}
																					else {
																						if (p[id].oweto>0) {
																							p[p[id].oweto].money+=(int)(t[i].values[7]/2);
																							display_money (p[id].oweto);
																						}
																					}
																				}
																				p[id].money+=(int)(t[i].values[7]/2);
																			}
																			else {
																				p[id].money+=t[i].values[7];
																			}
																			if (p[id].prop[i][1]&0xFFFF0000) {
																				// L'utente ha un hotel, viene
																				// tolto, e messo il numero
																				// massimo di case
																				p[id].prop[i][1]=max_houses;
																			}
																			else {
																				p[id].prop[i][1]--;
																			}
																		}
																	}
																	// viene salvata la j relativa d,
																	// in modo da non doverla ricavare
																	// in seguito
																	else {
																		k=j;
																	}
																	j++;
																}
															}
															if (p[id].prop[d][1]) {
																if (prevh[k]>p[id].prop[d][1]-1) {
																	if (p[id].money<0) {
																		if ((int)(t[i].values[7]/2)>abs(p[id].money)) {
																			if (p[id].oweto>0) {
																				p[p[id].oweto].money+=abs(p[id].money);
																				display_money (p[id].oweto);
																			}
																		}
																		else {
																			if (p[id].oweto>0) {
																				p[p[id].oweto].money+=(int)(t[i].values[7]/2);
																				display_money (p[id].oweto);
																			}
																		}
																	}
																	p[id].money+=(int)(t[d].values[7]/2);
																}
																else {
																	p[id].money+=t[d].values[7];
																}
																p[id].prop[d][1]--;
															}
														}
														else {
															// Alberghi
															// Viene fatta la divisione se
															// è il primo albergo (dopo ci sono
															// solo case) o no
															if (p[id].prop[d][1]>0x00010000) {
																// Ci sono più hotel, quindi
																// toglierne uno non comporta
																// il dover rimettere le case
																// (e anche sulle altre proprietà
																// ci sono 1+hotel)
																for (i=0,j=0;i<28;i++) {
																	if (t[c2].col==t[i].col) {
																		if (d!=i) {
																			// Se ci sono più case dell'altro, vengono
																			// tolte
																			if (p[id].prop[d][1]<p[id].prop[i][1]) {
																				if (prevh[j]>p[id].prop[i][1]-0x00010000) {
																					if (p[id].money<0) {
																						if ((int)((max_houses+1)*t[i].values[7])/2>abs(p[id].money)) {
																							if (p[id].oweto>0) {
																								p[p[id].oweto].money+=abs(p[id].money);
																								display_money (p[id].oweto);
																							}
																						}
																						else {
																							if (p[id].oweto>0) {
																								p[p[id].oweto].money+=(int)((max_houses+1)*t[i].values[7])/2;
																								display_money (p[id].oweto);
																							}
																						}
																					}
																					p[id].money+=(int)((max_houses+1)*t[i].values[7])/2;
																				}
																				else {
																					p[id].money+=((max_houses+1)*t[i].values[7]);
																				}
																				p[id].prop[i][1]-=0x00010000;
																			}
																		}
																		// viene salvata la j relativa d,
																		// in modo da non doverla ricavare
																		// in seguito
																		else {
																			k=j;
																		}
																		j++;
																	}
																}
																if (prevh[k]>p[id].prop[d][1]-0x00010000) {
																	if (p[id].money<0) {
																		if ((int)((max_houses+1)*t[i].values[7])/2>abs(p[id].money)) {
																			if (p[id].oweto>0) {
																				p[p[id].oweto].money+=abs(p[id].money);
																				display_money (p[id].oweto);
																			}
																		}
																		else {
																			if (p[id].oweto>0) {
																				p[p[id].oweto].money+=(int)((max_houses+1)*t[i].values[7])/2;
																				display_money (p[id].oweto);
																			}	
																		}
																	}
																	p[id].money+=((max_houses+1)*t[d].values[7])/2;
																}
																else {
																	p[id].money+=((max_houses+1)*t[d].values[7]);
																}
																p[id].prop[d][1]-=0x00010000;
															}
															else {
																// C'è solo un albergo,
																// se viene tolto, bisogna
																// mettere 4 case
																// Se c'è più di 1 albergo
																// sulle altre case, essi 
																// vanno tolti, rimborsando
																// il giocatore of course
																for (i=0,j=0;i<28;i++) {
																	if (t[c2].col==t[i].col) {
																		if (d!=i) {
																			// Se c'è più di un albergo (2)
																			// ne viene tolto 1
																			if (p[id].prop[i][1]>0x00010000) {
																				if (prevh[j]>p[id].prop[i][1]-0x00010000) {
																					if (p[id].money<0) {
																						if ((int)((max_houses+1)*t[i].values[7])/2>abs(p[id].money)) {
																							if (p[id].oweto>0) {
																								p[p[id].oweto].money+=abs(p[id].money);
																								display_money (p[id].oweto);
																							}
																						}
																						else {
																							if (p[id].oweto>0) {
																								p[p[id].oweto].money+=(int)((max_houses+1)*t[i].values[7])/2;
																								display_money (p[id].oweto);
																							}
																						}
																					}
																					p[id].money+=((max_houses+1)*t[i].values[7])/2;
																				}
																				else {
																					p[id].money+=((max_houses+1)*t[i].values[7]);
																				}
																				p[id].prop[i][1]-=0x00010000;
																			}
																		}
																		else {
																			k=j;
																		}
																		j++;
																	}
																}
																if (prevh[k]>max_houses) {
																	if (p[id].money<0) {
																		if (t[d].values[7]/2>abs(p[id].money)) {
																			if (p[id].oweto>0) {
																				p[p[id].oweto].money+=abs(p[id].money);
																				display_money (p[id].oweto);
																			}
																		}
																		else {
																			if (p[id].oweto>0) {
																				p[p[id].oweto].money+=t[d].values[7]/2;
																				display_money (p[id].oweto);
																			}
																		}
																	}
																	p[id].money+=t[d].values[7]/2;
																}
																else {
																	p[id].money+=t[d].values[7];
																}
																p[id].prop[d][1]=max_houses;
															}
														}
														display_houses (dir);
														display_money (id);
													}		
												}
											}
										}
										else if (a.button.x>683 && a.button.x<814) {
											// Pulsante reset
											if (a.button.y>360 && a.button.y<380) {
												for (i=0,j=0;i<28;i++) {
													if (t[c2].col==t[i].col) {
														p[id].prop[i][1]=prevh[j++];
													}
												}
												p[id].money=prevh[10];
												display_houses (dir);
												display_money (id);
											}
											// Pulsante DONE
											else if (a.button.y>390 && a.button.y<410) {
												s=1;
												_clear ();
											}
										}	
									}

								default:
									break;
							}
						}
						break;
					default:
						break;
				}
			}
			break;
		case 1:
			// Tutta la roba di AI
			break;
		default:
			break;
	}
}

void init_GUI () {
	int i,d;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_putenv("SDL_VIDEO_CENTERED=center");
	SDL_WM_SetCaption("MonopoLinux BETA", NULL);
	screen=SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_EnableUNICODE(1);
	TTF_Init();
	verdana = TTF_OpenFont("Verdana.ttf", 11); 
}
