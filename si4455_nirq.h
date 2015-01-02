/*!
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 *
 * Public API for the Si4455 nIRQ interface.
 */

#ifndef _SI4455_NIRQ_H
#define _SI4455_NIRQ_H

class tSi4455_nIRQ
{	
public:
	void ProcessIRQ();
};

extern tSi4455_nIRQ Si4455_nIRQ;

#endif //_SI4455_NIRQ_H
