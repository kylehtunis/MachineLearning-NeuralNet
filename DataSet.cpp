#include "DataSet.h"
using namespace std;

DataSet::DataSet() {
	name = "unnamed";
	seed = 3267243;
}

DataSet::DataSet(const Attributes& att) {
	attributes = att;
	name = "unnamed";
	seed = 3267243;
}

void DataSet::add(const Example& ex) {
	examples.push_back(ex);
}

void DataSet::add(const DataSet& ds) {
	for (int i = 0; i < ds.examples.size(); i++) {
		examples.push_back(ds.examples[i]);
	}
}

Attributes& DataSet::getAttributes() {
	return attributes;
}

Examples DataSet::getExamples() const {
	return examples;
}

long DataSet::getSeed() {
	return seed;
}

bool DataSet::getHasNominalAttributes() {
	return attributes.getHasNominalAttributes();
}

bool DataSet::getHasNumericAttributes() {
	return attributes.getHasNumericAttributes();
}

void DataSet::load(string file) {
	ifstream is(file.c_str());
	if (is) {
		parse(is);
	}
	else {
		cout << "could not load file";
	}
}

void DataSet::parse(istream& is) {
	string line = new char;
	getline(is, line);
	string sLine = line;
	istringstream iss(sLine);
	string word = "";
	iss >> word;
	word = "";
	iss >> word;
	name = word;
	attributes.parse(is);
	attributes.setClassIndex(attributes.getSize() - 1);
	examples.setAttributes(attributes);
	examples.parse(is);
}

void DataSet::setSeed(long s) {
	seed = s;
}

void DataSet::setOptions(const vector<string> options) {
	int s = 0;
	for (int i = 1; i < options.size(); i++) {
		char c = options[i][1];
		switch (c) {
		case 's':
			i++;
			s = atof(options[i].c_str());
			setSeed(s);
			break;
		default:
			i++;
			continue;
		}
	}
}

void DataSet::addExample(Example ex) {
	examples.push_back(ex);
}

double DataSet::entropy() {
	vector<int> counts(dynamic_cast<NominalAttribute*>(attributes.getClassAttribute())->getSize());
	double total = 0;
	for (int i = 0; i < examples.size(); i++) {
		counts[examples[i][examples[i].size()-1]]++;
		total++;
	}
	double e = 0;
	for (int i = 0; i < counts.size(); i++) {
		if (counts[i] != 0) {
			double p = counts[i] / total;
			e += ((-p)*log2(p));
		}
	}
	return e;
}

vector<DataSet> DataSet::partition(int n) const {
	vector<int> indexes(examples.size());
	vector<DataSet> partitions(n, attributes);
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}
	srand(seed);
	random_shuffle(indexes.begin(), indexes.end());
	for (int i = 0; i < examples.size(); i++) {
		partitions[i%n].add(examples[indexes[i]]);
	}
	return partitions;
}

vector<DataSet> DataSet::proportion(double p) const {
	int trainNo = examples.size()*p;
	vector<DataSet> sets(2, attributes);
	vector<int> indexes(examples.size());
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}
	srand(seed);
	random_shuffle(indexes.begin(), indexes.end());
	for (int i = 0; i < examples.size(); i++) {
		if (i < trainNo) {
			sets[0].add(examples[indexes[i]]);
		}
		else {
			sets[1].add(examples[indexes[i]]);
		}
	}
	return sets;
}

int DataSet::getMode() {
	vector<int> counts(attributes.get(attributes.getSize() - 1)->getSize());
	for (int i = 0; i < examples.size(); i++) {
		counts[examples[i][attributes.getSize()-1]]++;
	}
	int maxCount = -1;
	int maxLabel = -1;
	for (int i = 0; i < counts.size(); i++) {
		if (counts[i] > maxCount) {
			maxCount = counts[i];
			maxLabel = i;
		}
	}
	return maxLabel;
}

DataSet DataSet::nominalToBinary() {
	DataSet ds;
	for (int i = 0; i < examples.size(); i++) {
		Example ex(0);
		for (int j = 0; j < examples[i].size()-1; j++) {
			int val = examples[i][j];
			if (dynamic_cast<NominalAttribute*>(attributes.get(j))->getSize() != 2) {
				for (int k = 0; k < dynamic_cast<NominalAttribute*>(attributes.get(j))->getSize(); k++) {
					if (val == k) {
						ex.push_back(1);
					}
					else {
						ex.push_back(0);
					}
				}
			}
			else {
				ex.push_back(examples[i][j]);
			}
		}
		ex.push_back(-1);
		ex.push_back(examples[i][examples[i].size() - 1]);
		//ex.shrink_to_fit();
		ds.addExample(ex);
	}
	for (int i = 0; i < attributes.getSize()-1; i++) {
		BinaryAttribute* ba = new BinaryAttribute(attributes.get(i)->getName());
		if (dynamic_cast<NominalAttribute*>(attributes.get(i))->getSize() != 2) {
			for (int j = 0; j < dynamic_cast<NominalAttribute*>(attributes.get(i))->getSize(); j++) {
				ds.getAttributes().add(ba);
			}
		}
		else {
			ds.attributes.add(attributes.get(i));
		}
	}
	ds.getAttributes().add(attributes.get(attributes.getSize() - 1));
	ds.name = name;
	return ds;
}

ostream& operator<<(ostream& os, const DataSet& ds) {
	os << "@dataset " << ds.name << endl << endl;
	os << ds.attributes << endl;
	os << ds.examples;
	return os;
}