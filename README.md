## Graph-Based City Navigation System Using Dijkstra and BFS (C Program)

This C program implements a **city navigation system** that finds the **shortest path between cities** using **Dijkstra’s Algorithm** and **Breadth-First Search (BFS)**. Cities and distances between them are represented as a **graph using an adjacency list**.

---

### Graph Representation

The program uses **structs** to represent cities, connections, and the graph:

- **`struct edge`** – Represents a connection (road) between two cities, storing the destination and distance.
- **`struct vertex`** – Represents a city, containing its name and an array of edges.
- **`struct Graph`** – Stores all cities as an adjacency list.

---

### Graph Operations

- **`createNode()`** – Creates a new city (vertex).
- **`createGraph()`** – Initializes a graph with a given number of cities.
- **`addEdge()`** – Adds a road (edge) between two cities with a specified distance.
- **`printGraph()`** – Displays the city connections.

---

### Min-Heap Implementation (For Dijkstra’s Algorithm)

The program uses a **Min-Heap** to efficiently extract the city with the shortest distance while computing the shortest path using Dijkstra's algorithm.

#### Min-Heap Structures

```c
struct MinHeapNode {
    int dist;
    char city[100];
};

struct MinHeap {
    int size;
    int capacity;
    struct MinHeapNode **array;
};
```
#### Min-Heap Functions

- **`createMinHeap(int capacity)`** – Initializes a Min-Heap with a given capacity.
- **`newMinHeapNode(int dist, char city[100])`** – Creates a Min-Heap node with a city name and distance.
- **`insert(struct MinHeap *minHeap, int dist, char city[100])`** – Inserts a city into the Min-Heap.
- **`extractMin(struct MinHeap *minHeap)`** – Removes and returns the city with the shortest distance.
- **`updateHeap(struct MinHeap *minHeap, char city[100], int dist)`** – Updates a city's distance in the heap.
- **`minHeapify(struct MinHeap *minHeap, int idx)`** – Maintains the Min-Heap property.

---

### Queue Implementation (For BFS Traversal)

The program uses a queue for BFS traversal, implemented as a circular array-based queue.

#### Queue Structure (`struct QueueRecord`)

```c
struct QueueRecord {
    int Capacity;
    int Front;
    int Rear;
    int Size;
    char **Array;
};
```
#### Queue Functions

- **`CreateQueue(int MaxElements)`** – Initializes a queue with a given capacity.
- **`MakeEmpty(Queue Q)`** – Resets the queue to an empty state.
- **`IsEmpty(Queue Q)`** – Checks if the queue is empty.
- **`IsFull(Queue Q)`** – Checks if the queue is full.
- **`Enqueue(char *X, Queue Q)`** – Adds a city to the queue.
- **`Dequeue(Queue Q)`** – Removes a city from the queue.
- **`Front(Queue Q)`** – Retrieves the front city without removing it.
- **`FrontAndDequeue(Queue Q)`** – Retrieves and removes the front city.
- **`DisposeQueue(Queue Q)`** – Frees allocated memory.

---

### Shortest Path Algorithms

#### Dijkstra’s Algorithm (`dijkstra()`)

Finds the shortest path from a given source city using a priority queue (Min-Heap).

- Maintains an array `struct city` to store:
  - Distance from source (`distFromSrc`)
  - Parent city in the path (`parent`)
- Extracts the closest city and updates its neighbors.

#### BFS Variants

- **`BFS_Optimal()`** – Finds the actual shortest path using BFS.
- **`BFS_Not_Optimal()`** – A variation of BFS that does not guarantee the shortest path.

---

### Path Printing and File Output

- **`printPath()`** – Recursively prints the shortest path.
- **`printPathOnFile()`** – Writes the path to a file (`shortest_distance.txt`).

---

### Main Program Flow

1. Reads city connections from `cities.txt`.
2. Constructs a graph using adjacency lists.
3. Menu-based interaction:
   - **Option 1**: Load and display the graph.
   - **Option 2**: Enter a source city.
   - **Option 3**: Enter a destination city, compute the shortest path using Dijkstra & BFS.
   - **Option 4**: Save the shortest paths to `shortest_distance.txt` and exit.

---

### Features

**Graph representation** using adjacency lists  
**Efficient shortest path computation** using Dijkstra’s Algorithm  
**Breadth-First Search (BFS)** implementation for alternative pathfinding  
**Queue-based BFS traversal**  
**Min-Heap priority queue** for optimized shortest path search  
**File input/output** for city distances  
**Interactive menu-based navigation**

---
### Program Output Example

When running the program, the user is presented with the following menu:

Welcome to the city navigation system  
Select one of the following:
1) Load cities: Loads the file and constructs the graph  
2) Enter source: Read the source city  
3) Enter destination: Print the full route of the shortest path including the distance between each two cities and the total shortest cost for both algorithms (Dijkstra and BFS)  
4) Exit: Prints the information of step 3 to a file called `shortest_distance.txt` and exits the program

---

#### If the User Chooses 1: Load Cities  
The adjacency list of the graph is displayed:

Akka -> Haifa -> Nazareth -> Jenin  
Haifa -> Jenin -> Nazareth -> Tulkarem  
Nazareth -> Akka  
Jenin -> Tulkarem -> Qalqilya -> Tubas -> Nazareth -> Nablus  
Bethlehem -> Hebron -> Jerusalem -> Jaffa -> Jericho  
Hebron -> Jericho -> Jerusalem  
Jerusalem -> Ramallah  
Jaffa -> Qalqilya  
Jericho -> Jerusalem -> Ramallah  
Tulkarem -> Jaffa -> Haifa  
Qalqilya -> Salfit -> Tulkarem  
Tubas -> Jericho  
Nablus -> Jericho -> Qalqilya -> Tulkarem -> Tubas -> Salfit  
Ramallah -> Bethlehem -> Salfit -> Jaffa -> Jerusalem  
Salfit -> Qalqilya -> Tulkarem  


---

#### If the User Chooses 2: Enter Source City  
**Source City**: Akka

---

#### If the User Chooses 3: Enter Destination City  
**Destination City**: Jericho


The program will then find and show the shortest path, using both Dijkstra's Algorithm and BFS for comparison:

---
##### The Path Between Akka and Jericho Using Dijkstra

Akka -> Jenin (74km) -> Tubas (30km) -> Jericho (52km)

The distance between Akka and Jericho using Dijkstra is 156km

---

##### The Path Between Akka and Jericho Using BFS in the Optimal Way (Actual Shortest Path)

Akka -> Jenin (74km) -> Tubas (30km) -> Jericho (52km)  

The distance between Akka and Jericho using BFS in the optimal way is 156km

---

##### The Path Between Akka and Jericho Using BFS in the Not Optimal Way

Akka -> Jenin (74km) -> Tubas (30km) -> Jericho (52km)  

The distance between Akka and Jericho using BFS in the not optimal way is 156km

---

#### If the User Chooses 4: Exit  
The program ends and prints the results to the file `shortest_distance.txt`.

---

**The Program Ended**

