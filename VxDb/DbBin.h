#ifndef DB_BIN_H
#define DB_BIN_H

class VXDB_EXPORT DbBin
{
public:
    DbBin(const QString& dbName);
    ~DbBin();

public:
    void xnStart(const QString& xnName);
    void xnEnd(bool commit);

private:
    friend class DbBinImpl;
    DbBinImpl *d;
};

#endif