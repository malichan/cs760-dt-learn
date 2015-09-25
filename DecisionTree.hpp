#ifndef DecisionTree_hpp
#define DecisionTree_hpp

#include <set>

#include "Split.hpp"

struct DecisionTreeNode {
public:
    DecisionTreeNode* parent;
    int parentLinkNo;
    vector<Instance*> instances;
    
    double classLabel;
    vector<int> classCount;
    
    Split* split;
    vector<DecisionTreeNode*> children;
    
    DecisionTreeNode() : parent(0), parentLinkNo(-1), instances(0), classLabel(-1), classCount(0), split(0), children(0) {}
    
    ~DecisionTreeNode() {
        for (DecisionTreeNode* node : children)
            delete node;
        if (split)
            delete split;
    }
};

class DecisionTree {
private:
    DecisionTreeNode* root;
    const DatasetMetadata* metadata;
    int stopThreshold;
    
    void buildDecisionTree(DecisionTreeNode* root, set<int>& featureIndices);
    
public:
    DecisionTree(const DatasetMetadata* metadata, const vector<Instance*>& instances, int stopThreshold);
    ~DecisionTree() {
        if (root)
            delete root;
    }
};

#endif /* DecisionTree_hpp */
