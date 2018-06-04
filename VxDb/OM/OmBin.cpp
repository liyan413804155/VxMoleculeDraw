#include "VxDbCm.h"
#include "DB/DbBase.h"
#include "OM/OmBin.h"

#define STORE_UNIT_SIZE 1024

class OmStore
{
public:
    OmStore()
    {
        m_blockCapacity = STORE_UNIT_SIZE;
        m_blockList = (DbBase***)malloc(sizeof(DbBase**) * m_blockCapacity);
        memset(m_blockList, 0, sizeof(DbBase**) * m_blockCapacity);

        m_blockList[0] = (DbBase**)malloc(sizeof(DbBase*) * STORE_UNIT_SIZE);
        memset(m_blockList[0], 0, sizeof(DbBase*) * STORE_UNIT_SIZE);

        m_LastId = 0;
    }
    ~OmStore()
    {
        for (int i = 0; i < m_LastId - 1; i++)
        {
            int blockIndex = i / STORE_UNIT_SIZE;
            int innerIndex = i % STORE_UNIT_SIZE;

            if (m_blockList[blockIndex][innerIndex])
            {
                delete m_blockList[blockIndex][innerIndex];
            }
        }

        for (int i = 0; i < m_blockCapacity; i++)
        {
            if (m_blockList[i])
                free(m_blockList[i]);
        }

        free(m_blockList);
    }

public:

    int allocId()
    {
        if (m_reuseId.size() > 0)
        {
            int id = m_reuseId.front();
            m_reuseId.pop_front();
            return id;
        }

        return m_LastId++;
    }

    void freeId(int id)
    {
        assert(id < m_LastId);

        if (id == m_LastId - 1)
        {
            m_LastId--;
        }
        else
        {
            m_reuseId.push_back(id);
        }
    }

    void logonObj(DbBase * pDbObj)
    {
        int id = pDbObj->getId();
        int blockIndex = id / STORE_UNIT_SIZE;
        int innerIndex = id % STORE_UNIT_SIZE;

        if (blockIndex > m_blockCapacity - 1)
        {
            int inc = ((blockIndex - (m_blockCapacity - 1)) / STORE_UNIT_SIZE + 1) * STORE_UNIT_SIZE;

            m_blockList = (DbBase***)realloc(m_blockList, (m_blockCapacity + inc) * sizeof(DbBase**));
            memset(m_blockList + m_blockCapacity, 0, inc * sizeof(DbBase**));
        }

        if (!m_blockList[blockIndex])
        {
            m_blockList[blockIndex] = (DbBase**)malloc(sizeof(DbBase*) * STORE_UNIT_SIZE);
        }

        m_blockList[blockIndex][innerIndex] = pDbObj;
    }

    void logoffObj(DbBase * pDbObj)
    {
        int id = pDbObj->getId();

        int blockIndex = id / STORE_UNIT_SIZE;
        int innerIndex = id % STORE_UNIT_SIZE;

        assert(m_blockList[blockIndex]);

        m_blockList[blockIndex][innerIndex] = nullptr;
    }

    DbBase* getObjById(int id)
    {
        if (id < 0)
            return nullptr;

        int blockIndex = id / STORE_UNIT_SIZE;
        int innerIndex = id % STORE_UNIT_SIZE;

        if (blockIndex > m_blockCapacity - 1)
            return nullptr;

        if (!m_blockList[blockIndex])
            return nullptr;

        return m_blockList[blockIndex][innerIndex];
    }

private:
    DbBase***   m_blockList;
    int         m_blockCapacity;
    QQueue<int> m_reuseId;
    int         m_LastId;
};

class OmBinImpl
{
public:
    OmStore m_store;
};

OmBin::OmBin()
{
    d = new OmBinImpl;
}

OmBin::~OmBin()
{
    delete d;
}

int OmBin::allocId()
{
    return d->m_store.allocId();
}

void OmBin::freeId(int id)
{
    d->m_store.freeId(id);
}

void OmBin::logonObj(DbBase * pDbObj)
{
    d->m_store.logonObj(pDbObj);
}

void OmBin::logoffObj(DbBase * pDbObj)
{
    d->m_store.logoffObj(pDbObj);
}

DbBase* OmBin::getDbById(int id)
{
    return d->m_store.getObjById(id);
}