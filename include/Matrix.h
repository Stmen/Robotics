/*
 * Matrix.h
 *
 *  Created on: 24 juli 2017
 *      Author: mateusz.fraszczynski
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <memory>							// shared_ptr
using namespace std;

class Matrix {

	friend class MatrixOperations;
	shared_ptr < vector < vector < int > > > matrixDataPtr;

	bool isEmpty( void ) const;
	bool isEqualSize( const Matrix & ) const;
	bool isColumnsNoEqualRowsNo( const Matrix & ) const;
	void allocateMatrixVectors( unsigned int , unsigned int );
	void allocateVectorInVector( void );
	bool verifyRowIndex( unsigned int ) const;
	bool verifyColumnIndex( unsigned int ) const;

	unsigned int getRowsNo( void ) const;
	const vector< int > * getRow( unsigned int ) const;
	unsigned int getColumnsNo( void ) const;
	bool getColumn( vector< const int * > * , unsigned int ) const;

public:
	Matrix();
	Matrix( unsigned int , unsigned int );
	virtual ~Matrix();

	void printMatrix( void );
	// Rows
	void addRow( void );
	bool addRowWithData( vector < int > * );
	bool fillRowWithData( int * , unsigned int );
	bool fillRowWithData( vector < int > * , unsigned int );
	// Columns
	void addColumn( void );
	bool addColumnWithData( vector < int > * );
	bool fillColumnWithData( int * , unsigned int );
	bool fillColumnWithData( vector < int > * , unsigned int );
	// Operators
	void operator =( shared_ptr < Matrix > );
	shared_ptr < Matrix > operator *( const Matrix & );
	shared_ptr < Matrix > operator +( const Matrix & );
	shared_ptr < Matrix > operator -( const Matrix & );
};

#endif /* MATRIX_H_ */
