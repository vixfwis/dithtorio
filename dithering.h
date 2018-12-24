#ifndef DITHERING_DITHERING_H
#define DITHERING_DITHERING_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace cv;

void process_file(string srcFile, string dstFile, string bprFile, uchar threshold = 127, uint split = 65536);

#endif //DITHERING_DITHERING_H
