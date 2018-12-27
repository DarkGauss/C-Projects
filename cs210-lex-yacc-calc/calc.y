%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sym.h"
int count = 0;
bst_node_t * head = NULL;        //head for bst tree
%}
%union {
    double dval;
    struct sym * symptr;
}
%token <symptr> CONST
%token <symptr> NAME
%token <dval> NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS
%nonassoc PRINT

%type <dval> expression
%%
statement_list
    : statement '\n'
    | statement_list statement '\n'
    ;

statement
    : NAME '=' expression { $1->value = $3; }
    | CONST '=' expression { printf("assign to const\n"); }
    | expression { printf("= %g\n", $1); }
    | PRINT {print_tree(head); }
    ;

expression
    : expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { $$ = $3==0?printf("divide by zero\n"),$1:$1 / $3; }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')' { $$ = $2; }
    | NUMBER
    | NAME { $$ = $1->value; }
    | CONST { $$ = $1->value; }
    ;

%%

struct sym * sym_lookup(char * s)
{
    bst_node_t * temp;
    temp = search_tree(s,head);
    if(temp == NULL){
        count++;
        temp = add_tree(new_sym(s),head);
        }
    if(strcmp(s,"PHI")== 0){
        temp->data->value = 1.61803;
    }
    if(strcmp(s,"PI")== 0){
        temp->data->value = 3.14159;
    }
    return temp->data;
}
bst_node_t * new_node(struct sym* in){
    bst_node_t * temp = malloc(sizeof(bst_node_t));
    temp->left = NULL;
    temp->right = NULL;
    temp->data = in;
    return temp;
}
struct sym * new_sym(char* in){
    struct sym * temp = malloc(sizeof(struct sym));
    temp->name = strdup(in);
    temp->value = 0.0;
    return temp;
    }
bst_node_t * search_tree(char * in, bst_node_t * root){
    bst_node_t * current = root;
    if(current == NULL){
        return NULL;
    }else{
        int cmp = strcmp(in,current->data->name);
        if(cmp > 0){
            return search_tree(in,current->right); //branch right if in greator
        }else if(cmp < 0){
            return search_tree(in,current->left); //branch left if less
        }else{
            return current;
            }
        }
}
bst_node_t * add_tree(struct sym* in, bst_node_t * root){
   bst_node_t * current = root;
    if(head == NULL){   //WARNING CHECKING GLOBAL
        head = new_node(in);
        return head;
    }else{
        int cmp = strcmp(in->name,current->data->name);
        if(cmp == 0){   ///redundent since we added
            return current;
        }
        if(cmp > 0){
            if(current->right == NULL){
                current->right = new_node(in);
                return current->right;
            }else{
                return add_tree(in,current->right); //branch right if in greator
            }
        }else if(cmp < 0){
            if(current->left == NULL){
                current->left = new_node(in);
                return current->left;
            }else{
                return add_tree(in,current->left); //branch right if in greator
            }
        }
    }
};

void print_tree(bst_node_t * root){
    if(root == head){
        if(count == 0){
            sym_lookup("PI");
            sym_lookup("PHI");
        }  //really bad way to do consts
        root = head; /// the code smell is real here
        printf("num-syms: %d\n",count);
    }
    if(root == 0){
        return;
    }
    print_tree(root->left);
    printf("\t%s => %g\n",root->data->name,root->data->value);
    print_tree(root->right);
}