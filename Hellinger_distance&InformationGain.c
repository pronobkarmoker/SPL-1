#include <stdio.h>
#include <math.h>

#define NUM_SAMPLES 4
#define NUM_CLASSES 2
#define NUM_FEATURES 3

// Function to calculate Hellinger Distance between two class distributions
double calculateHellingerDistance(double p[], double q[], int numClasses) {
    double distance = 0.0;

    for (int i = 0; i < numClasses; i++) {
        double rootP = sqrt(p[i]);
        double rootQ = sqrt(q[i]);
        distance += pow(rootP - rootQ, 2);
    }

    return sqrt(0.5 * distance);
}

// Function to calculate Information Gain using Hellinger Distance for a specific class
double calculateInformationGainForClass(double classDistr[][NUM_CLASSES], int numSamples, int numClasses, int classIndex) {
    double totalClassDistr[NUM_CLASSES] = {0};
    double leftClassDistr[NUM_CLASSES] = {0};
    double rightClassDistr[NUM_CLASSES] = {0};

    // Calculate total class distribution
    for (int i = 0; i < numSamples; i++) {
        for (int j = 0; j < numClasses; j++) {
            totalClassDistr[j] += classDistr[i][j];
        }
    }

    // Calculate Hellinger Distance for left branch (in this case, the same as totalClassDistr)
    for (int j = 0; j < numClasses; j++) {
        leftClassDistr[j] = totalClassDistr[j];
    }

    // Calculate Hellinger Distance for right branch
    for (int j = 0; j < numClasses; j++) {
        rightClassDistr[j] = 0;
    }

    double totalHellingerDistance = calculateHellingerDistance(totalClassDistr, totalClassDistr, numClasses);
    double leftHellingerDistance = calculateHellingerDistance(totalClassDistr, leftClassDistr, numClasses);
    double rightHellingerDistance = calculateHellingerDistance(totalClassDistr, rightClassDistr, numClasses);

    double pLeft = (double) numSamples / (2.0 * NUM_SAMPLES); // Adjust the weight of the left branch
    double pRight = 1.0 - pLeft; // Adjust the weight of the right branch

    double informationGain = totalHellingerDistance - ((pLeft * leftHellingerDistance) + (pRight * rightHellingerDistance));

    return informationGain;
}

int main() {
    int data[NUM_SAMPLES][NUM_FEATURES] = {
        {5, 10, 15},
        {50, 55, 60},
        {100, 150, 200},
        {250, 300, 350}
    };

    int class_labels[NUM_SAMPLES] = {1, 1, 2, 2};

    double classDistr[NUM_SAMPLES][NUM_CLASSES] = {0};

    // Convert data into class distributions
    for (int i = 0; i < NUM_SAMPLES; i++) {
        classDistr[i][class_labels[i] - 1] = 1; // Assign 1 to the corresponding class
    }

    // Calculate Information Gain for each class
    for (int c = 0; c < NUM_CLASSES; c++) {
        double infoGain = calculateInformationGainForClass(classDistr, NUM_SAMPLES, NUM_CLASSES, c);
        printf("Information Gain for Class %d: %lf\n", c + 1, infoGain);
    }

    return 0;
}
