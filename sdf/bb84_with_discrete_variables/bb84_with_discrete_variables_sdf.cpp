#include "netxpto_20180418.h"
#include <stdio.h>

#include "binary_source_20180118.h"
#include "clock_20171219.h"
#include "alice_qkd_20180409.h"
#include "single_photon_source_20171218.h"
#include "polarization_rotator_20180113.h"
#include "sink_20180118.h"
#include "bob_qkd_20180409.h"
#include "polarization_beam_splitter_20180109.h"
#include "single_photon_detector_20180206.h"
#include "super_block_interface_20180118.h"
#include "single_photon_receiver_20180303.h"
#include "bit_error_rate_20171810.h"
#include "SOP_modulator_20180319.h"
#include "optical_attenuator_20180304.h"

class BB84DiscreteVariablesInputParameters : public SystemInputParameters {
public:
	//INPUT PARAMETERS
	double rateOfPhotons{ 1e3 };
	vector<t_iqValues> iqAmplitudeValues = { { 0.0,0.0 },{ -45.0,0.0 },{ 90.0,0.0 },{ 45.0,0.0 } };
	double detectorWindowTimeOpen{ 0.2 };			// in units of time (ms), symbol period is equals to 1 ms, so this value must be smaller than 1 ms
	double detectorPhaseDelay{ 0.7 };				// in units of time (ms)
	double detectorProbabilityDarkCount{ 0.00 };
	double rotationAngle{ 4.0 };					// for deterministic sop change
	double elevationAngle{ 0.0 };					// for deterministic sop change
	double alpha_dB{ 0.0 };							// attenuation

	/* Initializes default parameters */
	BB84DiscreteVariablesInputParameters() : SystemInputParameters() {
		initializeInputParameterMap(); //Initializes the parameters
	}

	/* Initializes input parameters according to the program arguments*/
	BB84DiscreteVariablesInputParameters(int argc, char*argv[]) : SystemInputParameters(argc, argv) {
		initializeInputParameterMap(); //Initializes the parameters
		readSystemInputParameters();
	}

	//METHODS
	//Each parameter must be added to the parameter map by calling addInputParameter(string,param*)
	void initializeInputParameterMap() {
		addInputParameter("rateOfPhotons",&rateOfPhotons);
		addInputParameter("detectorWindowTimeOpen", &detectorWindowTimeOpen);
		addInputParameter("detectorPhaseDelay", &detectorPhaseDelay);
		addInputParameter("detectorProbabilityDarkCount", &detectorProbabilityDarkCount);
		addInputParameter("rotationAngle", &rotationAngle);
		addInputParameter("elevationAngle", &elevationAngle);
		addInputParameter("alpha_dB", &alpha_dB);
	}
};

int main(int argc, char *argv[]) {
	// #####################################################################################################
	// ################################# System Input Parameters #########################################
	// #####################################################################################################

	BB84DiscreteVariablesInputParameters param(argc,argv);

	// #####################################################################################################
	// ################################# Simulation Parameters #########################################
	// #####################################################################################################

	int numberOfSamplesPerSylbom{ 16 };
	
	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	// #################################################################################Alice Implementation
	Binary NUM_A{ "NUM_A.sgn",param.getOutputFolderName() };
	NUM_A.setSaveSignal(false);

	Binary Bin_1{ "Bin_1.sgn",param.getOutputFolderName() };
	Bin_1.setSaveSignal(false);

	Binary Bin_2{ "Bin_2.sgn",param.getOutputFolderName() };
	Bin_2.setSaveSignal(false);

	Binary Bin_1out{ "Bin1_out.sgn",param.getOutputFolderName() };
	Bin_1out.setSaveSignal(false);

	Binary MI_A{ "MI_A.sgn",param.getOutputFolderName() };
	MI_A.setBufferLength(512);
//	MI_A.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_A{ "CLK_A.sgn",param.getOutputFolderName() };
	CLK_A.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_A_1{ "CLKA_out.sgn",param.getOutputFolderName() };
	CLK_A_1.setSaveSignal(false);

	PhotonStreamXY S2{ "S2.sgn",param.getOutputFolderName() };
	S2.setSaveSignal(false);

	TimeContinuousAmplitudeDiscreteReal S3{ "S3.sgn",param.getOutputFolderName() };
	S3.setSaveSignal(false);

	PhotonStreamXY Q_C_1{ "Q_C_1.sgn",param.getOutputFolderName() };

	PhotonStreamXY Q_C_2{ "Q_C_2.sgn",param.getOutputFolderName() };

	PhotonStreamXY Q_C_3{ "Q_C_3.sgn",param.getOutputFolderName() };

	TimeContinuousAmplitudeDiscreteReal theta{ "theta.sgn",param.getOutputFolderName() };

	TimeContinuousAmplitudeDiscreteReal phi{ "phi.sgn",param.getOutputFolderName() };

	TimeContinuousAmplitudeContinuousReal clk_sop{ "CLK_sop.sgn",param.getOutputFolderName() };

	// ##################################################################################### Bob Implementation

	Binary NUM_B{ "NUM_B.sgn",param.getOutputFolderName() };
	NUM_B.setSaveSignal(false);

	Binary Bin_6{ "Bin_6.sgn",param.getOutputFolderName() };
	Bin_6.setSaveSignal(false);

	Binary MI_B{ "MI_B.sgn",param.getOutputFolderName() };
	MI_B.setBufferLength(512);
//	MI_B.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_B{ "CLK_B.sgn",param.getOutputFolderName() };
	CLK_B.setSaveSignal(false);

	TimeContinuousAmplitudeContinuous CLK_B_out1{ "CLKB_out1.sgn",param.getOutputFolderName() };
	CLK_B_out1.setSymbolPeriod(1 / param.rateOfPhotons);
	CLK_B_out1.setSamplingPeriod(1 / param.rateOfPhotons / numberOfSamplesPerSylbom);

	TimeContinuousAmplitudeContinuous CLK_B_out2{ "CLKB_out2.sgn",param.getOutputFolderName() };
	CLK_B_out2.setSymbolPeriod(1 / param.rateOfPhotons);
	CLK_B_out2.setSamplingPeriod(1 / param.rateOfPhotons / numberOfSamplesPerSylbom);

	TimeContinuousAmplitudeDiscreteReal S4{ "S4.sgn",param.getOutputFolderName() };
	S4.setSymbolPeriod(1 / param.rateOfPhotons);
	S4.setSamplingPeriod(1 / param.rateOfPhotons / numberOfSamplesPerSylbom);
	S4.setSaveSignal(false);

	PhotonStreamXY S5{ "S5.sgn",param.getOutputFolderName() };
	S5.setSymbolPeriod(1 / param.rateOfPhotons);
	S5.setSamplingPeriod(1 / param.rateOfPhotons / numberOfSamplesPerSylbom);
	S5.setSaveSignal(false);

	PhotonStreamMPXY S6{ "S6.sgn",param.getOutputFolderName() };
	S6.setSymbolPeriod(1 /param.rateOfPhotons);
	S6.setSamplingPeriod(1 /param.rateOfPhotons / numberOfSamplesPerSylbom);
	S6.setSaveSignal(false);


	TimeContinuousAmplitudeContinuous S7{ "S7.sgn",param.getOutputFolderName() };
	S7.setSymbolPeriod(1 /param.rateOfPhotons);
	S7.setSamplingPeriod(1 /param.rateOfPhotons / numberOfSamplesPerSylbom);

	TimeContinuousAmplitudeContinuous S9{ "S9.sgn",param.getOutputFolderName() };
	S9.setSymbolPeriod(1 /param.rateOfPhotons);
	S9.setSamplingPeriod(1 /param.rateOfPhotons / numberOfSamplesPerSylbom);

	TimeDiscreteAmplitudeDiscreteReal S10{ "S10.sgn",param.getOutputFolderName() };

	PhotonStreamXY Q_C_4{ "Q_C_4.sgn",param.getOutputFolderName() };

	Messages C_C_2{ "C_C_2.sgn",param.getOutputFolderName() };
//	C_C_2.setSaveSignal(false);

	Messages C_C_1{ "C_C_1.sgn",param.getOutputFolderName() };
//	C_C_1.setSaveSignal(false);

	TimeDiscreteAmplitudeDiscreteReal SSPR{ "SSPR.sgn",param.getOutputFolderName() };

	Binary MI{ "BER.sgn",param.getOutputFolderName() };



	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ##################################
	// #####################################################################################################

	// ################################################################################
	// ############################################################Alice implementation
	// ################################################################################
	BinarySource B0{ vector <Signal*> {}, vector <Signal*> {&NUM_A, &Bin_1, &Bin_2} };
	B0.setBitPeriod(1 / (2 *param.rateOfPhotons));
	B0.setNumberOfBits(10000);
	B0.setMode(PseudoRandom);



	Clock B1{ vector<Signal*>{}, vector<Signal*>{&CLK_A} };
	B1.setClockPeriod(1 /param.rateOfPhotons);
	B1.setSamplingPeriod((1 /param.rateOfPhotons) / numberOfSamplesPerSylbom);
	B1.setClockPhase(3 * PI / 2);


	AliceQKD B2{ vector <Signal*> { &CLK_A, &NUM_A, &Bin_1, &Bin_2, &C_C_2}, vector <Signal*> {&CLK_A_1, &S3, &MI_A, &C_C_1} };
	B2.setIqAmplitudes(param.iqAmplitudeValues);
	B2.setNumberOfSamplesPerSymbol(numberOfSamplesPerSylbom);
	B2.setPulseShaperFilter(Square);
	B2.setSymbolPeriodMIA(1 /param.rateOfPhotons);
	B2.setSaveInternalSignals(false);

	SinglePhotonSource B3{ vector <Signal*> {&CLK_A_1}, vector <Signal*> {&S2} };

	PolarizationRotator B4{ vector<Signal*>{&S2,&S3}, vector <Signal*> {&Q_C_1} };

	/*Quantum channel*/

	Clock B14{ vector<Signal*>{}, vector<Signal*>{&clk_sop} };
	B14.setClockPeriod(1 /param.rateOfPhotons);
	B14.setSamplingPeriod((1 /param.rateOfPhotons) / numberOfSamplesPerSylbom);

	SOPModulator B15{ vector<Signal*>{&clk_sop}, vector<Signal*>{&theta, &phi} };
	B15.setElevationAngle(param.elevationAngle);
	B15.setRotationAngle(param.rotationAngle);

	PolarizationRotator B16{ vector<Signal*>{&Q_C_1,&theta,&phi}, vector<Signal*>{&Q_C_2} };

	OpticalAttenuator B17{ vector<Signal*>{&Q_C_2}, vector<Signal*>{&Q_C_3} };
	B17.setAttenuationCoef(param.alpha_dB);

	// ################################################################################
	//###############################################################Bob Implementation
	// ################################################################################
	BinarySource B5{ vector <Signal*> {}, vector <Signal*> {&NUM_B, &Bin_6} };
	B5.setBitPeriod(1 / (param.rateOfPhotons));
	B5.setMode(PseudoRandom);

	Clock B6{ vector <Signal*> {}, vector <Signal*> {&CLK_B} };
	B6.setClockPeriod(1 /param.rateOfPhotons);
	B6.setSamplingPeriod((1 /param.rateOfPhotons) / numberOfSamplesPerSylbom);

	BobQKD B7{ vector<Signal*>{&NUM_B, &CLK_B, &SSPR, &Bin_6, &C_C_1}, vector<Signal*>{&S4, &CLK_B_out1, &CLK_B_out2, &C_C_2, &MI_B} };
	B7.setPulseShaperFilter(Square);
	B7.setNumberOfSamplesPerSymbol(numberOfSamplesPerSylbom);
	B7.setPulseWidth(param.detectorWindowTimeOpen);
	B7.setPulseDelay(param.detectorPhaseDelay);
	B7.setSaveInternalSignals(false);


	PolarizationRotator B10(vector<Signal*>{&Q_C_3, &S4}, vector<Signal*>{&S5});

	PolarizationBeamSplitter B8(vector<Signal*>{&S5}, vector<Signal*>{&S6});

	SinglePhotonDetector B11(vector<Signal*>{&S6, &CLK_B_out1}, vector<Signal*>{&S7});
	B11.setPath(0);
	B11.setProbabilityDarkCount(param.detectorProbabilityDarkCount);

	SinglePhotonDetector B9(vector<Signal*>{&S6, &CLK_B_out2}, vector<Signal*>{&S9});
	B9.setPath(1);
	B9.setProbabilityDarkCount(param.detectorProbabilityDarkCount);

	SinglePhotonReceiver B12(vector<Signal*>{&S7, &S9}, vector<Signal*>{&SSPR});
	B12.setClockBitPeriod(1 /param.rateOfPhotons);
	B12.setClockSamplingPeriod(1 /param.rateOfPhotons / numberOfSamplesPerSylbom);
	B12.setClockPhase(3 * PI / 2);
	B12.setPulseShaperFilter(Square);
	B12.setPulseShaperWidth(0.9);
	B12.setThreshold(0.5);
	B12.setSaveInternalSignals(false);

	BitErrorRate B13(vector<Signal*>{&MI_B, &MI_A}, vector<Signal*>{&MI});
	
	Sink aux(vector<Signal*>{&MI}, vector <Signal*>{});
	aux.setDisplayNumberOfSamples(true);

	// ####################################################################################################
	// ########################### System Declaration and Inicialization ##################################
	// #####################################################################################################

	System MainSystem{ vector <Block*> {&B0, &B1, &B2, &B3, &B4, &B14, &B15, &B16, &B17, &B5, &B6, &B10, &B8, &B11, &B9, &B12, &B7, &B13, &aux},
					   param.getOutputFolderName(),
					   param.getLoadedInputParameters() };

	// ####################################################################################################
	// ########################### Run ##################################
	// #####################################################################################################

	MainSystem.run();


	return 0;
}