#include "compression.h"
#include "dithtorio.h"
#include "debug.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

// ============================================================================================
// Base64 Encoder/Decoder
// ============================================================================================
/**
 * The MIT License (MIT)
 * Copyright (c) 2016 tomykaira
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string>

namespace macaron {

    class Base64 {
    public:

        static std::string Encode(const std::string data) {
            static constexpr char sEncodingTable[] = {
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                    'w', 'x', 'y', 'z', '0', '1', '2', '3',
                    '4', '5', '6', '7', '8', '9', '+', '/'
            };

            size_t in_len = data.size();
            size_t out_len = 4 * ((in_len + 2) / 3);
            std::string ret(out_len, '\0');
            size_t i;
            char *p = const_cast<char*>(ret.c_str());

            for (i = 0; i < in_len - 2; i += 3) {
                *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
                *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
                *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int) (data[i + 2] & 0xC0) >> 6)];
                *p++ = sEncodingTable[data[i + 2] & 0x3F];
            }
            if (i < in_len) {
                *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
                if (i == (in_len - 1)) {
                    *p++ = sEncodingTable[((data[i] & 0x3) << 4)];
                    *p++ = '=';
                }
                else {
                    *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
                    *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
                }
                *p++ = '=';
            }

            return ret;
        }

        static std::string Decode(const std::string& input) {
            static constexpr unsigned char kDecodingTable[] = {
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
                    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
                    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
                    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
                    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
            };

            size_t in_len = input.size();
            if (in_len % 4 != 0) return "Input data size is not a multiple of 4";

            size_t out_len = in_len / 4 * 3;
            if (input[in_len - 1] == '=') out_len--;
            if (input[in_len - 2] == '=') out_len--;

            std::string out;
            out.resize(out_len);

            for (size_t i = 0, j = 0; i < in_len;) {
                uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
                uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
                uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
                uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

                uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

                if (j < out_len) out[j++] = (triple >> 2 * 8) & 0xFF;
                if (j < out_len) out[j++] = (triple >> 1 * 8) & 0xFF;
                if (j < out_len) out[j++] = (triple >> 0 * 8) & 0xFF;
            }

            return out;
        }

    };

}

// ============================================================================================

vector<Vec3b> palette;
vector<string> fobjects;
vector<bool> isEntity;

struct bp_object{
    bool isEntity;
    string name;
    int x, y;
    friend ostream& operator<<(ostream& out, const bp_object& obj){
        out<<"["<<((obj.isEntity)?"ENTITY, ":"TILE, ")<<obj.name<<", "<<obj.x<<", "<<obj.y<<"]";
        return out;
    }
};


string wrap_blueprint(vector<bp_object>& objs, uint size, uint bp_id, int offset){
    d("Starting blueprint wrapping with params [size] [bp id] [offset]", size, bp_id, offset);
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    char digits[4] = {
            char((bp_id / 1000) % 10 +48),
            char((bp_id / 100) % 10 +48),
            char((bp_id / 10) % 10 +48),
            char(bp_id % 10 +48)
    };
    char sig[] = "signal-0";
    d("Starting building JSON object");
    writer.StartObject();
    writer.Key("blueprint");
    writer.StartObject();
    writer.Key("icons");
    writer.StartArray();
    for(uint i = 0; i < 4; i++){
        writer.StartObject();
        writer.Key("signal");
        writer.StartObject();
        writer.Key("type");
        writer.String("virtual");
        writer.Key("name");
        sig[7] = digits[i];
        writer.String(sig);
        writer.EndObject();
        writer.Key("index");
        writer.Uint(i+1);
        writer.EndObject();
    }
    writer.EndArray();

    writer.Key("entities");
    writer.StartArray();
    uint entity_count = 1;
    for(uint i = 0; i < size; i++){
        if(!objs[i].isEntity)
            continue;
        writer.StartObject();
        writer.Key("entity_number");
        writer.Uint(entity_count++);
        writer.Key("name");
        writer.String(objs[i].name.c_str());
        writer.Key("position");
        writer.StartObject();
        writer.Key("x");
        writer.Int(objs[i].x);
        writer.Key("y");
        writer.Int(objs[i].y - offset);
        writer.EndObject();
        writer.EndObject();
    }
    writer.EndArray();

    writer.Key("tiles");
    writer.StartArray();
    for(uint i = 0; i < size; i++){
        if(objs[i].isEntity)
            continue;
        writer.StartObject();
        writer.Key("name");
        writer.String(objs[i].name.c_str());
        writer.Key("position");
        writer.StartObject();
        writer.Key("x");
        writer.Int(objs[i].x);
        writer.Key("y");
        writer.Int(objs[i].y - offset);
        writer.EndObject();
        writer.EndObject();
    }
    writer.EndArray();
    writer.Key("item");
    writer.String("blueprint");
    writer.Key("version");
    writer.Uint(0);
    writer.EndObject();
    writer.Key("index");
    writer.Uint(bp_id);
    writer.EndObject();
    d("JSON object completed");
    return s.GetString();
}

void save_blueprint_to_file(Mat_<uchar>& blueprint, string bprFile, uint split){
    d("Starting blueprint processing");
    uchar index;
    vector<bp_object> buffer(split);
    vector<string> bps;
    bp_object *ptr;
    uint array_count = 0;
    uint bp_count = 0;
    int offset = 0;
    int rows = blueprint.rows, cols = blueprint.cols;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            index = blueprint(i, j);
            if(index == 255)
                continue;
            d("Got index from blueprint with [id] [coords]", (uint)index, i, j);
            ptr = &buffer[array_count];
            ptr->x = j;
            ptr->y = i;
            ptr->name = fobjects[index];
            if(isEntity[index]){
                ptr->isEntity = true;
            }else{
                ptr->isEntity = false;
            }
            array_count++;
            d("fObject added to buffer [object] [array counter]", *ptr, array_count);
            if(array_count >= split){
                bps.push_back(wrap_blueprint(buffer, array_count, bp_count++, offset));
                offset = i;
                array_count = 0;
            }
        }
    }
    if(array_count > 0)
        bps.push_back(wrap_blueprint(buffer, array_count, bp_count++, offset));

    d("Starting building blueprint book");
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("blueprint_book");
    writer.StartObject();
    writer.Key("blueprints");
    writer.StartArray();
    for(auto it = bps.begin(); it != bps.end(); it++){
        const char* raw_bp = (*it).c_str();
        writer.RawValue(raw_bp, strlen(raw_bp), kObjectType);
    }
    writer.EndArray();
    writer.Key("item");
    writer.String("blueprint-book");
    writer.Key("active_index");
    writer.Uint(0);
    writer.Key("version");
    writer.Uint(0);
    writer.EndObject();
    writer.EndObject();
    d("Blueprint book finished");
    ofstream file(bprFile);
    file << "0" << macaron::Base64::Encode(compress_string(s.GetString()));
    file.close();
    d("Blueprint saved");
}

void add_factorio_entity(string name, uchar red, uchar green, uchar blue) {
    palette.push_back(Vec3b(blue, green, red));
    fobjects.push_back(name);
    isEntity.push_back(true);
}

void add_factorio_tile(string name, uchar red, uchar green, uchar blue) {
    palette.push_back(Vec3b(blue, green, red));
    fobjects.push_back(name);
    isEntity.push_back(false);
}

void clear_data() {
    palette.clear();
    fobjects.clear();
    isEntity.clear();
}

vector<Vec3b> get_palette() {
    return palette;
}

vector<string> get_fobjects() {
    return fobjects;
}

vector<bool> get_bools() {
    return isEntity;
}

void preload_modded() {
    d("Starting loading modded palette");
    palette.clear();
    fobjects.clear();
    isEntity.clear();
    palette.push_back(Vec3b(8, 12, 123));
    palette.push_back(Vec3b(8, 60, 123));
    palette.push_back(Vec3b(8, 125, 123));
    palette.push_back(Vec3b(8, 125, 8));
    palette.push_back(Vec3b(123, 125, 8));
    palette.push_back(Vec3b(123, 12, 8));
    palette.push_back(Vec3b(123, 12, 57));
    palette.push_back(Vec3b(123, 12, 123));
    palette.push_back(Vec3b(123, 125, 123));
    palette.push_back(Vec3b(8, 12, 8));
    palette.push_back(Vec3b(49, 48, 49));
    palette.push_back(Vec3b(99, 101, 99));
    palette.push_back(Vec3b(128, 93, 0));
    palette.push_back(Vec3b(66, 158, 206));
    palette.push_back(Vec3b(189, 203, 189));
    fobjects.push_back("refined-concrete-red");
    fobjects.push_back("refined-concrete-orange");
    fobjects.push_back("refined-concrete-yellow");
    fobjects.push_back("refined-concrete-green");
    fobjects.push_back("refined-concrete-cyan");
    fobjects.push_back("refined-concrete-blue");
    fobjects.push_back("refined-concrete-purple");
    fobjects.push_back("refined-concrete-magenta");
    fobjects.push_back("refined-concrete-white");
    fobjects.push_back("refined-concrete-black");
    fobjects.push_back("stone-path");
    fobjects.push_back("refined-concrete");
    fobjects.push_back("wooden-chest");
    fobjects.push_back("transport-belt");
    fobjects.push_back("stone-wall");
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(true);
    isEntity.push_back(true);
    isEntity.push_back(true);
#ifdef DEBUG
    for(uint i = 0; i < palette.size(); i++){
        d("Loaded [color] [name] [isEntity]", palette[i], fobjects[i], isEntity[i]);
    }
#endif
}

void preload_vanilla() {
    d("Starting loading vanilla palette");
    palette.clear();
    fobjects.clear();
    isEntity.clear();
    palette.push_back(Vec3b(49, 48, 49));
    palette.push_back(Vec3b(99, 101, 99));
    palette.push_back(Vec3b(99, 101, 99));
    palette.push_back(Vec3b(66, 158, 206));
    palette.push_back(Vec3b(189, 203, 189));
    fobjects.push_back("stone-path");
    fobjects.push_back("refined-concrete");
    fobjects.push_back("wooden-chest");
    fobjects.push_back("transport-belt");
    fobjects.push_back("stone-wall");
    isEntity.push_back(false);
    isEntity.push_back(false);
    isEntity.push_back(true);
    isEntity.push_back(true);
    isEntity.push_back(true);
#ifdef DEBUG
    for(uint i = 0; i < palette.size(); i++){
        d("Loaded [color] [name] [isEntity]", palette[i], fobjects[i], isEntity[i]);
    }
#endif
}
