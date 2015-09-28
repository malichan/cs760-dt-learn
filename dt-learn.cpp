#include <iostream>
#include <memory>
#include <chrono>
#include <random>
#include <algorithm>

#include "DecisionTree.hpp"

int main(int argc, const char * argv[]) {
    if (argc < 4) {
        cout << "usage: ./dt-learn train-set-file test-set-file m [percentage-of-train-set]" << endl;
    } else {
        string trainSetFile = argv[1];
        string testSetFile = argv[2];
        int stopThreshold = atoi(argv[3]);
        int percentageOfTrainSet = argc == 5 ? atoi(argv[4]) : 100;
        
        shared_ptr<Dataset> dataset(Dataset::loadDataset(trainSetFile, testSetFile));
        const DatasetMetadata* metadata = dataset->getMetadata();
    
        vector<Instance*> trainSet(dataset->getTrainSet().begin(), dataset->getTrainSet().end());
        if (percentageOfTrainSet < 100) {
            unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();
            shuffle (trainSet.begin(), trainSet.end(), default_random_engine(seed));
            int newSize = (int)trainSet.size() * percentageOfTrainSet / 100;
            trainSet.resize(newSize);
        }
    
        DecisionTree tree(metadata, trainSet, stopThreshold);
        cout << tree.toString() << endl;
    
        const vector<Instance*>& testSet = dataset->getTestSet();
        int correctCount = 0;
        cout << "# Predicted Actual" << endl;
        for (int i = 0; i < testSet.size(); ++i) {
            Instance* inst = testSet[i];
            string predicted = tree.predict(inst);
            string actual = inst->toString(metadata, true);
            if (predicted == actual)
                correctCount++;
            cout << predicted << " " << actual << endl;
        }
        cout << "# Accuracy: " << correctCount << "/" << testSet.size() << endl;
    }
}
