int load_key(char *, char *, int);
int load_key_by_tag(char *, char *, char *, int);
char * get_config_filename(char *);
int proc_file(char *, FILE *, char *, int);

#define CFG_KEY_RETRIEVED 0
#define CFG_MISC_ERROR 1
#define CFG_FILE_OPEN_ERROR 2
#define CFG_INVALID_FILE_PERMS 3