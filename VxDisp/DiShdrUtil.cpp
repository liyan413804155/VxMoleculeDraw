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

void diShdrSetSurfAt(const DiSurfAt& surfAt)
{
    VgDiShdr.setSurfAt(surfAt);
}

void diShdrSetProjMat(const QMatrix4x4& mat)
{
    VgDiShdr.setProjMat(mat);
}

void diShdrSetViewMat(const QMatrix4x4& mat)
{
    VgDiShdr.setViewMat(mat);
}