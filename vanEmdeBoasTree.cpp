// Credits due:
// Wikipedia https://en.wikipedia.org/wiki/Van_Emde_Boas_tree
// Geeks for Geeks https://www.geeksforgeeks.org/van-emde-boas-tree-set-1-basics-and-construction/
// Easy to parse source(with errors) that Geeks for Geeks linked to http://www-di.inf.puc-rio.br/~laber/vanEmdeBoas.pdf
// Textbook https://ebookcentral.proquest.com/lib/ucr/reader.action?docID=3339142

#include "pch.h"
#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <stdlib.h>
#include <string>


int returnLowerSquareRoot(int u) { 
	int lower = log2(u * 2) / 2;
	return pow(2, lower);
}
int returnHighKey(int x, int u) { return x / returnLowerSquareRoot(u); }
int returnLowKey(int x, int u) { return x % returnLowerSquareRoot(u); }
int returnIndex(int x, int u, int y) { return x * returnLowerSquareRoot(u) + y; }




void asserting(int assertion, int expected, int &testNumber) {
	if (assertion == expected)
		std::cout << " Test Number " << testNumber << ": assert " << assertion << " is equal to " << expected << " PASSED! \n";
	else
		std::cout << " Test Number " << testNumber << ": assert " << assertion << " is equal to " << expected << " FAILED! \n";

	testNumber++;
}

void assertTruth(bool assertion, bool expectation, int &testNumber) {
	if (assertion == expectation)
		std::cout << " Test Number " << testNumber << ": assert " << assertion << " is equal to " << expectation << " PASSED! \n";
	else
		std::cout << " Test Number " << testNumber << ": assert " << assertion << " is equal to " << expectation << " FAILED! \n";

	testNumber++;

}

bool assertTruthNoCout(bool assertion, bool expectation, int &testNumber) {
	if (assertion == expectation)
		return true;
	else
		return false;
}



/*Change History:
-removed redundant if(n == 2 ||...) from valueInTree , no change in runtime
-reduced returnHighKey calculations by saving calculation in a variable(runtime 105->90)
-further reduced calculations using the same trick(runtime 90->75)
-removing for loop to insert u into new vebTrees by having outer variable(TESTUTOSS), no change in runtime
-removed unneccessary recursive call (runtime 75seconds->283milliseconds)
-figured out the the wrong equation was being used to calculate the universe of each cluster(fixes must now be made)
-returnLowKey uses returnLowerSquareRoot.
-learned that major source had missleading information. changed returnLowerSquareRoot in order to compensate.
-squashed bug in findSuccessor if(u ==2 || cluster ==nullptr). cluster == nullptr was preventing single value clusters from returning valid numbers.
*/




class vebTree {
public:
	int u;
	int min;
	int max;
	vebTree * summary;
	vebTree * cluster;

	vebTree() {
		min = -1;
		max = -1;
		summary = nullptr;
		cluster = nullptr;

	}

	vebTree(int passedU) {
		min = -1;
		max = -1;
		summary = nullptr;
		cluster = nullptr;
		u = passedU;
	}
	vebTree(int passedU, int passedMax) {
		u = passedU;
		min = passedMax;
		max = passedMax;
		summary = nullptr;
		cluster = nullptr;
	}
	~vebTree() {
		delete summary;
		delete[] cluster;
	}

	void runVEBTreeFullTest(int maxNumber) {
		createVebTreeTestFiles(maxNumber);
		runTestFile(maxNumber, "vebTreeTestFile1", "File1");
		runTestFile(maxNumber, "vebTreeTestFile2", "File2");
		runTestFile(maxNumber, "vebTreeTestFile3", "File3");
	}

	void createVebTreeTestFiles(int maxNumber) {
		std::ofstream vebTreeTestFile1("vebTreeTestFile1");
		std::srand(49517);
		for (int i = 0; i < maxNumber*1.4; i++)
			vebTreeTestFile1 << std::rand() % maxNumber << "\n";
		vebTreeTestFile1.close();

		std::ofstream vebTreeTestFile2("vebTreeTestFile2");
		std::srand(75694);
		for (int i = 0; i < maxNumber*1.4; i++)
			vebTreeTestFile2 << std::rand() % maxNumber << "\n";
		vebTreeTestFile2.close();


		std::ofstream vebTreeTestFile3("vebTreeTestFile3");
		std::srand(63781);
		for (int i = 0; i < maxNumber*1.4; i++)
			vebTreeTestFile3 << std::rand() % maxNumber << "\n";
		vebTreeTestFile3.close();
	}



	void runTestFile(int maxNumber, std::string fileName, std::string printName) {
		vebTree testTree;
		int treeSize = maxNumber;
		std::string fileInput;
		testTree.u = treeSize;
		std::ifstream fileinsert(fileName);

		int testNumber = 0;

		auto start = std::chrono::high_resolution_clock::now();

		while (std::getline(fileinsert, fileInput)) {
			testTree.insert(stoi(fileInput));
		}

		auto stop = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

		std::cout << printName << " insert time(milliseconds): " << duration.count() << "\n";

		fileinsert.close();

		std::ifstream filecheck(fileName);

		start = std::chrono::high_resolution_clock::now();

		while (std::getline(filecheck, fileInput)) {
			if (assertTruthNoCout(testTree.valueInTree(stoi(fileInput)), true, testNumber) == false) {
				std::cout << "\n FAILED " << printName << " \n" << testNumber;
				std::cout << "\n FAILED " << printName << " \n" << testNumber;
				break;
			}
		}

		stop = std::chrono::high_resolution_clock::now();

		duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

		std::cout << printName << " check time(milliseconds): " << duration.count() << "\n";

	}

	bool insert(int numToInsert) {

		if (min == -1) {
			min = numToInsert;
			max = numToInsert;
			return true;
		}
		if (numToInsert < min) {
			int swapper = min;
			min = numToInsert;
			numToInsert = swapper;
		}
		if (u > 2) {

			int highKey = returnHighKey(numToInsert, u);
			int lowKey = returnLowKey(numToInsert, u);

			if (cluster == nullptr) {
				int upperSquareRoot = returnLowerSquareRoot(u);
				int lowerSquareRoot = returnLowerSquareRoot(u);

				cluster = new vebTree[upperSquareRoot];
				for (int i = 0; i < upperSquareRoot; i++) {
					cluster[i].u = lowerSquareRoot;
				}
			}
			if (summary == nullptr) {
				summary = new vebTree(returnLowerSquareRoot(u));
			}
			if (cluster[returnHighKey(numToInsert, u)].min == -1) {

				summary->insert(highKey);

				cluster[highKey].min = lowKey;
				cluster[highKey].max = lowKey;
			}
			else {
				cluster[highKey].insert(lowKey);
			}
		}

		if (numToInsert > max)
			max = numToInsert;
		return true;
	}

	bool valueInTree(int numToFind) {

		if (numToFind == min || numToFind == max)
			return true;
		if (u == 2 || cluster == nullptr)
			return false;
		return cluster[returnHighKey(numToFind, u)].valueInTree(returnLowKey(numToFind, u));
	}

	int returnMinimum() {
		return min;
	}

	int returnMaximum() {
		return max;
	}

	int findSuccessor(int numToFindSuccessorOf) {

		std::cout << min << " " << max << "\n";
		if (u == 2)
			if (numToFindSuccessorOf == 0 && max == 1)
				return 1;
			else
				return -1;
		else if ( min != -1 && numToFindSuccessorOf < min)
			return min;
		else {
			int highKey = returnHighKey(numToFindSuccessorOf, u);
			int lowKey = returnLowKey(numToFindSuccessorOf, u);
			int maxOfCluster = cluster[highKey].max;
			if (maxOfCluster != -1 && lowKey < maxOfCluster) {
				return returnIndex(highKey, u, cluster[highKey].findSuccessor(lowKey));
			}
			else {
				int successorCluster = summary->findSuccessor(highKey);
				if (successorCluster == -1)
					return -1;
				else {

					return returnIndex(successorCluster, u, cluster[successorCluster].min);
				}
			}
		}
	}

	bool deleteValueInTree(int numToFind) {
		if (min == max) {
			min = -1;
			max = -1;
		}
		else if (u == 2) {
			if (numToFind == 0)
				min = 1;
			else
				min = 0;
			max = min;
		}
		else if(numToFind == min){
			int firstCluster = summary->min;
			numToFind = returnIndex(firstCluster, u, cluster[firstCluster].min);
			min = numToFind;
		}
		int highKey = returnHighKey(numToFind, u);
		int lowKey = returnLowKey(numToFind, u);
		cluster[highKey].deleteValueInTree(lowKey);
		if (cluster[highKey].min == -1) {
			summary->deleteValueInTree(highKey);
			if (numToFind == max){
				int summaryMax = summary->max;
				if (summaryMax == -1)
					max = min;
				else 
					max = returnIndex(summaryMax, u, cluster[summaryMax].max);
			
			}
		}
		else if(numToFind == max){
			int highKey = returnHighKey(numToFind,u);
			max = returnIndex(highKey, u , cluster[highKey].max);
		}

	}
	

};


void returnLowerSquareRootTest() {
	int testNumber = 0;
	
	asserting(returnLowerSquareRoot(4), 2, testNumber);
	asserting(returnLowerSquareRoot(8), 4, testNumber);
	asserting(returnLowerSquareRoot(16), 4, testNumber);
	asserting(returnLowerSquareRoot(32), 8, testNumber);
	asserting(returnLowerSquareRoot(64), 8, testNumber);
	asserting(returnLowerSquareRoot(128), 16, testNumber);
	asserting(returnLowerSquareRoot(256), 16, testNumber);
}

void runVEBSmallTreeTests() {
	vebTree testTree;
	int treeSize = 64;
	testTree.u = treeSize;

	int testNumber = 0;

	testTree.insert(1);
	asserting(testTree.min, 1, testNumber);//test that 1 is min
	asserting(testTree.max, 1, testNumber);//test that 1 is max
	testTree.insert(0);
	testTree.insert(3);
	testTree.insert(15);

	asserting(testTree.findSuccessor(1), 3, testNumber);
	asserting(testTree.findSuccessor(0), 1, testNumber);
	asserting(testTree.findSuccessor(2), 3, testNumber);
	asserting(testTree.findSuccessor(3), 15, testNumber);


	assertTruth(testTree.valueInTree(0), true, testNumber);
	assertTruth(testTree.valueInTree(1), true, testNumber);

	assertTruth(testTree.valueInTree(2), false, testNumber);
	assertTruth(testTree.valueInTree(3), true, testNumber);

	/*
	for (int i = 0; i < treeSize; i++) {
		testTree.insert(i);
	}
	for (int i = 0; i < treeSize; i++) {
		assertTruth(testTree.valueInTree(i), true, testNumber);
	}
	*/
}

void runVEBBigTreeTests() {
	vebTree testTree;
	int treeSize = 131072;//Current maximum due to memory restrictions. takes about 4.1 seconds to insert and 0.5 seconds to check. 4.631 seconds total.
	//int treeSize = 8;
	testTree.u = treeSize;

	int testNumber = 0;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < treeSize; i++) {
		testTree.insert(i);
	}

	for (int i = 0; i < treeSize; i++) {
		if (assertTruthNoCout(testTree.valueInTree(i), true, testNumber) == false) {
			std::cout << "\n FAILED \n" << testNumber;
			std::cout << "\n FAILED \n" << testNumber;
			break;
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << "\n" << duration.count();
}

void testVEBHelperFunctions() {
	int testNumber = 0;

	asserting(returnLowerSquareRoot(4), 2, testNumber);

	asserting(returnLowerSquareRoot(8), 2, testNumber);

}

int main()
{
	vebTree testTree;
	//returnLowerSquareRootTest();
	testTree.runVEBTreeFullTest(131072);
	//runVEBBigTreeTests();
	//runVEBSmallTreeTests();
	//testVEBHelperFunctions();
	return 0;
}