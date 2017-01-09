#include <iostream>
#include "boardrep.hxx"
#include "eval.hxx"

using namespace std;

Position kvw25Bd4 = {	// Position after 25... Bd4
	{
		0x001a0e2018000000,
		0x00320a6018806000,
		0x0008044008000040,
		0X0028000010000060
	},
	0x8000,
	50,
	3
};

Position kvw3Bb5 = { 	// Game position of Garry Kasparov vs. The World at 3.Bb5+
	{
		0xfff3080400000000,
		0x2cf308061000ef0c,
		0x7600000200200016,
		0x9900000000000099
	},
	0x1780,
	5,
	5
};

Position kvw6Nf6 = {	// Position after 6... Nf6
	{
		0xb1fb2c0400000000,
		0x20fb08061400eb0c,
		0x3000240200240014,
		0x9108000000000099
	},
	0x8780,
	14,
	0
};

int main() {
	cout<< "Evaluation of 25... Bd4"<< endl;
	cout<< Evaluate( kvw25Bd4 )<< endl;
	cout<< endl<< endl<< "Evaluation of 3. Bb5+"<< endl;
	cout<< Evaluate( kvw3Bb5 )<< endl;
	cout<< endl<< endl<< "Evaluation of 6... Nf6"<< endl;
	cout<< Evaluate( kvw6Nf6 )<< endl;
	return 0;
}
