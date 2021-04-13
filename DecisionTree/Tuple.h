#ifndef TUPLE_H
#define TUPLE_H

#include "Attribute.h"
#include "Attribute.cpp"

namespace dt
{
	///<summary> <c>Tuple</c> is a template class used to create a data tuple of type T, which contains all attribute values with their associated names
	///and the user label</summary>
	template<typename T>
	class Tuple
	{
	public:
		//Default Constructor
		Tuple<T>();

		//Constructor
		Tuple<T>(const std::vector<T>& attrValues, const std::vector<T>& attrName, const std::string& label);

		//Destructor
		~Tuple() { };

		//Member Functions
		static int getTupleCount();

		std::unordered_map<std::string, T> tuple;
		std::string answer;

	private:
		//Member Functions
		//Static Member Variables
		static int tupleCount;
	};
}
#endif