/*
 * Ogni giocatore è definito con una struttura.
 * Questa struttura conterrà tutte le informazioni relative al giocatore stesso.
 */

//typedef enum {false,true} bool;

struct player {
	int id;
	int token; // id (o hex?) del colore della pedina
	int ai; // se settato a 1, il giocatore viene controllato da AI
	int pos; // indica la posizione sul percorso del giocatore
	int money; // indica la quantità di soldi del giocatore
	int prop[28][2]; // array che contiene nel primo campo un valore (0,1,-1) a seconda che il giocatore
			 // possegga o meno il terreno ed esso sia ipotecato, più le case (valore 1 suddiviso dai bit)
			 // (16 bit più alti=hotel, 16 bit + bassi = case)
	int outpr; // contiene il numero di "esci di prigione" presi
	int inprison; // se != 0, il giocatore deve passare in prigione ancora n turni
	int bankrupt; // se = 1, il giocatore è andato in bancarotta
	int oweto; // oweto contiene l'id dell'ultimo giocatore a cui
		   // il giocatore deve dei soldi. In caso di bancarotta, le proprietà
		   // passano al giocatore che deve ricevere i soldi. Se settato a -1
		   // i soldi vanno alla banca, quindi le proprietà etc vengono rimesse
		   // in vendita
};

struct player *p;

int new_players (int num, int *color, int *ai, int startmoney, unsigned char props) {
	// int num: numero di giocatori
	// int *color: elenco dei colori, in ordine, di ogni giocatore
	// char *ai: se settato a 1, indica che il giocatore è un AI
	// int startmoney: indica la quantità di soldi iniziale di ogni giocatore
	// char porps:	il bit più alto, se settato, indica che i soldi delle proprietà iniziali vanno detratti
	//		da quelli iniziali. Gli altri 7 bit indicano quante proprietà spettano a ciascun giocatore
	//		(se il valore è più alto di quelle assegnabili, ne viene assegnato il numero massimo.
	
/*>>>*/	srand (time(NULL)); // TEMPORANEOOOO

	int i,d,propnum,n,x,k,a;
	if (num<2)
		return -1; // Non è possibile giocare con meno di 2 giocatori
	/* CONTROLLO SU NUMERO MASSIMO DI GIOCATORI ? */
	p = malloc (num*sizeof(struct player));
	for (i=0;i<num;i++) {
		p[i].id=i; // id
		p[i].token=color[i]; // colore 
		p[i].ai=ai[i]; // se si tratta o meno di un ai
		p[i].pos=0; // Partenza = 0
		p[i].money = startmoney; // soldi iniziali
		propnum = (num*(props&0x7F)<29)?props&0x7F:28/num; // 01111111
		// inizializza l'array delle
		// proprietà a 0
		for (d=0;d<28;d++) {
			p[i].prop[d][0]=0;
			p[i].prop[d][1]=0;
		}
		// assegna il numero di proprietà
		// richiesto, scegliendole casualmente
		// tra quelle disponibili
		for (d=0;d<propnum;d++) {
			do {
				a=rand()%28;
				k=1;
				// controlla se la proprietà è disponibile
				for (n=0;n<=i;n++) {
					k -= p[n].prop[a][0];
				}
			} while (k!=1);
			// se il territorio è disponibile e
			// si è scelto dalle impostazioni di
			// sottrarre il valore della proprietà
			// da quelli iniziali, si tolgono i soldi.
			// (nota, si può andare in debito anche solo
			// con l'assegnazione dei terreni, se non
			// vengono dati soldi a sufficienza.
			if (k && props&0x80) {
				switch (t[a].type) {
					case 'P':
						x=t[a].values[6]*2;
						break;
					case 'S':
						x=t[a].values[4]*2;
						break;
					case 'U':
						x=t[a].values[2]*2;
						break;
					default:
						break;
				}
				p[i].money -= x;
			}
			// imposta la proprietà
			p[i].prop[a][0]=1;
		}
		p[i].outpr=0;
		p[i].inprison=0;
		p[i].bankrupt=0;
		p[i].oweto=-1;
	}
}


					
			
			
		
			
		
	
