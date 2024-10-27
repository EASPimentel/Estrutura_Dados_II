#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node{
    int key, priority;
    struct Node* left;
    struct Node* right;
}Node;

//create a random priority node
Node* CreateNode(int key){
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->priority =rand() % 100;
    node->left = node->right = NULL;
    return node;
}

Node* RotateRight(Node* desbalanced){
    Node* balanced = desbalanced->left;
    desbalanced->left = balanced ->right;
    balanced->right = desbalanced;
    return balanced;
}

Node* RotateLeft(Node* desbalanced){
    Node* balanced = desbalanced->right;
    desbalanced->right = balanced ->left;
    balanced->left = desbalanced;
    return balanced;
}

Node* Insert(Node* root, int key){
    if (root == NULL)
        return CreateNode(key);

    if(key < root->key ){
        root->left = Insert(root->left, key);
        if(root->left->priority > root->priority)
            root = RotateRight(root);
    }
    else if (key > root->key){
        root->right = Insert(root->right, key);
        if(root->right->priority > root -> priority)
            root = RotateLeft(root);
    }
    return root;
}

Node * DeleteNode(Node* root, int key){
    if(root == NULL)
        return root;

    if(key < root->key)
        root->left = DeleteNode(root->left, key);

    else if (key > root->key)
        root->right = DeleteNode(root->right, key);
    else{

        if(root->left == NULL){
            Node * temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL){
            Node* temp = root-> left;
            free(root);
            return temp;
        }

        if(root->left->priority < root->right->priority){
            root = RotateLeft(root);
            root->left = DeleteNode(root->left, key);
        }else{
            root = RotateRight(root);
            root->right = DeleteNode(root->right, key);
        }
    }
    return root;
}

Node* Search(Node* root, int key){

    if(root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return Search(root->left, key);

    return Search(root->right, key);

}

void PrintInOrder(Node* root){
    if(root){
        PrintInOrder(root->left);
        printf("Key: %d, Priority: %d\n", root-> key, root->priority);
        PrintInOrder(root->right);
    }
}
void PrintPreOrder(Node* root){
    if(root){
        printf("Key: %d, Priority: %d\n", root-> key, root->priority);
        PrintPreOrder(root->left);
        PrintPreOrder(root->right);
    }
}
void PrintPosOrder(Node* root){
    if(root){
        PrintPosOrder(root->left);
        PrintPosOrder(root->right);
        printf("Key: %d, Priority: %d\n", root-> key, root->priority);
    }
}
int main()
{
    srand(time(0));

    Node* root = NULL;

    root = Insert(root,5);
    root = Insert(root,6);
    root = Insert(root,12);
    root = Insert(root,55);
    root = Insert(root,23);
    root = Insert(root,2);
    root = Insert(root,32);
    root = Insert(root,17);
    root = Insert(root,40);
    root = Insert(root,9);



    PrintPosOrder(root);

    Node* search = Search(root,55);
    if(search != NULL)
        printf("Result: key: %d, Priority: %d\n", search->key,search->priority);
    else
        printf("F");

    return 0;
}
