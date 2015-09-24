#ifndef Instance_hpp
#define Instance_hpp

#include <vector>

using namespace std;

struct Instance {
public:
    vector<double> featureVector;
    double classLabel;
    
    Instance(int numOfFeatures) : featureVector(numOfFeatures, -1), classLabel(-1) {}
};

#endif /* Instance_hpp */
