#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include<list>

namespace dt
{
	///<summary> <c>TreeNode</c> is a template class that holds a T* to the rule and next attribute, and a <c>vector</c> of <c>TreeNode*</c> of type T
	///</summary>
	template<typename T>
	class TreeNode
	{
	public:
		//Default Constructor
		TreeNode<T>()
		{
			rule = nullptr;
			nextAttribute = nullptr;
		}

		//Constructors
		TreeNode<T>(const T& inputRule, const T& nextAttr)
		{
			*rule = inputRule;
			*nextAttribute = nextAttr;
		}

		//Destructor
		~TreeNode() { }

		//Member Functions

		void addChild(TreeNode<T>*& child)
		{
			children.push_back(child);
		}

		//Member Variables
		T* rule;
		T* nextAttribute;
		std::vector<TreeNode<T>*> children; //Stores Node pointers of each child in a list
	};
}
#endif