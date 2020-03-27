#include <stdio.h>
#include <fstream>
#include <type_traits>


#define MAXLINE 256
#ifndef CUTOFF
#define CUTOFF 600
#endif


// NOTE: All matrix arithmetic assumes equal dimensions
// (No error handling)
struct Matrix {
	int* mat;
	int actuald;
	int d;

	// Allocates a single block of contiguous memory for each matrix
	// this might speed up accesses who knows.
	Matrix(int d) {
		actuald = d;
		mat = new int[actuald * actuald];
		if(actuald % 2 != 0)
			this->d = actuald + 1;
		else this->d = actuald;

		
	};

	Matrix(Matrix* q1, Matrix* q2, Matrix* q3, Matrix* q4, int parentd) {
		this->d = parentd;
		actuald = parentd;
		int splitind = (parentd % 2 == 0) ? parentd / 2 : parentd / 2 + 1;

		mat = new int[parentd * parentd];

		// Quadrant 1
		for(int row = 0; row < splitind; row++) {
			for(int col = 0; col < splitind; col++) {
				set(row, col, q1->get(row,col));
			}
		}

		// Quadrant 2
		for(int row = 0; row < splitind; row++) {
			for(int col = splitind; col < parentd; col++) {
				set(row, col, q2->get(row,col - splitind));
			}
		}

		// Quadrant 3
		for(int row = splitind; row < parentd; row++) {
			for(int col = 0; col < splitind; col++) {
				set(row, col, q3->get(row - splitind,col));
			}
		}

		// Quadrant 4
		for(int row = splitind; row < parentd; row++) {
			for(int col = splitind; col < parentd; col++) {
				set(row, col, q4->get(row - splitind,col- splitind));
			}
		}
	}

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

	void printMatrix() {
		for(int row = 0; row < actuald; row++) {
			printf("%i ", get(row,row));
			printf("\n");
		}
	};

	// For testing
	// void printMatrix() {
	// 	for(int row = 0; row < d; row++) {
	// 		for(int col = 0; col < d; col++) 
	// 			printf("%i ", get(row,col));
	// 		printf("\n");
	// 	}
	// };

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

Matrix* addMatrices(subMat* m1, subMat* m2, bool neg=true) {
	int sub = neg ? 1 : -1;
	Matrix* res = new Matrix(m1->d);
	for(int row = 0; row < m1->d; row++) {
		for(int col = 0; col < m1->d; col++)
			res->set(row,col,m1->get(row,col) + sub * m2->get(row, col));
	}
	return res;
}

Matrix* addMatrices(Matrix* m1, Matrix* m2, bool neg=true) {
	int sub = neg ? 1 : -1;
	Matrix* res = new Matrix(m1->d);
	for(int i = 0; i < m1->d * m1->d; i++)
		res->mat[i] = m1->mat[i] + sub * m2->mat[i];
	return res;
}

template <class M, class T>
Matrix* multiplyMatricesStandard(M* m1, T* m2, int parentd) {
	Matrix* res = new Matrix(parentd);

	for (int row = 0; row < m1->d; row++) {
		for (int col = 0; col < m1->d; col++) {
			res->set(row,col,0);
			for (int k = 0; k < m1->d; k++) {
				res->set(row,col,
					res->get(row,col) + m1->get(row,k) * m2->get(k, col));
			}
		}
	}

	return res;
}

template <class M, class T>
Matrix* multiplyMatricesStrassens(M* m1, T* m2, int parentd) {
	if(parentd > CUTOFF) {	
		subMat* s1 = splitMatrix(m1);
		subMat* s2 = splitMatrix(m2);
		subMat* A = &s1[0];
		subMat* B = &s1[1];
		subMat* C = &s1[2];
		subMat* D = &s1[3];
		subMat* E = &s2[0];
		subMat* F = &s2[1];
		subMat* G = &s2[2];
		subMat* H = &s2[3];

		Matrix* FmH = addMatrices(F, H, false);
		Matrix* AB = addMatrices(A, B);
		Matrix* CD = addMatrices(C, D);
		Matrix* GmE = addMatrices(G, E, false);
		Matrix* AD = addMatrices(A, D);
		Matrix* EH = addMatrices(E, H);
		Matrix* BmD = addMatrices(B, D, false);
		Matrix* GH = addMatrices(G, H);
		Matrix* AmC = addMatrices(A, C, false);
		Matrix* EF = addMatrices(E, F);

		Matrix* p1 = multiplyMatricesStrassens(A, FmH, A->d);
		Matrix* p2 = multiplyMatricesStrassens(AB, H, H->d);
		Matrix* p3 = multiplyMatricesStrassens(CD, E, E->d);
		Matrix* p4 = multiplyMatricesStrassens(D, GmE, D->d);
		Matrix* p5 = multiplyMatricesStrassens(AD, EH, AD->d);
		Matrix* p6 = multiplyMatricesStrassens(BmD, GH, GH->d);
		Matrix* p7 = multiplyMatricesStrassens(AmC, EF, EF->d);

		Matrix* p5p4 = addMatrices(p5, p4);
		Matrix* p2mp6 = addMatrices(p2,p6,false);
		Matrix* p5p1 = addMatrices(p5, p1);
		Matrix* p3p7 = addMatrices(p3, p7);

		Matrix* q1 = addMatrices(p5p4, p2mp6, false);
		Matrix* q2 = addMatrices(p1, p2);
		Matrix* q3 = addMatrices(p3, p4);
		Matrix* q4 = addMatrices(p5p1, p3p7, false);

		Matrix* res = new Matrix(q1, q2, q3, q4, parentd);


		delete s1;
		delete s2;
		delete FmH;
		delete AB;
		delete CD;
		delete GmE;
		delete AD;
		delete EH;
		delete BmD;
		delete GH;
		delete AmC;
		delete EF;
		delete p5p4;
		delete p2mp6;
		delete p5p1;
		delete p3p7;
		delete p1;
		delete p2;
		delete p3;
		delete p4;
		delete p5;
		delete p6;
		delete p7;
		delete q1;
		delete q2;
		delete q3;
		delete q4;
		return res;
	} else return multiplyMatricesStandard(m1, m2, parentd);
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
	infile.close();

	Matrix* mults = multiplyMatricesStrassens(&m1, &m2, d);

	mults->printMatrix();

	delete mults;
}