#include <stdio.h>
#include <math.h>

double hellinger_distance(int data[][3], int class_labels[], int n_classes)
{
    double p[n_classes][3];
    double q[n_classes][3];
    double distance, sqrt_p, sqrt_q;
    int i, j;

    // Initialize p and q.
    for (i = 0; i < n_classes; i++)
    {
        for (j = 0; j < 3; j++)
        {
            p[i][j] = 0.0;
            q[i][j] = 0.0;
        }
    }

    // Calculate p.
    for (i = 0; i < n_classes; i++)
    {
        for (j = 0; j < n_classes; j++)
        {
            if (class_labels[j] == i)
            {
                for (int k = 0; k < 3; k++)
                {
                    p[i][k] += data[j][k];
                }
            }
        }
    }

    // Calculate q.
    for (i = 0; i < n_classes; i++)
    {
        for (j = 0; j < 3; j++)
        {
            q[i][j] = sqrt(p[i][j]);
        }
    }

    // Calculate the Hellinger distance.
    distance = 0.0;
    for (i = 0; i < n_classes; i++)
    {
        for (j = 0; j < 3; j++)
        {
            distance += (q[i][j] - p[i][j]) * (q[i][j] - p[i][j]);
        }
    }

    return 0.5 * distance;
}

int main()
{
    int data[4][3] = {
        {5, 10, 15},
        {50, 55, 60},
        {100, 150, 200},
        {250, 300, 350}};

    int class_labels[4] = {1, 1, 2, 2};

    double distance = hellinger_distance(data, class_labels, 2);

    printf("%ld", distance);
}
