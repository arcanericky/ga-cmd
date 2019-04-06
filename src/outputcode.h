typedef char *(*get_config_filename_handler)(char *);
typedef int (*load_key_by_tag_handler)(char *, char *, char *, int);
typedef int (*verf_key_handler)(char *);

int output_code_from_args(int, char *[], char *, FILE *, get_config_filename_handler gcf, load_key_by_tag_handler lkbt, verf_key_handler vk);
