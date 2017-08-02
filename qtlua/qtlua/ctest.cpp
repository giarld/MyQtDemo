#include "ctest.h"
#include "luaengine.h"
#include <QDebug>

CTest::CTest()
{
    id = 0;
}

CTest::~CTest()
{

}

QString CTest::getName() const
{
    return name;
}

void CTest::setName(const QString &value)
{
    name = value;
}

int CTest::getId() const
{
    return id;
}

void CTest::setId(int value)
{
    id = value;
}

//==================================
char *LuaCTest::className = "CTest";

void LuaCTest::Register()
{
    lua_State *L = LuaEngine::getInstance()->getLuaState();

    lua_newtable(L);
    lua_pushstring(L, "new");
    lua_pushcfunction(L, LuaCTest::create);
    lua_settable(L, -3);
    lua_setglobal(L, className);

    luaL_newmetatable(L, className);
    int metaPos = lua_gettop(L);

    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, LuaCTest::gc);
    lua_settable(L, metaPos);

    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    lua_pushstring(L, "getName");
    lua_pushcfunction(L, LuaCTest::getName);
    lua_settable(L, -3);

    lua_pushstring(L, "setName");
    lua_pushcfunction(L, LuaCTest::setName);
    lua_settable(L, -3);

    lua_pushstring(L, "getId");
    lua_pushcfunction(L, LuaCTest::getId);
    lua_settable(L, -3);

    lua_pushstring(L, "setId");
    lua_pushcfunction(L, LuaCTest::setId);
    lua_settable(L, -3);

    lua_pushstring(L, "setNameAsyn");
    lua_pushcfunction(L, LuaCTest::setNameAsyn);
    lua_settable(L, -3);
}

int LuaCTest::create(lua_State *L)
{
    CTest *obj = new CTest();
    void **p = (void**)lua_newuserdata(L, sizeof(void*));
    *p = obj;
    luaL_getmetatable(L, className);
    lua_setmetatable(L, -2);
    return 1;
}

int LuaCTest::gc(lua_State *L)
{
    CTest *u = (CTest*)(*(void**)lua_touserdata(L, -1));
    qDebug() << "LuaCTest::gc " << u->getId();
    delete u;
    return 0;
}

int LuaCTest::getName(lua_State *L)
{
    if(lua_gettop(L) < 1) {
        luaL_error(L, "LuaCTest::getName argv error");
        return 0;
    }
    CTest *obj = getObj(L);
    lua_pushstring(L, obj->getName().toStdString().c_str());
    return 1;
}

int LuaCTest::setName(lua_State *L)
{
    if(lua_gettop(L) < 2) {
        luaL_error(L, "LuaCTest::setName argv error");
        return 0;
    }
    CTest *obj = getObj(L);
    const char *name = lua_tostring(L, -1);
    obj->setName(name);
    return 0;
}

int LuaCTest::getId(lua_State *L)
{
    if(lua_gettop(L) < 1) {
        luaL_error(L, "LuaCTest::getId argv error");
        return 0;
    }
    CTest *obj = getObj(L);
    lua_pushinteger(L, obj->getId());
    return 1;
}

int LuaCTest::setId(lua_State *L)
{
    if(lua_gettop(L) < 2) {
        luaL_error(L, "LuaCTest::setId argv error");
        return 0;
    }
    CTest *obj = getObj(L);
    int id = lua_tointeger(L, -1);
    obj->setId(id);
    return 0;
}

int LuaCTest::setNameAsyn(lua_State *L)
{
    CTest *obj = getObj(L);
    int n = lua_gettop(L);
    if(n >= 2) {
        const char *name = lua_tostring(L, -2);
        lua_pushstring(L, name);
        lua_pcall(L, 1, 0, 0);
        obj->setName(name);
    } else {
        qDebug() << "LuaCTest::setNameAsyn argv error";
    }
    lua_pop(L, lua_gettop(L));
    return 0;
}

CTest *LuaCTest::getObj(lua_State *L)
{
//    luaL_checktype(L, 1, LUA_TUSERDATA);
//    void *ud = luaL_checkudata(L, 1, className);
    CTest *ud = *(CTest**)lua_topointer(L, 1);
    if(!ud) {
        luaL_error(L, "getObj error");
    }
    return ud;
}
