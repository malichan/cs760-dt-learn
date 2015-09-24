#ifndef Dataset_hpp
#define Dataset_hpp

#include "Feature.hpp"

class Dataset {
private:
    struct Instance {
        vector<double> featureVector;
        double classLabel;
        
        Instance(int numOfFeatures) : featureVector(numOfFeatures, -1), classLabel(-1) {}
    };
    
    string name;
    int numOfFeatures;
    vector<Feature*> featureList;
    Feature* classVariable;
    
    vector<Instance> trainSet;
    vector<Instance> testSet;
    
    Dataset() {}

public:
    static Dataset* loadDataset(string trainFile, string testFile);
    
    void print();
    
    ~Dataset() {
        for (Feature* f : featureList)
            if (f) delete f;
        if (classVariable) delete classVariable;
    }
};

#endif /* Dataset_hpp */
