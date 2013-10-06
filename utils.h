typedef enum {false, true} bool;

char *trim_spaces (char *str) {
	int i,d;
	char *n;
	n=malloc(1);
	for (i=0,d=0;i<=strlen(str);i++) {
		// Elenco dei caratteri "non voluti"
		if (str[i]!=' ' && str[i]!='\n' && str[i]!='\t') {
			d++;
			n=realloc(n,d);
			n[d-1]=str[i];
		}
	}
	return n;
}	
