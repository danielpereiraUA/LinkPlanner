# ifndef PROGRAM_INCLUDE_SUBTRACTOR_H_
# define PROGRAM_INCLUDE_SUBTRACTOR_H_

# include "netxpto.h"

// Simulates a subtractor
class subtractor : public Block {

	bool firstTime{ true };

public:

	double outputOpticalPower{ 1e-3 };
	double outputOpticalWavelength{ 1550e-9 };
	double outputOpticalFrequency{ SPEED_OF_LIGHT / outputOpticalWavelength };

	subtractor(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:
};


#endif // !PROGRAM_INCLUDE_SUBTRACTOR_H_
