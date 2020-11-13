#include "lists.h"
ListSem::ListSem(): head(0), tail(0) {

}

ListSem::~ListSem() {
    NodeSem *cur = 0;
    while (head) {
        cur = head;
        head = head->next;
        delete cur;
    }
}

void ListSem::insert(KernelSem *sem) {
    NodeSem *newNode = new NodeSem();
    newNode->sem = sem;
    newNode->next = 0;
    if (head)
        tail->next = newNode;
    else
        head = newNode;
    tail = newNode;
}

void ListSem::remove(KernelSem *target) {
    NodeSem *cur = head;
    NodeSem *prev = 0;
    for (; cur; prev = cur, cur = cur->next)
        if (cur->sem == target)
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

KernelSem *ListSem::get() {
    if (!head)
        return 0;
    NodeSem *first = head;
    head = head->next;
    if (!head)
        tail = 0;
    KernelSem *res = first->sem;
    delete first;
    return res;
}



