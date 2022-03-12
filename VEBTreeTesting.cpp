#include "pch.h"
#include "VEBTree.h"
#include <chrono>
#include <fstream>
#include <string>
#include <set>

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


void createVebTreeTestFiles(int maxNumber, float fileMultiplier) {
	std::ofstream vebTreeTestFile1("vebTreeTestFile1");
	int fileSize = maxNumber * fileMultiplier;
	std::srand(49517);
	for (int i = 0; i < fileSize; i++)
		vebTreeTestFile1 << std::rand() % maxNumber << "\n";
	vebTreeTestFile1.close();

	std::ofstream vebTreeTestFile2("vebTreeTestFile2");
	std::srand(75694);
	for (int i = 0; i < fileSize; i++)
		vebTreeTestFile2 << std::rand() % maxNumber << "\n";
	vebTreeTestFile2.close();


	std::ofstream vebTreeTestFile3("vebTreeTestFile3");
	std::srand(63781);
	for (int i = 0; i < fileSize; i++)
		vebTreeTestFile3 << std::rand() % maxNumber << "\n";
	vebTreeTestFile3.close();
}

int runTestFile(int maxNumber, std::string fileName, std::string printName) {
	vebTree testTree;
	int treeSize = maxNumber;
	std::string fileInput;
	testTree.u = treeSize;
	std::ifstream fileinsert(fileName);
	int testNumber = 0;
	int checkTime;

	auto start = std::chrono::high_resolution_clock::now();
	while (std::getline(fileinsert, fileInput)) {
		testTree.safeInsert(stoi(fileInput));
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
	filecheck.close();
	checkTime = duration.count();

	int currentNum = 0;
	int numCount = 0;
	if (testTree.valueInTree(currentNum))
		numCount++;
	while(currentNum != -1){
		currentNum = testTree.findSuccessor(currentNum);
		numCount++;
	}
	std::cout << "Total Number of unique number counted by findSuccessor: " << numCount << " Percentage: "<< 1.0*numCount/maxNumber << "\n";

	currentNum = maxNumber - 1;
	numCount = 0;
	if (testTree.valueInTree(currentNum))
		numCount++;
	while (currentNum != -1) {
		currentNum = testTree.findPrevious(currentNum);
		numCount++;
	}
	std::cout << "Total Number of unique number counted by findPrevious: " << numCount << "\n";

	std::ifstream fileDelete(fileName);
	start = std::chrono::high_resolution_clock::now();
	while (std::getline(fileDelete, fileInput)) {
		testTree.safeDelete(stoi(fileInput));
	}
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	fileDelete.close();
	if (testTree.min == -1 && testTree.max == -1)
		std::cout << "Successful Delete, ";
	else
		std::cout << "Failed Delete, ";
	std::cout << printName << " Delete time(milliseconds): " << duration.count() << "\n";

	return checkTime;
}

int runTestFileOnBinaryTree(int maxNumber, std::string fileName, std::string printName) {
	std::set<int> testTree;
	int treeSize = maxNumber;
	std::string fileInput;
	std::ifstream fileinsert(fileName);
	int testNumber = 0;
	int checkTime;

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
		testTree.find(stoi(fileInput));
	}
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << printName << " check time(milliseconds): " << duration.count() << "\n";
	filecheck.close();
	checkTime = duration.count();

	std::ifstream fileDelete(fileName);
	start = std::chrono::high_resolution_clock::now();
	while (std::getline(fileDelete, fileInput)) {
		testTree.erase(stoi(fileInput));
	}
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	fileDelete.close();
	std::cout << printName << " Delete time(milliseconds): " << duration.count() << "\n";
	return checkTime;
}

void runVEBTreeFullTest(int maxNumber,float fileMultiplier) {

	createVebTreeTestFiles(maxNumber, fileMultiplier);
	int averageCheckTime;
	float averageCheckTimePerCheck;
	float estimatedConstant = 0.001215;
	std::cout << "\nVan Emde Boas run times on the files of range 0 to " << maxNumber - 1 << ": \n";
	averageCheckTime = runTestFile(maxNumber, "vebTreeTestFile1", "File1");
	averageCheckTime += runTestFile(maxNumber, "vebTreeTestFile2", "File2");
	averageCheckTime += runTestFile(maxNumber, "vebTreeTestFile3", "File3");
	std::cout << "Van Emde Boas average check time =  " << averageCheckTime / 3 << ": \n";
	averageCheckTimePerCheck = averageCheckTime / (3.0*fileMultiplier*maxNumber);
	std::cout << "Van Emde Boas average check time per check =  " << averageCheckTimePerCheck << ": \n";
	std::cout << "Van Emde Boas log log u estimation =  " << averageCheckTimePerCheck/estimatedConstant << ": \n";

	std::cout << " \nstd::set run times on the same files: \n";

	averageCheckTime = runTestFileOnBinaryTree(maxNumber, "vebTreeTestFile1", "File1");
	averageCheckTime += runTestFileOnBinaryTree(maxNumber, "vebTreeTestFile2", "File2");
	averageCheckTime += runTestFileOnBinaryTree(maxNumber, "vebTreeTestFile3", "File3");
	std::cout << "std::set average check time =  " << averageCheckTime / 3 << ": \n";
	averageCheckTimePerCheck = averageCheckTime / (3.0*fileMultiplier*maxNumber);
	std::cout << "std::set average check time per check =  " << averageCheckTimePerCheck << ": \n";
}

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
	int treeSize = 32;
	testTree.u = treeSize;
	int testNumber = 0;

	std::cout << "Min/max Tests: \n";
	testTree.safeInsert(1);
	asserting(testTree.min, 1, testNumber);//test that 1 is min
	asserting(testTree.max, 1, testNumber);//test that 1 is max
	testTree.safeInsert(0);
	testTree.safeInsert(3);
	testTree.safeInsert(15);

	std::cout << "FindSuccessor Tests: \n";
	asserting(testTree.findSuccessor(1), 3, testNumber);
	asserting(testTree.findSuccessor(0), 1, testNumber);
	asserting(testTree.findSuccessor(2), 3, testNumber);
	asserting(testTree.findSuccessor(3), 15, testNumber);

	std::cout << "FindPredecessor Tests: \n";
	asserting(testTree.findPrevious(1), 0, testNumber);
	asserting(testTree.findPrevious(15), 3, testNumber);
	asserting(testTree.findPrevious(3), 1, testNumber);
	asserting(testTree.findPrevious(6), 3, testNumber);

	std::cout << "Value in tree Tests: \n";
	assertTruth(testTree.valueInTree(0), true, testNumber);
	assertTruth(testTree.valueInTree(1), true, testNumber);
	assertTruth(testTree.valueInTree(2), false, testNumber);
	assertTruth(testTree.valueInTree(3), true, testNumber);

	std::cout << "Delete Tests: \n";
	testTree.safeDelete(0);
	assertTruth(testTree.valueInTree(0), false, testNumber);
	testTree.safeInsert(0);
	assertTruth(testTree.valueInTree(0), true, testNumber);
	testTree.safeDelete(0);
	assertTruth(testTree.valueInTree(0), false, testNumber);
	testTree.safeDelete(1);

	testTree.safeInsert(0);
	assertTruth(testTree.valueInTree(0), true, testNumber);
	testTree.safeInsert(0);
	assertTruth(testTree.valueInTree(0), true, testNumber);

	testTree.safeDelete(1);
	assertTruth(testTree.valueInTree(1), false, testNumber);
	testTree.safeInsert(1);
	assertTruth(testTree.valueInTree(1), true, testNumber);

	std::cout << "Full Delete Tests: \n";

	for (int i = 0; i < treeSize; i++) {
		testTree.safeInsert(i);
	}
	for (int i = 0; i < treeSize; i++) {
		testTree.safeDelete(i);
	}
	for (int i = 0; i < treeSize; i++) {
		assertTruth(testTree.valueInTree(i), false, testNumber);
	}


}

void runVEBBigTreeTests() {

	vebTree testTree;
	int treeSize = 131072;
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

int main() {

	float fileMultiplier = 1.0;
	returnLowerSquareRootTest();
	runVEBTreeFullTest(1048576, fileMultiplier);
	runVEBTreeFullTest(524288, fileMultiplier);
	runVEBTreeFullTest(262144, fileMultiplier);
	runVEBTreeFullTest(131072, fileMultiplier);
	runVEBTreeFullTest(65536, fileMultiplier);
	runVEBTreeFullTest(32768, fileMultiplier);
	runVEBTreeFullTest(16384, fileMultiplier);
	runVEBTreeFullTest(8192, fileMultiplier);
	runVEBBigTreeTests();
	runVEBSmallTreeTests();
	testVEBHelperFunctions();
	return 0;
}
