#ifndef FILEPROCESSING_H
#define FILEPROCESSING_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <iomanip>
#include "Continuous.h"
#include "Tree.h"
#include "Tree.cpp"

//This file contains the function used to process the input files provided via command line arguments

void openAndProcessFiles(const char* fileName)
{
	std::fstream inFile;

	inFile.open(fileName); //try to open the file

	try
	{
		if (inFile.is_open()) //if the file is opened
		{
			std::vector<std::string> lines;
			int lineCount = 0;
			std::string tempLine;
			int numOfAttributes = 0;

			getline(inFile, tempLine);
			numOfAttributes = stoi(tempLine);

			//Reads all lines from input file into a vector
			while (!inFile.eof())
			{
				getline(inFile, tempLine);

				if (tempLine != "" && tempLine != " ")
					lines.push_back(tempLine); //inserts input file lines into a vector
			}

			std::vector<dt::Attribute<std::string>> attributes; //stores all possible attributes
			std::vector<std::string> attrNames; //stores all the names of the attributes
			std::vector<std::vector<std::string>> tempAttr; //used to read in data from the file
			std::vector<std::string> continuousAttr; //stores the continuous attributes names
			std::vector<int> continuousIndexes; //stores the indexes in the tuple where the continuous values will be located

			for (int i = 0; i < numOfAttributes; i++)
			{
				tempAttr.push_back(split(lines[i])); //separates attribute label from attribute values
				lineCount++;
			}

			//Account for all possible attributes
			for (unsigned int a = 0; a < tempAttr.size(); a++)
			{
				std::string* label = new std::string(tempAttr[a][0]); //label is the first index in the temporary vector
				std::vector<std::string> values;

				for (unsigned int b = 1; b < tempAttr[a].size(); b++)
				{
					values.push_back(tempAttr[a][b]); //push the possible attribute values into the vector

					if (tempAttr[a][b] == "continuous") //checking for continuous attributes
					{
						continuousAttr.push_back(*label); //insert the label name of the continuous attribute
						continuousIndexes.push_back(a); //stores the attribute index of the continuous value
					}
				}

				attrNames.push_back(*label);
				attributes.push_back(dt::Attribute<std::string>(*label, values)); /*inserts and builds the attributes with labels
																		 and their respective values*/
				delete label;
			}

			//Read in class and possible values
			std::vector<std::string> classVals = split(lines[lineCount]);
			lineCount++;

			std::string className = classVals[0]; //stores the name of the classifer (answer)
			classVals.erase(classVals.begin());

			dt::Attribute<std::string> classifer(className, classVals); //stores the user labels or the classier

			//Read in tuples
			std::vector<dt::Tuple<std::string>> tuples;
			std::vector<std::vector<std::string>> tupVals;

			for (unsigned int k = lineCount; k < lines.size(); ++k)
				tupVals.push_back(split(lines[k]));

			for (unsigned int i = 0; i < tupVals.size(); ++i)
			{
				std::string* ansVal = new std::string(tupVals[i][tupVals[i].size() - 1]); //last value in tuple is the answer
				std::vector<std::string> values;

				for (unsigned int j = 0; j < tupVals[i].size() - 1; ++j) //remove the answer from the tuple values vector
					values.push_back(tupVals[i][j]);

				tuples.push_back(dt::Tuple<std::string>(values, attrNames, *ansVal)); //push a new tuple into the list

				delete ansVal;
			}

			//Dealing with continuous attributes
			if (!continuousIndexes.empty()) //checks if there are any continuous attributes
				determineContVals(tuples, continuousIndexes, continuousAttr, classifer, attributes); /*handles calculating continuous values
																									 for each continuous attribute*/

																									 //Build the tree
			std::vector<std::string> ans = classifer.extract_values(classifer.values); //extract all the user labels
			dt::Tree<std::string> decisionTree(tuples, ans, attrNames, attributes); //calls the tree's constructor to build out the tree
			decisionTree.print(); //print the tree out to the console
		}
		else
		{
			inFile.close(); //close the file and display error message
			throw std::runtime_error(std::string("Error: could not open ") + fileName + std::string(".\n")); //throw a runtime error
		}
		inFile.close(); //close the file
	}
	catch (std::exception e) //catches an execption
	{
		std::cout << e.what() << std::endl; //display error message
	}
}
#endif