#include "Windowing/Window.hpp"

#ifdef USE_DX12
#include "../private/src/Windowing/Variants/WindowDX.cpp"
#else
#include "../private/src/Windowing/Variants/GLUTWrapper.cpp"
#endif
