#include "Dataset.hpp"

int main(int argc, const char * argv[]) {
    Dataset* dataset = Dataset::loadDataset("heart_train.arff", "heart_test.arff");
    dataset->print();
    delete dataset;
}
