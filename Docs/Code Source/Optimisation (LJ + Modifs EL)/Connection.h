#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include "includes.h"

#include "Calc.h"
#include "SubNet.h"


class Connection {
private:
	Calc* conCalc;
	SubNet* conSN;
	bool isFixed;

public:
	// constructors
	Connection(Calc* conCalc_, SubNet* conSN_);
	Connection(Calc* conCalc_, SubNet* conSN_, const bool isFixed_);
	~Connection();

	// get / set
	bool getisFixed() const { return isFixed; }
	Calc* getConCalc() const { return conCalc; }
	SubNet* getConSN() const { return conSN; }
};

#endif // CONNECTION_H_INCLUDED
