//
// Created by ou on 2021/8/6.
//

#ifndef TEST_REFL_H
#define TEST_REFL_H

#include <iostream>
#include <tuple>
/***
 * some utility macro
 */
#define EAT(...)

#define STRIP(x) EAT x                                      // STRIP((double) x) => EAT(double) x => x

#define PARE(...) __VA_ARGS__
#define PAIR(x) PARE x                                      // PAIR((double) x) => PARE(double) x => double x

#define STR(x) #x
#define STRING(x) STR(x)                                    // STRING(Point) => STR(Point) => "Point"

#define CONCATE(x, y) x ## y
#define PASTE(x, y) CONCATE(x, y)                           // PASTE(set_,x) => CONCATE(set_,x) => set_x

// W_MACRO_REMOVEPAREN((a)) => a
#define W_MACRO_MSVC_EXPAND(...) __VA_ARGS__
#define W_MACRO_DELAY(X, ...) W_MACRO_MSVC_EXPAND(X(__VA_ARGS__))
#define W_MACRO_DELAY2(X, ...) W_MACRO_MSVC_EXPAND(X(__VA_ARGS__))
#define W_MACRO_TAIL(A, ...) __VA_ARGS__

#define W_MACRO_REMOVEPAREN(A) W_MACRO_DELAY(W_MACRO_REMOVEPAREN2, W_MACRO_REMOVEPAREN_HELPER A)
#define W_MACRO_REMOVEPAREN2(...) W_MACRO_DELAY2(W_MACRO_TAIL, W_MACRO_REMOVEPAREN_HELPER_##__VA_ARGS__)
#define W_MACRO_REMOVEPAREN_HELPER(...) _ , __VA_ARGS__
#define W_MACRO_REMOVEPAREN_HELPER_W_MACRO_REMOVEPAREN_HELPER ,
/***
 * @note: for loop macro
 * @example: FOR_EACH_2(func,0,x,y) => func(0,x);func(1,y)
 */

#define FOR_EACH_1(func, i, arg)            func(i, arg);
#define FOR_EACH_2(func, i, arg, ...)       func(i, arg); FOR_EACH_1(func, i + 1, __VA_ARGS__)
#define FOR_EACH_3(func, i, arg, ...)       func(i, arg); FOR_EACH_2(func, i + 1, __VA_ARGS__)
#define FOR_EACH_4(func, i, arg, ...)       func(i, arg); FOR_EACH_3(func, i + 1, __VA_ARGS__)
#define FOR_EACH_5(func, i, arg, ...)       func(i, arg); FOR_EACH_4(func, i + 1, __VA_ARGS__)
#define FOR_EACH_6(func, i, arg, ...)       func(i, arg); FOR_EACH_5(func, i + 1, __VA_ARGS__)
#define FOR_EACH_7(func, i, arg, ...)       func(i, arg); FOR_EACH_6(func, i + 1, __VA_ARGS__)
#define FOR_EACH_8(func, i, arg, ...)       func(i, arg); FOR_EACH_7(func, i + 1, __VA_ARGS__)
#define FOR_EACH_9(func, i, arg, ...)       func(i, arg); FOR_EACH_8(func, i + 1, __VA_ARGS__)
#define FOR_EACH_10(func, i, arg, ...)      func(i, arg); FOR_EACH_9(func, i + 1, __VA_ARGS__)
#define FOR_EACH_11(func, i, arg, ...)      func(i, arg); FOR_EACH_10(func, i + 1, __VA_ARGS__)
#define FOR_EACH_12(func, i, arg, ...)      func(i, arg); FOR_EACH_11(func, i + 1, __VA_ARGS__)
#define FOR_EACH_13(func, i, arg, ...)      func(i, arg); FOR_EACH_12(func, i + 1, __VA_ARGS__)
#define FOR_EACH_14(func, i, arg, ...)      func(i, arg); FOR_EACH_13(func, i + 1, __VA_ARGS__)
#define FOR_EACH_15(func, i, arg, ...)      func(i, arg); FOR_EACH_14(func, i + 1, __VA_ARGS__)
#define FOR_EACH_16(func, i, arg, ...)      func(i, arg); FOR_EACH_15(func, i + 1, __VA_ARGS__)
#define FOR_EACH_17(func, i, arg, ...)      func(i, arg); FOR_EACH_16(func, i + 1, __VA_ARGS__)
#define FOR_EACH_18(func, i, arg, ...)      func(i, arg); FOR_EACH_17(func, i + 1, __VA_ARGS__)
#define FOR_EACH_19(func, i, arg, ...)      func(i, arg); FOR_EACH_18(func, i + 1, __VA_ARGS__)
#define FOR_EACH_20(func, i, arg, ...)      func(i, arg); FOR_EACH_19(func, i + 1, __VA_ARGS__)
#define FOR_EACH_21(func, i, arg, ...)      func(i, arg); FOR_EACH_20(func, i + 1, __VA_ARGS__)
#define FOR_EACH_22(func, i, arg, ...)      func(i, arg); FOR_EACH_21(func, i + 1, __VA_ARGS__)
#define FOR_EACH_23(func, i, arg, ...)      func(i, arg); FOR_EACH_22(func, i + 1, __VA_ARGS__)
#define FOR_EACH_24(func, i, arg, ...)      func(i, arg); FOR_EACH_23(func, i + 1, __VA_ARGS__)
#define FOR_EACH_25(func, i, arg, ...)      func(i, arg); FOR_EACH_24(func, i + 1, __VA_ARGS__)
#define FOR_EACH_26(func, i, arg, ...)      func(i, arg); FOR_EACH_25(func, i + 1, __VA_ARGS__)
#define FOR_EACH_27(func, i, arg, ...)      func(i, arg); FOR_EACH_26(func, i + 1, __VA_ARGS__)
#define FOR_EACH_28(func, i, arg, ...)      func(i, arg); FOR_EACH_27(func, i + 1, __VA_ARGS__)
#define FOR_EACH_29(func, i, arg, ...)      func(i, arg); FOR_EACH_28(func, i + 1, __VA_ARGS__)
#define FOR_EACH_30(func, i, arg, ...)      func(i, arg); FOR_EACH_29(func, i + 1, __VA_ARGS__)
#define FOR_EACH_31(func, i, arg, ...)      func(i, arg); FOR_EACH_30(func, i + 1, __VA_ARGS__)
#define FOR_EACH_32(func, i, arg, ...)      func(i, arg); FOR_EACH_31(func, i + 1, __VA_ARGS__)

/***
* @note: get the numbers of args
* @example: GET_ARG_COUNT(a,b) => GET_ARG_COUNT_(a,b,64,...,1) => 2
*/
#define GET_ARG_COUNT_(\
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, \
    _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, \
    _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, n, ...) n

#define GET_ARG_COUNT(...) GET_ARG_COUNT_(__VA_ARGS__,                  \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, \
        48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
        16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1)


namespace refl {
    namespace trait {
        /***
         * @brief:
         * @example: is_reflectable_v<int> => false;
         */
        template<typename T>
        struct is_reflectable {
        private:
            template<typename T1, typename = decltype(T1::template FIELD<T *, 0>::name)>
            static std::true_type is_reflectable_impl(int);

            template<typename>
            static std::false_type is_reflectable_impl(...);

        public:
            using type = decltype(is_reflectable_impl<T>(0));
        };

        template<typename T>
        inline constexpr bool is_reflectable_v{is_reflectable<T>::type::value};


        template<typename T>
        struct is_container {
        private:
            template<typename C, typename = decltype((std::begin(std::declval<C>()), std::end(
                    std::declval<C>()), std::size(std::declval<C>())))>
            static std::true_type is_container_impl(int);

            template<typename>
            static std::false_type is_container_impl(...) {};

        public:
            using type = decltype(is_container_impl<T>(0));
        };

        template<typename T>
        inline constexpr bool is_container_v{is_container<T>::type::value};


        template<typename C, typename T>
        struct is_ostream_printable {
            template<typename T1, typename = decltype(std::declval<std::basic_ostream<C> &>() << std::declval<T1>())>
            static std::true_type is_ostream_printable_impl(int);

            template<typename T1>
            static std::false_type is_ostream_printable_impl(...);

            using type = decltype(is_ostream_printable_impl<T>(0));
        };

        template<typename CharT, typename T>
        inline constexpr bool is_ostream_printable_v{is_ostream_printable<CharT, T>::type::value};

        template<typename T>
        struct has_attribute {
        private:
            template<typename T1, typename C= decltype(std::tuple_size_v<decltype(T1::attributes)> == 1)>
            static std::true_type has_attribute_impl(int);

            template<typename>
            static std::false_type has_attribute_impl(...);

        public:
            using type = decltype(has_attribute_impl<T>(0));
        };


        template<typename T>
        inline constexpr bool has_attribute_v{has_attribute<T>::type::value};
    };
    namespace detail {
        template<typename CharT>
        using OS = std::basic_ostream<CharT>;

        template<typename _T, typename F, size_t... Is>
        inline constexpr void for_each(_T &&obj, F &&f, std::index_sequence<Is...>) {
            using T = std::decay_t<_T>;
            (
                    f(typename T::template FIELD<T, Is>(obj),
                      typename T::template FIELD<T, Is>(obj).name(),
                      typename T::template FIELD<T, Is>(obj).value(),
                      Is, obj._field_count_),
                    ...
            );
        }

        template<typename T, typename F>
        inline constexpr void for_each(T &&obj, F &&f) {
            for_each(std::forward<T>(obj), std::forward<F>(f),
                     std::make_index_sequence<std::decay_t<T>::_field_count_>{});
        }

        struct Cfg {
            bool compact = false;
            size_t idx = 0;
            size_t cnt = 1;
            size_t depth = 0;
        };

        template<typename CharT>
        inline void indent(OS<CharT> &os, const Cfg &cfg) {
            if (not cfg.compact)
                for (int i = 0; i < cfg.depth; i++)
                    os << "    ";
        }

        template<typename CharT>
        inline void common(OS<CharT> &os, const Cfg &cfg) { os << (cfg.idx + 1 == cfg.cnt ? "" : ","); }

        template<typename CharT, typename T>
        void serialize_impl(OS<CharT> &, T &&, const char *, const Cfg &);


        template<typename CharT, typename T>
        void serialize_reflectable(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg) {

            indent(os, cfg);
            os << fieldName << ": {" << (cfg.compact ? ' ' : '\n');
            detail::for_each(obj, [&](auto && metadata, auto &&fieldName, auto &&value, auto &&idx, auto &&cnt) {
                Cfg new_cfg{cfg.compact, idx, cnt, cfg.depth + 1};
                if constexpr (std::tuple_size_v<decltype(metadata.attributes)> == 1) {
                    indent(os, new_cfg);
                    std::get<0>(metadata.attributes)(os, value, fieldName, new_cfg);
                    common(os, new_cfg);
                    os << (not new_cfg.compact || new_cfg.depth == 0 ? '\n' : ' ');
                } else {
                    serialize_impl(os, value, fieldName, new_cfg);
                }
            });
            indent(os, cfg);
            os << "}";
        }

        template<typename CharT, typename T>
        void serialize_container(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": [" << (cfg.compact ? ' ' : '\n');
            int i = 0;
            for (auto &&it:obj) {
                serialize_impl(os, it, std::to_string(i).c_str(), Cfg{cfg.compact, i, std::size(obj), cfg.depth + 1});
                i++;
            }
            indent(os, cfg);
            os << "]";
        }

        template<typename CharT, typename T>
        void serialize_os_print(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": " << obj;
        }

        template<typename CharT, typename T>
        void serialize_not_printable(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": " << "not printable";
        }

        template<typename CharT, typename _T>
        void serialize_impl(std::basic_ostream<CharT> &os, _T &&obj, const char *fieldName, const Cfg &cfg) {

            using T = std::decay_t<_T>;

            if constexpr (std::is_class_v<T> and trait::is_reflectable_v<T>) {
                serialize_reflectable(os, obj, fieldName, cfg);
            } else if constexpr (trait::is_container_v<T>) {
                serialize_container(os, obj, fieldName, cfg);
            } else if constexpr (trait::is_ostream_printable_v<CharT, T>) {
                serialize_os_print(os, obj, fieldName, cfg);
            } else {
                serialize_not_printable(os, obj, fieldName, cfg);
            }
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }
    }
    namespace utils {
        template<typename CharT, typename _T>
        void serialize(std::basic_ostream<CharT> &os, _T &&obj, const char *fieldName = "", bool compact = false) {
            using T = std::decay_t<_T>;
            static_assert(
                    trait::is_reflectable_v<T> || trait::is_container_v<T> || trait::is_ostream_printable_v<CharT, T>,
                    "Type is not reflectable, not a container of reflectable types and does not support operator<< also!");

            detail::serialize_impl(os, obj, fieldName, detail::Cfg{compact, 0, 1, 0});
        }
    }
}

/**
 * @brief: macro to define a reflectable struct
 * @example: STRUCT(cls_name,(type_1)e_1,...,(type_n)e_n)
 */
/*#define FIELD_EACH(i, arg) \
    PAIR(arg);                                \
    template <typename T>                     \
    struct FIELD< T, i > {                    \
        T& obj;                               \
        FIELD(T& obj): obj(obj) {}            \
        decltype(auto) value() {              \
            return (obj.STRIP(arg));          \
        }                                     \
        static auto name() {                  \
            return STRING(STRIP(arg));        \
        }                                     \
    };                                      *//*\
    template <typename T>                     \
    void PASTE(set_,STRIP(arg))(T&& _obj){STRIP(arg)=_obj;}; \
    auto PASTE(get_,STRIP(arg))(){return STRIP(arg);};*//*

#define STRUCT(st, ...)                                                         \
struct st {                                                                     \
    template <typename, size_t> struct FIELD;                              \
    static constexpr size_t _field_count_ = GET_ARG_COUNT(__VA_ARGS__);         \
    PASTE(FOR_EACH_, GET_ARG_COUNT(__VA_ARGS__)) (FIELD_EACH, 0, __VA_ARGS__)   \
    friend ostream & operator <<(std::ostream& os,const st &obj){               \
        refl::utils::serialize(os,const_cast<st&>(obj),name(),false);                 \
        return os;                                                              \
    };                                                                          \
    private:  static char const * name() {  return STRING(st); }        \
};*/


#define FILED_TYPE(TYPE, _2, ...)  W_MACRO_REMOVEPAREN(TYPE)
#define FILED_NAME_(NAME, ...) NAME
#define FILED_NAME(_1, ARG, ...)  FILED_NAME_ ARG
#define FILED_DEFAULT_(NAME, ...) __VA_ARGS__
#define FILED_DEFAULT(_1, ARG, ...)  FILED_DEFAULT_ ARG
#define FILED_ATTRIBUTE(_1, _2, ...) __VA_ARGS__

#define GENERATE_ONE_FILED(i, arg) \
FILED_TYPE  arg  FILED_NAME arg {  FILED_DEFAULT arg};  \
template <typename T>                                   \
struct FIELD< T, i > {                              \
    T& obj;                                         \
    FIELD(T& obj): obj(obj) {}                      \
    decltype(auto) value() {                        \
    return (obj.FILED_NAME arg);                    \
    }                                               \
    static auto name() {                            \
    return STRING(FILED_NAME arg);                  \
    }                              \
    static constexpr auto attributes = make_tuple(FILED_ATTRIBUTE arg);                                   \
};

#define REFL(st, ...) \
struct st{            \
    template <typename, size_t> struct FIELD;\
    static constexpr size_t _field_count_ = GET_ARG_COUNT(__VA_ARGS__);\
    PASTE(FOR_EACH_, GET_ARG_COUNT(__VA_ARGS__)) (GENERATE_ONE_FILED, 0, __VA_ARGS__)\
};

#endif //TEST_REFL_H
