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
        for (auto i : array) {
            cout << i->value << " ";
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
    while (1)
    {
        bool flag = false;
        #pragma omp parallel shared(A, flag)
        {
            vector<Node<T>*> nextnext(n);
            vector<T> listval(n);
            
            #pragma omp for //schedule(static, 1024)
            for (int i = 0; i < n; i++) {
                if (A.array[i]->next != NULL ) {
                    nextnext[i] = A.array[i]->next->next;
                    listval[i] = f2(A.array[i]->value, A.array[i]->next->value);
                    flag = true;
                }
                else {
                    nextnext[i] = NULL;
                    listval[i] = A.array[i]->value;
                }
            }
            #pragma omp barrier
            #pragma omp for 
            for (int i = 0; i < n; i++) {
                if (A.array[i]->next != NULL) {
                    A.array[i]->value = listval[i];
                }
                A.array[i]->next = nextnext[i];
            }
        }
        if (!flag) {
            break;
        }
    }
    return A;
}

template <typename T = string>
void compact_array(int n) {
    // Initialize array A with random values
    List<T> A;
    for (int i = 0; i < n; i++) {
        A.addNode(rand() % 2 == 0 ? "0" : "-1");
    }
    for (int i = 0; i < n; i++) {
        if (A.array[i]->value != "-1") {
            A.array[i]->value = "str" + to_string(rand() % 10 + 1);
        }
    }
    // for (int i = 0; i < n; i++) {
    //     A.addNode(rand() % 2 - 1);
    // }
    // for (int i = 0; i < n; i++) {
    //     if (A.array[i]->value != -1) {
    //         A.array[i]->value = rand() % 10 + 1;
    //     }
    // }

    auto f1 = [](int x) -> int { return x; };
    auto f2 = [](int x, int y) -> int { return x + y; };

    if (n <= 256) {
        cout << "Array A: " << A;
    }
    List<int> B(n);
    List<int> C(n);

    // compact the array A
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(static, 1024)
    for (int i = 0; i < n; i++) {
        B.array[n-i-1]->value = (A.array[i]->value != "-1") ? 1 : 0;
        C.array[n-i-1]->value = (A.array[i]->value != "-1") ? 1 : 0;
    }
    C = prefix_sum(C, n, f1, f2);
    List<T> D(C.array[0]->value);

    // cout << "B: " << B << "C: " << C;
    #pragma omp parallel for schedule(static, 1024)
    for (int i = 0; i < n; i++) {
        if (B.array[n-i-1]->value == 1) {
            D.array[C.array[n-i-1]->value - 1]->value = A.array[i]->value;
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
        for (int i = n - 1; i >=0; i--) {
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

    auto f1 = [](T x) -> T { return x; };
    auto f2 = [](T x, T y) -> T {return x + y; };
    // auto f2 = [](T x, T y) -> T { return x < y ? x : y; };

    double start = omp_get_wtime();
    List<T> B = prefix_sum(A, n, f1, f2);
    double end = omp_get_wtime();

    if (n < 100) {
        cout << "B : " << B;
    }
    cout << "Time taken: " << end - start << endl;
    cout << "Fist element of B: " << B.head->value << endl;
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
    // compact_array(n);
    doprefixsum<int>(n, 1);

    return 0;
}
