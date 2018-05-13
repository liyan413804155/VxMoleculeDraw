#ifndef DI_SHDRUTIL_H
#define DI_SHDRUTIL_H

#include "VxDispCm.h"
#include "DiShdrCm.h"

VXDISP_EXPORT void diShdrActive(VeShdrType shdrType);
VXDISP_EXPORT void diShdrBindVBO();
VXDISP_EXPORT void diShdrSetSurfAt(DiSurfAt& surfAt);

#endif
