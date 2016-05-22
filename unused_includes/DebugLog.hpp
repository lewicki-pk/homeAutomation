#include <iostream>

#ifdef DEBUG 
#define DEBUG_LOG(x) (std::cout << "[" << __PRETTY_FUNCTION__ << ":" << __LINE__ << "]" << (x) << std::endl)
#define D(x) x
#else 
#define DEBUG_LOG(x)
#define D(x)
#endif
