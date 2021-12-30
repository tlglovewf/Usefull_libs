#include "interface.h"


namespace MyInterface
{
#define SINGLEIMPL(N)   R_##N&  R_##N::getSingleton(){ \
							static R_##N single;  \
							return single;	  \
						}

	SINGLEIMPL(IBase);
}