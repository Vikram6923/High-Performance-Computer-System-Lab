#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include <climits>
using namespace std;

class Node {
public:
    int value;
    int index;
    Node* next;
    Node() {
        this->value = 0;
        this->next = NULL;
    }
    Node (int data) {
        this->value = data;
        this->next = NULL;
    }
};

class List {
public:
    vector<Node> array;
    Node* head;

    List() {
        this->head = NULL;
    }

    void addNode(int data) {
        Node* temp = new Node(data);
        if (this->head == NULL) {
            this->head = temp;
            array.push_back(*temp);
        }
        else {
            Node* curr = this->head;
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = temp;
            array.push_back(*temp);
        }
    }
    void print() {
        for (int i = 0; i < array.size(); i++) {
            cout << array[i].value << " ";
        }
        cout << endl;
    }
    int size() {
        return array.size();
    }

};

List sequentialPrefixSum(List list) {
    List prefixSum;
    int sum = 0;
    for (int i = 0; i < list.size(); i++) {
        sum += list.array[i].value;
        prefixSum.addNode(sum);
    }
    return prefixSum;
}

List prefix_sum(List A, int n, int (*f1)(int), int (*f2)(int, int)) {
    int log2n = log2(n);

    vector<int> M(2*n);
    vector<int> R(2*n);

    // Declaring offsets
    int offsets[log2n + 2];
    int offset = n << 1;
    for (int i = 0; i <= log2n + 1; i++) {
        offsets[i] = offset >> i;
    }

    // Bottom-up phase
    M[0] = 0;
    #pragma omp parallel for
    for (int i = n; i < 2*n; i++) {
        M[i] = f1(A.array[i-n].value);
    }

    for (int r = 1; r <= log2n; r++) {
        #pragma omp parallel for 
        for (int i = offsets[r + 1]; i < offsets[r]; i += 2) {
            M[i] = f2(M[2 * i], M[2 * i + 1]);
            M[i + 1] = f2(M[2 * (i + 1)], M[2 * (i + 1) + 1]);
        }
    }

    R[0] = R[1] = 0;
    int m = log2n + 1;
    for (int r = 2; r <= log2n + 1; r++) {
        #pragma omp parallel for 
        for (int i = offsets[m]; i < offsets[m - 1]; i++) {
            R[2*i] = R[i];
            R[2*i + 1] = f2(R[i], M[2*i]);
        }
        m--;
    }

    List prefixSum = A;
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        prefixSum.array[i].value = f2(A.array[i].value, R[i + n]);
    }

    return prefixSum;
}


int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads

    // read in values of n and n_t
    if (argc < 3) {
        cout << "Enter the size of the arrays: ";
        string input_str;
        cin >> input_str;
        if (input_str.substr(0, 2) == "2^") {
            int power = stoi(input_str.substr(2));
            n = pow(2, power);
        } else {
            n = stoi(input_str);
        }
        cout << "Enter the number of threads: ";
        cin >> n_t;
    } else {
        if (string(argv[1]).substr(0, 2) == "2^") {
            int power = stoi(string(argv[1]).substr(2));
            n = pow(2, power);
        } else {
            n = atoi(argv[1]);
        }
        n_t = atoi(argv[2]);
    }
    omp_set_num_threads(n_t);

    List list;
    for (int i = 0; i < n; i++) {
        // list.addNode(i + 1);
        list.addNode(1);
    }
    if (n < 100) {
        printf("A : ");
        list.print();
    }

    List B = prefix_sum(list, n, [](int x) {return x;}, [](int x, int y) {return x + y;});
    if (n < 100) {
        printf("B : ");
        B.print();
    }
    cout << "Last element of B: " << B.array[n-1].value << endl;

    return 0;
}
