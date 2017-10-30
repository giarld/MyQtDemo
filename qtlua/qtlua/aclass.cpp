#include "aclass.h"
#include "luaengine.h"
#include <QDebug>

AClass::AClass()
{

}

AClass::~AClass()
{
    qDebug("~AClass");
}

int AClass::getA() const
{
    return a;
}

void AClass::setA(int value)
{
    a = value;
}

// ===================================

char *LuaAClass::className = "AClass";

void LuaAClass::Register()
{
    lua_State *L = LuaEngine::getInstance()->getLuaState();

    luaL_newmetatable(L, className);
    int metaPos = lua_gettop(L);

    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, metaPos);

    //gc交给CTest的析构来做

    lua_pushstring(L, "getA");
    lua_pushcfunction(L, LuaAClass::getA);
    lua_settable(L, metaPos);

    lua_pushstring(L, "setA");
    lua_pushcfunction(L, LuaAClass::setA);
    lua_settable(L, metaPos);

    lua_pop(L, lua_gettop(L));
}

int LuaAClass::getA(lua_State *L)
{
    qDebug("LuaAClass::getA");
    if(lua_gettop(L) < 1) {
        luaL_error(L, "LuaAClass::getA this error");
        return 0;
    }
    AClass *obj = getObj(L);
    lua_pushinteger(L, obj->getA());
    return 1;
}

int LuaAClass::setA(lua_State *L)
{
    qDebug("LuaAClass::setA");
    if(lua_gettop(L) < 2) {
        luaL_error(L, "LuaAClass::getA this error");
        return 0;
    }
    AClass *obj = getObj(L);
    int a = lua_tointeger(L, -1);
    obj->setA(a);
    return 0;
}

AClass *LuaAClass::getObj(lua_State *L)
{
    AClass *ud = *(AClass**)lua_topointer(L, 1);
    if(!ud) {
        luaL_error(L, "getObj error");
    }
    return ud;
}
