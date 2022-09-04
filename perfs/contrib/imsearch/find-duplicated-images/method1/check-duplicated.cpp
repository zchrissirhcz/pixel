#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

void read_txt_line_by_line(const char* path, vector<string>& result)
{
    FILE* fin = fopen(path, "r");
    if(fin==NULL) {
        fprintf(stderr, "failed to open %s\n", path);
        exit(-1);
    }
    char line[1024];
    void* ret;
    while (true) {
        memset(line, 0, sizeof(line));
        ret = fgets(line, sizeof(line), fin);
        if (ret == NULL) break;
        if(line[strlen(line)-1]=='\n' || line[strlen(line)-1]=='\r') {
            line[strlen(line) - 1] = '\0';
        }
        if (strlen(line)>0) {
            if(line[strlen(line)-1]=='\r' || line[strlen(line)-1]=='\n') {
                line[strlen(line) - 1] = '\0';
            }
        }
        //printf("%s\n", line);
		result.push_back(line);
    }
    fclose(fin);
}

vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

class ImageInfo {
public:
    string hash;
    string path;
    ImageInfo(const string& _hash, const string& _path) {
        hash = _hash;
        path = _path;
    }
};

int main() {
    //string txt_pth = "md5sums.txt";
    string txt_pth = "downsample4x4_md5sums.txt";
    vector<string> lines;
	read_txt_line_by_line(txt_pth.c_str(), lines);

    string delim = " ";
    unordered_map<string, string> hash_map;
    vector<ImageInfo> info_lst;
    for (int i=0; i<lines.size(); i++) {
		vector<string> items = split(lines[i], delim);
        string hash = items[0];
        string image_path = items[1];
        ImageInfo image_info(hash, image_path);
        info_lst.push_back(image_info);
        if (hash_map.count(hash)==0) {
            hash_map.insert({hash, image_path});
        } else {
            cout << "duplicated: " << image_path << " with " << hash_map[hash] << endl;
        }
    }
    cout << hash_map.size() << endl;

    return 0;
}
