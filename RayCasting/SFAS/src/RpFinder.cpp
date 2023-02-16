#include "../RpFinder.hpp"
using namespace sfas;

std::string _ResPath_ = "";

std::string sfas::rp()
{
    
#ifdef _WIN32
    return _ResPath_;
#endif
    
#ifdef __APPLE__
    return applerp__::resourcePath() + _ResPath_;
#endif
}

void sfas::setResPath(std::string nP)
{
    _ResPath_ = nP;
}
