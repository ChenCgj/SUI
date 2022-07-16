#include <iostream>

#define LOG_OPEN
// #define DEBUG_OPEN
#define ERROR_OPEN

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

#define LOG_in (std::cout << "          [ LOG ]: " << "file: " << __FILE_NAME__ << ". line: "\
        << __LINE__ << ". function: "\
        << __func__ << ".\n          [MSG: ")

#define DBG_in (std::cout << "     [ DEBUG ]: " << "file: " << __FILE_NAME__ << ". line: "\
        << __LINE__ << ". function: "\
        << __func__ << ".\n     [MSG: ")

#define ERR_in (std::cerr << "[ *ERROR* ]: " << "file: " << __FILE_NAME__ << ". line: "\
        << __LINE__ << ". function: "\
        << __func__ << ".\n[ MSG: ")

#ifdef LOG_OPEN
#define LOG(msg) LOG_in msg << " ]" << std::endl << std::endl
#else
#define LOG(msg)
#endif
#ifdef DEBUG_OPEN
#define DBG(msg) DBG_in msg << " ]" << std::endl << std::endl
#else
#define DBG(msg)
#endif
#ifdef ERROR_OPEN
#define ERR(msg) ERR_in msg << " ]" << std::endl << std::endl
#else
#define ERR(msg)
#endif

namespace sui {
// class Debug_tool {
// public:
//     static Debug_tool &get_log_print();
//     static Debug_tool &get_error_print();
//     static Debug_tool &get_debug_print();
//     template<typename T>
//     Debug_tool &operator<<(T rhs) {
//         o << rhs;
//         return *this;
//     }
//     Debug_tool &operator<<(std::basic_ostream<char> &(*_Pfn)(std::basic_ostream<char> &)__attribute__((cdecl))) {
//         o << _Pfn;
//         return *this;
//     }
// private:
//     Debug_tool(std::ostream &out);
//     std::ostream &o;
// };
}
