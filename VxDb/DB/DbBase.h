#ifndef DB_BASE_H
#define DB_BASE_H

enum VeDbType
{
    VeDbGrp,
    VeDbDrawing,
    VeDbAtom,
    VeDbBone,
};

struct DbHead 
{
    int      index;
    VeDbType type;
    int      version;
    int      idx_next;
    int      idx_prev;
};

struct DbColor
{
    float r, g, b;
};

struct DbPnt
{
    float x, y, z;
};

struct DbDir
{

};

struct DbFaceAt
{
    DbColor front_color;
    DbColor back_color;
    float diffuse;
    float specular;
    float shininess;
};

class OmBin;

class VXDB_EXPORT DbBase
{
public:
    DbBase(VeDbType type, int version, OmBin *pBin);
    ~DbBase();

public:
    int getId() const;
    DbBase* getNext();
    DbBase* getPrev();

public:
    void insertBack(DbBase* pObj);
    void insertFront(DbBase* pObj);

private:
    friend class DbBaseImpl;
    DbBaseImpl *d;
};

#endif 
