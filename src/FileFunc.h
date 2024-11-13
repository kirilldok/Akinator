#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h>
#include <fstream>
#include <assert.h>
#include <string.h>

int GetFileSize(FILE *finput);

void *ReadFile(FILE *finput, char *str, const int numberBytesSize);

#endif // FILE_OPERATIONS_H
