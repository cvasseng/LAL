/************************************************************************
This sample demonstrates calling a lua function.  
This sample uses the script executer class.
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

	//Run the script
	//Note that running the script is needed to put the byte code on the stack
	executer.loadAndRunScript("script.lua");

	//Lets prepare calling the function "say" from the lua script.
	//You can also call functions inside user defined tables by passing the table name as the second parameter.
	executer.prepareCall("say");

	//Pass a parameter to the function
	//Just skip this step to call a parameter-less function.
	//You can push as many parameters as you'd like.
	executer.push(std::string("I come from C++!"));

	//Now lets preform the call
	executer.call();
	
	//We're done, destroy the state:
	state.destroy();

	return 0;
}
