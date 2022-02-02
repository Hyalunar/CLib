#include <stdio.h>
#include <stdlib.h>

char* readLine(char* prompt)
{
	int capacity = 100;
	int pos = 0;
	char* inputString = malloc(sizeof(char) * capacity);
	if (!inputString) return NULL;
	char inChar = '\0';
	printf("%s", prompt);
	while (inChar != '\n') {
		inChar = getchar();
		if (pos + 1 == capacity) {
			int newCapacity = capacity * 2;
			char* newString = realloc(inputString, sizeof(char) * newCapacity);
			if (!newString) return NULL;
			inputString = newString;
			capacity = newCapacity;
		}
		inputString[pos] = inChar;
		pos++;
	}
	inputString[pos - 1] = '\0';
	return inputString;
}

char* readFile(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int capacity = ftell(file) + 1;
	fseek(file, 0, SEEK_SET);
	int pos = 0;
	char* fileString = malloc(sizeof(char) * capacity);
	if (!fileString) return NULL;
	int res = fread(fileString, capacity, 1, file);
	fileString[pos] = 0;
	return fileString;
}
