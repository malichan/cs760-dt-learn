#include <iostream>
#include <fstream>
#include <algorithm>

#include "Dataset.hpp"

static inline string toLower(const string& str) {
    string newStr = str;
    transform(newStr.begin(), newStr.end(), newStr.begin(), ::tolower);
    return newStr;
}

static inline void removeComment(string& str) {
    size_t idx = str.find('%');
    if (idx != string::npos)
        str = str.substr(0, idx);
}

static vector<string> tokenize(const string& str) {
    vector<string> tokens;
    int start = 0;
    bool quote = false;
    for (int i = 0; i < str.length(); ++i) {
        if (quote) {
            if (str[i] == '\'' || str[i] == '"') {
                quote = false;
                tokens.push_back(str.substr(start, i - start));
                start = i + 1;
            }
        } else {
            switch (str[i]) {
                case '\'':
                case '"':
                    quote = true;
                case ' ':
                case ',':
                case '{':
                case '}':
                    if (i - start > 0)
                        tokens.push_back(str.substr(start, i - start));
                    start = i + 1;
                    break;
            }
        }
    }
    if (start != str.length())
        tokens.push_back(str.substr(start, str.length() - start));
    return tokens;
}

Dataset* Dataset::loadDataset(string trainFile, string testFile) {
    ifstream finTrain, finTest;
    finTrain.open(trainFile);
    finTest.open(testFile);
    if (!finTrain.is_open() || !finTest.is_open())
        return 0;
    
    Dataset* dataset = new Dataset;
    
    string line;
    int numOfFeatures = 0;
    bool header = true;
    while (finTrain.good()) {
        getline(finTrain, line);
        removeComment(line);
        if (line.empty())
            continue;
        vector<string> tokens = tokenize(line);
        if (header) {
            string lineType = toLower(tokens[0]);
            if (lineType == "@relation") {
                dataset->metadata->name = tokens[1];
            } else if (lineType == "@attribute") {
                string featureName = tokens[1];
                string featureType = toLower(tokens[2]);
                if (featureName == "class") {
                    vector<string> vals(tokens.begin() + 2, tokens.end());
                    Feature* f = new NominalFeature(-1, featureName, vals);
                    dataset->metadata->classVariable = f;
                } else if (featureType == "numeric" || featureType == "integer" || featureType == "real") {
                    Feature* f = new NumericFeature(numOfFeatures++, featureName);
                    dataset->metadata->featureList.push_back(f);
                } else {
                    vector<string> vals(tokens.begin() + 2, tokens.end());
                    Feature* f = new NominalFeature(numOfFeatures++, featureName, vals);
                    dataset->metadata->featureList.push_back(f);
                }
            } else if (lineType == "@data") {
                header = false;
                dataset->metadata->numOfFeatures = (int)dataset->metadata->featureList.size();
            }
        } else {
            if (tokens.size() != numOfFeatures + 1) {
                cerr << "Error parsing \"" << line << "\"." << endl;
            }
            Instance* inst = new Instance(numOfFeatures);
            for (int i = 0; i < numOfFeatures; ++i) {
                double internal = dataset->metadata->featureList[i]->convertValueToInternal(tokens[i]);
                inst->featureVector[i] = internal;
            }
            double classInternal = dataset->metadata->classVariable->convertValueToInternal(tokens[numOfFeatures]);
            inst->classLabel = classInternal;
            dataset->trainSet.push_back(inst);
        }
    }
    
    header = true;
    while (finTest.good()) {
        getline(finTest, line);
        removeComment(line);
        if (line.empty())
            continue;
        vector<string> tokens = tokenize(line);
        if (header) {
            string lineType = toLower(tokens[0]);
            if (lineType == "@data") {
                header = false;
            }
        } else {
            if (tokens.size() != numOfFeatures + 1) {
                cerr << "Error parsing \"" << line << "\"." << endl;
            }
            Instance* inst = new Instance(numOfFeatures);
            for (int i = 0; i < numOfFeatures; ++i) {
                double internal = dataset->metadata->featureList[i]->convertValueToInternal(tokens[i]);
                inst->featureVector[i] = internal;
            }
            double classInternal = dataset->metadata->classVariable->convertValueToInternal(tokens[numOfFeatures]);
            inst->classLabel = classInternal;
            dataset->testSet.push_back(inst);
        }
    }
    
    finTrain.close();
    finTest.close();
    
    return dataset;
}

void Dataset::print() {
    cout << "@relation " << metadata->name << endl;
    for (int i = 0; i < metadata->numOfFeatures; ++i) {
        Feature* feature = metadata->featureList[i];
        cout << "@attribute '" << feature->getName() << "' ";
        if (feature->getType() == "numeric") {
            cout << "numeric" << endl;
        } else {
            cout << "{'" << feature->convertInternalToValue(0) << "'";
            for (int j = 1; j < feature->getRange(); ++j)
                cout << ",'" << feature->convertInternalToValue(j) << "'";
            cout << "}" << endl;
        }
    }
    cout << "@attribute '" << metadata->classVariable->getName() << "' ";
    cout << "{'" << metadata->classVariable->convertInternalToValue(0) << "'";
    for (int j = 1; j < metadata->classVariable->getRange(); ++j)
        cout << ",'" << metadata->classVariable->convertInternalToValue(j) << "'";
    cout << "}" << endl;
    
    cout << "@data" << endl;
    
    cout << "%Training" << endl;
    for (int i = 0; i < trainSet.size(); ++i) {
        Instance* inst = trainSet[i];
        for (int j = 0; j < metadata->numOfFeatures; ++j) {
            string val = metadata->featureList[j]->convertInternalToValue(inst->featureVector[j]);
            if (metadata->featureList[j]->getType() == "numeric")
                cout << val << ",";
            else
                cout << "'" << val << "',";
        }
        cout << "'" << metadata->classVariable->convertInternalToValue(inst->classLabel) << "'" << endl;
    }
    
    cout << "%Testing" << endl;
    for (int i = 0; i < testSet.size(); ++i) {
        Instance* inst = testSet[i];
        for (int j = 0; j < metadata->numOfFeatures; ++j) {
            string val = metadata->featureList[j]->convertInternalToValue(inst->featureVector[j]);
            if (metadata->featureList[j]->getType() == "numeric")
                cout << val << ",";
            else
                cout << "'" << val << "',";
        }
        cout << "'" << metadata->classVariable->convertInternalToValue(inst->classLabel) << "'" << endl;
    }
}