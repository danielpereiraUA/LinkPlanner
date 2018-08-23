# ifndef PROGRAM_INCLUDE_NETXPTO_H_
# define PROGRAM_INCLUDE_NETXPTO_H_

# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <complex>
# include <algorithm>	// bind1st
# include <functional>	// bind1st


const int MAX_NAME_SIZE = 256;					// Maximum size of names
const long int MAX_Sink_LENGTH = 100000;		// Maximum Sink Block number of values
const int MAX_BUFFER_LENGTH = 10000;			// Maximum Signal buffer length
const int MAX_TOPOLOGY_SIZE = 100;				// Maximum System topology size 
const int MAX_TAPS = 1000;						// Maximum Taps Number
const double PI = 3.1415926535897932384;		// Value of pi
const double SPEED_OF_LIGHT = 299792458;		// Speed of light in vaccum
const double PLANCK_CONSTANT = 6.626070040e-34; // NIST
const int MAX_NUMBER_OF_PATHS = 2;

using namespace std;

typedef unsigned int t_binary;
typedef int t_integer;
typedef double t_real;
typedef complex<t_real> t_complex;
typedef struct { t_complex x; t_complex y; } t_complex_xy;
typedef struct { t_real probabilityAmplitude;  t_real polarization; } t_photon;
typedef struct { t_photon path[MAX_NUMBER_OF_PATHS]; } t_photon_mp;
typedef complex<t_real> t_iqValues;
typedef struct { string fieldName; string fieldValue; } t_message_field;
typedef vector<t_message_field> t_message;

enum signal_value_type {BinaryValue, IntegerValue, RealValue, ComplexValue, ComplexValueXY, PhotonValue, PhotonValueMP, Message};


//########################################################################################################################################################
//############################################################## SIGNALS DECLARATION AND DEFINITION ######################################################
//########################################################################################################################################################

// Root class for signals
class Signal {

	long int firstValueToBeSaved{ 1 };				// First value (>= 1) to be saved
	bool saveSignal{ true };
							
public:

	/* Parameters */

	string type;									// Signal type
	signal_value_type valueType;					// Signal samples type

	string fileName{ "" };							// Name of the file where data values are going to be saved
	string folderName{ "signals" };					// folder where signals are going to be saved by default

	
	long int numberOfValuesToBeSaved{ -1 };			// Number of values to be saved, if -1 all values are going to be saved

	int inPosition{ 0 };							// Next position for the buffer input values
	int outPosition{ 0 };							// Next position for the buffer output values
	bool bufferEmpty{ true };						// Flag bufferEmpty
	bool bufferFull{ false };						// Flag bufferFull
	long int numberOfSavedValues{ 0 };				// Number of saved values
	long int count;									// Number of values that have already entered in the buffer

	void *buffer{ NULL };							// Pointer to buffer

	int bufferLength{ 512 };						// Buffer length

	double symbolPeriod{ 1.0 };						// Signal symbol period (it is the inverse of the symbol rate)
	double samplingPeriod{ 1.0 };					// Signal sampling period (it is the time space between two samples)
	double samplesPerSymbol{ 1.0 };					// samplesPerSymbol = symbolPeriod / samplingPeriod;

	double centralWavelength{ 1550E-9 };
	double centralFrequency{ SPEED_OF_LIGHT / centralWavelength };


	/* Methods */

	Signal() {};
	Signal(string fName) {setFileName(fName); };	// Signal constructor
	Signal(string fName, bool sSignal) { setFileName(fName); setSaveSignal(sSignal); };
	Signal(int bLength) { setBufferLength(bLength); };

	// Signal constructor

	~Signal() { if (!(valueType == Message)) { delete buffer; }; };					// Signal destructor

	void close();									// Empty the signal buffer and close the signal file
	int space();									// Returns the signal buffer space
	int ready();									// Returns the number of samples in the buffer ready to be processed
	void writeHeader();								// Opens the signal file in the default signals directory, \signals, and writes the signal header
	void writeHeader(string signalPath);			// Opens the signal file in the signalPath directory, and writes the signal header


	template<typename T>							// Puts a value in the buffer
	void bufferPut(T value) {
		(static_cast<T *>(buffer))[inPosition] = value;
		if (bufferEmpty) bufferEmpty = false;
		inPosition++;
		if (inPosition == bufferLength) { 
			inPosition = 0; 
			if (saveSignal) {
				if (firstValueToBeSaved <= bufferLength) {
					char *ptr = (char *) buffer;
					ptr = ptr + (firstValueToBeSaved - 1)*sizeof(T);
					ofstream fileHandler("./" + folderName + "/" + fileName, ios::out | ios::binary | ios::app);
					int auxSizeOf = sizeof(T);
					fileHandler.write(ptr, (bufferLength - (firstValueToBeSaved - 1 ))* sizeof(T));
					fileHandler.close();
					firstValueToBeSaved = 1;
				}
				else {
					firstValueToBeSaved = firstValueToBeSaved - bufferLength;
				}
			}
		}
		if (inPosition == outPosition) bufferFull = true;
	};

	void virtual bufferGet();
	void virtual bufferGet(t_binary *valueAddr);
	void virtual bufferGet(t_integer *valueAddr);
	void virtual bufferGet(t_real *valueAddr);
	void virtual bufferGet(t_complex *valueAddr);
	void virtual bufferGet(t_complex_xy *valueAddr);
	void virtual bufferGet(t_photon *valueAddr);
	void virtual bufferGet(t_photon_mp *valueAddr);
	void virtual bufferGet(t_message *valueAdr);
	
	void setSaveSignal(bool sSignal){ saveSignal = sSignal; };
	bool const getSaveSignal(){ return saveSignal; };

	void setType(string sType, signal_value_type vType) { type = sType; valueType = vType; };
	void setType(string sType) { type = sType; };
	string getType(){ return type; };

	void setInPosition(int iPosition) { inPosition = iPosition; };
	int getInPosition() { return inPosition; };

	void setOutPosition(int oPosition) { outPosition = oPosition; };
	int getOutPosition() { return outPosition; };

	void setValueType(signal_value_type vType) { valueType = vType; };
	signal_value_type getValueType(){ return valueType; };

	void setFileName(string fName) { fileName = fName; };
	string getFileName(){ return fileName; };

	void setFolderName(string fName) { folderName = fName; };
	string getFolderName(){ return folderName; };
	
	void setBufferLength(int bLength) { bufferLength = bLength; };
	int getBufferLength(){ return bufferLength; };

	void setFirstValueToBeSaved(long int fValueToBeSaved) { firstValueToBeSaved = fValueToBeSaved; };
	long int getFirstValueToBeSaved(){ return firstValueToBeSaved; };

	void setNumberOfValuesToBeSaved(long int nOfValuesToBeSaved) { numberOfValuesToBeSaved = nOfValuesToBeSaved; };
	long int getNumberOfValuesToBeSaved(){ return numberOfValuesToBeSaved; };

	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSymbolPeriod() { return symbolPeriod; };

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSamplingPeriod(){ return samplingPeriod; };

	void setSamplesPerSymbol(double sPeriod) { samplesPerSymbol = sPeriod; symbolPeriod = samplesPerSymbol * samplingPeriod; samplingPeriod = symbolPeriod / samplesPerSymbol; };
	double getSamplesPerSymbol(){ return samplesPerSymbol; };

	void setCentralFrequency(double cFrequency){ centralFrequency = cFrequency; centralWavelength = SPEED_OF_LIGHT / centralFrequency; }
	double getCentralFrequency(){ return centralFrequency; };

	void setCentralWavelength(double cWavelength){ centralWavelength = cWavelength; centralFrequency = SPEED_OF_LIGHT / centralWavelength; }
	double getCentralWavelength(){ return centralWavelength; }

};


class TimeDiscrete : public Signal {
public:
	TimeDiscrete(string fName) { setFileName(fName); }
	TimeDiscrete(){}
};


class TimeDiscreteAmplitudeDiscrete : public Signal {
public:
	TimeDiscreteAmplitudeDiscrete(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscrete(){}
};


class TimeDiscreteAmplitudeContinuous : public Signal {
public:
	TimeDiscreteAmplitudeContinuous(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeContinuous(){}
};


class TimeDiscreteAmplitudeDiscreteReal : public Signal {
public:
	TimeDiscreteAmplitudeDiscreteReal(string fName) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeDiscreteAmplitudeDiscreteReal(string fName, int bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeDiscreteReal(int bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeDiscreteReal(){ setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeDiscreteAmplitudeDiscreteComplex : public Signal {
	
public:
	TimeDiscreteAmplitudeDiscreteComplex(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscreteComplex() {}
};


class Binary : public Signal {
	
public:
	Binary(string fName) { setType("Binary", BinaryValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_binary[bufferLength]; }
	Binary(string fName, int bLength) { setType("Binary", BinaryValue);  setFileName(fName); setBufferLength(bLength); }
	Binary(int bLength) { setType("Binary", BinaryValue);  setBufferLength(bLength); }
	Binary() { setType("Binary", BinaryValue); if (buffer == nullptr) buffer = new t_binary[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_binary[bLength]; };
};


class TimeDiscreteAmplitudeContinuousReal : public Signal {
public:
	TimeDiscreteAmplitudeContinuousReal(string fName) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeDiscreteAmplitudeContinuousReal(string fName, int bLength) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousReal(int bLength) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousReal(){ setType("TimeDiscreteAmplitudeContinuousReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeDiscreteAmplitudeContinuousComplex : public Signal {
public:
	TimeDiscreteAmplitudeContinuousComplex(string fName) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	TimeDiscreteAmplitudeContinuousComplex(string fName, int bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousComplex(int bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousComplex(){ setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};


class TimeContinuous : public Signal {
public:
	TimeContinuous(){}
};

class PhotonStream : public Signal {

public:
	PhotonStream(int bLength) { setType("PhotonStream", PhotonValue); setBufferLength(bLength); }
	PhotonStream() { setType("PhotonStream", PhotonValue); if (buffer == nullptr) buffer = new t_photon[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_photon[bLength]; };
};

class PhotonStreamMP : public Signal {

public:
	PhotonStreamMP(int bLength) { setType("PhotonStreamMP", PhotonValueMP); setBufferLength(bLength); }
	PhotonStreamMP() { setType("PhotonStreamMP", PhotonValueMP); if (buffer == nullptr) buffer = new t_photon_mp[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_photon_mp[bLength]; };

};


class TimeContinuousAmplitudeDiscrete : public Signal {
public:
	TimeContinuousAmplitudeDiscrete(){}
};


class TimeContinuousAmplitudeContinuous : public Signal {
public:
	TimeContinuousAmplitudeContinuous(){}
};


class TimeContinuousAmplitudeDiscreteReal : public Signal {
public:
	TimeContinuousAmplitudeDiscreteReal(string fName) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeContinuousAmplitudeDiscreteReal(string fName, int bLength) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue);  setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteReal(int bLength) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue);  setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteReal(){ setType("TimeContinuousAmplitudeDiscreteReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeContinuousAmplitudeDiscreteComplex : public Signal {
public:
	TimeContinuousAmplitudeDiscreteComplex(string fName) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	TimeContinuousAmplitudeDiscreteComplex(string fName, int bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteComplex(int bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteComplex(){ setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};


class TimeContinuousAmplitudeContinuousReal : public Signal {
public:
	TimeContinuousAmplitudeContinuousReal(string fName) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeContinuousAmplitudeContinuousReal(string fName, int bLength) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousReal(int bLength) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousReal(){ setType("TimeContinuousAmplitudeContinuousReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };

};


class TimeContinuousAmplitudeContinuousComplex : public Signal {
public:
	TimeContinuousAmplitudeContinuousComplex(string fName) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	TimeContinuousAmplitudeContinuousComplex(string fName, int bLength) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousComplex(int bLength) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousComplex(){ setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};

class BandpassSignal : public Signal {
public:
	BandpassSignal(string fName) { setType("BandpassSignal", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	BandpassSignal(string fName, int bLength) { setType("BandpassSignal", ComplexValue); setFileName(fName); setBufferLength(bLength); }
	BandpassSignal(int bLength) { setType("BandpassSignal", ComplexValue); setBufferLength(bLength); }
	BandpassSignal(){ setType("BandpassSignal", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };

};

// the setType is BandpassSignal to garantee the compatibility with the Visualizer
class OpticalSignal : public Signal {
public:
	OpticalSignal(string fName) { setType("BandpassSignal", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	OpticalSignal(string fName, int bLength) { setType("BandpassSignal", ComplexValue); setFileName(fName); setBufferLength(bLength); }
	OpticalSignal(int bLength) { setType("BandpassSignal", ComplexValue); setBufferLength(bLength); }
	OpticalSignal() { setType("BandpassSignal", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };

};


class OpticalSignalXY : public Signal {
public:
	OpticalSignalXY(string fName) { setType("OpticalSignalXY", ComplexValueXY); setFileName(fName); if (buffer == nullptr) buffer = new t_complex_xy[bufferLength]; }
	OpticalSignalXY(string fName, int bLength) { setType("OpticalSignalXY", ComplexValueXY); setFileName(fName); setBufferLength(bLength); }
	OpticalSignalXY(int bLength) { setType("OpticalSignalXY", ComplexValueXY); setBufferLength(bLength); }
	OpticalSignalXY() { setType("OpticalSignalXY", ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_complex_xy[bLength]; };
};


/*class MultiModeBandpassSignal : BandpassSignal {
public:
	MultiModeBandpassSignal(int nBandpassSignals) {

	};
private:
	int numberOfBandpassSignals;
	vector<BandpassSignal> bandpasslSignals;
	vector<double> centralWavelengths;
	vector<double> centralFrequencies;
};*/

class Messages : public Signal {
public:
	Messages(string fName) { setType("Message", Message); setFileName(fName); if (buffer == nullptr) buffer = new t_message[bufferLength]; }
	Messages(string fName, int bLength) { setType("Message", Message); setFileName(fName); setBufferLength(bLength); }
	Messages(int bLength) { setType("Message", Message); setBufferLength(bLength); }
	Messages() { setType("Message", Message); if (buffer == nullptr) buffer = new t_message[bufferLength]; }

	void setBufferLength(int bLength) { bufferLength = bLength; delete[] buffer; if (bLength != 0) buffer = new t_message[bLength]; };
	
	void bufferPut(t_message);
};

//########################################################################################################################################################
//########################################################## GENERIC BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################


// Descrives a generic Block.
class Block {

 public:

	/* Input Parameters */
	int numberOfInputSignals;
	int numberOfOutputSignals;
	vector<Signal *> inputSignals;
	vector<Signal *> outputSignals;

	/* Methods */
	Block(){};
	Block(vector<Signal*> &InputSig, vector<Signal*> &OutputSig);

	void initializeBlock(vector<Signal*> &InputSig, vector<Signal*> &OutputSig);
	void initializeBlock();
	virtual void initialize(void){};

	virtual bool runBlock();

	void terminateBlock();
	virtual void terminate(void){};
};


class SuperBlock : public Block {

	/* State Variables */

	vector<Block*> moduleBlocks;

	/* Input Parameters */

	bool saveInternalSignals{ false };

public:


	/* Methods */
	SuperBlock() {};
	SuperBlock(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :Block(inputSignal, outputSignal){ setSaveInternalSignals(false); };

	void initialize(void);

	bool runBlock(void);

	void terminate(void);

	/* Set Methods */

	void setModuleBlocks(vector<Block*> mBlocks){ moduleBlocks = mBlocks; };

	void setSaveInternalSignals(bool sSignals);
	bool const getSaveInternalSignals(void){ return saveInternalSignals; };

};

class FIR_Filter : public Block {

	/* State Variable */
	vector<t_real> delayLine;

	bool saveImpulseResponse{ true };
	string impulseResponseFilename{ "impulse_response.imp" };

	/* Input Parameters */
	bool seeBeginningOfImpulseResponse{ true };

public:

	/* State Variable */
	vector<t_real> impulseResponse;

	/* Input Parameters */
	int impulseResponseLength;							// filter order + 1 (filter order = number of delays)


	/* Methods */
	FIR_Filter() {};
	FIR_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig){};

	void initializeFIR_Filter(void);

	bool runBlock(void);

	void terminate(void){};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void){ return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength(){ return impulseResponseLength; }

	void setSeeBeginningOfImpulseResponse(bool sBeginning){ seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse(){ return seeBeginningOfImpulseResponse; };

};


class FD_Filter : public Block {
	
	/* State Variable */
	
	vector<t_real> inputBufferTimeDomain;
	vector<t_real> outputBufferTimeDomain;

	int inputBufferPointer{ 0 };
	int outputBufferPointer{ 0 };
	
	/* Input Parameters */
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };
	int transferFunctionLength{ 128 };

	int inputBufferTimeDomainLength{ transferFunctionLength };
	int outputBufferTimeDomainLength{ transferFunctionLength };

public:
	/* State Variable */
	vector<t_complex> transferFunction;

	/* Methods */
	FD_Filter() {};
	FD_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter(void);

	bool runBlock(void);

	void terminate(void) {};

	void setInputBufferTimeDomainLength(int iBufferTimeDomainLength) { inputBufferTimeDomainLength = iBufferTimeDomainLength; };
	int const getInputBufferTimeDomainLength() { return inputBufferTimeDomainLength; }

	void setOutputBufferTimeDomainLength(int oBufferTimeDomainLength) { outputBufferTimeDomainLength = oBufferTimeDomainLength; };
	int const getOutputBufferTimeDomainLength() { return outputBufferTimeDomainLength; }

	void setInputBufferPointer(int iBufferPointer) { inputBufferPointer = iBufferPointer; };
	int const getInputBufferPointer() { return inputBufferPointer; }

	void setOutputBufferPointer(int oBufferPointer) { outputBufferPointer = oBufferPointer; };
	int const getOutputBufferPointer() { return outputBufferPointer; }

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };
	int const getTransferFunctionLength() { return transferFunctionLength; };

};


// Generates a complex signal knowing the real part and the complex part.
class RealToComplex : public Block {
 public:
	 RealToComplex(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};

// Separates the complex signal into two parts: real and imaginary.
/*class ComplexToReal : public Block {
 public:
	 ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};*/




//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################


class System {

 public:
  System(vector<Block *> &MainSystem);	
  void terminate();										
  void run();
  void run(string signalPath);

  string signalsFolder{ "signals" };
  char fileName[MAX_NAME_SIZE];  // Name of the file with system description (.sdf)
  char name[MAX_NAME_SIZE];  // System Name
  int numberOfBlocks;  // Number of system Blocks
  int (*topology)[MAX_TOPOLOGY_SIZE];  // Relationship matrix
  vector<Block *> SystemBlocks;  // Pointer to an array of pointers to Block objects
};

//########################################################################################################################################################
//############################################################### GENERIC DSP FUNCTIONS ##################################################################
//########################################################################################################################################################


class OverlapMethod
{

public:

	void overlapSaveSymComplexIn(vector<complex <double>> &v_in, vector<complex <double>> &v_out, vector<complex <double>> Hf, int NFFT);
	void overlapSaveSyRealIn(vector<double> &v_in, vector<double> &v_out, vector<double> Hf, int NFFT);
	void overlapSaveAsym(vector<double> &real_in, vector<double> &imag_in, vector<double> &real_out, vector<double> &imag_out, vector<double> h_real, vector<double> h_imag, int M, int L, int N);
	void overlapSaveSym(vector<double> &real_in, vector<double> &imag_in, vector<double> &real_out, vector<double> &imag_out, vector<double> h_real, vector<double> h_imag, int NFFT);
	void checkSize(vector<double> &real_in, vector<double> &imag_in, int L);

};

class Fft
{

public:
	vector<complex <double>> directTransformInReal(vector<double> real);

	vector<double> inverseTransformInCP(vector<complex <double>> &In);

	void directTransform(vector<double> &real, vector<double> &imag);

	void inverseTransform(vector<double> &real, vector<double> &imag);

	void transformRadix2(vector<double> &real, vector<double> &imag);

	void transformBluestein(vector<double> &real, vector<double> &imag);

	void convolve(const vector<double> &x, const vector<double> &y, vector<double> &out);

	void convolve(const vector<double> &xreal, const vector<double> &ximag, const vector<double> &yreal, const vector<double> &yimag, vector<double> &outreal, vector<double> &outimag);

};



class ComplexMult
{

public:

	void CMultVector(vector<double> &v1_real, vector<double> &v1_imag, vector<double> v2_real, vector<double> v2_imag);
	void CMultVector_Loop(vector<double> &v1_real, vector<double> &v1_imag, vector<double> v2_real, vector<double> v2_imag);
	vector<complex <double>> CMultVectorInCP(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);
	void ComplexVect2ReImVect(vector<complex <double>> &v_in, vector<double> &v1_real, vector<double> &v1_imag);
	void CMultVector_InComplex(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);
	void ReImVect2ComplexVect(vector<double> &v1_real, vector<double> &v1_imag, vector<complex <double>> &v_out);

};

/*
///////////////////// FFT function ////////////////////////
vector <complex<double>> fft(vector <double> real)
{
	Fft F;

	vector <complex<double>> Output;						// Type of output of this function : vector <complex<double> & Name of the function : Output
	
	ComplexMult CMult;
	vector<double> im(real.size(), 0);						// Create a vector for imaginary values 
	vector<complex <double>> v_out(real.size(), 0);
	size_t n = real.size();


	if (n == 0)
		return v_out;
	else if ((n & (n - 1)) == 0)							// Is power of 2 : Radix-2 Algorithim
		F.transformRadix2(real, im);
	else													// More complicated algorithm for arbitrary sizes : Bluestein Algorithim
		F.transformBluestein(real, im);


	CMult.ReImVect2ComplexVect(real, im, Output);
	return Output;

};

///////////////////// IFFT function ////////////////////////
vector<double> ifft(vector<complex<double>> input)
{
	Fft IF;
	ComplexMult split;
	
	vector <double> re(input.size(),0);						// Vector for holding REAL data
	vector <double> im(input.size(),0);						// Vector for holding IMAG data

	split.ComplexVect2ReImVect(input, re, im);				// Split complex data into real and imaginary vector

	IF.directTransform(im,re);								// Inverse fourier transformation

	for (int i=0; i<re.size(); i++)
	{
		re[i] = re[i] / re.size();							// Normalization of real data
		im[i] = im[i] / re.size();							// This will be zero in case of real time signal
	}
	
	vector <double> Output;
	Output = re;

	return Output;
};

*/


# endif // PROGRAM_INCLUDE_netxpto_H_