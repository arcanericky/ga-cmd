typedef int (*cfg_file_key_handler)(char *, char *, void *user_data);

int load_key(char *, char *, int);
int parse_key_file(char *, cfg_file_key_handler, void *user_data);
char * get_config_filename(char *);
int proc_file(FILE *, cfg_file_key_handler, void *);

#define CFG_KEY_FOUND 0
#define CFG_KEY_NOT_FOUND 1
#define CFG_FILE_OPEN_ERROR 2
#define CFG_INVALID_FILE_PERMS 3