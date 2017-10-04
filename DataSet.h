#ifndef DATASET_H
#define DATASET_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <math.h>
#include "Attributes.h"
#include "Examples.h"
#include "Nominal.h"
#include "Binary.h"

using namespace std;

class DataSet {
private:
	friend ostream& operator<<(ostream& os, const DataSet& ds);
protected:
	string name;
	Attributes attributes;
	Examples examples;
	long seed;
public:
	DataSet();
	DataSet(const Attributes& att);
	void add(const DataSet& ds);
	void add(const Example& example);
	Attributes& getAttributes();
	Examples getExamples() const;
	long getSeed();
	bool getHasNominalAttributes();
	bool getHasNumericAttributes();
	void load(string file);
	void parse(istream& is);
	void setOptions(const vector<string> options);
	void setSeed(long s);
	void addExample(Example ex);
	vector<DataSet> partition(int) const;
	vector<DataSet> proportion(double) const;
	double entropy();
	int getMode();
	DataSet nominalToBinary();
};
#endif // !DATASET_H
