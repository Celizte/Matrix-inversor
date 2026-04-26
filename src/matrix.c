#include "matrix.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

Fraction sum_fraction(Fraction n1, Fraction n2) {
	if (n1.denominator == 0)
		return n2;
	else if (n2.denominator == 0)
		return n1;

	if (n1.denominator == n2.denominator)
		return simplify((Fraction) {
			.numerator		= n1.numerator + n2.numerator,
			.denominator	= n1.denominator
		});

	return simplify((Fraction) {
		.numerator		= (n1.numerator * n2.denominator) + (n2.numerator * n1.denominator),
		.denominator	= n1.denominator * n2.denominator
	});
}

Fraction sub_fraction(Fraction n1, Fraction n2) {
	if (n1.denominator == 0)
		return n2;

	if (n1.denominator == n2.denominator)
		return simplify((Fraction) {
			.numerator		= n1.numerator - n2.numerator,
			.denominator	= n1.denominator
		});

	return simplify((Fraction) {
		.numerator		= (n1.numerator * n2.denominator) - (n2.numerator * n1.denominator),
		.denominator	= n1.denominator * n2.denominator
	});

}

Fraction mult_fraction(Fraction n1, Fraction n2) {
	return simplify((Fraction) {
		.numerator 		= n1.numerator * n2.numerator,
		.denominator	= n1.denominator * n2.denominator
	});
}

Fraction div_fraction(Fraction n1, Fraction n2) {
	Fraction tmp = {
		.numerator 		= n2.denominator,
		.denominator	= n2.numerator
	};

	return mult_fraction(n1, tmp);
}

Fraction simplify(Fraction n) {
	if (n.denominator == 0)
		return (Fraction) { 0, 0 };

	int gc = gcd(n);

	Fraction f = n;
	if (f.denominator < 0) {
		f.numerator *= -1;
		f.denominator *= -1;
	}

	f.numerator /= gc;
	f.denominator /= gc;

	return f;
}

Fraction to_fraction(int n, Fraction d) {
	return (Fraction) { .numerator = n, .denominator = d.denominator };
}

bool is_int(Fraction n) {
	Fraction f = simplify(n);

	if (f.denominator == 0)
		return false;

	return (f.numerator % f.denominator) == 0 ? true : false;
}

int to_int(Fraction n) {
	Fraction f = simplify(n);

	if (f.denominator == 0)
		return 0;

	if (!is_int(f))
		return f.numerator;

	return f.numerator / f.denominator;
}

int gcd(Fraction n) {
	int nm = abs(n.numerator);
	int dm = abs(n.denominator);

	while (dm != 0) {
		int tmp = dm;
		dm = nm % dm;
		nm = tmp;
	}

	return nm;
}

void print_fraction(const Fraction n) {
	if (is_int(n))
		printf(" [%d] ", to_int(n));
	else
		printf(" [%d / %d] ", n.numerator, n.denominator);
}

void print_row(size_t size, Fraction n[]) {
	for (size_t i = 0; i < size; i++)
		print_fraction(n[i]);
}

struct Matrix {
	size_t size;				// Only supports square matrices for now

	Fraction **fractions;		// NO need to self-initialize as it just does it
};

Matrix *create_matrix(size_t size, Fraction fractions[size][size]) {
	Matrix *matrix = malloc(sizeof(Matrix));
	if (matrix == NULL) {
		perror("Could not allocate Matrix");
		return NULL;
	}

	matrix->size = size;
	matrix->fractions = malloc(size * sizeof(Fraction*));

	for (size_t h = 0; h < size; h++) {
		matrix->fractions[h] = malloc(size * sizeof(Fraction));
		if (matrix->fractions[h] == NULL) {
			perror("Could not allocate Matrix again");

			free_matrix(matrix);
			return NULL;
		}

		for (size_t j = 0; j < size; j++) {
			matrix->fractions[h][j] = fractions[h][j];
		}
	}

	return matrix;
}

void free_matrix(Matrix *self) {
	if (self == NULL)
		return;

	if (self->fractions == NULL) {
		free(self);
		return;
	}

	for (size_t i = 0; i < self->size; i++)
		if (self->fractions[i] != NULL)
			free(self->fractions[i]);

	free(self->fractions);
	free(self);
}

void print_matrix(const Matrix *matrix) {
	size_t size = matrix->size;
	Fraction **fractions = matrix->fractions;

	printf("|-----------------------------------------------|\n");

	printf(ANSI_COLOR_CYAN);
	for (size_t h = 0; h < size; h++) {
		print_row(size, fractions[h]);

		printf("\n");
	}
	printf(ANSI_COLOR_RESET);
	printf("|-----------------------------------------------|\n");
}



void invert_matrix(Matrix *self) {
	clear_screen();
	puts("Inverting matrix!");

	printf("Original Matrix:\n");
	print_matrix(self);

	size_t size = self->size;

	Fraction fractions[size][size];
	for (size_t h = 0; h < size; h++)
		for (size_t j = 0; j < size; j++)
			fractions[h][j] = self->fractions[h][j];

	Matrix *matrix = create_matrix(size, fractions);
	if (matrix == NULL) {
		perror("Could not allocate new matrix :(");
		return;
	}

	// Initializes the identity matrix array lol
	Fraction identity[size][size];
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			if (i == j)
				identity[i][j] = (Fraction) { 1, 1 };
			else
				identity[i][j] = (Fraction) { 0, 0 };
		}
	}

	Matrix *endMatrix = create_matrix(size, identity);
	if (endMatrix == NULL) {
		perror("Could not allocate identity matrix :(((");
		free_matrix(matrix);
		return;
	}

	bool complete = false;
	size_t onesIndex = 0;
	while (!complete) {
		if (onesIndex >= size) {
			complete = true;
			break;
		}

		// Checks if the number at the index ('1,1' || '2,2' || '3,3' || ...) is '1'
		// If so, then it skips to avoid doing extra unnecessary calculations (not that it matters lol)
		if (
			is_int(fractions[onesIndex][onesIndex])
			&& (to_int(fractions[onesIndex][onesIndex]) == 1)
		) goto eliminate_column;


		// Divides entire row by the number at the ones position
		Fraction divisor = simplify(fractions[onesIndex][onesIndex]);
		for (size_t i = 0; i < size; i++) {
			Fraction f = fractions[onesIndex][i];
			f = div_fraction(f, divisor);

			fractions[onesIndex][i] = f;
		}

		// Also modifies the identity matrix lol
		for (size_t i = 0; i < size; i++) {
			Fraction f = identity[onesIndex][i];
			f = div_fraction(f, divisor);

			identity[onesIndex][i] = f;
		}

		modify_matrix_row(matrix, fractions[onesIndex], onesIndex);
		modify_matrix_row(endMatrix, identity[onesIndex], onesIndex);

		printf("\n1 at (%lu, %lu) gotten by multiplying *", onesIndex + 1, onesIndex + 1);
		print_fraction(fractions[onesIndex][onesIndex]);
		printf("\n");

		print_matrix(matrix);
		print_matrix(endMatrix);

	eliminate_column: // This assumes that the number at index 'onesIndex' is a '1'

		printf("\nEliminating 0s:\n");	// This is here so i can declare variables after the label

		Fraction ogRow[size];
		Fraction ogIdt[size];

		for (size_t i = 0; i < size; i++) {
			if (i == onesIndex)
				continue;

			Fraction multiplier = simplify(fractions[i][onesIndex]);
			multiplier.numerator = -multiplier.numerator;

			printf("Eliminating 0 at index [%lu, %lu] with mult:", i + 1, onesIndex + 1);
			print_fraction(multiplier);
			printf("\n");

			for (size_t i = 0; i < size; i++) {
				ogRow[i] = fractions[onesIndex][i];
				ogIdt[i] = identity[onesIndex][i];
			}

			for (size_t j = 0; j < size; j++) {
				ogRow[j] = mult_fraction(ogRow[j], multiplier);
				ogIdt[j] = mult_fraction(ogIdt[j], multiplier);
			}

			// Print the results
			print_row(size, ogRow);
			printf(" | ");
			print_row(size, ogIdt);
			printf(" + \n");

			print_row(size, fractions[i]);
			printf(" | ");
			print_row(size, identity[i]);
			printf("\n");

			// Do the math then print it
			for (size_t j = 0; j < size; j++) {
				fractions[i][j] = sum_fraction(ogRow[j], fractions[i][j]);
				identity[i][j]	= sum_fraction(ogIdt[j], identity[i][j]);
			}

			printf("-------------------------------------------\n");
			print_row(size, fractions[i]);
			printf(" | ");
			print_row(size, identity[i]);
			printf("\n");
			printf("-------------------------------------------\n\n");

			modify_matrix_row(matrix, fractions[i], i);
			modify_matrix_row(endMatrix, identity[i], i);
		}

		onesIndex++;

		if (onesIndex >= size) {
			complete = true;
			break;
		}

		printf("Matrix number %lu:\n", onesIndex);
		print_matrix(matrix);
		print_matrix(endMatrix);


		printf(ANSI_COLOR_GREEN "Press [Enter] for next matrix: " ANSI_COLOR_RESET);
		getchar();
		clear_screen();
	}

	printf("Final Matrix: \n");
	print_matrix(matrix);
	print_matrix(endMatrix);

	free_matrix(matrix);
	free_matrix(endMatrix);
}

void modify_matrix_row(Matrix *self, Fraction *fractions, int row) {
	for (size_t i = 0; i < self->size; i++) {
		self->fractions[row][i] = fractions[i];
	}
}

// This function will only really work with a 2x2, i don't want to figure out the rest
Fraction determinant(const Matrix *self) {
	Fraction D = {0, 0};
	size_t size = self->size;

	Fraction **f = self->fractions;

	if (size == 2) {
		D = (
			sub_fraction(
				mult_fraction(f[0][0], f[1][1]),
				mult_fraction(f[0][1], f[1][0])
			)
		);

		return D;
	}


	for (int i = 0; i < size; i++) {

	}


	return D;
}
