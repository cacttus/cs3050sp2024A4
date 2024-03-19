#ifndef _bst_h
#define _bst_h

// Symbolic Constants

#include <stdio.h>
#include <string.h>

#define __BE_QUIET__
#ifndef __BE_QUIET__
#define msg(...)         \
  do                     \
  {                      \
    printf(__VA_ARGS__); \
    printf("\n");        \
  } while (0)

#define _print(...)      \
  do                     \
  {                      \
    printf(__VA_ARGS__); \
    fflush(stdout);      \
  } while (0)
#else
#define _print(...)
#define msg(...)
#endif

// Data types
typedef long long int64;

// NOTE! - You will want to fill out this data structure.  Do you want a separate
// NODE data structure as well?
typedef struct BSTNode
{
  struct BSTNode *_left;
  struct BSTNode *_right;
  struct BSTNode *_parent;
  int64 _key;
  void *_satellite;
} BSTNode;
typedef struct TreeData
{
  int _th; // zero based
  int _keyw;
  int _plh;
  int _prh;
} TreeData;
typedef struct BST
{
  struct BSTNode *_root;
} BST;

typedef void (*NODEVISITFUNC)(void *parm);

void printBSTFormatted(BST *root, int level);
void print_tree(BST *root);

// Prototypes
BST *NewBST();
void TreeInsert(BST *pBST, void *satellite, long long key);
void InOrder(BST *pBST, NODEVISITFUNC func);
void PreOrder(BST *pBST, NODEVISITFUNC func);
void PostOrder(BST *pBST, NODEVISITFUNC func);
void *Search(BST *pBST, long long key);
void TreeDelete(BST *pBST, long long key);

#endif
