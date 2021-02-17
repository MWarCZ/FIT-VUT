//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

/**
 * Struktura pro testování prázdného stromu.
 */
struct Empty_BinaryTree_Test : testing::Test
{
	BinaryTree* tree; 
	std::pair<bool, BinaryTree::Node_t *> x1,x2;
	int testNum;
	bool b;
	BinaryTree::Node_t *node;

	Empty_BinaryTree_Test()
	{
		tree = new BinaryTree();
	}
	virtual ~Empty_BinaryTree_Test()
	{
		delete tree;
	}
};

// Empty BinaryTree - is realy empty?
TEST_F(Empty_BinaryTree_Test, IsReallyEmpty)
{
	EXPECT_TRUE(tree->GetRoot() == NULL);
	std::vector<BinaryTree::Node_t *> outNodes;
	tree->GetAllNodes(outNodes);
	EXPECT_TRUE(outNodes.size() == 0);
	tree->GetLeafNodes(outNodes);
	EXPECT_TRUE(outNodes.size() == 0);
	tree->GetNonLeafNodes(outNodes);
	EXPECT_TRUE(outNodes.size() == 0);
}

// Empty BinaryTree - InsertNode
TEST_F(Empty_BinaryTree_Test, AbleToInsertPositiveNumber)
{
	x1 = tree->InsertNode(2);
	EXPECT_EQ(true, x1.first);
}

TEST_F(Empty_BinaryTree_Test, AbleToInsertNegativeNumber)
{
	x1 = tree->InsertNode(-2);
	EXPECT_EQ(true, x1.first);
}

TEST_F(Empty_BinaryTree_Test, AbleToInsertDiffNumbers)
{
	testNum = 2;

	x1 = tree->InsertNode(2);
	ASSERT_EQ(true, x1.first);

	testNum = 5;

	x2 = tree->InsertNode(5);
	ASSERT_EQ(true, x2.first);
	EXPECT_NE(x1.second, x2.second);
}

TEST_F(Empty_BinaryTree_Test, UnableToInsertSameNumbers)
{
	testNum = 10;

	x1 = tree->InsertNode(testNum);
	ASSERT_EQ(true, x1.first);
	x2 = tree->InsertNode(testNum);
	ASSERT_NE(true, x2.first);
	EXPECT_EQ(x1.second, x2.second);
}

// Empty BinaryTree - DeleteNode
TEST_F(Empty_BinaryTree_Test, UnableToDeleteNumber)
{
	b = tree->DeleteNode(5);
	EXPECT_NE(true, b);
}

// Empty BinaryTree - FindNode
TEST_F(Empty_BinaryTree_Test, UnableToFindNumber)
{;
	node = tree->FindNode(10);
	EXPECT_EQ(NULL, node);
}

// Empty BinaryTree - is all pointers null in first insert and second insert?
/*
 * Jaktože nově vložená položka do prázdného stromu něco ukazuje?
 * Jelikož by ve stomu měla být jen jediná položka, měli by být její ukazatele nastaveny na NULL.
 */
TEST_F(Empty_BinaryTree_Test, IsPointersNullForNewInsert)
{
	testNum = 5;
	x1 = tree->InsertNode(testNum);
	ASSERT_EQ(true, x1.first);   
	EXPECT_TRUE(x1.second->pLeft == NULL);
	EXPECT_TRUE(x1.second->pRight == NULL);
}


/**
 * Struktura pro testovování neprázdného stromu.
 */
struct Nonempty_BinaryTree_Test : testing::Test
{
	BinaryTree* tree;
	std::pair<bool, BinaryTree::Node_t *> x1,x2; 
	int testNum;
	bool b;
	BinaryTree::Node_t *node;

	Nonempty_BinaryTree_Test()
	{
		tree = new BinaryTree();
		tree->InsertNode(0);    
		tree->InsertNode(1);
		tree->InsertNode(2);
		tree->InsertNode(-1);
		tree->InsertNode(-2);

	}
	virtual ~Nonempty_BinaryTree_Test()
	{
		delete tree;
	}
};


// Nonempty BinaryTree - InsertNode 
TEST_F(Nonempty_BinaryTree_Test, AbleToInsertDiffPositiveNumber)
{
	x1 = tree->InsertNode(5);
	EXPECT_EQ(true, x1.first);
}

TEST_F(Nonempty_BinaryTree_Test, AbleToInsertDiffNegativeNumber)
{
	x1 = tree->InsertNode(-5);
	EXPECT_EQ(true, x1.first);
}
 
TEST_F(Nonempty_BinaryTree_Test, UnableToInsertSamePositiveNumber)
{
	x1 = tree->InsertNode(2);
	EXPECT_NE(true, x1.first);
}

TEST_F(Nonempty_BinaryTree_Test, UnableToInsertSameNegativeNumber)
{
	x1 = tree->InsertNode(-2);
	EXPECT_NE(true, x1.first);
}

TEST_F(Nonempty_BinaryTree_Test, AbleToInsertDiffNumbers)
{
	testNum = 10;

	x1 = tree->InsertNode(testNum);
	ASSERT_EQ(true, x1.first);

	testNum = -10;

	x2 = tree->InsertNode(testNum);
	ASSERT_EQ(true, x2.first);
	EXPECT_NE(x1.second, x2.second);
}

TEST_F(Nonempty_BinaryTree_Test, UnableToInsertSameNumbers)
{
	testNum = 10;

	x1 = tree->InsertNode(testNum);
	ASSERT_EQ(true, x1.first);
	x2 = tree->InsertNode(testNum);
	ASSERT_NE(true, x2.first);
	EXPECT_EQ(x1.second, x2.second);
}

// Nonempty BinaryTree - DeleteNode  
TEST_F(Nonempty_BinaryTree_Test, AbleToDeleteNumber)
{
	b = tree->DeleteNode(1);
	EXPECT_EQ(true, b);
}

TEST_F(Nonempty_BinaryTree_Test, UnableToDeleteNumber)
{
	b = tree->DeleteNode(10);
	EXPECT_NE(true, b);
}

// Nonempty BinaryTree - FindNode  
TEST_F(Nonempty_BinaryTree_Test, UnableToFindNumber)
{ 
	node = tree->FindNode(10);
	EXPECT_EQ(NULL, node);
}

// Nonempty BinaryTree - InsertNode and FindNode 
TEST_F(Nonempty_BinaryTree_Test, AbleToInsertAndFindNumber)
{
	testNum = 11;

	x1 = tree->InsertNode(testNum);
	node = tree->FindNode(testNum);
	ASSERT_EQ(true, x1.first);
	EXPECT_EQ(x1.second, node);
}
 
TEST_F(Nonempty_BinaryTree_Test, AbleToFindNumberAndInsertSameNumber)
{
	testNum = 0;

	node = tree->FindNode(testNum);
	x1 = tree->InsertNode(testNum);
	ASSERT_NE(true, x1.first);
	EXPECT_EQ(x1.second, node);
}

// Nonempty BinaryTree - DeleteNode and FindNode
TEST_F(Nonempty_BinaryTree_Test, UnableToFindAndDeleteAndFindNumber)
{
	BinaryTree::Node_t *node2;
	testNum = 1;

	node = tree->FindNode(testNum);
	b = tree ->DeleteNode(testNum);
	ASSERT_EQ(true, b);
	node2 = tree->FindNode(testNum);
	EXPECT_NE(node, node2);
}

// Nonempty BinaryTree - 
TEST_F(Nonempty_BinaryTree_Test, IsPointersNullInAllLeaf)
{
	std::vector<BinaryTree::Node_t *> outNodes;
	tree->GetLeafNodes(outNodes);
	ASSERT_TRUE(outNodes.size() > 0);
	
	for(std::vector<BinaryTree::Node_t *>::iterator it = outNodes.begin(); it != outNodes.end(); ++it)
	{
		EXPECT_FALSE( (*it)->pParent == NULL );
		EXPECT_TRUE( (*it)->pLeft == NULL);
		ASSERT_TRUE( (*it)->pRight == NULL );
	}	
	
}

TEST_F(Nonempty_BinaryTree_Test, IsNewInsertCorrect)
{
	testNum = 20;
	x1 = tree->InsertNode(testNum);
	ASSERT_TRUE(x1.first);

	if(x1.second->pLeft != NULL)
	{
		try 
		{
			node = x1.second->pLeft;
			ASSERT_TRUE(node->color == BinaryTree::BLACK || node->color == BinaryTree::RED);
		}
		catch(...)
		{
			ASSERT_EQ("FATAL","ERROR");
		}
	}
}


//////////////////////////////////////////
// Axiomy

/**
 * Struktura pro testovování neprázdného stromu.
 */
struct Axiom_Nonempty_BinaryTree_Test : testing::Test
{
	BinaryTree* tree;
	//std::pair<bool, BinaryTree::Node_t *> x1,x2; 
	//int testNum;
	//bool b;
	BinaryTree::Node_t *node;
	std::vector<BinaryTree::Node_t *> nodes1, nodes2;

	Axiom_Nonempty_BinaryTree_Test()
	{
		tree = new BinaryTree();
		tree->InsertNode(0);    
		tree->InsertNode(1);
		tree->InsertNode(2);
		tree->InsertNode(3);
		tree->InsertNode(4);
		tree->InsertNode(16);
		tree->InsertNode(22);
		tree->InsertNode(-5);
		tree->InsertNode(-66);
		tree->InsertNode(-4);
		tree->InsertNode(-8);

	}
	virtual ~Axiom_Nonempty_BinaryTree_Test()
	{
		delete tree;
	}
};

// Prvni axiom
TEST_F(Axiom_Nonempty_BinaryTree_Test, LeafNodesAreBlack)
{
	tree->GetLeafNodes(nodes1);
	
	for(std::vector<BinaryTree::Node_t *>::iterator it = nodes1.begin(); it != nodes1.end(); ++it)
	{
		ASSERT_EQ(true, (*it)->IsLeaf() );
		ASSERT_EQ(BinaryTree::BLACK, (*it)->color);
	}	
}

// Druhy axiom
TEST_F(Axiom_Nonempty_BinaryTree_Test, RedNonLeafNodesHaveBlackLeaf)
{
	tree->GetAllNodes(nodes1);
	
	for(std::vector<BinaryTree::Node_t *>::iterator it = nodes1.begin(); it != nodes1.end(); ++it)
	{
		if( (*it)->color == BinaryTree::RED)
		{
			ASSERT_EQ(BinaryTree::BLACK, (*it)->pLeft->color );
			ASSERT_EQ(BinaryTree::BLACK, (*it)->pRight->color );
		}
	}	
}

// Třeti axiom
TEST_F(Axiom_Nonempty_BinaryTree_Test, PathFromLeafNodeToRootHasSameNumberBlackNode)
{
	int lastBlack = -1;
	BinaryTree::Node_t *root = tree->GetRoot();
	tree->GetLeafNodes(nodes1);

	for(std::vector<BinaryTree::Node_t *>::iterator it = nodes1.begin(); it != nodes1.end(); ++it)
	{
		node = *it;
		int black = 0;

		while( node != root && node->pParent != NULL)
		{
			node = node->pParent;
			if(node->color == BinaryTree::BLACK)
			{
				black++;
			}
		}
		if(lastBlack >= 0)
		{
			ASSERT_EQ(lastBlack, black);
			//std::cout<<"\nlast: "<<lastBlack<<" now: "<<black<<"\n";
		}
		lastBlack = black;
	}	
}



/*** Konec souboru black_box_tests.cpp ***/
