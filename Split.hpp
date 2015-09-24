#ifndef Split_hpp
#define Split_hpp

#include "Dataset.hpp"

class Split {
private:
    const Feature* feature;
    
protected:
    Split(const Feature* feature) : feature(feature) {}
    
public:
    static Split* createSplit(int featureIndex, const DatasetMetadata* metadata, const vector<Instance*>& instances, double* informationGain);
    
    double getValue(const Instance* instance) const {
        return instance->featureVector[feature->getIndex()];
    }
    
    virtual int split(const Instance* instance) const = 0;
    
    virtual ~Split() {}
};

class NumericSplit : public Split {
private:
    double threshold;
    
public:
    NumericSplit(const NumericFeature* feature, int threshold) : Split(feature), threshold(threshold) {}
    
    virtual int split(const Instance* instance) const;
};

class NominalSplit : public Split {
private:
    
public:
    NominalSplit(const NominalFeature* feature) : Split(feature) {}
    
    virtual int split(const Instance* instance) const;
};

#endif /* Split_hpp */
