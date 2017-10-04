#include "Evaluator.h"

Evaluator::Evaluator() {
	folds = 10;
	seed = 123456789;
	p = .8;
	t = crossFold;
};

Evaluator::Evaluator(const TrainTestSets &tts, vector<string> &options) {
	seed = 123456789;
	folds = 10;
	p = .8;
	t = crossFold;
	setOptions(options);
	partitions.resize(2);
	partitions[0] = tts.getTrainingSet();
	partitions[1] = tts.getTestSet();
}

void Evaluator::evaluateOne(vector<int> results, DataSet& test) {
	double correct = 0;
	double total = 0;
	for (int i = 0; i < results.size(); i++) {
		if (test.getExamples()[i][test.getAttributes().getSize()-1] == results[i]) {
			correct++;
		}
		total++;
	}
	//	cout << "Accuracy = " << correct / total << endl;
	performance.addAccuracy(correct, total);
}

Performance Evaluator::getPerformance() const {
	return performance;
}

void Evaluator::evaluateX(Classifier* c) {
	partition();
	performance.clear();
	for (int i = 0; i < folds; i++) {
		DataSet train(attributes);
		for (int j = 0; j < folds; j++) {
			if (j != i) {
				for (int k = 0; k < partitions[j].getExamples().size(); k++) {
					train.add(partitions[j].getExamples()[k]);
				}
			}
		}
		c->train(train);
		evaluateOne(c->classify(partitions[i]), partitions[i]);
	}
	cout << "Accuracy = " << getPerformance().getAccuracy() << ", variance = " << getPerformance().getVariance() << endl;
}

void Evaluator::evaluateP(Classifier* c) {
	performance.clear();
	proportion();
	c->train(partitions[0]);
	evaluateOne(c->classify(partitions[1]), partitions[1]);
	cout << "Accuracy = " << getPerformance().getAccuracy() << endl;
}

void Evaluator::evaluateT(Classifier *c) {
	performance.clear();
	c->train(partitions[0]);
	evaluateOne(c->classify(partitions[1]), partitions[1]);
	cout << "Accuracy = " << getPerformance().getAccuracy() << endl;
}

void Evaluator::evaluate(Classifier* c) {
	if (t == crossFold) {
		//cout << "Crossfold\n";
		evaluateX(c);
	}
	else if (t == holdOut) {
		//cout << "Holdout\n";
		evaluateP(c);
	}
	else {
		//cout << "TestSet\n";
		evaluateT(c);
	}
}

void Evaluator::setOptions(vector<string>& options) {
	for (int i = 1; i < options.size(); i++) {
		char c = options[i][1];
		switch (c) {
		case 's':
			i++;
			seed = atoi(options[i].c_str());
			break;
		case 'x':
			i++;
			t = crossFold;
			folds = atoi(options[i].c_str());
			break;
		case 'p':
			i++;
			p = atof(options[i].c_str());
			t = holdOut;
			break;
		case 'T':
			i++;
			t = testSet;
			break;
		default:
			i++;
			break;
		}
	}
}

void Evaluator::partition() {
	attributes = partitions[0].getAttributes();
	partitions = partitions[0].partition(folds);
}

void Evaluator::proportion() {
	attributes = partitions[0].getAttributes();
	partitions = partitions[0].proportion(p);
}