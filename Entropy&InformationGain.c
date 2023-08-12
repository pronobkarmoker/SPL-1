#include <stdio.h>
#include <math.h>
#include<stdlib.h>

double log2(double x) {
    return log(x) / log(2);
}

double calculate_entropy(int* class_labels, int num_samples) {
    int pos_count = 0;
    int neg_count = 0;

    for (int i = 0; i < num_samples; i++) {
        if (class_labels[i] == 1) {
            pos_count++;
        } else {
            neg_count++;
        }
    }

    double p_pos = (double)pos_count / num_samples;
    double p_neg = (double)neg_count / num_samples;

    double entropy = 0.0;
    if (p_pos != 0) {
        entropy -= p_pos * log2(p_pos);
    }
    if (p_neg != 0) {
        entropy -= p_neg * log2(p_neg);
    }

    return entropy;
}

double calculate_information_gain(int* class_labels, int* feature_values, int num_samples, int threshold, int num_classes) {
    int left_count = 0;
    int right_count = 0;
    int* left_class_counts = (int*)malloc(num_classes * sizeof(int));
    int* right_class_counts = (int*)malloc(num_classes * sizeof(int));

    // Initialize the arrays to 0
    for (int i = 0; i < num_classes; ++i) {
        left_class_counts[i] = 0;
        right_class_counts[i] = 0;
    }

    for (int i = 0; i < num_samples; i++) {
        if (feature_values[i] <= threshold) {
            left_count++;
            left_class_counts[class_labels[i] - 1]++;
        } else {
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

int main() {
    // Input data
    int data[4][3] = {
        {5, 10, 15},
        {50, 55, 60},
        {100, 150, 200},
        {250, 300, 350}
    };

    int class_labels[4] = {1, 1, 2, 2};
    int num_samples = 4;
    int num_classes = 2;

    double entropy = calculate_entropy(class_labels, num_samples);
    printf("Entropy: %lf\n", entropy);

    int feature_values[4] = {5, 50, 100, 250};
    int threshold = 100;
    double information_gain = calculate_information_gain(class_labels, feature_values, num_samples, threshold, num_classes);
    printf("Information Gain: %lf\n", information_gain);

    return 0;
}
