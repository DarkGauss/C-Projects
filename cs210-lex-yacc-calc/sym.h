#ifndef SYMTBL_H
#define SYMTBL_H

#define NSYMS   (3)

struct sym {
    char * name;
    double value;
};

typedef struct _bst_node bst_node_t;
struct _bst_node{
    struct sym * data;
    bst_node_t* left;
    bst_node_t* right;
};


bst_node_t * new_node(struct sym*);
bst_node_t * search_tree(char *,bst_node_t *);
bst_node_t * add_tree(struct sym*,bst_node_t *);
struct sym * sym_lookup(char *);
struct sym * new_sym(char*);
void print_tree(bst_node_t *);
#endif /* SYMTBL_H */
