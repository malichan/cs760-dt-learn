#ifndef DecisionTree_hpp
#define DecisionTree_hpp

#include <set>

#include "Split.hpp"

class DecisionTree;

struct DecisionTreeNode {
public:
    const DecisionTree* owner;
    
    DecisionTreeNode* parent;
    int parentLinkNo;
    vector<Instance*> instances;
    
    double classLabel;
    vector<int> classCount;
    
    Split* split;
    vector<DecisionTreeNode*> children;
    
    DecisionTreeNode(const DecisionTree* owner) :owner(owner), parent(0), parentLinkNo(-1), instances(0), classLabel(-1), classCount(0), split(0), children(0) {}
    
    ~DecisionTreeNode() {
        for (DecisionTreeNode* node : children)
            delete node;
        if (split)
            delete split;
    }
    
    string toString() const;
};

class DecisionTree {
private:
    DecisionTreeNode* root;
    const DatasetMetadata* metadata;
    int stopThreshold;
    
    void buildDecisionTree(DecisionTreeNode* root, set<int>& featureIndices);
    void printDecisionTree(DecisionTreeNode* node, int level, stringstream& ss) const;
    
public:
    DecisionTree(const DatasetMetadata* metadata, const vector<Instance*>& instances, int stopThreshold);
    
    ~DecisionTree() {
        if (root)
            delete root;
    }
    
    const DatasetMetadata* getMetadata() const {
        return metadata;
    }
    
    string toString() const;
};

#endif /* DecisionTree_hpp */
