#include "Performance.h"

Performance::Performance() {}

vector<double> Performance::getAccuracies() const {
	return accuracies;
}

void Performance::addAccuracy(double acc) {
	accuracies.push_back(acc);
}

void Performance::addAccuracy(double correct, double total) {
	accuracies.push_back(correct / total);
}

double Performance::getVariance() {
	if (accuracies.size() <= 1) {
		return 0;
	}
	double sumSquares = 0;
	double sum = 0;
	for (int i = 0; i < accuracies.size(); i++) {
		sum += accuracies[i];
		sumSquares += (accuracies[i]*accuracies[i]);
	}
	return ((sumSquares - ((sum*sum) / accuracies.size())) / (accuracies.size() - 1));
}

double Performance::getAccuracy() {
	double sum = 0;
	for (int i = 0; i < accuracies.size(); i++) {
		sum += accuracies[i];
	}
	return (sum / accuracies.size());
}

void Performance::clear() {
	accuracies.clear();
}