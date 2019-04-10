typedef char *(*get_config_filename_handler)(char *);
typedef int (*parse_key_file_handler)(char *, cfg_file_key_handler, void *);
typedef int (*verf_key_handler)(char *);

int output_code_from_args(int, char *[], char *, FILE *, get_config_filename_handler gcf, parse_key_file_handler lkbt, verf_key_handler vk);
