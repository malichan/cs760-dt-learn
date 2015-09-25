#include <sstream>

#include "DecisionTree.hpp"

string DecisionTreeNode::toString() const {
    if (!parent)
        return "";
    
    stringstream ss;
    ss << parent->split->toString(parentLinkNo) << " [" << classCount[0];
    for (int i = 1; i < classCount.size(); ++i)
        ss << " " << classCount[i];
    ss << "]";
    
    if (!split)
        ss << ": " << owner->getMetadata()->classVariable->convertInternalToValue(classLabel);
    
    return ss.str();
}

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
    
    if (feature->getType() == "nominal")
        featureIndices.erase(featureIdx);
    for (int i = 0; i < feature->getRange(); ++i) {
        DecisionTreeNode* node = new DecisionTreeNode(this);
        node->parent = root;
        node->parentLinkNo = i;
        node->instances = splittedInsts[i];
        root->children.push_back(node);
        buildDecisionTree(node, featureIndices);
    }
    if (feature->getType() == "nominal")
        featureIndices.insert(featureIdx);
}

DecisionTree::DecisionTree(const DatasetMetadata* metadata, const vector<Instance*>& instances, int stopThreshold) : root(0), metadata(metadata), stopThreshold(stopThreshold) {
    root = new DecisionTreeNode(this);
    root->instances = instances;
    set<int> featureIndices;
    for (int i = 0; i < metadata->numOfFeatures; ++i)
        featureIndices.insert(i);
    buildDecisionTree(root, featureIndices);
}

void DecisionTree::printDecisionTree(DecisionTreeNode* node, int level, stringstream& ss) const {
    for (int i = 0; i < level; ++i)
        ss << "|\t";
    ss << node->toString() << endl;
    for (int i = 0; i < node->children.size(); ++i)
        printDecisionTree(node->children[i], level + 1, ss);
}

string DecisionTree::toString() const {
    stringstream ss;
    for (int i = 0; i < root->children.size(); ++i)
        printDecisionTree(root->children[i], 0, ss);
    return ss.str();
}