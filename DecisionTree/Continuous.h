#ifndef CONTINUOUS_H
#define CONTINUOUS_H

#include <algorithm>
#include "Tuple.h"

//This file contains the functions used to calculate the continuous attribute's values

std::vector<std::string> split(const std::string& attribute) //splits the string into a vector of words
{
	std::vector<std::string> attr;

	std::string word = "";
	char prev = '\0';

	for (auto ch : attribute) //for each character in the string
	{
		if (prev != ' ' && ch == ' ')
		{
			attr.push_back(word); //adds word to the vector
			word = ""; //resets to an empty string
			prev = ch;
		}
		else
		{
			if (ch != ' ')
				word += ch; //add charachter to word
			prev = ch;
		}
	}

	if (word != "")
		attr.push_back(word); //adds last word to vector if not accounted for

	return attr; //returns the vector of words
}

double expectedInfo(std::list<dt::Tuple<std::string>>& tupleList, dt::Attribute<std::string>& tupleClasses)
{
	std::list<dt::Tuple<std::string>>::iterator it = tupleList.begin();
	double sum = 0;

	for (unsigned int i = 0; i <= tupleClasses.values.size(); i++)
	{
		int classCount = 0;

		for (unsigned int j = 0; j < tupleList.size(); j++)
		{
			std::advance(it, j); //set iterator to jth tuple in the list
			auto& tup = *it;

			auto classVals = tupleClasses.getValues(); //set to the class value vector

			if (tup.answer == classVals[i]) //if the tuple class value = the tuple's answer val then increment
				classCount++;

			it = tupleList.begin(); //reset iterator
		}
		if (classCount != 0)
			sum += -1 * log2((static_cast<double>(classCount) / static_cast<double>(tupleList.size())))* static_cast<double>(classCount) / static_cast<double>(tupleList.size());
	}

	return sum;
}

double expectedInfoWithPartition(std::vector<dt::Tuple<std::string>>& fullTupleList, std::vector<std::list<dt::Tuple<std::string>>>& tuplePartitions,
	dt::Attribute<std::string>& tupleClasses)
{
	double sum = 0;
	for (unsigned int n = 0; n < tuplePartitions.size(); n++)
	{
		sum += static_cast<double>(tuplePartitions[n].size()) / static_cast<double>(fullTupleList.size())* expectedInfo(tuplePartitions[n], tupleClasses);
	}

	return sum;
}

void determineContVals(std::vector<dt::Tuple<std::string>>& t, std::vector<int>& contIndexes,
	const std::vector<std::string>& attrNames, dt::Attribute<std::string>& classifer, std::vector<dt::Attribute<std::string>>& attributes)
{
	std::list<std::vector<double>> vals; //holds all continuous values
	std::vector<double> contVals; //holds the continuous values at a specific continuous index

	for (unsigned int z = 0; z < contIndexes.size(); z++/*it; it != contIndexes.end(); it++*/) //check every continuous index in the tuple
	{
		for (unsigned int i = 0; i < t.size(); i++)
		{
			for (unsigned int j = 0; j < t[i].tuple.size(); j++)
			{
				if (j == contIndexes[z]) //when reaching a continuous index each tuple add to vector
				{
					double cVal = stod(t[i].tuple.at(attrNames[z]));
					cVal = (int)(cVal * 10 + 0.5);
					cVal = cVal / 10;
					contVals.push_back(cVal);
				}
			}
		}
		std::sort(contVals.begin(), contVals.end()); //sort the continuous values
		vals.push_back(contVals);
	}

	std::list<std::vector<double>>::iterator vIt = vals.begin();

	for (auto element : attrNames)
	{
		double bestSplit;
		auto& valList = *vIt;
		bestSplit = (valList[0] + valList[1]) / 2.0;
		double minExpectedInfo = std::numeric_limits<double>::max();

		for (unsigned int i = 0; i < t.size() - 1; ++i)
		{
			double splitPoint = (valList[0] + valList[i + 1]) / 2.0;
			std::vector<std::list<dt::Tuple<std::string>>>* subList = new std::vector<std::list<dt::Tuple<std::string>>>();
			subList->reserve(2);
			subList->push_back(std::list<dt::Tuple<std::string>>());
			subList->push_back(std::list<dt::Tuple<std::string>>());

			for (dt::Tuple<std::string> tup : t)
			{
				auto num = stod(tup.tuple.at(element));
				if (num <= splitPoint)
					subList->at(0).push_back(tup);
				else
					subList->at(1).push_back(tup);
			}

			double expectedInfo = expectedInfoWithPartition(t, *subList, classifer);

			if (expectedInfo < minExpectedInfo)
			{
				minExpectedInfo = expectedInfo;
				bestSplit = splitPoint;
			}

			delete subList;
		}

		for (unsigned int i = 0; i < t.size(); ++i)
		{
			if (stod(t[i].tuple[element]) <= bestSplit)
				t[i].tuple[element] = " <= " + std::to_string((float)bestSplit);
			else
				t[i].tuple[element] = " > " + std::to_string((float)bestSplit);
		}

		int contAttributeIndex = -1;

		for (unsigned int i = 0; i < attributes.size(); ++i)
		{
			auto x = (attributes[i].extract_keys(attributes[i].values));
			if (x[0] == element)
			{
				contAttributeIndex = i;
				break;
			}
		}

		std::vector<std::string> newVals;
		newVals.reserve(2);

		if (bestSplit != 0)
		{
			newVals.push_back(" <= " + std::to_string(bestSplit));
			newVals.push_back(" > " + std::to_string(bestSplit));
		}
		else
		{
			newVals.push_back(" <= 0.000000");
			newVals.push_back(" > 0.000000");
		}
		attributes[contAttributeIndex].values[element] = newVals;
	}
}
#endif
