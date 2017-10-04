#ifndef PERFORMANCE__H
#define PERFORMANCE__H

#include <iostream>
#include "DataSet.h"

using namespace std;

class Performance {
private:
	vector<double> accuracies;
public:
	Performance();
	vector<double> getAccuracies() const;
	void addAccuracy(double acc);
	void addAccuracy(double correct, double total);
	double getVariance();
	double getAccuracy();
	void clear();
};




#endif // !PERFORMANCE__H
