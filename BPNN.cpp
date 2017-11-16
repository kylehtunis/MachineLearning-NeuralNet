#include "BPNN.h"

BPNN::BPNN() {
	eta = .9;
	eMin = .1;
	hiddenLayerSize = 5;
	force = false;
}

BPNN::BPNN(DataSet& ds) {
	eta = .9;
	eMin = .1;
	hiddenLayerSize = 5;
	force = false;
	train(ds);
}

void BPNN::setOptions(vector<string> options) {
	for (int i = 1; i < options.size(); i++) {
		char c = options[i][1];
		switch (c) {
		case 'e':
			i++;
			eMin = atof(options[i].c_str());
			break;
		case 'l':
			i++;
			eta = atof(options[i].c_str());
			break;
		case 'h':
			i++;
			hiddenLayerSize = atoi(options[i].c_str());
			break;
		case 'f':
			force = true;
		default:
			i++;
		}
	}
}

void BPNN::train(DataSet& dsN) {
	//step 1: determine hyperparameters
	srand(dsN.getSeed());
	DataSet dsB = dsN.nominalToBinary();
	v.resize(hiddenLayerSize,
		vector<double>(dsB.getAttributes().getSize())); // HL node, input node
	w.resize(dynamic_cast<NominalAttribute*>(dsB.getAttributes().get(dsB.getAttributes().getSize() - 1))->getSize(),
		vector<double>(hiddenLayerSize + 1)); // output node, HL node
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			v[i][j] = (double)rand() / RAND_MAX;
		}
	}
	for (int i = 0; i < w.size(); i++) {
		for (int j = 0; j < w[i].size(); j++) {
			w[i][j] = (double)rand() / RAND_MAX;
		}
	}
	double previousError = 0;
	int repeatCount = 0;
	double error = 0;
	int q = 1;
	//step 2: start training by classifying each sample
	do {
		error = 0;
		for (int p = 0; p < dsB.getExamples().size(); p++) {
			Example z = dsB.getExamples()[p];
			vector<double> d(dynamic_cast<NominalAttribute*>(dsB.getAttributes().get(dsB.getAttributes().getSize()-1))->getSize(), 0);
			d[z.at(z.size() - 1)] = 1;
			vector<double> y(hiddenLayerSize + 1);
			for (int j = 0; j < y.size() - 1; j++) {
				double dotProduct = 0;
				for (int i = 0; i < z.size() - 1; i++) {
					double x = z[i];
					double b = v[j][i];
					dotProduct += (z[i] * v[j][i]);
				}
				y[j] = sigmoid(dotProduct);
			}
			y[y.size() - 1] = -1;
			vector<double> o(w.size());
			for (int k = 0; k < o.size(); k++) {
				double dotProduct = 0;
				for (int i = 0; i < y.size(); i++) {
					dotProduct += (y[i] * w[k][i]);
				}
				o[k] = sigmoid(dotProduct);
			}

			//step 3: compute error
			for (int k = 0; k < o.size(); k++) {
				error += (.5*(d[k] - o[k])*(d[k]-o[k]));
			}
			//cout << error << endl;
			//step 4: compute error signal vectors
			vector<double> eO(o.size());
			vector<double> eY(y.size() - 1);
			for (int k = 0; k < eO.size(); k++) {
				eO[k] = (d[k] - o[k])*(1 - o[k])*o[k];
			}
			for (int j = 0; j < eY.size(); j++) {
				eY[j] = 0;
				for (int k = 0; k < eO.size(); k++) {
					eY[j] += (eO[k] * w[k][j]);
				}
				eY[j] *= (y[j] * (1 - y[j]));
			}

			//step 5: adjust output layer weights
			for (int k = 0; k < o.size(); k++) {
				for (int j = 0; j < y.size() - 1; j++) {
					w[k][j] += eta*eO[k] * y[j];
				}
			}
			
			//step 6: adjust hidden layer weights
			for (int j = 0; j < y.size() - 1; j++) {
				for (int i = 0; i < z.size()-1; i++) {
					v[j][i] += eta*eY[j] * z[i];
				}
			}
		}
		//cout << "Error: " << error << endl;
		q++;
		if (previousError - error <= (.0000001)) {
			repeatCount++;
		}
		if (repeatCount >= 25 && !force) {
			cout << "Got stuck in a local minimum :( Exiting after " << q << " epochs" << endl;
			cout << "Error got down to " << error << endl;
			break;
		}

		previousError = error;
	} while (error > eMin);
	if (repeatCount<10) {
		cout << "Converged after " << q << " epochs" << endl;
	}
}

int BPNN::classify(Example& z) {
	vector<double> y(hiddenLayerSize + 1);
	for (int j = 0; j < y.size() - 1; j++) {
		double dotProduct = 0;
		for (int i = 0; i < z.size() - 1; i++) {
			double x = z[i];
			double b = v[j][i];
			dotProduct += (z[i] * v[j][i]);
		}
		y[j] = sigmoid(dotProduct);
	}
	y[y.size() - 1] = -1;
	vector<double> o(w.size());
	for (int k = 0; k < o.size(); k++) {
		double dotProduct = 0;
		for (int i = 0; i < y.size(); i++) {
			dotProduct += (y[i] * w[k][i]);
		}
		o[k] = sigmoid(dotProduct);
	}
	int oMaxNdx = -1;
	double oMax = -1;
	for (int i = 0; i < o.size(); i++) {
		if (o[i] > oMax) {
			oMax = o[i];
			oMaxNdx = i;
		}
	}
	return oMaxNdx;
}

vector<int> BPNN::classify(DataSet& dsN) {
	DataSet ds = dsN.nominalToBinary();
	vector<int> result(ds.getExamples().size());
	for (int i = 0; i < ds.getExamples().size(); i++) {
		result[i] = classify(ds.getExamples()[i]);
	}
	return result;
}



double BPNN::sigmoid(double x) {
	double num = exp(x*-1);
	num++;
	return (1 / num);
}