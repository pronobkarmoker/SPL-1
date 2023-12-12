#include <bits/stdc++.h>
#include "random_forest.h"
#include <windows.h>
#include <direct.h>

using namespace std;
typedef long long ll;

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    setcolor1(6);

    printf("\trandom ");
    setcolor1(7);
    printf("for random forest algorithm\n");
    setcolor1(6);
    printf("\tdecision ");
    setcolor1(7);
    printf("for decision tree algorithm\n");
    // setcolor3(11);
    setcolor1(9);
    printf("\ttype the name : ");
    string s;
    cin >> s;

    if (s == "random")
    {
        random_forest();
        test_random();
    }
    else if (s == "decision")
    {
        DecisionTree();
        test_decision();
    }
}
