#ifndef DI_SHDRUTIL_H
#define DI_SHDRUTIL_H

VXDISP_EXPORT void diShdrActive(VeShdrType shdrType);
VXDISP_EXPORT void diShdrBindVBO();
VXDISP_EXPORT void diShdrSetSurfAt(const DiSurfAt& surfAt);
VXDISP_EXPORT void diShdrSetProjMat(const QMatrix4x4& mat);
VXDISP_EXPORT void diShdrSetViewMat(const QMatrix4x4& mat);

#endif
