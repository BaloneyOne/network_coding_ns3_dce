/**
 * Copyright (c) 2015
 *	Muhammad Muhammad
 *	All rights reserved.
 *
 * @file Galois_Field.h
 *
 * @author 	Muhammad Muhammad
 * 		<mxmuhammad@gmail.com>
 * 		www.mxmuhammad.com
 */
 

/**
 * @note This file will include the functions that will help us in doing
 * arithmatical operations within the finit field -> Galois Field.
 * These operations will be used in the Matrix class for Encoding and decoding,
 * respectively. 
 */

#ifndef GaloisField_H
#define GaloisField_H


#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdint.h>


class GaloisField {
  private:
	std::vector< std::vector<uint8_t> > mTable;	/* multiplication table */
	std::vector< std::vector<uint8_t> > dTable;	/* division table */
	u_int q;					/* field size */

  public:
	/// Constructor with default field size
	GaloisField(u_int = 256);

	/// Destructor
	~GaloisField();

	/// Initialize GF
	bool initialize(u_int);

	/// Get field size: q
	u_int getField();

	/// Add two numbers
	uint8_t gfAdd(uint8_t, uint8_t);

	/// Subrtact two numbers
	uint8_t gfSub(uint8_t, uint8_t);

	/// Multiply two numbers
	uint8_t gfMul(uint8_t, uint8_t);

	/// Divide two numbers
	uint8_t gfDiv(uint8_t, uint8_t);

	/// Get power of a number
	uint8_t gfPow(uint8_t, u_int);

	/// Get size of tables
	int tSize();
	
	/// Print table: true = mTable, false = dTable 
	void printTable(bool);

  private:
	/// Set field size: q
	void setField(u_int);

	/// Read table
	bool readTables();
};


/**
 * @brief Constructor
 *
 * @param q		GF field size
 */
inline
GaloisField::GaloisField(u_int q)
{
	if (!initialize(q)) {
		std::cerr << "Error: Could not initialize GF!" << std::endl;
	}
}


/**
 * @brief Destructor
 */
inline
GaloisField::~GaloisField()
{
	/* XXX */
}


/**
 * @brief Initialize method, to set the field size
 *
 * @param q		GF size: q=2^x
 *
 * @return true if everything went well, false otherwise
 */
inline bool
GaloisField::initialize(u_int q)
{
	setField(q);
	if (readTables()) {
		return true;
	} else {
		std::cout << "Error: Tables NOT read!!!" << std::endl;
		return false;
	}
}


/**
 * @brief Get field size
 *
 * @return field size (q)
 */
inline u_int
GaloisField::getField()
{
	return (q);
}


/**
 * @brief Set field size
 *
 * @param s		field size
 */
inline void
GaloisField::setField(u_int s)
{
	q = s;
}


/**
 * @brief Get size of table.
 * It should be the same for both tables;
 * since we are using a single finite field.
 *
 * @return size of table
 */
inline int
GaloisField::tSize()
{
	return (mTable.size());
}



/**
 * @brief Add two numbers in the GF finite field
 *
 * @param num1			first number
 * @param num1			second number
 *
 * @return result of addition (simply the XOR (^) operation)
 */
inline uint8_t
GaloisField::gfAdd(uint8_t num1, uint8_t num2)
{
	if (num1 < tSize() && num2 < tSize()) {
		return (num1 ^ num2);
	} else {
		printf("Error: element in GF (Addition) doesn't exits!\n");
		exit(-1);
	}
}


/**
 * @brief Subtract two numbers in the GF finite field
 *
 * @param num1			first number
 * @param num1			second number
 *
 * @return result of subtraction (also the XOR (^) operation)
 */
inline uint8_t
GaloisField::gfSub(uint8_t num1, uint8_t num2)
{
	if (num1 < tSize() && num2 < tSize()) {
		return (num1 ^ num2);
	} else {
		printf("Error: element in GF (Subtraction) doesn't exits!\n");
		exit(-1);
	}
}


/**
 * @brief Multiply two numbers in the GF finite field
 *
 * @param num1			first number
 * @param num1			second number
 *
 * @return result of Multiplication (from mTable)
 */
inline uint8_t
GaloisField::gfMul(uint8_t num1, uint8_t num2)
{
	if (num1 < tSize() && num2 < tSize()) {
		return (mTable[num1][num2]);
	} else {
		printf("Error: element in GF (Multiply) doesn't exits!\n");
		exit(-1);
	}
}


/**
 * @brief Divide two numbers in the GF finite field
 *
 * @param num1			first number
 * @param num1			second number
 *
 * @return result of Division (from dTable)
 */
inline uint8_t
GaloisField::gfDiv(uint8_t num1, uint8_t num2)
{
	if (num1 < tSize() && num2 < tSize() && num2 != 0) {
		return (dTable[num1][num2-1]);
	} else {
		printf("num1 = %d, num2 = %d, Size = %d\n", num1, num2, tSize());
		printf("Error: element in GF (Divide) doesn't exits!\n");
		exit(-1);
	}
}


/**
 * Get the power of a number in the GF domain (n^p).
 * That is, 'n' is multiplied by itself 'p' times
 *
 * @param n		number to calculate
 * @param p		power for the number
 *
 * @return result of calculation
 */
inline uint8_t
GaloisField::gfPow(uint8_t n, u_int p)
{
	/* to hold final result */
	uint8_t result = 1;

	for (u_int i = 0; i < p; i++) {
		result = gfMul(result, n);
	}

	return (result);
}


#endif
