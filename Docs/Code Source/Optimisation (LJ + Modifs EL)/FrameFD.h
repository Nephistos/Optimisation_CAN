#ifndef FRAMEFD_H_INCLUDED
#define FRAMEFD_H_INCLUDED

#include "Frame.h"
#include "FrameHS.h"

class FrameFD:public Frame{
private:
    vector<FrameHS *> frame_included;

};
#endif // FRAMEFD_H_INCLUDED
