
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
