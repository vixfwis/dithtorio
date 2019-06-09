#ifndef DITHTORIO_H
#define DITHTORIO_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
using namespace rapidjson;
using namespace std;
using namespace cv;

void save_blueprint_to_file(Mat_<uchar>& blueprint, string bprFile, uint split);
void add_factorio_entity(string name, uchar red, uchar green, uchar blue);
void add_factorio_tile(string name, uchar red, uchar green, uchar blue);
void clear_data();
void preload_modded();
void preload_vanilla();

vector<Vec3b> get_palette();
vector<string> get_fobjects();
vector<bool> get_bools();

void process_file(string srcFile, string dstFile, string bprFile, bool forceNT = false, uchar threshold = 0, uint split = 65536);

#endif //DITHTORIO_H
