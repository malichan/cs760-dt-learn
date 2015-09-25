#include "DecisionTree.hpp"

void DecisionTree::buildDecisionTree(DecisionTreeNode* root, set<int>& featureIndices) {
    root->classCount.resize(metadata->numOfClasses);
    if (root->instances.empty()) {
        root->classLabel = root->parent->classLabel;
        return;
    }
    
    for (int i = 0; i < root->instances.size(); ++i)
        root->classCount[(int)round(root->instances[i]->classLabel)]++;
    
    int majority = -1;
    int majorityCount = -1;
    for (int i = 0; i < root->classCount.size(); ++i) {
        if (root->classCount[i] > majorityCount) {
            majority = i;
            majorityCount = root->classCount[i];
        }
    }
    root->classLabel = majority;
    
    if (root->instances.size() < stopThreshold || root->instances.size() == majorityCount || featureIndices.empty())
        return;
    
    Split* bestSplit = 0;
    double maxInfoGain = -INFINITY;
    for (set<int>::iterator it = featureIndices.begin(); it != featureIndices.end(); ++it) {
        double infoGain = 0;
        Split* split = Split::createSplit(*it, metadata, root->instances, &infoGain);
        if (infoGain > 0 && infoGain > maxInfoGain) {
            if (bestSplit) delete bestSplit;
            bestSplit = split;
            maxInfoGain = infoGain;
        } else {
            delete split;
        }
    }
    
    if (maxInfoGain < 0)
        return;
    
    root->split = bestSplit;
    int featureIdx = bestSplit->getFeature()->getIndex();
    const Feature* feature = bestSplit->getFeature();
    
    vector<vector<Instance*> > splittedInsts(feature->getRange());
    for (int i = 0; i < root->instances.size(); ++i) {
        int val = bestSplit->split(root->instances[i]);
        splittedInsts[val].push_back(root->instances[i]);
    }
    
    featureIndices.erase(featureIdx);
    for (int i = 0; i < feature->getRange(); ++i) {
        DecisionTreeNode* node = new DecisionTreeNode;
        node->parent = root;
        node->parentLinkNo = i;
        node->instances = splittedInsts[i];
        root->children.push_back(node);
    }
    featureIndices.insert(featureIdx);
}

DecisionTree::DecisionTree(const DatasetMetadata* metadata, const vector<Instance*>& instances, int stopThreshold) : root(0), metadata(metadata), stopThreshold(stopThreshold) {
    root = new DecisionTreeNode;
    root->instances = instances;
    set<int> featureIndices;
    for (int i = 0; i < metadata->numOfFeatures; ++i)
        featureIndices.insert(i);
    buildDecisionTree(root, featureIndices);
}