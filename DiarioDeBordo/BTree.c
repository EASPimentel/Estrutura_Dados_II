#include <stdio.h>
#include <stdlib.h>
#define MAX 3
#define MIN 1
#define ORDER 4

typedef struct PageBTree {
    int nodes[ORDER];
    struct PageBTree *children[ORDER];
    int numNodesInPage;
    int isLeaf;
} PageBTree;

PageBTree* CreateNode(int isLeaf) {
    PageBTree* newNode = (PageBTree*)malloc(sizeof(PageBTree));
    newNode->isLeaf = isLeaf;
    newNode->numNodesInPage = 0;
    for (int i = 0; i < ORDER; i++)
        newNode->children[i] = NULL;
    return newNode;
}

void SplitChild(PageBTree* parent, int index) {
    PageBTree* child = parent->children[index];
    PageBTree* newChild = CreateNode(child->isLeaf);
    newChild->numNodesInPage = MIN;

    for (int j = 0; j < MIN; j++)
        newChild->nodes[j] = child->nodes[j + MIN + 1];

    if (!child->isLeaf) {
        for (int j = 0; j <= MIN; j++)
            newChild->children[j] = child->children[j + MIN + 1];
    }

    child->numNodesInPage = MIN;
    for (int j = parent->numNodesInPage; j >= index + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[index + 1] = newChild;

    for (int j = parent->numNodesInPage - 1; j >= index; j--)
        parent->nodes[j + 1] = parent->nodes[j];

    parent->nodes[index] = child->nodes[MIN];
    parent->numNodesInPage++;
}

void InsertNonFull(PageBTree* node, int key) {
    int i = node->numNodesInPage - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->nodes[i]) {
            node->nodes[i + 1] = node->nodes[i];
            i--;
        }
        node->nodes[i + 1] = key;
        node->numNodesInPage++;
    } else {
        while (i >= 0 && key < node->nodes[i])
            i--;

        i++;
        if (node->children[i]->numNodesInPage == MAX) {
            SplitChild(node, i);
            if (key > node->nodes[i])
                i++;
        }
        InsertNonFull(node->children[i], key);
    }
}

void Insert(PageBTree** root, int key) {
    if (*root == NULL) {
        *root = CreateNode(1);
        (*root)->nodes[0] = key;
        (*root)->numNodesInPage = 1;
    } else {
        if ((*root)->numNodesInPage == MAX) {
            PageBTree* newRoot = CreateNode(0);
            newRoot->children[0] = *root;
            SplitChild(newRoot, 0);
            int i = 0;
            if (newRoot->nodes[0] < key)
                i++;
            InsertNonFull(newRoot->children[i], key);
            *root = newRoot;
        } else {
            InsertNonFull(*root, key);
        }
    }
}

void Search(PageBTree* root, int key) {
    if (root == NULL) {
        printf("Chave %d não encontrada.\n", key);
        return;
    }

    int i = 0;
    while (i < root->numNodesInPage && key > root->nodes[i])
        i++;

    if (i < root->numNodesInPage && root->nodes[i] == key) {
        printf("Chave %d encontrada.\n", key);
        return;
    }

    if (root->isLeaf) {
        printf("Chave %d nao encontrada.\n", key);
        return;
    }

    Search(root->children[i], key);
}

void printBTree(PageBTree* node, int level) {
    if (node == NULL) return;

    // Imprimir cada nível da árvore
    printf("Nível %d: ", level);
    for (int i = 0; i < node->numNodesInPage; i++) {
        printf("%d ", node->nodes[i]);
    }
    printf("\n");

    // Recursivamente imprimir os filhos
    if (!node->isLeaf) {
        for (int i = 0; i <= node->numNodesInPage; i++) {
            printBTree(node->children[i], level + 1);
        }
    }
}

int main() {
    PageBTree* root = NULL;

    Insert(&root, 10);
    Insert(&root, 20);
    Insert(&root, 5);
    Insert(&root, 6);
    Insert(&root, 12);
    Insert(&root, 30);
    Insert(&root, 7);
    Insert(&root, 17);

    Search(root, 6);
    Search(root, 15);

    Insert(&root, 11);
    Insert(&root, 24);
    Insert(&root, 53);
    Insert(&root, 60);
    Insert(&root, 1);
    Insert(&root, 38);
    Insert(&root, 71);
    Insert(&root, 170);

    Search(root, 69);
    Search(root, 170);
    printf("Conteudo da arvore B:\n");
    printBTree(root, 0);  // Chama a função para imprimir a árvore

    return 0;
}
