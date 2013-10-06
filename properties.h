struct property {
	int id;	// id della proprietà (0-27)
	char type; // tipo di proprietà (P: terreno, S: stazione, U: società)
	char *name; // nome della proprietà
	int *values;	// contiene tutti i valori relativi al tipo di proprietà:
			// P: 	- costo passaggio
			//	- costo con 1 casa
			//	- costo con 2 case
			//	- costo con 3 case
			//	- costo con 4 case
			//	- costo con 1 albergo (il prezzo con eventuali alberghi in
			//	  più viene calcolato automaticamente dai valori precedenti)
			//	- prezzo ipoteca
			//	- costo per casa (ogni albergo successivo al primo costerà 5x questo valore)
			//
			// S:	- costo passaggio con 1 stazione
			//	- costo passaggio con 2 stazioni
			//	- costo passaggio con 3 stazioni
			//	- costo passaggio con 4 stazioni
			//	- prezzo ipoteca
			//
			// U:	(i due valori seguenti vanno considerati come fattore con cui
			//	moltiplicare il numero tirato con i dadi. Questo sarà il valore
			//	pagato al passaggio)
			//	- costo passaggio possedendo una sola società
			//	- costo passaggio con entrambe le società
			//	- prezzo ipoteca
			//
			// NOTA: il valore originario di una proprietà sarà 2x questo valore
	int col; // colore (solo per P), per U viene usato per indicare se è electric(0) o water(1)
};

struct property *t;

int load_properties (char *dir) {
	FILE *fp;
	char *path, line[1024],tmp[1024];
	int i,d;
	path = malloc (strlen(dir)+16);
	sprintf (path,"%s/properties.txt",dir);
	fp = fopen (path,"r");
	if (fp==NULL)
		return -1;
	t = malloc (28*sizeof(struct property));
	for (i=0;i<28;) {
		fgets (line,sizeof(line),fp);
		// Controlla che non si tratti di un commento
		// (identificato da un '#' come primo carattere
		if (line[0]!='#') {
			t[i].id=i;
			sscanf (line,"%c",&t[i].type);
			if (t[i].type!='P' && t[i].type!='S' && t[i].type!='U')
				return -1;
			t[i].name=malloc(strchr(line+4,'-')-(line+4)+1);
			printf ("%d -> ",strchr(line+4,'-')-(line+4));
			strncpy (t[i].name,line+4,strchr(line+4,'-')-(line+4));
			t[i].name[strchr(line+4,'-')-(line+4)-1]=0;
//			t[i].name[strlen(t[i].name)-1]=0;
			printf ("|%s|\n",t[i].name);
			// a seconda del tipo di proprietà,
			// prende il numero di argomenti necessari.
			switch (t[i].type) {
				case 'P':
					t[i].values=malloc(8*sizeof(int));
					sscanf (line+6+strlen(t[i].name),"%d - %d - %d - %d - %d - %d - %d - %d - %x",&t[i].values[0],
														      &t[i].values[1],
														      &t[i].values[2],
														      &t[i].values[3],
														      &t[i].values[4],
														      &t[i].values[5],
														      &t[i].values[6],
														      &t[i].values[7],
														      &t[i].col
						);
					break;
				case 'S':
					t[i].values=malloc(5*sizeof(int));
					sscanf (line+6+strlen(t[i].name),"%d - %d - %d - %d - %d",&t[i].values[0],
												  &t[i].values[1],
												  &t[i].values[2],
												  &t[i].values[3],
												  &t[i].values[4]
							);
					break;
				case 'U':
					t[i].values=malloc(3*sizeof(int));
					sscanf (line+6+strlen(t[i].name),"%d - %d - %d - %d",&t[i].values[0],
											     &t[i].values[1],
											     &t[i].values[2],
											     &t[i].col
							);
					break;
				default:
					break;
			}
			i++;
		}
	}
	return 0;
}

