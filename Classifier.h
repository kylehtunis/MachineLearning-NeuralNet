#ifndef CLASSIFIER__H
#define CLASSIFIER__H

#include <iostream>
#include "DataSet.h"
#include "Attributes.h"

class Classifier {
public:
	virtual void train(DataSet&);
	virtual int classify(Example &ex);
	virtual vector<int> classify(DataSet &ds);
};





#endif // !CLASSIFIER__H
