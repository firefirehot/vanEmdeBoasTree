## vanEmdeBoasTree
# Summary(Wikipedia):
A van Emde Boas tree is a tree data structure which implements an associative array with m-bit integer keys. It performs all operations in O(log m) time, 
or equivalently in O(log log M) time, where M = 2^m is the maximum number of elements that can be stored in the tree. 
The M is not to be confused with the actual number of elements stored in the tree, by which the performance of other tree data-structures is often measured. 

# Members:

u: universe of the tree.

min: minimum value inside of the tree

max: maximum value inside of the tree

cluster: a pointer to an array of vanEmdeBoasTrees. Initially set to nullptr. When insert() function is used the function will make cluster point to a new vanEmdeBoasTree array with u of sqrt(u)

summary: a pointer to a vanEmdeBoasTree. Initially set to nullptr. When insert() function is used the function will make summary point to a new vanEmdeBoasTree with u of sqrt(u)

# Constructors:
vebTree(): No input. u must be set after this constructor is called, otherwise the other functions will not work.

vebTree(int passedU): Sets u when constructor is called.

vebTree(int passedU, int passedMax): Sets u when constructor is called and it inserts the first value into the tree.


# Unique Functions:
runVEBTreeFullTest(int maxNumber): Runs a test of insert, delete, min, max, and value in tree functions by creating three files of random numbers that range from 0 to maxNumber.

safeDelete vs deleteValueInTree: safeDelete checks if the number is in the tree before deleting.

safeInsert vs insert: safeInsert checks if the number is in the tree before inserting.
