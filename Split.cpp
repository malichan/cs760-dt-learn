#include "Split.hpp"

#include <sstream>
#include <cmath>
#include <algorithm>

static double computeEntropy(const vector<int>& splitCount, int& allCount) {
    int classRange = (int)splitCount.size();
    
    allCount = 0;
    for (int i = 0; i < classRange; ++i)
        allCount += splitCount[i];
    if (allCount == 0)
        return 0;
    
    double entropy = 0;
    for (int i = 0; i < classRange; ++i) {
        double freq = (double)splitCount[i] / allCount;
        if (freq > 0)
            entropy -= freq * log2(freq);
    }
    
    return entropy;
}

static double computeInfoGain(const vector<vector<int> >& splitCount) {
    int featureRange = (int)splitCount.size();
    int classRange = (int)splitCount[0].size();
    
    vector<int> beforeSplitCount(classRange);
    for (int i = 0; i < featureRange; ++i) {
        for (int j = 0; j < classRange; ++j) {
            beforeSplitCount[j] += splitCount[i][j];
        }
    }
    int beforeAllCount = 0;
    double beforeEntropy = computeEntropy(beforeSplitCount, beforeAllCount);
    
    double afterEntropyAll = 0;
    for (int i = 0; i < featureRange; ++i) {
        int afterAllCount = 0;
        double afterEntropy = computeEntropy(splitCount[i], afterAllCount);
        afterEntropyAll += afterEntropy * afterAllCount / beforeAllCount;
    }
    
    return beforeEntropy - afterEntropyAll;
}

Split* Split::createSplit(int featureIndex, const DatasetMetadata* metadata, const vector<Instance*>& instances, double* informationGain) {
    Split* split = 0;
    double infoGain = 0;
    
    Feature* feature = metadata->featureList[featureIndex];
    Feature* class_ = metadata->classVariable;
    string type = feature->getType();
    if (type == "numeric") {
        vector<pair<double, int> > sortedInsts(instances.size());
        for (int i = 0; i < instances.size(); ++i) {
            Instance* inst = instances[i];
            double featureVal = inst->featureVector[featureIndex];
            int classVal = (int)round(inst->classLabel);
            sortedInsts[i] = {featureVal, classVal};
        }
        sort(sortedInsts.begin(), sortedInsts.end());
        
        vector<pair<double, int> > sortedSets;
        sortedSets.push_back(sortedInsts[0]);
        for (int i = 1; i < instances.size(); ++i) {
            if (sortedInsts[i].first == sortedSets.back().first) {
                if (sortedSets.back().second != -1 && sortedInsts[i].second != sortedSets.back().second)
                    sortedSets.back().second = -1;
            } else {
                sortedSets.push_back(sortedInsts[i]);
            }
        }
        
        vector<double> candidates;
        for (int i = 1; i < sortedSets.size(); ++i) {
            if (sortedSets[i].second != sortedSets[i - 1].second || sortedSets[i].second == -1) {
                double mid = (sortedSets[i].first + sortedSets[i - 1].first) * 0.5;
                candidates.push_back(mid);
            }
        }
        
        int classRange = class_->getRange();
        double bestInfoGain = -INFINITY;
        double threshold = 0;
        for (int i = 0; i < candidates.size(); ++i) {
            double candidate = candidates[i];
            vector<vector<int> > splitCount(2);
            splitCount[0].resize(classRange);
            splitCount[1].resize(classRange);
            for (int j = 0; j < instances.size(); ++j) {
                if (sortedInsts[j].first <= candidate)
                    splitCount[0][sortedInsts[j].second]++;
                else
                    splitCount[1][sortedInsts[j].second]++;
            }
            double currInfoGain = computeInfoGain(splitCount);
            if (currInfoGain > bestInfoGain) {
                bestInfoGain = currInfoGain;
                threshold = candidate;
            }
        }
        
        infoGain = bestInfoGain;
        split = new NumericSplit((NumericFeature*)feature, threshold);
    } else {
        int featureRange = feature->getRange();
        int classRange = class_->getRange();
        vector<vector<int> > splitCount(featureRange);
        for (int i = 0; i < featureRange; ++i)
            splitCount[i].resize(classRange);
        
        for (int i = 0; i < instances.size(); ++i) {
            Instance* inst = instances[i];
            int featureVal = (int)round(inst->featureVector[featureIndex]);
            int classVal = (int)round(inst->classLabel);
            splitCount[featureVal][classVal]++;
        }
        
        infoGain = computeInfoGain(splitCount);
        split = new NominalSplit((NominalFeature*)feature);
    }
    
    if (informationGain)
        *informationGain = infoGain;
    return split;
}

int NumericSplit::split(const Instance* instance) const {
    return getValue(instance) <= threshold ? 0 : 1;
}

int NominalSplit::split(const Instance* instance) const {
    return round(getValue(instance));
}

string NumericSplit::toString(int index) const {
    stringstream ss;
    if (index == 0)
        ss << feature->getName() << " <= " << feature->convertInternalToValue(threshold);
    else
        ss << feature->getName() << " > " << feature->convertInternalToValue(threshold);
    return ss.str();
}

string NominalSplit::toString(int index) const {
    stringstream ss;
    ss << feature->getName() << " = " << feature->convertInternalToValue(index);
    return ss.str();
}