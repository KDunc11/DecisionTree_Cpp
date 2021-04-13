#ifndef TREE_H
#define TREE_H

#include<iostream>
#include <math.h>
#include <algorithm>
#include <limits>
#include "TreeNode.h"
#include "TreeNode.cpp"
#include "Tuple.h"
#include "Tuple.cpp"

namespace dt
{
	///<summary> <c>Answer</c> is a template struct of type T used to hold the user T label values and the total number of times each user label appears 
	///in the training data </summary>
	template<typename T>
	struct Answer
	{
		T answerVal;
		int answerCount = 0;
	};

	///<summary> <c>Tree</c> is a template class of type T that contains a root <c>TreeNode</c>, information about the decision tree,
	///functions to build the decision tree, and some utility functions </summary>
	template<typename T>
	class Tree
	{
	public:
		//Constructor
		Tree(std::vector<Tuple<T>>& tuples, const std::vector<T>& ans, std::vector<std::string>& attributeNames,
			std::vector<Attribute<T>>& attributes);

		//Destructor
		~Tree() { };

		//Member Functions
		void print();

	private:
		//Private Member Functions
		void printDFS(TreeNode<T>* node, int tabs = 0); //preforms a depth first search to recursively print the decision tree
		bool continuous(std::string str); //checks a string representation of a leaf node value to determine if the value was continuous or not

		///Functions to preform calculations
		void calcTotalProb(const std::vector<Tuple<T>>& data, const std::vector<T>& ans); //Calculates the total probability of each possible answer
		void calcInfoGain(std::vector<Attribute<T>>& attributes); //Calculates the information gain of each attribute
		///

		///Functions used to build the tree
		double entropy(int& pos, int& neg, int& total);
		std::vector<double> condEntropy(const std::vector<std::list<double>>& prob); //Calculates the conditional entropy
		double dataSetEntropy(const std::vector<double>& totalProb); //Calculates the entropy of the entire data set
		void split(TreeNode<T>* currentNode, std::vector<Tuple<T>>& tuples, const std::vector<T>& ans, std::vector<std::string>& attributeNames,
			std::vector<Attribute<T>>& attributes); //recursive function that splits the tree based on the highest information gain
		///
		
		//Private Member Variables
		TreeNode<T>* root;
		std::vector<double> totalProb; //Total Probability for each answer occuring
		std::vector<Answer<T>> answers; //All possible user labels
		double dataEntropy; //stores the info gain of the entire data set, uses the total probabilities
		unsigned int totalTuples; //holds the total number of tuples
	};
}
#endif