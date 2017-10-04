#include "Main.h"

using namespace std;

int main(int argc, char* argv[]) {
	vector<string> options(argc);
	for (int i = 0; i < argc; i++) {
		options[i] = argv[i];
	}
	TrainTestSets tts(options);
	Evaluator eval(tts, options);
	BPNN* bp = new BPNN;
	bp->setOptions(options);
	eval.evaluate(bp);
	return 0;
}