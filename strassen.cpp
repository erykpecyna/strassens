#include <stdio.h>
#include <fstream>

const int MAXLINE = 256;

// Note: this could have been done in half the lines of code but
// by the time I realized I should be using inheritance, it was
// too late for my lazy self to go back and change it. 

struct Matrix {
	int* mat;
	int actuald;
	int d;

	// Allocates a single block of contiguous memory for each matrix
	// this might speed up accesses who knows.
	Matrix(int d) {
		actuald = d;
		if(actuald % 2 != 0)
			this->d = actuald + 1;
		else this->d = actuald;

		mat = new int[actuald*actuald];
	};

	~Matrix() {
		delete mat;
	};

	int get(int row, int col) {
		int index = row * d + col;
		if(row == actuald || col == actuald)
			return 0;
		if(actuald != d)
			index -= row;
		if(row < actuald || col < actuald)
			return mat[index];
		else return 0;
	}

	void set(int row, int col, int val) {
		int index = row * d + col;
		if(row == actuald || col == actuald)
			return;
		if(actuald != d)
			index -= row;
		if(row < actuald || col < actuald)
			mat[index] = val;
	}

	// For testing
	void printMatrix() {
		for(int row = 0; row < d; row++) {
			for(int col = 0; col < d; col++) 
				printf("%i ", get(row,col));
			printf("\n");
		}
	};

};

struct subMat {
	Matrix* orig;
	int rowstart, rowend;
	int colstart, colend;
	// actuald represents what dimension of this matrix
	// has non-zero values in orig
	int d;

	subMat() {

	};
	
	subMat(Matrix* parent, int d, int rs, int cs, int re, int ce) {
		rowstart = rs;
		colstart = cs;
		rowend = re;
		colend = ce;

		if(d % 2 != 0)
			this->d = d + 1;
		else this->d = d;

		orig = parent;
	}

	int get(int row, int col) {
		int rind = row + rowstart;
		int cind = col + colstart;

		if(rind < rowend && cind < colend)
			return orig->get(rind, cind);
		else return 0;
	};

	void printMatrix() {
		for(int row = 0; row < d; row++) {
			for(int col = 0; col < d; col++) 
				printf("%i ", get(row,col));
			printf("\n");
		}
	}
};

subMat* splitMatrix(subMat* parent) {
	int splitind = parent->d / 2;

	subMat m1(parent->orig,
		splitind,
		parent->rowstart,
		parent->colstart,
		parent->rowstart + splitind,
		parent->colstart + splitind);
	subMat m2(parent->orig,
		splitind,
		parent->rowstart,
		parent->colstart + splitind,
		parent->rowstart + splitind,
		parent->colend);
	subMat m3(parent->orig,
		splitind,
		parent->rowstart + splitind,
		parent->colstart,
		parent->rowend,
		parent->colstart + splitind);
	subMat m4(parent->orig,
		splitind,
		parent->rowstart + splitind,
		parent->colstart + splitind,
		parent->rowend,
		parent->colend);
	
	subMat* arr = new subMat[4];
	arr[0] = m1;
	arr[1] = m2;
	arr[2] = m3;
	arr[3] = m4;

	return arr;
}

subMat* splitMatrix(Matrix* parent) {
	int splitind = parent->d / 2;
	int actuald = parent->actuald;

	subMat m1(parent,
		splitind,
		0, 0,
		splitind, splitind);
	subMat m2(parent,
		splitind,
		0, splitind,
		splitind, actuald);
	subMat m3(parent,
		splitind,
		splitind, 0,
		actuald, splitind);
	subMat m4(parent,
		splitind,
		splitind, splitind,
		actuald, actuald);
	
	subMat* arr = new subMat[4];
	arr[0] = m1;
	arr[1] = m2;
	arr[2] = m3;
	arr[3] = m4;

	return arr;
}

Matrix addMatrices(subMat* m1, subMat* m2, bool neg=false) {
	int sub = neg ? -1 : 1;
	Matrix res(m1->d);
	for(int row = 0; row < m1->d; row++) {
		for(int col = 0; col < m1->d; col++)
			res.set(row,col,m1->get(row,col) + sub * m2->get(row, col));
	}

	return res;
}

Matrix addMatrices(Matrix* m1, Matrix* m2, bool neg=false) {
	int sub = neg ? -1 : 1;
	Matrix res(m1->d);
	for(int i = 0; i < m1->d * m1->d; i++)
		res.mat[i] = m1->mat[i] + sub * m2->mat[i];
	return res;
}

int main(int argc, char* argv[]) {
	int d = std::stoi(argv[2]);
	char* inputfile = argv[3];

	Matrix m1(d), m2(d);

	// Parse input into matrices
	std::ifstream infile(inputfile, std::ios::in);
	char buff[MAXLINE];
	for (int i = 0; i < d*d; i++) {
		if (infile.getline(buff, MAXLINE) )
		m1.mat[i] = std::stoi(buff);
	}
	for (int i = 0; i < d*d; i++) {
		if (infile.getline(buff, MAXLINE) )
		m2.mat[i] = std::stoi(buff);
	}

	m1.printMatrix();
	// m2.printMatrix();

	
	subMat* arr = splitMatrix(&m1);

	for(int i = 0; i < 4; i++) {
		printf("\n"); 
		arr[i].printMatrix();
		printf("\n"); 
	}



	// subMat* sarr = splitMatrix(&arr[0]);

	// for(int i = 0; i < 4; i++) {
	// 	printf("\n"); 
	// 	sarr[i].printMatrix();
	// 	printf("\n"); 
	// }

	Matrix testadd = addMatrices(&arr[0], &arr[1]);
	testadd.printMatrix();
}