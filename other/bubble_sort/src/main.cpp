#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

double msElapsedTime(chrono::system_clock::time_point start) {
    auto end = chrono::system_clock::now();

    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

chrono::system_clock::time_point now() {
    return chrono::system_clock::now();
}

void bubble_sort(int* data, int n) {
    int i, j, temp;
    for (i=0; i<n-1; i++) {
        for (j=0; j<n-1; j++) {
            if (data[j] > data[j+1]) {
                temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}

int main()
{
    int n = 32768;
    int* data = (int*)malloc(n*sizeof(int));

    auto t_start = now();
    bubble_sort(data, n);
    auto t_cost = msElapsedTime(t_start);

    string res = "naive impl cost " + to_string((int)t_cost) + " ms";
    cout << res << endl;

    free(data);

    return 0;
}