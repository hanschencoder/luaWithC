#include <stdio.h>
#include "../lua/lauxlib.h"

#define TRUE 1
#define FALSE 0
typedef int bool;

int dumpStack(lua_State* L) {

    printf("=================== ջ�� ===================\n");
    printf("%-10s%-13s%s\n", "index ", "type", "value");
    int i;
    int length = lua_gettop(L);
    for (i = length; i >= 1; i--) {
        int t = lua_type(L, i);
        const char *type = lua_typename(L, t);
        switch (t) {
        case LUA_TNIL:
            printf("%-10d%-13s%s\n", i, type, "nil");
            break;
        case LUA_TBOOLEAN:
            printf("%-10d%-13s%s\n", i, type, (lua_toboolean(L, i) ? "true" : "false"));
            break;
        case LUA_TLIGHTUSERDATA:
            printf("%-10d%-13s%s\n", i, type, "light_user_data");
            break;
        case LUA_TNUMBER:
            printf("%-10d%-13s%f\n", i, type, lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            printf("%-10d%-13s%s\n", i, type, lua_tostring(L, i));
            break;
        case LUA_TTABLE:
            printf("%-10d%-13s%s\n", i, type, "table");
            break;
        case LUA_TFUNCTION:
            printf("%-10d%-13s%s\n", i, type, "function");
            break;
        case LUA_TUSERDATA:
            printf("%-10d%-13s%s\n", i, type, "user_data");
            break;
        case LUA_TTHREAD:
            printf("%-10d%-13s%s\n", i, type, "thread");
            break;
        default:
            break;
        }
    }
    printf("=================== ջ�� ===================\n");
    return 0;
}

lua_State *load_lua(char *filename, bool run_script) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    //���ؽű�������
    if (luaL_loadfile(L, filename) || (run_script && lua_pcall(L, 0, 0, 0))) {
        printf("load Lua script failed: %s\n", lua_tostring(L, -1));
        return NULL;
    }

    return L;
}

void c_call_lua() {
    char* lua_filename = "resource/c_call_lua.lua";
    lua_State *L = load_lua(lua_filename, TRUE);
    if (NULL == L) {
        return;
    }

    //��ȡ����
    lua_getglobal(L, "str");
    printf("[line:%d] luaV_tostring(L, -1):%s\n", __LINE__, lua_tostring(L, -1));

    //��ȡtable
    lua_getglobal(L, "table");
    lua_getfield(L, -1, "name");
    printf("[line:%d] lua_tostring(L,-1);:%s\n", __LINE__, lua_tostring(L, -1));

    //���ú���
    lua_getglobal(L, "add"); //��ȡ������ջ��
    lua_pushnumber(L, 10); //ѹ�����
    lua_pushnumber(L, 20);

    if (lua_pcall(L, 2, 1, 0) != 0) {
        printf("lua_pcall failed: %s\n", lua_tostring(L, -1));
        return;
    }
    int result = lua_tonumber(L, -1);
    printf("[line:%d] result:%d\n", __LINE__, result);
    dumpStack(L);

    //���ˣ�ջ�е�����ǣ�
    //=================== ջ�� ===================
    //  ����  ����      ֵ
    //   4   int��            30
    //   3   string��      hans
    //   2   table:     table
    //   1   string:    Hello, Lua !
    //=================== ջ�� ===================
    lua_close(L);
}

/**
 * ������lua�пɵ��õĺ�����Ҫ��ѭ�淶������ֵ����Ϊint���贫��lua_State
 */
int getTwoVar(lua_State *L) {

    dumpStack(L);
    //    =================== ջ�� ===================
    //    index     type         value
    //    2         number       123.000000
    //    1         string       hello, this string is from lua world
    //    =================== ջ�� ===================
    lua_pushnumber(L, 10);
    lua_pushstring(L, "hello");
    return 2;
}

void lua_call_c() {

    char* lua_filename = "resource/lua_call_c.lua";

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "getTwoVar", getTwoVar);
    luaL_dofile(L, lua_filename);
    lua_close(L);
}

int main(int argc, const char *argv[]) {
    c_call_lua();
    lua_call_c();
    return 0;
}

