#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int features = 4;

struct Node {                                   //nodes in tree
    vector<int> f;
    vector<int> r;
    double acc;
};

vector<Node> q;

void outNode(Node n) {
    printf("\tUsing features {");
    for(int i = 0; i < features; i++) {
        if(n.r[i] != -1) {
            printf("%d ", n.r[i]);
        }
    }
    printf("} accuracy is %f%%\n", n.acc);
}

int eval(){                                     //rand eval
    return (rand() % 100 + 1) / 100;
}

void expand(Node n, bool e) {

}

void search(Node node, bool elim=0) {           //search alg
    int max = node.acc;
    int m = 0;
    int x = 0;

    printf("Beninning search\n");

    expand(node, elim);

    while(1) {}
    if(!q.empty()) {
        m = q[0].acc;
    }
    for(int i = 0; i < q.size(); i++) {
        if(q[i].acc > m) {
            m = q[i].acc;
            x = i;
        }
    }

    if(m > max) {
        
    }
}

int main() {
    int c = 0;
    Node init;
    init.acc = 0;

    printf("\nWelcome to Austin Espinosa's Feature Selection Algorithm.\n\n");
    printf("Enter # of features: ");
    cin >> features;
    printf("\n\n");

    while(!c) {
        printf("Select algorithm:\n");
        printf("\t1. Forward Selection\n");
        printf("\t2. Backward Selection\n");
        printf("\t3. Special Algorithm\n");
        cin >> c;

        if(c == 1) {
            for(int i = 0; i < features; i++) {
                init.f.push_back(i);
            }
            if(!init.r.empty()) {
                init.r.clear();
            }
            search(init);
        }
        else if(c == 2) {
            if(!init.f.empty()) {
                init.f.clear();
            }
            for(int i = 0; i < features; i++) {
                init.r.push_back(i);
            }
            search(init, 1);
        }
        else if(c == 3) {

        }
        else {
            c = 0;
        }
    }
    


    return 0;
}