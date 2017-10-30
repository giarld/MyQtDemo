#include "bclass.h"
#include "luaengine.h"
#include <QDebug>


BClass::BClass()
{
    b = 0;
    count = 1;
}

BClass::~BClass()
{
    qDebug() << "~BClass";
}

void BClass::retain()
{
    count ++;
}

void BClass::release()
{
    count --;
    if(count == 0)
        delete this;
}

int BClass::getB() const
{
    return b;
}

void BClass::setB(int value)
{
    b = value;
}

char *LuaBClass::className = "BClass";

void LuaBClass::Register()
{
    lua_State *L = LuaEngine::getInstance()->getLuaState();

    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, LuaBClass::create);
    lua_settable(L, -3);
    lua_setglobal(L, className);

    luaL_newmetatable(L, className);
    int metaPos = lua_gettop(L);

    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, LuaBClass::gc);
    lua_settable(L, metaPos);

    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, metaPos);

    lua_pushstring(L, "getB");
    lua_pushcfunction(L, LuaBClass::getB);
    lua_settable(L, metaPos);

    lua_pushstring(L, "setB");
    lua_pushcfunction(L, LuaBClass::setB);
    lua_settable(L, metaPos);

    lua_pop(L, lua_gettop(L));
}

int LuaBClass::create(lua_State *L)
{
    void **p = (void**)lua_newuserdata(L, sizeof(void*));
    *p = new BClass;
    luaL_getmetatable(L, className);
    lua_setmetatable(L, -2);
    return 1;
}

int LuaBClass::gc(lua_State *L)
{
    BClass *u = (BClass*)(*(void**)lua_touserdata(L, 1));
    u->release();
    return 0;
}

int LuaBClass::getB(lua_State *L)
{
    if(lua_gettop(L) < 1) {
        luaL_error(L, "LuaBClass::getB not super");
        return 0;
    }
    BClass *obj = getObj(L);
    lua_pushinteger(L, obj->getB());
    return 1;
}

int LuaBClass::setB(lua_State *L)
{
    if(lua_gettop(L) < 2) {
        luaL_error(L, "LuaBClass::setB args error");
        return 0;
    }

    BClass *obj = getObj(L);
    obj->setB(lua_tointeger(L, -1));
    return 0;
}

BClass *LuaBClass::getObj(lua_State *L)
{
    BClass *u = (BClass*)(*(void**)lua_touserdata(L, 1));
    if(!u) {
        luaL_error(L, "getObj error");
    }
    return u;
}
