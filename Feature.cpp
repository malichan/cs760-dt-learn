#include <sstream>
#include <cmath>

#include "Feature.hpp"

double NumericFeature::convertValueToInternal(const string& str) const {
    return stod(str);
}

double NominalFeature::convertValueToInternal(const string& str) const {
    return (double)getValueIndex(str);
}

string NumericFeature::convertInternalToValue(double val) const {
    stringstream ss;
    ss << val;
    return ss.str();
}

string NominalFeature::convertInternalToValue(double val) const {
    return getValueName((int)round(val));
}