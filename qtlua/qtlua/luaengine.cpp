#include "luaengine.h"

LuaEngine *LuaEngine::mInstance = NULL;

LuaEngine::LuaEngine()
{
    init();
}

LuaEngine *LuaEngine::getInstance()
{
    if(mInstance == NULL) {
        mInstance = new LuaEngine;
    }
    return mInstance;
}

lua_State *LuaEngine::getLuaState()
{
    return L_State;
}

void LuaEngine::closeState()
{
    lua_close(L_State);
}

void LuaEngine::init()
{
    L_State = luaL_newstate();
    luaL_openlibs(L_State);
}
