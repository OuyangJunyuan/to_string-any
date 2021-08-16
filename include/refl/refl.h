//
// Created by ou on 2021/8/6.
//

#ifndef TEST_REFL_H
#define TEST_REFL_H

#include "pp/preprocess.h"
#include <utility>
#include <sstream>

namespace reflect::trait {
    PP_MAKE_TRAITS_HAS_XXX(is_reflect, (std::void_t<decltype(U::template member<U *, 0>::name)> *) 0)

    PP_MAKE_TRAITS_HAS_XXX(is_printable, PP_DECLVAL(std::basic_ostream<char>) << PP_DECLVAL(U))

    PP_MAKE_TRAITS_HAS_XXX(is_container, std::begin(PP_DECLVAL(U)), std::end(PP_DECLVAL(U)))

    PP_MAKE_TRAITS_HAS_XXX(is_smart_pointer, *PP_DECLVAL(U), PP_DECLVAL(U).operator->(), PP_DECLVAL(U).get())

    PP_MAKE_TRAITS_HAS_XXX(is_map, PP_DECLVAL(U).key_comp())

    PP_MAKE_TRAITS_CALLABLE_XXX(serialize)

    template<bool>
    struct is_valid : std::false_type {
    };
    template<>
    struct is_valid<true> : std::true_type {
    };
}
namespace reflect::trait {
    template<class T> inline constexpr
    bool is_string_v{std::disjunction<std::is_same<T, std::string>, std::is_same<T, const char *>>::value};

    template<class T>
    inline constexpr bool is_general_pointer_v{std::is_pointer_v<T> && is_smart_pointer_v<T>};

    template<class T>
    inline constexpr bool is_valid_v = is_valid<is_reflect_v<T> || is_printable_v<T> || is_container_v<T>>::value;
}

namespace reflect {
    namespace detail {
        template<typename CHAR_T>
        using OS = std::basic_ostream<CHAR_T>;

        template<typename CHAR_T, typename U>
        void serialize_impl(OS<CHAR_T> &, const U &, const char *,bool, size_t, size_t, size_t);

        template<typename _T, typename F, size_t... Is>
        inline constexpr void for_each(_T &&obj, F &&f, std::index_sequence<Is...>) {
            using T = std::decay_t<_T>;
            (
                    f(typename T::template member<T, Is>(obj),
                      typename T::template member<T, Is>(obj).name(),
                      typename T::template member<T, Is>(obj).value(),
                      Is, obj.member_numbers), ...
            );
        }

        template<typename T, typename F>
        inline constexpr void for_each(T &&obj, F &&f) {
            for_each(std::forward<T>(obj), std::forward<F>(f),
                     std::make_index_sequence<std::decay_t<T>::member_numbers>{});
        }

        template<typename CHAR_T>
        inline void indent(OS<CHAR_T> &os, bool compact = false, size_t idx = 0, size_t cnt = 1, size_t depth = 0) {
            if (not compact) for (int i = 0; i < depth; i++) os << "    ";
        }

        template<typename CHAR_T>
        inline void common(OS<CHAR_T> &os, bool compact = false, size_t idx = 0, size_t cnt = 1, size_t depth = 0) {
            os << (idx + 1 == cnt ? "" : ",");
        }

        template<typename CHAR_T, typename T>
        void serialize_reflectable(OS<CHAR_T> &os, const T &obj, const char *name,
                                   bool compact = false, size_t idx = 0, size_t cnt = 1, size_t depth = 0) {
            using namespace reflect;
            indent(os, compact, idx, cnt, depth);
            os << name << ": {" << (compact ? ' ' : '\n');
            detail::for_each(obj, [&](auto &&metadata, auto &&name, auto &&value, auto &&idx, auto &&cnt) {
                if constexpr (trait::is_serialize_callable_v<decltype(os) &, decltype(value), const char *, bool, size_t, size_t, size_t>) {
                    serialize(os, value, name, compact, idx, cnt, depth + 1);
                } else {
                    serialize_impl(os, value, name, compact, idx, cnt, depth + 1);
                }
            });
            indent(os, compact, idx, cnt, depth);
            os << "}";
            common(os, compact, idx, cnt, depth);
            os << (not compact || depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename T, typename F=nullptr_t>
        void serialize_container(OS<CHAR_T> &os, T &&obj, const char *name,
                                 bool compact = false, size_t idx = 0, size_t cnt = 1, size_t depth = 0,
                                 F &&f = nullptr) {
            indent(os, compact, idx, cnt, depth);
            os << name << ": {" << (compact ? ' ' : '\n');
            size_t i = 0;
            for (auto &&it:obj) {
                if constexpr (not std::is_null_pointer_v<F>) {
                    f(os, it, std::to_string(i).c_str(), compact, i, cnt, depth + 1);
                } else {
                    std::stringstream ss;
                    if constexpr (trait::is_map_v<T>) {
                        ss << "[";
                        trait::is_printable_v<decltype(it.first)> ? ss << it.first << "]" : ss << i << "]";
                        serialize_impl(os, it.second, ss.str().c_str(), compact, i, cnt, depth + 1);
                    } else {
                        ss << "[" << i << "]";
                        serialize_impl(os, it, ss.str().c_str(), compact, i, cnt, depth + 1);
                    }
                }
                ++i;
            }
            indent(os, compact, idx, cnt, depth);
            os << "}";
            common(os, compact, idx, cnt, depth);
            os << (not compact || depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename T>
        void serialize_os_print(OS<CHAR_T> &os, T &&obj, const char *name,
                                bool compact = false, size_t idx = 0, size_t cnt = 1, size_t depth = 0) {
            indent(os, compact, idx, cnt, depth);
            os << name << ": " << obj;
            common(os, compact, idx, cnt, depth);
            os << (not compact || depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename T>
        void serialize_not_printable(OS<CHAR_T> &os, T &&obj, const char *name,
                                     bool compact = false, size_t idx = 0, size_t cnt = 1, size_t depth = 0) {
            indent(os, compact, idx, cnt, depth);
            os << name << ": " << "not printable";
            common(os, compact, idx, cnt, depth);
            os << (not compact || depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename U>
        void serialize_impl(OS<CHAR_T> &os, const U &obj, const char *name,
                            bool compact, size_t idx, size_t cnt, size_t depth) {
            using rawtype_t = std::decay_t<U>;
            using no_pointer_t = std::remove_pointer_t<U>;

            if constexpr (trait::is_general_pointer_v<rawtype_t> and not std::is_void_v<no_pointer_t>) {
                if (obj == nullptr) {
                    indent(os, compact, idx, cnt, depth);//serialize_impl(os,"nullptr",name,cfg)
                    os << name << ": nullptr";
                } else {
                    serialize_impl(os, *obj, (std::to_string('*') + name), compact, idx, cnt, depth);
                }
            } else if constexpr (trait::is_serialize_callable_v<decltype(os), U, const char *, bool, size_t, size_t, size_t>) {
                serialize(os, obj, name, compact, idx, cnt, depth);
            } else if constexpr (std::is_class_v<U> and trait::is_reflect_v<U>) {
                serialize_reflectable(os, obj, name, compact, idx, cnt, depth);
            } else if constexpr (trait::is_container_v<U> && not trait::is_string_v<U>) {
                serialize_container(os, obj, name, compact, idx, cnt, depth);
            } else if constexpr (trait::is_printable_v<U>) {
                serialize_os_print(os, obj, name, compact, idx, cnt, depth);
            } else {
                serialize_not_printable(os, obj, name, compact, idx, cnt, depth);
            }
        }
    }

    template<typename CHAR_T=char, typename U>
    inline void serialize(std::basic_ostream<CHAR_T> &os, const U &obj, const char *title = "", bool compact = false) {
        using T = std::decay_t<U>;
        static_assert(trait::is_valid_v<T>, "Type is not support to serialize !");
        detail::serialize_impl(os, obj, title, compact, 0, 1, 0);
    }

    template<typename U>
    inline auto to_string(const U &obj) {
        std::stringstream ss;
        serialize(ss, obj);
        return ss.str();
    }
}

#define REFL(NAME, ...) struct NAME {                                                                                   \
    template <typename, size_t>                                                                                         \
    struct member;                                                                                                      \
    static constexpr size_t member_index_offset = __COUNTER__ + 1;                                                      \
    PP_FOR_EACH(REFL_DECODE_EACH,_,__VA_ARGS__)                                                                         \
    static constexpr size_t member_numbers = __COUNTER__  - member_index_offset;                                        \
}
#define REFL_DECODE_EACH(_, i, data)            REFL_DECODE_##data

#define REFL_DECODE_FILED_DEFAULT_0(...)
#define REFL_DECODE_FILED_DEFAULT_1(T, ...)     {PP_TRY_REMOVE_PARENS(T)}
#define REFL_DECODE_FILED_DEFAULT_2(T, _, ...)  {PP_TRY_REMOVE_PARENS(T)}
#define REFL_DECODE_FIELD_DEFAULT(...)          PP_CAT(REFL_DECODE_FILED_DEFAULT_,PP_GET_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define REFL_DECODE___(...)                   __VA_ARGS__
#define REFL_DECODE_L(T, N, ...)                PP_TRY_REMOVE_PARENS(T) N REFL_DECODE_FIELD_DEFAULT(__VA_ARGS__);       \
                                                template <typename U>                                                   \
                                                struct member<U, __COUNTER__ - member_index_offset > {                  \
                                                    using member_type = U;                                              \
                                                    const member_type& obj;                                             \
                                                    explicit member(const member_type & obj): obj(obj) {}               \
                                                    constexpr decltype(auto) value() { return obj.N ;}                  \
                                                    static constexpr auto name() { return PP_STR(N);}                   \
                                                };

/*
 * 嵌套结构体声明
 */
#define L_(T, N, ...)                           L((REFL(T,__VA_ARGS__)), N)

#endif //TEST_REFL_H
