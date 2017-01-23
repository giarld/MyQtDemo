#include <iostream>

using namespace std;

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

int main(int argc, char *argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(luaL_loadfile(L, "test.lua")) {
        cout << "open file error" << lua_tostring(L, -1) <<endl;
        return 1;
    }

    if(lua_pcall(L, 0, 0, 0)) {
        cout << "run error" << lua_tostring(L, -1) <<endl;
        return 2;
    }

    lua_getglobal(L, "sub");
    lua_pushinteger(L, 2);
    lua_pushinteger(L, 3);

    if(lua_pcall(L, 2, 1, 0) == 0) {
        int nTop = lua_gettop(L);
        if(lua_isinteger(L, nTop)) {
            cout << "2 + 3 = " << lua_tointeger(L, nTop) << endl;
        }
        lua_pop(L, nTop);
    }
    else {
        cout << "run error " << lua_tostring(L, -1) <<endl;
        lua_pop(L, lua_gettop(L));
    }

    return 0;
}
