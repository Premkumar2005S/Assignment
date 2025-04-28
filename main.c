// Name: Premkumar S
// Roll No: ME24B1021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMELEN 20
#define QSIZE 10
#define SSIZE 10
#define LSIZE 5

typedef struct {
    char name[NAMELEN];
} Part;

typedef struct {
    Part parts[QSIZE];
    int start, end, count;
} Queue;

typedef struct {
    Part parts[SSIZE];
    int top;
} Stack;

typedef struct {
    char action[NAMELEN];
    int time;
} Log;

typedef struct Leak {
    Part part;
    struct Leak* next;
} Leak;

typedef struct Fix {
    Part part;
    struct Fix* next;
    struct Fix* prev;
} Fix;

typedef struct Prio {
    Part part;
    struct Prio* next;
} Prio;

Queue queue;
Stack stack;
Log logs[LSIZE];
int logCount = 0;
Leak* leakList = NULL;
Fix* fixList = NULL;
Prio* prioList = NULL;

void initQueue(Queue* queue) {
    queue->start = 0;
    queue->end = -1;
    queue->count = 0;
}

int isQueueEmpty(Queue* queue) {
    if (queue->count == 0) return 1;
    else return 0;
}

int isQueueFull(Queue* queue) {
    if (queue->count == QSIZE) return 1;
    else return 0;
}

void enqueue(Queue* queue, Part part) {
    if (isQueueFull(queue)) {
        printf("Queue full!\n");
        return;
    }
    queue->end = (queue->end + 1) % QSIZE;
    queue->parts[queue->end] = part;
    queue->count++;
    printf("Enqueued %s\n", part.name); // Added part to queue
}

Part dequeue(Queue* queue) {
    Part empty = {""};
    if (isQueueEmpty(queue)) {
        printf("Queue empty!\n");
        return empty;
    }
    Part temp = queue->parts[queue->start];
    queue->start = (queue->start + 1) % QSIZE;
    queue->count--;
    printf("Dequeued %s\n", temp.name); // Removed part from queue
    return temp;
}

void initStack(Stack* stack) {
    stack->top = -1;
}

int isStackEmpty(Stack* stack) {
    if (stack->top == -1) return 1;
    else return 0;
}

int isStackFull(Stack* stack) {
    if (stack->top == SSIZE - 1) return 1;
    else return 0;
}

void push(Stack* stack, Part part) {
    if (isStackFull(stack)) {
        printf("Stack full!\n");
        return;
    }
    stack->parts[++(stack->top)] = part;
    printf("Pushed %s\n", part.name); // Part pushed onto stack
}

Part pop(Stack* stack) {
    Part empty = {""};
    if (isStackEmpty(stack)) {
        printf("Stack empty!\n");
        return empty;
    }
    Part temp = stack->parts[(stack->top)--];
    printf("Popped %s\n", temp.name); // Part removed from stack
    return temp;
}

void addLog(char* action) {
    if (logCount >= LSIZE) {
        printf("Log full, removed %s\n", logs[0].action);
        for (int i = 0; i < LSIZE - 1; i++) {
            logs[i] = logs[i + 1];
        }
        strcpy(logs[LSIZE - 1].action, action);
        logs[LSIZE - 1].time = logCount;
    } else {
        strcpy(logs[logCount].action, action);
        logs[logCount].time = logCount;
    }
    logCount++;
    printf("Logged %s\n", action); // Log action added
}

void showLogs() {
    printf("\nLogs:\n");
    int first = 0;
    int total = logCount;
    if (logCount > LSIZE) {
        first = logCount % LSIZE;
        total = LSIZE;
    }
    for (int i = 0; i < total; i++) {
        int pos = (first + i) % LSIZE;
        printf("%d: %s at %d\n", i, logs[pos].action, logs[pos].time); // Displaying logs
    }
}

void addLeak(Part part) {
    Leak* node = malloc(sizeof(Leak));
    if (!node) {
        printf("No memory!\n");
        return;
    }
    node->part = part;
    node->next = leakList;
    leakList = node;
    printf("Leak added %s\n", part.name); // Added leak to the list
}

void removeLeak(char* name) {
    Leak* curr = leakList;
    Leak* prev = NULL;
    while (curr) {
        if (strcmp(curr->part.name, name) == 0) {
            if (prev == NULL) {
                leakList = curr->next;
            } else {
                prev->next = curr->next;
            }
            printf("Leak removed %s\n", name); // Leak removed
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("%s not found\n", name); // Leak not found
}

void showLeaks() {
    printf("\nLeaks:\n");
    Leak* curr = leakList;
    if (!curr) {
        printf("None!\n");
        return;
    }
    while (curr) {
        printf("Leak: %s\n", curr->part.name); // Showing all leaks
        curr = curr->next;
    }
}

void addFix(Part part) {
    Fix* node = malloc(sizeof(Fix));
    if (!node) {
        printf("No memory!\n");
        return;
    }
    node->part = part;
    node->next = fixList;
    node->prev = NULL;
    if (fixList) {
        fixList->prev = node;
    }
    fixList = node;
    printf("Fixed added %s\n", part.name); // Fixed part added to the list
}

void showFixFwd() {
    printf("\nFixed (front to back):\n");
    Fix* curr = fixList;
    if (!curr) {
        printf("None!\n");
        return;
    }
    while (curr) {
        printf("Fixed: %s\n", curr->part.name); // Showing fixed parts (forward)
        curr = curr->next;
    }
}

void showFixBwd() {
    printf("\nFixed (back to front):\n");
    Fix* curr = fixList;
    if (!curr) {
        printf("None!\n");
        return;
    }
    while (curr->next) {
        curr = curr->next;
    }
    while (curr) {
        printf("Fixed: %s\n", curr->part.name); // Showing fixed parts (backward)
        curr = curr->prev;
    }
}

void addPrio(Part part) {
    Prio* node = malloc(sizeof(Prio));
    if (!node) {
        printf("No memory!\n");
        return;
    }
    node->part = part;
    if (!prioList) {
        prioList = node;
        node->next = prioList;
    } else {
        Prio* temp = prioList;
        while (temp->next != prioList) {
            temp = temp->next;
        }
        temp->next = node;
        node->next = prioList;
    }
    printf("Priority added %s\n", part.name); // Added priority to list
}

void showPrio(int times) {
    printf("\nPriority parts:\n");
    if (!prioList) {
        printf("None!\n");
        return;
    }
    Prio* curr = prioList;
    int count = 0;
    do {
        printf("Priority %d: %s\n", count, curr->part.name); // Displaying priority parts
        curr = curr->next;
        count++;
    } while (curr != prioList && count < times * 100);
}

void testQueueStack() {
    printf("\nTesting queue and stack...\n");
    initQueue(&queue);
    initStack(&stack);
    Part items[6] = {{"Pump"}, {"Pipe"}, {"Valve"}, {"Sensor"}, {"Sprinkler"}, {"Filter"}};
    for (int i = 0; i < 6; i++) {
        enqueue(&queue, items[i]);
    }
    while (!isQueueEmpty(&queue)) {
        Part temp = dequeue(&queue);
        push(&stack, temp);
    }
    while (!isStackEmpty(&stack)) {
        pop(&stack); // Removed from stack
    }
}

void testLogs() {
    printf("\nTesting logs...\n");
    addLog("Morning");
    addLog("Noon");
    addLog("Evening");
    addLog("Night");
    addLog("Test1");
    addLog("Test2");
    showLogs(); // Displaying logs
}

void testParts() {
    printf("\nTesting parts...\n");
    Part p1 = {"Pipe"};
    Part p2 = {"Sprinkler"};
    addLeak(p1);
    addLeak(p2);
    showLeaks();
    removeLeak("Pipe");
    addFix(p1);
    showLeaks();
    showFixFwd();
    showFixBwd();
    Part p3 = {"Pump"};
    Part p4 = {"Valve"};
    addPrio(p3);
    addPrio(p4);
    showPrio(2); // Displaying priority parts
}

void main() {
    printf("Starting system...\n");
    testQueueStack();
    testLogs();
    testParts();
    printf("\nDone!\n"); // End of the process
}
