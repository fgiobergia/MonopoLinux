

int load_card (char *dir, char *type, int card, int player) {
	FILE *fp;
	int i,d,l,tmp,j,v,n,r,k,du[2];
	char **varname;
	int *varval;
	int /***/condition;
	char *cond;
	int ret;
	char *path,line[1024],title[1024],*cl,*var,tm[2];
	path = malloc (strlen(dir)+10+strlen(type));
	sprintf (path,"%s/%s/%d.txt",dir,type,card);
	fp = fopen (path,"r");
	if (fp==NULL)
		return -1;
	fgets (title,sizeof(title),fp);
	// Viene mostrato il titolo; finchè non si clicka non va via
	for (i=0,d=0,l=0;i<strlen(title)-1;i++) {
		tm[1]=0;
		tm[0]=title[i];
		l+=display (390+14*d,598+5+l,tm,0);
		if (l>285) {
			d++;
			l=0;
		}
//		display (370+10*d,598+5+l,tm,0);
	}
	display (390+14*(++d),598+5,"Click anywhere to continue",0);
	display (390+14*d,598+6,"Click anywhere to continue",0);
	SDL_Flip(screen);
	i=1;
	while (i) {
		while (SDL_PollEvent(&a)) {
			switch (a.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (a.button.button==1) {
						i=0;
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
	rectangle (390,598+1,496,900,0xFFFFFF);
	SDL_Flip(screen);
	i=0;
	d=0;
	varname=malloc(1*sizeof(char*));
	varval=malloc(1*sizeof(int));
//	conditions=malloc(d*sizeof(int));
	l=2;
	ret=0;
	while (1) {
		// legge fino a fine file (finchè fgets () non ritorna NULL)
		if (fgets (line,sizeof(line),fp)==NULL)
			break;
		cl=trim_spaces(line);
		// La riga non è un commento, quindi
		// viene interpretata
		if (cl[0]!='#') {
			// si tratta di una condizione
			if (!strncmp (cl,"if(",3)) {
				// se la condizione non è scritta come
				// if (condizione){
				// allora viene ritornato un errore
				if (!strstr (cl+3,"){")) {
					printf ("Errore nella if riga %d\n",l);
					return -2;
				}
				cond=malloc(strstr(cl+3,"){")-cl+4);
				strncpy (cond,cl+3,strstr(cl+3,"){")-cl+3);
				cond[strstr(cl+3,"){")-cl+3]=0;
				for (tmp=0;(cond[tmp]>='a' && cond[tmp]<='z');tmp++);
				//prende il nome della variabile
				var=malloc(tmp+1);
				strncpy (var,cond,tmp);
				var[tmp]=0;
				tmp=atoi(cond+strlen(var)+2);
				for (j=0;j<i;j++) {
					if (!strcmp(varname[j],var))
						break;
				}
				if (j==i) {
					printf ("Variabile %s non trovata riga %d\n",var,l);
					return -2;
				}
				d++;
//				conditions=realloc(conditions,d);
				if (!strncmp(cond+strlen(var),"<<",2))
					condition/*[d-1]*/=(varval[j]<tmp);
				else if (!strncmp(cond+strlen(var),">>",2))
					condition=(varval[j]>tmp);
				else if (!strncmp(cond+strlen(var),"<=",2))
					condition=(varval[j]<=tmp);
				else if (!strncmp(cond+strlen(var),">=",2))
					condition=(varval[j]>=tmp);
				else if (!strncmp(cond+strlen(var),"==",2))
					condition=(varval[j]==tmp);
				else if (!strncmp(cond+strlen(var),"!=",2))
					condition=(varval[j]!=tmp);
				else {
					printf ("Operatore sconosciuto riga %d\n",l);
					return -2;
				}
				if (!condition) {
					while (strcmp (cl,"else{")) {
						free (cl);
						fgets (line,sizeof(line),fp);
						cl=trim_spaces (line);
					}
//					free(cl);
				}
				free (cond);
				free (var);
			}
			// Niente di importante, solo un
			// if terminato
			else if (cl[0]=='}') {
			}
			// Questo è un else che non dev'essere
			// eseguito, perchè se dovesse essere stato
			// eseguito sarebbe stata cancellata la riga
			// che lo rappresenta. Da qua fino al prossimo
			// } viene cancellato tutto.
			else if (!strncmp (cl,"else{",5)) {
				while (cl[0]!='}') {
					free(cl);
					fgets (line,sizeof(line),fp);
					cl=trim_spaces (line);
				}
			}
			else if (strchr(cl,'=')) {
				for (tmp=0;(cl[tmp]>='a' && cl[tmp]<='z');tmp++);
				if (cl[tmp]!='=') {
					printf ("Sintassi errata riga %d\n",l);
					return -2;
				}
				var=malloc(tmp+1);
				strncpy (var,cl,tmp);
				var[tmp]=0;
/*					
int dice (void)		-> return a random value between 2 and 12 (sum of two number between 1 and 6)
int getmoney (void)	-> return the amount of money of the player
int gethouses (void)	-> return the amount of houses owned by the player
int gethotels (void)	-> return the amount of hotels owned by the player*/
				// funzione che richiede un numero random
				if (!strncmp(cl+strlen(var)+1,"dice()",6)) {
					for (k=0;k<2;k++) {
						for (j=0;j<7;j++) {
							show_dice (10,790+k*55);
							SDL_Flip (screen);
							usleep (100000);
						}
						du[k]=show_dice (10,790+k*55);
						SDL_Flip(screen);
					}
					usleep (500000);
					rectangle (9,789,70,899,0xFFFFFF);
					v=du[0]+du[1];
				}
				else if (!strncmp(cl+strlen(var)+1,"getmoney()",10)) {
					v=p[player].money;
				}
				else if (!strncmp(cl+strlen(var)+1,"gethouses()",11)) {
					// conta le case
					for (tmp=0,j=0;j<28;j++) {
						if (t[j].type=='P') {
							tmp += p[player].prop[j][1]&0x0000FFFF;
						}
					}
					v=tmp;
				}
				else if (!strncmp(cl+strlen(var)+1,"gethotels()",11)) {
					for (tmp=0,j=0;j<28;j++) {
						if (t[j].type=='P') {
							tmp += (p[player].prop[j][1]&0xFFFF0000)>>16;
						}
					}
					v=tmp;
					// conta gli hotel =)
				}
				else if (!strncmp(cl+strlen(var)+1,"getpos()",8)) {
					v=p[player].pos;
				}
				else {
					v=0;
					d=1;
					for (r=strlen(var)+1,j=strlen(var)+1;r<=strlen(cl);r++) {
						if (cl[r]=='+' || cl[r]=='-' || cl[r]=='*' || cl[r]=='/' || !cl[r]) {
							cond=malloc(r-j);
							strncpy (cond,cl+j,r-j);
							cond[r-j]=0;
							// controlla se si tratta di una variabile
							// o di un numero
							for (j=0,tmp=1;j<strlen(cond);j++) {
								if (cond[j]<'a' || cond[j]>'z')
									tmp=0;
							}
							// se si tratta di una variabile,
							// ne mette il valore dentro v
							if (tmp) {
								for (j=0;j<i;j++) {
									if (!strcmp (varname[j],cond)) {
										break;
									}
								}
								if (i==j) {
									printf ("Variabile %s non trovata riga %d\n",cond,l);
									return -2;
								}
								else {
									n=varval[j];
								}
							}
							else {
								n=atoi(cond);						
							}
							switch (d) {
								case 1:
									v+=n;
									break;
								case 2:
									v-=n;
									break;
								case 3:
									v*=n;
									break;
								case 4:
									v/=n;
									break;
								default:
									break;
							}
							switch (cl[r]) {
								case '+':
									d=1;
									break;
								case '-':
									d=2;
									break;
								case '*':
									d=3;
									break;
								case '/':
									d=4;
									break;
								default:
									break;
							}
							j=r+1;
							free(cond);
						}
					}
					// c'è qualche operazione da fare
					// C - R - A - P
				}
				// Ora il valore ottenuto dev'essere messo
				// nella variabile. Se c'è già, la aggiorna,
				// altrimenti ne crea una nuova.
				for (j=0;j<i;j++) {
					if (!strcmp (varname[j],var)) {
						varval[j]=v;
						break;
					}
				}
				// se la variabile non esiste, la crea
				if (i==j) {
					i++;
					varname=realloc(varname,i*sizeof(char*));
					varname[i-1]=malloc(strlen(var));
					strcpy (varname[i-1],var);
//					varname[i-1][var]=0;
					varval=realloc(varval,i*sizeof(int));
					varval[i-1]=v;
				}
				free (var);
			}
			else {
				if (strchr(cl,')')==NULL) {
					printf ("Errore di sintassi riga %d\n",l);
					return -2;
				}
				if (!strncmp(cl,"moveto(",7)) {
					var=malloc(strchr(cl,')')-(cl+7));
					strncpy (var,cl+7,strchr(cl,')')-(cl+7));
					var[strchr(cl,')')-(cl+7)]=0;
					for (j=0;j<i;j++)
						if (!strcmp(varname[j],var))
							break;
					if (j==i) {
						// L'utente passerà dal via
						if (atoi(var)<p[player].pos) {
							p[player].money+=money_per_lap;
							display_money (player);
						}
						print_token (dir,atoi(var),player);
					}
					else {
						if (varval[j]<p[player].pos) {
							p[player].money+=money_per_lap;
							display_money (player);
						}
						print_token (dir,varval[j],player);
					}
					free (var);
					ret=10;

				}
				else if (!strncmp (cl,"advance(",8)) {
					var=malloc(strchr(cl,')')-(cl+8));
					strncpy (var,cl+8,strchr(cl,')')-(cl+8));
					var[strchr(cl,')')-(cl+8)]=0;
					for (j=0;j<i;j++)
						if (!strcmp(varname[j],var))
							break;
					if (j==i) {
						k=atoi(var);
					}
					else {
						k=varval[j];
					}
					// se k è positivo, procede,
					// sennò retrocede:
					if (k>0) {
						for (j=0,d=p[player].pos;j<k;j++) {
							d++;
							if (d==40) {
								d=0;
								p[player].money+=money_per_lap;
								display_money (player);
							}
						}
					}
					else {
						for (j=0,d=p[player].pos;j<abs(k);j++) {
							d--;
							if (d<0) {
								d=39;
								// Non vengono dati però soldi
							}
						}
					}
					ret=10;
					print_token (dir,d,player);
				}
				else if (!strcmp(cl,"injail()")) {
					print_token (dir,10,player);
					p[player].inprison=jail_turns;
					display_money (player);
					ret=5;
				}
				else if (!strncmp(cl,"money(",6)) {
					if (strchr(cl+6,',')==NULL) {
						printf ("Troppi pochi argomenti per money () riga %d\n",l);
						return -2;

					}
					var=malloc(strchr(cl+6,',')-(cl+6));
					strncpy (var,cl+6,strchr(cl+6,',')-(cl+6));
					var[strchr(cl+6,',')-(cl+6)]=0;
					for (j=0;j<i;j++)
						if (!strcmp(varname[j],var)) 
							break;
					if (i==j) {
						v=atoi(cl+6);
					}
					else {
						v=varval[j];
					}
					if (!atoi(cl+7+strlen(var))) {
						p[player].money+=v;
						// Se il giocatore deve dei soldi alla
						// banca, viene messo come debitore alla banca stessa
						if (v<0) {
							p[player].oweto=-1;
						}
					}
					else {	
						for (j=0;j<players_num;j++) {
							// Riceve soldi da tutti
							// i giocatori non in 
							// bancarotta
							if (!p[j].bankrupt) {
								p[player].money+=v;
								p[j].money-=v;
								display_money (j);
								if (v<0) {
									// Il giocatore deve 
									// soldi agli altri
									p[player].oweto=j;
								}
								else {
									// Il giocatore deve
									// ricevere, viene 
									// messo "creditore"
									p[j].oweto=player;
								}
							}
						}
					}
					free (var);
					display_money (player);
				}
				else if (!strncmp(cl,"station(",8)) {
					for (j=p[player].pos+1;j!=p[player].pos;j++) {
						if (j==40) {
							/* Vengono presi i soldi
							 * dal via
							 */
							p[player].money+=money_per_lap;
							display_money (player);
							j=0;
						}
//						p[player].pos=j;
						if (b[j].type=='P' && t[b[j].v].type=='S') {
							break;
						}
					}
					print_token (dir,j,player);
					// Il giocatore ora si trova su una
					// stazione
					var=malloc(strchr(cl,')')-(cl+8));
					strncpy (var,cl+8,strchr(cl,')')-(cl+8));
					var[strchr(cl,')')-(cl+8)]=0;
					for (j=0;j<i;j++)
						if (!strcmp(varname[j],var))
							break;
					if (j==i) {
						v=atoi(var);
					}
					else {
						v=varval[j];
					}
					// v=numero di volte da pagare
					
					
					// Viene assicurato che il giocatore sia
					// su una stazione (se non c'è alcuna 
					// stazione non viene fatto)
					if (t[b[p[player].pos].v].type=='S') {
						for (j=0;j<players_num;j++) {
							if (p[j].prop[b[p[player].pos].v][0]==1) {
								break;
							}
						}
						if (j!=players_num && j!=player) {
							// La stazione è di un altro giocatore
							// e non è ipotecata, quindi viene fatta
							// pagare la somma richiesta
							for (d=0,k=0;d<28;d++) {
								if (p[j].prop[d][0]==1 && t[d].type=='S') {
									k++;
								}
							}
							if (k<5) {
								p[player].money-=(t[b[p[player].pos].v].values[k-1])*v;
								d=(t[b[p[player].pos].v].values[k-1])*v;
							}
							else {
								p[player].money-=(abs((t[b[p[player].pos].v].values[3]-t[b[p[player].pos].v].values[2])*(k-3)+t[b[p[player].pos].v].values[2]))*v;
								d=(abs((t[b[p[player].pos].v].values[3]-t[b[p[player].pos].v].values[2])*(k-3)+t[b[p[player].pos].v].values[2]))*v;
							}
							if (p[player].money>=0) {
								p[j].money+=d;
							}
							else {
								p[j].money+=d+p[player].money;
								p[player].oweto=j;
							}
							p[player].oweto=j;
							display_money (j);
							display_money (player);
						}
					}
					/*
					 * Tutta la parte in cui la stazione
					 * viene comprata oppure si paga il doppio
					 */
					free (var);
				}
				else if (!strncmp(cl,"utility(",8)) {
					for (j=p[player].pos+1;j!=p[player].pos;j++) {
						if (j==40) {
							p[player].money+=money_per_lap;
							display_money (player);
							j=0;
						}
//						p[player].pos=j;
						if (b[j].type=='P' && t[b[j].v].type=='U') {
							break;
						}
					}
					print_token (dir,j,player);
					var=malloc(strchr(cl,')')-(cl+8));
					strncpy (var,cl+8,strchr(cl,')')-(cl+8));
					var[strchr(cl,')')-(cl+8)]=0;
					for (j=0;j<i;j++)
						if (!strcmp(varname[j],var))
							break;
					if (j==i) {
						v=atoi(var);
					}
					else {
						v=varval[j];
					}
					// v=numero di volte da pagare
					printf ("%c\n",t[b[p[player].pos].v].type);
					if (t[b[p[player].pos].v].type=='U') {
						for (j=0;j<players_num;j++) {
							if (p[j].prop[b[p[player].pos].v][0]==1) {
								break;
							}
						}
						if (j!=players_num && j!=player) {
							for (d=0,k=0;d<28;d++) {
								if (p[i].prop[d][0]==1 && t[d].type=='U') {
									k++;
								}
							}
							for (k=0;k<2;k++) {
								for (j=0;j<7;j++) {
									show_dice (10,790+k*55);
									SDL_Flip (screen);
									usleep (100000);
								}
								du[k]=show_dice (10,790+k*55);
								SDL_Flip(screen);
							}
							usleep (500000);
							rectangle (9,789,70,899,0xFFFFFF);
							printf ("%d %d\n",du[0],du[1]);
							p[player].money-=v*(du[0]+du[1]);
							if (p[player].money>=0) {
								p[j].money+=v*(du[0]+du[1]);
							}
							else {
								p[j].money+=p[player].money+v*(du[0]+du[1]);
								p[player].oweto=j;
							}
							display_money (player);
							display_money (j);
						}
					}
					free (var);
					/*
					 * Tutta la parte in cui l'utility
					 * viene comprata oppure si lanciano i dadi 
					 * e si paga l'ammontare
					 */


				}
				else if (!strcmp(cl,"outofjail()")) {
					// massimo "esci gratis di prigione"
					// consentiti = 2
					if (p[player].outpr<2) {
						p[player].outpr++;
					}
				}
				/*
				void moveto (int p)	-> send the player to a desired position p
				void advance (int p)	-> send the player p steps forward (backward if p<0)
				void injail (void)	-> send the player to prison
				void money (int n,int p)-> increase player of n $ (if n<0, the money is subtracted). if p=0, the money is taken/given to the bank, if p=1 									the money is taken/given to each player
				void station (int p)	-> advance to the next station, if free the player can buy it, otherwise he pays p times the value
				void utility (void)	-> advance to the next utility, if free the player can buy it, otherwise the dice are rolled and the value is paid
				void outofjail (void)	-> gives a "get out of jail free" ticket
				*/
			}
		}
		l++;
	}
	free(cl);
	free (path);
	for (j=0;j<i;j++) {
		free (varname[j]);
	}
	free (varname);
	free (varval);
	return ret;
}


/*Cards programming language

Message:
A message has to be showed on the card. The first line will be this message.

Comments:
Each line starting with a '#' will be considered as a 
comment and will not be read.

Variables:
name = value

Operations:

+ - * /

Functions list:
int dice (void)		-> return a random value between 2 and 12 (sum of two number between 1 and 6)
int getmoney (void)	-> return the amount of money of the player
int gethouses (void)	-> return the amount of houses owned by the player
int gethotels (void)	-> return the amount of hotels owned by the player
int getpos (void)	-> return the player's position
void moveto (int p)	-> send the player to a desired position p
void advance (int p)	-> send the player p steps forward (backward if p<0)
void injail (void)	-> send the player to prison
void money (int n,int p)-> increase player of n $ (if n<0, the money is subtracted). if p=0, the money is taken/given to the bank, if p=1 the money is taken/given to each player
void station (int p)	-> advance to the next station, if free the player can buy it, otherwise he pays p times the value
void utility (int p)	-> advance to the next utility, if free the player can buy it, otherwise the dice are rolled and p times the value is paid.
void outofjail (void)	-> gives a "get out of jail free" ticket

Conditions:
if (condition) {
	...actions...
}
else {
	...other actions...
}

condition -> comparison between a variable and a number (<<,>>,<=,>=,==,!=)*/
