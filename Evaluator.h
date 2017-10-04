#ifndef Evaluator__H
#define Evaluator__H

#include <iostream>
#include "Classifier.h"
#include "DataSet.h"
#include "Performance.h"
#include "TrainTestSets.h"
#include <algorithm>

enum testType {crossFold, holdOut, testSet};

class Evaluator {
private:
	vector<DataSet> partitions;
	Attributes attributes;
	Performance performance;
	void evaluateOne(vector<int> results, DataSet& test);
	void evaluateX(Classifier*);
	void evaluateP(Classifier*);
	void evaluateT(Classifier*);
	int folds;
	int seed;
	double p;
	testType t;
public:
	Evaluator();
	Evaluator(const TrainTestSets&, vector<string>&);
	void partition();
	void proportion();
	Performance getPerformance() const;
	void evaluate(Classifier*);
	void setOptions(vector<string>&);
};




#endif // !Evaluator__H
