#ifndef Dataset_hpp
#define Dataset_hpp

#include "Feature.hpp"
#include "Instance.hpp"

struct DatasetMetadata {
public:
    string name;
    int numOfFeatures;
    vector<Feature*> featureList;
    Feature* classVariable;
    
    DatasetMetadata() {}
    ~DatasetMetadata() {
        for (Feature* f : featureList)
            if (f) delete f;
        if (classVariable)
            delete classVariable;
    }
};

class Dataset {
private:
    DatasetMetadata* metadata;
    
    vector<Instance*> trainSet;
    vector<Instance*> testSet;
    
    Dataset() {
        metadata = new DatasetMetadata;
    }

public:
    static Dataset* loadDataset(string trainFile, string testFile);
    
    const DatasetMetadata* getMetadata() const {
        return metadata;
    }
    
    const vector<Instance*>& getTrainSet() const {
        return trainSet;
    }
    
    const vector<Instance*>& getTestSet() const {
        return testSet;
    }
    
    ~Dataset() {
        if (metadata)
            delete metadata;
        for (Instance* i : trainSet)
            if (i) delete i;
        for (Instance* i : testSet)
            if (i) delete i;
    }
    
    void print();
};

#endif /* Dataset_hpp */
