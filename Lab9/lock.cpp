#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
using namespace std;


//Node with lock
class Node {
    public:
        int data;
        Node* next;
        //lock
        omp_lock_t lck;
        Node(int data) {
            this->data = data;
            this->next = NULL;
            omp_init_lock (&this->lck);
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
        new_node->next = head;
        head = new_node;
        return;
    }

    // Lock the head node before traversal
    omp_set_lock(&current->lck);

    // Traverse the linked list until we find the correct position to insert the new node
    while (current != NULL && current->data < data) {
        if (previous != NULL) {
            omp_unset_lock(&previous->lck);
        }
        previous = current;
        current = current->next;
        if (current != NULL) {
            omp_set_lock(&current->lck);
        }
    }

    // Insert the new node at the correct position in the list
    if (previous == NULL) {
        new_node->next = head;
        head = new_node;
    }
    else {
        new_node->next = current;
        previous->next = new_node;
    }

    // Unlock all locked nodes
    if (previous != NULL) {
        omp_unset_lock(&previous->lck);
    }
    if (current != NULL) {
        omp_unset_lock(&current->lck);
    }
    // omp_unset_lock(&head->lck);
}


void LinkedList::deleteNode(int data) {
    Node* current = head;
    Node* previous = NULL;
    omp_set_lock(&current->lck);

    // Iterate through the list to find the node to delete
    while (current != NULL) {
        if (current->data == data) {
            if (previous == NULL && head == current) {
                head = current->next;
            } else {
                if (previous->next == current)
                    previous->next = current->next;
            }

            // Unlock the current node after deleting it
            omp_unset_lock(&current->lck);
            omp_destroy_lock(&current->lck);
            delete current;
            // Unlock the previous node 
            if (previous != NULL) {
                omp_unset_lock(&previous->lck);
            }
            return;
        }
        if (current->next != NULL) {
            omp_set_lock(&current->next->lck);
        }
        if (previous != NULL) {
            omp_unset_lock(&previous->lck);
        }
        previous = current;
        current = current->next;
    }
    // Unlock the previous node
    if (previous != NULL) {
        omp_unset_lock(&previous->lck);
    }
    return;
}

// Check if node is present or not
bool LinkedList::find(int data) {
    Node* current = head;
    omp_set_lock(&current->lck);

    // Iterate through the list to find the node
    while (current != NULL) {
        if (current->data == data) {
            // printf("Found %d\n", data);
            // Unlock the current node
            omp_unset_lock(&current->lck);
            return true;
        }
        if (current->next != NULL) {
            omp_set_lock(&current->next->lck);
        }
        omp_unset_lock(&current->lck);
        current = current->next;
    }
    // printf("Not found %d\n", data);
    return false;
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
    printf("Size after insert: %d\n", list->size());

    #pragma omp parallel for
    for (int i = n/4; i < 6*n/4; i++) 
    {
        // int data = rand() % (n - 20) + 10;
        // printf("Deleting %d\n", data);
        // list->deleteNode(data);

        list->deleteNode(i);
    }
    if (condition) {
        list->print();
    }
    printf("Size after deletion: %d\n", list->size());

    int found = 0, not_found = 0;
    // Find numbers
    #pragma omp parallel for shared(found, not_found)
    for (int i = 0; i < n; i++) {
        if (list->find(i)) {
            #pragma omp atomic
            found++;
        }
        else {
            #pragma omp atomic
            not_found++;
        }
    }
    printf("Found: %d, Not found: %d\n", found, not_found);

    return 0;
}