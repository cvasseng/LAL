#include "base/lwState.h"
#include "base/lwExecuter.h"
#include "base/lwClass.h"
#ifndef _LIB


#include "base/lwClassRegister.h"
#include "base/lwVariable.h"
#include "base/lwTable.h"

#include <string>

lw::cState state;
lw::cExecuter script(state);

class TestClass : public lw::cLWClass
{
public:
	TestClass(lw::cState &state) : lw::cLWClass(state) 
	{

	}

	static std::string className()
	{
		return "TestClass";
	}
protected:
private:
};

int main(int argc, char** argv)
{
	state.open();
	lw::cLWClassRegister<TestClass> test(state);
	test.doRegistration("TestClass");

	script.loadAndRunScript("test.lua");

	lw::cTable test2(state, "level");
	std::cout << test2.readFieldString("name") << std::endl;

	state.destroy();

	return 0;
}

#endif