#ifndef Feature_hpp
#define Feature_hpp

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Feature {
private:
    int index;
    string name;
    string type;
    int range;
    
protected:
    Feature(int index, const string& name, const string& type, int range) : index(index), name(name), type(type), range(range) {}
    
public:
    int getIndex() const {
        return index;
    }
    
    const string& getName() const {
        return name;
    }
    
    const string& getType() const {
        return type;
    }
    
    int getRange() const {
        return range;
    }
    
    virtual ~Feature() {}
    virtual double convertValueToInternal(const string& str) const = 0;
    virtual string convertInternalToValue(double val) const = 0;
};

class NumericFeature : public Feature {
private:
    
public:
    NumericFeature(int index, const string& name) : Feature(index, name, "numeric", INT_MAX) {}
    
    virtual double convertValueToInternal(const string& str) const;
    virtual string convertInternalToValue(double val) const;
};

class NominalFeature : public Feature {
private:
    int numOfVals;
    vector<string> idxToName;
    unordered_map<string, int> nameToIdx;
    
public:
    NominalFeature(int index, const string& name, const vector<string>& values) : Feature(index, name, "nominal", (int)values.size()), idxToName(values) {
        numOfVals = (int)idxToName.size();
        for (int i = 0; i < numOfVals; ++i)
            nameToIdx[idxToName[i]] = i;
    }
    
    int getNumberOfValues() const {
        return numOfVals;
    }
    
    const string& getValueName(int index) const {
        return idxToName.at(index);
    }
    
    int getValueIndex(const string& name) const {
        unordered_map<string, int>::const_iterator it = nameToIdx.find(name);
        if (it != nameToIdx.end())
            return it->second;
        else
            return -1;
    }
    
    virtual double convertValueToInternal(const string& str) const;
    virtual string convertInternalToValue(double val) const;
};

#endif /* Feature_hpp */
