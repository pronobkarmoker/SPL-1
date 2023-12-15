#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

int g = 0;
struct Node
{
    int featureIndex;
    double threshold;
    bool isLeaf;
    bool decision;
    Node *left;
    Node *right;
};

Node *head;

double calculateEntropy(vector<bool> &labels)
{
    int totalInstances = labels.size();
    int trueCount = 0;

    for (bool label : labels)
    {
        if (label)
        {
            trueCount++;
        }
    }

    double trueProbability = (double)(trueCount) / totalInstances;
    double falseProbability = 1.0 - trueProbability;

    // Avoid log(0)
    if (trueProbability == 0.0 || falseProbability == 0.0)
    {
        return 0.0;
    }

    return -trueProbability * log2(trueProbability) - falseProbability * log2(falseProbability);
}

bool areAllLabelsEqual(vector<bool> &labels)
{
    if (labels.empty())
    {
        return true; // Empty vector is considered to have equal labels
    }

    bool firstLabel = labels[0];
    for (bool label : labels)
    {
        if (label != firstLabel)
        {
            cout << "returning false\n";
            return false; // Labels are not equal
        }
    }
    cout << "returning true\n";
    return true; // All labels are equal
}

double calculate_information_gain(vector<bool> parentLabel, vector<bool> leftLabel, vector<bool> rightLabel)
{
    double parentEntropy = calculateEntropy(parentLabel);

    double leftChildEntropy = (leftLabel.size() * calculateEntropy(leftLabel)) / parentLabel.size();

    double rightChildEntropy = (rightLabel.size() * calculateEntropy(rightLabel)) / parentLabel.size();

    return parentEntropy - leftChildEntropy - rightChildEntropy;
}

pair<int, double> findBestSplit(vector<vector<double>> &data, vector<bool> &labels)
{

    int numFeatures = data[0].size(); // number of columns
    int numInstances = data.size();   // number of rows

    double currentEntropy = calculateEntropy(labels);
    double maxInfoGain = 0.0;
    int bestFeatureIndex = -1;
    double bestThreshold = 0.0;

    // cout << data[0][0] << "\n";

    for (int featureIndex = 0; featureIndex < numFeatures; featureIndex++)
    {
        // Sort data based on the current feature
        vector<pair<double, bool>> featureData;
        for (int instance = 0; instance < numInstances; instance++)
        {
            featureData.push_back({data[instance][featureIndex], labels[instance]});
        }

        /* for(auto it : featureData)
         {
           cout << it.first << " " << it.second << endl;
         }
         cout << endl;*/
        sort(featureData.begin(), featureData.end());

        // cout << "after sort labels :\n";
        /* for(auto it : featureData)
         {
           cout << it.first << " " << it.second << endl;
         }
         cout << endl;*/

        // Iterate through possible thresholds
        for (int i = 0; i < numInstances - 1; ++i)
        {

            double threshold = (featureData[i].first + featureData[i + 1].first) / 2.0;

            vector<bool> leftLabels;
            vector<bool> rightLabels;

            // Update left and right labels based on threshold
            for (int j = 0; j <= i; ++j)
            {
                leftLabels.push_back(featureData[j].second);
            }

            /*cout << "left labels" << endl;
            for(auto it : leftLabels)
            {
              cout << it << " ";
            }

            cout << endl << endl;*/

            for (int j = i + 1; j < numInstances; ++j)
            {
                rightLabels.push_back(featureData[j].second);
            }

            /*   cout << "right labels" << endl;
              for(auto it : rightLabels)
              {
                cout << it << " ";
              }

              cout << endl << endl;*/

            // cout << "left Entropy " << calculateEntropy(leftLabels) << "\n";
            // cout << "right Entropy " << calculateEntropy(rightLabels) << "\n";
            double infoGain = calculate_information_gain(labels, leftLabels, rightLabels);

            // cout << "infoGain " << infoGain << endl;

            if (infoGain > maxInfoGain)
            {
                maxInfoGain = infoGain;
                bestFeatureIndex = featureIndex;
                bestThreshold = threshold;
            }
        }
    }

    cout << "max info gain " << maxInfoGain << "\n";

    return {bestFeatureIndex, bestThreshold};
}

void deleteTree(Node *root)
{
    if (root == nullptr)
    {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

Node *buildDecisionTree(vector<vector<double>> &data, vector<bool> &labels)
{
    Node *root = new Node;

    for (auto it : labels)
    {
        cout << it << " ";
    }
    cout << "\n";

    // Base case: If all labels are the same, create a leaf node
    if (areAllLabelsEqual(labels))
    {
        // cout << "G "<<g << "\n";g++;
        root->isLeaf = true;
        root->decision = labels[0];

        /*  cout << "this is root " << root->decision << "\n";
          for(auto it : labels)
          {
              cout << it << " ";
          }
          cout << "\n";*/
        return root;
    }

    // Find the best split
    pair<int, double> bestSplit = findBestSplit(data, labels);
    root->featureIndex = bestSplit.first;
    root->threshold = bestSplit.second;

    cout << "root feature index " << root->featureIndex << " "
         << "root->threshold " << root->threshold << "\n";

    // Split the data and labels based on the best split
    vector<vector<double>> leftData, rightData;
    vector<bool> leftLabels, rightLabels;

    for (size_t i = 0; i < data.size(); ++i)
    {
        if (data[i][bestSplit.first] <= bestSplit.second)
        {
            leftData.push_back(data[i]);
            leftLabels.push_back(labels[i]);
        }
        else
        {
            rightData.push_back(data[i]);
            rightLabels.push_back(labels[i]);
        }
    }

    // Recursively build the left and right subtrees
    root->left = buildDecisionTree(leftData, leftLabels);
    root->right = buildDecisionTree(rightData, rightLabels);

    return root;
}

bool classifyInstance(Node *root, vector<double> &instance)
{
    // Base case: If the current node is a leaf, return its decision
    if (root->isLeaf)
    {
        return root->decision;
    }

    // Determine which child node to traverse based on the instance's feature
    if (instance[root->featureIndex] <= root->threshold)
    {
        return classifyInstance(root->left, instance);
    }
    else
    {
        return classifyInstance(root->right, instance);
    }
}

int main()
{
    ifstream infile("iris.data");
    string line;

    vector<vector<double>> data;
    vector<bool> labels;

    while (getline(infile, line))
    {
        stringstream ss(line);
        vector<double> instance;
        double value;
        for (int i = 0; i < 4; ++i)
        {
            ss >> value;
            instance.push_back(value);
        }

        string label;
        ss >> label;

        // Use boolean labels for training
        labels.push_back(label == "Iris-setosa");
        data.push_back(instance);
    }

    infile.close();

    head = buildDecisionTree(data, labels);

    // Test the decision tree on a few instances
    vector<vector<double>> testInstances = {{5.1, 3.5, 1.4, 0.2},
                                           {6.2, 2.9, 4.3, 1.3},
                                           {7.3, 2.8, 6.4, 1.8}};

    for (const auto &instance : testInstances)
    {
        // Use boolean labels for testing
        bool prediction = classifyInstance(head, instance);
        cout << "Prediction: " << (prediction ? "Iris-setosa" : "Not Iris-setosa") << "\n";
    }

    // Clean up the decision tree
    deleteTree(head);

    return 0;
}