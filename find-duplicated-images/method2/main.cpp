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
    ImageInfo() {
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
    FILE* fout = fopen("uniq.txt", "w");
    for ( ; it!=hash_map.end(); it++ ) {
        vector<string>& image_names = it->second;
        if (image_names.size()>1) {
            duplicated_groups++;
            int len = image_names.size();
            printf("duplicated group %d\n", duplicated_groups);
            for (int i=0; i<len; i++) {
                printf("\t%s\n", image_names[i].c_str());
            }
        } else {
            fprintf(fout, "%s\n", image_names[0].c_str());
        }
    }
    fclose(fout);
}

void extract_and_save_features()
{
    string im_list = "/Users/chris/data/traffic_sign/dir.txt";
    string im_dir = "/Users/chris/data/traffic_sign/JPEGImages";

    vector<string> image_names;
    read_txt_line_by_line(im_list.c_str(), image_names);

    int len = image_names.size();

    unordered_map<string, vector<string> > hash_map;
    FILE* fout = fopen("phash_features.txt", "w");
    for(int i=0; i<len; i++) {
        string image_path = im_dir + "/" + image_names[i];
        cv::Mat src = cv::imread(image_path);
        string feature = get_phash(src);
        fprintf(fout, "%s %s\n", image_names[i].c_str(), feature.c_str());
    }
    fclose(fout);
}

int hamming_distance(const string& feature, const string& feature2)
{
    if (feature.length() != feature2.length()) {
        return -1;
    }
    if (feature.length() == 0 || feature2.length() == 0) {
        return -2;
    }
    int len = feature.length();
    int dist = 0;
    for (int i=0; i<len; i++) {
        if (feature[i]!=feature2[i]) {
            dist ++;
        }
    }
    return dist;
}

int find(int x, vector<int>& pre) {
    // find parent, with path squeeze operation
    return pre[x]==x ? x : pre[x] = find(pre[x], pre);
}

void find_similar_images()
{
    string txt_pth = "phash_features.txt";
    vector<string> lines;
    read_txt_line_by_line(txt_pth.c_str(), lines);

    // read features
    printf("--- read features\n");
    int len = lines.size();
    string delim = " ";
    vector<ImageInfo> info_lst(len);
    for (int i=0; i<len; i++) {
        vector<string> items = split(lines[i], delim);
        string& image_name = items[0];
        string& feature = items[1];
        string im_head = image_name.substr(0, 6);
        int idx = std::stoi(im_head);
        info_lst[idx].feature = feature;
        info_lst[idx].path = image_name;
    }

    // compute similarity matrix
    printf("--- compute similarity matrix\n");
    vector<vector<int> > similarity(len, vector<int>(len, 0));
    for (int i=0; i<len; i++) {
        string& base_feature = info_lst[i].feature;
        for (int j=i+1; j<len; j++) {
            string& current_feature = info_lst[j].feature;
            int dist = hamming_distance(base_feature, current_feature);
            if (dist>=0 && dist<2) {
                similarity[i][j] = 1;
                similarity[j][i] = 1;
            }
        }
    }

    // use find-union-set algorithm to form groups, each group shares similar features
    printf("--- find-union-set\n");
    vector<int> speaker(len);
    for (int i=0; i<len; i++) {
        speaker[i] = i; // each image feature is its own speaker
    }
    int group_cnt = len;
    for (int i=0; i<len; i++) {
        for (int j=0; j<len; j++) {
            if (i!=j && similarity[i][j]==1) {
                int i_speaker = find(i, speaker);
                int j_speaker = find(j, speaker);
                if (i_speaker != j_speaker) {
                    speaker[i_speaker] = j_speaker;
                    group_cnt--;
                }
            }
        }
    }

    // find all group's speakers
    // printf("--- find all group's speakers\n");
    set<int> speaker_set;
    for (int i=0; i<len; i++) {
        speaker_set.insert(speaker[i]);
    }
    unordered_map<string, vector<string> > duplicated_groups;
    for (int i=0; i<len; i++) {
        int idx = speaker[i];
        if (i==idx) continue;
        string& speaker_image_path = info_lst[idx].path;
        string& duplicated_image_path = info_lst[i].path;
        duplicated_groups[speaker_image_path].push_back(duplicated_image_path);
    }

    // print out each duplicated groups
    printf("forms %d groups, number of groups that contain duplicated images is %zu\n",
            group_cnt,
            duplicated_groups.size());
    unordered_map<string, vector<string> >::iterator it = duplicated_groups.begin();

    int dup_cnt = 0;
    for ( ; it!=duplicated_groups.end(); it++) {
        string speaker_image_path = it->first;
        vector<string>& duplicated_images = it->second;
        dup_cnt ++;
        printf("%d-th duplicated group, speaker %s\n", dup_cnt, speaker_image_path.c_str());
        for (int i=0; i<duplicated_images.size(); i++) {
            printf("\t%s\n", duplicated_images[i].c_str());
        }
    }
}

int main()
{
    // find_duplicated_image_groups();

    //extract_and_save_features();
    find_similar_images();
    return 0;
}
