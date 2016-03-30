/************************************************************************
This sample demonstrates calling a C function from lua
This sample uses the script executer class, and the variable class
************************************************************************/

#include <lw.h>

int main(int argc, char** argv)
{
	//First, we need to create a state to keep our script. 
	//It is recommended to have only one state accross the program.
	lw::cState state;

	//Lets open our state:
	state.open();

	//Create an executer so we can load and run a script
	lw::cExecuter executer(state);

	//Create a varialbe so we can read/write to global vars
	lw::cVariable variable(state);

	//Write a value to the lua variable "var1"
	variable.writeString("var1", "Hello World! I was set in C++!");

	//Run the script
	//Note that running the script is needed to put the byte code on the stack
	executer.loadAndRunScript("script.lua");

	//Read a value from the lua variable "var2" and print it to the console
	std::cout << variable.readString("var2") << std::endl;

	//We're done, destroy the state:
	state.destroy();

	return 0;
}
