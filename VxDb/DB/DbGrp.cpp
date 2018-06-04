#include "VxDbCm.h"
#include "DB/DbBase.h"
#include "Db/DbGrp.h"

#define DbGrpVersion    0

DbGrp::DbGrp(OmBin *pBin)
    : DbBase(VeDbGrp, DbGrpVersion, pBin)
{

}

