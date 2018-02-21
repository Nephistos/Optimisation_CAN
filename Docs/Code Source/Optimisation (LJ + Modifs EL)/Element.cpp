#include "Element.h"



//builder
Element::Element(const string& id_, bool type, bool fd):
	id_element(id_),
	isSubNet(type),
	isFD(fd)
{
}

Element::Element(int idn, const string& id_, bool type, bool fd):
    id(idn),
	id_element(id_),
	isSubNet(type),
	isFD(fd)
{
   // cout << id << " " << id_element << endl;
}


Element::~Element(){
}


int numObj(vector<Element>* vecObj, string id) {
	int num = -1;
	for (int ii=0; ii<(int)(*vecObj).size(); ++ii) {
		if ((*vecObj)[ii].getIdElem().compare(id) == 0) num = ii;
	}
	if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}
