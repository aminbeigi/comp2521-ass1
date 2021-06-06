/**
 * Name: Amin Ghasembeigi
 * ZID:  z5555555
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Record.h"
#include "AVLTree.h"

typedef struct node *Node;
struct node {
    Record rec;
    Node   left;
    Node   right;
    int    height;
};

struct tree {
    Node    root;
    int     (*compare)(Record, Record);
};

////////////////////////////////////////////////////////////////////////
// Auxiliary functions

static void doTreeFree(Node n, bool freeRecords);
static Node newNode(Record rec);
static Record doTreeSearch(Tree t, Node n, Record rec);

////////////////////////////////////////////////////////////////////////

static Node newNode(Record rec) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    n->rec = rec;
    n->left = NULL;
    n->right = NULL;
    n->height = 0;
    return n;
}

////////////////////////////////////////////////////////////////////////

Tree TreeNew(int (*compare)(Record, Record)) {
    Tree t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    t->root = NULL;
    t->compare = compare;
    return t;
}

////////////////////////////////////////////////////////////////////////

void TreeFree(Tree t, bool freeRecords) {
    doTreeFree(t->root, freeRecords);
    free(t);
}

static void doTreeFree(Node n, bool freeRecords) {
    if (n != NULL) {
        doTreeFree(n->left, freeRecords);
        doTreeFree(n->right, freeRecords);
        if (freeRecords) {
            RecordFree(n->rec);
        }
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////

Record TreeSearch(Tree t, Record rec) {
    return doTreeSearch(t, t->root, rec);
}

static Record doTreeSearch(Tree t, Node n, Record rec) {
    if (n == NULL) {
        return NULL;
    }

    int cmp = t->compare(rec, n->rec);
    if (cmp < 0) {
        return doTreeSearch(t, n->left, rec);
    } else if (cmp > 0) {
        return doTreeSearch(t, n->right, rec);
    } else {
        return n->rec;
    }
}


////////////////////////////////////////////////////////////////////////
/* IMPORTANT: 
   Do NOT modify the code above this line. 
   You must not modify the 'node' and 'tree' structures defined above.
   You must not modify the functions defined above.
*/
////////////////////////////////////////////////////////////////////////

static int height(Node n);
static Node rotateRight(Node n1);
static Node rotateLeft(Node n2);

/////////////////////////////////////////////////////////////////////////////////
//                                AVLTree ADT                                  //
/////////////////////////////////////////////////////////////////////////////////

/**
 * wrapper for TreeInsert
 * source: week 2 lecture slide (6/9) pseudocode
 */
static Node doTreeInsert(Tree t, Node n, Record rec, bool* res) {
    if (n == NULL) {
        Node newNodeInsert = newNode(rec);
        return newNodeInsert;
    }

     int cmp = t->compare(rec, n->rec);

    if (cmp == 0) {
        // rec already exists in tree
        *res = false;
        return n;

    } else if (cmp < 0) {
        // insert into left subtree
        n->left = doTreeInsert(t, n->left, rec, res);

    } else {
        // insert into right subtree
        n->right = doTreeInsert(t, n->right, rec, res);
    }

    int lHeight = height(n->left);
    int rHeight = height(n->right);

    if ((lHeight - rHeight) > 1) {
        // tree is unbalanced
        // left subtree is greater than right
         int cmpLeft = t->compare(rec, n->left->rec);
        if (cmpLeft > 0) {
            n->left = rotateLeft(n->left);
        }
        n = rotateRight(n);

    } else if ((rHeight - lHeight) > 1) {
        // tree is unbalanced
        // right subtree is greater than left
         int cmpRight = t->compare(rec, n->right->rec);
        if (cmpRight < 0) {
            n->right = rotateRight(n->right);
        }
        n = rotateLeft(n);
    }

    return n;
}

/**
 * returns the smallest record greater than or equal to the given record
 * r (according to the comparison function), or NULL if there is no such
 * record.
 */
bool TreeInsert(Tree t, Record rec) {
    bool res = true;
    t->root = doTreeInsert(t, t->root, rec, &res);
    return res;
}

/**
 * searches for all records between the two given records, inclusive
 * (according to the comparison function) and returns the records in a
 * list in order.
 * source: Shirley Zhou (tutor)
 */
static void doTreeSearchBetween(Tree t, Node n, Record lower, Record upper, List l) {
    if (n == NULL) {
        return;
    }
  
    if (t->compare(n->rec, lower) < 0) { 
        // look right for larger values
        doTreeSearchBetween(t, n->right, lower, upper, l); 
  
    } else if (t->compare(n->rec, upper) > 0) {
        // look left for smaller values
        doTreeSearchBetween(t, n->left, lower, upper, l);
  
    } else { 
        // in the correct range of values 
        // move left and right and add values to the list
        doTreeSearchBetween(t, n->left, lower, upper, l); 
        ListAppend(l, n->rec);
        doTreeSearchBetween(t, n->right, lower, upper, l); 
    }
}

/**
 * wrapper for TreeSearchBetween
 */
List TreeSearchBetween(Tree t, Record lower, Record upper) {
    List l = ListNew();
    doTreeSearchBetween(t, t->root, lower, upper, l);
    return l;
}

/**
 * wrapper for TreeNext
 */
static void doTreeNext(Tree t, Node n, Record r, Record* nextFlight) {
    if (n == NULL) {
        return;
    }

    if (t->compare(n->rec, r) < 0) {
        // look right for larger values
        return doTreeNext(t, n->right, r, nextFlight); 
  
    } else {
        // look left for smaller values
        *nextFlight = n->rec; // update nextFlight with current earliest next flight
        return doTreeNext(t, n->left, r, nextFlight); 
    }
}

/**
 * returns the smallest record greater than or equal to the given record
 * r (according to the comparison function), or NULL if there is no such
 * record.
 */
Record TreeNext(Tree t, Record r) {
    Record nextFlight = NULL;
    doTreeNext(t, t->root, r, &nextFlight);
    return nextFlight;
}

//////////////////////////////////////////////////////////////////////////////////
//                               helper functions                               //
//////////////////////////////////////////////////////////////////////////////////

/**
 * returns the maximum height of a tree
 */
static int height(Node n) {
    if (n == NULL) {
        return -1;
    }

    // compute the height of each subtree
    int leftHeight = height(n->left);
    int rightHeight = height(n->right);

    // use the larger one
    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    }
    return rightHeight + 1;
}

/**
 * rotates right around the given node
 * source: week 2 lecture exercises
 */
static Node rotateRight(Node n1) {
    if (n1 == NULL || n1->left == NULL)
        return n1;
    Node n2 = n1->left;
    n1->left = n2->right;
    n2->right = n1;
    return n2;
}

/**
 * rotates left around the given node
 * source: week 2 lecture exercises
 */
static Node rotateLeft(Node n2) {
    if (n2 == NULL || n2->right == NULL)
        return n2;
    Node n1 = n2->right;
    n2->right = n1->left;
    n1->left = n2;
    return n1;
}

//////////////////////////////////////////////////////////////////////////////////
//                              testing functions                               //
//////////////////////////////////////////////////////////////////////////////////

/**
 * wrapper for PrintTreePrefix
 */
static void doPrintTreePrefix(Node n) {
    if (n == NULL) {
        return;
    }
    printf("%s ", RecordGetFlightNumber(n->rec));
    doPrintTreePrefix(n->left);
    doPrintTreePrefix(n->right);
}

/**
 * prints the values in the given Tree in prefix order
 */
void PrintTreePrefix(Tree t) {
    doPrintTreePrefix(t->root);
    printf("\n");
}
