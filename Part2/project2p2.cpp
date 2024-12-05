#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

int correct;

struct Inst {                                   //instances
    int c;
    vector<float> f;
};

vector<Inst> data;
vector<int> fSub;
// vector<int> clssifd;
// vector<float> dists;

void train(vector<int>& id){                    //train instances
    return;
}

int test(int index) {                           //test instances
    return 0;
}

void classify() {                               //classifier
    for(int a = 0; a < data.size(); a++) {
        int minC = 0;
        float min = 999;
        // dists.clear();
        for(int b = 0; b < data.size(); b++) {
            if(a != b) {
                float sum = 0;
                for(int c = 0; c < fSub.size(); c++) {
                    float num;
                    num = data[b].f[fSub[c]] - data[a].f[fSub[c]];
                    sum += num * num;
                }
                // dists.push_back(sum);
                sum = sqrt(sum);
                if(sum <= min) {
                    min = sum;
                    if(minC != data[b].c){
                        minC = data[b].c;
                    }
                }
            }
        }
        printf("Classified Instance: %d\t\tClassified: %d\tActual: %d\n", a, minC, data[a].c);
        if(data[a].c == minC) {
            correct++;
        }
    }
    printf("\n");
    return;
}

double valid() {                                 //validator
    return ((double)correct/data.size()) * 100;
}

int main(int argc, char* argv[]) {

    ifstream file;
    file.open(argv[1]);

    if(!file.is_open()) {
        printf("\n\nERROR: cannot open file %s\n\n", argv[1]);
        return 0;
    }
    else {
        printf("\n\nUsing data set: %s\n\n", argv[1]);
    }

    bool first = 1;
    float num;
    Inst i;

    while(file >> num) {

        if(num == 1 || num == 2) {
            if(!first) {
                data.push_back(i);
            }
            else {
                first = 0;
            }
            i.c = -1;
            i.f.clear();
            i.c = num;
            // printf("\nClass: %d\n", i.c);
        }
        else {
            i.f.push_back(num);
            // printf("Feature: %f\n", i.f[0]);
        }
    }

    data.push_back(i);

    // for(int a = 0; a < data.size(); a++) {
    //     ft += data[a].f.size();
    // }
    // printf("Class Total: %d\nTot Features: %d\n", n, ft);
    // printf("Class Total: %lu\n", data.size());
    // printf("F Ex: %f\n", data[0].f[9]);

    file.close();

    int ft = -1;
    printf("Choose the subset of features(1 - %lu):\n\n", data[0].f.size());
    printf("Enter feature(enter 0 when finished or enter -1 to enter all features): ");
    cin >> ft;
    printf("\n");
    if(ft == -1 || ft == 0) {
        for(int a = 0; a < data[0].f.size(); a++) {
            fSub.push_back(a);
        }
    }
    else {
        while(ft) { // != 0
            if(ft > data[0].f.size() || ft < -1) {
                printf("ERROR: invalid feature\n");
            }
            else {
                fSub.push_back(ft-1);
            }
            cin >> ft;
        }
        printf("\n\n");
    }

    classify();

    printf("Correct: %d/%lu\n", correct, data.size());
    printf("Accuracy: %f%%\n\n", valid());

    return 0;
}