#include <stdio.h>
#include <math.h>

#define num_sample 5
#define num_class 2
#define num_features 3

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

int main()
{
    FILE *fp = fopen("gini.txt", "r");
    int data[num_sample][num_features];

    for (int i = 0; i < num_sample; i++)
    {
        for (int j = 0; j < num_features; j++)
        {
            fscanf(fp, "%d", &data[i][j]);
        }
    }

    for (int i = 0; i < num_sample; i++)
    {
        for (int j = 0; j < num_features; j++)
        {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }
    //= {
    // {5, 10, 15},
    // {50, 55, 60},
    // {100, 150, 200},
    // {250, 300, 350}};
    int class_labels[num_sample] = {1, 1, 2, 2, 2};

    for (int i = 0; i < num_features; i++)
    {
        double gini_index = calculate_gini_index(class_labels, num_sample, num_class);
        printf("attribute %d gini index : %lf\n", i, gini_index);
    }
    return 0;
}