#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

class Node {
public:
    int data;
    int priority;
    Node *next;

    Node(int d, int p) {
        data = d;
        priority = p;
        next = NULL;
    }
};

Node* newNode(int x, int p) {
    Node *node = new Node(x, p);
    return node;
}

Node *head = NULL;
void push(int x, int p) {

    Node *node = newNode(x, p);


    if (head == NULL) {
        head = node;
        return;
    }

    Node *temp = head;
    Node *prev = NULL;


    while (temp != NULL && temp->priority > p) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {

        prev->next = node;
    } else {
        if (prev == NULL) {

            node->next = head;
            head = node;
        } else {

            node->next = temp;
            prev->next = node;
        }
    }
}
int extractmin() {

    if (head != NULL) {
        int data = head->data;

        head = head->next;
        return data;
    }
    return -1;
}
int peek() {
    if (head != NULL) {
        return head->data;
    }
    return -1;
}
int main() {


    auto start = high_resolution_clock::now();
    push(5, 2);
    auto t1=high_resolution_clock::now();
    auto d1=duration_cast<microseconds>(t1-start);
    cout << "Time taken by function: "
         << d1.count() << " microseconds" << endl;
    push(1, 3);
    auto t2=high_resolution_clock::now();
    auto d2=duration_cast<microseconds>(t2-start);
    cout << "Time taken by function: "
         << d2.count() << " microseconds" << endl;
    cout<<peek()<<endl;
    auto t3=high_resolution_clock::now();
    auto d3=duration_cast<microseconds>(t3-start);
    cout << "Time taken by function: "
         << d3.count() << " microseconds" << endl;
    push(7, 5);
    auto t4=high_resolution_clock::now();
    auto d4=duration_cast<microseconds>(t4-start);
    cout << "Time taken by function: "
         << d4.count() << " microseconds" << endl;
    push(9, 1);
    cout<<extractmin()<<endl;
    cout<<extractmin()<<endl;
    cout<<peek()<<endl;
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;
}
