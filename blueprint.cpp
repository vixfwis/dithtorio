#include "compression.h"
#include "base64.h"
#include "blueprint.h"
#ifdef DEBUG
std::chrono::time_point<std::chrono::steady_clock> t = std::chrono::steady_clock::now();
#endif

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
