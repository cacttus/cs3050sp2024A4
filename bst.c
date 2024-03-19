#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <math.h>
#include "bst.h"
#include "limits.h"
#include "assert.h"
struct BST *NewBST() {
  BST *b = (BST *)malloc(sizeof(BST));
  b->_root = NULL;
  return b;
}
int _count_digits(int n) {
  if (n < 0) {
    return _count_digits(-n) + 1;  //+1 for neg
  }
  if (n < 10) {
    return 1;
  }
  return _count_digits(n / 10) + 1;
}
void _tree_data(BSTNode *root, int h, TreeData *td, int pl, int pr) {
  if (root) {
    if (h > td->_th) {
      td->_th = h;
    }
    int kd = _count_digits(root->_key);
    if (kd > td->_keyw) {
      td->_keyw = kd;
    }

    _tree_data(root->_left, h + 1, td, pl, pr + 1);
    _tree_data(root->_right, h + 1, td, pl + 1, pr);
  }
  else if (pl == 0) {
    td->_plh = h;
  }
  else if (pr == 0) {
    td->_prh = h;
  }
}
void _printrp(int count, char c) {
  for (int i = 0; i < count; ++i) {
    _print("%c", c);
  }
}
void _printsp(int count) {
  _printrp(count, ' ');
}
void print_tree(BST *bst) {
  assert(bst != NULL);

  TreeData td;
  td._keyw = td._th = td._plh = td._prh = 0;
  _tree_data(bst->_root, 0, &td, 0, 0);
  BSTNode **nlev = (BSTNode **)malloc(sizeof(BSTNode *));
  int nlev_size = 1;
  nlev[0] = bst->_root;
  //  msg("td: keyw=%i, th=%i, plh=%i, prh=%i\n", td._keyw, td._th, td._plh, td._prh);
  _print("----------------------------\n");
  int nilind = 0;
  for (int ih = 0; ih <= td._th; ih++) {
    // int stplh = 0;
    int rlsp = (((pow(2, (td._th - ih)) - 1) * 2) * td._keyw) / 2;
    //  _print("ih=%i, rlsp=%i",ih,rlsp);
    //_printsp((td._th - ih) * td._keyw); // indent

    _printsp(nilind);
    int nlev2_size = pow(2, ih + 1);
    BSTNode **nlev2 = (BSTNode **)malloc(sizeof(BSTNode *) * nlev2_size);
    int nlev2_i = 0;
    for (int ni = 0; ni < nlev_size; ni++) {
      BSTNode *cnode = nlev[ni];
      if (nlev[ni] != NULL) {
        int dig = _count_digits(cnode->_key);
        _printsp(rlsp);
        _printrp(td._keyw - dig, ' ');
        _print("%lld", cnode->_key);
        _printsp(rlsp);
        nlev2[nlev2_i++] = cnode->_left;
        nlev2[nlev2_i++] = cnode->_right;
        _printsp(td._keyw);
        //    stplh = 1;
      }
      else {
        // TODO: cut tree NULLs down
        //  if (ih == td._plh && stplh==0)
        //  {
        //    nilind = rlsp * 2 + td._keyw * 2;
        //    stplh=1;
        //  }
        //  else
        {
          _printsp(rlsp);
          _printrp(td._keyw - 1, ' ');
          _print(" ");  // _print("N");
          _printsp(rlsp);
          _printsp(td._keyw);
          nlev2[nlev2_i++] = NULL;
          nlev2[nlev2_i++] = NULL;
        }
      }
    }

    free(nlev);
    nlev = nlev2;
    nlev_size = nlev2_size;
    _print("\n");
  }
  free(nlev);
  _print("----------------------------\n");
}
void tree_insert(BST *pBST, void *satellite, int64 key) {
  BSTNode *b = (BSTNode *)malloc(sizeof(BSTNode));
  b->_key = key;
  b->_satellite = satellite;
  b->_left = NULL;
  b->_right = NULL;

  if (pBST->_root == NULL) {
    b->_parent = NULL;
    pBST->_root = b;
  }
  else {
    BSTNode *node = pBST->_root;
    BSTNode *pn = node;

    while (pn != NULL) {
      if (key < pn->_key) {
        if (pn->_left != NULL) {
          pn = pn->_left;
        }
        else {
          pn->_left = b;
          b->_parent = pn;
          break;
        }
      }
      else {
        if (pn->_right != NULL) {
          pn = pn->_right;
        }
        else {
          pn->_right = b;
          b->_parent = pn;
          break;
        }
      }
    }
  }
}
void in_order(BSTNode *pn, NODEVISITFUNC func) {
  if (pn == NULL) {
    return;
  }
  in_order(pn->_left, func);
  func(pn->_satellite);
  in_order(pn->_right, func);
}
void pre_order(BSTNode *pn, NODEVISITFUNC func) {
  if (pn == NULL) {
    return;
  }
  func(pn->_satellite);
  pre_order(pn->_left, func);
  pre_order(pn->_right, func);
}
void post_order(BSTNode *pn, NODEVISITFUNC func) {
  if (pn == NULL) {
    return;
  }
  post_order(pn->_left, func);
  post_order(pn->_right, func);
  func(pn->_satellite);
}
void *serach_bst(BSTNode *n, int64 key) {
  if (n == NULL || n->_key == key) {
    return n->_satellite;
  }
  if (key < n->_key) {
    return serach_bst(n->_left, key);
  }
  else {
    return serach_bst(n->_right, key);
  }
}
BSTNode *tree_minimum(BSTNode *p) {
  BSTNode *pr = p;
  while (pr->_left != NULL) {
    pr = pr->_left;
  }
  return pr;
}
BSTNode *tree_succesor(BSTNode *p) {
  if (p->_right != NULL) {
    return tree_minimum(p->_right);
  }
  else {
    BSTNode *x = p;
    BSTNode *y = p->_parent;
    while (y != NULL && x == y->_right) {
      x = y;
      y = y->_parent;
    }
  }
  return NULL;
}
void transplant(BST *pBST, BSTNode *u, BSTNode *v) {
  _print("  transplant: u=%lld with v=%lld\n", u->_key, v ? v->_key : -999);
  _print("            : u is null? %i v is null? %i\n", u ? 0 : 1, v ? 0 : 1);
  _print("            : u->_parent=%lld , v->_parent=%lld\n", u->_parent ? u->_parent->_key : -999, v ? (v->_parent ? v->_parent->_key : -999) : -999999);
  if (u->_parent == NULL) {
    _print("    transplant A");
    _print(" -> root (%lld) = v (%lld)\n", pBST->_root->_key, v ? v->_key : -999);
    pBST->_root = v;
  }
  else if (u->_parent->_left == u) {
    _print("    transplant B");
    _print(" -> u->parent->left (%lld) = v (%lld)\n", u->_parent->_left->_key, v ? v->_key : -999);
    u->_parent->_left = v;
  }
  else {
    _print("    transplant C");
    _print("-> u->parent->right (%lld) = v (%lld)\n", u->_parent->_right->_key, v ? v->_key : -999);
    u->_parent->_right = v;
  }
  if (v != NULL) {
    _print("    v->_parent = u->_parent");
    _print(" (%lld)\n", u->_parent ? u->_parent->_key : -999);
    v->_parent = u->_parent;
  }
}
void tree_delete(BST *pBST, BSTNode *z, int64 key) {
  if (z == NULL) {
    return;
  }
  else if (z->_key == key) {
    if (z->_left == NULL) {
      _print("delete A, z=%lld\n", key);
      transplant(pBST, z, z->_right);
    }
    else if (z->_right == NULL) {
      _print("delete B, z=%lld\n", key);
      transplant(pBST, z, z->_left);
    }
    else {
      _print("delete C, z=%lld\n", key);

      BSTNode *y = tree_minimum(z->_right);  //
      msg("tree min = %lld", y->_key);
      if (y != z->_right) {
        msg("got here1");
        //  transplant(pBST, y, y->_right);
        //  msg("got here2");
        y->_right = z->_right;
        y->_right->_parent = y;
      }

      // msg("got here3");
      transplant(pBST, z, y);
      //  msg("got here4");
      y->_left = z->_left;
      y->_left->_parent = y;
    }

    free(z);
  }
  else if (key < z->_key) {
    tree_delete(pBST, z->_left, key);
  }
  else {
    tree_delete(pBST, z->_right, key);
  }
}
// thunks
void InOrder(BST *pBST, NODEVISITFUNC func) {
  in_order(pBST->_root, func);
}
void PreOrder(BST *pBST, NODEVISITFUNC func) {
  pre_order(pBST->_root, func);
}
void PostOrder(BST *pBST, NODEVISITFUNC func) {
  post_order(pBST->_root, func);
}
void *Search(BST *pBST, int64 key) {
  return serach_bst(pBST->_root, key);
}
void TreeDelete(BST *pBST, int64 key) {
  msg("delete, tree pre:");
  print_tree(pBST);
  tree_delete(pBST, pBST->_root, key);
}
void TreeInsert(BST *pBST, void *satellite, int64 key) {
  tree_insert(pBST, satellite, key);
}
