#ifndef PATHFRAME_H_INCLUDED
#define PATHFRAME_H_INCLUDED

#include "includes.h"
#include <stack>
#include <queue>

#include "Calc.h"
#include "SubNet.h"
#include "Frame.h"

#include "const_tool.h"



class PathFrame {
private:
	Frame* frame;									// Frame that use this path
	Calc* calc_source;								// source of the frame/of the path
	Calc* calc_target;								// target of the frame
	int length;										// number of Element-s used by the path
	vector<Element*> elementList;					// pointers to the Element-s used by the path


public:
	// constructor
	PathFrame(Frame* frame_, Calc* source_, Calc* target_);
	~PathFrame();

	// get set
	Frame* getFrame() const { return frame; }
	void setFrame(Frame* frame_) { frame = frame_; }
	Calc* getSource() const { return calc_source; }
	void setSource(Calc* source_) { calc_source = source_; }
	Calc* getTarget() const { return calc_target; }
	void setTarget(Calc* target_) { calc_target = target_; }
	int getLength() const { return length; }
	void setLength(int length_) { length = length_; }
	vector<Element*> getElementList() const { return elementList; }
	void setElementList(vector<Element*> elt_) { elementList = elt_; }

	// methods
	/***********
	 * isPathValid()
	 *
	 * Test if the path is valid regarding to the alternance of Calc/SubNet, the connexion between
	 * following elements, the unicity of elements, the source and target within the path. Return a bool.
	 *
	 * OUTPUT	A bool.
	 ***********/
	bool isPathValid() const;

	bool addElement(Element* element_);
	bool remElement();	// remove the last element



	/***********
	 * findShortestPath(vector<Element*>* elems_)
	 *
	 * Try to find the shortest sequence of Element-s that connect the PathFrame::calc_source of the
	 * present object and its PathFrame::calc_target. A breadth-first search in a tree algorithm is used.
	 * The methods returns if it succeeded and store the result (if any) using the pointer passed in
	 * parameter.
	 * If at least one path exists, the algorithm will find one of the shortest paths.
	 *
	 * MODIFY	vector<Element*>* elems_
	 *				refers to the vector that will contain the elements that form the shortest path
	 *
	 * OUTPUT	A bool that qualifies if at least one path has been found
	 ***********/
	bool findShortestPath(vector<Element*>* elems_) const;

	/***********
	 * generatePath()
	 *
	 * Try to connect the calculators calc_source and calc_target (given the network topology) and return
	 * if it succeeded.
	 *
	 * OUTPUT	A bool that qualifies if at least one path has been found
	 ***********/
	bool generatePath();


	string toString(bool toPrint=true) const;
};

bool compareLoads(PathFrame* p1, PathFrame* p2);

#endif // PATHFRAME_H_INCLUDED
