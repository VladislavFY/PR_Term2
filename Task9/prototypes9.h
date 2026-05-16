#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALUE_LEN 1024

typedef struct TreeNode {

 struct TreeNode *parent;

 struct TreeNode *left_child;

 struct TreeNode *right_child;

 char *value;

} TreeNode;

typedef struct {

 TreeNode *root;

 TreeNode *current;

} Tree;

char *copy_string(const char *value);
TreeNode *create_node(const char *value, TreeNode *parent);
Tree *create_tree(const char *value);
void free_subtree(TreeNode *node);
void free_tree(Tree *tree);
int add_left(Tree *tree, const char *value);
int add_right(Tree *tree, const char *value);
int delete_left(Tree *tree);
int delete_right(Tree *tree);
int move_parent(Tree *tree);
int move_left(Tree *tree);
int move_right(Tree *tree);
void print_node(TreeNode *node, int depth, const char *name, TreeNode *current);
void print_tree(Tree *tree);
void print_commands(void);
void clear_input(void);
int main_function(void);
