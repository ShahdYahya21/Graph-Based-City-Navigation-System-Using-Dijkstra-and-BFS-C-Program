#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define maxLength 100
//struct to save the distenation between each 2 cities and the destination of each city
struct edge {
    int weight;
    char dest[maxLength];
};

//struct for the vertex in the graph contains array of edges that points to other vertices
struct vertex {
    char city[maxLength];
    int dist;
    struct vertex* next;
    struct edge** edges;
    int edgeIdx;
};

//struct of a graph that contains an array of vertices
struct Graph {
    int v;
    struct vertex** array;
};
struct vertex* createNode(char city[maxLength]);
struct Graph* createGraph(int V, char cities[V][maxLength]);
void printGraph(struct Graph* graph);
void addEdge(struct Graph* graph, char src[maxLength], char dest[maxLength], int weight);

// Function to create a vertex with the given city name
struct vertex* createNode(char city[maxLength]) {
    struct vertex* newNode = (struct vertex*)malloc(sizeof(struct vertex));
    strcpy(newNode->city, city);
    newNode->edges = (struct edge**)malloc(100 * sizeof(struct edge*));
    newNode->dist = 0;
    newNode->edgeIdx = 0;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with the given city array
struct Graph* createGraph(int V, char cities[V][maxLength]) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->array = (struct vertex**)malloc(V * sizeof(struct vertex*));
    graph->v = V;

    for (int i = 0; i < V; i++) {
        graph->array[i] = createNode(cities[i]);
        graph->array[i]->next = NULL;
    }


    return graph;
}
// Function to add edges to the graph and connect the dest city to the src city
void addEdge(struct Graph* graph, char src[maxLength], char dest[maxLength], int weight) {
    int j = 0;
    while (strcmp(graph->array[j]->city, dest) != 0) {
        j++;
    }

    int i = 0;
    struct vertex* temp = graph->array[i];
    // Iterate through cities to find the src city first
    while (i < graph->v && strcmp(temp->city, src) != 0) {
        i++;
        temp = graph->array[i];
    }

    int edgeIdx = graph->array[i]->edgeIdx;

    if (edgeIdx >= maxLength) {
        printf("Maximum edge count exceeded.\n");
        return;
    }
    // Add new edge to the src city with the given information
    graph->array[i]->edges[edgeIdx] = (struct edge*)malloc(sizeof(struct edge));
    strcpy(graph->array[i]->edges[edgeIdx]->dest, dest);
    graph->array[i]->edges[edgeIdx]->weight = weight;
    ++graph->array[i]->edgeIdx;

    if (temp == NULL) {
        printf("Source vertex not found.\n");
        return;
    }

    while (temp->next != NULL) {
        temp = temp->next;
    }
    // Connect the new edge to the dest Vertex
    struct vertex* r = createNode(dest);
    temp->next = r;

}


// Function to print the graph, including cities and their adjacent cities
void printGraph(struct Graph* graph) {
    struct vertex* temp;
    // Iterate through the graph array to print the cities and their adjacent cities
    for (int i = 0; i < graph->v; i++) {
        temp = graph->array[i];
        while (temp -> next != NULL) {
            printf("%s -> ", temp->city);
            temp = temp->next;
        }
        printf("%s", temp->city);
        printf("\n");
    }
}


struct MinHeapNode {
    int dist;
    char city[maxLength];
};

struct MinHeap {
    int size;
    int capacity;
    struct MinHeapNode **array;
};
void minHeapify(struct MinHeap *minHeap, int idx);
struct MinHeapNode *newMinHeapNode(int dist, char city[maxLength]);
struct MinHeap *createMinHeap(int capacity);
void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b);
int isEmpty(struct MinHeap *minHeap);
struct MinHeapNode *extractMin(struct MinHeap *minHeap);
void updateHeap(struct MinHeap *minHeap, char city[maxLength], int dist);
void insert(struct MinHeap *minHeap, int dist, char city[maxLength]);


// Heapify the Min Heap to maintain the min-heap property
void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}


// Create a new MinHeapNode with the given distance and city name
struct MinHeapNode *newMinHeapNode(int dist, char city[maxLength]) {
    struct MinHeapNode *minHeapNode = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    minHeapNode->dist = dist;
    strcpy(minHeapNode->city, city);
    return minHeapNode;
}

// Create a new MinHeap with the given capacity
struct MinHeap *createMinHeap(int capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

// Swap two MinHeapNodes
void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b) {
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

// Check if the MinHeap is empty
int isEmpty(struct MinHeap *minHeap) {
    return minHeap->size == 0;
}

// Extract the node with the minimum distance from the MinHeap
struct MinHeapNode *extractMin(struct MinHeap *minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode *root = minHeap->array[0];

    struct MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Update the distance of a node in the MinHeap
void updateHeap(struct MinHeap *minHeap, char city[maxLength], int dist) {
    int i = 0;
    while (i < minHeap->size && strcmp(minHeap->array[i]->city, city) != 0) {
        i++;
    }

    if (i == minHeap->size) {
        return;
    }

    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    if(i == 0) minHeapify(minHeap, 0);

}

// Insert a new node with the given distance and city name into the MinHeap
void insert(struct MinHeap *minHeap, int dist, char city[maxLength]) {
    if (minHeap->size == minHeap->capacity) {
        printf("Overflow: Could not insert more elements\n");
        return;
    }

    struct MinHeapNode *minHeapNode = newMinHeapNode(dist, city);
    minHeap->array[minHeap->size] = minHeapNode;
    minHeap->size++;
    updateHeap(minHeap, city, dist);
}


struct QueueRecord {
    int Capacity;
    int Front;
    int Rear;
    int Size;
    char **Array;
};
typedef struct QueueRecord *Queue;
void MakeEmpty(Queue Q);
int IsEmpty(Queue Q);
int IsEmpty(Queue Q);
Queue CreateQueue(int MaxElements);
void MakeEmpty(Queue Q);
void DisposeQueue(Queue Q);
int Succ(int Value, Queue Q);
void Enqueue(char *X, Queue Q);
char *Front(Queue Q);
void Dequeue(Queue Q);
char *FrontAndDequeue(Queue Q);

// Check if the queue is empty
int IsEmpty(Queue Q) {
    return Q->Size == 0;
}

// Check if the queue is full
int IsFull(Queue Q) {
    return Q->Size == Q->Capacity;
}


// Create a new queue with the given capacity
Queue CreateQueue(int MaxElements) {
    Queue q;
    q = (Queue) malloc(sizeof(struct QueueRecord));
    if (q == NULL) {
        printf("Out of space\n");
        return NULL;
    }
    q->Array = (char**) malloc(sizeof(char*) * MaxElements);
    if (q->Array == NULL) {
        printf("Out of space\n");
        free(q);
        return NULL;
    }
    q->Capacity = MaxElements;
    MakeEmpty(q);
    return q;
}

// Make the queue empty by resetting its properties
void MakeEmpty(Queue Q) {
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

// Dispose of the queue and deallocate memory
void DisposeQueue(Queue Q) {
    if (Q != NULL) {
        free(Q->Array);
        free(Q);
    }
}

// Get the next index after a given value in the queue
int Succ(int Value, Queue Q) {
    if (++Value == Q->Capacity)
        Value = 0;
    return Value;
}

// Enqueue an element into the queue
void Enqueue(char *X, Queue Q) {
    if (IsFull(Q))
        printf("Full Queue\n");
    else {
        Q->Size++;
        Q->Rear = Succ(Q->Rear, Q);
        Q->Array[Q->Rear] = strdup(X);
    }
}

// Get the front element of the queue
char *Front(Queue Q) {
    if (!IsEmpty(Q)) {
        return Q->Array[Q->Front];
    }
    printf("Empty Queue!\n");
    return NULL;
}

// Dequeue an element from the queue
void Dequeue(Queue Q) {
    if (IsEmpty(Q))
        printf("Empty Queue!\n");
    else {
        free(Q->Array[Q->Front]);
        Q->Size--;
        Q->Front = Succ(Q->Front, Q);
    }
}

// Get the front element of the queue and dequeue it
char *FrontAndDequeue(Queue Q) {
    char *X = NULL;
    if (IsEmpty(Q)) {
        printf("Empty Queue!\n");
    } else {
        X = Q->Array[Q->Front];
        Q->Size--;
        Q->Front = Succ(Q->Front, Q);
    }
    return X;
}


// Struct to store information about a city, including its name, parent in the path, distance from the source (src), and distance from its parent
struct city{
    char city[maxLength];
    char parent[maxLength];
    int distFromSrc;
    int distFromParent;

};
void dijkstra(struct Graph* graph, struct city city[],int n, char src[maxLength]);
bool isSelected(char selected[][maxLength], int n, char city[maxLength]);
int findIdx(struct city cities[],int n, char city[maxLength]);
void printPath(struct city city[],int n, char destination[maxLength]);
void BFS_Optimal(struct Graph* graph, struct city city[],int n, char src[maxLength]);
void printPathOnFile(struct city city[],int n, char destination[maxLength], FILE* outputFile);
int extractNumber(char str[maxLength]);
void BFS_Not_Optimal(struct Graph* graph, struct city city[],int n, char src[maxLength],char dest[maxLength]);


int main() {
    int n = 0;
    char src[maxLength];
    char dest[maxLength];
    char dist[maxLength];
    char cites[maxLength][maxLength];// To save all the cities in the file
    int scrCitiesIdx = 0;
    int destCitiesIdx = 0;
    char scrCities[maxLength][maxLength]; // To save all the source cities the user entered to finally print it in the graph
    char destCities[maxLength][maxLength];// To save all the destination cities the user entered to finally print it in the graph

    FILE *ptr;
    ptr = fopen("/Users/shahdyahya/Desktop/datastructure/Project_4/cities.txt", "r");
    if (ptr == NULL) {
        perror("File could not be opened");
        return 1;
    }
    int count = 0;

    while(fscanf(ptr,"%s %s %s",src,dest,dist) != EOF){
        // Check if the city to be added already exists in the graph or not
        if(!isSelected(cites, count, src)){
            strcpy(cites[count],src);
            count++;
        }
        if(!isSelected(cites, count, dest)){
            strcpy(cites[count],dest);
            count++;
        }
    }
    // Create a graph using the array of cities that was just made
    struct Graph* graph = createGraph(count,cites);

    // Create an array of struct city that have the following property(its name, parent in the path, distance from the source (src), and distance from its parent)
    struct city city[count];

    // Fill the array with the cities and initialize the distance from the source of each city with zeros
    for(int i = 0; i < count; i++) {
        strcpy(city[i].city, cites[i]);
        city[i].distFromSrc = 0;
    }
    ptr = fopen("/Users/shahdyahya/Desktop/datastructure/Project_4/cities.txt", "r");
    if (ptr == NULL) {
        printf("File could not be opened.\n");
        return 1;
    }
    // Iterate through a file to add edges to each city to build the graph
    while (fscanf(ptr, "%s %s %s", src, dest, dist) != EOF) {
        // extract the number(distance) from the string dist for example 22km -> 22
        int distenation = extractNumber(dist);
        addEdge(graph, src, dest,distenation);
    }
    printf("Select one of the following:\n1)Load cities: loads the file and construct the graph\n2)Enter source:read the source city\n3)Enter destination: print the full route of the shortest path includinge the distance between each two cities and the total shortest cost for both algorithms (Dijkstra and BFS)\n4)Exit: prints the information of step 3 to a file called ""shortest_distance.txt"" and exits the program the distance between each two cities and the total shortest cost for both algorithms (Dijkstra and BFS)\n");
    int choice,temp = 0;
    scanf("%d",&choice);
    int start = true;
    while(choice != 4){
        if(start == false) {
            printf("\nSelect other option\n");
            scanf("%d", &choice);
        }
        if(choice == 1){
            printf("The adjacency list of the graph: \n\n");
            printGraph(graph);
        }
        else if(choice == 2){
            printf("Enter source city: \n");
            scanf("%s",src);
            //Check if the city the user entered exist in the graph or not
            while(isSelected(cites, count, src) == false){
                printf("This city doesnt exist in the graph\nEnter another city\n");
                scanf("%s",src);
            }
            strcpy(scrCities[scrCitiesIdx],src);
            scrCitiesIdx++;

        }
        else if(choice == 3){
            if(temp == 0) {
                printf("you shoud choose 1 first to see the graph\n");
                start = false;
                continue;
            }
            if(temp != 2){
                printf("You should enter 2 first\n");
                continue;
            }
            printf("Enter destination city: \n");
            scanf("%s",dest);
            //Check if the city the user entered exist in the graph or not
            while(isSelected(cites, count, dest) == false){
                printf("This city doesnt exist in the graph\nEnter another city\n");
                scanf("%s",dest);
            }
            strcpy(destCities[destCitiesIdx],dest);
            destCitiesIdx++;
            printf("The path between %s and %s using Dijkstra\n", src,dest);
            dijkstra(graph, city,count, src);
            printPath(city,count,dest);
            int idx = findIdx(city,count,dest);
            printf("\nThe distance between %s and %s using Dijkstra is %dkm\n", src,dest,city[idx].distFromSrc);

            printf("\n\nThe path between %s and %s using BFS in the optimal way(the actual shortest path)\n", src,dest);
            BFS_Optimal(graph,city,count,src);
            printPath(city,count,dest);
            printf("\nThe distance between %s and %s using BFS in the optimal way(the actual shortest path) is %dkm\n", src,dest,city[idx].distFromSrc);

            printf("\n\nThe path between %s and %s using BFS in the not optimal way\n", src,dest);
            BFS_Not_Optimal(graph,city,count,src,dest);
            printPath(city,count,dest);
            printf("\nThe distance between %s and %s using BFS  in the not optimal way %dkm\n", src,dest,city[idx].distFromSrc);



        }
        else if(choice == 4){
            if(temp == 3) {
                FILE *file = fopen("/Users/shahdyahya/Desktop/datastructure/Project_4/shortest_distance.txt", "a");
                if (file == NULL) {
                    printf("Failed to open output file\n");
                    return 1;
                }
                for (int i = 0; i < scrCitiesIdx; i++) {
                    int idx = findIdx(city, count, destCities[i]);

                    fprintf(file, "The path between %s and %s using Dijkstra\n", scrCities[i], destCities[i]);
                    dijkstra(graph, city, count, scrCities[i]);
                    printPathOnFile(city, count, destCities[i], file);
                    fprintf(file, "\n\nThe distance between %s and %s using Dijkstra is %dkm\n", scrCities[i], destCities[i],
                            city[idx].distFromSrc);

                    fprintf(file, "\n\nThe path between %s and %s using BFS in the optimal way(the actual shortest path)\n", scrCities[i], destCities[i]);
                    BFS_Optimal(graph, city, count, scrCities[i]);
                    printPathOnFile(city, count, destCities[i], file);
                    fprintf(file, "\n\nThe distance between %s and %s using BFS in the optimal way(the actual shortest path) is %dkm\n", scrCities[i], destCities[i],
                            city[idx].distFromSrc);

                    fprintf(file, "\n\nThe path between %s and %s using BFS in the not optimal way\n", scrCities[i], destCities[i]);
                    BFS_Optimal(graph, city, count, scrCities[i]);
                    printPathOnFile(city, count, destCities[i], file);
                    fprintf(file, "\n\nThe distance between %s and %s using BFS  in the not optimal way %dkm\n", scrCities[i], destCities[i],
                            city[idx].distFromSrc);
                    fprintf(file, "\n______________________________________________________________________________\n");
                }
                fclose(file);
            }
            break;
        }

        else{
            printf("this selection is invalid\n");
        }
        start = false;
        temp = choice;

    }
    printf("\nThe program ended");




}



void printPath(struct city city[],int n, char destination[maxLength]) {
    int idx = findIdx(city,n, destination);

    // Base case: reached the source vertex
    if (strcmp(city[idx].parent, "NULL") == 0) {
        printf("%s ", city[idx].city);
        return;
    }

    // Recursive case: print the path to the parent first
    printPath(city,n, city[idx].parent);
    printf("-> %s (%dkm) ", city[idx].city, city[idx].distFromParent);
}

void printPathOnFile(struct city city[],int n, char destination[maxLength], FILE* outputFile) {
    int idx = findIdx(city,n, destination);

    // Base case: reached the source vertex
    if (strcmp(city[idx].parent, "NULL") == 0) {
        fprintf(outputFile, "%s", city[idx].city);
        return;
    }

    // Recursive case: print the path to the parent first
    printPathOnFile(city, n,city[idx].parent, outputFile);
    fprintf(outputFile, " -> %s (%dkm)", city[idx].city, city[idx].distFromParent);
}


void dijkstra(struct Graph* graph, struct city city[],int n, char src[maxLength]) {
    // Initialize the distenation from src with zeros
    for(int i = 0; i < n; i++) {
        city[i].distFromSrc = 0;
    }
    int V = graph->v;
    int visitedIdx = 0;
    int selectedIdx = 0;
    char visited[n][maxLength]; // To mark the cities the enter the min heap
    char selected[n][maxLength];// To mark the cities that is extracted from the min heap
    struct MinHeap* minHeap = createMinHeap(V);

    // Set distance of source vertex to 0
    int srcIdx = 0;
    // Find the source city in the graph
    while (srcIdx < V && strcmp(graph->array[srcIdx]->city, src) != 0) {
        srcIdx++;
    }
    if (srcIdx == V) {
        printf("Source vertex not found.\n");
        return;
    }

    int idx = findIdx(city,n, src);
    // Set the parent of the src city with NULL
    strcpy(city[idx].parent, "NULL");
    struct vertex* temp = graph->array[srcIdx];
    strcpy(visited[visitedIdx], temp->city);
    visitedIdx++;
    strcpy(selected[selectedIdx], temp->city);
    selectedIdx++;
    int k = 0;

    // Add all the cities connected to the src city to the min heap and add it to the visited array
    while (k < temp->edgeIdx) {
        idx = findIdx(city,n, temp->edges[k]->dest);
        insert(minHeap, city[idx].distFromSrc + temp->edges[k]->weight, temp->edges[k]->dest);
        strcpy(visited[visitedIdx], temp->edges[k]->dest);
        visitedIdx++;
        //Update the distenation from src
        city[idx].distFromSrc = temp->edges[k]->weight + city[srcIdx].distFromSrc;

        //Update the distenation from parent
        city[idx].distFromParent = temp->edges[k]->weight;

        //Update the parent
        strcpy(city[idx].parent, temp->city);

        k++;
    }

    // Dijkstra's algorithm
    while (!isEmpty(minHeap)) {
        // Extract the min distance node from the heap
        struct MinHeapNode *minHeapNode = extractMin(minHeap);
        // Mark it as selected(extract it from the heap)
        strcpy(selected[selectedIdx], minHeapNode->city);
        selectedIdx++;
        int l = 0;
        // Find the city in the graph
        while (l < V && strcmp(graph->array[l]->city, minHeapNode->city) != 0) {
            l++;
        }
        temp = graph->array[l];
        int idx1 = findIdx(city,n, temp->city);
        k = 0;
        // Iterate through the adjacent cities and add it to the heap
        while (k < temp->edgeIdx) {
            idx = findIdx(city,n, temp->edges[k]->dest);
            // If it is not in the heap I added it to the heap and update the property of the city
            if (!isSelected(visited, n, temp->edges[k]->dest)) {
                //Check if the new distance is shorter than the present distance or not
                if (city[idx].distFromSrc == 0 || city[idx].distFromSrc > temp->edges[k]->weight + city[idx1].distFromSrc) {
                    city[idx].distFromSrc = temp->edges[k]->weight + city[idx1].distFromSrc;
                    city[idx].distFromParent = temp->edges[k]->weight;
                    strcpy(city[idx].parent, city[idx1].city);
                    insert(minHeap, city[idx].distFromSrc, city[idx].city);
                    strcpy(visited[visitedIdx], city[idx].city);
                    visitedIdx++;
                }
            }

                // If it in the heap I update the heap and update the property of the city
            else if (isSelected(visited, n, temp->edges[k]->dest) && !isSelected(selected, n, temp->edges[k]->dest) && strcmp(city[idx].parent, "NULL") != 0) {
                //Check if the new distance is shorter than the present distance or not
                if (city[idx].distFromSrc == 0 || city[idx].distFromSrc > temp->edges[k]->weight + city[idx1].distFromSrc) {
                    city[idx].distFromSrc = temp->edges[k]->weight + city[idx1].distFromSrc;
                    city[idx].distFromParent = temp->edges[k]->weight;
                    strcpy(city[idx].parent, city[idx1].city);
                    updateHeap(minHeap, city[idx].city, city[idx].distFromSrc);

                }


            }
                // If it is removed from the heap I update the property directly
            else if(isSelected(selected, n, temp->edges[k]->dest) && strcmp(city[idx].parent, "NULL") != 0) {
                if (city[idx].distFromSrc == 0 || city[idx].distFromSrc > temp->edges[k]->weight + city[idx1].distFromSrc) {
                    city[idx].distFromSrc = temp->edges[k]->weight + city[idx1].distFromSrc;
                    city[idx].distFromParent = temp->edges[k]->weight;
                    strcpy(city[idx].parent, city[idx1].city);
                }
            }
            k++;
        }

    }
}
// check if the city to be added is already added or not
bool isSelected(char selected[][maxLength], int n, char city[maxLength]){
    for(int i = 0; i < n; i++){
        if(strcmp(selected[i],city) == 0)return true;
    }
    return false;


}
// Find the index of a given city
int findIdx(struct city cities[],int n, char city[maxLength]){
    int i;
    for(i = 0; i < n; i++){
        if(strcmp(cities[i] . city,city)==0) return i;
    }
    return -1;

}
// Function to find the shortest path and distance in the optimal way (the actual shortest path) using BFS
void BFS_Optimal(struct Graph* graph, struct city city[],int n, char src[maxLength]) {
    // Initialize the distenation from src with zeros
    for(int i = 0; i < n; i++) {
        city[i].distFromSrc = 0;
    }
    int V = graph->v;
    int visitedIdx = 0;
    char visited[n][maxLength]; // To mark the cities the enter the queue
    Queue Q = CreateQueue(V);
    MakeEmpty(Q);

    // Set distance of source vertex to 0
    int srcIdx = 0;
    // Find the source city in the graph
    while (srcIdx < V && strcmp(graph->array[srcIdx]->city, src) != 0) {
        srcIdx++;
    }
    if (srcIdx == V) {
        printf("Source vertex not found.\n");
        return;
    }

    int idx = findIdx(city,n, src);
    // Set the parent of the src city with NULL
    strcpy(city[idx].parent, "NULL");
    struct vertex* temp = graph->array[srcIdx];
    strcpy(visited[visitedIdx], temp->city);
    visitedIdx++;
    int k = 0;
    // Add all the cities connected to the src city to the queue and add it to the visited array
    while (k < temp->edgeIdx) {
        idx = findIdx(city,n, temp->edges[k]->dest);
        Enqueue(temp->edges[k]->dest, Q);
        strcpy(visited[visitedIdx], temp->edges[k]->dest);
        visitedIdx++;
        city[idx].distFromSrc = temp->edges[k]->weight + city[srcIdx].distFromSrc;
        city[idx].distFromParent = temp->edges[k]->weight;
        strcpy(city[idx].parent, temp->city);

        k++;
    }

    while (!IsEmpty(Q)) {
        char* x = FrontAndDequeue(Q);
        int l = 0;
        while (l < V && strcmp(graph->array[l]->city, x) != 0) {
            l++;
        }
        temp = graph->array[l];
        int idx1 = findIdx(city,n, temp->city);
        k = 0;
        while (k < temp->edgeIdx) {
            idx = findIdx(city,n, temp->edges[k]->dest);
            // If it is not in the queue I added it to the queue and update the property of the city
            if (!isSelected(visited, n, temp->edges[k]->dest)) {
                //Check if the new distance is shorter than the present distance or not
                if (city[idx].distFromSrc == 0 || city[idx].distFromSrc > temp->edges[k]->weight + city[idx1].distFromSrc) {
                    city[idx].distFromSrc = temp->edges[k]->weight + city[idx1].distFromSrc;
                    city[idx].distFromParent = temp->edges[k]->weight;
                    strcpy(city[idx].parent, city[idx1].city);
                    Enqueue( city[idx].city, Q);
                    strcpy(visited[visitedIdx], city[idx].city);
                    visitedIdx++;
                }
            }
                // If the city is added to the queue, we check if the new distance is less than the previously recorded distance for the city.
                // If it is less, we update the distance and enqueue it again in the queue to explore further and potentially find a shorter path.
            else if (isSelected(visited, n, temp->edges[k]->dest) && strcmp(city[idx].parent, "NULL") != 0) {
                //Check if the new distance is shorter than the present distance or not
                if (city[idx].distFromSrc == 0 || city[idx].distFromSrc > temp->edges[k]->weight + city[idx1].distFromSrc) {
                    city[idx].distFromSrc = temp->edges[k]->weight + city[idx1].distFromSrc;
                    city[idx].distFromParent = temp->edges[k]->weight;
                    strcpy(city[idx].parent, city[idx1].city);
                    Enqueue( city[idx].city, Q);


                }

            }

            k++;
        }

    }
}

// Function to find the shortest path and distance not in the optimal way (not the actual shortest path) using BFS
void BFS_Not_Optimal(struct Graph* graph, struct city city[], int n, char src[maxLength], char dest[maxLength]) {
    // Initialize the distenation from src with 0
    for (int i = 0; i < n; i++) {
        city[i].distFromSrc = 0;
    }

    int V = graph->v;
    int visitedIdx = 0;
    char visited[n][maxLength];
    Queue Q = CreateQueue(V);
    MakeEmpty(Q);


    int srcIdx = -1;
    // Find the source city in the graph
    for (int i = 0; i < V; i++) {
        if (strcmp(graph->array[i]->city, src) == 0) {
            srcIdx = i;
            break;
        }
    }
    if (srcIdx  == -1) {
        printf("Source vertex not found.\n");
        return;
    }

    int idx = findIdx(city, n, src);
    // Set the parent of the src city with NULL
    strcpy(city[idx].parent, "NULL");
    struct vertex* temp = graph->array[srcIdx];
    strcpy(visited[visitedIdx], temp->city);
    visitedIdx++;

    int k = 0;
    while (k < temp->edgeIdx) {
        // Add all the cities connected to the src city to the queue and add them to the visited array
        idx = findIdx(city, n, temp->edges[k]->dest);
        Enqueue(temp->edges[k]->dest, Q);
        strcpy(visited[visitedIdx], temp->edges[k]->dest);
        visitedIdx++;
        city[idx].distFromSrc = temp->edges[k]->weight + city[srcIdx].distFromSrc;
        city[idx].distFromParent = temp->edges[k]->weight;
        strcpy(city[idx].parent, temp->city);

        k++;
    }

    while (!IsEmpty(Q)) {
        char* x = FrontAndDequeue(Q);
        int l = 0;
        while (l < V && strcmp(graph->array[l]->city, x) != 0) {
            l++;
        }
        temp = graph->array[l];
        int idx1 = findIdx(city, n, temp->city);
        k = 0;
        while (k < temp->edgeIdx) {
            idx = findIdx(city, n, temp->edges[k]->dest);
            // If it is not in the visited array, add it to the queue and update the properties of the city
            if (!isSelected(visited, visitedIdx, temp->edges[k]->dest)) {
                city[idx].distFromSrc = temp->edges[k]->weight + city[idx1].distFromSrc;
                city[idx].distFromParent = temp->edges[k]->weight;
                strcpy(city[idx].parent, city[idx1].city);
                Enqueue(city[idx].city, Q);
                strcpy(visited[visitedIdx], city[idx].city);
                visitedIdx++;

                // Break when the destination city is found
                if (strcmp(city[idx].city, dest) == 0) break;
            }

            k++;
        }
    }
}

//Function to extract the number from the string
int extractNumber(char str[maxLength]) {
    int len = strlen(str);
    char number[maxLength];
    int numIndex = 0;

    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            number[numIndex++] = str[i];
        }
        else {
            break;
        }
    }

    number[numIndex] = '\0';
    int extractedNum = atoi(number);

    return extractedNum;
}