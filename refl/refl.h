//
// Created by ou on 2021/8/6.
//

#ifndef TEST_REFL_H
#define TEST_REFL_H

#include "pp/preprocess.h"
#include <utility>
#include <string>
#include <ostream>
#include <sstream>

namespace reflect::trait {
    PP_MAKE_TRAITS_HAS_XXX(is_reflect, (std::void_t<decltype(U::template member<U *, 0>::name)> *) 0)

    PP_MAKE_TRAITS_HAS_XXX(is_printable, PP_DECLVAL(std::basic_ostream<char>) << PP_DECLVAL(U))

    PP_MAKE_TRAITS_HAS_XXX(is_container, std::begin(PP_DECLVAL(U)), std::end(PP_DECLVAL(U)))

    PP_MAKE_TRAITS_HAS_XXX(is_smart_pointer, *PP_DECLVAL(U), PP_DECLVAL(U).operator->(), PP_DECLVAL(U).get())

    PP_MAKE_TRAITS_HAS_XXX(is_map, PP_DECLVAL(U).key_comp())

    PP_MAKE_TRAITS_CALLABLE_XXX(debug)

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
        struct Cfg {
            bool compact = false;
            size_t idx = 0, cnt = 1, depth = 0;
        };
        inline constexpr Cfg none_indent_line{true, 0, 1, 1};

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
        inline void indent(OS<CHAR_T> &os, const Cfg &cfg) {
            if (not cfg.compact) for (int i = 0; i < cfg.depth; i++) os << "    ";
        }

        template<typename CHAR_T>
        inline void common(OS<CHAR_T> &os, const Cfg &cfg) { os << (cfg.idx + 1 == cfg.cnt ? "" : ","); }

        template<typename CHAR_T, typename T>
        void serialize_reflectable(OS<CHAR_T> &os, const T &obj, const char *fieldName, const Cfg &cfg) {
            using namespace reflect;
            indent(os, cfg);
            os << fieldName << ": {" << (cfg.compact ? ' ' : '\n');
            detail::for_each(obj, [&](auto &&metadata, auto &&fieldName, auto &&value, auto &&idx, auto &&cnt) {
                Cfg new_cfg{cfg.compact, idx, cnt, cfg.depth + 1};
                if constexpr (trait::is_debug_callable_v<std::decay_t<decltype(metadata)>>) {
                    debug(value);
                } else {
                    serialize_impl(os, value, fieldName, new_cfg);
                }
            });
            indent(os, cfg);
            os << "}";
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename T, typename F=nullptr_t>
        void serialize_container(OS<CHAR_T> &os, T &&obj, const char *fieldName, const Cfg &cfg, F &&f = nullptr) {
            indent(os, cfg);
            os << fieldName << ": {" << (cfg.compact ? ' ' : '\n');
            size_t i = 0;
            for (auto &&it:obj) {
                Cfg new_depth{cfg.compact, i, std::size(obj), cfg.depth + 1};
                if constexpr (not std::is_null_pointer_v<F>) {
                    f(os, it, std::to_string(i).c_str(), new_depth);
                } else {
                    std::stringstream ss;
                    if constexpr (trait::is_map_v<T>) {
                        ss << "[";
                        trait::is_printable_v<decltype(it.first)> ? ss << it.first << "]" : ss << i << "]";
                        serialize_impl(os, it.second, ss.str().c_str(), new_depth);
                    } else {
                        ss << "[" << i << "]";
                        serialize_impl(os, it, ss.str().c_str(), new_depth);
                    }
                }
                ++i;
            }
            indent(os, cfg);
            os << "}";
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename T>
        void serialize_os_print(OS<CHAR_T> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": " << obj;
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename T>
        void serialize_not_printable(OS<CHAR_T> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": " << "not printable";
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CHAR_T, typename U>
        void serialize_impl(std::basic_ostream<CHAR_T> &os, const U &obj, const char *fieldName, const Cfg &cfg) {
            using rawtype_t = std::decay_t<U>;
            using no_pointer_t = std::remove_pointer_t<U>;

            if constexpr (trait::is_general_pointer_v<rawtype_t> and not std::is_void_v<no_pointer_t>) {
                if (obj == nullptr) {
                    indent(os, cfg);//serialize_impl(os,"nullptr",fieldName,cfg)
                    os << fieldName << ": nullptr";
                } else {
                    serialize_impl(os, *obj, (std::to_string('*') + fieldName), cfg);
                }
            } else if constexpr (trait::is_debug_callable_v<U>) {
                debug(obj);
            } else if constexpr (std::is_class_v<U> and trait::is_reflect_v<U>) {
                serialize_reflectable(os, obj, fieldName, cfg);
            } else if constexpr (trait::is_container_v<U> && not trait::is_string_v<U>) {
                serialize_container(os, obj, fieldName, cfg);
            } else if constexpr (trait::is_printable_v<U>) {
                serialize_os_print(os, obj, fieldName, cfg);
            } else {
                serialize_not_printable(os, obj, fieldName, cfg);
            }
        }
    }

    template<typename CHAR_T=char, typename U>
    inline void serialize(std::basic_ostream<CHAR_T> &os, const U &obj, const char *title = "", bool compact = false) {
        using T = std::decay_t<U>;
        static_assert(trait::is_valid_v<T>, "Type is not support to serialize !");
        detail::serialize_impl(os, obj, title, {compact, 0, 1, 0});
    }
}

#define REFL(NAME, ...) struct NAME {                                                                                 \
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
