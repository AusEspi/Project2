#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int features = 4;

struct Node {                                   //nodes in tree
    vector<int> f;
    vector<int> r;
    int acc;
};

vector<Node> q;

void outNode(Node n) {
    printf("\tUsing features {");
    for(int i = 0; i < n.r.size(); i++) {
        printf("%d", n.r[i]);
        if(i != (n.r.size()-1)) {
            printf(",");
        }
    }
    printf("} accuracy is %d%%\n", n.acc);
}

int eval(){                                     //rand eval
    return rand() % 100 + 1;
}

void expand(Node n, bool e) {                   //expands node
    if(!q.empty()) {
        q.clear();
    }

    if(!e) {
        for(int i = 0; i < n.f.size(); i++){
            Node node;
            // node.f.clear();
            // node.r.clear();

            node.acc = eval();
            
            for(int a = 0; a < n.f.size(); a++) {
                if(n.f[i] != n.f[a]) {
                    node.f.push_back(n.f[a]);
                }
            }
            for(int b = 0; b < n.r.size(); b++) {
                node.r.push_back(n.r[b]);
            }

            node.r.push_back(n.f[i]);

            q.push_back(node);
        }
    }
    else if(e) {
        for(int i = 0; i < n.r.size(); i++){
            Node node;
            // node.f.clear();
            // node.r.clear();

            node.acc = eval();
            
            for(int a = 0; a < n.r.size(); a++) {
                if(n.r[i] != n.r[a]) {
                    node.r.push_back(n.f[a]);
                }
            }
            for(int b = 0; b < n.f.size(); b++) {
                node.f.push_back(n.f[b]);
            }

            node.f.push_back(n.r[i]);

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

    printf("Beninning search\n");

    for(int i = 0; i < node.r.size(); i++) {
        set.push_back(node.r[i]);
    }
    
    expand(node, elim);
    // printf("\nhere\n");

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
            
            printf("} was best, with an accuracy of %d%%\n", q[x].acc);

            set.clear();
            for(int i = 0; i < q[x].r.size(); i++) {
                set.push_back(q[x].r[i]);
            }
            max = q[x].acc;
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
            printf("}, with an accuracy of %d\n", max);
            break;
        }

        expand(q[x], elim);
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
        printf("\n");

        if(c == 1) {
            for(int i = 0; i < features; i++) {
                init.f.push_back(i+1);
            }
            if(!init.r.empty()) {
                init.r.clear();
            }
            search(init);
        }
        else if(c == 2) {
            init.acc = eval();
            if(!init.f.empty()) {
                init.f.clear();
            }
            for(int i = 0; i < features; i++) {
                init.r.push_back(i+1);
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