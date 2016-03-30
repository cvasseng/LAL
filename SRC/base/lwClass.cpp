#include "../../INCLUDE/lwClass.h"

using namespace lw;


lw::cLWClass::cLWClass( cState &scriptState ) : m_scriptExecuter(scriptState)
{
	m_state = &scriptState;
}

lw::cLWClass::~cLWClass()
{

}
