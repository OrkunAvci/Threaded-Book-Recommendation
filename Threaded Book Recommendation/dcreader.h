#ifndef __DCREADER_H__
#define __DCREADER_H__

#include "datacontainer.h"
#include <stdio.h>
#include <stdlib.h>

DC* readDC(char*);
void terminateDC(DC*);
void output_dc(DC*);

#endif /*__DCREADER_H__*/
