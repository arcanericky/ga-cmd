typedef int version_handler(FILE *, char *);
typedef int list_handler(FILE *);

int proc_cli_flags(int , char *[], char *, FILE *, version_handler *, list_handler *);

