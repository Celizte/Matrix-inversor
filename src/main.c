#include "matrix.h"
#include "input.h"
#include "utilities.h"
#include "random_quotes.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define AUTHOR "Celizte"

#ifndef M_VERSION
	#define M_VERSION "Placeholder version"
	#warning "Using placeholder version"
#endif

#define MAX_BUFFER_SIZE 32

void signal_handler(int sig) {
	printf("\nBye!!\n");
	fflush(stdout);
	exit(0);
}

int main(void) {
	signal(SIGINT, signal_handler);
	srand(time(NULL));

	clear_screen();

	printf(ANSI_COLOR_YELLOW 	"Matrix Inversor, using the Gauss–Jordan method\n" 	ANSI_COLOR_RESET);
	printf(ANSI_COLOR_MAGENTA 	"Version: " 										ANSI_COLOR_RESET);
	printf(ANSI_COLOR_CYAN 		"%s\n\n" 											ANSI_COLOR_RESET, M_VERSION);

	printf(ANSI_COLOR_MAGENTA 	"Made by @%s, you will probably find a better program\n\n" 	ANSI_COLOR_RESET, AUTHOR);

	printf(ANSI_COLOR_RED		"Hint: You can close the program by pressing Ctrl + C\n" 	ANSI_COLOR_RESET);

	Input input;
	bool exit = false;

	while (!exit) {
		printf("Select a square matrix size: ");
		input = inputs();

		if (strncmp(input.text, "q", 1) == 0) {
			printf("\nBye!!\n");
			fflush(stdout);
			return 0;
		}

		if (!input.hasFraction)
			continue;

		if (input.fraction.numerator <= 0) {
			printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, random_quote());
			continue;
		}

		size_t buffer = (size_t) to_int(input.fraction);

		// I don't want to allocate all of the fractions, so it goes to the stack
		// which means that any stupidly high number will just cause a seg fault, dummy
		if (buffer > MAX_BUFFER_SIZE) {
			printf(ANSI_COLOR_RED "You don't want to input all those numbers, trust me\n" ANSI_COLOR_RESET);
			continue;
		}
		Fraction fractions[buffer][buffer];

		clear_screen();

		printf("\nPlease initialize [%lux%lu] matrix:\n", buffer, buffer);
		printf(ANSI_COLOR_YELLOW "Hint: Type whole numbers as: \"13\" or fractions as \"13 / 2\"\n\n" ANSI_COLOR_RESET);

		for (size_t i = 0; i < buffer; i++) {
			for (size_t j = 0; j < buffer; j++) {
				printf("[%lu, %lu]: ", i + 1, j + 1);

				input = inputs();

				if (!input.hasFraction) {
					printf("\nPlease make sure that it's written in the correct format!\n");
					j--;
					continue;
				}

				fractions[i][j] = input.fraction;
			}
		}

		Matrix *m = create_matrix(buffer, fractions);
		invert_matrix(m);
		free_matrix(m);

		printf("\n");
	}

	return 0;
}
