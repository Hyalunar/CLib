#ifndef DEFAULTIO_H
#define DEFAULTIO_H

#include <stdbool.h>

char* readLine(const char* prompt);
char* readFile(FILE* file);
char* nextLineStart(char* src);
void* tryAlloc(int size, int fail, const char* prompt);

#endif
