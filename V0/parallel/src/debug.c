#include "../headers/debug.h"

void* __SafeMalloc(size_t size, const char* file, const int line){
	void* temp = malloc(size);
	
	if(temp == NULL){
		fprintf(stderr, "%s:%d Cant allocate memory\n", file, line);
		exit(-1);
	}else{
		return temp;
	}
};

FILE* __Safefopen(const char* path, const char* mode, const char* file, const int line){
	FILE* temp = fopen(path, mode);

	if(temp == NULL){
		fprintf(stderr, "%s:%d Cant open file: %s", file, line, path);
		exit(-1);
	}
	else{
		return temp;
	}
}
