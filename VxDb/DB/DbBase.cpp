#include "VxDbCm.h"
#include "DB/DbBase.h"
#include "OM/OmBin.h"

class DbBaseImpl
{
public:
    DbHead m_head;
    OmBin  *m_bin;
};

DbBase::DbBase(VeDbType type, int version, OmBin *pBin)
{
    d = new DbBaseImpl;
    d->m_head.type = type;
    d->m_head.version = version;
    d->m_bin = pBin;

    d->m_head.index = pBin->allocId();
    d->m_bin->logonObj(this);

    d->m_head.idx_next = d->m_head.idx_prev = d->m_head.index;
}

DbBase::~DbBase()
{
    d->m_bin->freeId(d->m_head.index);
    d->m_bin->logoffObj(this);
    delete d;
}

int DbBase::getId() const
{
    return d->m_head.index;
}

DbBase* DbBase::getNext()
{
    return d->m_bin->getDbById(d->m_head.idx_next);
}

DbBase* DbBase::getPrev()
{
    return d->m_bin->getDbById(d->m_head.idx_prev);
}

void DbBase::insertBack(DbBase* pObj)
{
    DbBase* pNext = pObj->getNext();
    DbBase* pPrev = pObj->getPrev();
    DbBase* pSelfNext = getNext();

    pNext->d->m_head.idx_prev = pPrev->d->m_head.index;
    pPrev->d->m_head.idx_next = pNext->d->m_head.index;

    d->m_head.idx_next = pObj->d->m_head.index;
    pSelfNext->d->m_head.idx_prev = pObj->d->m_head.index;

    pObj->d->m_head.idx_prev = d->m_head.index;
    pObj->d->m_head.idx_next = pSelfNext->d->m_head.index;
}

void DbBase::insertFront(DbBase* pObj)
{
    getPrev()->insertBack(pObj);
}