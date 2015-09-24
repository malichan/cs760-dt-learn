#ifndef Feature_hpp
#define Feature_hpp

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Feature {
private:
    string name;
    string type;
    int range;
    
protected:
    Feature(const string& name, const string& type, int range) : name(name), type(type), range(range) {}
    
public:
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
    virtual double convertValueToInternal(const string& str) = 0;
    virtual string convertInternalToValue(double val) = 0;
};

class NumericFeature : public Feature {
private:
    
public:
    NumericFeature(const string& name) : Feature(name, "numeric", INT_MAX) {}
    
    virtual double convertValueToInternal(const string& str);
    virtual string convertInternalToValue(double val);
};

class NominalFeature : public Feature {
private:
    int numOfVals;
    vector<string> idxToName;
    unordered_map<string, int> nameToIdx;
    
public:
    NominalFeature(const string& name, const vector<string>& values) : Feature(name, "nominal", (int)values.size()), idxToName(values) {
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
    
    virtual double convertValueToInternal(const string& str);
    virtual string convertInternalToValue(double val);
};

#endif /* Feature_hpp */
