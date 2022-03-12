// Credits due:
// Wikipedia https://en.wikipedia.org/wiki/Van_Emde_Boas_tree
// Geeks for Geeks https://www.geeksforgeeks.org/van-emde-boas-tree-set-1-basics-and-construction/
// Easy to parse source(with errors) that Geeks for Geeks linked to http://www-di.inf.puc-rio.br/~laber/vanEmdeBoas.pdf
// Textbook https://ebookcentral.proquest.com/lib/ucr/reader.action?docID=3339142

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
-cut off around 200 milliseconds by changing the calcualtions of returnHighKey and returnLowKey to use bitwise operators over division and modulus.
*/

#include <math.h>
#include <stdlib.h>
#include <iostream>


int returnLowerSquareRoot(int u) {
	int lower = log2(u * 2) / 2;
	return 1 << lower;
}
int returnLowerSquareRootBits(int u) {
	int lower = log2(u * 2) / 2;
	return lower;
}

int returnHighKey(int x, int u) { return x >> returnLowerSquareRootBits(u); }
int returnLowKey(int x, int u) { return x & (returnLowerSquareRoot(u) - 1); }
int returnIndex(int x, int u, int y) { 
	if (y == -1)
		return -1;
	return x * returnLowerSquareRoot(u) + y; 
}


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

	bool safeInsert(int numToInsert) {
		if (valueInTree(numToInsert) == false)
			return insert(numToInsert);
		return false;
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
				int lowerSquareRoot = returnLowerSquareRoot(u);
				cluster = new vebTree[lowerSquareRoot];
				for (int i = 0; i < lowerSquareRoot; i++) {
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

		if (u == 2)
			if (numToFindSuccessorOf == 0 && max == 1)
				return 1;
			else
				return -1;
		else if (min != -1 && numToFindSuccessorOf < min)
			return min;
		else {
			if (cluster == nullptr)
				return -1;
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

	int findPrevious(int numToFindPreviousOf) {
		if (u == 2)
			if (numToFindPreviousOf == 1 && min == 0)
				return 0;
			else
				return -1;
		else if (max != -1 && numToFindPreviousOf > max)
			return max;
		else {
			if (cluster == nullptr)
				return -1;
			int highKey = returnHighKey(numToFindPreviousOf, u);
			int lowKey = returnLowKey(numToFindPreviousOf, u);
			int minOfCluster = cluster[highKey].min;

			if (minOfCluster != -1 && lowKey > minOfCluster) {
				return returnIndex(highKey, u, cluster[highKey].findPrevious(lowKey));
			}
			else {
				int previousCluster = 0;

				previousCluster = summary->findPrevious(highKey);
				if (previousCluster == -1) {
					if (min != -1 && numToFindPreviousOf > min)
						return min;
					else
						return -1;
				}
				else {
					return returnIndex(previousCluster, u, cluster[previousCluster].max);
				}
			}
		}
	}

	void safeDelete(int numToFind) {

		if (valueInTree(numToFind) == true)
			deleteValueInTree(numToFind);
	}

	bool deleteValueInTree(int numToFind) {

		bool foundKey = false;
		if (min == max) {
			min = -1;
			max = -1;
			return true;
		}
		else if (u == 2) {
			if (numToFind == 0)
				min = 1;
			else
				min = 0;
			max = min;
			return true;
		}
		else {
			if (numToFind == min) {
				numToFind = returnIndex(summary->min, u, cluster[summary->min].min);
				min = numToFind;
			}
			int highKey = returnHighKey(numToFind, u);
			int lowKey = returnLowKey(numToFind, u);

			foundKey = cluster[highKey].deleteValueInTree(lowKey);
			if (cluster[highKey].min == -1) {
				summary->deleteValueInTree(highKey);
				if (numToFind == max) {
					int summaryMax = summary->max;

					if (summaryMax == -1)
						max = min;
					else
						max = returnIndex(summaryMax, u, cluster[summaryMax].max);
				}
			}
			else if (numToFind == max) {
				int highKey = returnHighKey(numToFind, u);
				max = returnIndex(highKey, u, cluster[highKey].max);
			}
			return foundKey;
		}
	}
};
