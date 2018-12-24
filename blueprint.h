#ifndef DITHERING_BLUEPRINT_H
#define DITHERING_BLUEPRINT_H
#include <fstream>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

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

using namespace std;
using namespace cv;
using namespace rapidjson;

void save_blueprint_to_file(Mat_<uchar>& blueprint, string bprFile, uint split);
void add_factorio_entity(string name, uchar red, uchar green, uchar blue);
void add_factorio_tile(string name, uchar red, uchar green, uchar blue);
void clear_data();
void preload_modded();
void preload_vanilla();

vector<Vec3b> get_palette();
vector<string> get_fobjects();
vector<bool> get_bools();

#endif //DITHERING_BLUEPRINT_H
