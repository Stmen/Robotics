/*
 * MatrixOperations.h
 *
 *  Created on: 2 aug. 2017
 *      Author: mateusz.fraszczynski
 */

#ifndef MATRIXOPERATIONS_H_
#define MATRIXOPERATIONS_H_

#include <vector>
#include "Matrix.h"

using namespace std;

class MatrixOperations {

	static double vectorMultiplication( const vector < double > * , vector < const double * > * );
	static double smallDetCalculation( const Matrix & base );
	static double detSarrusMethod( const Matrix & );
	static void detSarrusMethodFunWay( const Matrix & );
	static double detLaplaceMethod( const Matrix & );
	static double detGaussMethod( const Matrix & );
	static void createSubMatrix( const Matrix & , Matrix & , unsigned int , unsigned int );
	static unsigned int subRank( const Matrix & );

public:
	MatrixOperations();
	virtual ~MatrixOperations();
	static void multiplication( Matrix & , const Matrix & , const Matrix & );
	static void scalarmultiplication( Matrix & , const Matrix & , double );
	static void scalarmultiplication( Matrix & , double );
	static void addition( Matrix & , const Matrix & , const Matrix & );
	static void subtraction( Matrix & , const Matrix & , const Matrix & );
	static void transposition( Matrix & , const Matrix & );
	static double determinant( const Matrix & );
	static unsigned int rank( const Matrix & );
	//TODO Following implementation needed
	static void inverse( Matrix & , const Matrix & );
	static void inverse( Matrix & );


};

#endif /* MATRIXOPERATIONS_H_ */
