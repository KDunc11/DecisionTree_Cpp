#include "Tree.h"
#include "Attribute.h"

//Constructor
template<typename T>
dt::Tree<T>::Tree(std::vector<Tuple<T>>& tuples, const std::vector<T>& ans, std::vector<std::string>& attributeNames,
	 std::vector<Attribute<T>>& attributes)
{
	root = new TreeNode<T>;

	//calculate the total probabilities
	calcTotalProb(tuples, ans);

	dataEntropy = dataSetEntropy(totalProb);

	split(root, tuples, ans, attributeNames, attributes);
}

//Public Member Functions
template<typename T>
void dt::Tree<T>::print()
{
	printDFS(root); //start printing the tree from the root
}

//Private Member Functions
template<typename T>
void dt::Tree<T>::printDFS(TreeNode<T>* node, int tabs)
{
	if (node != nullptr)
	{
		std::vector<TreeNode<T>*> childVec = node->children;

		if (childVec.size() != 0)
		{
			for (unsigned int i = 0; i < childVec.size(); ++i)
			{
				if (tabs == 0)
				{
					if (continuous(static_cast<std::string>(*childVec[i]->rule)))
						std::cout << *node->nextAttribute << " " << *childVec[i]->rule << ":" << std::endl;
					else
						std::cout << *node->nextAttribute << " = " << *childVec[i]->rule << ":" << std::endl;
				}
				else
				{
					for (int j = 0; j < tabs; ++j)
						std::cout << "\t";

					if (node->nextAttribute != nullptr)
					{
						if (continuous(static_cast<std::string>(*childVec[i]->rule)))
							std::cout << *node->nextAttribute << " " << *childVec[i]->rule << std::endl;
						else
							std::cout << *node->nextAttribute << " = " << *childVec[i]->rule << std::endl;
					}	
					else
						std::cout << *childVec[i]->rule << std::endl;
				}

				printDFS(/*ostrm,*/ childVec[i], ++tabs);
				tabs--;
			}
		}

		else //there are no children so a the current node is a leaf node
		{
			for (int i = 0; i < tabs; ++i)
				std::cout << "\t";

			std::cout << *node->nextAttribute << std::endl;
		}
	}
	else
	{
		throw std::runtime_error("Error: the tree was empty, cannot print an empty tree.");
	}
}

template<typename T>
void dt::Tree<T>::calcTotalProb(const std::vector<Tuple<T>>& tuples, const std::vector<T>& ans)
{
	//iterate through the vector of user labels and create an Answer instance for each label
	for (unsigned int i = 0; i < ans.size(); ++i)
	{
		Answer<T>* insert = new Answer<T>();
		insert->answerVal = ans[i];
		answers.push_back(*insert);

		delete insert;
	}

	int total = 0;

	//get total probabilities
	for (unsigned int i = 0; i < tuples.size(); ++i) //iterate through all the tuples
	{
		for (unsigned int j = 0; j < answers.size(); ++j) //iterate through each possible answer
		{
			if (tuples[i].answer == answers[j].answerVal)
			{
				answers[j].answerCount++; //increment the count
				total++;
			}
		}
	}

	//calculate the total probabilities
	for (unsigned int i = 0; i < answers.size(); ++i)
	{
		double tProb = answers[i].answerCount / static_cast<double>(total); /*divides the # of answers for a unique answer by the 
																			total number of tuples to obtain the probability */
		totalProb.push_back(tProb);
	}
}

template<typename T>
void dt::Tree<T>::calcInfoGain(std::vector<Attribute<T>>& attributes)
{
	for (unsigned int i = 0; i < attributes.size(); ++i)
		attributes[i].setInfoGain(dataEntropy); //uses the data sets entropy tp determine the info gain of each attribute
}

template<typename T>
double dt::Tree<T>::entropy(int& pos, int& neg, int& total)
{
	if (pos == neg)
		return 1.0;
	else if (pos == 0 || neg == 0)
		return 0;
	else
	{
		double pVal = static_cast<double>(pos) / static_cast<double>(total);
		double nVal = static_cast<double>(neg) / static_cast<double>(total);
		double sum = (-1 * log2(pVal) * pVal) + (-1 * log2(nVal) * nVal);
		return sum;
	}
}

template<typename T>
std::vector<double> dt::Tree<T>::condEntropy(const std::vector<std::list<double>>& prob)
{
	std::vector<double> sums;
	

	for (unsigned int i = 0; i < prob.size(); ++i)
	{
		double sum = 0.0;
		std::list<double>::const_iterator it;

		for (it = prob[i].begin(); it != prob[i].end(); it++)
		{
			if (*it != 0)
				sum += -1 * log2(*it) * (*it);
		}
		sums.push_back(sum);
	}
	return sums;
}

template<typename T>
double dt::Tree<T>::dataSetEntropy(const std::vector<double>& totalProb)
{
	double sum = 0.0;

	for (unsigned int i = 0; i < totalProb.size(); ++i)
	{
		if (totalProb[i] != 0)
			sum += -1 * log2(totalProb[i]) * totalProb[i];
	}

	return sum;
}

template<typename T>
bool dt::Tree<T>::continuous(std::string str)
{
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		if (ch == '>' || ch == '<')
			return true;
	}

	return false;
}

template<typename T>
void dt::Tree<T>::split(TreeNode<T>* currentNode, std::vector<Tuple<T>>& tuples, const std::vector<T>& ans, std::vector<std::string>& attributeNames,
	std::vector<Attribute<T>>& attributes)
{
	if (tuples.size() != 0)
	{
		for (unsigned int i = 0; i < attributes.size(); ++i) //iterate through all of the attributes to find calculate the entropy
		{
			std::vector<T> attrVals = attributes[i].getValues();
			std::vector<Tuple<T>> subList;
			int attributeCount = 0;
			int tupleCount = 0;

			typename std::unordered_map<std::string, T>::iterator it = tuples[0].tuple.begin();

			for (it; it != tuples[0].tuple.end(); ++it) //iterate till the appropriate attribute name is found
			{
				if (it->first == attributeNames[i])
					break;
			}


			for (unsigned int j = 0; j < tuples.size(); ++j)
			{
				if (it->first == attributeNames[i])
				{
					subList.push_back(tuples[j]);
					attributeCount++;
				}
				tupleCount++;
			}

			std::vector<double> conditionalEntropy;
			std::vector<int> attrValProbability;

			for (unsigned int j = 0; j < attrVals.size(); ++j)
			{
				int attributeValueCount = 0;
				std::vector<Tuple<T>> attributeValueSubList;

				for (unsigned int k = 0; k < subList.size(); ++k)
				{
					typename std::unordered_map<std::string, T>::iterator subIt = subList[k].tuple.begin();

					for (subIt; subIt != subList[k].tuple.end(); ++subIt)
					{
						if (subIt->first == attributeNames[i])
							break;
					}

					if (subIt->second == attrVals[j])
					{
						attributeValueSubList.push_back(subList[k]);
						attributeValueCount++;
					}
				}

				int posCount = 0;
				int negCount = 0;

				for (unsigned int n = 0; n < attributeValueSubList.size(); ++n)
				{
					if (attributeValueSubList[n].answer == ans[0])
						posCount++;
					else
						negCount++;
				}

				double attrValueEntropy = entropy(posCount, negCount, attributeValueCount);
				attrValProbability.push_back(attributeValueCount);
				conditionalEntropy.push_back(attrValueEntropy);
			}
			attributes[i].setCondEntropy(conditionalEntropy);
			double attributeEntropy = 0;

			for (unsigned int j = 0; j < conditionalEntropy.size() && j < attrValProbability.size(); ++j)
			{
				double attrValProb = static_cast<double>(attrValProbability[j]);
				double attrCount = static_cast<double>(attributeCount);
				double attrProb = attrValProb / attrCount;
				double attrEntropy = conditionalEntropy[j];
				attributeEntropy += (attrProb * attrEntropy);
			}
			attributes[i].setEntropy(attributeEntropy);
		}

		calcInfoGain(attributes);

		std::string bestAttribute = "";
		int bestAttributeIndex = -1;
		double maxInfoGain = -1.0;
		bool duplicate_flag = false;

		for (unsigned int i = 0; i < attributes.size(); ++i) //finding the attribute with the highest information gain
		{
			double infoGain = attributes[i].getInfoGain();

			if (infoGain > maxInfoGain&& infoGain != dataEntropy)
			{
				maxInfoGain = infoGain;
				bestAttribute = attributeNames[i];
				bestAttributeIndex = i;
			}
			else if (infoGain == maxInfoGain)
			{
				if (!duplicate_flag)
				{
					std::cout << "** duplicate information gain **" << std::endl;
					duplicate_flag = true;
				}
			}
			else if (infoGain == dataEntropy) // no information gain
			{
				bestAttribute = attributeNames[i];
				bestAttributeIndex = i;
				break;
			}
		}

		if (attributes.size() - 1 != 0 && attributes[bestAttributeIndex].getInfoGain() != dataEntropy)
		{
			currentNode->nextAttribute = new T(bestAttribute);

			std::vector<T> attrVals = attributes[bestAttributeIndex].getValues();

			for (unsigned int i = 0; i < attrVals.size(); ++i) //add the attribute values to the children
			{
				TreeNode<T>* newNode = new TreeNode<T>;
				newNode->rule = new T(attrVals[i]);
				currentNode->addChild(newNode);
			}

			std::vector<std::vector<Tuple<T>>> subList;

			for (unsigned int i = 0; i < attrVals.size(); ++i)
			{
				std::vector<Tuple<T>> attrValSubList;

				for (unsigned int j = 0; j < tuples.size(); ++j) //add sublists of tuples that equal each attribute value
				{
					typename std::unordered_map<std::string, T>::iterator it;

					for (it = tuples[j].tuple.begin(); it != tuples[j].tuple.end(); ++it)
					{
						if (it->first == bestAttribute && it->second == attrVals[i])
							attrValSubList.push_back(tuples[j]);
					}
				}
				subList.push_back(attrValSubList);
			}

			for (unsigned int i = 0; i < subList.size(); ++i) //remove the attribute already used from the subset of tuples
			{
				for (unsigned int j = 0; j < subList[i].size(); ++j)
				{
					typename std::unordered_map<std::string, T>::iterator it;

					for (it = subList[i][j].tuple.begin(); it != subList[i][j].tuple.end(); ++it)
					{
						if (it->first == bestAttribute)
						{
							subList[i][j].tuple.erase(it);
							break;
						}
					}
				}
			}

			std::vector<Attribute<T>> newAttrVec = attributes;
			std::vector<std::string> newAttrNameVec = attributeNames;

			for (unsigned int i = 0; i < newAttrVec.size() && newAttrNameVec.size(); ++i) //remove the attribute from the vector of attributes left
			{
				if (i == bestAttributeIndex)
				{
					newAttrVec.erase(newAttrVec.begin() + i);
					newAttrNameVec.erase(newAttrNameVec.begin() + i);
					break;
				}
			}

			std::vector<TreeNode<T>*> childVec = currentNode->children;

			for (unsigned int i = 0; i < childVec.size() && i < subList.size(); ++i)
				split(childVec[i], subList[i], ans, newAttrNameVec, newAttrVec);
		}
		else
		{
			std::vector<T> attrVals = attributes[bestAttributeIndex].getValues();

			int answerCount = 0;
			std::string answer = ans[0];

			for (unsigned int i = 0; i < tuples.size(); ++i)
			{
				if (answerCount == 0 && tuples[i].answer != answer) //try the other user label
					answer = ans[1];
				if (answer == tuples[i].answer)
					answerCount++;
			}

			if (answerCount != tuples.size()) // if not all the attribute values gave the same user label
			{
				for (unsigned int i = 0; i < attrVals.size(); ++i)
				{
					for (unsigned int j = 0; j < tuples.size(); ++j)
					{
						if (tuples[j].tuple[bestAttribute] == attrVals[i])
						{
							currentNode->nextAttribute = new T(bestAttribute);
							TreeNode<T>* leafNode = new TreeNode<T>;
							leafNode->rule = new T(attrVals[i]);
							leafNode->nextAttribute = new T(tuples[j].answer);
							currentNode->addChild(leafNode);
							break;
						}
					}
				}
			}
			else
				currentNode->nextAttribute = new T(answer);
		}
	}
	else
		currentNode->nextAttribute = new T("Not enough data");
}