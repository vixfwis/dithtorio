#include "dithering.h"
#include "blueprint.h"

const Vec3i w(11, 59, 30);

void find_nearest_color(vector<Vec3b>& palette, Vec3b& color, Vec3b& nearest_color, uchar& index) {
    int color_dist;
    int min_color_dist = INT_MAX;

    vector<Vec3b>::iterator it;
    for (uchar i = 0; i < palette.size(); i++){
        color_dist = (color[0] - palette[i][0]) * (color[0] - palette[i][0]) * w[0];
        color_dist += (color[1] - palette[i][1]) * (color[1] - palette[i][1]) * w[1];
        color_dist += (color[2] - palette[i][2]) * (color[2] - palette[i][2]) * w[2];
        if (color_dist < min_color_dist) {
            min_color_dist = color_dist;
            nearest_color = palette[i];
            index = i;
        }
    }
}

void dither(Mat_<Vec3b>& img, vector<Vec3b>& palette, Mat_<uchar>& blueprint, Mat_<uchar> alpha, uchar threshold = 127) {
    int width = img.size().width;
    int height = img.size().height;
    copyMakeBorder(img, img, 1, 1, 1, 1, BORDER_CONSTANT);

    Vec3b color;
    Vec3b nearest_color;
    Vec3i err, d0, d1, d2, d3;
    uchar index;

    d("Starting dithering loop with transparency channel");
    for (int j = 1; j < img.cols - 1; ++j) {
        for (int i = 1; i < img.rows - 1; ++i) {
            if(alpha(i, j) < threshold) {
                d("Skipping transparent pixel at [coords]", i, j);
                continue;
            }
            color = img(i, j);
            d("Got color with [coords] [BGR]", i, j, color);
            find_nearest_color(palette, color, nearest_color, index);
            d("Nearest color found [BGR]", nearest_color);
            blueprint(i - 1, j - 1) = index;
            err = (Vec3i)color - (Vec3i)nearest_color;
            img(i, j) = nearest_color;

            d0 = (Vec3i)img(i, j + 1) +     Vec3i(err[0] * 7 >> 4, err[1] * 7 >> 4, err[2] * 7 >> 4);
            d1 = (Vec3i)img(i + 1, j - 1) + Vec3i(err[0] * 3 >> 4, err[1] * 3 >> 4, err[2] * 3 >> 4);
            d2 = (Vec3i)img(i + 1, j) +     Vec3i(err[0] * 5 >> 4, err[1] * 5 >> 4, err[2] * 5 >> 4);
            d3 = (Vec3i)img(i + 1, j + 1) + Vec3i(err[0]     >> 4, err[1]     >> 4, err[2]     >> 4);

            img(i, j + 1) = Vec3b(saturate_cast<uchar>(d0[0]), saturate_cast<uchar>(d0[1]), saturate_cast<uchar>(d0[2]));
            img(i + 1, j - 1) = Vec3b(saturate_cast<uchar>(d1[0]), saturate_cast<uchar>(d1[1]), saturate_cast<uchar>(d1[2]));
            img(i + 1, j) = Vec3b(saturate_cast<uchar>(d2[0]), saturate_cast<uchar>(d2[1]), saturate_cast<uchar>(d2[2]));
            img(i + 1, j + 1) = Vec3b(saturate_cast<uchar>(d3[0]), saturate_cast<uchar>(d3[1]), saturate_cast<uchar>(d3[2]));
        }
    }
    img = Mat(img, Rect(1, 1, width, height));
}

void dither(Mat_<Vec3b>& img, vector<Vec3b>& palette, Mat_<uchar>& blueprint) {
    int width = img.size().width;
    int height = img.size().height;
    copyMakeBorder(img, img, 1, 1, 1, 1, BORDER_CONSTANT);

    Vec3b color;
    Vec3b nearest_color;
    Vec3i err, d0, d1, d2, d3;
    uchar index;

    d("Starting dithering loop");
    for (int j = 1; j < img.cols - 1; ++j) {
        for (int i = 1; i < img.rows - 1; ++i) {
            color = img(i, j);
            d("Got color with [coords] [BGR]", i, j, color);
            find_nearest_color(palette, color, nearest_color, index);
            d("Nearest color found [BGR]", nearest_color);
            blueprint(i - 1, j - 1) = index;
            err = (Vec3i)color - (Vec3i)nearest_color;
            img(i, j) = nearest_color;

            d0 = (Vec3i)img(i, j + 1) +     Vec3i(err[0] * 7 >> 4, err[1] * 7 >> 4, err[2] * 7 >> 4);
            d1 = (Vec3i)img(i + 1, j - 1) + Vec3i(err[0] * 3 >> 4, err[1] * 3 >> 4, err[2] * 3 >> 4);
            d2 = (Vec3i)img(i + 1, j) +     Vec3i(err[0] * 5 >> 4, err[1] * 5 >> 4, err[2] * 5 >> 4);
            d3 = (Vec3i)img(i + 1, j + 1) + Vec3i(err[0]     >> 4, err[1]     >> 4, err[2]     >> 4);

            img(i, j + 1) = Vec3b(saturate_cast<uchar>(d0[0]), saturate_cast<uchar>(d0[1]), saturate_cast<uchar>(d0[2]));
            img(i + 1, j - 1) = Vec3b(saturate_cast<uchar>(d1[0]), saturate_cast<uchar>(d1[1]), saturate_cast<uchar>(d1[2]));
            img(i + 1, j) = Vec3b(saturate_cast<uchar>(d2[0]), saturate_cast<uchar>(d2[1]), saturate_cast<uchar>(d2[2]));
            img(i + 1, j + 1) = Vec3b(saturate_cast<uchar>(d3[0]), saturate_cast<uchar>(d3[1]), saturate_cast<uchar>(d3[2]));
        }
    }
    img = Mat(img, Rect(1, 1, width, height));
}

void process_file(string srcFile, string dstFile, string bprFile, uchar threshold, uint split) {
    d("Starting process_file with args", srcFile, dstFile, bprFile, (uint)threshold, split);
    Mat _img;
    _img = imread(srcFile, IMREAD_UNCHANGED);
    d("Source matrix [rows] [cols] [channels]", _img.rows, _img.cols, _img.channels());
    Mat_<Vec3b> img3b;
    Mat_<Vec4b> img4b;
    vector<Vec3b> palette = get_palette();
    if(palette.empty()) {
        preload_modded();
        palette = get_palette();
    }
    Mat_<uchar> alpha;
    Mat_<uchar> blueprint(_img.rows, _img.cols, 255);

    if(_img.channels() == 3) {
        img3b = _img;
        dither(img3b, palette, blueprint);
        save_blueprint_to_file(blueprint, bprFile, split);
        imwrite(dstFile, img3b);
        d("Saving resulting image");
    }else if(_img.channels() == 4){
        d("Splitting 4 channel transparent image");
        Mat origChan[4];
        cv::split(_img, origChan);
        Mat origClean[3] = {origChan[0], origChan[1], origChan[2]};
        merge(origClean, 3, img3b);
        alpha = origChan[3];
        dither(img3b, palette, blueprint, alpha, threshold);
        Mat newChan[3];
        cv::split(img3b, newChan);
        Mat newClean[4] = {newChan[0], newChan[1], newChan[2], alpha};
        d("Merging transparent channel with resulting image");
        merge(newClean, 4, img4b);
        save_blueprint_to_file(blueprint, bprFile, split);
        imwrite(dstFile, img4b);
        d("Saving resulting 4 channel image");
    }
}

// slapped together to provide some semblance of CLI control
int main(int argc, char** argv) {
    if (argc == 1)
        return 1;
    if(argc == 3 && strcmp(argv[2], "vanilla") == 0)
        preload_vanilla();
    else
        preload_modded();
    string source(argv[1]);
    string destination(argv[1]);
    string blueprint(argv[1]);
    destination.replace(source.find('.') + 1, source.length() - source.find('.'), "png");
    destination = "converted_" + destination;
    blueprint.replace(blueprint.find('.') + 1, blueprint.length() - blueprint.find('.'), "txt");
    blueprint = "blueprint_" + blueprint;
    process_file(source, destination, blueprint);
    return 0;
}