#ifndef LIST_H_
#define LIST_H_
#include "pcb.h"
class List {
    public:
        List();
        ~List();
        void insert(PCB *pcb);
        void remove(PCB *target);
        PCB *get();

        friend class PCB;
        struct Node {
            PCB  *pcb;
            Node *next;
        };
        Node *head;
        Node *tail;
    };



#endif /* LIST_H_ */
