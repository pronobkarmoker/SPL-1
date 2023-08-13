#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SAMPLES 4

typedef struct Node
{
    int feature_index;
    int threshold;
    int class_label;
    struct Node *left_child;
    struct Node *right_child;
} Node;

double log2(double x)
{
    return log(x) / log(2);
}

double calculate_entropy(int *class_labels, int num_samples)
{
    int pos_count = 0;
    int neg_count = 0;

    for (int i = 0; i < num_samples; i++)
    {
        if (class_labels[i] == 1)
        {
            pos_count++;
        }
        else
        {
            neg_count++;
        }
    }

    double p_pos = (double)pos_count / num_samples;
    double p_neg = (double)neg_count / num_samples;

    double entropy = 0.0;
    if (p_pos != 0)
    {
        entropy -= p_pos * log2(p_pos);
    }
    if (p_neg != 0)
    {
        entropy -= p_neg * log2(p_neg);
    }

    return entropy;
}

double calculate_information_gain(int *class_labels, int *feature_values, int num_samples, int threshold, int num_classes)
{
    int left_count = 0;
    int right_count = 0;
    int *left_class_counts = (int *)malloc(num_classes * sizeof(int));
    int *right_class_counts = (int *)malloc(num_classes * sizeof(int));

    // Initialize the arrays to 0
    for (int i = 0; i < num_classes; ++i)
    {
        left_class_counts[i] = 0;
        right_class_counts[i] = 0;
    }

    for (int i = 0; i < num_samples; i++)
    {
        if (feature_values[i] <= threshold)
        {
            left_count++;
            left_class_counts[class_labels[i] - 1]++;
        }
        else
        {
            right_count++;
            right_class_counts[class_labels[i] - 1]++;
        }
    }

    double total_entropy = calculate_entropy(class_labels, num_samples);
    double left_entropy = calculate_entropy(left_class_counts, left_count);
    double right_entropy = calculate_entropy(right_class_counts, right_count);

    double p_left = (double)left_count / num_samples;
    double p_right = (double)right_count / num_samples;

    double information_gain = total_entropy - (p_left * left_entropy + p_right * right_entropy);

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

Node *build_decision_tree(int data[][3], int class_labels[], int num_samples, int num_features, int num_classes)
{
    // Base case: If all samples belong to the same class, create a leaf node
    int unique_class = get_unique_class(class_labels, num_samples);
    if (unique_class != -1)
    {
        return create_node(-1, -1, unique_class);
    }

    // Calculate the best split
    double max_information_gain = -1.0;
    int best_feature_index = -1;
    int best_threshold = -1;

    for (int i = 0; i < num_features; i++)
    {
        for (int j = 0; j < num_samples; j++)
        {
            int threshold = data[j][i]; // Calculate information gain for each value
            double information_gain = calculate_information_gain(class_labels, data[i], num_samples, threshold, num_classes);

            if (information_gain > max_information_gain)
            {
                max_information_gain = information_gain;
                best_feature_index = i;
                best_threshold = threshold;
            }
        }
    }

    // Create the current node
    Node *current_node = create_node(best_feature_index, best_threshold, -1);

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

    // Recursively build left and right subtrees
    current_node->left_child = build_decision_tree(data, class_labels, left_count, num_features, num_classes);
    current_node->right_child = build_decision_tree(data, class_labels, right_count, num_features, num_classes);

    return current_node;
}

void free_decision_tree(Node *node)
{
    if (node == NULL)
    {
        return;
    }

    // Recursively free left and right subtrees
    free_decision_tree(node->left_child);
    free_decision_tree(node->right_child);

    // Free the current node
    free(node);
}



int main()
{
    // Input data
    int data[4][3] = {
        {5, 10, 15},
        {50, 55, 60},
        {100, 150, 200},
        {250, 300, 350}};

    int class_labels[4] = {1, 1, 2, 2};
    int num_samples = 4;
    int num_classes = 2;
    int num_features = 3;

    // Calculate the information gain for each attribute value
    double max_information_gain = -1.0;
    int best_feature_index = -1;
    int best_threshold = -1;

    for (int i = 0; i < num_features; i++)
    {
        for (int j = 0; j < num_samples; j++)
        {
            int threshold = data[j][i]; // Calculate information gain for each value
            double information_gain = calculate_information_gain(class_labels, data[i], num_samples, threshold, num_classes);

            if (information_gain > max_information_gain)
            {
                max_information_gain = information_gain;
                best_feature_index = i;
                best_threshold = threshold;
            }
        }
    }

    // Build the decision tree
    Node *root = create_node(best_feature_index, best_threshold, -1); // Create root node
    root->left_child = create_node(-1, -1, 1);                        // Create left child node with class 1
    root->right_child = create_node(-1, -1, 2);                       // Create right child node with class 2

    // Print the selected root and its max information gain
    printf("Selected Root Feature: %d\n", best_feature_index);
    printf("Selected Threshold: %d\n", best_threshold);
    printf("Max Information Gain: %lf\n", max_information_gain);

    // Print the decision tree
    printf("Decision Tree:\n");
    //print_decision_tree_info(root, data, num_samples, num_features, num_classes);

    // Clean up: Free memory used by the decision tree
    free_decision_tree(root);

    return 0;
}
