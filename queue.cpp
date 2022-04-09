#include <cstring>
#include <cstdio>
struct ListNode {
    int data;
    ListNode* ptr; // XOR of next and previous pointer
};

struct List{
    ListNode* start = nullptr;
    ListNode* end = nullptr;
};

struct Queue{
    List* list = new List();
    ListNode* postFront = nullptr;
    ListNode* front = nullptr;
    ListNode* preBack = nullptr;
    ListNode* back = nullptr;
    bool isEmpty = true;
};

ListNode* getPointer(ListNode* left, ListNode* right){
    return (ListNode*)((uintptr_t)left^(uintptr_t)right);
}

// Resets empty list
void clear(List* list) {
    list->start = nullptr;
    list->end = nullptr;
}

void printList(ListNode* head){
    ListNode* current = head;
    ListNode* previous = nullptr;
    while(current != nullptr){
        printf("%d ", current->data);
        ListNode* tmp = getPointer(previous, current->ptr);
        previous = current;
        current = tmp;
    }
    printf("\n");
}

void pushBegin(List* list, int value){
    ListNode* first = new ListNode();
    first->data = value;
    first->ptr = getPointer(nullptr, list->start);
    if(list->start != nullptr){
        list->start->ptr = getPointer(first, getPointer(nullptr, list->start->ptr));
    }
    list->start = first;
    if(list->end == nullptr) list->end = first;
}

void pushBegin(Queue* queue, int value){
    bool isFirst = false;
    if(queue->back !=nullptr && queue->preBack == nullptr){
        isFirst = true;
    }
    pushBegin(queue->list, value);
    if(isFirst){
        queue->preBack = queue->list->start;
    }
}

void deleteBegin(List* list){
    if(list->start == nullptr)return;
    ListNode* node = list->start;
    ListNode* next = getPointer(nullptr, node->ptr);
    if(next == nullptr){
        delete node;
        clear(list);
        return;
    }
    ListNode* next2 = getPointer(node, next->ptr);
    next->ptr = getPointer(nullptr, next2);
    list->start = next;
    delete node;

}

void deleteBegin(Queue* queue){
    if(queue->list->start == nullptr)return;
    if(queue->front == queue->list->start){
        queue->front = queue->list->end;
        queue->postFront = nullptr;
    }
    if(queue->preBack == nullptr){
        ListNode* node = getPointer(nullptr, queue->back->ptr);
        queue->back = node;
        queue->preBack = nullptr;
    }
    deleteBegin(queue->list);
}

// Memory cleanup
void dispose(List* list){
    while(list->start != nullptr){
        deleteBegin(list);
    }
}

void pushBack(List* list, int value){
    ListNode* last = new ListNode();
    last->data = value;
    last->ptr = getPointer(list->end, nullptr);
    if(list->end != nullptr){
        list->end->ptr = getPointer(last, getPointer(nullptr, list->end->ptr));
    }
    list->end = last;
    if(list->start == nullptr) list->start = last;
}

void pushBack(Queue* queue, int value){
    bool isLast = false;
    if(queue->front != nullptr && queue->postFront == nullptr){
        isLast = true;
    }
    pushBack(queue->list, value);
    if(isLast){
        queue->postFront = queue->list->end;
    }
}

void deleteEnd(List* list){
    if(list->end == nullptr)return;
    ListNode* node = list->end;
    ListNode* prev = getPointer(node->ptr, nullptr);
    if(prev == nullptr){
        delete node;
        clear(list);
        return;
    }
    ListNode* prev2 = getPointer(node, prev->ptr);
    delete node;
    prev->ptr = getPointer(nullptr, prev2);
    list->end = prev;

}

void deleteEnd(Queue* queue){
    if(queue->list->end == nullptr)return;
    if(queue->back == queue->list->end){
        queue->back = queue->list->start;
        queue->preBack = nullptr;
    }
    if(queue->postFront == nullptr){
        ListNode* node = getPointer(queue->front->ptr, nullptr);
        queue->front = node;
        queue->postFront = nullptr;
    }
    deleteEnd(queue->list);
}

void pushToQueue(Queue* queue, int value){
    queue->isEmpty = false;

    if(queue->back == nullptr){
        if(queue->list->end == nullptr){
            pushBack(queue->list, value);
        }

        queue->back = queue->list->end;
        queue->front = queue->list->end;
        queue->preBack = getPointer(queue->list->end->ptr, nullptr);
        queue->postFront = nullptr;
        queue->back->data = value;
        return;
    }

    if(queue->preBack == nullptr){
        if(queue->front == queue->list->end){
            pushBegin(queue->list, value);
            queue->back = queue->list->start;
            queue->preBack = nullptr;
        }else{
            queue->back = queue->list->end;
            queue->back->data = value;
            queue->preBack = getPointer(queue->back->ptr, nullptr);
        }
        return;
    }

    if(queue->preBack != queue->front){
        ListNode* preBack2 = getPointer(queue->preBack->ptr, queue->back);
        queue->preBack->data = value;
        queue->back = queue->preBack;
        queue->preBack = preBack2;
        return;
    }

    ListNode* node = new ListNode();
    node->data = value;
    node->ptr = getPointer(queue->preBack, queue->back);

    ListNode* preBack2 = getPointer(queue->preBack->ptr, queue->back);
    ListNode* postBack = getPointer(queue->preBack, queue->back->ptr);

    queue->preBack->ptr = getPointer(preBack2, node);
    queue->back->ptr = getPointer(node, postBack);

    if(queue->postFront == queue->back){
        queue->postFront = node;
    }

    queue->back = node;
}

void pop(Queue* queue){
    if(queue->front == nullptr || queue->isEmpty) return;

    if(queue->front == queue->back){
        queue->isEmpty = true;
        return;
    }

    if(queue->front == queue->list->start){
        queue->postFront = nullptr;
        queue->front = queue->list->end;
        return;
    }

    ListNode* preFront = getPointer(queue->front->ptr, queue->postFront);
    queue->postFront = queue->front;
    queue->front = preFront;
}

void printFront(Queue* queue){
    if(queue->front == nullptr || queue->isEmpty){
        printf("NULL\n");
        return;
    }
    printf("%i\n", queue->front->data);
}

void printQueue(Queue* queue){
    if(queue->front == nullptr || queue->back == nullptr || queue->isEmpty){
        printf("NULL\n");
        return;
    }
    if(queue->front == queue->back){
        printf("%i\n", queue->front->data);
        return;
    }
    ListNode* current = queue->front;
    ListNode* previous = queue->postFront;
    do{
        printf("%i ", current->data);
        ListNode* next = getPointer(previous, current->ptr);
        previous = current;
        current = next;
        if(current == nullptr){
            current = queue->list->end;
            previous = nullptr;
        }
    }while(current!=queue->back);
    printf("%i\n", queue->back->data);
}

int getQueueSize(Queue* queue){
    if(queue->front == nullptr || queue->back == nullptr || queue->isEmpty){
        return 0;
    }
    if(queue->front == queue->back){
        return 1;
    }
    ListNode* current = queue->front;
    ListNode* previous = queue->postFront;
    int count = 1;
    do{
        count++;
        ListNode* next = getPointer(previous, current->ptr);
        previous = current;
        current = next;
        if(current == nullptr){
            current = queue->list->end;
            previous = nullptr;
        }
    }while(current!=queue->back);
    return count;
}

void softCleanup(Queue* queue){
    if(queue->front == nullptr || queue->back == nullptr || queue->isEmpty){
        ListNode* current = queue->list->start;
        ListNode* previous = nullptr;
        while(current != nullptr){
            current->data = 0;
            ListNode* next = getPointer(previous, current->ptr);
            previous = current;
            current = next;
        }
        return;
    }
    if(queue->front == queue->back){
        return;
    }
    ListNode* current = queue->preBack;
    ListNode* previous = queue->back;
    do{
        current->data = 0;
        ListNode* next = getPointer(previous, current->ptr);
        previous = current;
        current = next;
        if(current == nullptr){
            current = queue->list->end;
            previous = nullptr;
        }
    }while(current!=queue->front);
}

void hardCleanup(Queue* queue){
    if(queue->front == nullptr || queue->back == nullptr || queue->isEmpty){
        dispose(queue->list);
        clear(queue->list);
    }
    if(queue->front == queue->back){
        return;
    }
    ListNode* current = queue->preBack;
    ListNode* previous = queue->back;
    bool hasLooped = false;

    do{
        ListNode* next = getPointer(previous, current->ptr);
        if(previous != queue->back){
            delete previous;
        }
        previous = current;
        current = next;
        if(current == queue->front){
            ListNode* preFront = getPointer(queue->front->ptr, queue->postFront);
            delete queue->postFront;
            if(hasLooped){
                queue->postFront = nullptr;
                queue->front->ptr = getPointer(preFront, nullptr);
            }else{
                queue->front->ptr = getPointer(preFront, queue->back);
                queue->postFront = queue->back;
                ListNode* postBack = getPointer(queue->preBack, queue->back->ptr);
                queue->back->ptr = getPointer(queue->front, postBack);
                queue->preBack = queue->front;
            }
            return;
        }
        if(current == nullptr){
            ListNode* postBack = getPointer(queue->preBack, queue->back->ptr);
            delete queue->preBack;
            queue->preBack = nullptr;
            queue->back->ptr = getPointer(nullptr, postBack);
            current = queue->list->end;
            previous = nullptr;
            hasLooped = true;
        }
    }while(current!=queue->front);
}

int getListSize(ListNode* head){
    ListNode* current = head;
    ListNode* previous = nullptr;
    int count = 0;
    while(current != nullptr){
        count++;
        ListNode* tmp = getPointer(previous, current->ptr);
        previous = current;
        current = tmp;
    }
    return count;
}

void resolveInput(const char* input, Queue* queue){
    if(strcmp("ADD_BEG", input) == 0){
        int n;
        scanf("%i", &n);
        pushBegin(queue, n);
    } else if(strcmp("ADD_END",input) == 0){
        int n;
        scanf("%i", &n);
        pushBack(queue,n);
    } else if(strcmp("DEL_BEG",input) == 0){
        deleteBegin(queue);
    } else if(strcmp("DEL_END",input) == 0){
        deleteEnd(queue);
    } else if(strcmp("PRINT_FORWARD",input) == 0){
        if(queue->list->start == nullptr)
            printf("NULL\n");
        else
            printList(queue->list->start);
    } else if(strcmp("PRINT_BACKWARD",input) == 0){
        if(queue->list->end == nullptr)
            printf("NULL\n");
        else
            printList(queue->list->end);
    } else if(strcmp("PUSH",input) == 0){
        int n;
        scanf("%i", &n);
        pushToQueue(queue, n);
    } else if(strcmp("POP",input) == 0){
        printFront(queue);
        pop(queue);
    } else if(strcmp("PRINT_QUEUE",input) == 0){
        printQueue(queue);
    } else if(strcmp("SIZE",input) == 0){
        printf("%i\n", getListSize(queue->list->start));
    } else if(strcmp("COUNT",input) == 0){
        printf("%i\n", getQueueSize(queue));
    } else if(strcmp("GARBAGE_SOFT",input) == 0){
        softCleanup(queue);
    } else if(strcmp("GARBAGE_HARD",input) == 0){
        hardCleanup(queue);
    }else{
        printf("%25s\n", input);
    }
}

int main() {

    char input[25];
    Queue* queue = new Queue();
    while(scanf("%24s", input) != EOF){
        resolveInput(input, queue);
    }

    dispose(queue->list);
    delete queue;

    return 0;
}
