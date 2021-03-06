/* 2021.12.30 by tuligen*/
#pragma once
#include <comm_global.h>
#include <windows.h>

#include <string>


namespace CommUtils {

    /** DynamicLibrary - encapsulates the loading and unloading of dynamic libraries,
        typically used for loading ReaderWriter plug-ins.
    */
    class COMM_API DynamicLibrary
    {
    public:

        typedef void* HANDLE;
        typedef void* PROC_ADDRESS;

        /** returns a pointer to a DynamicLibrary object on successfully
          * opening of library returns NULL on failure.
          */
        static DynamicLibrary* loadLibrary(const std::string& libraryName);

        /** return name of library stripped of path.*/
        const std::string& getName() const { return _name; }

        /** return name of library including full path to it.*/
        const std::string& getFullName() const { return _fullName; }

        /** return handle to .dso/.dll dynamic library itself.*/
        HANDLE             getHandle() const { return _handle; }

        /** return address of function located in library.*/
        PROC_ADDRESS       getProcAddress(const std::string& procName);

        ~DynamicLibrary();
    protected:

        /** get handle to library file */
        static HANDLE getLibraryHandle(const std::string& libraryName);

        /** disallow copy constructor.*/
        DynamicLibrary(const DynamicLibrary&) = delete;
        /** disallow copy operator.*/
        DynamicLibrary& operator = (const DynamicLibrary&) = delete;

        /** Disallow public construction so that users have to go
          * through loadLibrary() above which returns NULL on
          * failure, a valid DynamicLibrary object on success.
          */
        DynamicLibrary(const std::string& name, HANDLE handle);
        

        HANDLE          _handle;
        std::string     _name;
        std::string     _fullName;

    };

}