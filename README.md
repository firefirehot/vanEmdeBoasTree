#Summarized Implementations:
#Variables: 
  Variables are min, max, cluster, summary, and u. Min and max hold the minimum and maximum number currently in the tree. Cluster is a pointer to the array of subtrees. Summary is a pointer to a VEB tree that acts as a data structure to maintain whether a subtree is empty or not.
	##Constructor: 
  Min and max are initially set to be equal to -1. Cluster and summary are initially set to be equal to nullptr. There are 3 different constructors that can be used to create a VEB Tree depending on what information is currently available.
  ##Lower Square Root aka returnLowerSquareRoot(): 
  Calculates the lower square root which is used to determine how large the subtrees will be. The lower square root also determines the divide between the lower bits and the higher bits.
  ##Low Key aka returnLowKey(): 
  Returns the lower bits of the passed-in number. These lower bits are used to determine the position a number will be in its cluster.
  ##High Key aka returnHighKey(): 
  Returns the upper bits of the passed-in number. These higher bits are used to determine which cluster a number will be in. I call the cluster a number might be found the “corresponding cluster.”
  ##Index aka returnIndex():
  Is used to calculate the value of a number within a tree. This is used to determine the value of the number returned from Find Successor and Find Predecessor.
  ##Insert aka insert(): 
  If the current tree is empty, set both variables to be equal to the passed value. Otherwise, if u (the universe of the tree) is greater than 2 the program checks if the passed value’s corresponding cluster is empty. If it is empty and the summary is equal to nullptr then create a space for the cluster array of size square-root u and the summary. Then set the min and max of the corresponding cluster to the passed value and recursively call insert on the summary. If the cluster is not empty recursively call Insert on the corresponding cluster. 
  ##Return Maximum aka returnMaximum():
  Return the max value stored in the tree.
  ##Return Minimum aka returnMinimum():
  Return the min value stored in the tree.
  ##Check For a Value In The Tree aka valueInTree(): 
  If the passed value is the min or max of the tree return true. Otherwise, if the current node is a leaf node return false. Recursively call the Check For a Value on the passed value’s corresponding cluster.
  ##Find Successor aka findSuccessor(): 
  If the current node is a leaf node return true or false depending on if the successor is in the current node. Otherwise, if the minimum of the current node is greater than the passed value, return the minimum of the current node. Otherwise, if the passed value is less than the maximum of its cluster then recursively call Find Successor on the cluster. Otherwise, call Find Successor on the summary to find the cluster that will have the successor and return the minimum value of that cluster.
  ##Find Predecessor aka findPrevious(): 
	If the current node is a leaf node return true or false depending on if the predecessor is in the current node. Otherwise, if the passed value is greater than the current maximum of the tree, return the current max. Otherwise, check if the minimum of the passed value’s corresponding cluster is less than the passed value. If it is then recursively call Find Predecessor on the passed value’s corresponding cluster. If not call Find Predecessor on the summary to find the cluster that will have the predecessor and return the maximum value of that cluster.
  ##Delete Value aka deleteValueInTree():
  If min is equal to max set both to -1. Otherwise, if the current node is a leaf node delete the value by setting max equal to min or min equal to max accordingly. Otherwise, if the passed value is equal to min find a number to replace min in the minimum value of the passed value’s cluster. If the passed value’s corresponding cluster is empty delete the value in the summary by recursively calling Delete Value on the summary. Otherwise, if the number to delete is equal to the maximum of the current node find a new maximum in the passed value’s corresponding cluster.

#Testing data:
  To thoroughly test the data I created 3 files of a variable number of random integers from 0 to $2^k$. Here the variable number of random integers and k are both controllable. Some k I used while testing were k = 17, 18, 19.
I used these three files to insert into a VEB Tree, check that all the integers in the file were in the tree, and deleting all the integers in the tree. I kept track of the time it took to perform all the above actions seperately.

  To test Find Successor and Finding Predecessor I made two types of tests. First I made smaller tests that I could directly see the results. Second I used both Find Successor and Find Predecessor to count up the number of unique values inside of the trees created by random integers. I then compared Find Successor and Find Predecessor against each other and I compared them against what I expected. 
For example, if I created $2^k$ integers from 0 to $2^k$ - 1, Find Successor and Find Predecessor would find about 0.63\times2k unique integers. If I created 1.4\times$2^k$ integers from 0 to 2k-1 , Find Successor and Find Predecessor would find about 0.75\times$2^k$ unique integers. These results are in accordance with the expectation.

Experiments and Analysis:
The first experiment done was to compare the Van Emde Boas Tree to std::set which is implemented as a binary tree. The results were given 217 numbers ranging from 0 to 217-1 the Van Emde Boas Tree inserted, checked, and deleted about twice as fast as std::set. This relationship held for different random files and across different universes of numbers. 
The comparison to std::set’s usefulness ends after this point due to the fact that std::set’s functions are log (n) which are fundamentally different to the Van Emde Boas Tree’s functions of log (log (u)).

  The second experiment was to create a chart mapping the average time it took to perform a check for value in the tree against the universe of numbers. I expected to see a logarithmic curve after creating a logarithmic scaling on the universe. What I got instead was a spike in cost around from $2^13$ to $2^17$, with only $2^18$ and $2^19$ following the curve I expected. Bellow I will try to explain why.
Due to the nature of the Van Emde Boas tree it is very difficult to get a substantial number of data points because the universes must be $2^k$ and my computer cannot handle more than $2^19$. Below the universe universes from $2^13$ to $2^19$, a total of 9 data points. At $2^13$ the cost per check is at its lowest, at $2^17$ the cost is at its highest, at $2^18$ it seems that the cost is leveling out. 
I believe that the graph looks the way it does because of the relationship that 
log (log (u)) has with its constant. At lower “u” the constant plays a greater role in determining the average time to perform a check for a value, peaking with $2^17$. After $2^17$, the constant plays a lesser role in revealing the log (log (u)) time. This is revealed due to the logarithmic separation of values of the graph.


