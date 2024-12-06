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

int features = 4;
int MAX = 0;

struct Node {                                   //nodes in tree
    vector<int> f;
    vector<int> r;
    double acc;
};

vector<Node> q;
vector<int> maxSet;

void outNode(Node n) {
    printf("\tUsing features {");
    for(int i = 0; i < n.r.size(); i++) {
        printf("%d", n.r[i]);
        if(i != (n.r.size()-1)) {
            printf(",");
        }
    }
    printf("} accuracy is %f%%\n", n.acc);
}

void classify(vector<int> fSet) {                               //classifier
    for(int a = 0; a < data.size(); a++) {
        int minC = 0;
        float min = 999;
        for(int b = 0; b < data.size(); b++) {
            if(a != b) {
                float sum = 0;
                for(int c = 0; c < fSet.size(); c++) {
                    float num;
                    num = data[b].f[fSet[c]] - data[a].f[fSet[c]];
                    sum += num * num;
                }
                sum = sqrt(sum);
                if(sum <= min) {
                    min = sum;
                    if(minC != data[b].c){
                        minC = data[b].c;
                    }
                }
            }
        }
        // printf("Classified Instance: %d\t\tClassified: %d\tActual: %d\n", a, minC, data[a].c);
        if(data[a].c == minC) {
            correct++;
        }
    }
    // printf("\n");
    return;
}

double valid() {                                 //validator
    double num = ((double)correct/data.size()) * 100;
    correct = 0;
    return num;
}

double eval(vector<int> fSet){                   //eval func
    classify(fSet);
    return valid();
}

void expand(Node n, bool e) {                    //expands node
    if(!q.empty()) {
        q.clear();
    }

    if(!e) {
        for(int i = 0; i < n.f.size(); i++){
            Node node;
            // node.f.clear();
            // node.r.clear();
            
            for(int a = 0; a < n.f.size(); a++) {
                if(n.f[i] != n.f[a]) {
                    node.f.push_back(n.f[a]);
                }
            }
            for(int b = 0; b < n.r.size(); b++) {
                node.r.push_back(n.r[b]);
            }

            node.r.push_back(n.f[i]);

            node.acc = eval(n.r);               //might be wrong

            q.push_back(node);
        }
    }
    else if(e) {
        for(int i = 0; i < n.r.size(); i++){
            Node node;
            // node.f.clear();
            // node.r.clear();
            
            for(int a = 0; a < n.r.size(); a++) {
                if(n.r[i] != n.r[a]) {
                    node.r.push_back(n.r[a]);
                }
            }
            for(int b = 0; b < n.f.size(); b++) {
                node.f.push_back(n.f[b]);
            }

            node.f.push_back(n.r[i]);

            node.acc = eval(n.f);               //might be wrong (elim)

            q.push_back(node);
        }
    }
    
}

void search(Node node, bool elim=0) {           //search alg
    vector<int> set;
    int max = node.acc;
    int m = 0;
    int x = 0;
    int d = 0;

    if(!elim) {
        printf("Beninning search\n");
    }
    else if(elim) {
        printf("Beninning search with an accuracy of %f%%\n", node.acc);
    }

    for(int i = 0; i < node.r.size(); i++) {
        set.push_back(node.r[i]);
    }
    
    expand(node, elim);

    while(d < features-1) {
        if(!q.empty()) {
            m = q[0].acc;
        }
        for(int i = 0; i < q.size(); i++) {
            outNode(q[i]);

            if(q[i].acc > m) {
                m = q[i].acc;
                x = i;
            }
        }

        d++;

        if(m >= max) {
            // printf("\nhere1\n");
            printf("\nFeature set {");
            if(!q[x].r.empty()) {
                for(int i = 0; i < q[x].r.size(); i++) {
                    if(q[x].r[i] != -1) {
                        printf("%d", q[x].r[i]);
                        if(i != (q[x].r.size()-1)) {
                            printf(",");
                        }
                    }
                }
            }
            
            printf("} was best, with an accuracy of %f%%\n", q[x].acc);

            set.clear();
            for(int i = 0; i < q[x].r.size(); i++) {
                set.push_back(q[x].r[i]);
            }
            max = q[x].acc;

            if(max > MAX) {
                maxSet.clear();
                for(int i = 0; i < q[x].r.size(); i++) {
                    maxSet.push_back(q[x].r[i]);
                }
                MAX = max;
            }
        }
        else if(m < max || /*d >= (features-1)*/ q.size() == 1) {
            // printf("\nhere2\n");
            if(q.size() != 1) {
                printf("(Warning, accuracy has decreased!)\n");
            }
            printf("Finished search! The best feature subset is {");
            if(!set.empty()) {
                for(int i = 0; i < set.size(); i++) {
                    printf("%d", set[i]);
                    if(i != (set.size()-1)) {
                        printf(",");
                    }
                }
            }
            printf("}, with an accuracy of %d\n\n", max);

            if(max > MAX) {
                maxSet.clear();
                for(int i = 0; i < set.size(); i++) {
                    maxSet.push_back(set[i]);
                }
                MAX = max;
            }
            break;
        }

        expand(q[x], elim);
    }
}

int main(int argc, char* argv[]) {
    int c = 0;
    Node init;
    init.acc = 0;

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

    printf("\nWelcome to Austin Espinosa's Feature Selection Algorithm.\n\n");
    printf("Enter # of features(-1 to use all features): ");
    cin >> features;
    printf("\n\n");

    if(!(features <= -1) && features > data[0].f.size()) {
        printf("ERROR: # of features greater than feature set(%d > %lu)\n\n", features, data[0].f.size());
        return 0;
    }
    else if(features <= -1) {
        features = data[0].f.size();
    }

    int ft = -1;
    int fCount = 0;
    printf("Choose the subset of features(1 - %lu):\n\n", data[0].f.size());
    printf("Enter features(-1 to use all features): ");
    cin >> ft;
    printf("\n");
    if(ft <= -1 && features == data[0].f.size()) {
        for(int a = 0; a < data[0].f.size(); a++) {
            fSub.push_back(a);
        }
    }
    else if(ft <= -1 && features != data[0].f.size()){
        printf("ERROR: Surpassed given feature limit\n\n");
        return 0;
    }
    else {
        while(fCount < features) {
            if(ft > data[0].f.size() || ft < -1) {
                printf("ERROR: invalid feature\n");
            }
            else {
                fSub.push_back(ft-1);
            }
            cin >> ft;
            fCount++;
        }
        printf("\n\n");
    }

    while(!c) {
        printf("Select algorithm:\n");
        printf("\t1. Forward Selection\n");
        printf("\t2. Backward Selection\n");
        printf("\t3. Special Algorithm\n");
        cin >> c;
        printf("\n");

        if(c == 1) {
            for(int i = 0; i < fSub.size(); i++) {
                init.f.push_back(fSub[i]);
            }
            if(!init.r.empty()) {
                init.r.clear();
            }
            search(init);
        }
        else if(c == 2) {
            init.acc = eval(fSub);                          //might be wrong
            if(!init.f.empty()) {
                init.f.clear();
            }
            for(int i = 0; i < fSub.size(); i++) {
                init.r.push_back(fSub[i]);
            }
            search(init, 1);
        }
        else if(c == 3) {
            for(int i = 0; i < fSub.size(); i++) {
                init.f.push_back(fSub[i]);
            }
            if(!init.r.empty()) {
                init.r.clear();
            }
            search(init);

            init.acc = eval(fSub);                          //might be wrong
            if(!init.f.empty()) {
                init.f.clear();
            }
            for(int i = 0; i < fSub.size(); i++) {
                init.r.push_back(fSub[i]);
            }
            search(init, 1);

            printf("THE BEST features are the set {");
            if(!maxSet.empty()) {
                for(int i = 0; i < maxSet.size(); i++) {
                    printf("%d", maxSet[i]);
                    if(i != (maxSet.size()-1)) {
                        printf(",");
                    }
                }
            }
            printf("}, with an accuracy of %d\n\n", MAX);
        }
        else {
            c = 0;
        }
    }
    
    return 0;
}