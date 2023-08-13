#include <stdio.h>
#include <stdlib.h>

// Binary Search Tree Node
struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Function to create a new node
struct TreeNode* createNode(int data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a node into the BST
struct TreeNode* insertNode(struct TreeNode* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insertNode(root->left, data);
    } else if (data > root->data) {
        root->right = insertNode(root->right, data);
    }

    return root;
}

// Function to search for a node in the BST
struct TreeNode* searchNode(struct TreeNode* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }

    if (data < root->data) {
        return searchNode(root->left, data);
    }

    return searchNode(root->right, data);
}

// Function to find the minimum value node in a BST
struct TreeNode* findMinNode(struct TreeNode* node) {
    struct TreeNode* current = node;

    while (current && current->left != NULL) {
        current = current->left;
    }

    return current;
}

// Function to delete a node from the BST
struct TreeNode* deleteNode(struct TreeNode* root, int data) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        // Case 1: No child or one child
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Case 2: Two children
        struct TreeNode* temp = findMinNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

// Function to traverse the BST in Inorder
void inorderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}
void pretravarsal(struct TreeNode *root){
    if(root != NULL){
        printf("%d " , root ->data);
        pretravarsal(root->left);
        pretravarsal(root->right);
    }
}

void posttravarsal(struct TreeNode *root){
    if(root != NULL){
        posttravarsal(root->left);
        posttravarsal(root->right);
        printf("%d " , root ->data);
    }
}

// Test the BST operations
int main() {
    struct TreeNode* root = NULL;

    // Insert nodes into the BST
    root = insertNode(root, 50);
    root = insertNode(root, 30);
    root = insertNode(root, 20);
    root = insertNode(root, 40);
    root = insertNode(root, 70);
    root = insertNode(root, 60);
    root = insertNode(root, 80);

    // Search for a node
    struct TreeNode* searchResult = searchNode(root, 40);
    if (searchResult != NULL) {
        printf("Node found: %d\n", searchResult->data);  // Output: Node found: 40
    } else {
        printf("Node not found.\n");
    }

    // Delete a node
    root = deleteNode(root, 30);

    // Traverse the BST
    printf("Inorder Traversal: ");
    inorderTraversal(root);  // Output: 20 40 50 60 70 80
    printf("\n");

    return 0;
}
