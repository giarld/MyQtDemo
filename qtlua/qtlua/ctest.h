#ifndef CTEST_H
#define CTEST_H

#include <lua.hpp>
#include <QObject>

class CTest
{
public:
    CTest();
    ~CTest();

    QString getName() const;
    void setName(const QString &value);

    int getId() const;
    void setId(int value);

private:
    QString name;
    int id;
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

    static CTest *getObj(lua_State *L);
private:
    static char *className;
};

#endif // CTEST_H
