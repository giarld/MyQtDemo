#ifndef CTEST_H
#define CTEST_H

#include <lua.hpp>
#include <QObject>
#include "aclass.h"
#include "bclass.h"

class CTest
{
public:
    CTest();
    ~CTest();

    QString getName() const;
    void setName(const QString &value);

    int getId() const;
    void setId(int value);

    AClass *getA() const;
    void setA(AClass *value);

    BClass *getB() const;
    void setB(BClass *value);

private:
    QString name;
    int id;
    AClass *a;
    BClass *b;
};

class LuaCTest
{
public:
    static void Register();

private:
    LuaCTest() {}
    ~LuaCTest() {}

    static int create(lua_State *L);
    static int gc(lua_State *L);

    static int getName(lua_State *L);
    static int setName(lua_State *L);
    static int getId(lua_State *L);
    static int setId(lua_State *L);

    static int setNameAsyn(lua_State *L);

    //对象变量导出示例
    static int getA(lua_State *L);

    static int setB(lua_State *L);
    static int getB(lua_State *L);

    static CTest *getObj(lua_State *L);
private:
    static char *className;
};

#endif // CTEST_H
