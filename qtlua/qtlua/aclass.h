#ifndef ACLASS_H
#define ACLASS_H

#include <lua.hpp>

// 作为CTest的变量导出

class AClass
{
public:
    AClass();
    ~AClass();

    int getA() const;
    void setA(int value);

private:
    int a;
};

/**
 * 作为CTest的内部对象导出
 */
class LuaAClass {
public:
    static void Register();

private:
    LuaAClass() {}
    ~LuaAClass() {}

    static int getA(lua_State *L);
    static int setA(lua_State *L);

    static AClass *getObj(lua_State *L);


private:
    static char *className;
    friend class LuaCTest;
};

#endif // ACLASS_H
