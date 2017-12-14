
#include "netxpto.h"
#include <stdio.h>

#include "binary_source.h"
#include "clock.h"
#include "alice_qkd.h"
#include "single_photon_source.h"
#include "polarizer.h"
#include "sink.h"
#include "bob_qkd.h"
#include "rotator_linear_polarizer.h"
#include "polarization_beam_splitter.h"
#include "single_photon_detector.h"
#include "optical_switch.h"
#include "eve_qkd.h"

int main()
{
	// #####################################################################################################
	// ################################# System Input Parameters #########################################
	// #####################################################################################################

	double RateOfPhotons{ 1e3 };
	vector<t_iqValues> iqAmplitudeValues = { {-45.0,0.0},{0.0,0.0},{45.0,0.0},{90.0,0.0} };

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	// Alice Implementation
	Binary NUM_A{ "NUM_A.sgn" };
	Binary MI_A{ "MI_A.sgn" };
	TimeContinuousAmplitudeContinuous CLK_A{"CLK_A.sgn"};
	TimeContinuousAmplitudeContinuous CLK_A_1{ "CLKA_out.sgn" };
	PhotonStream S2 { };
	TimeContinuousAmplitudeDiscreteReal S3{ "S3.sgn" };
	Messages C_C_1{ "C_C_1.sgn" };
	Messages C_C_6{ "C_C_1.sgn" };
	PhotonStream Q_C_1{ };

	// Bob Implementation

	Binary NUM_B{ "NUM_B.sgn" };
	Binary MI_B{ "MI_B.sgn" };
	TimeContinuousAmplitudeContinuous CLK_B{ };
	TimeContinuousAmplitudeContinuousReal SB_1{ "SB_1.sgn" };
	TimeContinuousAmplitudeContinuousReal SB_2{ "SB_2.sgn" };
	Binary SB_3{ "SB_3.sgn" };
	PhotonStream Q_C_4{};
	PhotonStream SB_4{};
	PhotonStream SB_5{};
	PhotonStream SB_6{};
	Messages C_C_4{ "C_C_4.sgn" };
	Messages C_C_3{ "C_C_3.sgn" };

	// Eve Implementation
	Binary NUM_E{ "NUM_E.sgn" };
	Binary MI_E{ "MI_E.sgn" };
	TimeContinuousAmplitudeContinuous CLK_E{ };
	Messages C_C_5{ "C_C_5.sgn" };
	Messages C_C_2{ "C_C_2.sgn" };
	Binary BS_2{ "BS_2.sgn" };
	Binary BS_3{ "BS_3.sgn" };
	TimeDiscreteAmplitudeDiscreteReal Sr_1{ "Sr_1.sgn" };
	TimeContinuousAmplitudeContinuousReal Sr_2{ "Sr_2.sgn" };
	TimeContinuousAmplitudeContinuousReal Sr_3{ "Sr_3.sgn" };
	PhotonStream SE_2{};
	PhotonStream SE_3{};
	PhotonStream SE_4{};
	PhotonStream SE_5{};
	PhotonStream Q_C_2{};
	TimeDiscreteAmplitudeDiscreteReal St_2{ "St_2.sgn" };
	TimeDiscreteAmplitudeDiscreteReal St_1{ "St_1.sgn" };
	PhotonStream SE_6{};
	PhotonStream SE_7{};
	PhotonStream SE_8{};
	PhotonStream Q_C_3{};
	
	
	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ##################################
	// #####################################################################################################

	// Alice implementation
	BinarySource B0{ vector <Signal*> {}, vector <Signal*> {&NUM_A} };
	B0.setBitPeriod(1/(2 * RateOfPhotons));
	B0.setMode( PseudoRandom );

	Clock B1{ vector<Signal*>{}, vector<Signal*>{&CLK_A} };
	B1.setClockPeriod(1 / RateOfPhotons);
	B1.setSamplingPeriod((1 / RateOfPhotons) / 16);
	B1.setClockPhase(PI);

	AliceQKD SB1{ vector <Signal*> { &CLK_A, &NUM_A}, vector <Signal*> {&CLK_A_1, &S3} };
	//SB1.setM(4);
	SB1.setIqAmplitudes(iqAmplitudeValues);
	//SB1.setNumberOfSamplesPerSymbol(1);
	SB1.setPulseShaperFilter(Square);
	

	Sink BB1{ vector <Signal*> {&S3}, vector <Signal*> {} };
	BB1.setNumberOfSamples(1000);
	BB1.setDisplayNumberOfSamples(true);

	Sink BB2{ vector <Signal*> {&CLK_A_1}, vector <Signal*> {} };
	BB2.setNumberOfSamples(1000);


	// ################################################################################

	/*
	
	SinglePhotonSource B4{ vector <Signal*> {&S_A1}, vector <Signal*> {&S_A3} };
	
	Polarizer B5{ vector <Signal*> {&S_A3, &S_A2}, vector <Signal*> {&Q_C_1} };

	Sink B6{ vector <Signal*> {&Q_C_1}, vector <Signal*> {} };

	Sink B7{ vector <Signal*> {&C_C_1}, vector <Signal*> {} };

	Sink B8{ vector <Signal*> {&MI_A}, vector <Signal*> {} };*/

	//Bob Implementation
	/*BinarySource B9{ vector <Signal*> {}, vector <Signal*> {&NUM_B} };
	B9.setMode(PseudoRandom);

	Clock B10{ vector <Signal*> {}, vector <Signal*> {&CLK_B} };

	RotatorLinearPolarizer B11{ vector <Signal*> {&Q_C_4, &SB_3}, vector <Signal*> {&SB_5} };

	PolarizationBeamSplitter B12{ vector <Signal*> {&SB_5}, vector <Signal*> {&SB_4, &SB_6} };

	SinglePhotonDetector B13{ vector <Signal*> {&SB_4}, vector <Signal*> {&SB_1} };

	SinglePhotonDetector B14{ vector <Signal*> {&SB_6}, vector <Signal*> {&SB_2} };

	BobQKD B15{ vector <Signal*> {&NUM_B, &CLK_B, &C_C_3, &SB_1, &SB_2}, vector <Signal*> {&MI_B, &SB_3, &C_C_4} };

	Sink B16{ vector <Signal*> {&C_C_4}, vector <Signal*> {} };

	Sink B17{ vector <Signal*> {&MI_B}, vector <Signal*> {} };*/

	//Eve Implementation
	/*BinarySource B18{ vector <Signal*> {}, vector <Signal*> {&NUM_E} };
	B18.setMode(PseudoRandom);

	Clock B19{ vector <Signal*> {}, vector <Signal*> {&CLK_E} };

	OpticalSwitch B20{ vector <Signal*> {&Q_C_2, &BS_3}, vector <Signal*> {&SE_8, &SE_2} };

	RotatorLinearPolarizer B21{ vector <Signal*> {&SE_2, &Sr_1}, vector <Signal*> {&SE_3} };

	PolarizationBeamSplitter B22{ vector <Signal*> {&SE_3}, vector <Signal*> {&SE_4, &SE_5} };

	SinglePhotonDetector B23{ vector <Signal*> {&SE_4}, vector <Signal*> {&Sr_3} };

	SinglePhotonDetector B24{ vector <Signal*> {&SE_5}, vector <Signal*> {&Sr_2} };

	EveQKD B25{ vector <Signal*> {&NUM_E, &CLK_E, &C_C_5, &C_C_2, &Sr_2, &Sr_3}, vector <Signal*> {&MI_E, &BS_3, &BS_2, &Sr_1, &St_2, &St_1} };

	SinglePhotonSource B26{ vector <Signal*> {&St_2}, vector <Signal*> {&SE_6} };

	Polarizer B27{ vector <Signal*> {&SE_6, &St_1}, vector <Signal*> {&SE_7} };

	OpticalSwitch B28{ vector <Signal*> {&SE_8, &SE_7, &BS_2}, vector <Signal*> {&Q_C_3} };

	Sink B29{ vector <Signal*> {&Q_C_3}, vector <Signal*> {} };

	Sink B30{ vector <Signal*> {&MI_E}, vector <Signal*> {} };*/

	// ####################################################################################################
	// ########################### System Declaration and Inicialization ##################################
	// #####################################################################################################

	//System MainSystem{ vector <Block*> {&B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11, &B12, &B13, &B14, &B15, &B16, &B17, &B18, &B19, &B20, &B21, &B22, &B23, &B22, &B23, &B24, &B25, &B26, &B27, &B28, &B29, &B30} };

	System MainSystem{ vector <Block*> {&B0, &B6, &SB1, &BB1, &BB2} };
	// ####################################################################################################
	// ########################### Run ##################################
	// #####################################################################################################

	MainSystem.run();


    return 0;
}

