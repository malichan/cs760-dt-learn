#include <sstream>
#include <cmath>

#include "Feature.hpp"

double NumericFeature::convertValueToInternal(const string& str) {
    return stod(str);
}

double NominalFeature::convertValueToInternal(const string& str) {
    return (double)getValueIndex(str);
}

string NumericFeature::convertInternalToValue(double val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

string NominalFeature::convertInternalToValue(double val) {
    return getValueName((int)round(val));
}