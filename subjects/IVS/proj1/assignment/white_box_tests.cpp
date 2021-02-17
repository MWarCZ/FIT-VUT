//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"
#include <cmath>
//============================================================================//

// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

TEST(NonLegalMatrix, CreateMatrix0x0)
{
	Matrix mat;
	try
	{	
		mat = Matrix(0,0);
		ASSERT_EQ("ERROR","Set Matrix 0x0");
		
	}
	catch(...) { }
}

TEST(NonLegalMatrix, CreateMatrixNegativexNegative)
{
	Matrix mat;
	try
	{	
		mat = Matrix(-5,-4);
		ASSERT_EQ("ERROR","Set Matrix -5x-4");
		
	}
	catch(...) { }
}

TEST(NonLegalMatrix, CreateMatrixPositivexNegative)
{
	Matrix mat;
	try
	{	
		mat = Matrix(5,-3);
		ASSERT_EQ("ERROR","Set Matrix 5x-3");
		
	}
	catch(...) { }
}

TEST(NonLegalMatrix, CreateMatrixPositivexPositive)
{
	Matrix mat;
	try
	{	
		mat = Matrix(3,2);
		
	}
	catch(...) 
	{
		ASSERT_EQ("ERROR","Unset Matrix 3x2");
	}
}




struct ZeroMatrix1x1 : testing::Test
{
	Matrix mat;

	ZeroMatrix1x1()
	{
		mat = Matrix();
	}
	~ZeroMatrix1x1()
	{
	}

	
};
struct ZeroMatrix5x5 : testing::Test
{
	Matrix mat;

	ZeroMatrix5x5()
	{
		mat = Matrix(5, 5);
	}
	~ZeroMatrix5x5()
	{
	}
};

// 1x1
TEST_F(ZeroMatrix1x1, AbleGetAndSet )
{
	EXPECT_EQ( 0, mat.get(0, 0) );
	EXPECT_TRUE( mat.set(0, 0, 1) );
	EXPECT_EQ( 1, mat.get(0, 0) );
}
TEST_F(ZeroMatrix1x1, UnableSetOutOfRange )
{
	EXPECT_FALSE( mat.set(0, 5, 1) );
	EXPECT_FALSE( mat.set(5, 0, 1) );
	EXPECT_FALSE( mat.set(5, 5, 1) );
	EXPECT_FALSE( mat.set(-1, -1, 1) );
}
TEST_F(ZeroMatrix1x1, UnableGetOutOfRange)
{
	EXPECT_TRUE(isnan(mat.get(2,0)) ); 
	EXPECT_TRUE(isnan(mat.get(0,2)) );
	EXPECT_TRUE(isnan(mat.get(2,2)) );
}
// 5x5  
TEST_F(ZeroMatrix5x5, AbleSetByVector5x5)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5,7,9},
		{2,4,6,8,0},
		{0,1,2,3,4},
		{9,8,7,6,5},
		{1.1,2.2,3.3,4.4,5.5}
	};
	EXPECT_TRUE(mat.set(testData));
	for(int i = 0; i<5; i++)
	{
		for(int j = 0; j<5; j++)
		{
			ASSERT_EQ(mat.get(i,j), testData[i][j]);
		}
	}

} 

/*
 * Chyba v souboru white_box_code.cpp na řádku 49.
 * Dochází k porovnávání špatného rozměru.
 * Původni:
 * ... matrix[0].size() > values.size() ...
 * Mělo by tam spíše být:
 * ... matrix[0].size() > values[0].size()...
 * A ani to není správně. Chyba je v ciklu. Protože mi nic nezaručuje, že jsem zadal správní vektror vektorů.
 */
TEST_F(ZeroMatrix5x5, UnableSetByLittleVector5x3)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5},
		{2,4,6},
		{0,1,2},
		{9,8,7},
		{1.1,2.2,3.3}
	}; 
//	EXPECT_TRUE(mat.set(testData)); // Pouze proto aby šlo spustit LCOV a GCOV
	EXPECT_FALSE(mat.set(testData)); 
} 
TEST_F(ZeroMatrix5x5, UnableSetByLittleVector3x5)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5,7,9},
		{2,4,6,8,0},
		{1.1,2.2,3.3,4.4,5.5}
	};
	EXPECT_FALSE(mat.set(testData));
} 

TEST_F(ZeroMatrix5x5, UnableSetByLittleVector3x3)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5},
		{2,4,6},
		{1.1,2.2,3.3}
	};
	EXPECT_FALSE(mat.set(testData));
} 

/*
 * Stejná chyba viz popis u testu UnableSetByLittleVector5x3
 */
TEST_F(ZeroMatrix5x5, UnableSetByVector6x3)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5},
		{2,4,6},
		{9,8,7},
		{1,2,3},
		{1,0,9},
		{5,3,8}
	};
//	EXPECT_TRUE(mat.set(testData)); // Pouze proto aby šlo spustit LCOV a GCOV
	EXPECT_FALSE(mat.set(testData)); 
}

/*
 * Stejná chyba viz popis u testu UnableSetByLittleVector5x3
 */
TEST_F(ZeroMatrix5x5, UnableSetByVector5xN)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,2,3,4,5},
		{2},
		{3},
		{4},
		{5}
	};
//	EXPECT_TRUE(mat.set(testData)); // Pouze proto aby šlo spustit LCOV a GCOV
	EXPECT_FALSE(mat.set(testData)); 
} 


TEST_F(ZeroMatrix5x5, UnableSetByVector3x6)
{
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5,7,9,1},
		{2,4,6,8,0,2},
		{1,0,2,9,3,8},
		{5,6,4,7,3,8}
	};
	EXPECT_FALSE(mat.set(testData));
}


TEST_F(ZeroMatrix5x5, AbleSetByBigVector6x6)
{	
	std::vector<std::vector<double>> testData = 
	{
		{1,3,5,7,9,1},
		{2,4,6,8,0,2},
		{9,8,7,6,5,4},
		{1,2,3,4,5,6},
		{1,0,2,9,3,8},
		{5,6,4,7,3,8}
	};
	EXPECT_TRUE(mat.set(testData));
	
	for(int i = 0; i<5; i++)
	{
		for(int j = 0; j<5; j++)
		{
			ASSERT_EQ(mat.get(i,j), testData[i][j]);
		}
	}
}

/////////////////////////

struct SetMatrix : testing::Test
{
	Matrix mat1, mat2, mat3;
	SetMatrix()
	{
	}
	~SetMatrix()
	{
	}
	bool SetMatrixA(Matrix *mat,size_t rows, size_t cols)
	{
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols; j++)
			{
				mat->set(i,j,i+j);
			}
		}
	} 
	bool SetMatrixB(Matrix *mat,size_t rows, size_t cols)
	{
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols; j++)
			{
				mat->set(i,j,i*j);
			}
		}
	}
	bool SetMatrix1(Matrix *mat,size_t rows, size_t cols)
	{
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols; j++)
			{
				mat->set(i,j,1);
			}
		}
	}

};

// operator ==
TEST_F(SetMatrix, AbleCompareOneMatrix3x3)
{
	mat1 = Matrix(3,3);
	SetMatrixA(&mat1,3,3);

	EXPECT_TRUE(mat1 == mat1);
}

TEST_F(SetMatrix, AbleCompareTwoSameMatrix3x3)
{
	mat1 = Matrix(3,3);
	mat2 = Matrix(3,3);
	SetMatrixA(&mat1,3,3);
	SetMatrixA(&mat2,3,3);

	EXPECT_TRUE(mat1 == mat2);
}

TEST_F(SetMatrix, AbleCompareTwoDiffMatrix3x3)
{
	mat1 = Matrix(3,3);
	mat2 = Matrix(3,3);
	SetMatrixA(&mat1,3,3);
	SetMatrixB(&mat2,3,3);

	EXPECT_FALSE(mat1 == mat2);
}

TEST_F(SetMatrix, UnableCompareMatrix3x3andMatrix5x5)
{
	mat1 = Matrix(3,3);
	mat2 = Matrix(5,5);
	SetMatrixA(&mat1,3,3);
	SetMatrixA(&mat2,5,5);
	try
	{
		mat1 == mat2;
		ASSERT_EQ("ERROR","Compared Matrix 3x3 and Matrix 5x5 without exception!");
	}
	catch(...) { }
}

// operator +
TEST_F(SetMatrix, AbleAddTwoSameMatrix3x3)
{
	mat1 = Matrix(3,3);
	mat2 = Matrix(3,3);
	mat3 = mat1 + mat2;

	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			ASSERT_EQ(0, mat3.get(i,j) );
		}
	}
}
TEST_F(SetMatrix, AbleAddTwoDiffMatrix4x4)
{
	mat1 = Matrix(4,4);
	mat2 = Matrix(4,4);
	SetMatrixA(&mat1,4,4);

	mat3 = mat1 + mat2;

	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			ASSERT_EQ(mat1.get(i,j), mat3.get(i,j) );
		}
	}
}

TEST_F(SetMatrix, UnableAddMatrix3x3andMatrix4x4)
{
	mat1 = Matrix(3,3);
	mat2 = Matrix(4,4);
	
	try
	{
		mat3 = mat1 + mat2;
		ASSERT_EQ("ERROR","Add matrix 3x3 and matrix 4x4 without exception!");
	}
	catch(...) { }
}

// operator * 
TEST_F(SetMatrix, AbleMultiplyMatrix2x3andMatrix3x2)
{
	mat1 = Matrix(2,3);
	mat2 = Matrix(3,2);
	SetMatrix1(&mat1,2,3);
	SetMatrix1(&mat2,3,2);
	try
	{
		mat3 = mat1 * mat2;
		for(int i = 0; i<2; i++)
		{
			for(int j = 0; j<2; j++)
			{
				ASSERT_EQ(3,mat3.get(i,j));
			}
		}
	}
	catch(...) 
	{
		ASSERT_EQ("ERROR","Multiply matrix 2x3 and matrix 3x2 with exception!");
	}
}
 
TEST_F(SetMatrix, AbleMultiplyMatrix3x2andMatrix2x3)
{
	mat1 = Matrix(2,3);
	mat2 = Matrix(3,2);
	SetMatrix1(&mat1,2,3);
	SetMatrix1(&mat2,3,2);
	try
	{
		mat3 = mat2 * mat1;
		for(int i = 0; i<3; i++)
		{
			for(int j = 0; j<3; j++)
			{
				ASSERT_EQ(2,mat3.get(i,j));
			}
		}
	}
	catch(...) 
	{
		ASSERT_EQ("ERROR","Multiply matrix 2x3 and matrix 3x2 with exception!");
	}
}

TEST_F(SetMatrix, UnableMultiplyMatrix2x2andMatrix3x3)
{
	mat1 = Matrix(2,2);
	mat2 = Matrix(3,3);
	try
	{
		mat3 = mat1 * mat2;
		ASSERT_EQ("ERROR","Multiply matrix 2x2 and matrix 3x3 without exception!");
	}
	catch(...) 
	{
	}
}

TEST_F(SetMatrix, AbleMultiplyMatrix4x4andDouble)
{
	mat1 = Matrix(4,4);
	SetMatrixB(&mat1,4,4);

	mat2 = mat1 * 2.1;
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			ASSERT_EQ(mat1.get(i,j)*2.1,mat2.get(i,j));
		}
	}
}

//////////

// solveEquation 
struct Equation : testing::Test
{
	Matrix mat1, mat2, mat3;
	std::vector<std::vector<double> > dataL;
	std::vector<double> result, dataR; 

	Equation()
	{
	}
	~Equation()
	{
	}
};

/*
 * Chyba při výpočtu rovnice o jedné neznámé. 
 */
TEST_F(Equation, AbleSolveEquation1)
{
	mat1 = Matrix(1,1);
	dataL = 
	{
		{2}
	};
	mat1.set(dataL);
	//mat1.set(0,0,2);

	dataR = { 10 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( 1, result.size() );
//		EXPECT_EQ( reuslt[0], 0 );  // Pouze proto aby šlo spustit LCOV a GCOV
		EXPECT_EQ( result[0], 5 );
	}
	catch(...)
	{
		ASSERT_EQ("ERROR","solvedEquation throw exception!");
	}
}

TEST_F(Equation, AbleSolveEquation2)
{
	mat1 = Matrix(2,2);
	dataL = 
	{
		{4,3},
		{2,1}
	};
	mat1.set(dataL);

	dataR = { 6, 4 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( 2, result.size() );
		EXPECT_EQ( result[0], 3 );
		EXPECT_EQ( result[1], -2 );
	}
	catch(...)
	{
		ASSERT_EQ("ERROR","solvedEquation throw exception!");
	}
}

TEST_F(Equation, UnableSolveEquationWithShortRight)
{
	mat1 = Matrix(2,2);
	dataL = 
	{
		{4,3},
		{2,1}
	};
	mat1.set(dataL);

	dataR = { 6 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( "ERROR", "Two left equation, but only one on right." );
	}
	catch(...)
	{
	}
}

TEST_F(Equation, UnableSolveEquationWithNonsquareMatrix)
{
	mat1 = Matrix(2,4);
	dataR = { 6, 7, 8, 9 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( "ERROR", "Matrix is not square." );
	}
	catch(...)
	{
	}
}

TEST_F(Equation, UnableSolveEquationWithDeterminat0)
{
	mat1 = Matrix();
	dataR = { 0 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( "ERROR", "Determinant is 0." );
	}
	catch(...)
	{
	}
}

TEST_F(Equation, AbleSolveEquation3)
{
	mat1 = Matrix(3,3);
	dataL = 
	{
		{1,-1,-1},
		{-1,1,-1},
		{-1,-1,1}
	};
	mat1.set(dataL);

	dataR = { 5,1,-15 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( 3, result.size() );
		EXPECT_EQ( result[0], 7 );
		EXPECT_EQ( result[1], 5 );
		EXPECT_EQ( result[2], -3);
	}
	catch(...)
	{
		ASSERT_EQ("ERROR","solvedEquation throw exception!");
	}
}

TEST_F(Equation, AbleSolveEquation4)
{
	mat1 = Matrix(4,4);
	dataL = 
	{
		{2,2,-3,1},
		{1,2,4,2},
		{-1,1,-1,1},
		{1,-1,2,-2}
	};
	mat1.set(dataL);

	dataR = { 3,5,1,-4 }; 
	try
	{
		result = mat1.solveEquation(dataR);
		ASSERT_EQ( 4, result.size() );
		EXPECT_EQ( result[0], 1 );
		EXPECT_EQ( result[1], -1 );
		EXPECT_EQ( result[2], 0);
		EXPECT_EQ( result[3], 3 );
	}
	catch(...)
	{
		ASSERT_EQ("ERROR","solvedEquation throw exception!");
	}
}






/*** Konec souboru white_box_tests.cpp ***/
