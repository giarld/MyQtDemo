#ifndef LUAENGINE_H
#define LUAENGINE_H
#include <lua.hpp>

class LuaEngine
{
public:
    LuaEngine();
    static LuaEngine *getInstance();

public:
    lua_State *getLuaState();
    void closeState();

private:
    void init();

private:
    static LuaEngine* mInstance;

    lua_State *L_State;
};

#endif // LUAENGINE_H
