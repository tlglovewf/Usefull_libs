#pragma once

#ifdef BUILD_DYNAMIC
#    if defined(Export_Library)
#        define COMM_API __declspec(dllexport)
#    else
#        define COMM_API __declspec(dllimport)
#    endif
#else
#    define COMM_API
#endif

#define PROCMARK extern "C" _declspec(dllexport)