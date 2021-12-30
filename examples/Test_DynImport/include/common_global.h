#pragma once

#ifdef BUILD_DYNAMIC
#    if defined(Export_Library)
#        define DLL_API __declspec(dllexport)
#    else
#        define DLL_API __declspec(dllimport)
#    endif
#else
#    define DLL_API
#endif