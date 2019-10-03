#ifndef OPTION_INCLUDE
#define OPTION_INCLUDE

typedef enum ftype_t{
	BIN,
	TXT,
}FTYPE;

typedef struct option_t {
	FTYPE ftype_instr;
	FTYPE ftype_data;
	char *fname_instr;
	char *fname_data;
}OPTION;

void option_init(OPTION *option);
void option_set(int argn, char **arg, OPTION *option);
void option_free(OPTION *option);

void c2b(uint8_t *dest, char *source, size_t size);

#endif
