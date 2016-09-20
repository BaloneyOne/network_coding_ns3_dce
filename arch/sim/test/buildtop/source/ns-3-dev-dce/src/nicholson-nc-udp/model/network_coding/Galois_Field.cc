/**
 * Copyright (c) 2015
 *	Muhammad Muhammad
 *	All rights reserved.
 *
 * @file Galois_Field.cc
 *
 * @author 	Muhammad Muhammad
 * 		<mxmuhammad@gmail.com>
 * 		www.mxmuhammad.com
 */


#include "Galois_Field.h"


/**
 * @brief Read tables from the files in the "files" directory.
 * These will be the results for only multiplication and division operations
 *
 * @return true if read was successful else false
 */
bool
GaloisField::readTables()
{
	/* VARIABLES */
	std::vector<std::string> files;		/* files names */
	std::vector<uint8_t> vec;		/* to hold elements */
	const int LINE_SIZE = 1024;		/* line size */
	char *fLine = NULL;			/* line from file */
	uint8_t element;			/* element read */
	const char *num = NULL;			/* for tokenizing */
	int count;				/* number of rows in table */
	char q[sizeof(u_int)];			/* GF field size */
	std::string fSize;			/* GF fiels size: string */
	std::string tPath;			/* GF tables path */

	/* change q - field size value - to string */
	int n = sprintf(q, "%d", getField());
	if (n > 0) {
		fSize = std::string(q);
	} else {
		std::cerr << "Error: Could not convert field size to string!"
		    << std::endl;
		return (false);
	}

	/* add files names */
	tPath = "./tables/";
	files.push_back(tPath + "gfMul_" + fSize + ".txt");
	files.push_back(tPath + "gfDiv_" + fSize + ".txt");

	/* init line size */
	fLine = new char[LINE_SIZE];

	/* read files, tokenize lines and push values in vectors */
	for (u_int f = 0; f < files.size(); f++) {
		count = 0;
		/* open file */
		std::ifstream inFile(files[f].c_str());
		if (!inFile) {
			std::cout << "Error: Could not read file!" << std::endl;
			return (false);
		}
		/* start reading file */
		while (!inFile.eof()) {
			inFile.getline(fLine, LINE_SIZE, '\n');
			num = strtok(fLine, " ");
			while (num != NULL) {
				element = atoi(num);
				vec.push_back((u_int8_t)element);
				num = strtok(NULL, " ");
			}
			if (f == 0) {
				mTable.push_back(vec);
			}
			if (f == 1) {
				dTable.push_back(vec);
			}
			vec.clear();
			count++;
		}
		mTable.resize(count);
		/* close file */
		inFile.close();
	}
	/* free memory */
	fLine = NULL;
	num = NULL;
	
	/* success */
	return (true);
}


/**
 * @brief Print Table for GF
 *
 * @param true		print mTable
 * @param false		print dTable
 */
void
GaloisField::printTable(bool t)
{
	if (t) {
		for (int i = 0; i < tSize(); i++) {
			for (int j = 0; j < tSize(); j++) {
				std::cout << (u_int)mTable[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	} else {
		for (int i = 0; i < tSize(); i++) {
			for (int j = 0; j < tSize(); j++) {
				std::cout << (u_int)dTable[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}
}



