#ifndef BPNN__H
#define BPNN__H

#include <iostream>
#include <vector>
#include <math.h>
#include "Classifier.h"
#include "DataSet.h"

class BPNN : public Classifier {
private:
	double eta;
	double eMin;
	int hiddenLayerSize;
	bool force;
	vector< vector <double> > w;
	vector< vector <double> > v;
	double sigmoid(double);
public:
	BPNN();
	BPNN(DataSet&);
	void setOptions(vector<string>);
	virtual void train(DataSet& ds);
	virtual int classify(Example& ex);
	virtual vector<int> classify(DataSet &ds);

};





#endif // !BPNN__H
