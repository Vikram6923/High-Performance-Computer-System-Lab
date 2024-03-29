#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include <climits>
#include <bits/stdc++.h>
using namespace std;

template<typename T>
class Node {
public:
    T value;
    int index;
    Node<T>* next;
    Node() {
        this->value = T();
        this->next = NULL;
    }
    Node(T data) {
        this->value = data;
        this->next = NULL;
    }
};

template<typename T>
class List {
public:
    vector<Node<T>*> array;
    Node<T>* head;

    List() {
        this->head = NULL;
    }
    // Intialize list of size n
    List(int n) {
        this->head = NULL;
        for (int i = 0; i < n; i++) {
            this->addNode(T());
        }
    }

    void addNode(T data) {
        Node<T>* temp = new Node<T>(data);
        if (this->head == NULL) {
            this->head = temp;
            array.push_back(temp);
        }
        else {
            Node<T>* curr = this->head;
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = temp;
            array.push_back(temp);
        }
    }
    void print() {
        Node<T>* curr = this->head;
        while (curr != NULL) {
            cout << curr->value << " ";
            curr = curr->next;
        }
        cout << endl;
    }
    int size() {
        return array.size();
    }

};


//Operator overloading
template<typename T>
ostream& operator << (ostream& stream, List<T>& A) {
    A.print();
    return stream;
}

template<typename T>
List<T> sequentialPrefixSum(const List<T>& A) {
    List<T> prefixSum;
    T sum = 0;
    for (int i = 0; i < A.size(); i++) {
        sum += A.array[i]->value;
        prefixSum.addNode(sum);
    }
    return prefixSum;
}

template<typename T, typename F1, typename F2>
List<T> prefix_sum(List<T> A, int n, F1 f1, F2 f2) {
    int log2n = log2(n);

    vector<T> M(2*n);
    vector<T> R(2*n);

    // Declaring offsets
    int offsets[log2n + 2];
    int offset = n << 1;
    for (int i = 0; i <= log2n + 1; i++) {
        offsets[i] = offset >> i;
    }

    // Bottom-up phase
    M[0] = T();
    #pragma omp parallel for schedule(static, 1024)
    for (int i = n; i < 2*n; i++) {
        M[i] = f1(A.array[i-n]->value);
    }

    for (int r = 1; r <= log2n; r++) {
        #pragma omp parallel for schedule(static, 1024)
        for (int i = offsets[r + 1]; i < offsets[r]; i += 2) {
            M[i] = f2(M[2 * i], M[2 * i + 1]);
            M[i + 1] = f2(M[2 * (i + 1)], M[2 * (i + 1) + 1]);
        }
    }

    R[0] = R[1] = T();
    int m = log2n + 1;
    for (int r = 2; r <= log2n + 1; r++) {
        #pragma omp parallel for schedule(static, 1024)
        for (int i = offsets[m]; i < offsets[m - 1]; i++) {
            R[2*i] = R[i];
            R[2*i + 1] = f2(R[i], M[2*i]);
        }
        m--;
    }

    List<T> prefixSum(n);
    #pragma omp parallel for schedule(static, 1024)
    for (int i = 0; i < n; i++) {
        prefixSum.array[i]->value = f2(R[i + n], A.array[i]->value);
    }

    return prefixSum;
}

template <typename T = int>
void compact_array(int n) {
    // Initialize array A with random values
    List<T> A;
    // for (int i = 0; i < n; i++) {
    //     A.addNode(rand() % 2 == 0 ? "0" : "-1");
    // }
    // for (int i = 0; i < n; i++) {
    //     if (A.array[i]->value != "-1") {
    //         A.array[i]->value = "str" + to_string(rand() % 10 + 1);
    //     }
    // }
    for (int i = 0; i < n; i++) {
        A.addNode(rand() % 2 - 1);
    }
    for (int i = 0; i < n; i++) {
        if (A.array[i]->value != -1) {
            A.array[i]->value = rand() % 10 + 1;
        }
    }

    auto f1 = [](int x) -> int { return x; };
    auto f2 = [](int x, int y) -> int { return x + y; };

    if (n <= 256) {
        cout << "Array A: " << A;
    }
    List<int> B(n);

    // compact the array A
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(static, 1024)
    for (int i = 0; i < n; i++) {
        B.array[i]->value = (A.array[i]->value != -1) ? 1 : 0;
    }
    List<int> C = prefix_sum(B, n, f1, f2);
    List<T> D(C.array[n - 1]->value);

    #pragma omp parallel for schedule(static, 1024)
    for (int i = 0; i < n; i++) {
        if (B.array[i]->value == 1) {
            D.array[C.array[i]->value - 1]->value = A.array[i]->value;
        }
    }
    end_time = omp_get_wtime();

    if (n <= 256) {
        cout << "Array D: " << D;
    }
    cout << "Time taken: " << end_time - start_time << " s" << endl;
    cout << "Length of array D: " << D.size() << endl;
}

template<typename T>
void doprefixsum(int n, int option) {
    List<T> A;
    if (option == 1) {
        for (int i = 0; i < n; i++) {
            A.addNode(T(1));
        }
    } else if (option == 2) {
        for (int i = 0; i < n; i++) {
            A.addNode(T(i + 1));
        }
    } else if (option == 3) {
        for (int i = 0; i < n; i++) {
            A.addNode(T(rand() % 20));
        }
    // } else 
    // if (option == 4) {
    //     for (int i = 0; i < n; i++) {
    //         A.addNode(to_string(i+1));
    //     }
    } else {
        cout << "Invalid option" << endl;
        return;
    }

    if (n < 100) {
        cout << "A : " << A;
    }

    auto f1 = [](int x) -> int { return x; };
    auto f2 = [](int x, int y) -> int { return x + y; };

    double start = omp_get_wtime();
    List<T> B = prefix_sum(A, n, f1, f2);
    double end = omp_get_wtime();

    if (n < 100) {
        cout << "B : " << B;
    }
    cout << "Time taken: " << end - start << endl;
    cout << "Last element of B: " << B.array[n-1]->value << endl;
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

    cout << "n = " << n << endl;
    compact_array(n);
    doprefixsum<int>(n, 1);

    return 0;
}
