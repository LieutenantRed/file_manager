#include "graphics.h"

int main(int argc, char** argv) {
	char* current_path = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	strncpy(current_path, argv[0], BUFFER_SIZE);

	init_display();
	display_navigation(current_path);
	end_display();

	return 0;
}