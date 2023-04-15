#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <mutex>
using namespace std;


class Node {
public:
    int data;
    Node* next;

    Node(int data) {
        this->data = data;
        this->next = NULL;
    }
};

class LinkedList {
public:
    Node* head;
    LinkedList() {
        this->head = NULL;
    }

    void print() {
        Node* temp = this->head;
        while (temp != NULL) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
    int size() {
        int size = 0;
        Node* temp = this->head;
        while (temp != NULL) {
            size++;
            temp = temp->next;
        }
        return size;
    }
    void insert(int data) ;
    void deleteNode(int data) ;
    bool find(int data);
};

void LinkedList::insert(int data) {
    Node* new_node = new Node(data);
    Node* current = head;
    Node* previous = NULL;

    // If the list is empty or value is less than the head, insert at the beginning
    if (head == NULL || head->data >= data) {
        #pragma omp atomic capture
        {
            new_node->next = head;
            head = new_node;
        }
        return;
    }

    // Find the position to insert the new node
    #pragma omp parallel shared(current, previous)
    {
        #pragma omp critical 
        {
            while (current != NULL && current->data < data) {
                previous = current;
                current = current->next;
            }
                new_node->next = current;
                previous->next = new_node;
        }
    }
}


int main(int argc, char* argv[]){
    int n, n_t;
    // read in values of n and n_t
    if (argc < 3) {
        printf("Enter the size of the arrays: ");
        scanf("%d", &n);
        printf("Enter the number of threads: ");
        scanf("%d", &n_t);
    } 
    else {
        n = atoi(argv[1]);
        n_t = atoi(argv[2]);
    }
    omp_set_num_threads(n_t);

    LinkedList* list = new LinkedList();
    list->insert(0);
    list->insert(1);
    list->insert(2);
    list->insert(3);
    list->insert(4);

    #pragma omp parallel for 
    for (int i = 5; i < n; i++) {
        list->insert(i);
    }
    
    bool condition = list->size() <= 128;
    if (condition) {
        list->print();
    }
    else {
        Node *temp = list->head;
        for (int i = 0; i < 64 ; i++) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
    }
    printf("Size after insert: %d\n", list->size());

    
    return 0;
}