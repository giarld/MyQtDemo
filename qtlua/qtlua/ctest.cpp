#include "ctest.h"
#include "luaengine.h"
#include <QDebug>

CTest::CTest()
{
    static int _id = 0;
    id = _id++;
    a = new AClass();
    b = nullptr;
}

CTest::~CTest()
{
    qDebug("~CTest");
    delete a;
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

AClass *CTest::getA() const
{
    return a;
}

void CTest::setA(AClass *value)
{
    a = value;
}

BClass *CTest::getB() const
{
    return b;
}

void CTest::setB(BClass *value)
{
    b = value;
}

//==================================
char *LuaCTest::className = "CTest";

void LuaCTest::Register()
{
    LuaAClass::Register();
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
    lua_settable(L, metaPos);

    lua_pushstring(L, "getName");
    lua_pushcfunction(L, LuaCTest::getName);
    lua_settable(L, -3);    //metaPos == -3

    lua_pushstring(L, "setName");
    lua_pushcfunction(L, LuaCTest::setName);
    lua_settable(L, metaPos);

    lua_pushstring(L, "getId");
    lua_pushcfunction(L, LuaCTest::getId);
    lua_settable(L, metaPos);

    lua_pushstring(L, "setId");
    lua_pushcfunction(L, LuaCTest::setId);
    lua_settable(L, metaPos);

    lua_pushstring(L, "setNameAsyn");
    lua_pushcfunction(L, LuaCTest::setNameAsyn);
    lua_settable(L, metaPos);

    lua_pushstring(L, "getA");
    lua_pushcfunction(L, LuaCTest::getA);
    lua_settable(L, metaPos);

    lua_pushstring(L, "getB");
    lua_pushcfunction(L, LuaCTest::getB);
    lua_settable(L, metaPos);

    lua_pushstring(L, "setB");
    lua_pushcfunction(L, LuaCTest::setB);
    lua_settable(L, metaPos);

    lua_pop(L, lua_gettop(L));
}

int LuaCTest::create(lua_State *L)
{
    QString name;
    if(lua_gettop(L) >= 1 && lua_isstring(L, -1)) {
        name = lua_tostring(L, -1);
    }
    CTest *obj = new CTest();
    obj->setName(name);
    void **p = (void**)lua_newuserdata(L, sizeof(void*));
    *p = obj;
    luaL_getmetatable(L, className);
    lua_setmetatable(L, -2);
    return 1;
}

int LuaCTest::gc(lua_State *L)
{
    CTest *u = (CTest*)(*(void**)lua_touserdata(L, 1));
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
        obj->setName(name);
        lua_pushstring(L, name);
        lua_pcall(L, 1, 0, 0);
    } else {
        qDebug() << "LuaCTest::setNameAsyn argv error";
    }
    lua_pop(L, lua_gettop(L));
    return 0;
}

int LuaCTest::getA(lua_State *L)
{
    if(lua_gettop(L) < 1) {
        return 0;
    }
    CTest *obj = getObj(L);
    AClass *aobj = obj->getA();

    // 导出AClass对象的userdate
    void **p = (void**)lua_newuserdata(L, sizeof(void*));
    *p = aobj;
    luaL_getmetatable(L, LuaAClass::className); //取出初始化过的metatable
    lua_setmetatable(L, -2);

    return 1;
}

int LuaCTest::setB(lua_State *L)
{
    if(lua_gettop(L) < 2)
        return 0;
    if(!lua_isuserdata(L, -1)) {
        return 0;
    }

    CTest *obj = getObj(L);
    BClass *ud = *(BClass**)lua_topointer(L, -1);
    if(ud) {
        obj->setB(ud);
    }
    return 0;
}

int LuaCTest::getB(lua_State *L)
{
    if(lua_gettop(L) < 1) {
        return 0;
    }
    CTest *obj = getObj(L);
    BClass *b = obj->getB();
    if(b == nullptr)
        return 0;
    void **p = (void**)lua_newuserdata(L, sizeof(void*));
    *p = b;
    b->retain();
    luaL_getmetatable(L, LuaBClass::className);
    lua_setmetatable(L, -2);
    return 1;
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
