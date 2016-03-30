/************************************************************************
This sample demonstrates reading values from a lua table
This sample uses the script executer class, and the table class
************************************************************************/

#include <lw.h>
#include <iostream>

int main(int argc, char** argv)
{
	//First, we need to create a state to keep our script. 
	//It is recommended to have only one state accross the program.
	lw::cState state;

	//Lets open our state:
	state.open();

	//Create an executer so we can load and run a script
	lw::cExecuter executer(state);

	//Run the script
	executer.loadAndRunScript("script.lua");

	//Create a table reader which reads the lua table "MyTable"
	lw::cTable table(state, "MyTable");
	
	//Read the fields value1 and value2, and output the result to the console
	std::cout << "value1 = " << table.readFieldString("value1") << "\nvalue2 = " << table.readFieldInt("value2") << std::endl;

	//Note: You can nest tables, and fetch the result using table.readTable(<nested table name>). You can nest as many as you want,
	//but the more you nest, the slower it runs. You can also fetch all values of a spesific type inside a table using table.readAllStrings/Ints/etc

	//We're done, destroy the state:
	state.destroy();


	return 0;
}
