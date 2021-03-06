/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab *symtab;
extern Token *currentToken;

Object *lookupObject(char *name)
{
  // DONE
  Scope *scope = symtab->currentScope;
  Object *obj;
  while (scope)
  {
    obj = findObject(scope->objList, name);
    if (obj) // if found then return that obj
      return obj;
    scope = scope->outer; // if not found enter outer scope
  }
  // Find in global object list
  obj = findObject(symtab->globalObjectList, name);
  if (obj)
    return obj;
  return NULL;
}

void checkFreshIdent(char *name)
{
  // DONE
  // chi tim trong pham vi hien tai
  Scope *scope = symtab->currentScope;
  Object *obj = findObject(scope->objList, currentToken->string);
  if (obj != NULL)
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
}

Object *checkDeclaredIdent(char *name)
{
  // DONE
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }

  return obj;
}

Object *checkDeclaredConstant(char *name)
{
  // DONE
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_CONSTANT)
  {
    error(ERR_INVALID_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredType(char *name)
{
  // DONE
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_TYPE)
  {
    error(ERR_INVALID_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredVariable(char *name)
{
  // DONE
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_VARIABLE)
  {
    error(ERR_INVALID_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredFunction(char *name)
{
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_FUNCTION)
  {
    error(ERR_INVALID_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredProcedure(char *name)
{
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_PROCEDURE)
  {
    error(ERR_INVALID_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredLValueIdent(char *name)
{
  Object *obj = lookupObject(currentToken->string);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  switch (obj->kind)
  {
  case OBJ_VARIABLE:
  case OBJ_PARAMETER:
    break;
  case OBJ_FUNCTION:
    if (obj != symtab->currentScope->owner)
    {
      error(ERR_INVALID_IDENT, currentToken->lineNo, currentToken->colNo);
    }
    break;
  default:
    break;
  }
  return obj;
}
