/*
** LuaSQL, SQLite driver
** Author: Tiago Dionizio, Eduardo Quintao
** See Copyright Notice in license.html
** $Id: ls_sqlite.c,v 1.13 2008/05/04 02:46:17 tomas Exp $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sqlite.h"

#include "lua.h"
#include "lauxlib.h"
#if ! defined (LUA_VERSION_NUM) || LUA_VERSION_NUM < 501
#include "compat-5.1.h"
#endif


#include "luasql.h"

#define LUASQL_ENVIRONMENT_SQLITE "SQLite environment"
#define LUASQL_CONNECTION_SQLITE "SQLite connection"
#define LUASQL_CURSOR_SQLITE "SQLite cursor"

typedef struct
{
    short       closed;
} env_data;


typedef struct
{
	short        closed;
	int          env;                /* reference to environment */
	short        auto_commit;        /* 0 for manual commit */
	unsigned int cur_counter;          
	sqlite      *sql_conn;
} conn_data;


typedef struct
{
	short       closed;
	int         conn;               /* reference to connection */
	int         numcols;            /* number of columns */
	int         colnames, coltypes; /* reference to column information tables */
	sqlite_vm  *sql_vm;
} cur_data;

LUASQL_API int luaopen_luasql_sqlite(lua_State *L);


/*
** Check for valid environment.
*/
static env_data *getenvironment(lua_State *L) {
	env_data *env = (env_data *)luaL_checkudata(L, 1, LUASQL_ENVIRONMENT_SQLITE);
	luaL_argcheck(L, env != NULL, 1, LUASQL_PREFIX"environment expected");
	luaL_argcheck(L, !env->closed, 1, LUASQL_PREFIX"environment is closed");
	return env;
}


/*
** Check for valid connection.
*/
static conn_data *getconnection(lua_State *L) {
	conn_data *conn = (conn_data *)luaL_checkudata (L, 1, LUASQL_CONNECTION_SQLITE);
	luaL_argcheck(L, conn != NULL, 1, LUASQL_PREFIX"connection expected");
	luaL_argcheck(L, !conn->closed, 1, LUASQL_PREFIX"connection is closed");
	return conn;
}


/*
** Check for valid cursor.
*/
static cur_data *getcursor(lua_State *L) {
	cur_data *cur = (cur_data *)luaL_checkudata (L, 1, LUASQL_CURSOR_SQLITE);
	luaL_argcheck(L, cur != NULL, 1, LUASQL_PREFIX"cursor expected");
	luaL_argcheck(L, !cur->closed, 1, LUASQL_PREFIX"cursor is closed");
	return cur;
}

/*
** Finalizes the vm
** Return nil + errmsg or nil in case of sucess
*/
static int finalize(lua_State *L, cur_data *cur) {
    char *errmsg;
    if (sqlite_finalize(cur->sql_vm, &errmsg) != SQLITE_OK)
    {
        cur->sql_vm = NULL;
        lua_pushnil(L);
        lua_pushliteral(L, LUASQL_PREFIX);
        lua_pushstring(L, errmsg);
        sqlite_freemem(errmsg);
        lua_concat(L, 2);
        return 2;
    }
	lua_pushnil(L);
    return 1;
}


/*
** Get another row of the given cursor.
*/
static int cur_fetch (lua_State *L) {
	cur_data *cur = getcursor(L);
    sqlite_vm *vm = cur->sql_vm;
    const char **row = NULL;
    int res;

    if (vm == NULL)
        return 0;

    res = sqlite_step(vm, NULL, &row, NULL);

    /* no more results? */
    if (res == SQLITE_DONE)
        return finalize(L, cur);

    if (res != SQLITE_ROW)
        return finalize(L, cur);

	if (lua_istable (L, 2))
    {
		int i;
		const char *opts = luaL_optstring(L, 3, "n");

		if (strchr(opts, 'n') != NULL)
        {
			/* Copy values to numerical indices */
			for (i = 0; i < cur->numcols;)
            {
                lua_pushstring(L, row[i]);
				lua_rawseti(L, 2, ++i);
			}
        }
		if (strchr(opts, 'a') != NULL)
        {
			/* Copy values to alphanumerical indices */
            lua_rawgeti(L, LUA_REGISTRYINDEX, cur->colnames);

			for (i = 0; i < cur->numcols; i++)
            {
				lua_rawgeti(L, -1, i+1);
                lua_pushstring(L, row[i]);
				lua_rawset (L, 2);
			}
        }
		lua_pushvalue(L, 2);
		return 1; /* return table */
	}
	else
    {
		int i;
		luaL_checkstack (L, cur->numcols, LUASQL_PREFIX"too many columns");
		for (i = 0; i < cur->numcols; ++i)
			lua_pushstring(L, row[i]);
		return cur->numcols; /* return #numcols values */
	}
}


/*
** Close the cursor on top of the stack.
** Return 1
*/
static int cur_close(lua_State *L)
{
	conn_data *conn;
	cur_data *cur = (cur_data *)luaL_checkudata(L, 1, LUASQL_CURSOR_SQLITE);
	luaL_argcheck(L, cur != NULL, 1, LUASQL_PREFIX"cursor expected");
	if (cur->closed) {
		lua_pushboolean(L, 0);
		return 1;
	}

	/* Nullify structure fields. */
	cur->closed = 1;
	sqlite_finalize(cur->sql_vm, NULL);

	/* Decrement cursor counter on connection object */
	lua_rawgeti (L, LUA_REGISTRYINDEX, cur->conn);
	conn = lua_touserdata (L, -1);
	conn->cur_counter--;

	luaL_unref(L, LUA_REGISTRYINDEX, cur->conn);
	luaL_unref(L, LUA_REGISTRYINDEX, cur->colnames);
	luaL_unref(L, LUA_REGISTRYINDEX, cur->coltypes);

	lua_pushboolean(L, 1);
	return 1;
}


/*
** Return the list of field names.
*/
static int cur_getcolnames(lua_State *L)
{
    cur_data *cur = getcursor(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, cur->colnames);
	return 1;
}


/*
** Return the list of field types.
*/
static int cur_getcoltypes(lua_State *L)
{
    cur_data *cur = getcursor(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, cur->coltypes);
	return 1;
}


/*
** Create a new Cursor object and push it on top of the stack.
*/
/* static int create_cursor(lua_State *L, int conn, sqlite_vm *sql_vm,
    int numcols, const char **row, const char **col_info)*/
static int create_cursor(lua_State *L, int o, conn_data *conn, 
		sqlite_vm *sql_vm, int numcols, const char **col_info)
{
    int i;
	cur_data *cur = (cur_data*)lua_newuserdata(L, sizeof(cur_data));
	luasql_setmeta (L, LUASQL_CURSOR_SQLITE);

	/* increment cursor count for the connection creating this cursor */
	conn->cur_counter++;

	/* fill in structure */
	cur->closed = 0;
	cur->conn = LUA_NOREF;
	cur->numcols = numcols;
	cur->colnames = LUA_NOREF;
	cur->coltypes = LUA_NOREF;
	cur->sql_vm = sql_vm;

    lua_pushvalue(L, o);
	cur->conn = luaL_ref(L, LUA_REGISTRYINDEX);

    /* create table with column names */
    lua_newtable(L);
    for (i = 0; i < numcols;)
    {
        lua_pushstring(L, col_info[i]);
        lua_rawseti(L, -2, ++i);
    }
    cur->colnames = luaL_ref(L, LUA_REGISTRYINDEX);

    /* create table with column types */
    lua_newtable(L);
    for (i = 0; i < numcols;)
    {
        lua_pushstring(L, col_info[numcols+i]);
        lua_rawseti(L, -2, ++i);
    }
    cur->coltypes = luaL_ref(L, LUA_REGISTRYINDEX);

	return 1;
}


/*
** Close a Connection object.
*/
static int conn_close(lua_State *L)
{
	conn_data *conn = (conn_data *)luaL_checkudata(L, 1, LUASQL_CONNECTION_SQLITE);
	luaL_argcheck (L, conn != NULL, 1, LUASQL_PREFIX"connection expected");
	if (conn->closed) {
		lua_pushboolean(L, 0);
		return 1;
	}

	if (conn->cur_counter > 0)
		return luaL_error (L, LUASQL_PREFIX"there are open cursors");

	/* Nullify structure fields. */
	conn->closed = 1;
	luaL_unref(L, LUA_REGISTRYINDEX, conn->env);
    sqlite_close(conn->sql_conn);
	lua_pushboolean(L, 1);
	return 1;
}


/*
** Execute an SQL statement.
** Return a Cursor object if the statement is a query, otherwise
** return the number of tuples affected by the statement.
*/
static int conn_execute(lua_State *L)
{
	conn_data *conn = getconnection(L);
	const char *statement = luaL_checkstring(L, 2);
    int res;
    sqlite_vm *vm;
    char *errmsg;
    int numcols;
    const char **col_info;

    res = sqlite_compile(conn->sql_conn, statement, NULL, &vm, &errmsg);
    if (res != SQLITE_OK)
    {
        lua_pushnil(L);
        lua_pushliteral(L, LUASQL_PREFIX);
        lua_pushstring(L, errmsg);
        sqlite_freemem(errmsg);
        lua_concat(L, 2);
        return 2;
    }

    /* process first result to retrive query information and type */
    res = sqlite_step(vm, &numcols, NULL, &col_info);

    /* real query? if empty, must have numcols!=0 */
	if ((res == SQLITE_ROW) || ((res == SQLITE_DONE) && numcols))
	{
		sqlite_reset(vm, NULL);
		return create_cursor(L, 1, conn, vm, numcols, col_info);
	}

    if (res == SQLITE_DONE) /* and numcols==0, INSERT,UPDATE,DELETE statement */
    {
        sqlite_finalize(vm, NULL);
        /* return number of columns changed */
        lua_pushnumber(L, sqlite_changes(conn->sql_conn));
        return 1;
    }

    /* error */
    sqlite_finalize(vm, &errmsg);
    lua_pushnil(L);
    lua_pushliteral(L, LUASQL_PREFIX);
    lua_pushstring(L, errmsg);
    sqlite_freemem(errmsg);
    lua_concat(L, 2);
    return 2;
}


/*
** Commit the current transaction.
*/
static int conn_commit(lua_State *L)
{
    char *errmsg;
	conn_data *conn = getconnection(L);
	int res;
    const char *sql = "COMMIT";

    if (conn->auto_commit == 0) sql = "COMMIT;BEGIN";

    res = sqlite_exec(conn->sql_conn, sql, NULL, NULL, &errmsg);
    if (res != SQLITE_OK)
    {
		lua_pushnil(L);
        lua_pushliteral(L, LUASQL_PREFIX);
        lua_pushstring(L, errmsg);
        sqlite_freemem(errmsg);
        lua_concat(L, 2);
        return 2;
    }
    lua_pushboolean(L, 1);
	return 1;
}


/*
** Rollback the current transaction.
*/
static int conn_rollback(lua_State *L)
{
    char *errmsg;
	conn_data *conn = getconnection(L);
	int res;
    const char *sql = "ROLLBACK";

    if (conn->auto_commit == 0) sql = "ROLLBACK;BEGIN";

    res = sqlite_exec(conn->sql_conn, sql, NULL, NULL, &errmsg);
    if (res != SQLITE_OK)
    {
		lua_pushnil(L);
        lua_pushliteral(L, LUASQL_PREFIX);
        lua_pushstring(L, errmsg);
        sqlite_freemem(errmsg);
        lua_concat(L, 2);
        return 2;
    }
    lua_pushboolean(L, 1);
	return 1;
}


/*
** Set "auto commit" property of the connection.
** If 'true', then rollback current transaction.
** If 'false', then start a new transaction.
*/
static int conn_setautocommit(lua_State *L)
{
	conn_data *conn = getconnection(L);
	if (lua_toboolean(L, 2))
    {
		conn->auto_commit = 1;
        /* undo active transaction - ignore errors */
        sqlite_exec(conn->sql_conn, "ROLLBACK", NULL, NULL, NULL);
	}
	else
    {
        char *errmsg;
        int res;
		conn->auto_commit = 0;
        res = sqlite_exec(conn->sql_conn, "BEGIN", NULL, NULL, &errmsg);
        if (res != SQLITE_OK)
        {
            lua_pushliteral(L, LUASQL_PREFIX);
            lua_pushstring(L, errmsg);
            sqlite_freemem(errmsg);
            lua_concat(L, 2);
            lua_error(L);
        }
	}
	lua_pushboolean(L, 1);
	return 1;
}


/*
** Create a new Connection object and push it on top of the stack.
*/
static int create_connection(lua_State *L, int env, sqlite *sql_conn)
{
	conn_data *conn = (conn_data*)lua_newuserdata(L, sizeof(conn_data));
	luasql_setmeta(L, LUASQL_CONNECTION_SQLITE);

	/* fill in structure */
	conn->closed = 0;
	conn->env = LUA_NOREF;
	conn->auto_commit = 1;
	conn->sql_conn = sql_conn;
	conn->cur_counter = 0;
	lua_pushvalue (L, env);
	conn->env = luaL_ref (L, LUA_REGISTRYINDEX);
	return 1;
}


/*
** Connects to a data source.
*/
static int env_connect(lua_State *L)
{
    const char *sourcename;
	sqlite *conn;
    char *errmsg;
	getenvironment(L);  /* validate environment */
    sourcename = luaL_checkstring(L, 2);
    conn = sqlite_open(sourcename, 0, &errmsg);
    if (conn == NULL)
    {
        lua_pushnil(L);
        lua_pushliteral(L, LUASQL_PREFIX);
        lua_pushstring(L, errmsg);
        sqlite_freemem(errmsg);
        lua_concat(L, 2);
        return 2;
    }
    return create_connection(L, 1, conn);
}


/*
** Close environment object.
*/
static int env_close (lua_State *L)
{
	env_data *env = (env_data *)luaL_checkudata(L, 1, LUASQL_ENVIRONMENT_SQLITE);
	luaL_argcheck(L, env != NULL, 1, LUASQL_PREFIX"environment expected");
	if (env->closed) {
		lua_pushboolean(L, 0);
		return 1;
	}

	env->closed = 1;
	lua_pushboolean(L, 1);
	return 1;
}

static int conn_escape(lua_State *L)
{
    const char *from = luaL_checklstring (L, 2, 0);
    char *escaped = sqlite_mprintf("%q", from);
    if (escaped == NULL) 
    {
        lua_pushnil(L);
    } 
    else
    {
        lua_pushstring(L, escaped);        
        sqlite_freemem(escaped);
    }
    return 1;
}

/*
** Create metatables for each class of object.
*/
static void create_metatables (lua_State *L)
{
    struct luaL_reg environment_methods[] = {
        {"__gc", env_close},
        {"close", env_close},
        {"connect", env_connect},
		{NULL, NULL},
	};
    struct luaL_reg connection_methods[] = {
        {"__gc", conn_close},
        {"close", conn_close},
		{"escape", conn_escape},
        {"execute", conn_execute},
        {"commit", conn_commit},
        {"rollback", conn_rollback},
        {"setautocommit", conn_setautocommit},
		{NULL, NULL},
    };
    struct luaL_reg cursor_methods[] = {
        {"__gc", cur_close},
        {"close", cur_close},
        {"getcolnames", cur_getcolnames},
        {"getcoltypes", cur_getcoltypes},
        {"fetch", cur_fetch},
		{NULL, NULL},
    };
	luasql_createmeta(L, LUASQL_ENVIRONMENT_SQLITE, environment_methods);
	luasql_createmeta(L, LUASQL_CONNECTION_SQLITE, connection_methods);
	luasql_createmeta(L, LUASQL_CURSOR_SQLITE, cursor_methods);
	lua_pop (L, 3);
}

/*
** Creates an Environment and returns it.
*/
static int create_environment (lua_State *L)
{
	env_data *env = (env_data *)lua_newuserdata(L, sizeof(env_data));
	luasql_setmeta(L, LUASQL_ENVIRONMENT_SQLITE);

	/* fill in structure */
	env->closed = 0;
	return 1;
}


/*
** Creates the metatables for the objects and registers the
** driver open method.
*/
LUASQL_API int luaopen_luasql_sqlite(lua_State *L)
{
	struct luaL_reg driver[] = {
		{"sqlite", create_environment},
		{NULL, NULL},
	};
	create_metatables (L);
	luaL_openlib (L, LUASQL_TABLENAME, driver, 0);
	luasql_set_info (L);
	return 1;
}
