#include <xmemory>

#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//constant conditional expression
#pragma warning(disable: 4127)

//0 sized array
#pragma warning(disable: 4200)

//nameless struct/union
#pragma warning(disable: 4201)

//convert from T to T&
#pragma warning(disable: 4239)

//unreferenced local function
#pragma warning(disable: 4505)

//assignment in conditional expression
#pragma warning(disable: 4706)
