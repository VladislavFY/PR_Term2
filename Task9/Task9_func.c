#include "prototypes9.h"

char *copy_string(const char *value) {
 char *new_value;
  
 if (value == NULL) {
  return NULL;
 }

 new_value = (char *)malloc((strlen(value) + 1) * sizeof(char));

 if (new_value == NULL) {
  return NULL;
 }
  
 strcpy(new_value, value);
 return new_value;
}

TreeNode *create_node(const char *value, TreeNode *parent) {
 TreeNode *node;
 node = (TreeNode *)malloc(sizeof(TreeNode));

 if (node == NULL) {
  return NULL;
 }

 node->value = copy_string(value);

 if (node->value == NULL) {
  free(node);
  return NULL;
 }

 node->parent = parent;
 node->left_child = NULL;
 node->right_child = NULL;
 return node;
}

Tree *create_tree(const char *value) {
 Tree *tree;

 tree = (Tree *)malloc(sizeof(Tree));

 if (tree == NULL) {
  return NULL;
 }

 tree->root = create_node(value, NULL);

 if (tree->root == NULL) {
  free(tree);
  return NULL;
 }

 tree->current = tree->root;
 return tree;
}

void free_subtree(TreeNode *node) {
 if (node == NULL) {
  return;
 }

 free_subtree(node->left_child);
 free_subtree(node->right_child);
 free(node->value);
 free(node);
}

void free_tree(Tree *tree) {
 if (tree == NULL) {
  return;
 }

 free_subtree(tree->root);
 free(tree);
}

int add_left(Tree *tree, const char *value) {
 TreeNode *node;

 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->left_child != NULL) {
  return -2;
 }

 node = create_node(value, tree->current);

 if (node == NULL) {
  return -3;
 }

 tree->current->left_child = node;
 return 0;
}

int add_right(Tree *tree, const char *value) {
 TreeNode *node;
  
 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->right_child != NULL) {
  return -2;
 }

 node = create_node(value, tree->current);

 if (node == NULL) {
  return -3;
 }

 tree->current->right_child = node;
 return 0;
}

int delete_left(Tree *tree) {

 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->left_child == NULL) {
  return -2;
 }

 free_subtree(tree->current->left_child);
 tree->current->left_child = NULL;

 return 0;
}

int delete_right(Tree *tree) {
 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->right_child == NULL) {
  return -2;
 }

 free_subtree(tree->current->right_child);
 tree->current->right_child = NULL;
 return 0;
}

int move_parent(Tree *tree) {
 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->parent == NULL) {
  return -2;
 }

 tree->current = tree->current->parent;
 return 0;
}

int move_left(Tree *tree) {
 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->left_child == NULL) {
  return -2;
 }

 tree->current = tree->current->left_child;
 return 0;
}

int move_right(Tree *tree) {
 if (tree == NULL || tree->current == NULL) {
  return -1;
 }

 if (tree->current->right_child == NULL) {
  return -2;
 }

 tree->current = tree->current->right_child;
 return 0;
}

void print_node(TreeNode *node, int depth, const char *name, TreeNode *current) {
 int i;

 if (node == NULL) {
  return;
 }

 for (i = 0; i < depth; ++i) {
  printf("  ");
 }

 printf("%s: %s", name, node->value);

 if (node == current) {
  printf(" <- current");
 }

 printf("\n");
 print_node(node->left_child, depth + 1, "left", current);
 print_node(node->right_child, depth + 1, "right", current);
}

void print_tree(Tree *tree) {
 if (tree == NULL || tree->root == NULL) {
  printf("Tree is empty.\n");
  return;
 }

 print_node(tree->root, 0, "root", tree->current);
}

void print_commands(void) {
 printf("\nCommands:\n");
 printf("1 - create tree\n");
 printf("2 - add left child\n");
 printf("3 - add right child\n");
 printf("4 - delete left subtree\n");
 printf("5 - delete right subtree\n");
 printf("6 - move to parent\n");
 printf("7 - move to left child\n");
 printf("8 - move to right child\n");
 printf("9 - print tree\n");
 printf("0 - exit\n");
}

void clear_input(void) {
 int c;
 while ((c = getchar()) != '\n' && c != EOF) {
 }
}

int main_function(void) {
 Tree *tree = NULL;
 char value[VALUE_LEN];
 int command;
 int status;
 size_t len;

 while (1) {

  print_commands();
  printf("Enter command: ");

  if (scanf("%d", &command) != 1) {
   printf("Invalid command input!\n");
   clear_input();
   continue;
  }

  clear_input();

  if (command == 0) {
   free_tree(tree);
   return 0;
  }

  if (command == 1) {
   printf("Enter root value: ");
   if (fgets(value, VALUE_LEN, stdin) == NULL) {
    printf("Invalid value input!\n");
    continue;
   }

   len = strlen(value);
   if (len > 0 && value[len - 1] == '\n') {
    value[len - 1] = '\0';
   } else {
    clear_input();
   }

   if (strlen(value) == 0) {
    printf("Error: empty value.\n");
    continue;
   }

   free_tree(tree);
   tree = create_tree(value);
   if (tree == NULL) {
    printf("Error: memory allocation failed.\n");
    return -1;
   }
   printf("Tree created.\n");
   continue;
  }

  if (tree == NULL) {
   printf("Error: tree is not created.\n");
   continue;
  }

  if (command == 2) {
   printf("Enter left child value: ");
   if (fgets(value, VALUE_LEN, stdin) == NULL) {
    printf("Invalid value input!\n");
    continue;
   }

   len = strlen(value);
   if (len > 0 && value[len - 1] == '\n') {
    value[len - 1] = '\0';
   } else {
    clear_input();
   }

   if (strlen(value) == 0) {
    printf("Error: empty value.\n");
    continue;
   }

   status = add_left(tree, value);
   if (status == 0) {
    printf("Left child added.\n");
   } else if (status == -2) {
    printf("Error: left child already exists.\n");
   } else {
    printf("Error: left child was not added.\n");
   }

  } else if (command == 3) {
   printf("Enter right child value: ");
   if (fgets(value, VALUE_LEN, stdin) == NULL) {
    printf("Invalid value input!\n");
    continue;
   }

   len = strlen(value);
   if (len > 0 && value[len - 1] == '\n') {
    value[len - 1] = '\0';
   } else {
    clear_input();
   }

   if (strlen(value) == 0) {
    printf("Error: empty value.\n");
    continue;
   }

   status = add_right(tree, value);
   if (status == 0) {
    printf("Right child added.\n");
   } else if (status == -2) {
    printf("Error: right child already exists.\n");
   } else {
    printf("Error: right child was not added.\n");
   }

  } else if (command == 4) {
   status = delete_left(tree);
   if (status == 0) {
    printf("Left subtree deleted.\n");
   } else {
    printf("Error: left subtree is empty.\n");
   }

  } else if (command == 5) {
   status = delete_right(tree);
   if (status == 0) {
    printf("Right subtree deleted.\n");
   } else {
    printf("Error: right subtree is empty.\n");
   }

  } else if (command == 6) {
   status = move_parent(tree);
   if (status == 0) {
    printf("Moved to parent.\n");
   } else {
    printf("Error: parent does not exist.\n");
   }

  } else if (command == 7) {
   status = move_left(tree);
   if (status == 0) {
    printf("Moved to left child.\n");
   } else {
    printf("Error: left child does not exist.\n");
   }

  } else if (command == 8) {
   status = move_right(tree);
   if (status == 0) {
    printf("Moved to right child.\n");
   } else {
    printf("Error: right child does not exist.\n");
   }

  } else if (command == 9) {
   print_tree(tree);
  } else {
   printf("Unknown command.\n");
  }
 }
}
