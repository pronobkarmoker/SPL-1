#include <stdio.h>
#include <math.h>

// Function to calculate Hellinger Distance
double calculateHellingerDistance(double p[], double q[], int n)
{
    double distance = 0.0;
    for (int i = 0; i < n; i++)
    {
        double sqrt_p = sqrt(p[i]);
        double sqrt_q = sqrt(q[i]);
        distance += (sqrt_p - sqrt_q) * (sqrt_p - sqrt_q);
    }
    distance = sqrt(0.5 * distance);
    return distance;
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
    int num_features = 3;
    int num_classes = 2;

    // Calculate class-wise counts
    int class_counts[num_classes] ; 
    class_counts[num_classes] = 0;
    for (int i = 0; i < num_samples; i++)
    {
        class_counts[class_labels[i] - 1]++;
    }

    // Calculate probability distributions for each class
    double class_probs[num_classes][num_features];
    for (int i = 0; i < num_classes; i++)
    {
        for (int j = 0; j < num_features; j++)
        {
            int feature_sum = 0;
            for (int k = 0; k < num_samples; k++)
            {
                if (class_labels[k] == i + 1)
                {
                    feature_sum += data[k][j];
                }
            }
            class_probs[i][j] = (double)feature_sum / class_counts[i];
        }
    }

    // Calculate Hellinger Distance between class probability distributions
    double hellinger_distance = calculateHellingerDistance(class_probs[0], class_probs[1], num_features);

    printf("Hellinger Distance: %lf\n", hellinger_distance);

    return 0;
}
