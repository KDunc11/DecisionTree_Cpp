#include "Tuple.h"

//Static Variable Initialization
template<typename T>
int dt::Tuple<T>::tupleCount = 0;

//Default Constructor
template<typename T>
dt::Tuple<T>::Tuple()
{
	tuple = NULL;
	tupleCount++;
}

//Constructor
template<typename T>
dt::Tuple<T>::Tuple(const std::vector<T>& attrValues, const std::vector<T>& attrNames, const std::string& label)
{
	for (unsigned int i = 0; i < attrNames.size(); ++i)
        tuple.insert(make_pair(attrNames[i], attrValues[i]));

    answer = label;
	tupleCount++;
}

//Member Functions
template<typename T>
int dt::Tuple<T>::getTupleCount()
{
	return tupleCount;
}