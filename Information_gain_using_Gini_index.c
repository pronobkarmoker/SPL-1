#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define num_sample 5
#define num_class 2

double calculate_gini_index(int cls_label[], int num_samples, int num_classes)
{
    double gini = 1.0;

    int cls_count[num_classes];
    for (int i = 0; i < num_classes; i++)
    {
        cls_count[i] = 0;
    }

    for (int i = 0; i < num_samples; i++)
    {
        cls_count[cls_label[i] - 1]++;
    }
    for (int i = 0; i < num_classes; i++)
    {
        double p = (double)cls_count[i] / num_samples;
        gini -= p * p;
    }

    return gini;
}

double calculate_information_gain(int *class_labels, int *feature_values, int num_samples, int threshold, int num_classes)
{
    int left_count = 0;
    int right_count = 0;
    int *left_class_counts = (int *)malloc(num_classes * sizeof(int));
    int *right_class_counts = (int *)malloc(num_classes * sizeof(int));

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

    double total_gini = calculate_gini_index(class_labels, num_samples, num_classes);
    double left_gini = calculate_gini_index(left_class_counts, left_count, num_classes);
    double right_gini = calculate_gini_index(right_class_counts, right_count, num_classes);

    double p_left = (double)left_count / num_samples;
    double p_right = (double)right_count / num_samples;

    // Calculate information gain using Gini index
    double information_gain = total_gini - (p_left * left_gini + p_right * right_gini);

    // Free allocated memory
    free(left_class_counts);
    free(right_class_counts);

    return information_gain;
}

int main()
{
    int data[4][3] = {
        {5, 10, 15},
        {50, 55, 60},
        {100, 150, 200},
        {250, 300, 350}};

    int class_labels[4] = {1, 1, 2, 2};
    int num_samples = 4;
    int num_classes = 2;
    int num_features = 3;

    for (int i = 0; i < num_features; i++)
    {
        double gini_index = calculate_gini_index(class_labels, num_sample, num_class);
        printf("attribute %d gini index : %lf\n", i, gini_index);
    }

    // for (int i = 0; i < num_features; i++)
    // {
    //     int *feature_values = (int *)malloc(num_samples * sizeof(int)); // Allocate memory for feature values
    //     for (int j = 0; j < num_samples; j++)
    //     {
    //         feature_values[j] = data[j][i]; // Copy the values of the i-th column to the feature values array
    //     }
    //     for (int threshold = 0; threshold < num_samples; threshold++) // Iterate over thresholds for each feature
    //     {
    //         double info_gain = calculate_information_gain(class_labels, feature_values, num_samples, data[threshold][i], num_classes);

    //         printf("Feature %d, Threshold %d: Information Gain = %lf\n", i, data[threshold][i], info_gain);
    //     }
    //     free(feature_values); // Free allocated memory
    // }

    return 0;
}
