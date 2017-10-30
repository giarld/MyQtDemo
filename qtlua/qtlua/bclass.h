#ifndef BCLASS_H
#define BCLASS_H

#include <lua.hpp>

class BClass
{
public:
    BClass();
    ~BClass();

    //引用计数解决对象gc
    void retain();
    void release();

    int getB() const;
    void setB(int value);

private:
    int b;

    int count;
};

class LuaBClass {
public:
    static void Register();

private:
    LuaBClass() {}
    ~LuaBClass() {}

    static int create(lua_State *L);
    static int gc(lua_State *L);

    static int getB(lua_State *L);
    static int setB(lua_State *L);

    static BClass *getObj(lua_State *L);

private:
    static char *className;

    friend class LuaCTest;
};

#endif // BCLASS_H
