#include <SDL/SDL.h>
#include <math.h>

int print_board (char *dir) {
	FILE *fp;
	char *path;
	char line[1024];
	unsigned int board[598][598];
	unsigned int c[76][48];
	int i,d,x,y,n,j,k;
	int tmp[3];
	path = malloc (strlen(dir)+11);
	sprintf (path,"%s/board.ppm",dir);
	fp=fopen (path,"r");
	if (fp==NULL) {
		return -1;
	}
	for (i=0;i<598;i++) {
		for (d=0;d<900;d++) {
			pixel (i,d,0xFFFFFF);
		}
	}
	while (1) {
		fgets (line,sizeof(line),fp);
		if (sscanf (line,"%d %d",&x,&y)==2) {
			fgets (line,sizeof(line),fp);
			break;
		}
		if (feof (fp)) {
			return -1;
		}
	}
	// controllo coordinate
	if (x!=598 || y!=598)
		return -1;
	// carica lo "scheletro" del tabellone
	for (i=0;i<y;i++) {
		for (d=0;d<x;d++) {
			fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
			board[i][d]=SDL_MapRGB (screen->format,tmp[0],tmp[1],tmp[2]);
//			pixel (screen,i,d,board[i][d]);
		}
	}
	fclose (fp);
	free (path);
	// carica le immagini delle caselle
	for (n=0;n<40;n++) {
		if (n%10) {
			switch (b[n].type) {
				// terreno, prende la casella relativa
				case 'P':
					switch (t[b[n].v].type) {
						case 'P':
							strcpy (line,"/prop.ppm");
							break;
						case 'S':
							strcpy (line,"/station.ppm");
							break;
						case 'U':
							switch (t[b[n].v].col) {
								case 0:
									strcpy (line,"/electric.ppm");
									break;
								case 1:
									strcpy (line,"/water.ppm");
									break;
								default:
									break;
							}
						default:
							break;
					}
					break;
				case 'C':
					strcpy (line,"/chest.ppm");
					break;
				case 'I':
					strcpy (line,"/income.ppm");
					break;
				case 'H':
					strcpy (line,"/chance.ppm");
					break;
				case 'L':
					strcpy (line,"/super.ppm");
					break;
				default:
					break;
			}
			path = malloc (strlen(dir)+strlen(line)+1);
			sprintf (path,"%s%s",dir,line);
			fp=fopen(path,"r");
			if (fp==NULL) {
				return -1;
			}
			while (1) {
				fgets (line,sizeof(line),fp);
				if (sscanf (line,"%d %d",&x,&y)==2) {
					fgets (line,sizeof(line),fp);
					break;
				}
				if (feof (fp)) {
					return -1;
				}
			}
			// legge le coordinate, per "sicurezza"
			if (x!=48 || y!=76)
				return -1;
			for (i=0;i<y;i++) {
				for (d=0;d<x;d++) {
					fscanf (fp,"%d %d %d",&tmp[0],&tmp[1],&tmp[2]);
					// se il pixel è bianco, lo colora del colore
					// del terreno desiderato
					if (tmp[0]==255 && tmp[1]==255 && tmp[2]==255 && t[b[n].v].type=='P') {
						if (!t[b[n].v].col);
						//	printf ("WTF? %d -> %d\n",b[n].v,t[b[n].v].values[7]);
						tmp[0]=((t[b[n].v].col)&0xFF0000)>>16;
						tmp[1]=((t[b[n].v].col)&0x00FF00)>>8;
						tmp[2]=((t[b[n].v].col)&0x0000FF);
					}
					c[i][d]=SDL_MapRGB (screen->format,tmp[0],tmp[1],tmp[2]);
				}
			}
			// se si stanno usando le prime 10
			// caselle, esse vengono stampate
			// così come vengono lette
			if (n<10) {
				for (i=520,j=0;j<76;i++,j++) {
					for (d=471-(49*(n-1)),k=0;k<48;d++,k++) {
						board[i][d]=c[j][k];
					}
				}
			}
			// dalla 10 alla 20 le caselle
			// sono stampate girandole di 90°
			else if (n<20) {
				for (j=0,d=77;j<76;j++,d--) {
					for (k=0,i=(471-(49*(n-11)));k<48;k++,i++) {
						board[i][d]=c[j][k];
					}
				}
			}
			// dalla 20 alla 30 le caselle
			// son girate di 180°
			else if (n<30) {
				for (j=0,i=77;j<76;j++,i--) {
					for (k=0,d=(126+(49*(n-21)));k<48;k++,d--) {
						board[i][d]=c[j][k];
					}
				}
			}
			// dalla 30 alla 40 son girate
			// 270°
			else {
				for (j=0,d=520;j<76;j++,d++) {
					for (k=0,i=(126+(49*(n-31)));k<48;k++,i--) {
						board[i][d]=c[j][k];
					}
				}
			}						
			free (path);
			fclose (fp);
		}
	}
	for (i=0;i<598;i++)
		for (d=0;d<598;d++)
			pixel (i,d,board[i][d]);				
				
	
	/* TMP */
	SDL_Flip(screen);
	return 0;
}

