
#ifndef LISTS_H_
#define LISTS_H_
#include "kernels.h"
class ListSem {
    public:
        ListSem();
        ~ListSem();
        void insert(KernelSem *sem);
        void remove(KernelSem *target);
        KernelSem *get();
        friend class KernelSem;
        struct NodeSem {
        	KernelSem  *sem;
            NodeSem *next;
        };
        NodeSem *head;
        NodeSem *tail;
    };






#endif /* LISTS_H_ */
