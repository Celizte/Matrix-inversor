#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

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
	f.numerator /= gc;
	f.denominator /= gc;

	return f;
}

Fraction to_fraction(int n, Fraction d) {
	return (Fraction) { .numerator = n, .denominator = d.denominator };
}

bool is_int(Fraction n) {
	return (n.numerator == n.denominator);
}

int to_int(Fraction n) {
	return n.numerator;
}

int gcd(Fraction n) {
	int nm = abs(n.numerator);
	int dm = abs(n.denominator);

	while (nm != 0) {
		int tmp = dm;
		dm = nm % dm;
		nm = tmp;
	}

	return nm;
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

	for (int h = 0; h < size; h++) {
		matrix->fractions[h] = malloc(size * sizeof(Fraction));
		if (matrix->fractions[h] == NULL) {
			perror("Could not allocate Matrix again");

			free_matrix(matrix);
			return NULL;
		}

		for (int j = 0; j < size; j++) {
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

	for (int i = 0; i < self->size; i++)
		if (self->fractions[i] != NULL)
			free(self->fractions[i]);

	free(self->fractions);
	free(self);
}

void print_matrix(const Matrix *matrix) {
	size_t size = matrix->size;
	Fraction **fractions = matrix->fractions;

	printf("|-----------------------------------------------|");

	for (size_t h = 0; h < size; h++) {
		printf("|");
		for (size_t j = 0; j < size; j++) {
			Fraction f = fractions[h][j];

			if (is_int(f))
				printf(" [%d] ", to_int(f));
			else
				printf(" [%d/%d] ", f.numerator, f.denominator);
		}

		printf("|\n");
	}

	printf("|-----------------------------------------------|");
}



void invert_matrix(Matrix *self) {
	puts("Inverting matrix!");

	printf("Original Matrix:\n");
	print_matrix(self);

	size_t size = self->size;

	Fraction fractions[size][size];
	for (int h = 0; h < size; h++)
		for (int j = 0; j < size; j++)
			fractions[h][j] = self->fractions[h][j];

	Matrix *matrix = create_matrix(size, fractions);
	if (matrix == NULL) {
		perror("Could not allocate new matrix :(");
		return;
	}

	// Initializes the identity matrix array lol
	Fraction identity[size][size];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
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
	int onesIndex = 0;
	while (!complete) {

		// Checks if the number at the index ('1,1' || '2,2' || '3,3' || ...) is '1'
		// If so, then it skips to avoid doing extra unnecessary calculations (not that it matters lol)
		if (
			is_int(fractions[onesIndex][onesIndex])
			&& (to_int(fractions[onesIndex][onesIndex]) == 1)
		) goto eliminate_column;


		// Divides entire row by the number at the ones position
		Fraction divisor = fractions[onesIndex][onesIndex];
		for (int i = 0; i < size; i++) {
			Fraction f = fractions[onesIndex][i];
			f = div_fraction(f, divisor);

			fractions[onesIndex][i] = f;
		}

		// Also modifies the identity matrix lol
		for (int i = 0; i < size; i++) {
			Fraction f = identity[onesIndex][i];
			f = div_fraction(f, divisor);

			identity[onesIndex][i] = f;
		}

		modify_matrix_row(matrix, fractions[onesIndex], onesIndex);
		modify_matrix_row(endMatrix, identity[onesIndex], onesIndex);

		printf("1 at (%d, %d) gotten:\n", onesIndex, onesIndex);
		print_matrix(matrix);

	eliminate_column: // This assumes that the number at index 'onesIndex' is a '1'


		onesIndex++;
	}
}

void modify_matrix_row(Matrix *self, Fraction *fractions, int row) {
	for (size_t i = 0; i < self->size; i++) {
		self->fractions[row][i] = fractions[i];
	}
}
