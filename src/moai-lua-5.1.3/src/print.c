/*
** $Id: print.c,v 1.55a 2006/05/31 13:30:05 lhf Exp $
** print bytecodes
** See Copyright Notice in lua.h
*/

#include <ctype.h>
#include <stdio.h>
#include <zipfs/zipfs.h>

#define luac_c
#define LUA_CORE

#include "ldebug.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lundump.h"

#define PrintFunction	luaU_print

#define Sizeof(x)	((int)sizeof(x))
#define VOID(p)		((const void*)(p))

static void PrintString(const TString* ts)
{
 const char* s=getstr(ts);
 size_t i,n=ts->tsv.len;
 putchar('"');
 for (i=0; i<n; i++)
 {
  int c=s[i];
  switch (c)
  {
   case '"': zipfs_printf("\\\""); break;
   case '\\': zipfs_printf("\\\\"); break;
   case '\a': zipfs_printf("\\a"); break;
   case '\b': zipfs_printf("\\b"); break;
   case '\f': zipfs_printf("\\f"); break;
   case '\n': zipfs_printf("\\n"); break;
   case '\r': zipfs_printf("\\r"); break;
   case '\t': zipfs_printf("\\t"); break;
   case '\v': zipfs_printf("\\v"); break;
   default:	if (isprint((unsigned char)c))
   			zipfs_putchar(c);
		else
			zipfs_printf("\\%03u",(unsigned char)c);
  }
 }
 putchar('"');
}

static void PrintConstant(const Proto* f, int i)
{
 const TValue* o=&f->k[i];
 switch (ttype(o))
 {
  case LUA_TNIL:
	zipfs_printf("nil");
	break;
  case LUA_TBOOLEAN:
	zipfs_printf(bvalue(o) ? "true" : "false");
	break;
  case LUA_TNUMBER:
	zipfs_printf(LUA_NUMBER_FMT,nvalue(o));
	break;
  case LUA_TSTRING:
	PrintString(rawtsvalue(o));
	break;
  default:				/* cannot happen */
	zipfs_printf("? type=%d",ttype(o));
	break;
 }
}

static void PrintCode(const Proto* f)
{
 const Instruction* code=f->code;
 int pc,n=f->sizecode;
 for (pc=0; pc<n; pc++)
 {
  Instruction i=code[pc];
  OpCode o=GET_OPCODE(i);
  int a=GETARG_A(i);
  int b=GETARG_B(i);
  int c=GETARG_C(i);
  int bx=GETARG_Bx(i);
  int sbx=GETARG_sBx(i);
  int line=getline(f,pc);
  zipfs_printf("\t%d\t",pc+1);
  if (line>0) zipfs_printf("[%d]\t",line); else zipfs_printf("[-]\t");
  zipfs_printf("%-9s\t",luaP_opnames[o]);
  switch (getOpMode(o))
  {
   case iABC:
    zipfs_printf("%d",a);
    if (getBMode(o)!=OpArgN) zipfs_printf(" %d",ISK(b) ? (-1-INDEXK(b)) : b);
    if (getCMode(o)!=OpArgN) zipfs_printf(" %d",ISK(c) ? (-1-INDEXK(c)) : c);
    break;
   case iABx:
    if (getBMode(o)==OpArgK) zipfs_printf("%d %d",a,-1-bx); else zipfs_printf("%d %d",a,bx);
    break;
   case iAsBx:
    if (o==OP_JMP) zipfs_printf("%d",sbx); else zipfs_printf("%d %d",a,sbx);
    break;
  }
  switch (o)
  {
   case OP_LOADK:
    zipfs_printf("\t; "); PrintConstant(f,bx);
    break;
   case OP_GETUPVAL:
   case OP_SETUPVAL:
    zipfs_printf("\t; %s", (f->sizeupvalues>0) ? getstr(f->upvalues[b]) : "-");
    break;
   case OP_GETGLOBAL:
   case OP_SETGLOBAL:
    zipfs_printf("\t; %s",svalue(&f->k[bx]));
    break;
   case OP_GETTABLE:
   case OP_SELF:
    if (ISK(c)) { zipfs_printf("\t; "); PrintConstant(f,INDEXK(c)); }
    break;
   case OP_SETTABLE:
   case OP_ADD:
   case OP_SUB:
   case OP_MUL:
   case OP_DIV:
   case OP_POW:
   case OP_EQ:
   case OP_LT:
   case OP_LE:
    if (ISK(b) || ISK(c))
    {
     zipfs_printf("\t; ");
     if (ISK(b)) PrintConstant(f,INDEXK(b)); else zipfs_printf("-");
     zipfs_printf(" ");
     if (ISK(c)) PrintConstant(f,INDEXK(c)); else zipfs_printf("-");
    }
    break;
   case OP_JMP:
   case OP_FORLOOP:
   case OP_FORPREP:
    zipfs_printf("\t; to %d",sbx+pc+2);
    break;
   case OP_CLOSURE:
    zipfs_printf("\t; %p",VOID(f->p[bx]));
    break;
   case OP_SETLIST:
    if (c==0) zipfs_printf("\t; %d",(int)code[++pc]);
    else zipfs_printf("\t; %d",c);
    break;
   default:
    break;
  }
  zipfs_printf("\n");
 }
}

#define SS(x)	(x==1)?"":"s"
#define S(x)	x,SS(x)

static void PrintHeader(const Proto* f)
{
 const char* s=getstr(f->source);
 if (*s=='@' || *s=='=')
  s++;
 else if (*s==LUA_SIGNATURE[0])
  s="(bstring)";
 else
  s="(string)";
 zipfs_printf("\n%s <%s:%d,%d> (%d instruction%s, %d bytes at %p)\n",
 	(f->linedefined==0)?"main":"function",s,
	f->linedefined,f->lastlinedefined,
	S(f->sizecode),f->sizecode*Sizeof(Instruction),VOID(f));
 zipfs_printf("%d%s param%s, %d slot%s, %d upvalue%s, ",
	f->numparams,f->is_vararg?"+":"",SS(f->numparams),
	S(f->maxstacksize),S(f->nups));
 zipfs_printf("%d local%s, %d constant%s, %d function%s\n",
	S(f->sizelocvars),S(f->sizek),S(f->sizep));
}

static void PrintConstants(const Proto* f)
{
 int i,n=f->sizek;
 zipfs_printf("constants (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  zipfs_printf("\t%d\t",i+1);
  PrintConstant(f,i);
  zipfs_printf("\n");
 }
}

static void PrintLocals(const Proto* f)
{
 int i,n=f->sizelocvars;
 zipfs_printf("locals (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  zipfs_printf("\t%d\t%s\t%d\t%d\n",
  i,getstr(f->locvars[i].varname),f->locvars[i].startpc+1,f->locvars[i].endpc+1);
 }
}

static void PrintUpvalues(const Proto* f)
{
 int i,n=f->sizeupvalues;
 zipfs_printf("upvalues (%d) for %p:\n",n,VOID(f));
 if (f->upvalues==NULL) return;
 for (i=0; i<n; i++)
 {
  zipfs_printf("\t%d\t%s\n",i,getstr(f->upvalues[i]));
 }
}

void PrintFunction(const Proto* f, int full)
{
 int i,n=f->sizep;
 PrintHeader(f);
 PrintCode(f);
 if (full)
 {
  PrintConstants(f);
  PrintLocals(f);
  PrintUpvalues(f);
 }
 for (i=0; i<n; i++) PrintFunction(f->p[i],full);
}
