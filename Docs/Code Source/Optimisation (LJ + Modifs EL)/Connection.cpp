#include "Connection.h"



Connection::Connection(Calc* conCalc_, SubNet* conSN_) :
	conCalc(conCalc_),
	conSN(conSN_),
	isFixed(false)
{
	conSN_->addCalc(conCalc_);
}


Connection::Connection(Calc* conCalc_, SubNet* conSN_, const bool isFixed_) :
	conCalc(conCalc_),
	conSN(conSN_),
	isFixed(isFixed_)
{
	conSN_->addCalc(conCalc_);
}
