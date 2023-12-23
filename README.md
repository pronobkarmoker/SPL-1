# Classification of Data Using Decision Tree and Random Forest Based on Three Different Criteria

C++ implementation of Decision Trees and Random Forests for classification of Insurance Dataset


We build decision trees and random forests for a insurance dataset, evaluating it for various experiments .
Dataset taken from : https://archive.ics.uci.edu/ml/datasets/Insurance+Company+Benchmark+%28COIL+2000%29 


### HOW TO RUN : 

1) Go to the folder :
```sh
   cd Final
 ```

2) Compile the program by entering the following command :
```sh
   g++  -o ID3 ID3.cpp
 ```

  
3) Run the executable by entering the following command : 
```sh
   ./ID3  ticdata2000.txt  experiment_no
   ```
   ticdata2000.txt contains the dataset for creating the tree.
   
4) Press enter to print the output.
   
5) Please refer to the Results and Conclusion file to see the final results of all the experiments.  



### Experiments : 
>1. We vary the "stopping criteria" that prevents further splitting of node. Changes in accuracy and complexity of model are observed.
>2. Add noise to the dataset and evaluate the accuracy of the model along with the change in its complexity (number of nodes)
>3. Perform "Reduced Error Pruning" on the tree and measure the change in accuracy of the tree.
>4. Create a random forest using "Feature Bagging" approach where we select a subset of features, make multiple trees, and take majority vote for the result.
