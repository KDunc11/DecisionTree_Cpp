#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>

namespace dt
{
	///<summary> <c>Attribute</c> is a template class used to create an attribute with a name and vector of T values, as well as other information 
	///pertaining to the attribute</summary>
	template<typename T>
	class Attribute
	{
	public:
		//Constructor
		Attribute<T>(const std::string& labelName, const std::vector<T>& inputData);

		//Destructor
		~Attribute() { };

		//Member Functions

		///Modifier Functions
		void setEntropy(const double& num);
		void setCondEntropy(const std::vector<double>& nums);
		void setProb(const double& num);
		void setCondProb(const std::vector<std::list<double>>& num);
		void setInfoGain(const double& num);
		///

		///Accessor Functions
		static int getAttributeCount(); // returns the attribute count
		std::string getAttributeName(); //gets the name of the attribute stored in the std::map
		std::vector<T> getValues(); //gets the values stored in the std::map
		std::vector<double> getProb()const; //returns the probability
		std::vector<std::list<double>> getCondProb()const; //returns the conditional probability at the specified index if in bounds
		std::vector<double> getCondEntropy()const; //returns the conditional entropy
		double getInfoGain()const; //returns the info gain of the attribute
		std::vector<std::string> extract_keys(std::unordered_map<std::string, std::vector<T>> const& input_map); //returns the label names
		std::vector<T> extract_values(std::unordered_map<std::string, std::vector<T>> const& input_map); //returns all values
		///

		//Member Variables
		std::unordered_map<std::string, std::vector<T>> values; //Stores the Attribute label and the possible values 

	private:
		//Private Member Variables
		double entropy;
		std::vector<double>* conditionalEntropy;
		std::vector<std::list<double>>* conditionalProb;
		std::vector<double>* prob;
		double infoGain;

		//Static Member Variables
		static int attributeCount; //Keeps track of the created tuples
	};
}
#endif