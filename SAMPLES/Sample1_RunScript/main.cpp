/************************************************************************
 This sample demonstrates loading and running a Lua script.  
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
	executer.loadAndRunScript("hello_world.lua");

	//We're done, destroy the state:
	state.destroy();
	

	return 0;
}
