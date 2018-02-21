#include "PathFrame.h"



// constructor
PathFrame::PathFrame(Frame* frame_, Calc* source_, Calc* target_):
	frame(frame_),
	calc_source(source_),
	calc_target(target_),
	length(0),
	elementList(vector<Element*>())
{
}


PathFrame::~PathFrame() {
	// TODO
}

// get set


// methods
bool PathFrame::isPathValid() const {

	bool isValid = false;

	if (length > 1) {
		isValid = true;
		// Is the first element calc_source ?
		if (calc_source->getIdElem().compare( elementList.front()->getIdElem() ) != 0) isValid = false;
		// Is the last element calc_target ?
		if (calc_target->getIdElem().compare( elementList.back()->getIdElem() ) != 0) isValid = false;

		vector<string> listIdCalc;
		vector<string> listIdSN;

		for (int ielt=0;ielt<length;++ielt) {
			if (ielt%2 == 0) {
				// it should be a calculator
				if (elementList[ielt]->getIsSubNet() != 0) isValid = false;
				// Does the calculator appear twice ?
				if (listIdCalc.size() > 0) {
					if (find(listIdCalc.begin(), listIdCalc.end(), elementList[ielt]->getIdElem())
						== listIdCalc.end()) {
						isValid = false;
					} else {
						listIdCalc.push_back(elementList[ielt]->getIdElem());
					}
				}
			}
			if (ielt%2 == 1) {
				// it should be a sub-network
				if (elementList[ielt]->getIsSubNet() != 1) isValid = false;
				// Does the calculator appear twice ?
				if (listIdSN.size() > 0) {
					if (find(listIdSN.begin(), listIdSN.end(), elementList[ielt]->getIdElem())
						== listIdSN.end()) {
						isValid = false;
					} else {
						listIdSN.push_back(elementList[ielt]->getIdElem());
					}
				}
			}
		}
	}

	return isValid;;
}


bool PathFrame::addElement(Element* element_) {
	elementList.push_back(element_);
	length++;
	return true;
}


bool PathFrame::remElement() {	// remove the last element
	if (length<=0) return false;
	else {
		elementList.pop_back();
		length--;
		return true;
	}
}



bool PathFrame::findShortestPath(vector<Element*>* elems_) const {
	queue< vector<Element*> > pathqueue;		// queue that will contain the paths to explore further
	vector<Element*> alreadyVisited;

	pathqueue.push( vector<Element*> (1, (Element*) calc_source) ); //1st to explore: source


	//cout << "frame: "<< frame->getIdFrame() << endl <<
      //      "source: "<< calc_source->getIdCalc() << endl <<
        //    "target: " << calc_target->getIdCalc() << endl;

	while (!pathqueue.empty()) {

		vector<Element*> curPath = pathqueue.front();
		pathqueue.pop();

        //for (vector<Element*>::iterator it = curPath.begin();it != curPath.end(); it++)
        //    cout << (*it)->getIdElem() << endl;
      //  getchar();

		// curPath should end with a Calc*
		Calc* lastCalc;
		if (!curPath.back()->getIsSubNet()) {
			lastCalc = (Calc*) curPath.back();
		} else {
			cerr << "Problem encountered while finding a shortest path." << endl;
			lastCalc = NULL;
		}

		for (int iconsn = 0;
			(iconsn < lastCalc->getNbSNConnexion()) && ((int)curPath.size() <= 2*path_order_max-1);
			++iconsn) 																		{

			SubNet* nextSN = lastCalc->getConnectedSN()[iconsn];

			if (( find(alreadyVisited.begin(), alreadyVisited.end(),
               (Element*) nextSN) == alreadyVisited.end()	) &&
			   (numObj(frame->getSNFilled(), nextSN->getIdElem()) >= 0)	) {

				// This is a new Element and is allowed.
				alreadyVisited.push_back((Element*) nextSN);

				for (int inc = 0; inc < nextSN->getNbCalc(); ++inc) {
					Calc* nextCalc = nextSN->getCalcList()[inc];

					if (find(alreadyVisited.begin(), alreadyVisited.end(), (Element*) nextCalc)
																		== alreadyVisited.end()	) {
						// This is a new Element
						alreadyVisited.push_back((Element*) nextCalc);

						vector<Element*> newPath = curPath;
						newPath.push_back((Element*) nextSN);
						newPath.push_back((Element*) nextCalc);

						// Is the end reached ?
						if (nextCalc == calc_target) {	// A correct path has been found
							*elems_ = newPath;
							return true;
						} else if ((int)newPath.size() <= 2*path_order_max-1) {
							// if no, add the path to the queue of paths to be explored
							// but only if its size will allow to add to extra elements.
							pathqueue.push(newPath);
						}
					} // if the element has already been visited, nothing to do
				}
			} // if the element has already been visited, nothing to do
		}
	} // if it reaches the end of the loop, no path from calc_source to calc_target has been found

	return false;
}


bool PathFrame::generatePath() {
	//bool success = false;
	elementList.clear();

	vector<Element*> elems;
	bool success = findShortestPath(&elems);

	if (success) {
		elementList = elems;
		length = elems.size();
	}

	if(!this->isPathValid()) {
		success = false;
		elementList.clear();
		length = 0;
	} else {
		vector<SubNet*> snfilled = frame->getSNFilled();	// add to the SN filled for other paths of the
															// same frame
		for (int ielt = 1; ielt < length; ielt+=2) {
			if (numObj(snfilled, elementList[ielt]->getIdElem()) < 0) {
				snfilled.push_back((SubNet*) elementList[ielt]);
			}
		}
		frame->setSNFilled(snfilled);
	}

	return success;
}


string PathFrame::toString(bool toPrint) const {
	ostringstream str;
	str << "---------- PathFrame for Frame " << frame->getIdFrame() << " ----------" << endl;
	str << "Source: " << calc_source->getIdElem() << endl;
	str << "Target: " << calc_target->getIdElem() << endl;
	str << "Length of the path: " << length << endl;
	for (int ielt=0;ielt<length;++ielt) {
		str << "\t" << ielt << "\t" << this->getElementList()[ielt]->getIdElem() << endl;
	}
	str << "-------------------" << endl;

	if (toPrint){
		cout << str.str();
	}

	return str.str();
}


bool compareLoads(PathFrame* p1, PathFrame* p2) {
	return (p1->getFrame()->getLoad() < p2->getFrame()->getLoad());
}


