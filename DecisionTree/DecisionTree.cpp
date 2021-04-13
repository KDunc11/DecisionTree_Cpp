//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// Name: Kyle Duncan                                                            //
//                                                                              //
// Assignment: Decision Tree                                                    //
//                                                                              //
// Description: This program uses command line arguments to read in training    //
// data from files and then extract data from the input files to create tuples. //
// The tuples are then processed to form a decision tree based off of each      //
// attribute in the tuples and their corresponding information gain.            //
//                                                                              //                                                                           //
//////////////////////////////////////////////////////////////////////////////////

#include "FileProcessing.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 3 /*|| argc > sizeof(argv)*/)
		{
			throw std::runtime_error("Error: Invalid number of arguments.\n");
		}
		else
		{

			for (unsigned int i = 2; i < argc; ++i) //run all provided training files through the decision tree
			{
				std::cout << "Creating tree for " << argv[i] << std::endl << std::endl;
				openAndProcessFiles(argv[i]);
				std::cout << std::endl << std::endl;
			}
			std::cout << "Done creating trees from input files, press enter to end program.";
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.ignore();
    return 0;
}

