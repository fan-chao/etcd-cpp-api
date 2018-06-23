
#ifndef ETCD_MACROS_H
#define ETCD_MACROS_H

// If cppkafka was built into a shared library
#if defined(_WIN32) && !defined(CPPKAFKA_STATIC)
    // Export/import symbols, depending on whether we're compiling or consuming the lib
    #ifdef etcd_cpp_api_EXPORTS
        #define ETCD_API __declspec(dllexport)
    #else
        #define ETCD_API __declspec(dllimport)
    #endif // etcd_cpp_api_EXPORTS
#else 
    // Otherwise, default this to an empty macro
    #define ETCD_API
#endif // _WIN32 && !CPPKAFKA_STATIC

#endif // ETCD_MACROS_H
