#ifndef DI_SHDR_H
#define DI_SHDR_H

#include "DiShdrCm.h"

class DiShdr
{
public:
    DiShdr();
    ~DiShdr();

    void active(VeShdrType shdrType);
    void bindVBO();
    void setSurfAt(DiSurfAt& surfAt);

private:
    friend class DiShdrImpl;
    DiShdrImpl *d;
};

extern DiShdr VgDiShdr;

#endif

