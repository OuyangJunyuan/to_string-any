//
// Created by ou on 2021/8/10.
//
/**
 * @brief:
 * 宏函数的 参数规则：
 * 0. 没有类型的概念，只是文本替换与符号生成
 * 1. 不同参数由逗号 ， 分隔
 * 2. 参数可以是空 FOO(,,) 参数个数3
 * 3. 一个参数内的括号必须匹配，然后不能有逗号
 * @example:
 * Foo(map<string,int>) -> 两个参数 map<string, // int>
 *
 * @brief:
 * 解决方法: 利用括号匹配要求比不允许出现逗号的优先级高
 * #define PP_REMOVE_PARENS(T) PP_REMOVE_PARENS_IMPL T
 * #define PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__
 * #define Foo(T) PP_REMOVE_PARENS(T)
 * Foo((map<string,int>)) -> 一个参数 (map<string,int>) 再去除括号 得到 map<string,int>
 *
 *
 * @brief:
 * 符号拼接操作 ## : 将宏参数拼接成符号，然后对拼接后的结果进行再次扫描展开。
 * 然而如果 一个宏参数 被用于 拼接[##]或获取字面量[#]，则在拼接前不会被展开。
 * 即展开顺序为：https://zhuanlan.zhihu.com/p/125062325
 * 1. 展开 # 操作符
 * 2. 展开 ## 操作符
 * 3. 完全展开 宏参数 (预扫描)
 * 4. 展开的宏参数代入替换
 * 5. 对替换的结果进行整体扫描与展开 （后扫描）
 * 预扫描与后扫描都是为了展开宏并判断是否可以进行其他宏调用
 *
 * @note:
 * 预扫描前后的参数个数都必须匹配
 * #define Foo(T1,T2)
 * Foo(int PP_COMMA int) // X 预扫描前 参数个数2个(过少)
 * Foo(int , PP_COMMA)   // X 预扫描后 参数个数3个(过多)
 *
 * @example :
 * #define FOO(SYMBOL) foo_ ## SYMBOL
 * #define BAR() bar
 * FOO(bar)     -> foo_bar
 * FOO(BAR())   -> foo_BAR() 因为BAR()在被拼接前不会被展开优先级2>3
 *
 * @brief: 解决方法 即延迟拼接(或获取字面量)
 * 先通过外层辅助宏 把所有宏参数先展开，再丢进真正的拼接宏内
 * #define FOO1(SYMBOL) FOO(SYMBOL)
 * #define FOO(SYMBOL) foo_ ## SYMBOL
 * #define BAR() bar
 * FOO1(BAR()) -> FOO(bar) -> foo_bar
 */


/***
 * @brief: 可变长宏
 * 1. #__VA_ARGS__ 实现 逗号+空格分隔的参数字面量
 * 2. 允许传递 空参数，即 __VA_ARGS__ 替换为空
 * 3. , ## __VA_ARGS__ 如果不传变成参数则省略逗号。
 * 4. C++20推出 __VA_OPT__ 标识符。如果参数是空参数则不展开该符号
 * @example:
 * #define log(format, ...) printf("LOG: " format, ## __VA_ARGS__)
 * 3. log("hello world")   -> printf("LOG: hello world");    // 不传递可变参数
 * 3. log("hello world", ) -> printf("LOG: hello world", );  // 传递空可变参数
 *
 * #define log(format, ...) printf("LOG: " format __VA_OPT__(,) __VA_ARGS__)
 * 4. log("hello world")    -> printf("LOG: hello world")
 * 4. log("hello world", )  -> printf("LOG: hello world");
 */


#ifndef TEST_PREPROCESS_REPEAT_H
#define TEST_PREPROCESS_REPEAT_H

/***
 * @brief: for increase 1
 */
#define PP_INC_0                                1
#define PP_INC_1                                2
#define PP_INC_2                                3
#define PP_INC_3                                4
#define PP_INC_4                                5
#define PP_INC_5                                6
#define PP_INC_6                                7
#define PP_INC_7                                8
#define PP_INC_8                                9
#define PP_INC_9                                10
#define PP_INC_10                               11
#define PP_INC_11                               12
#define PP_INC_12                               13
#define PP_INC_13                               14
#define PP_INC_14                               15
#define PP_INC_15                               16
#define PP_INC_16                               17
#define PP_INC_17                               18
#define PP_INC_18                               19
#define PP_INC_19                               20
#define PP_INC_20                               21
#define PP_INC_21                               22
#define PP_INC_22                               23
#define PP_INC_23                               24
#define PP_INC_24                               25
#define PP_INC_25                               26
#define PP_INC_26                               27
#define PP_INC_27                               28
#define PP_INC_28                               29
#define PP_INC_29                               30
#define PP_INC_30                               31
#define PP_INC_31                               32

/***
 * @brief: for decrease 1
 */
#define PP_DEC_1                                0
#define PP_DEC_2                                1
#define PP_DEC_3                                2
#define PP_DEC_4                                3
#define PP_DEC_5                                4
#define PP_DEC_6                                5
#define PP_DEC_7                                6
#define PP_DEC_8                                7
#define PP_DEC_9                                8
#define PP_DEC_10                               9
#define PP_DEC_11                               10
#define PP_DEC_12                               11
#define PP_DEC_13                               12
#define PP_DEC_14                               13
#define PP_DEC_15                               14
#define PP_DEC_16                               15
#define PP_DEC_17                               16
#define PP_DEC_18                               17
#define PP_DEC_19                               18
#define PP_DEC_20                               19
#define PP_DEC_21                               20
#define PP_DEC_22                               21
#define PP_DEC_23                               22
#define PP_DEC_24                               23
#define PP_DEC_25                               24
#define PP_DEC_26                               25
#define PP_DEC_27                               26
#define PP_DEC_28                               27
#define PP_DEC_29                               28
#define PP_DEC_30                               29
#define PP_DEC_31                               30
#define PP_DEC_32                               31

/***
 * @brief: for boolean operation
 */
#define PP_BOOL_0                               0
#define PP_BOOL_1                               1
#define PP_BOOL_2                               1
#define PP_BOOL_3                               1
#define PP_BOOL_4                               1
#define PP_BOOL_5                               1
#define PP_BOOL_6                               1
#define PP_BOOL_7                               1
#define PP_BOOL_8                               1
#define PP_BOOL_9                               1
#define PP_BOOL_10                              1
#define PP_BOOL_11                              1
#define PP_BOOL_12                              1
#define PP_BOOL_13                              1
#define PP_BOOL_14                              1
#define PP_BOOL_15                              1
#define PP_BOOL_16                              1
#define PP_BOOL_17                              1
#define PP_BOOL_18                              1
#define PP_BOOL_19                              1
#define PP_BOOL_20                              1
#define PP_BOOL_21                              1
#define PP_BOOL_22                              1
#define PP_BOOL_23                              1
#define PP_BOOL_24                              1
#define PP_BOOL_25                              1
#define PP_BOOL_26                              1
#define PP_BOOL_27                              1
#define PP_BOOL_28                              1
#define PP_BOOL_29                              1
#define PP_BOOL_30                              1
#define PP_BOOL_31                              1

#define PP_NOT_0                                1
#define PP_NOT_1                                0

#define PP_AND_00                               0
#define PP_AND_01                               0
#define PP_AND_10                               0
#define PP_AND_11                               1

#define PP_IF_1(THEN, ELSE)                     THEN
#define PP_IF_0(THEN, ELSE)                     ELSE

/***
 * @brief: for giving n-th args
 */
#define PP_GET_N_0(e0, ...) e0
#define PP_GET_N_1(e0, e1, ...) e1
#define PP_GET_N_2(e0, e1, e2, ...) e2
#define PP_GET_N_3(e0, e1, e2, e3, ...) e3
#define PP_GET_N_4(e0, e1, e2, e3, e4, ...) e4
#define PP_GET_N_5(e0, e1, e2, e3, e4, e5, ...) e5
#define PP_GET_N_6(e0, e1, e2, e3, e4, e5, e6, ...) e6
#define PP_GET_N_7(e0, e1, e2, e3, e4, e5, e6, e7, ...) e7
#define PP_GET_N_8(e0, e1, e2, e3, e4, e5, e6, e7, e8, ...) e8
#define PP_GET_N_9(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ...) e9
#define PP_GET_N_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, ...) e10
#define PP_GET_N_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, ...) e11
#define PP_GET_N_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, ...) e12
#define PP_GET_N_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, ...) e13
#define PP_GET_N_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, ...) e14
#define PP_GET_N_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, ...) e15
#define PP_GET_N_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, ...) e16
#define PP_GET_N_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, ...) e17
#define PP_GET_N_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, ...) e18
#define PP_GET_N_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, ...) e19
#define PP_GET_N_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, ...) e20
#define PP_GET_N_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, ...) e21
#define PP_GET_N_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, ...) e22
#define PP_GET_N_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, ...) e23
#define PP_GET_N_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, ...) e24
#define PP_GET_N_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, ...) e25
#define PP_GET_N_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, ...) e26
#define PP_GET_N_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, ...) e27
#define PP_GET_N_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, ...) e28
#define PP_GET_N_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, ...) e29
#define PP_GET_N_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, ...) e30
#define PP_GET_N_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, ...) e31
#define PP_GET_N_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, ...) e32

/***
 * @brief: for each
 */
#define PP_FOR_EACH_0(F, CTX, IDX, ...)
#define PP_FOR_EACH_1(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)
#define PP_FOR_EACH_2(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_1 (F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_3(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_2( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_4(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_3( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_5(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_4( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_6(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_5( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_7(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_6( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_8(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_7( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_9(F, CTX, IDX, VAR, ...)        F(CTX, IDX, VAR)  PP_FOR_EACH_8( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_10(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_9( F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_11(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_10(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_12(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_11(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_13(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_12(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_14(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_13(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_15(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_14(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_16(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_15(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_17(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_16(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_18(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_17(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_19(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_18(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_20(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_19(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_21(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_20(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_22(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_21(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_23(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_22(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_24(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_23(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_25(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_24(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_26(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_25(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_27(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_26(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_28(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_27(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_29(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_28(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_30(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_29(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_31(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_30(F,CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_32(F, CTX, IDX, VAR, ...)       F(CTX, IDX, VAR)  PP_FOR_EACH_31(F,CTX, PP_INC(IDX), __VA_ARGS__)

/***
 * @brief: for while
 */
#define PP_WHILE_1(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_2, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_2(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_3, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_3(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_4, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_4(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_5, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_5(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_6, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_6(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_7, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_7(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_8, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_8(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_9, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_9(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_10, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_10(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_11, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_11(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_12, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_12(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_13, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_13(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_14, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_14(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_15, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_15(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_16, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_16(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_17, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_17(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_18, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_18(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_19, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_19(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_20, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_20(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_21, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_21(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_22, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_22(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_23, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_23(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_24, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_24(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_25, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_25(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_26, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_26(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_27, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_27(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_28, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_28(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_29, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_29(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_30, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_30(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_31, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_31(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_32, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_32(PRED, OP, VAL)   PP_IF(PRED(VAL), PP_WHILE_33, VAL PP_EMPTY_V)(PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_33(PRED, OP, VAL)

#define PP_EAT_HEAD_1(_1,...)        __VA_ARGS__
#endif //TEST_PREPROCESS_REPEAT_H
