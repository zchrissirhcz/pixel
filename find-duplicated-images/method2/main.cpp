#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <unordered_map>

#include <opencv2/opencv.hpp>

#include "phash.h"

using namespace std;
using namespace cv;

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

void find_duplicated_image_pairs() {

    string im_list = "/Users/chris/data/traffic_sign/dir.txt";
    string im_dir = "/Users/chris/data/traffic_sign/JPEGImages";

	vector<string> image_names;
	read_txt_line_by_line(im_list.c_str(), image_names);

    int len = image_names.size();

    unordered_map<string, string> hash_map;
    for(int i=0; i<len; i++) {
        string image_path = im_dir + "/" + image_names[i];
        cv::Mat src = cv::imread(image_path);
		string feature = get_phash(src);
        if (hash_map.count(feature)==0) {
            hash_map.insert({feature, image_path});
        } else {
            printf("phash duplicated: %s with %s\n", hash_map[feature].c_str(), image_path.c_str());
        }
    }
}


class ImageInfo {
public:
    string feature;
    string path;
    ImageInfo(const string& _feature, const string& _path) {
        feature = _feature;
        path = _path;
    }
};

void find_duplicated_image_groups() {

    string im_list = "/Users/chris/data/traffic_sign/dir.txt";
    string im_dir = "/Users/chris/data/traffic_sign/JPEGImages";

	vector<string> image_names;
	read_txt_line_by_line(im_list.c_str(), image_names);

    int len = image_names.size();

    unordered_map<string, vector<string> > hash_map;
    for(int i=0; i<len; i++) {
        string image_path = im_dir + "/" + image_names[i];
        cv::Mat src = cv::imread(image_path);
		string feature = get_phash(src);
        if (hash_map.count(feature)==0) {
            hash_map[feature] = vector<string>();
        }
        hash_map[feature].push_back(image_names[i]);
    }
    unordered_map<string, vector<string> >::iterator it = hash_map.begin();
    int duplicated_groups = 0;
    for ( ; it!=hash_map.end(); it++ ) {
        vector<string>& image_names = it->second;
        if (image_names.size()>1) {
            duplicated_groups++;
            int len = image_names.size();
            printf("duplicated group %d\n", duplicated_groups);
            for (int i=0; i<len; i++) {
                printf("\t%s\n", image_names[i].c_str());
            }
        }
    }
}

int main()
{
    find_duplicated_image_groups();
    return 0;
}
