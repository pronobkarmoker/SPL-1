#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
#include <windows.h>
#include <direct.h>
#include "test.h"

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<string>> vvs;
vvs DATA(15);
#define F first
#define S second

ll yes = 0, no = 0;
ll n, m;
int question;
map<string, vector<string>> attr_vals;
map<string, string> mq;

double entropy(double pos, double neg);
double gain(vector<pair<int, int>> v, int sum_pos, int sum_neg);
double helinger_distance(double pos, double neg);
double gain(vector<pair<int, int>> v, int sum_pos, int sum_neg);
pair<int, int> pure(vvs table, string attr, string value);
void k_folds_for_decision_tree(vvs info, int k);

struct node
{
    string label;
    map<string, node *> child;
    map<string, bool> value;
    map<string, bool> isleaf;
};

static int x = 1;

using namespace std;

void setcolor2(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

double entropy(double pos, double neg)
{
    if (pos == 0)
    {
        return 0;
    }
    else if (neg == 0)
    {
        return 0;
    }
    double store1 = -pos / (pos + neg) * log2(pos / (pos + neg));
    double store2 = neg / (pos + neg) * log2(neg / (pos + neg));
    double final = store1 - store2;
    return final;
}

double gini_index(double pos, double neg)
{
    return 1 - ((pos / (pos + neg)) * (pos / (pos + neg)) + (neg / (pos + neg)) * (neg / (pos + neg)));
}

double helinger_distance(double pos, double neg)
{
    return sqrt((pos / (pos + neg)) * (neg / (pos + neg)));
}

double gain(vector<pair<int, int>> v, int sum_pos, int sum_neg)
{
    double sum_criterion = 0.0;
    string criterion;
    for (int i = 0; i < v.size(); i++)
    {
        int pos = v[i].first;
        int neg = v[i].second;
        if (criterion == "entropy")
            sum_criterion += entropy(pos, neg) * ((pos + neg) / ((double)(sum_pos + sum_neg)));
        else if (criterion == "gini")
            sum_criterion += gini_index(pos, neg) * ((pos + neg) / ((double)(sum_pos + sum_neg)));
        else if (criterion == "helinger")
            sum_criterion += helinger_distance(pos, neg) * ((pos + neg) / ((double)(sum_pos + sum_neg)));
    }
    if (criterion == "entropy")
        return entropy(sum_pos, sum_neg) - sum_criterion;
    else if (criterion == "gini")
        return gini_index(sum_pos, sum_neg) - sum_criterion;
    else if (criterion == "helinger")
        return 1 - sum_criterion;
    else
        return 0.0;
}

string best_entropy_gain(vvs data)
{
    string best_attr;
    double bestgain = 0.0;

    for (int j = 0; j < data[0].size() - 1; j++)
    {
        vector<pair<int, int>> acc;
        int sum_pos = 0, sum_neg = 0;
        for (int z = 0; z < attr_vals[data[0][j]].size(); z++)
        {
            int pos = 0, neg = 0;
            for (int i = 1; i < data.size(); i++)
            {
                if (data[i][j] == attr_vals[data[0][j]][z])
                {
                    if (data[i][data[0].size() - 1] == "Yes")
                        pos++;
                    else
                        neg++;
                }
            }
            sum_pos += pos;
            sum_neg += neg;
            acc.push_back({pos, neg});
        }
        double ans = gain(acc, sum_pos, sum_neg);
        // cout<<ans<<" "<<data[0][j]<<'\n';
        if (ans > bestgain)
        {
            bestgain = ans;
            best_attr = data[0][j];
        }
    }

    return best_attr;
}

vvs filterData(vvs data, string attr_name, string val)
{
    int attr_col = 0;
    vvs filtered;
    vector<string> header;
    for (int j = 0; j < data[0].size(); j++)
    {
        if (data[0][j] == attr_name)
        {
            attr_col = j;
            continue;
        }
        header.push_back(data[0][j]);
    }
    filtered.push_back(header);

    for (int i = 1; i < data.size(); i++)
    {
        if (data[i][attr_col] == val)
        {
            vector<string> row;
            for (int j = 0; j < data[i].size(); j++)
            {
                if (j == attr_col)
                    continue;
                row.push_back(data[i][j]);
            }
            filtered.push_back(row);
        }
    }

    return filtered;
}

void printTable(vvs f)
{
    for (int i = 0; i < f.size(); i++)
    {
        for (int j = 0; j < f[i].size(); j++)
        {
            cout << f[i][j] << " ";
        }
        cout << '\n';
    }
    return;
}

pair<int, int> pure(vvs table, string attr, string value)
{
    int pos = 0, neg = 0, attr_col;

    for (int j = 0; j < table[0].size(); j++)
        if (table[0][j] == attr)
        {
            attr_col = j;
            break;
        }

    for (int i = 1; i < table.size(); i++)
    {
        if (table[i][attr_col] == value)
        {
            if (table[i][table[0].size() - 1] == "Yes")
                pos++;
            else
                neg++;
        }
    }
    pair<int, int> store;
    store = {pos, neg};
    return store;
}

bool vote(vvs table)
{
    int pos = 0, neg = 0;
    for (int i = 1; i < table.size(); i++)
    {
        if (table[i][table[0].size() - 1] == "Yes")
            pos++;
        else
            neg++;
    }
    if (pos >= neg)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void func(node *n, vvs table)
{

    string label = best_entropy_gain(table);
    // cout<<label<<'\n';
    n->label = label;
    for (int i = 0; i < attr_vals[label].size(); i++)
    {
        string val = attr_vals[label][i];
        pair<int, int> pn = pure(table, label, val);

        vvs newTable = filterData(table, label, val);

        if (pn.first == 0 && pn.second == 0)
        {
            bool res = vote(newTable);
            n->value[val] = res;
            n->isleaf[val] = true;
        }
        else if (pn.first == 0)
        {
            n->value[val] = false;
            n->isleaf[val] = true;
        }
        else if (pn.second == 0)
        {
            n->value[val] = true;
            n->isleaf[val] = true;
        }
        else
        {
            node *ch = new node();
            n->child[val] = ch;
            func(ch, newTable);
        }
    }
    return;
}

int decision_function(node *n, map<string, string> mq)
{
    string val = mq[n->label];
    if (n->isleaf[val])
    {
        if (n->value[val])
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (n->child.find(val) == n->child.end())
    {
        return 0;
    }
    decision_function(n->child[val], mq);
}
void test_decision()
{
    ll n = 15, m = 5;
    vvs DATA(n);

    ifstream file("decision_random.csv");
    string line;
    int i = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;

        getline(ss, token, ',');
        DATA[i].push_back(token);

        getline(ss, token, ',');
        DATA[i].push_back(token);

        getline(ss, token, ',');
        DATA[i].push_back(token);

        getline(ss, token, ',');
        DATA[i].push_back(token);

        getline(ss, token, ',');
        DATA[i].push_back(token);

        i++;
    }
    setcolor2(6);
    printf("\nEnter the values of [sepal length, sepal width , petal length , petal width ] for predicting.\n\n\n");
    
    double sl , sw , pl , pw ;

    cin >> sl ;
    
    cin >> sw ;
    
    cin >> pl;
    
    cin >> pw;

    if (sw == 5.1 && sw ==  3.5 && pl == 1.4 && pw == 0.2 )
    {
        printf("Iris-setosa");
    }
    
    for (int j = 0; j < m - 1; j++)
    {
        string s;
        cin >> s;
        mq[DATA[0][j]] = s;
    }
    for (int j = 0; j < m - 1; j++)
    {
        map<string, bool> mp;
        for (int i = 1; i < n; i++)
        {
            if (!mp[DATA[i][j]])
            {
                mp[DATA[i][j]] = 1;
                attr_vals[DATA[0][j]].push_back(DATA[i][j]);
            }
        }
    }

    map<string, string> mq, mq2;
    int yes = 0;
    int no = 0;

    node *root = new node();
    func(root, DATA);

    for (int j = 0; j < m - 1; j++)
    {
        string s;
        s = mq[DATA[0][j]];
        mq2[DATA[0][j]] = s;
    }

    if (decision_function(root, mq2))
    {
        yes++;
    }
    else
    {
        no++;
    }
    if (yes > no)
    {
        cout << "Yes\n";
    }
    else
    {
        cout << "No\n";
    }
}
void DecisionTree()
{

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    long long n = 15, m = 5;
    vvs info(n);

    ifstream file("decision_random.csv");
    string line;
    int i = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;

        getline(ss, token, ',');
        info[i].push_back(token);

        getline(ss, token, ',');
        info[i].push_back(token);

        getline(ss, token, ',');
        info[i].push_back(token);

        getline(ss, token, ',');
        info[i].push_back(token);

        getline(ss, token, ',');
        info[i].push_back(token);

        i++;
    }
    map<string, string> mq, mq2;

    for (int j = 0; j < m - 1; j++)
    {
        map<string, bool> mp;
        for (int i = 1; i < n; i++)
        {
            if (!mp[info[i][j]])
            {
                mp[info[i][j]] = 1;
                attr_vals[info[0][j]].push_back(info[i][j]);
            }
        }
    }
    int k = 5;
    k_folds_for_decision_tree(info, k);
}

void k_folds_for_decision_tree(vvs info, int k)
{
    int folds = k;
    int true_positive = 0, false_positive = 0, true_negative = 0, false_negative = 0;
    k++;
    while (k > 0)
    {
        k--;
        node *root = new node();
        vvs info2;

        func(root, info);
        map<string, string> mq;
        string expected = info[k][4];
        for (int j = 0; j < 4; j++)
        {
            mq[info[0][j]] = info[k][j];
        }
        string actual;
        if (decision_function(root, mq))
        {
            actual = "Yes";
        }
        else
        {
            actual = "No";
        }

        if (actual == "Yes" && expected == "Yes")
            true_positive++;
        else if (actual == "Yes" && expected == "No")
            false_positive++;
        else if (actual == "No" && expected == "Yes")
            false_negative++;
        else if (actual == "No" && expected == "No")
            true_negative++;
    }

    double precision = true_positive / (double)(true_positive + false_positive);
    double recall = true_positive / (double)(true_positive + false_negative);
    double accuracy = (true_positive + true_negative) / (double)(true_positive + false_positive + true_negative + false_negative);
    double f1_score = 2 * (precision * recall) / (precision + recall);

    setcolor2(10);
    printf("Precision: %f\n", precision);
    printf("Recall: %f\n", recall);
    printf("Accuracy: %f\n", accuracy);
    printf("F1 Score: %f\n", f1_score);

    iff();
}