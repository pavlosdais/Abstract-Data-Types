#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "UndirectedGraph.h"
#include "../../Queue/queue.h"
#include "../../Stack/stack.h"

typedef int Vertex;

typedef struct edge
{
   Vertex endpoint;
   struct edge *nextedge;
}
edge;
typedef struct edge* Edge;

typedef struct _undir_graph
{
   uint n;           // number of vertices in the graph
   Edge* firstedge;  // adjacency list representation
   VisitFunc visit;  // function that visits the vertices
}
_undir_graph;

typedef struct p* point;
typedef struct p
{
   point parent;   // the the parent of the vertex (meaning the vertex it came from): needed in order to print the path
   Vertex vertex;  // the actual vertex
}
p;

void ug_init(undir_graph* G, uint num_of_vertices, VisitFunc visit)
{
   assert(visit != NULL);  // a visit function needs to be given

   *G = malloc(sizeof(_undir_graph));
   assert(*G != NULL);  // allocation failure

   (*G)->firstedge = calloc(sizeof(Edge), num_of_vertices);
   assert((*G)->firstedge != NULL);  // allocation failure

   (*G)->n = num_of_vertices;
   (*G)->visit = visit;
}

void ug_insert(undir_graph G, Vertex A, Vertex B)
{
   // Insert vertex B at the start of the list A - O(1)
   Edge* al = &(G->firstedge[A]);

   Edge new_edge = malloc(sizeof(edge));
   assert(new_edge != NULL);  // allocation failure

   new_edge->endpoint = B;
   new_edge->nextedge = *al;
   *al = new_edge;

   // Insert vertex A at the start of the list B (undirected graph) - O(1)
   al = &(G->firstedge[B]);

   new_edge = malloc(sizeof(edge));
   assert(new_edge != NULL);  // allocation failure

   new_edge->endpoint = A;
   new_edge->nextedge = *al;
   *al = new_edge;
}

void ug_print(undir_graph G)
{
   for (uint i = 0; i < G->n; i++)
   {
      Edge a = G->firstedge[i];
      printf("[%d]", i);
      if (a != NULL)
         printf(": ");
      while (a != NULL)
      {
         printf("%d ", a->endpoint);
         a = a->nextedge;
      }
      printf("\n");
   }
}

// Breadth-first search of the graph from
// source: https://en.wikipedia.org/wiki/Breadth-first_search

// Helper function prototypes
static point CreatePoint(Vertex v, point source);
static void printPath(point a);
void ug_simplepathcheck(undir_graph G, Vertex start, Vertex goal)
{
   int* visited = calloc(sizeof(int), G->n);
   assert(visited != NULL);  // allocation failure

   Vertex v, w;
   Edge curedge;

   // create queue - necessary for the bfs algorithm
   Queue Q;
   queue_init(&Q, free);

   // create stack that stores vertices that are not the goal vertex
   Stack S;
   stack_init(&S, free);

   point a = CreatePoint(start, NULL);
   queue_enqueue(Q, a);
   do
   {
      a = queue_dequeue(Q);
      stack_push(S, a);
      w = a->vertex;
      if (w == goal)  // found goal vertex, print the path
      {
         // print the path
         printf("\nPath: ");
         printPath(a); printf("\n");
         break;
      }

      visited[w] = true;  // mark the vertex as visited

      // not the goal vertex, explore its neighbours
      curedge = G->firstedge[w];
      while (curedge != NULL)
      {
         w = curedge->endpoint;
         if (!visited[w])
         {
            point new_p = CreatePoint(w, a);
            queue_enqueue(Q, new_p);
         }
         curedge=curedge->nextedge;  // get next edge
      }
   }
   while (!is_queue_empty(Q));  // have not traversed through all vertices
   
   if (w != goal)  // no simple path exists that connects the vertices
      printf("\nNo simple path between vertices %d and %d exists!\n", start, goal);

   // free allocated memory
   stack_destroy(S);
   queue_destroy(Q);
   free(visited);
}

void ug_destroy(undir_graph G)
{
   for (uint i = 0; i < G->n; i++)
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

// recursive function that prints the actual path
static void printPath(point a)
{
   if (a == NULL)
      return;
   printPath(a->parent);

   printf("[%d] ", a->vertex);
}

// allocate memory for the point
static point CreatePoint(Vertex v, point source)
{
   point new_point = malloc(sizeof(p));
   assert(new_point != NULL);  // allocation failure

   new_point->vertex = v;
   new_point->parent = source;

   return new_point;
}
