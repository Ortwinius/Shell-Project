#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
	int n_args;
	char** args;
} Command;

typedef struct {
	const char* cmd;
	const unsigned int min_args;
	const unsigned int max_args;
	void (*handler)(Command*);
}Exec;

#endif //STRUCTS_H