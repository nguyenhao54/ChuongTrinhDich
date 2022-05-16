/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object *obj);
void freeScope(Scope *scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab *symtab;
Type *intType;
Type *charType;

/******************* Type utilities ******************************/

Type *makeIntType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type *makeCharType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type *makeArrayType(int arraySize, Type *elementType)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type *duplicateType(Type *type)
{
  Type *typedup = (Type *)malloc(sizeof(Type));
  typedup->typeClass = type->typeClass;
  if (type->typeClass == TP_ARRAY)
  {
    typedup->arraySize = type->arraySize;
    typedup->elementType = duplicateType(type->elementType);
  }

  return typedup;
}

int compareType(Type *type1, Type *type2)
{
  // TODO
  if (type1->typeClass == type2->typeClass)
  {
    if (type1->typeClass == TP_ARRAY)
    {
      if (type1->arraySize == type2->arraySize)
        return compareType(type1->elementType, type2->elementType);
      else
        return 0;
    }
    else
      return 1;
  }
  else
    return 0;
}

void freeType(Type *type)
{
  if (type->elementType == TP_INT || type->elementType == TP_CHAR)
  {
    free(type);
  }
  else // type->elementType == TP_ARRAY
  {
    freeType(type->elementType);
    freeType(type);
  }
}

/******************* Constant utility ******************************/

ConstantValue *makeIntConstant(int i)
{
  ConstantValue *constValue = (ConstantValue *)malloc(sizeof(ConstantValue));
  constValue->type = TP_INT;
  constValue->intValue = i;
  return constValue;
}

ConstantValue *makeCharConstant(char ch)
{
  ConstantValue *constValue = (ConstantValue *)malloc(sizeof(ConstantValue));
  constValue->type = TP_CHAR;
  constValue->charValue = ch;
  return constValue;
}

ConstantValue *duplicateConstantValue(ConstantValue *v)
{
  ConstantValue *constValue = (ConstantValue *)malloc(sizeof(ConstantValue));
  if (v->type == TP_CHAR)
  {
    constValue->type = TP_CHAR;
    constValue->charValue = v->charValue;
  }
  else
  {
    constValue->type = TP_INT;
    constValue->intValue = v->intValue;
  }
  return constValue;
}

/******************* Object utilities ******************************/

Scope *createScope(Object *owner, Scope *outer)
{
  Scope *scope = (Scope *)malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object *createProgramObject(char *programName)
{
  Object *program = (Object *)malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes *)malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program, NULL);
  symtab->program = program;

  return program;
}

Object *createConstantObject(char *name)
{
  Object *constObj = (Object *)malloc(sizeof(Object));
  strcpy(constObj->name, name);
  constObj->kind = OBJ_CONSTANT;
  constObj->constAttrs = (ConstantAttributes *)malloc(sizeof(ConstantAttributes));

  return constObj;
}

Object *createTypeObject(char *name)
{
  Object *typeObj = (Object *)malloc(sizeof(Object));
  strcpy(typeObj->name, name);
  typeObj->kind = OBJ_TYPE;
  typeObj->typeAttrs = (TypeAttributes *)malloc(sizeof(TypeAttributes));
  return typeObj;
}

Object *createVariableObject(char *name)
{
  Object *varObj = (Object *)malloc(sizeof(Object));
  strcpy(varObj->name, name);
  varObj->kind = OBJ_VARIABLE;
  varObj->varAttrs = (VariableAttributes *)malloc(sizeof(VariableAttributes));
  varObj->varAttrs->scope = createScope(varObj, symtab->currentScope);
  return varObj;
}

Object *createFunctionObject(char *name)
{
  Object *funcObj = (Object *)malloc(sizeof(Object));
  strcpy(funcObj->name, name);
  funcObj->kind = OBJ_FUNCTION;
  funcObj->funcAttrs = (FunctionAttributes *)malloc(sizeof(FunctionAttributes));
  funcObj->funcAttrs->paramList = NULL;
  funcObj->funcAttrs->scope = createScope(funcObj, symtab->currentScope);
  return funcObj;
}

Object *createProcedureObject(char *name)
{
  Object *procObj = (Object *)malloc(sizeof(Object));
  strcpy(procObj->name, name);
  procObj->kind = OBJ_PROCEDURE;
  procObj->procAttrs = (ProcedureAttributes *)malloc(sizeof(ProcedureAttributes));
  procObj->procAttrs->paramList = NULL;
  procObj->procAttrs->scope = createScope(procObj, symtab->currentScope);
  return procObj;
}

Object *createParameterObject(char *name, enum ParamKind kind, Object *owner)
{
  Object *paramObj = (Object *)malloc(sizeof(Object));
  strcpy(paramObj->name, name);
  paramObj->kind = OBJ_PARAMETER;
  paramObj->paramAttrs = (ParameterAttributes *)malloc(sizeof(ParameterAttributes));
  return paramObj;
}

void freeObject(Object *obj)
{
  // TODO
  switch (obj->kind)
  {
  case OBJ_VARIABLE:
    freeType(obj->varAttrs->type);
    freeScope(obj->varAttrs->scope);
    free(obj->varAttrs);
    break;
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    break;
  case OBJ_TYPE:
    freeType(obj->typeAttrs->actualType);
    free(obj->typeAttrs);
    break;
  case OBJ_FUNCTION:
    freeType(obj->funcAttrs->returnType);
    freeScope(obj->funcAttrs->scope);
    freeReferenceList(obj->funcAttrs->paramList);
    free(obj->funcAttrs);
    break;
  case OBJ_PROCEDURE:
    freeReferenceList(obj->procAttrs->paramList);
    freeScope(obj->procAttrs->paramList);
    free(obj->procAttrs);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    free(obj->paramAttrs);
    break;
  case OBJ_PROGRAM:
    freeScope(obj->progAttrs->scope);
    free(obj->progAttrs);
  default:
    break;
  }
  free(obj);
}
void freeScope(Scope *scope)
{
  // TODO
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList)
{
  // TODO
  ObjectNode *list = objList;

  while (list != NULL)
  {
    ObjectNode *node = list;
    list = list->next;
    freeObject(node->object);
    free(node);
  }
}

void freeReferenceList(ObjectNode *objList)
{
  // TODO
  ObjectNode *list = objList;

  while (list != NULL)
  {
    ObjectNode *node = list;
    list = list->next;
    free(node);
  }
}

void addObject(ObjectNode **objList, Object *obj)
{
  ObjectNode *node = (ObjectNode *)malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else
  {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object *findObject(ObjectNode *objList, char *name)
{
  // TODO
  while (objList != NULL)
  {
    if (strcmp(objList->object->name, name))
    {
      return objList->object;
    }
    else
    {
      objList = objList->next;
    }
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void)
{
  Object *obj;
  Object *param;

  symtab = (SymTab *)malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void)
{
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope *scope)
{
  symtab->currentScope = scope;
}

void exitBlock(void)
{
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object *obj)
{
  if (obj->kind == OBJ_PARAMETER)
  {
    Object *owner = symtab->currentScope->owner;
    switch (owner->kind)
    {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  addObject(&(symtab->currentScope->objList), obj);
}
