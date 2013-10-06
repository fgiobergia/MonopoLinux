// Carica tutte le informazioni necessarie da config.txt

int load_config (char *dir) {
	FILE *fp;
	int v;
	char *path,line[1024],*ptr;
	char list[16][20]={	{"money_at_start"},
				{"free_parking_money"},
				{"money_per_lap"},
				{"land_on_start"},
				{"prop_at_beginning"},
				{"prop_values_sub"},
				{"max_houses"},
				{"max_hotels"},
				{"houses_available"},
				{"hotels_available"},
				{"luxury_tax"},
				{"income_tax"},
				{"bail"},
				{"jail_turns"},
				{"community_chest"},
				{"chance"}
			  };
	path = malloc (strlen(dir)+12);
	sprintf (path,"%s/config.txt",dir);
	fp = fopen (path,"r");
	if (fp==NULL) {
		return -1;
	}
	while (!feof (fp)) {
		fgets (line,sizeof(line),fp);
		ptr=trim_spaces (line);
		// il carattere '#' è usato per i commenti,
		// viene tutto ignorato quindi
		if (ptr[0]!='#' && ptr[0]!=0) {
			if (strchr(ptr,'=')==NULL) {
				printf ("Sintassi sbagliata nel file config.txt\n");
				return -1;
			}
			v=atoi(strchr(ptr,'=')+1);
			if (!strncmp(list[0],ptr,strlen(list[0]))) {
				money_at_start=v;
			}
			else if (!strncmp(list[1],ptr,strlen(list[1]))) {
				free_parking_money=v;
			}
			else if (!strncmp(list[2],ptr,strlen(list[2]))) {
				money_per_lap=v;
			}
			else if (!strncmp(list[3],ptr,strlen(list[3]))) {
				land_on_start=v;
			}
			else if (!strncmp(list[4],ptr,strlen(list[4]))) {
				prop_at_beginning=v;
			}
			else if (!strncmp(list[5],ptr,strlen(list[5]))) {
				prop_values_sub=v;
			}
			else if (!strncmp(list[6],ptr,strlen(list[6]))) {
				max_houses=v;
			}
			else if (!strncmp(list[7],ptr,strlen(list[7]))) {
				max_hotels=v;
			}
			else if (!strncmp(list[8],ptr,strlen(list[8]))) {
				houses_available=v;
			}
			else if (!strncmp(list[9],ptr,strlen(list[9]))) {
				hotels_available=v;
			}
			else if (!strncmp(list[10],ptr,strlen(list[10]))) {
				luxury_tax=v;
			}
			else if (!strncmp(list[11],ptr,strlen(list[11]))) {
				income_tax=v;
			}
			else if (!strncmp(list[12],ptr,strlen(list[12]))) {
				bail=v;
			}
			else if (!strncmp(list[13],ptr,strlen(list[13]))) {
				jail_turns=v;
			}
			else if (!strncmp(list[14],ptr,strlen(list[14]))) {
				community_chest=v;
			}
			else if (!strncmp(list[15],ptr,strlen(list[15]))) {
				chance=v;
			}
			
		}
	}
	return 0;
}
			
			
				
