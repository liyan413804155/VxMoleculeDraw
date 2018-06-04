#ifndef OM_BIN_H
#define OM_BIN_H

class DbBase;

class VXDB_EXPORT OmBin
{
public:
    OmBin();
    ~OmBin();

public:
    int allocId();
    void freeId(int id);
    void logonObj(DbBase * pDbObj);
    void logoffObj(DbBase * pDbObj);
    DbBase* getDbById(int id);

private:
    friend class OmBinImpl;
    OmBinImpl *d;
};


#endif 