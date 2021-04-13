#include "Attribute.h"

//Static Variables
template<typename T>
int dt::Attribute<T>::attributeCount = 0; //initalizes tuple count to zero

//Constructor
template<typename T>
dt::Attribute<T>::Attribute(const std::string& labelName, const std::vector<T>& inputData)
{
	values.insert(make_pair(labelName, inputData));
	entropy = 0;
	conditionalEntropy = nullptr;
	conditionalProb = nullptr;
	prob = nullptr;
	infoGain = 0;
	attributeCount++;
}

//Member Functions
//Modifier Functions
template<typename T>
void dt::Attribute<T>::setEntropy(const double& num)
{
	entropy = num;
}

template<typename T>
void dt::Attribute<T>::setCondEntropy(const std::vector<double>& nums)
{
	if (conditionalEntropy == nullptr)
	{
		conditionalEntropy = new std::vector<double>;
		*conditionalEntropy = nums;
	}
	else
		*conditionalEntropy = nums;
}

template<typename T>
void dt::Attribute<T>::setProb(const double& num)
{
	if (prob == nullptr)
	{
		prob = new std::vector<double>;
		prob->push_back(num);
	}
	else
		prob->push_back(num);
}

template<typename T>
void dt::Attribute<T>::setCondProb(const std::vector<std::list<double>>& num)
{
	if (conditionalProb == nullptr)
	{
		conditionalProb = new std::vector<std::list<double>>;
		*conditionalProb = num;
	}
	else
		*conditionalProb = num;
}

template<typename T>
void dt::Attribute<T>::setInfoGain(const double& num)
{
	infoGain = num - entropy; //Gain(x) = Entropy(DataSet) - Entropy(x) where x is a specific attribute
}

//Accessor Functions
template<typename T>
int dt::Attribute<T>::getAttributeCount()
{
	return attributeCount;
}

template<typename T>
std::string dt::Attribute<T>::getAttributeName()
{
	std::vector<std::string> retVal = extract_keys(values);

	return retVal[0]; //only one key in the std::map so reutrn the key at index 0 of the returned vector
}

template<typename T>
std::vector<T> dt::Attribute<T>::getValues()
{
	std::vector<T> retVal = extract_values(values);

	return retVal;
}

template<typename T>
std::vector<double> dt::Attribute<T>::getProb()const
{
	if (prob != nullptr)
		return *prob;
	else
	{
		std::vector<double> emptyVec;
		return emptyVec;
	}
}

template<typename T>
std::vector<std::list<double>> dt::Attribute<T>::getCondProb()const
{
	if (conditionalProb != nullptr)
		return *conditionalProb;
	else
	{
		std::vector<std::list<double>> emptyVec;
		return emptyVec;
	}
}

template<typename T>
std::vector<double> dt::Attribute<T>::getCondEntropy() const
{
	if (conditionalEntropy != nullptr)
		return *conditionalEntropy;
	else
		return std::vector<double>();
}

template<typename T>
double dt::Attribute<T>::getInfoGain()const
{
	return infoGain;
}

template<typename T>
std::vector<std::string> dt::Attribute<T>::extract_keys(std::unordered_map<std::string, std::vector<T>> const& input_map)
{
	std::vector<std::string> retval;
	for (auto const& element : input_map) {
		retval.push_back(element.first);
	}
	return retval;
}

template<typename T>
std::vector<T> dt::Attribute<T>::extract_values(std::unordered_map<std::string, std::vector<T>> const& input_map)
{
	std::vector<T> retval;
	for (auto const& element : input_map) {
		retval = (element.second);
	}
	return retval;
}