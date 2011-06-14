/*
** LuaSQL, Oracle driver
** Authors: Tomas Guisasola, Leonardo Godinho
** See Copyright Notice in license.html
** $Id: ls_oci8.c,v 1.31 2009/02/07 23:16:23 tomas Exp $
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "oci.h"
#include "oratypes.h"
#include "ociapr.h"
#include "ocidem.h"

#include "lua.h"
#include "lauxlib.h"
#if ! defined (LUA_VERSION_NUM) || LUA_VERSION_NUM < 501
#include "compat-5.1.h"
#endif


#include "luasql.h"

#define LUASQL_ENVIRONMENT_OCI8 "Oracle environment"
#define LUASQL_CONNECTION_OCI8 "Oracle connection"
#define LUASQL_CURSOR_OCI8 "Oracle cursor"


typedef struct {
	short         closed;
	int           conn_counter;
	OCIEnv       *envhp;
	OCIError     *errhp; /* !!! */
} env_data;


typedef struct {
	short         closed;
	short         loggedon;
	short         auto_commit;        /* 0 for manual commit */
	int           cur_counter;
	int           env;                /* reference to environment */
	OCISvcCtx    *svchp;              /* service handle */
	OCIError     *errhp; /* !!! */
} conn_data;


typedef union {
	int     i;
	char   *s;
	double  d;
} column_value;


typedef struct {
	ub2           type;    /* database type */
	text         *name;    /* column name */
	ub4           namelen; /* column name length */
	ub2           max;     /* maximum size */
	sb2           null;    /* is null? */
	OCIDefine    *define;  /* define handle */
	column_value  val;
} column_data;


typedef struct {
	short         closed;
	int           conn;               /* reference to connection */
	int           numcols;            /* number of columns */
	int           colnames, coltypes; /* reference to column info tables */
	int           curr_tuple;         /* next tuple to be read */
	char         *text;               /* text of SQL statement */
	OCIStmt      *stmthp;             /* statement handle */
	OCIError     *errhp; /* !!! */
	column_data  *cols;               /* array of columns */
} cur_data;


int checkerr (lua_State *L, sword status, OCIError *errhp);
#define ASSERT(L,exp,err) {sword s = exp; if (s) return checkerr (L, s, err);}
LUASQL_API int luaopen_luasql_oci8 (lua_State *L);


/*
** Check for valid environment.
*/
static env_data *getenvironment (lua_State *L) {
	env_data *env = (env_data *)luaL_checkudata (L, 1, LUASQL_ENVIRONMENT_OCI8);
	luaL_argcheck (L, env != NULL, 1, LUASQL_PREFIX"environment expected");
	luaL_argcheck (L, !env->closed, 1, LUASQL_PREFIX"environment is closed");
	return env;
}


/*
** Check for valid connection.
*/
static conn_data *getconnection (lua_State *L) {
	conn_data *conn = (conn_data *)luaL_checkudata (L, 1, LUASQL_CONNECTION_OCI8);
	luaL_argcheck (L, conn != NULL, 1, LUASQL_PREFIX"connection expected");
	luaL_argcheck (L, !conn->closed, 1, LUASQL_PREFIX"connection is closed");
	return conn;
}


/*
** Check for valid cursor.
*/
static cur_data *getcursor (lua_State *L) {
	cur_data *cur = (cur_data *)luaL_checkudata (L, 1, LUASQL_CURSOR_OCI8);
	luaL_argcheck (L, cur != NULL, 1, LUASQL_PREFIX"cursor expected");
	luaL_argcheck (L, !cur->closed, 1, LUASQL_PREFIX"cursor is closed");
	return cur;
}


/*
** Push nil plus an error message.
*/
int checkerr (lua_State *L, sword status, OCIError *errhp) {
	lua_pushnil (L);
	switch (status) {
		case OCI_SUCCESS:
			lua_pushnil (L); /* !!!!!!!!!!!!! */
			break;
		case OCI_SUCCESS_WITH_INFO:
			lua_pushstring (L, LUASQL_PREFIX"Success with info!");
			break;
		case OCI_NEED_DATA:
			lua_pushstring (L, LUASQL_PREFIX"OCI_NEED_DATA");
			break;
		case OCI_NO_DATA:
			lua_pushstring (L, LUASQL_PREFIX"OCI_NODATA");
			break;
		case OCI_ERROR: {
			text errbuf[512];
			sb4 errcode = 0;
			OCIErrorGet (errhp, (ub4) 1, (text *) NULL, &errcode,
				errbuf, (ub4) sizeof (errbuf), OCI_HTYPE_ERROR);
			lua_pushstring (L, LUASQL_PREFIX);
			lua_pushstring (L, errbuf);
			lua_concat (L, 2);
			break;
		}
		case OCI_INVALID_HANDLE:
			lua_pushstring (L, LUASQL_PREFIX"OCI_INVALID_HANDLE");
			break;
		case OCI_STILL_EXECUTING:
			lua_pushstring (L, LUASQL_PREFIX"OCI_STILL_EXECUTE");
			break;
		case OCI_CONTINUE:
			lua_pushstring (L, LUASQL_PREFIX"OCI_CONTINUE");
			break;
	}
	return 2;
}


/*
** Copy the column name to the column structure and convert it to lower case.
*/
static void copy_column_name (column_data *col, text *name) {
	unsigned int i;
	col->name = (text *)malloc (col->namelen);
	memcpy (col->name, name, col->namelen);
	for (i = 0; i < col->namelen; i++)
		col->name[i] = tolower (col->name[i]);
}


/*
** Alloc buffers for column values.
*/
static int alloc_column_buffer (lua_State *L, cur_data *cur, int i) {
	/* column index ranges from 1 to numcols */
	/* C array index ranges from 0 to numcols-1 */
	column_data *col = &(cur->cols[i-1]);
	OCIParam *param;
	text *name;

	ASSERT (L, OCIParamGet (cur->stmthp, OCI_HTYPE_STMT, cur->errhp,
		(dvoid **)&param, i), cur->errhp);
	ASSERT (L, OCIAttrGet (param, OCI_DTYPE_PARAM,
		(dvoid *)&(name), (ub4 *)&(col->namelen),
		OCI_ATTR_NAME, cur->errhp), cur->errhp);
	copy_column_name (col, name);
	ASSERT (L, OCIAttrGet (param, OCI_DTYPE_PARAM,
		(dvoid *)&(col->type), (ub4 *)0, OCI_ATTR_DATA_TYPE,
		cur->errhp), cur->errhp);

	switch (col->type) {
		case SQLT_CHR:
		case SQLT_STR:
		case SQLT_VCS:
		case SQLT_AFC:
		case SQLT_AVC:
			ASSERT (L, OCIAttrGet (param, OCI_DTYPE_PARAM,
				(dvoid *)&(col->max), 0, OCI_ATTR_DATA_SIZE,
				cur->errhp), cur->errhp);
			col->val.s = calloc (col->max + 1, sizeof(col->val.s));
			ASSERT (L, OCIDefineByPos (cur->stmthp, &(col->define),
				cur->errhp, (ub4)i, col->val.s, col->max+1,
				SQLT_STR /*col->type*/, (dvoid *)&(col->null), (ub2 *)0,
				(ub2 *)0, (ub4) OCI_DEFAULT), cur->errhp);
			break;
		case SQLT_NUM:
		case SQLT_FLT:
		case SQLT_INT:
		/* case SQLT_UIN: */
			ASSERT (L, OCIDefineByPos (cur->stmthp, &(col->define),
				cur->errhp, (ub4)i, &(col->val.d), sizeof(col->val.d),
				SQLT_FLT, (dvoid *)&(col->null), (ub2 *)0,
				(ub2 *)0, (ub4) OCI_DEFAULT), cur->errhp);
			break;
		case SQLT_CLOB: {
			env_data *env;
			conn_data *conn;
			lua_rawgeti (L, LUA_REGISTRYINDEX, cur->conn);
			conn = (conn_data *)lua_touserdata (L, -1);
			lua_rawgeti (L, LUA_REGISTRYINDEX, conn->env);
			env = (env_data *)lua_touserdata (L, -1);
			lua_pop (L, 2);
			ASSERT (L, OCIDescriptorAlloc (env->envhp, (dvoid *)&(col->val.s),
				OCI_DTYPE_LOB, (size_t)0, (dvoid **)0), cur->errhp);
			ASSERT (L, OCIDefineByPos (cur->stmthp, &(col->define),
				cur->errhp, (ub4)i, &(col->val.s), (sb4)sizeof(dvoid *),
				SQLT_CLOB, (dvoid *)&(col->null), (ub2 *)0, (ub2 *)0,
				OCI_DEFAULT), cur->errhp);
			break;
		}
		default:
			luaL_error (L, LUASQL_PREFIX"invalid type %d #%d", col->type, i);
			break;
	}
	return 0;
}


/*
** Deallocate column buffers.
*/
static int free_column_buffers (lua_State *L, cur_data *cur, int i) {
	/* column index ranges from 1 to numcols */
	/* C array index ranges from 0 to numcols-1 */
	column_data *col = &(cur->cols[i-1]);
	free (col->name);
	switch (col->type) {
		case SQLT_INT:
		case SQLT_FLT:
		case SQLT_NUM:
			break;
		case SQLT_CHR:
		case SQLT_STR:
		case SQLT_VCS:
		case SQLT_AFC:
		case SQLT_AVC:
			free(col->val.s);
			break;
		case SQLT_CLOB:
			ASSERT (L, OCIDescriptorFree (col->val.s,
				OCI_DTYPE_LOB), cur->errhp);
			break;
		default:
			luaL_error (L, LUASQL_PREFIX"unknown type");
			/*printf("free_buffers(): Unknow Type: %d count: %d\n",cols.item[count].type, count );*/
			break;
	}
    return 0;
}


/*
** Push a value on top of the stack.
*/
static int pushvalue (lua_State *L, cur_data *cur, int i) {
	/* column index ranges from 1 to numcols */
	/* C array index ranges from 0 to numcols-1 */
	column_data *col = &(cur->cols[i-1]);
	if (col->null) {
		/* Oracle NULL => Lua nil */
		lua_pushnil (L);
		return 1;
	}
	switch (col->type) {
		case SQLT_NUM:
		case SQLT_INT:
		case SQLT_FLT:
			lua_pushnumber (L, col->val.d);
			break;
		case SQLT_CHR:
		case SQLT_STR:
		case SQLT_VCS:
		case SQLT_AFC:
		case SQLT_AVC:
			lua_pushstring (L, (char *)(col->val.s));
			break;
		case SQLT_CLOB: {
			ub4 lob_len;
			conn_data *conn;
			env_data *env;
			lua_rawgeti (L, LUA_REGISTRYINDEX, cur->conn);
			conn = lua_touserdata (L, -1);
			lua_rawgeti (L, LUA_REGISTRYINDEX, conn->env);
			env = lua_touserdata (L, -1);
			lua_pop (L, 2);
			ASSERT (L, OCILobGetLength (conn->svchp, cur->errhp,
				(OCILobLocator *)col->val.s, &lob_len), cur->errhp);
			if (lob_len > 0) {
				char *lob_buffer=malloc(lob_len);
				ub4 amount = lob_len;

				ASSERT(L, OCILobRead(conn->svchp, cur->errhp,
					(OCILobLocator *) col->val.s, &amount, (ub4) 1,
					(dvoid *) lob_buffer, (ub4) lob_len, (dvoid *)0,
					(sb4 (*) (dvoid *, CONST dvoid *, ub4, ub1)) 0,
					(ub2) 0, (ub1) SQLCS_IMPLICIT), cur->errhp);
				lua_pushlstring (L, lob_buffer, amount);
				free(lob_buffer);
			} else {
				lua_pushstring (L, "");
			}
			break;
		}
		default:
			luaL_error (L, LUASQL_PREFIX"unexpected error");
	}
	return 1;
}


/*
** Get another row of the given cursor.
*/
static int cur_fetch (lua_State *L) {
	cur_data *cur = getcursor (L);
	sword status = OCIStmtFetch (cur->stmthp, cur->errhp, 1,
		OCI_FETCH_NEXT, OCI_DEFAULT);

	if (status == OCI_NO_DATA) {
		/* No more rows */
		lua_pushnil (L);
		return 1;
	} else if (status != OCI_SUCCESS) {
		/* Error */
		return checkerr (L, status, cur->errhp);
	}

	if (lua_istable (L, 2)) {
		int i;
		const char *opts = luaL_optstring (L, 3, "n");
		if (strchr (opts, 'n') != NULL)
			/* Copy values to numerical indices */
			for (i = 1; i <= cur->numcols; i++) {
				int ret = pushvalue (L, cur, i);
				if (ret != 1)
					return ret;
				lua_rawseti (L, 2, i);
			}
		if (strchr (opts, 'a') != NULL)
			/* Copy values to alphanumerical indices */
			for (i = 1; i <= cur->numcols; i++) {
				column_data *col = &(cur->cols[i-1]);
				int ret;
				lua_pushlstring (L, col->name, col->namelen);
				if ((ret = pushvalue (L, cur, i)) != 1)
					return ret;
				lua_rawset (L, 2);
			}
		lua_pushvalue(L, 2);
		return 1; /* return table */
	}
	else {
		int i;
		luaL_checkstack (L, cur->numcols, LUASQL_PREFIX"too many columns");
		for (i = 1; i <= cur->numcols; i++) {
			int ret = pushvalue (L, cur, i);
			if (ret != 1)
				return ret;
		}
		return cur->numcols; /* return #numcols values */
	}
}


/*
** Close the cursor on top of the stack.
** Return 1
*/
static int cur_close (lua_State *L) {
	int i;
	conn_data *conn;
	cur_data *cur = (cur_data *)luaL_checkudata (L, 1, LUASQL_CURSOR_OCI8);
	luaL_argcheck (L, cur != NULL, 1, LUASQL_PREFIX"cursor expected");
	if (cur->closed) {
		lua_pushboolean (L, 0);
		return 1;
	}

	/* Deallocate buffers. */
	for (i = 1; i <= cur->numcols; i++) {
		int ret = free_column_buffers (L, cur, i);
		if (ret)
			return ret;
	}
	free (cur->cols);
	free (cur->text);

	/* Nullify structure fields. */
	cur->closed = 1;
	if (cur->stmthp)
		OCIHandleFree ((dvoid *)cur->stmthp, OCI_HTYPE_STMT);
	if (cur->errhp)
		OCIHandleFree ((dvoid *)cur->errhp, OCI_HTYPE_ERROR);
	/* Decrement cursor counter on connection object */
	lua_rawgeti (L, LUA_REGISTRYINDEX, cur->conn);
	conn = lua_touserdata (L, -1);
	conn->cur_counter--;
	luaL_unref (L, LUA_REGISTRYINDEX, cur->conn);
	luaL_unref (L, LUA_REGISTRYINDEX, cur->colnames);
	luaL_unref (L, LUA_REGISTRYINDEX, cur->coltypes);

	lua_pushboolean (L, 1);
	return 1;
}


/*
** Return the list of field names as a table on top of the stack.
*/
static int cur_getcolnames (lua_State *L) {
	cur_data *cur = getcursor (L);
	if (cur->colnames != LUA_NOREF)
		lua_rawgeti (L, LUA_REGISTRYINDEX, cur->colnames);
	else {
		int i;
		lua_newtable (L);
		for (i = 1; i <= cur->numcols; i++) {
			column_data *col = &(cur->cols[i-1]);
			lua_pushlstring (L, col->name, col->namelen);
			lua_rawseti (L, -2, i);
		}
		lua_pushvalue (L, -1);
		cur->colnames = luaL_ref (L, LUA_REGISTRYINDEX);
	}
	return 1;
}


/*
**
*/
static char *getcolumntype (column_data *col) {
	switch (col->type) {
		case SQLT_CHR:
		case SQLT_STR:
		case SQLT_VCS:
		case SQLT_AFC:
		case SQLT_AVC:
			return "string";
		case SQLT_NUM:
		case SQLT_FLT:
		case SQLT_INT:
		/* case SQLT_UIN: */
			return "number";
		case SQLT_CLOB:
			return "string";
		default:
			return "";
	}
}


/*
** Return the list of field types as a table on top of the stack.
*/
static int cur_getcoltypes (lua_State *L) {
	cur_data *cur = getcursor (L);
	if (cur->coltypes != LUA_NOREF)
		lua_rawgeti (L, LUA_REGISTRYINDEX, cur->coltypes);
	else {
		int i;
		lua_newtable (L);
		for (i = 1; i <= cur->numcols; i++) {
			column_data *col = &(cur->cols[i-1]);
			lua_pushnumber (L, i);
			lua_pushstring (L, getcolumntype (col));
			lua_rawset (L, -3);
		}
		lua_pushvalue (L, -1);
		cur->coltypes = luaL_ref (L, LUA_REGISTRYINDEX);
	}
	return 1;
}


/*
** Push the number of rows.
*/
static int cur_numrows (lua_State *L) {
	int n;
	cur_data *cur = getcursor (L);
	ASSERT (L, OCIAttrGet ((dvoid *) cur->stmthp, OCI_HTYPE_STMT, (dvoid *)&n,
		(ub4)0, OCI_ATTR_NUM_ROWS, cur->errhp), cur->errhp);
	lua_pushnumber (L, n);
	return 1;
}


/*
** Close a Connection object.
*/
static int conn_close (lua_State *L) {
	env_data *env;
	conn_data *conn = (conn_data *)luaL_checkudata (L, 1, LUASQL_CONNECTION_OCI8);
	luaL_argcheck (L, conn != NULL, 1, LUASQL_PREFIX"connection expected");
	if (conn->closed) {
		lua_pushboolean (L, 0);
		return 1;
	}
	if (conn->cur_counter > 0)
		return luaL_error (L, LUASQL_PREFIX"there are open cursors");

	/* Nullify structure fields. */
	conn->closed = 1;
	if (conn->svchp) {
		if (conn->loggedon)
			OCILogoff (conn->svchp, conn->errhp);
		else
			OCIHandleFree ((dvoid *)conn->svchp, OCI_HTYPE_SVCCTX);
	}
	if (conn->errhp)
		OCIHandleFree ((dvoid *)conn->errhp, OCI_HTYPE_ERROR);
	/* Decrement connection counter on environment object */
	lua_rawgeti (L, LUA_REGISTRYINDEX, conn->env);
	env = lua_touserdata (L, -1);
	env->conn_counter--;
	luaL_unref (L, LUA_REGISTRYINDEX, conn->env);

	lua_pushboolean (L, 1);
	return 1;
}


/*
** Create a new Cursor object and push it on top of the stack.
*/
static int create_cursor (lua_State *L, int o, conn_data *conn, OCIStmt *stmt, const char *text) {
	int i;
	env_data *env;
	cur_data *cur = (cur_data *)lua_newuserdata(L, sizeof(cur_data));
	luasql_setmeta (L, LUASQL_CURSOR_OCI8);

	conn->cur_counter++;
	/* fill in structure */
	cur->closed = 0;
	cur->numcols = 0;
	cur->colnames = LUA_NOREF;
	cur->coltypes = LUA_NOREF;
	cur->curr_tuple = 0;
	cur->stmthp = stmt;
	cur->errhp = NULL;
	cur->cols = NULL;
	cur->text = strdup (text);
	lua_pushvalue (L, o);
	cur->conn = luaL_ref (L, LUA_REGISTRYINDEX);

	/* error handler */
	lua_rawgeti (L, LUA_REGISTRYINDEX, conn->env);
	env = lua_touserdata (L, -1);
	lua_pop (L, 1);
	ASSERT (L, OCIHandleAlloc((dvoid *) env->envhp,
		(dvoid **) &(cur->errhp), (ub4) OCI_HTYPE_ERROR, (size_t) 0,
		(dvoid **) 0), conn->errhp);
	/* get number of columns */
	ASSERT (L, OCIAttrGet ((dvoid *)stmt, (ub4)OCI_HTYPE_STMT,
		(dvoid *)&cur->numcols, (ub4 *)0, (ub4)OCI_ATTR_PARAM_COUNT,
		cur->errhp), cur->errhp);
	cur->cols = (column_data *)malloc (sizeof(column_data) * cur->numcols);
	/* define output variables */
	/* Oracle and Lua column indices ranges from 1 to numcols */
	/* C array indices ranges from 0 to numcols-1 */
	for (i = 1; i <= cur->numcols; i++) {
		int ret = alloc_column_buffer (L, cur, i);
		if (ret)
			return ret;
	}

	return 1;
}


/*
** Execute an SQL statement.
** Return a Cursor object if the statement is a query, otherwise
** return the number of tuples affected by the statement.
*/
static int conn_execute (lua_State *L) {
	env_data *env;
	conn_data *conn = getconnection (L);
	const char *statement = luaL_checkstring (L, 2);
	sword status;
	ub4 prefetch = 0;
	ub4 iters;
	ub4 mode;
	ub2 type;
	OCIStmt *stmthp;

	/* get environment */
	lua_rawgeti (L, LUA_REGISTRYINDEX, conn->env);
	if (!lua_isuserdata (L, -1))
		luaL_error(L,LUASQL_PREFIX"invalid environment in connection!");
	env = (env_data *)lua_touserdata (L, -1);
	/* statement handle */
	ASSERT (L, OCIHandleAlloc ((dvoid *)env->envhp, (dvoid **)&stmthp,
		OCI_HTYPE_STMT, (size_t)0, (dvoid **)0), conn->errhp);
	ASSERT (L, OCIAttrSet ((dvoid *)stmthp, (ub4)OCI_HTYPE_STMT,
		(dvoid *)&prefetch, (ub4)0, (ub4)OCI_ATTR_PREFETCH_ROWS,
		conn->errhp), conn->errhp);
	ASSERT (L, OCIStmtPrepare (stmthp, conn->errhp, (text *)statement,
		(ub4) strlen(statement), (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT),
		conn->errhp);
	/* statement type */
	ASSERT (L, OCIAttrGet ((dvoid *)stmthp, (ub4) OCI_HTYPE_STMT,
		(dvoid *)&type, (ub4 *)0, (ub4)OCI_ATTR_STMT_TYPE, conn->errhp),
		conn->errhp);
	if (type == OCI_STMT_SELECT)
		iters = 0;
	else
		iters = 1;
	if (conn->auto_commit)
		mode = OCI_COMMIT_ON_SUCCESS;
	else
		mode = OCI_DEFAULT;
	/* execute statement */
	status = OCIStmtExecute (conn->svchp, stmthp, conn->errhp, iters,
		(ub4)0, (CONST OCISnapshot *)NULL, (OCISnapshot *)NULL, mode);
	if (status && (status != OCI_NO_DATA)) {
		OCIHandleFree ((dvoid *)stmthp, OCI_HTYPE_STMT);
		return checkerr (L, status, conn->errhp);
	}
	if (type == OCI_STMT_SELECT) {
		/* create cursor */
		return create_cursor (L, 1, conn, stmthp, statement);
	} else {
		/* return number of rows */
		int rows_affected;
		ASSERT (L, OCIAttrGet ((dvoid *)stmthp, (ub4)OCI_HTYPE_STMT,
			(dvoid *)&rows_affected, (ub4 *)0,
			(ub4)OCI_ATTR_ROW_COUNT, conn->errhp), conn->errhp);
		OCIHandleFree ((dvoid *)stmthp, OCI_HTYPE_STMT);
		lua_pushnumber (L, rows_affected);
		return 1;
	}
}


/*
** Commit the current transaction.
*/
static int conn_commit (lua_State *L) {
	conn_data *conn = getconnection (L);
	ASSERT (L, OCITransCommit (conn->svchp, conn->errhp, OCI_DEFAULT),
		conn->errhp);
/*
	if (conn->auto_commit == 0) 
		ASSERT (L, OCITransStart (conn->svchp, conn->errhp...
*/
	return 0;
}


/*
** Rollback the current transaction.
*/
static int conn_rollback (lua_State *L) {
	conn_data *conn = getconnection (L);
	ASSERT (L, OCITransRollback (conn->svchp, conn->errhp, OCI_DEFAULT),
		conn->errhp);
/*
	if (conn->auto_commit == 0) 
		sql_begin(conn); 
*/
	return 0;
}


/*
** Set "auto commit" property of the connection.
** If 'true', then rollback current transaction.
** If 'false', then start a new transaction.
*/
static int conn_setautocommit (lua_State *L) {
	conn_data *conn = getconnection (L);
	if (lua_toboolean (L, 2)) {
		conn->auto_commit = 1;
		/* Undo active transaction. */
		ASSERT (L, OCITransRollback (conn->svchp, conn->errhp,
			OCI_DEFAULT), conn->errhp);
	}
	else {
		conn->auto_commit = 0;
		/* sql_begin(conn);*/
	}
	lua_pushboolean(L, 1);
	return 1;
}


/*
** Connects to a data source.
*/
static int env_connect (lua_State *L) {
	env_data *env = getenvironment (L);
	const char *sourcename = luaL_checkstring(L, 2);
	const char *username = luaL_optstring(L, 3, NULL);
	const char *password = luaL_optstring(L, 4, NULL);
	/* Sizes of strings */
	size_t snlen = strlen(sourcename);
	size_t userlen = (username) ? strlen(username) : 0;
	size_t passlen = (password) ? strlen(password) : 0;
	/* Alloc connection object */
	conn_data *conn = (conn_data *)lua_newuserdata(L, sizeof(conn_data));

	/* fill in structure */
	luasql_setmeta (L, LUASQL_CONNECTION_OCI8);
	conn->env = LUA_NOREF;
	conn->closed = 1;
	conn->auto_commit = 1;
	conn->cur_counter = 0;
	conn->loggedon = 0;
	conn->svchp = NULL;
	conn->errhp = NULL;
	lua_pushvalue (L, 1);
	conn->env = luaL_ref (L, LUA_REGISTRYINDEX);

	/* error handler */
	ASSERT (L, OCIHandleAlloc((dvoid *) env->envhp,
		(dvoid **) &(conn->errhp), /* !!! */
		(ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0), env->errhp);
	/* service handler */
	/*ASSERT (L, OCIHandleAlloc((dvoid *) env->envhp,
		(dvoid **) &(conn->svchp),
		(ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0), conn->errhp);
*/
	/* login */
	ASSERT (L, OCILogon(env->envhp, conn->errhp, &(conn->svchp),
		(CONST text*)username, userlen,
		(CONST text*)password, passlen,
		(CONST text*)sourcename, snlen), conn->errhp);
	conn->closed = 0;
	env->conn_counter++;
	conn->loggedon = 1;

	return 1;
}


/*
** Close environment object.
*/
static int env_close (lua_State *L) {
	env_data *env = (env_data *)luaL_checkudata (L, 1, LUASQL_ENVIRONMENT_OCI8);
	luaL_argcheck (L, env != NULL, 1, LUASQL_PREFIX"environment expected");
	if (env->closed) {
		lua_pushboolean (L, 0);
		return 1;
	}
	if (env->conn_counter > 0)
		return luaL_error (L, LUASQL_PREFIX"there are open connections");

	env->closed = 1;
	/* desalocar: env->errhp e env->envhp */
	if (env->envhp)
		OCIHandleFree ((dvoid *)env->envhp, OCI_HTYPE_ENV);
	if (env->errhp)
		OCIHandleFree ((dvoid *)env->errhp, OCI_HTYPE_ERROR);
	lua_pushboolean (L, 1);
	return 1;
}


/*
** Creates an Environment and returns it.
*/
static int create_environment (lua_State *L) {
	env_data *env = (env_data *)lua_newuserdata(L, sizeof(env_data));
	luasql_setmeta (L, LUASQL_ENVIRONMENT_OCI8);
	/* fill in structure */
	env->closed = 0;
	env->conn_counter = 0;
	env->envhp = NULL;
	env->errhp = NULL;
/* maybe OCI_SHARED and OCI_THREADED ??? */
	if (OCIEnvCreate ( &(env->envhp), (ub4)OCI_DEFAULT, (dvoid *)0,
			(dvoid * (*)(dvoid *, size_t)) 0,
			(dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
			(void (*)(dvoid *, dvoid *)) 0,
			(size_t) 0,
			(dvoid **) 0))
		luasql_faildirect (L, "couldn't create environment");
	/* error handler */
	ASSERT (L, OCIHandleAlloc((dvoid *) env->envhp,
		(dvoid **) &(env->errhp), /* !!! */
		(ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0), env->errhp);
	return 1;
}


/*
** Create metatables for each class of object.
*/
static void create_metatables (lua_State *L) {
	struct luaL_reg environment_methods[] = {
		{"__gc", env_close},
		{"close", env_close},
		{"connect", env_connect},
		{NULL, NULL},
	};
	struct luaL_reg connection_methods[] = {
		{"__gc", conn_close},
		{"close", conn_close},
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
		{"numrows", cur_numrows},
		{NULL, NULL},
	};
	luasql_createmeta (L, LUASQL_ENVIRONMENT_OCI8, environment_methods);
	luasql_createmeta (L, LUASQL_CONNECTION_OCI8, connection_methods);
	luasql_createmeta (L, LUASQL_CURSOR_OCI8, cursor_methods);
	lua_pop (L, 3);
}


/*
** Creates the metatables for the objects and registers the
** driver open method.
*/
LUASQL_API int luaopen_luasql_oci8 (lua_State *L) {
	struct luaL_reg driver[] = {
		{"oci8", create_environment},
		{NULL, NULL},
	};
	create_metatables (L);
	luaL_openlib (L, LUASQL_TABLENAME, driver, 0);
	luasql_set_info (L);
	return 1;
}
