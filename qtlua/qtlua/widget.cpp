#include "widget.h"
#include "ctest.h"
#include "luaengine.h"
#include "QDebug"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    LuaCTest::Register();
    lua_State *L = LuaEngine::getInstance()->getLuaState();

    luaL_dofile(L, "/Users/gxin/codes/gxin/MyQtDemo/qtlua/test.lua");
    lua_gc(L, LUA_GCCOLLECT, 0);

    exit(0);
}

Widget::~Widget()
{
    LuaEngine::getInstance()->closeState();
}
