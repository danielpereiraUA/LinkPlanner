#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "delayer.h"


void delayer::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool delayer::runBlock(void){
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();


	int process = min(ready, space);

	t_real out;
	int delay = 9;

	if (process == 0) return false;


	for (int i = 0; i < ready; i++) {

		t_real signalValue;
		inputSignals[0]->bufferGet(&signalValue);
		auxint = auxint + 1;

		if (auxint >= delay)
		{
			out = signalValue;
			outputSignals[0]->bufferPut(out);
		}

	}

	return true;
}