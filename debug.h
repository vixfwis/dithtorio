#ifndef DITHTORIO_DEBUG_H
#define DITHTORIO_DEBUG_H

#ifdef DEBUG
#include <iostream>
#include <utility>
#include <chrono>
extern std::chrono::time_point<std::chrono::steady_clock> t;
template <typename Arg, typename... Args>
void var_out(std::ostream& out, Arg&& arg, Args&&... args){
    out << std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now()-t).count() << " ";
    out << std::forward<Arg>(arg);
    using ex = int[];(void)ex{0,(void(out<<", "<<std::forward<Args>(args)), 0)...};
    out<<std::endl;
}
#define d(...) var_out(std::cout, __VA_ARGS__)
#else
#define d(...)
#endif

#endif //DITHTORIO_DEBUG_H
