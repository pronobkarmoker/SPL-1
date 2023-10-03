#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SAMPLES 4
#define NUM_FEATURES 3
#define NUM_CLASSES 2

// Define a struct for the decision tree node
typedef struct Node
{
    int feature_index;        // Index of the feature to split on (or -1 for leaf nodes)
    int threshold;            // Threshold value for the split (or -1 for leaf nodes)
    int class_label;          // Class label if it's a leaf node (or -1 for non-leaf nodes)
    struct Node *left_child;  // Pointer to the left child node
    struct Node *right_child; // Pointer to the right child node
} Node;

double log2(double x)
{
    return log(x) / log(2);
}

// Function to calculate the entropy of a set of class labels
double calculate_entropy(int *class_labels, int num_samples)
{
    double entropy = 0.0;

    // Count the number of samples in each class
    int class_counts[NUM_CLASSES] = {0};
    for (int i = 0; i < num_samples; i++)
    {
        int label = class_labels[i];
        class_counts[label]++;
    }

    // Calculate entropy using the formula: -p_i * log2(p_i)
    for (int i = 0; i < NUM_CLASSES; i++)
    {
        if (class_counts[i] > 0)
        {
            double probability = (double)class_counts[i] / num_samples;
            entropy -= probability * log2(probability);
        }
    }

    return entropy;
}

// Function to calculate the information gain for a given split
double calculate_information_gain(int *class_labels, int *feature_values, int num_samples, int threshold, int num_classes)
{
    // Calculate the total entropy before the split
    double total_entropy = calculate_entropy(class_labels, num_samples);

    // Split the data into two subsets based on the threshold
    int left_count = 0, right_count = 0;
    int left_class_counts[NUM_CLASSES] = {0};
    int right_class_counts[NUM_CLASSES] = {0};

    for (int i = 0; i < num_samples; i++)
    {
        if (feature_values[i] <= threshold)
        {
            left_count++;
            left_class_counts[class_labels[i]]++;
        }
        else
        {
            right_count++;
            right_class_counts[class_labels[i]]++;
        }
    }

    // Calculate the weighted average of the entropies of the two subsets
    double left_entropy = calculate_entropy(left_class_counts, left_count);
    double right_entropy = calculate_entropy(right_class_counts, right_count);
    double weighted_entropy = ((double)left_count / num_samples) * left_entropy + ((double)right_count / num_samples) * right_entropy;

    // Calculate the information gain
    double information_gain = total_entropy - weighted_entropy;

    return information_gain;
}

int get_unique_class(int class_labels[], int num_samples)
{
    int first_class = class_labels[0];
    for (int i = 1; i < num_samples; i++)
    {
        if (class_labels[i] != first_class)
        {
            return -1; // Not all samples belong to the same class
        }
    }
    return first_class;
}

Node *create_node(int feature_index, int threshold, int class_label)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    new_node->feature_index = feature_index;
    new_node->threshold = threshold;
    new_node->class_label = class_label;
    new_node->left_child = NULL;
    new_node->right_child = NULL;

    return new_node;
}

// Define a function to build the decision tree
void func(Node *root, int data[][NUM_FEATURES], int class_labels[], int num_samples, int num_features, int num_classes)
{
    // Base case: If all samples belong to the same class, create a leaf node
    int unique_class = get_unique_class(class_labels, num_samples);
    if (unique_class != -1)
    {
        root->feature_index = -1;
        root->threshold = -1;
        root->class_label = unique_class;
        root->left_child = NULL;
        root->right_child = NULL;
        return;
    }

    // Calculate the best split
    double max_information_gain = -1.0;
    int best_feature_index = -1;
    int best_threshold = -1;

    for (int i = 0; i < num_features; i++)
    {
        for (int j = 0; j < num_samples; j++)
        {
            int threshold = data[j][i];
            double information_gain = calculate_information_gain(class_labels, data[j], num_samples, threshold, num_classes);

            if (information_gain > max_information_gain)
            {
                max_information_gain = information_gain;
                best_feature_index = i;
                best_threshold = threshold;
            }
        }
    }

    // Set the current node's attributes
    root->feature_index = best_feature_index;
    root->threshold = best_threshold;
    root->class_label = -1; // This is an intermediate node, not a leaf

    // Split the data based on the best split
    int left_indices[MAX_SAMPLES];
    int right_indices[MAX_SAMPLES];
    int left_count = 0;
    int right_count = 0;

    for (int i = 0; i < num_samples; i++)
    {
        if (data[i][best_feature_index] <= best_threshold)
        {
            left_indices[left_count++] = i;
        }
        else
        {
            right_indices[right_count++] = i;
        }
    }

    // Create left and right child nodes
    root->left_child = create_node(-1, -1, -1);
    root->right_child = create_node(-1, -1, -1);

    // Recursively build left and right subtrees
    func(root->left_child, data, class_labels, left_count, num_features, num_classes);
    func(root->right_child, data, class_labels, right_count, num_features, num_classes);
}

// Define a function to make predictions using the decision tree
int decision_function(Node *root, int data[NUM_FEATURES])
{
    // Traverse the decision tree to make a prediction

    while (root != NULL)
    {
        // Check if this node is a leaf node
        if (root->left_child == NULL && root->right_child == NULL)
        {
            return root->class_label; // This is a leaf node, return the class label
        }

        // Decide which child to follow based on the feature value
        if (data[root->feature_index] <= root->threshold)
        {
            root = root->left_child; // Follow the left child
        }
        else
        {
            root = root->right_child; // Follow the right child
        }
    }

    // In case the tree traversal somehow reaches a NULL node, return a default value
    return -1; // You can choose an appropriate default value for unclassified cases
}

// Define the k-fold cross-validation function
void k_folds_cross_validation(int data[][NUM_FEATURES], int class_labels[], int num_samples, int num_features, int num_classes, int k)
{
    int folds = k;
    int accuracy = 0;

    int fold_size = num_samples / k;

    for (int i = 0; i < k; i++)
    {
        Node *root = (Node *)malloc(sizeof(Node)); // Allocate memory for the decision tree
        // Initialize your decision tree root here

        int data2[MAX_SAMPLES][NUM_FEATURES]; // Create a copy of the data

        // Call your function to build the decision tree
        func(root, data, class_labels, num_samples, num_features, num_classes);

        int expected = class_labels[i];
        printf("Expected Class: %d\n", expected);

        // Get the attributes for the current sample
        int attributes[NUM_FEATURES];
        for (int j = 0; j < NUM_FEATURES; j++)
        {
            attributes[j] = data[i][j];
        }

        // Implement your decision function and get the actual result
        int actual = decision_function(root, attributes);

        if (actual == expected)
        {
            accuracy++;
        }

        printf("Actual Class: %d\n", actual);
        printf("Accuracy: %d\n", accuracy);

        // Free memory allocated for the decision tree
        free(root);
    }

    printf("Average Accuracy is: %.2lf%%\n", ((double)accuracy / folds) * 100);
}

int main()
{
    // Sample data, replace with your dataset
    int data[MAX_SAMPLES][NUM_FEATURES] = {
        {5, 10, 15},
        {50, 55, 60},
        {100, 150, 200},
        {250, 300, 350}};

    int class_labels[MAX_SAMPLES] = {1, 1, 2, 2};
    int k = 4; // Number of folds
    k_folds_cross_validation(data, class_labels, MAX_SAMPLES, NUM_FEATURES, NUM_CLASSES, k);

    return 0;
}
