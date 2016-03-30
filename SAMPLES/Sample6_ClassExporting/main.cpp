/************************************************************************
This sample demonstrates exporting a class to lua.  
This sample uses the script executer class, and the class exporter
************************************************************************/

#include <lw.h>
#include <iostream>

//This is the class we'll be exporting
class MyClass
{
public:
	//We need to have a function that returns the name of the class as it will be known by lua
	static std::string className(){return "MyClass";}

	//A static function that registers the class with a state.
	//This does not need to be part of the class, but its cleaner this way IMO..
	static void registerClass(lw::cState& state)
	{
		lw::cLWClassRegister<MyClass> reg(state);
		reg.addFunction("helloWorld", &MyClass::helloWorld);
		reg.doRegistration();
	}

	//An exported function
	//All export functions needs this signature.
	//This is unavoidable without heavy use of dirty meta programming, 
	//which would complicate things by magnitudes, and slow down compile times :(
	int helloWorld(lua_State* L)
	{
		//Print hello world
		std::cout << "Hello World From MyClass:helloWorld!" << std::endl;
		return 0;
	}
protected:
private:
};

int main(int argc, char** argv)
{
	//First, we need to create a state to keep our script. 
	//It is recommended to have only one state across the program.
	lw::cState state;

	//Lets open our state:
	state.open();

	//Register the class:
	MyClass::registerClass(state);

	//Create an executer so we can load and run a script
	lw::cExecuter executer(state);

	//Run the script
	executer.loadAndRunScript("script.lua");

	//Note: The executer accepts exported classes as a function parameter (executer.push(<class instance>)
	//This way, you can instance classes in C++, and pass them to lua.

	//We're done, destroy the state:
	state.destroy();


	return 0;
}
