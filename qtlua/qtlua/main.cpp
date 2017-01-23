#include "widget.h"
#include <QApplication>
#include <lua.hpp>
#include <tolua.h>
#include <QFile>
#include <QTextStream>

static int tolua_new_widget(lua_State* L)
{
    Widget *widget = new Widget();
    tolua_pushusertype(L, widget, "Widget");
    return 1;
}

static int tolua_delete_widget(lua_State* L)
{
    Widget * widget = (Widget*) tolua_tousertype(L, 1, 0);
    if(widget != Q_NULLPTR) {
        widget->close();
        delete widget;
    }
    return 1;
}

static int tolua_show_widget(lua_State* L)
{
    Widget * widget = (Widget*) tolua_tousertype(L, 1, 0);
    if(widget != Q_NULLPTR) {
        widget->show();
    }
    return 1;
}

static int tolua_settitle_widget(lua_State* L)
{
    Widget * widget = (Widget*) tolua_tousertype(L, 1, 0);
    if(widget != Q_NULLPTR) {
        QString title = tolua_tostring(L, 2, 0);
        widget->setWindowTitle(title);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    tolua_open(L);
    tolua_module(L, NULL, 0);

    tolua_beginmodule(L, NULL);
    tolua_usertype(L, "Widget");
    tolua_cclass(L, "Widget", "Widget", "", tolua_delete_widget);

    tolua_beginmodule(L, "Widget");
    tolua_function(L, "new", tolua_new_widget);
    tolua_function(L, "show", tolua_show_widget);
    tolua_function(L, "setWindowTitle", tolua_settitle_widget);

    tolua_endmodule(L);
    tolua_endmodule(L);

    QFile file("./test.lua");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString codes = in.readAll();

        luaL_loadbuffer(L, codes.toLocal8Bit().data(), codes.toLocal8Bit().length(), "line");
        lua_pcall(L, 0, 0, 0);
    }
    file.close();

    return a.exec();
}
