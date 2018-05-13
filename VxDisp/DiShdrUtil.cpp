#include "VxDispCm.h"
#include "DiShdr.h"
#include "DiShdrUtil.h"

void diShdrActive(VeShdrType shdrType)
{
    VgDiShdr.active(shdrType);
}

void diShdrBindVBO()
{
    VgDiShdr.bindVBO();
}

void diShdrSetSurfAt(DiSurfAt& surfAt)
{
    VgDiShdr.setSurfAt(surfAt);
}