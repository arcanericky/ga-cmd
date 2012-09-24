int verf_key(char *);
int verf_key_len(char *);
int verf_key_chars(char *);

int hide_key(char *, char *, int);
int reveal_key(char *);

int gen_verf_code(char *, int);

int load_key(char *, char *, int);
char * get_config_filename(char *);
