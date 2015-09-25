#include <iostream>
#include <memory>

#include "DecisionTree.hpp"

int main(int argc, const char * argv[]) {
    shared_ptr<Dataset> dataset(Dataset::loadDataset("heart_train.arff", "heart_test.arff"));
    DecisionTree tree(dataset->getMetadata(), dataset->getTrainSet(), 2);
}
