/**
 * Copyright (c) 2015
 *	Muhammad Muhammad
 *	All rights reserved.
 *
 * @file Matrix.h
 *
 * @author 	Muhammad Muhammad
 * 		<mxmuhammad@gmail.com>
 * 		www.mxmuhammad.com
 */


/**
 * @note This file will include the functions for both encoding and adecoding.
 * It will include all operations that can be done to solve a system of linear
 * equations in a matrix form such as forward elimination operation, etc...
 * This version already include the GF(256 & 16) operations such as ADD, SUB, 
 * MUL & DIV.
 */

#ifndef Matrix_H
#define Matrix_H


#include <iostream>
#include <sys/types.h>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Galois_Field.h"


#define DBG			1			// Debugging flag


class Matrix {
  private:						/* System: Ax=b -> to be solved */
	std::vector< std::vector<uint8_t> > mtrx;	/* main matrix "A" */
	std::vector<uint8_t> results;			/* vector "x" to hold results */
	std::vector<uint8_t> constants;			/* vector "b" with constants */
	u_int rows;					/* number of rows in the matrix */
	u_int cols;					/* number of columns in the matrix */
	GaloisField *GF;				/* GF arithmatics class */

  public:
	/// Constructor
	Matrix();

	/// Constructor with single size yields a square matrix
	Matrix(u_int);

	/// Constructor with both number of rows and cols
	Matrix(u_int, u_int);
	
	/// Destructor
	~Matrix();

	/// Initialization of sizes
	void initialize(u_int, u_int);

	/// Set GF pointer to an object created in main
	void setGFPtr(GaloisField&);

	/// Get GF size
	u_int getGFSize();

	/// Overload () operator instead of [][] to access from other code
	uint8_t &operator()(u_int, u_int);

	/// Print matrix in the form: [ A | b ]
	void printSystem();

	/// Print matrix in the form: [ A ]
	void printMatrix();

	/// Print result vector as: (x[i] = value) where i=1...rows
	void printResult();

	/// Print a single/any vector
	void printVec(std::vector<uint8_t>&);
	
	/// Generate a random Matrix with given size
	void generateRandomSystem(u_int, u_int);

	/// Get number of rows
	u_int getRows();

	/// Get number of cols
	u_int getCols();

	/// Set number of rows
	void setRows(u_int);

	/// Set number of cols
	void setCols(u_int);

	/// Set matrix size (rows & cols)
	void setSize(u_int, u_int);

	/// Set the constants vector (i.e. assign it to a given vector)
	void setConstants(std::vector<uint8_t>&);

	/// Set Matrix dimensions; sizes of: rows, cols, constants & results
	void setDimensions(u_int, u_int);

	/// Clear a matrix - after creating it to re-fill again
	void clearMatrix();

	/// Get results vector
	std::vector<uint8_t>& getResults();

	/// Get constants vector
	std::vector<uint8_t>& getConstants();

	/// Gaussian eliminations - includes both forwardElimination() & backwardSubstitution()
	bool gaussianElimination(bool);

	/// Add a row to the matrix after checking for the row innovativity
	bool addRow(std::vector<uint8_t>&);

	/// Add constants element
	void addConstant(uint8_t);
	
	/// Multiple two matrices
	Matrix multiply(Matrix&);

	/// Transpose a matrix
	void transpose();

	/// Append a row (vector of size: cols) to the matrix
	void appendRow(std::vector<uint8_t>&);
	
	/// Calculate consistant constant element based on result & coefficients
	uint8_t calcConstant(std::vector<uint8_t>&, std::vector<uint8_t>&);

  private:
	/// Swap two rows given there indexes
	void swapRows(u_int, u_int);

	/// Operate on a row: make the linear operation to eliminate an entry under pivot
	void operateRow(u_int, u_int, u_int);

	/// Divide a row by a constant (pivot) to make pivot equal to one
	void divideRow(u_int, u_int);

	/// Get the index of the row of the largest element in a specific column under pivot
	int getColLargest(u_int, u_int);

	/// Transform the matrix into upper triangular form
	bool forwardElimination();

	/// Solve the system of linear equations and put results in the corresponding results vector
	bool backwardSubstitution();

	/// Seeing a packet. Check whether the new received packet is innovative
	bool seeingPacket(std::vector<uint8_t>&);
};


/**
 * @brief Constructor of a 0x0 Matrix
 */
inline
Matrix::Matrix()
{
	/* set matrix dim to zeros */
	initialize(0, 0);
}


/**
 * @brief Constructor of a square matrix
 *
 * @param s		size of matrix
 */
inline
Matrix::Matrix(u_int s)
{
	/* set matrix dim to s */
	initialize(s, s);
}


/**
 * @brief Constructor with definite matrix rows and cols size
 *
 * @param r		number of rows
 * @param c		number of cols
 */
inline
Matrix::Matrix(u_int r, u_int c)
{
	/* set matrix dim to r & c */
	initialize(r, c);
}


/**
 * @brief Destructor
 */
inline
Matrix::~Matrix()
{
	clearMatrix();
}


/**
 * @brief Initialization for rows & cols sizes
 *
 * @param r		number of rows
 * @param c		number of cols
 */
inline void
Matrix::initialize(u_int r, u_int c)
{
	/* set matrix dim to r & c */
	setDimensions(r, c);
}


/**
 * @brief Set the GF pointer to an object created in main. Helps in reducing 
 * memory usage by allowing mutiple matrices to use the same GF tables
 *
 * @param p		pointer to a GF object
 */
inline void
Matrix::setGFPtr(GaloisField &p)
{
	GF = &p;
}


/**
 * @brief Get GF size
 *
 * @return the GF size
 */
inline u_int
Matrix::getGFSize()
{
	return(GF->getField());
}


/**
 * @brief Get number of rows
 *
 * @return 	number of rows for the specified matrix
 */
inline u_int
Matrix::getRows()
{
	return (rows);
}


/**
 * @brief Get number of cols
 *
 * @return 	number of columns for the specified matrix
 */
inline u_int
Matrix::getCols()
{
	return (cols);
}


/**
 * @brief Set number of rows for the matrix
 *
 * @param r		requested number of rows
 */
inline void
Matrix::setRows(u_int r)
{
	rows = r;
}


/**
 * @brief Set number of columns for the matrix
 *
 * @param c		requested number of columns
 */
inline void
Matrix::setCols(u_int c)
{
	cols = c;
}


/**
 * @brief Set size of the matrix i.e. rows & cols
 *
 * @param r		requested number of rows
 * @param c		requested number of columns
 */
inline void
Matrix::setSize(u_int r, u_int c)
{
	setRows(r);
	setCols(c);
}


/**
 * Set Matrix dimensions; sizes of: rows, cols, constants & results
 *
 * @param r		size of rows
 * @param c		size of columns
 */
inline void
Matrix::setDimensions(u_int r, u_int c)
{
	setSize(r, c);
	constants.resize(r, 0);
	results.resize(c, 0);
}


/**
 * @brief Get the constants vector
 *
 * @return constants
 */
inline std::vector<uint8_t>&
Matrix::getConstants()
{
	return (constants);
}


/**
 * @brief Get the results vectors
 *
 * @return results
 */
inline std::vector<uint8_t>&
Matrix::getResults()
{
	return (results);
}


/**
 * @brief Add a constant element
 *
 * @param e		element to be added
 */
inline void
Matrix::addConstant(uint8_t e)
{
	constants.push_back(e);
	constants.resize(constants.size());
}


/**
 * @brief Calculate consistant constant element based on result & coefficients
 * that are generated randomly in order to avoid inconsistant system when we
 * have more equations than unknowns
 *
 * @param v		coefficients vector
 * @param r		results vector (not used afterwards)
 *
 * @return coef		the right coefficient
 */
inline uint8_t
Matrix::calcConstant(std::vector<uint8_t> &v, std::vector<uint8_t> &r)
{
	uint8_t coef = 0;
	for (u_int i = 0; i < v.size(); i++) {
		coef = GF->gfAdd(coef, GF->gfMul(v[i], r[i]));
	}
	return (coef);
}


/**
 * @brief Set the constants vector
 *
 * @param c		vector of constants
 */
inline void
Matrix::setConstants(std::vector<uint8_t> &c)
{
	constants = c;
	constants.resize(c.size());
}


/**
 * @brief Append a row to the matrix
 *
 * @param row		row to be added
 */
inline void
Matrix::appendRow(std::vector<uint8_t> &r)
{
	mtrx.push_back(r);
	setRows(rows+1);
	setCols(r.size());
	results.resize(r.size());
}


/**
 * @brief Add a row to the matrix after being checked for innovativity
 *
 * @param row		row to be added
 *
 * @return true if operation succeeded, else false
 */
inline bool
Matrix::addRow(std::vector<uint8_t> &r)
{
	if (seeingPacket(r)) {
		appendRow(r);
		return (true);
	} else {
		return (false);
	}
}


/**
 * @brief Clear the matrix. Used to build it again from scratch
 */
inline void
Matrix::clearMatrix()
{
	for (int i = getRows()-1; i >= 0; i--) {
		for (u_int j = 0; j < cols; j++) {
			//mtrx[i].erase(mtrx[i].begin(), mtrx[i].end());
			//mtrx[i].clear();
			/* de-reference all elements in the vector */
			mtrx[i].pop_back();
		}
		/* resize vector and take it out of matrix */
		mtrx[i].resize(0);
		mtrx.pop_back();
		/* also constants and results vectors */
		constants.pop_back();
		results.pop_back();
	}
	/* resize matrix, rows & cols */
	mtrx.resize(0);
	setSize(0, 0);
	constants.resize(0);
	results.resize(0);
}


/**
 * @brief Swaps the contents of two rows given their corresponding indexes
 * plus the related constants values
 *
 * @param r1		index of row one
 * @param r2		index of row two
 */
inline void
Matrix::swapRows(u_int r1, u_int r2)
{
	/* swap rows in matrix */
	mtrx[r1].swap(mtrx[r2]);

	/* hold a value temporarly */
	uint8_t tmp;

	/* swap corresponding elements in constants vector */
	tmp = constants[r1];
	constants[r1] = constants[r2];
	constants[r2] = tmp;
}


/**
 * @brief Divide a row by the pivot to make the pivot equal to one, and
 * and all the elements also divided by pivot. Further, it is supposed that
 * elements to the right of the pivot are zeros
 *
 * @param r		index of row to be divided
 * @param c		index of column where the pivot is
 */
inline void
Matrix::divideRow(u_int r, u_int c)
{
	/* pivot element to divide the entire row with */
	uint8_t pivot = mtrx[r][c];

	for (u_int i = c; i < getCols(); i++) {
		mtrx[r][i] = GF->gfDiv(mtrx[r][i], pivot);
	}

	/* don't forget constant element as well */
	constants[r] = GF->gfDiv(constants[r], pivot);
}


/**
 * @brief Does arithmatical operations on a row with the pivot row to eliminate
 * and element under the pivot element in the linear system
 *
 * @param r1		index of pivot row
 * @param r2		index of row to eliminate element from (operated row)
 * @param c		column number to start operating from
 */
inline void
Matrix::operateRow(u_int r1, u_int r2, u_int c)
{
	/* get element to multiply row-2 with */
	uint8_t element = mtrx[r2][c];

	/* operate on the entire row */
	for (u_int i = c; i < getCols(); i++) {
		mtrx[r2][i] = GF->gfSub(mtrx[r2][i], 
		    GF->gfMul(element, mtrx[r1][i]));
	}

	/* also the constant element */
	constants[r2] = GF->gfSub(constants[r2],
	    GF->gfMul(element, constants[r1]));
}


/**
 * @brief Get row index of the largest element (in absolute value)
 * in the requested column, to be swapped with the current one. It will
 * be the new pivot instead of the current row
 *
 * @param r		row index of current row
 * @param c		column index
 *
 * @return row of the largest elemet in the row else (-1) indicating that
 * the current pivot is the largest
 */
inline int
Matrix::getColLargest(u_int r, u_int c)
{
	/* to hold the largest number in the row */
	uint8_t max = fabs(mtrx[r][c]);
	u_int row_max = r;

	/* search for max. in absolute value */
	for (u_int i = r+1; i < getRows(); i++) {
		if (fabs(mtrx[i][c]) > max) {
			max = mtrx[i][c];
			row_max = i;
		}
	}

	/* return row index of max element */
	if (row_max == r) {
		return (-1);
	} else {
		return (row_max);
	}
}


/**
 * @brief Gaussian elimination process. Simply, composed of both:
 * forwardElimination() & backwardSubstitution() methods.
 *
 * @param f		true if result to be printed, false otherwise
 *
 * @return true on success, false on failure
 */
inline bool
Matrix::gaussianElimination(bool f)
{
	if (backwardSubstitution()) {
		if (f) {
			printResult();
		}
		return (true);
	} else {
		if (DBG) {
			std::cout << "No results...\n" << std::endl;
		}
		return (false);
	}
}


/**
 * @brief Overloading operator () to be used from outside instead of [][]
 *
 * @param r		row index of requested element
 * @param c		column index of requested element
 *
 * @return requested element
 */
inline uint8_t &
Matrix::operator()(u_int r, u_int c)
{
	if (r < getRows() && c < getCols()) {
		return (mtrx[r][c]);
	} else {
		if (DBG) {
			std::cout << "Error: Boundries exceeded!" << std::endl;
		}
		throw(911);
	}
}


/**
 * @brief Seeing a packet. Check whether the new received packet is innovative.
 * This function helps us to build our matrix in a way ready to be decoded.
 * That is, free of dependent rows. The return value tells us whether to add
 * this new vector to our matrix or not. Further, we can send an ACK to the
 * sender telling that the packet was received; however, not decoded!
 *
 * @param vec		the new received vector
 *
 * @return true if the vector reveals innovactive packet. False otherwise.
 */
inline bool
Matrix::seeingPacket(std::vector<uint8_t>& vec)
{
	/* VARIABLES */
	Matrix cM = (*this);		/* get a copy of the matrix */
	bool resultFlag = false;	/* final returned results */
	uint8_t pivot;			/* pivot element */

	/* append new vector as a new row to the matrix */
	cM.appendRow(vec);
	cM.setSize(getRows()+1, vec.size());

	if (cM.getRows() <= cM.getCols()) {
		for (u_int c = 0; c < cM.getRows(); c++) {
			pivot = cM(c, c);

			if (pivot != 1 && pivot != 0) {
				cM.divideRow(c, c);
			}

			/* operate on underlying rows of pivot row */
			for (u_int ro = c+1; ro < cM.getRows(); ro++) {
				if (pivot != 0) {
					cM.operateRow(c, ro, c);
				}
			}
		}
		/* check if the new packet reveals innovative info */
		for (u_int i = 0; i < cM.getCols(); i++) {
			if (cM(cM.getRows()-1, i) != 0) {
				resultFlag = true;
			}
		}
	}


	return (resultFlag);
}


#endif
