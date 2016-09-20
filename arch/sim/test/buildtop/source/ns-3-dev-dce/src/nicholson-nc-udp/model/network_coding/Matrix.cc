/**
 * Copyright (c) 2015
 *	Muhammad Muhammad
 *	All rights reserved.
 *
 * @file Matrix.cc
 *
 * @author 	Muhammad Muhammad
 * 		<mxmuhammad@gmail.com>
 * 		www.mxmuhammad.com
 */


#include "Matrix.h"


/**
 * @brief Print the system (Ax=b) in matrix form i.e. (A|b)
 */
void
Matrix::printSystem()
{
	/* to be printed in the form: [ A | b] */
	std::cout << "* SYSTEM: " << getRows() << " x " << getCols() << " *"
	    << std::endl;
	for (u_int i = 0; i < getRows(); i++) {
		for (u_int j = 0; j < getCols(); j++) {
			std::cout << std::setprecision (3) << "\t" <<
			    (u_int)mtrx[i][j] << "\t";
			/* to set a marker for the constants value */
			if (cols - j == 1)
				std::cout << "|\t" << (u_int)constants[i];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


/**
 * @brief Print the matrix (A)
 */
void
Matrix::printMatrix()
{
	/* to be printed in the form: [ A ] */
	std::cout << "* MATRIX *" << std::endl;
	for (u_int i = 0; i < getRows(); i++) {
		for (u_int j = 0; j < getCols(); j++) {
			std::cout << std::setprecision (3) << "\t" <<
			    (u_int)mtrx[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


/**
 * @brief Print the results <x> vector from (Ax=b)
 */
void
Matrix::printResult()
{
	if (DBG) {
		std::cout << "* RESULT *" << std::endl;
	}
	for (u_int i = 0; i < results.size(); i++){
		std::cout << "x[" << i << "] = "
		    << (u_int)results[i] << std::endl;
	}
	std::cout << std::endl;
}


/**
 * @brief Prints a single/any vector from matrix or outside
 *
 * @param v		vector to be printed (mtrx[i])
 */
void
Matrix::printVec(std::vector<uint8_t> &v)
{
	if (DBG) {
		std::cout << "* VECTOR *" << std::endl;
	}
	for (u_int i = 0; i < v.size(); i++)
		std::cout << "v[" << i << "] = " << (u_int)v[i] << std::endl;

	std::cout << std::endl;
}


/**
 * @brief Generate a random Matrix (System) with requested size
 *
 * @param r		number of rows
 * @param c		number of cols
 */
void
Matrix::generateRandomSystem(u_int r, u_int c)
{
	/* VARIABLES */
	std::vector<uint8_t> vec;	/* vector to hold data */
	std::vector<uint8_t> res;	/* vector to hold result for rand calc*/
	u_int gField;			/* GF field size */

	/* init field */
	gField = getGFSize();

	/* generate results vector - one for all of the system: consistancy */
	for (u_int j = 0; j < c; j++) {
		res.push_back((uint8_t)(rand() % gField));
	}

	/* generate matrix */
	for (u_int i = 0; i < r; i++) {
		for (u_int j = 0; j < c; j++) {
			vec.push_back((uint8_t)(rand() % gField));
		}
		/* add constants element */
		addConstant(calcConstant(vec, res));
		/* XXX only append row if it will reveal innovation */
		appendRow(vec);		/* -> addRow(vec) */
		vec.clear();
	}
	res.clear();
}


/**
 * @brief Transfor the matrix into an upper triangular form with use
 * of forward elimination techniques. Further, the  digonal elements will
 * have all values of one
 *
 * @return true when the system has a solution, false otherwise
 */
bool
Matrix::forwardElimination()
{
	/* VARIABLES */
	uint8_t pivot;		/* to hold the pivot element */
	int row_max;		/* row index of largest element in col (c) */
	bool dFlag = false;	/* dependency flag - linearly dependent vectors */
	bool fullRank = false;	/* full rank system check */

	/* system solution information */
	if (getRows() < getCols()) {
		if (DBG) {
			std::cout << "System of linear equations is not ready yet. "
			    << "Unsolvable system! - Infinite solutions\n"
			    << ">>> More unknowns than equations <<<\n" << std::endl;
		}
		/* to make sure that we have enough equations to solve the system */
		return (false);
	} else if (getRows() > getCols()) {
		if (DBG) {
			std::cout << "System of linear equations should have one solution.\n"
			    << "If system is consistant\n"
			    << ">>> More equations than unknowns <<<\n" << std::endl;
		}
	} else {
		if (DBG) {
			std::cout << "There exists exactly one solution to this system.\n"
			    << std::endl;
		}
	}

	/* go  through all lower matrix elements */
	for (u_int c = 0; c < getCols(); c++) {
		pivot = mtrx[c][c];
		/* avoiding unnecessary searching and swapping */
		if (pivot == 0) {
			row_max = getColLargest(c, c);
			/* to avoid unnecessary swapping */
			if (row_max != -1) {
				swapRows(c, row_max);
			}
		}

		/* new pivot */
		/* probably change because of swapping */
		pivot = mtrx[c][c];
		/* linearly dependent vectors */
		/* signalling for backwardSubstitution() method */
		if (pivot == 0) {
			if (DBG) {
				std::cout << "Linearly dependent vectors!\n"
				    << std::endl;
			}
			dFlag = true;
		}

		/* to avoid unnecessary division */
		if (pivot != 1 && pivot != 0) {
			divideRow(c, c);
		}

		/* operate on underlying rows of pivot row */
		for (u_int ro = c+1; ro < getRows(); ro++) {
			if (pivot != 0) {
				operateRow(c, ro, c);
			}
		}
	}

	/* full rank system check! */
	for (u_int i = 0; i < getCols(); i++) {
		if (mtrx[i][i] != 1) {
			fullRank = false;
		} else {
			fullRank = true;
		}
	}

	if (dFlag) {
		/* solution does not exist */
		return (false);
	} else {
		/* system solved */
		return (fullRank);
	}
}


/**
 * @brief Solve the system of linear equations, after being transformed into
 * upper triangular form
 *
 * @return true if solution exists, false otherwise
 */
bool
Matrix::backwardSubstitution()
{
	if (!forwardElimination()) {
		if (DBG) {
			std::cout << "Solution does not exist...\n" << std::endl;
		}
		return (false);
	} else {
		/* to hold the row sum value */
		uint8_t row_sum = 0;
		for (int r = getCols()-1; r >= 0; r--) {
			for (int col = getCols()-1; col > r; col--) {
				row_sum = GF->gfAdd(row_sum,
				    GF->gfMul(mtrx[r][col], results[col]));
			}
			results[r] = GF->gfSub(constants[r], row_sum);
			row_sum = 0;
		}
		return (true);
	}
}


/**
 * @brief Multiply two matrices together
 *
 * @param M		the second matrix
 *
 * @return		the result in a matrix form
 */
Matrix 
Matrix::multiply(Matrix& M)
{
	/* get size of the two matrices */
	/* first matrix */
	u_int r1 = getRows();
	u_int c1 = getCols();
	/* second matrix */
	u_int r2 = M.getRows();
	u_int c2 = M.getCols();
	
	/* check matrices dimensions */
	if (c1 != r2) {
		if (DBG) {
			std::cout << "Error: Dimensions of the Matrices should match!"
			    << std::endl;
		}
		exit(0);
	}

	/* Else, create a matrix to be returned with the final values */
	Matrix res;
	
	/* do the arithmetical operations here */
	uint8_t sum = 0;
	std::vector<uint8_t> vec;
	for (u_int i = 0; i < r1; i++) {
		for (u_int j = 0; j < c2; j++) {
			for (u_int k = 0; k < c1; k++) {
				sum = GF->gfAdd(sum, 
				    GF->gfMul(mtrx[i][k], M(k, j)));
				//std::cout << "i: " << i << ", j: " << j << ", k: " << k << std::endl;
			}
			vec.push_back(sum);
			sum = 0;
		}
		res.appendRow(vec);
		//printVec(vec);
		vec.clear();
	}

	return (res);
}


/**
 * @brief Multiply two matrices together
 */
void
Matrix::transpose()
{
	Matrix temp;
	std::vector<uint8_t> tempVec;
	
	// Copy current matrix into a temp matrix
	for(u_int i = 0; i < getRows(); i++) {
		for(u_int j = 0; j < getCols(); j++) {
			tempVec.push_back( mtrx[i][j] );
		}
		temp.appendRow(tempVec);
		tempVec.clear();
	}
	
	// Do the transpose after clearing the current matrix
	clearMatrix();
	for(u_int i = 0; i < temp.getCols(); i++) {
		for(u_int j = 0; j < temp.getRows(); j++) {
			tempVec.push_back( temp(j, i) );
		}
		appendRow(tempVec);
		tempVec.clear();
	}
	// Clear temp
	temp.clearMatrix();
}













