//
// Created by ou on 2021/8/6.
//

#ifndef TEST_PREPROCESS_H
#define TEST_PREPROCESS_H

#include "preprocess_repeat.h"
/***
 * @brief: 特殊符号, 用于延迟展开
 */
#define PP_COMMA()                              ,
#define PP_COMMA_V(...)                         ,
#define PP_LPAREN()                             (
#define PP_RPAREN()                             )
#define PP_EMPTY()
#define PP_EMPTY_V(...)

/***
 * @brief: 连接字符
 *
 */
#define PP_CONCAT(A, B)                         PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B)                    A##B

/***
 * @brief: 吞掉参数
 */
#define PP_EAT(...)

/***
 * @brief: 字面量字符串获取
 */
#define PP_STR(C)                               PP_STR_IMPL(C)
#define PP_STR_IMPL(C)                          #C

/***
 * @brief: 参数转发
 */
#define PP_FORWARD(...)                         __VA_ARGS__

/***
 * @brief: 移除括号
 */
#define PP_REMOVE_PARENS(T)                     PP_REMOVE_PARENS_IMPL T
#define PP_REMOVE_PARENS_IMPL(...)              __VA_ARGS__

/***
 * @brief: 自增1
 */
#define PP_INC(N)                               PP_CONCAT(PP_INC_, N)

/***
 * @brief: 自减1
 */
#define PP_DEC(N)                               PP_CONCAT(PP_DEC_, N)

/***
 * @brief: 布尔操作
 */
#define PP_BOOL(N)                              PP_CONCAT(PP_BOOL_,N)

#define PP_NOT(N)                               PP_CONCAT(PP_NOT_, PP_BOOL(N))
#define PP_NOT_(N)                              PP_CONCAT(PP_NOT_, N)

#define PP_AND(A, B)                            PP_CONCAT(PP_AND_,PP_CONCAT(A,B))
#define PP_AND_(A, B)                           PP_CONCAT(PP_AND_,PP_CONCAT(PP_BOOL(A),PP_BOOL(B)))

/***
 * @brief: 分支 (注意惰性求值的使用，因为预扫描时要求THEN，ELSE都会被扫描，必须存在该符号或可调用)
 */
#define PP_IF(PRED, THEN, ELSE)                 PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)
#define PP_IF_CALL(PRED, THEN, ELSE, ...)       PP_IF(PRED,THEN,ELSE)(__VA_ARGS__)
#define PP_IF_TUPLE_N(N, T, THEN, ELSE)         PP_IF(PP_EQUAL(N, PP_GET_TUPLE_COUNT(T)), THEN, ELSE)

/***
 * @brief: N为真则展开为逗号 (延迟调用PP_COMMA 否则第二次扫描展开PP_COMMA() 导致PP_IF参数个数出错)
 */
#define PP_COMMA_IF(N)                          PP_IF(N, PP_COMMA, PP_EMPTY)()

/***
 * @brief: 获取参数列表中的第 i 个值
 */
#define PP_GET_N(N, ...)                        PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)

/***
 * @brief: 获取元组的第 i 个元素
 */
#define PP_GET_TUPLE(N, T)                      PP_GET_N(N, PP_REMOVE_PARENS(T))
#define PP_GET_TUPLE2D(N, M, T)                 PP_GET_TUPLE(M,PP_GET_TUPLE(N,T))
/***
 * @brief: 判断可变参数 是否为空
 */
#define PP_IS_EMPTY(...)                        PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(__VA_ARGS__)), PP_NOT(PP_HAS_COMMA(__VA_ARGS__()))),   PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)),PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__())))

/***
 * @brief: 判断可变参数是否 包含逗号
 */
#define PP_HAS_COMMA(...)                       PP_GET_N_31(__VA_ARGS__, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 0)

/***
 * @brief: 实现C++20的 __VA_OPT__(S,...)， 当可变参数不为0时 展开为 符号S
 */
#define PP_VA_OPT(OPT, ...)                     PP_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)),OPT,PP_EMPTY())
#define PP_VA_OPT_COMMA(...)                    PP_COMMA_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)))

/***
* @brief: 获取可变参数个数
*/
#define PP_GET_ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, n, ...) n
#define PP_GET_ARG_COUNT(...)                   PP_GET_ARG_COUNT_IMPL(__VA_ARGS__,32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1)
#define PP_GET_TUPLE_COUNT(T)                   PP_GET_ARG_COUNT(PP_TRY_REMOVE_PARENS(T))

/***
 * @brief: 对可变参数中的每个值调用一次 F(CTX,IDX,VAR)
 */
#define PP_FOR_EACH(F, CTX, ...)                PP_CONCAT(PP_FOR_EACH_,PP_GET_ARG_COUNT(__VA_ARGS__))(F, CTX, 0, __VA_ARGS__)

/***
 * @brief: 用于判断是否是 符号 symbol，需要预先 #defeine PREFIX_SYMBOL 然后 PP_IS_SYMBOL(PREFIX_,SYMBOL)
 */
#define PP_IS_SYMBOL(PREFIX, SYMBOL)            PP_IS_EMPTY(PP_CONCAT(PREFIX, SYMBOL))

/***
 * @brief: 判断参数是否 被括号包围
 */
#define PP_IS_PARENS(SYMBOL)                    PP_IS_EMPTY(PP_EMPTY_V SYMBOL)

/***
 * @brief: 尝试去除包围参数的括号
 */
#define PP_TRY_REMOVE_PARENS(T)                PP_IF(PP_IS_PARENS(T), PP_REMOVE_PARENS, PP_FORWARD)(T)

/***
 * @brief: WHILE 循环，每次循环先执行判断PRED(VAL) == 1,然后更新VAL = OP(VAL)，再进行下一次循环。
 */
#define PP_WHILE                    PP_WHILE_1

/***
 * @brief: 四则运算
 */
#define PP_ADD(X, Y)                            PP_GET_TUPLE(0, PP_WHILE(PP_ADD_P, PP_ADD_O, (X, Y)))
#define PP_SUB(X, Y)                            PP_GET_TUPLE(0, PP_WHILE(PP_SUB_P, PP_SUB_O, (X, Y)))
#define PP_MUL(X, Y)                            PP_GET_TUPLE(0, PP_WHILE(PP_MUL_P, PP_MUL_O, (0, X, Y)))
#define PP_DIV(X, Y)                            PP_GET_TUPLE(0, PP_DIV_BASE(X, Y))
#define PP_MOD(X, Y)                            PP_GET_TUPLE(1, PP_DIV_BASE(X, Y))

#define PP_EQUAL(X, Y)                          PP_FORWARD(PP_EQUAL_IMPL PP_CMP(X, Y))
#define PP_LESS(X, Y)                           PP_IDENTITY(PP_LESS_IMPL PP_CMP(X, Y))
#define PP_MIN(X, Y)                            PP_IF(PP_LESS(X, Y), X, Y)
#define PP_MAX(X, Y)                            PP_IF(PP_LESS(X, Y), Y, X)


#define PP_ADD_P(V)                             PP_GET_TUPLE(1, V)
#define PP_ADD_O(V)                             (PP_INC(PP_GET_TUPLE(0, V)), PP_DEC(PP_GET_TUPLE(1, V)))

#define PP_SUB_P(V)                             PP_GET_TUPLE(1, V)
#define PP_SUB_O(V)                             (PP_DEC(PP_GET_TUPLE(0, V)), PP_DEC(PP_GET_TUPLE(1, V)))

#define PP_MUL_P(V)                             PP_GET_TUPLE(2, V)
#define PP_MUL_O(V)                             (PP_ADD(PP_GET_TUPLE(0, V), PP_GET_TUPLE(1, V)), PP_GET_TUPLE(1, V),PP_DEC(PP_GET_TUPLE(2, V)))

#define PP_CMP(X, Y)                            PP_WHILE(PP_CMP_P, PP_CMP_O, (X, Y))
#define PP_CMP_P(V)                             PP_AND(PP_BOOL(PP_GET_TUPLE(0, V)), PP_BOOL(PP_GET_TUPLE(1, V)))
#define PP_CMP_O(V)                             (PP_DEC(PP_GET_TUPLE(0, V)), PP_DEC(PP_GET_TUPLE(1, V)))

#define PP_EQUAL_IMPL(RX, RY)                   PP_AND(PP_NOT(PP_BOOL(RX)), PP_NOT(PP_BOOL(RY)))

#define PP_LESS_IMPL(RX, RY)                    PP_AND(PP_NOT(PP_BOOL(RX)), PP_BOOL(RY))

#define PP_DIV_BASE(X, Y)                       PP_WHILE(PP_DIV_BASE_P, PP_DIV_BASE_O, (0, X, Y))
#define PP_DIV_BASE_P(V)                        PP_NOT(PP_LESS(PP_GET_TUPLE(1, V), PP_GET_TUPLE(2, V)))  // X >= Y
#define PP_DIV_BASE_O(V)                        (PP_INC(PP_GET_TUPLE(0, V)), PP_SUB(PP_GET_TUPLE(1, V), PP_GET_TUPLE(2, V)), PP_GET_TUPLE(2, V))


#endif //TEST_PREPROCESS_H
