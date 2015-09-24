#include <iostream>
using namespace std;

#include "Dataset.hpp"
#include "Split.hpp"

int main(int argc, const char * argv[]) {
    Dataset* dataset = Dataset::loadDataset("heart_train.arff", "heart_test.arff");
//    dataset->print();
    
    double infoGain = 0;
    Split* split = Split::createSplit(0, dataset->getMetadata(), dataset->getTrainSet(), &infoGain);
    cout << infoGain << endl;
    delete split;
    
    delete dataset;
}
