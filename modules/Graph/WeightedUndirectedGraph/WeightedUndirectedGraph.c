#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include "WeightedUndirectedGraph.h"

typedef struct edge
{
   Vertex endpoint;
   cost weight;  // the weight of the edge
   struct edge *nextedge;
}
edge;
typedef struct edge* Edge;

typedef struct _wu_graph
{
   uint n;           // number of vertices in the graph
   Edge* firstedge;  // adjacency list representation
}
_wu_graph;

void wug_init(wu_graph* G, uint num_of_vertices)
{
   assert(num_of_vertices > 0);

   *G = malloc(sizeof(_wu_graph));
   assert(*G != NULL);

   (*G)->firstedge = calloc(sizeof(Edge), num_of_vertices);
   assert((*G)->firstedge != NULL);  // allocation failure

   (*G)->n = num_of_vertices;
}

void wug_insert(wu_graph G, Vertex A, Vertex B, cost weight)
{
   // insert vertex B at the start of the list A - O(1)
   Edge* al = &(G->firstedge[A]);

   Edge new_edge = malloc(sizeof(edge));
   assert(new_edge != NULL);  // allocation failure

   new_edge->endpoint = B;
   new_edge->weight = weight;
   new_edge->nextedge = *al;
   *al = new_edge;

   // insert vertex A at the start of the list B (undirected graph) - O(1)
   al = &(G->firstedge[B]);

   new_edge = malloc(sizeof(edge));
   assert(new_edge != NULL);  // allocation failure

   new_edge->endpoint = A;
   new_edge->weight = weight;
   new_edge->nextedge = *al;
   *al = new_edge;
}

// helper function prototypes
typedef struct pq* PQueue;
typedef struct n
{
   int v;                // vertex v
   unsigned int weight;  // weight of v
}
n;
static PQueue createPQueue(uint n, int* E, unsigned int* C);
static n pq_remove(PQueue PQ);
static bool is_pq_empty(PQueue PQ);
static void updateWeights(PQueue PQ, int v, unsigned int new_weight);
static bool pq_exists(PQueue PQ, int v);
static void pq_destroy(PQueue PQ);
static void print_min_span_tree(wu_graph G, int* arr, int n);

// source: https://en.wikipedia.org/wiki/Prim%27s_algorithm#Description
void wug_minspantree(wu_graph G)
{
   assert(G!=NULL);

   int size_of_graph = G->n;

   // create helper arrays
   int* E = calloc(sizeof(int), size_of_graph);
   assert(E != NULL);  // allocation failure

   cost* C = calloc(sizeof(cost), size_of_graph);
   assert(C != NULL);  // allocation failure

   // create priority queue
   PQueue Q = createPQueue(size_of_graph, E, C);

   while (!is_pq_empty(Q))  // not an empty queue
   {
      n min_vertex = pq_remove(Q);
      Vertex vert = min_vertex.v;

      Edge ed = G->firstedge[vert];
      while (ed != NULL)
      {
         int dest = ed->endpoint;
         if (pq_exists(Q, dest) && ed->weight < C[dest])  // edge with lower weight found
         {
            // update weight / keep pq heapified
            updateWeights(Q, dest, ed->weight);

            // update the parent and minimum weight of vertex dest
            C[dest] = ed->weight;
            E[dest] = vert;
         }
         ed = ed->nextedge;  // get next vertex
      }
   }

   // print the minimum spanning tree created
   print_min_span_tree(G, E, size_of_graph);

   // free allocated memory
   pq_destroy(Q);
   free(E);
   free(C);
}

static void print_min_span_tree(wu_graph G, int* E, int n)
{
   cost total_weight = 0;
   for (int i = 1; i < n; i++)
   {
      // vertex is not included in the minimum spanning tree
      if (E[i] == INT_MIN)
         continue;
      
      Edge ed = G->firstedge[i];
      printf("(%d-%d)", E[i], i);
      
      // find the weight of the edge
      while (ed != NULL)
      {
         if (ed->endpoint == E[i])
         {
            total_weight += ed->weight;
            printf(" || weight = %d\n", ed->weight);
            break;
         }
         ed = ed->nextedge;
      }
   }
   printf("Total weight = %d\n", total_weight);
}

void wug_print(wu_graph G)
{
   for (int i = 0; i < G->n; i++)
   {
      Edge a = G->firstedge[i];
      printf("[%d]", i);
      if (a != NULL)
         printf(" :");
      while (a != NULL)
      {
         printf("%d|%d| ", a->endpoint, a->weight);
         a = a->nextedge;
      }
      printf("\n");
   }
}

void DestroyGraph(wu_graph G)
{
   for (int i = 0; i < G->n; i++)
   {
      Edge a = G->firstedge[i];
      while (a != NULL)
      {
         Edge tmp = a;
         a = a->nextedge;
         free(tmp);
      }
   }
   free(G->firstedge);
   free(G);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Custom priority queue
/////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct n* node;
typedef struct pq
{
   node arr;        // array of nodes containing the data
   int* pos;        // position of vertices in the queue
   uint curr_size;  // current size of the heap
   uint capacity;   // max capacity of the heap
}
pq;
#define ROOT 0
#define find_parent(i) ((i-1)/2)
#define find_left_child(i) (2*i + 1)
#define find_right_child(i) (2*i + 2)

static void bubble_down(PQueue PQ, uint node);

static void pq_init(PQueue* PQ, unsigned int size)
{
   *PQ = malloc(sizeof(pq));
   assert(*PQ != NULL);  // allocation failure

   // allocate memory for the array of nodes
   (*PQ)->arr = calloc(size, sizeof( *((*PQ)->arr)) );
   assert((*PQ)->arr != NULL);  // allocation failure

   (*PQ)->pos = calloc(size, sizeof(int));
   assert((*PQ)->pos != NULL);  // allocation failure

   (*PQ)->capacity = size;
   (*PQ)->curr_size = size;
}

static bool is_pq_empty(PQueue PQ)
{
   return PQ->curr_size == 0;
}

static PQueue createPQueue(uint n, int* E, uint* C)
{
   // create priority queue
   PQueue pq;
   pq_init(&pq, n);

   // initialize pq
   for (uint i = 0; i < n; i++)
   {
      C[i] = INT_MAX;
      E[i] = INT_MIN;
      pq->arr[i].weight = INT_MAX;
        
      pq->pos[i] = i;
      pq->arr[i].v = i;
   }

   // make sure the item removed has the highest priority
   pq->arr[0].weight = 0;

   return pq;
}

static n pq_remove(PQueue PQ)
{
   // store root node
   n root = PQ->arr[ROOT];

   // swap positions
   PQ->pos[root.v] = PQ->curr_size-1;
	PQ->pos[PQ->arr[PQ->curr_size-1].v] = ROOT;

   PQ->arr[ROOT] = PQ->arr[PQ->curr_size-1];

   PQ->curr_size--;  // node removed, decrement the number of elements in the queue

   bubble_down(PQ, ROOT);  // heapify

   // return root node - node with the highest priority
   return root;
}

static void bubble_down(PQueue PQ, uint node)
{
   // get left child
   uint left = find_left_child(node);
   if (left > PQ->curr_size)  // children do not exist
      return;

   // get right child
   uint max_child, right = find_right_child(node);

   // find max child
   max_child = left;
   if (right <= PQ->curr_size && PQ->arr[right].weight < PQ->arr[max_child].weight)
      max_child = right;

   if (PQ->arr[max_child].weight < PQ->arr[node].weight)
   {
      // swap positions
      PQ->pos[PQ->arr[max_child].v] = node;
      PQ->pos[PQ->arr[node].v] = max_child;

      // swap values
      n tmp = PQ->arr[node];
      PQ->arr[node] = PQ->arr[max_child];
      PQ->arr[max_child] = tmp;

      bubble_down(PQ, max_child);
   }
}

static void updateWeights(PQueue PQ, int v, unsigned int new_weight)
{
   // update weight for vertex v
   uint curr_node = PQ->pos[v];
   PQ->arr[curr_node].weight = new_weight;

   // heapify
   uint parent = find_parent(curr_node);
   while (curr_node > 0 && PQ->arr[curr_node].weight < PQ->arr[parent].weight)
   {
      // swap positions
      PQ->pos[PQ->arr[curr_node].v] = parent;
      PQ->pos[PQ->arr[parent].v] = curr_node;

      // swap values
      n tmp = PQ->arr[curr_node];
      PQ->arr[curr_node] = PQ->arr[parent];
      PQ->arr[parent] = tmp;

      // move up
      curr_node = parent;
      parent = find_parent(curr_node);
   }
}

static bool pq_exists(PQueue PQ, int v)
{
   return (PQ->pos[v] < PQ->curr_size ? true: false);
}

static void pq_destroy(PQueue PQ)
{
   free(PQ->pos);
   free(PQ->arr);
   free(PQ);
}