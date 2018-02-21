#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED

#include "includes.h"
using namespace std;

class Element {
protected:
    int id; //Try substitute id_element
	string id_element;									// id of the element (must be unique !)
	bool isSubNet;
	bool isFD;                                     // is the element a subnetworks (if not, it is a
														// calculator)
    //static int counter_elements;
public:
	// ctor
	Element(const string& id_, bool type, bool fd=false);
	Element(int idn, const string& id_, bool type, bool fd=false);
	~Element();

	// get/set
	int getIdNUMElem() const { /*cout<<"ID Elément = "<<id<<" et ID_string = "<<id_element<<endl;*/ return id; }
	void setIdNum( int id_) { id = id_; }
	string getIdElem() const { return id_element; }
	void setIdElem(const string& id_) { id_element = id_; }
	bool getIsSubNet() const { return isSubNet; }
	void setIsSubNet(bool isSubNet_) { isSubNet = isSubNet_;}
	bool getIsFD() const {return isFD;}
};


/***********
 * numObj(vector<T>* vecObj, string id)
 *
 * Return the position in vecObj of the object whose id is id. vecObj should contain unique id objects. If
 * the id is not found, returns -1.
 *
 * INPUT	vector<Template>* vecObj: the vector of object of class T in which looking for
 * 			string id: the id to be found
 * OUTPUT	A position (int)
 ***********/
int numObj(vector<Element>* vecObj, string id);

#endif // ELEMENT_H_INCLUDED
