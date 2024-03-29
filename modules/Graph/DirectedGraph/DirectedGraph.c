#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "DirectedGraph.h"
#include "../../Queue/queue.h"
#include "../../Stack/stack.h"

typedef int* Toporder;

typedef struct edge
{
   Vertex endpoint;
   struct edge *nextedge;
}
edge;
typedef struct edge* Edge;

typedef struct _dir_graph
{
   uint32_t n;       // number of vertices in the graph
   Edge* firstedge;  // adjacency list representation
   VisitFunc visit;  // function that visits the vertices
}
_dir_graph;

// function prototype
static Vertex* createVertex(Vertex V);

dir_graph dg_create(const uint32_t num_of_vertices, const VisitFunc visit)
{
   assert(visit != NULL);  // a visit function needs to be given

   dir_graph G = malloc(sizeof(_dir_graph));
   assert(G != NULL);
   
   G->firstedge = calloc(sizeof(Edge), num_of_vertices);
   assert(G->firstedge != NULL);  // allocation failure

   G->n = num_of_vertices;
   G->visit = visit;

   return G;
}

void dg_insert(const dir_graph G, const Vertex A, const Vertex B)
{
   // sorted insert of vertex B at list A - O(n)
   Edge* al = &(G->firstedge[A]);

   Edge new_edge = malloc(sizeof(edge));
   assert(new_edge != NULL);  // allocation failure

   new_edge->endpoint = B;

   if (*al == NULL)
   {
      new_edge->nextedge = NULL;
      *al = new_edge;
      return;
   }

   // the head of the list has less priority than the new node, meaning the new vertex should be first
   if ((*al)->endpoint > B)
   {
      new_edge->nextedge = (*al);
      (*al) = new_edge;
      return;
   }

   // in any other case, traverse the list and find the correct position to insert the vertex
   Edge tmp = *al;
   while (tmp->nextedge != NULL && tmp->nextedge->endpoint < B) tmp = tmp->nextedge;

   // put the vertex at its place
   new_edge->nextedge = tmp->nextedge;
   tmp->nextedge = new_edge;
}

void dg_print(const dir_graph G)
{
   assert(G != NULL);

   for (uint32_t i = 0; i < G->n; i++)
   {
      Edge a = G->firstedge[i];
      
      printf("[%d]", i);
      if (a != NULL)
         printf(": ");
      while (a != NULL)
      {
         G->visit(a->endpoint);
         a = a->nextedge;
      }
      printf("\n");
   }
}

// Helper function prototypes
static void Traverse(const dir_graph, const Vertex, bool*, const int*, const int*);
static void Explore(const dir_graph, const Vertex, bool*, int*, int*, int*, int*);
static void print_green();
static void print_black();
static void print_red();
static void reset_col();

void dg_dfs(const dir_graph G)
{
   assert(G != NULL);

   Vertex v;
   bool* visited = calloc(sizeof(bool), G->n);
   assert(visited != NULL);  // allocation failure

   int* pre = calloc(sizeof(int), G->n);
   assert(pre != NULL);  // allocation failure

   int* post = calloc(sizeof(int), G->n);
   assert(post != NULL);  // allocation failure

   // traverse the graph and store the preorder and postorder numberings
   // in arrays pre and post, respectively and use them for traverse in 
   // order to classify the edges as back, cross or forward.
   int count1 = 0;
   int count2 = 0;

   for (v = 0; v < G->n; v++)
      if (!visited[v])
         Explore(G, v, visited, pre, post, &count1, &count2);

   // reset visited array
   for (v = 0; v < G->n; v++)
      visited[v] = false;

   // dfs
   for (v = 0; v < G->n; v++)
      if (!visited[v])
         Traverse(G, v, visited, pre, post);

   // free allocated memory
   free(post);
   free(pre);
   free(visited);
}

static void Traverse(const dir_graph G, const Vertex v, bool* visited, const int* pre, const int* post)
{
   Vertex w;
   visited[v] = true;
   
   G->visit(v); printf("\n");

   Edge curedge = G->firstedge[v];  // curedge is a pointer to the first edge (v,_) of V 
   
   while (curedge != NULL)
   {
      w = curedge->endpoint;  // w is a successor of v and (v,w) is the current edge
      
      if (!visited[w])  // it is a tree edge
      {
         Traverse(G, w, visited, pre, post);
         printf("Tree edge:");
      }
      else  // have already visited this vertex
      {
         if (post[v] < post[w])  // it leads to a vertex with a higher postorder number
         {
            print_green();  // print with green color back edges
            printf("Back edge:");
         }
         else if (pre[v] > pre[w])  // it leads to a vertex with a lower preorder number
         {
            print_red();  // print with red color cross edges
            printf("Cross edge:");
         }
         else if (pre[v] < pre[w])  // it leads to a vertex with a higher preorder number
         {
            print_black();  // print with black color forward edges
            printf("Forward edge:");
         }
      }
      printf(" (%d,%d)\n", v, w);
      reset_col();
      curedge = curedge->nextedge;  // curedge is a pointer to the next edge (v,_) of V
   }
}

static void print_green() { printf("\033[0;32m"); }

static void print_black() { printf("\033[0;30m"); }

static void print_red() { printf("\033[0;31m"); }

// reset the color
static void reset_col() { printf("\033[0m"); }

// explore is used to store the preorder and postorder numbering of the vertices
static void Explore(const dir_graph G, const Vertex v, bool* visited, int* pre, int* post, int* counter1, int* counter2)
{
   Vertex w;
   visited[v] = true;

   pre[v] = (*counter1)++;  // preorder numbering

   Edge curedge = G->firstedge[v];  // curedge is a pointer to the first edge (v,_) of V 
   
   while (curedge != NULL)
   {
      w = curedge->endpoint;  // w is a successor of v and (v,w) is the current edge
      
      if (!visited[w])
         Explore(G, w, visited, pre, post, counter1, counter2);
      
      post[v] = (*counter2)++;  // postorder numbering
      curedge = curedge->nextedge;
   }
}

static void TopSort(const dir_graph, const Toporder);
void dg_bts(const dir_graph G)
{
   Toporder T = malloc(sizeof(*T) * G->n);
   for (uint32_t i = 0; i < G->n; i++)
      T[i] = -1;
   
   TopSort(G, T);

   // print the order
   for (uint32_t v = 0; v < G->n; v++)
   {
      if (T[v] == -1)
         break;
      G->visit(T[v]);
   }
   
   printf("\n");
   free(T);
}

static void TopSort(const dir_graph G, const Toporder T)
{
   int* predecessorcount = calloc(sizeof(int), G->n);    /* number of predecessors of each vertex */
   assert(predecessorcount != NULL);  // allocation failure

   // increase the predecessor count for each vertex that is a successor of another one
   for (uint32_t v = 0; v < G->n; v++)
      for (Edge curedge=G->firstedge[v]; curedge != NULL; curedge=curedge->nextedge)
         ++predecessorcount[curedge->endpoint];
  
   // initialize a queue
   Queue Q = queue_create(free);

   // place all vertices with no predecessors into the queue
   for (uint32_t v = 0; v < G->n; v++)
      if (predecessorcount[v] == 0)
         queue_enqueue(Q, createVertex(v));
  
   // start the breadth-first traversal
   int place = 0;
   while (!is_queue_empty(Q))
   {
      // visit v by placing it into the topological order
      Pointer element = queue_dequeue(Q);
      T[place] = *((int*)element);
      free(element);
    
      // traverse the list of successors of v 
      for (Edge curedge=G->firstedge[T[place]]; curedge != NULL; curedge = curedge->nextedge)
      {
         // reduce the predecessor count for each successor
         predecessorcount[curedge->endpoint]--;

         if (predecessorcount[curedge->endpoint] == 0)  // succ has no further predecessors, so it is ready to process
            queue_enqueue(Q, createVertex(curedge->endpoint));
      }
      place++;
   }
   
   // free allocated memory
   queue_destroy(Q);
   free(predecessorcount);
}

dir_graph dg_reverse(const dir_graph G)
{
   // create the new reversed graph
   dir_graph revG = dg_create(G->n, G->visit);
   
   for (uint32_t v = 0; v < G->n; v++)
   {
      Edge a = G->firstedge[v];
      while (a != NULL)
      {
         Vertex old_eld = a->endpoint;
         dg_insert(revG, old_eld, v);
         a = a->nextedge;
      }
   }

   // return the reversed graph
   return revG;
}

// Helper function prototypes
static void Assign(const dir_graph, const Vertex, int*);
static void Visit(const dir_graph, const int, int*, const Stack);

// Kosaraju's algorithm for finding Strongly-Connected Components
// source: https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm#The_algorithm , where the container L is a stack
void dg_scc(const dir_graph G)
{
   int* visited = calloc(sizeof(int), G->n);
   assert(visited != NULL);  // allocation failure

   // create stack
   Stack L = stack_create(free);

   // perform dfs on g and store the finish order of each vertex on a stack
   for (uint32_t i = 0; i < G->n; i++)
      if (!visited[i])
         Visit(G, i, visited, L);

   // reset visited array
   for (uint32_t i = 0; i < G->n; i++)
      visited[i] = false;

   // get reversed graph
   dir_graph rev_graph = dg_reverse(G);
   
   while (!is_stack_empty(L))  // there are still vertices to be assigned to a component
   {
      // perform dfs on the reversed graph with the graphs on the stack
      // the output in each line printed is a strongly-connected component
      Vertex* v = stack_pop(L);

      // find new strongly-connected component
      if (!visited[*v])
      {
         // print vertices in the component
         Assign(rev_graph, *v, visited);
         printf("\n");
      }
      free(v);
   }

   // free allocated memory
   stack_destroy(L);
   free(visited);
   dg_destroy(rev_graph);
}

static void Visit(const dir_graph G, const int s, int* visited, const Stack L)
{
   if (visited[s])
      return;
   
   visited[s] = true;
   Edge a = G->firstedge[s];

   while (a != NULL)
   {
      Vertex n = a->endpoint;
      if (!visited[n])
         Visit(G, n, visited, L);
      a = a->nextedge;
   }
   // store vertex
   stack_push(L, createVertex(s));
}

static void Assign(const dir_graph G, const Vertex s, int* visited)
{
   if (visited[s])  // s has already been assigned to a component
      return;
   
   // visit the vertex
   visited[s] = true;

   // visit vertex that's in the component
   G->visit(s);

   Edge a = G->firstedge[s];
   while (a != NULL)
   {
      Vertex n = a->endpoint;
      if (!visited[n])
         Assign(G, n, visited);
      a = a->nextedge;  // get next edge/ vertex
   }
}

// allocate memory for the vertex
static Vertex* createVertex(const Vertex V)
{
   Vertex* new_v = malloc(sizeof(Vertex));
   assert(new_v != NULL);  // allocation failure

   *new_v = V;
   return new_v;
}

void dg_destroy(const dir_graph G)
{
   for (uint32_t i = 0; i < G->n; i++)
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
