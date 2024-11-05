#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>

int main() {
	char* args[] = {"cat", "main.cpp", "&&", "makefile", NULL};
	execvp(args[0], args);
	return 0;
}
