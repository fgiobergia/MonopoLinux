/*# id type data
# S start [none]
# P property [id 0-27]
# C community chest [none]
# I Income tax [none]
# H Chance [none]
# J Jail [none]
# F Free Parking [none]
# G Goto Jail [none]
# L Luxury Tax [none]
*/

struct board {
	int id;
	char type;
	int v;
};

struct board *b;

int load_board (char *dir) {
	FILE *fp;
	char *path,line[1024];
	int i;
	path = malloc (strlen(dir)+11);
	sprintf (path,"%s/board.txt",dir);
	fp = fopen (path,"r");
	if (fp==NULL) {
		return -1;
	}
	b=malloc(40*sizeof(struct board));
	i=0;
	while (true) {
		if (fgets(line,sizeof(line),fp)==NULL || i==40) {
			break;
		}
		// Se la riga inizia con #,
		// è un commento e viene ignorato
		if (line[0]!='#') {
			b[i].id=i;
			b[i].type=line[0];
			if (b[i].type=='P') {
				sscanf (line+2,"%d",&b[i].v);
			}
			else {
				b[i].v=-1;
			}
			switch (b[i].type) {
				case 'S':
				case 'P':
				case 'C':
				case 'I':
				case 'H':
				case 'J':
				case 'F':
				case 'G':
				case 'L':
					break;
				default:
					return -1;
					break;
			}
			i++;
		}
	}
}
		
	
