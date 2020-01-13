#pragma once

#include <iostream>

/**
 * From StackOverflow with love
 */

#ifndef NDEBUG

#define AssertMsg(Expr, Msg) \
    _assert_with_msg(#Expr, Expr, __FILE__, __LINE__, Msg)
#define AssertNoMsg(Expr) \
    _assert_without_msg(#Expr, Expr, __FILE__, __LINE__)

#else
#   define AssertMsg(Expr, Msg) ;
#   define AssertNoMsg(Expr) ;
#endif

#define GET_MACRO(_1,_2,NAME,...) NAME
#define Assert(...) GET_MACRO(__VA_ARGS__, AssertMsg, AssertNoMsg)(__VA_ARGS__)

#define AssertAlways() \
    Assert(0)

#define AssertAlwaysMsg(msg) \
    AssertMsg(0, msg)

#define AssertNotImplemented() \
    AssertMsg(0, "Not implemented")


void _assert_with_msg(const char* expr_str, bool expr, const char* file, int line, const char* msg);
void _assert_without_msg(const char* expr_str, bool expr, const char* file, int line);