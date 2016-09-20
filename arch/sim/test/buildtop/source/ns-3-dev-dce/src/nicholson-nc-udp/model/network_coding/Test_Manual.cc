/**
 * Copyright (c) 2015
 *	Muhammad Muhammad
 *	All rights reserved.
 *
 * @file Test_Manual.h
 *
 * @author 	Muhammad Muhammad
 * 		<mxmuhammad@gmail.com>
 * 		www.mxmuhammad.com
 */


/// This file is for testing purposes only

#include <stdio.h>
#include <sys/time.h>

#include "Matrix.h"


int main()
{
	/* get a random seed - time dependent */
	srand ( time(NULL) );

	// create a GF object
	GaloisField gf(16);
	
	/////////////////////////
	// Manual matrix filling
	/////////////////////////
	Matrix m;
	m.setGFPtr(gf);
	
	std::vector<uint8_t> vec;

	m.addConstant((uint8_t)11);
	vec.push_back((uint8_t)13);
	vec.push_back((uint8_t)15);
	vec.push_back((uint8_t)7);
	vec.push_back((uint8_t)8);
	m.addRow(vec);

	vec.clear();
	m.addConstant((uint8_t)9);
	vec.push_back((uint8_t)3);
	vec.push_back((uint8_t)1);
	vec.push_back((uint8_t)12);
	vec.push_back((uint8_t)4);
	m.addRow(vec);

	vec.clear();
	m.addConstant((uint8_t)7);
	vec.push_back((uint8_t)2);
	vec.push_back((uint8_t)4);
	vec.push_back((uint8_t)5);
	vec.push_back((uint8_t)9);
	m.addRow(vec);

	vec.clear();
	m.addConstant((uint8_t)10);
	vec.push_back((uint8_t)3);
	vec.push_back((uint8_t)8);
	vec.push_back((uint8_t)9);
	vec.push_back((uint8_t)4);
	m.addRow(vec);

	m.printSystem();
	m.gaussianElimination(false);
	m.printSystem();
	
	m.clearMatrix();
	m.generateRandomSystem(5, 5);
	m.printSystem();
	m.gaussianElimination(false);
	m.printSystem();

	return 0;
}
