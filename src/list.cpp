#include "list.h"
List::List(): head(0), tail(0) {

}

List::~List() {
    Node *cur = 0;
    while (head) {
        cur = head;
        head = head->next;
        delete cur;
    }
}

void List::insert(PCB *pcb) {
    Node *newNode = new Node();
    newNode->pcb = pcb;
    newNode->next = 0;
    if (head)
        tail->next = newNode;
    else
        head = newNode;
    tail = newNode;
}

void List::remove(PCB *target) {
    Node *cur = head;
    Node *prev = 0;
    for (; cur; prev = cur, cur = cur->next)
        if (cur->pcb == target)
            break;
    if (!cur)
        return;
    if (!prev) // if (cur == head)
        head = cur->next;
    else
        prev->next = cur->next;
    if (!cur->next)
        tail = prev;
    delete cur;
}

PCB *List::get() {
    if (!head)
        return 0;
    Node *first = head;
    head = head->next;
    if (!head)
        tail = 0;
    PCB *res = first->pcb;
    delete first;
    return res;
}


