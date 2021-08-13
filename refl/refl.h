//
// Created by ou on 2021/8/6.
//

#ifndef TEST_REFL_H
#define TEST_REFL_H

#include "pp/preprocess.h"
#include <utility>

namespace refl {
    namespace trait {
        namespace detail {
            template<typename T>
            struct is_reflectable {
            private:
                template<typename T1, typename = decltype(T1::template FIELD<T *, 0>::name)>
                static std::true_type is_reflectable_impl(int);

                template<typename>
                static std::false_type is_reflectable_impl(...);

            public:
                using type = typename decltype(is_reflectable_impl<T>(0))::type;
            };

            template<typename T>
            struct is_container {
            private:
                template<typename C, typename = decltype(std::begin(std::declval<C>()), std::end(std::declval<C>()))>
                static std::true_type is_container_impl(int);

                template<typename>
                static std::false_type is_container_impl(...) {};

            public:
                using type = typename decltype(is_container_impl<T>(0))::type;
            };

            template<typename C, typename T>
            struct is_ostream_printable {
                template<typename T1, typename = decltype(std::declval<std::basic_ostream<C> &>()
                        << std::declval<T1>())>
                static std::true_type is_ostream_printable_impl(int);

                template<typename T1>
                static std::false_type is_ostream_printable_impl(...);

                using type = typename decltype(is_ostream_printable_impl<T>(0))::type;
            };

            template<typename T>
            struct is_user_serializable {
            private:
                template<typename T1, typename = decltype((T1::write))>
                static std::true_type is_user_serializable_impl(int);

                template<typename>
                static std::false_type is_user_serializable_impl(...) {};

            public:
                using type = typename decltype(is_user_serializable_impl<T>(0))::type;
            };

            template<typename T>
            struct is_pointer_smart {
            private:
                template<typename T1, typename = decltype((*std::declval<T1>(), std::declval<T1>().operator->(), std::declval<T1>().get()))>
                static std::true_type is_pointer_smart_impl(int);

                template<typename>
                static std::false_type is_pointer_smart_impl(...);

            public:
                using type = typename decltype(is_pointer_smart_impl<T>(0))::type;
            };

            template<typename T>
            struct is_cpp_string {
            private:
                template<typename U, typename = decltype(std::declval<U>().c_str())>
                static std::true_type is_cpp_string_test(int);

                template<typename>
                static std::false_type is_cpp_string_test(...);

            public:
                using type = typename decltype(is_cpp_string_test<T>(0))::type;
            };

            template<typename T>
            struct is_c_string : std::false_type {
            };

            template<>
            struct is_c_string<const char *> : std::true_type {
            };

            template<>
            struct is_c_string<char *> : std::true_type {
            };
        }
        template<typename T>
        inline constexpr bool is_reflectable_v{detail::is_reflectable<T>::type::value};

        template<typename T>
        inline constexpr bool is_container_v{detail::is_container<T>::type::value};

        template<typename CharT, typename T>
        inline constexpr bool is_ostream_printable_v{detail::is_ostream_printable<CharT, T>::type::value};

        template<typename T>
        inline constexpr bool is_user_serializable_v{detail::is_user_serializable<T>::type::value};

        template<typename T>
        inline constexpr bool is_pointer_smart_v{detail::is_pointer_smart<T>::type::value};
        template<typename T>
        inline constexpr bool is_pointer_all_v{is_pointer_smart_v<T> || std::is_pointer_v<T>};

        template<typename T>
        inline constexpr bool is_string_v{detail::is_cpp_string<T>::type::value || detail::is_c_string<T>::value};
    };
    namespace detail {
        template<typename CharT>
        using OS = std::basic_ostream<CharT>;
        struct Cfg {
            bool compact = false;
            size_t idx = 0;
            size_t cnt = 1;
            size_t depth = 0;
        };
        inline constexpr Cfg none_indent_line{true, 0, 1, 1};

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
            detail::for_each(obj, [&](auto &&metadata, auto &&fieldName, auto &&value, auto &&idx, auto &&cnt) {
                Cfg new_cfg{cfg.compact, idx, cnt, cfg.depth + 1};
                if constexpr (trait::is_user_serializable_v<std::decay_t<decltype(metadata)>>) {
                    metadata.write(os, value, fieldName, new_cfg);
                } else {
                    serialize_impl(os, value, fieldName, new_cfg);
                }
            });
            indent(os, cfg);
            os << "}";
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CharT, typename T, typename F=nullptr_t>
        void serialize_container(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg, F &&f = nullptr) {
            indent(os, cfg);
            os << fieldName << ": [" << (cfg.compact ? ' ' : '\n');
            size_t i = 0;
            for (auto &&it:obj) {
                Cfg new_depth{cfg.compact, i, std::size(obj), cfg.depth + 1};
                if constexpr (not std::is_null_pointer_v<F>) f(os, it, std::to_string(i).c_str(), new_depth);
                else serialize_impl(os, it, std::to_string(i).c_str(), new_depth);
                ++i;
            }
            indent(os, cfg);
            os << "]";
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CharT, typename T>
        void serialize_os_print(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": " << obj;
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CharT, typename T>
        void serialize_not_printable(OS<CharT> &os, T &&obj, const char *fieldName, const Cfg &cfg) {
            indent(os, cfg);
            os << fieldName << ": " << "not printable";
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        }

        template<typename CharT, typename _T>
        void serialize_impl(std::basic_ostream<CharT> &os, _T &&obj, const char *fieldName, const Cfg &cfg) {

            using T = std::decay_t<_T>;
            using no_pointer_t = std::remove_pointer_t<T>;
            if constexpr (trait::is_pointer_all_v<T> && !std::is_void_v<no_pointer_t>) {
                if (obj == nullptr) {
                    indent(os, cfg);
                    os << fieldName << ": nullptr";
                } else {
                    serialize_impl(os, *obj, ("*" + std::string(fieldName)).c_str(), cfg);
                    return;
                }
            } else if constexpr (std::is_class_v<T> and trait::is_reflectable_v<T>) {
                serialize_reflectable(os, obj, fieldName, cfg);
            } else if constexpr (trait::is_container_v<T> && not trait::is_string_v<T>) {
                serialize_container(os, obj, fieldName, cfg);
            } else if constexpr (trait::is_ostream_printable_v<CharT, T>) {
                serialize_os_print(os, obj, fieldName, cfg);
            } else {
                serialize_not_printable(os, obj, fieldName, cfg);
            }
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

        inline auto decorate = [](auto &&os, const auto &obj, const char *fieldName,
                                  const refl::detail::Cfg &cfg, auto &&f) {
            indent(os, cfg);
            f();
            common(os, cfg);
            os << (not cfg.compact || cfg.depth == 0 ? '\n' : ' ');
        };
    }

}
#define __FILED_TYPE_IS_SIMBLE_DECL(...)
#define __FILED_TYPE_GET_DECL(...)      __VA_ARGS__

#define __FILED_TUPLE_FIRST(T)          PP_TRY_REMOVE_PARENS(PP_GET_TUPLE(0,T))
#define __FILED_TYPE_IS_DECL_TRUE(T)    PP_CONCAT(__FILED_TYPE_GET_, __FILED_TUPLE_FIRST(T))
#define __FILED_TYPE_IS_SYMBOL(PRE, T)  PP_IF(PP_IS_PARENS(T),PP_IS_EMPTY,PP_IS_SYMBOL)(PRE,T)
#define __FILED_TYPE(T)                 PP_IF(__FILED_TYPE_IS_SYMBOL(__FILED_TYPE_IS_SIMBLE_,PP_GET_TUPLE(0,T)), \
                                              __FILED_TYPE_IS_DECL_TRUE, __FILED_TUPLE_FIRST)(T)

#define __FILED_NAME(T)                 PP_GET_N(0,PP_TRY_REMOVE_PARENS(PP_GET_TUPLE(1,T)))
#define __FIELD_DEFAULT(T)              PP_IF_TUPLE_N(2, PP_GET_TUPLE(1, T), PP_TRY_REMOVE_PARENS, PP_EMPTY_V)(PP_GET_TUPLE2D(1, 1, T))
#define __FILED_VAR(T)                  __FILED_TYPE(T) __FILED_NAME(T) {PP_IF_TUPLE_N(2, PP_GET_TUPLE(1, T), __FIELD_DEFAULT,PP_EMPTY_V)(T)} ;

#define __FIELD_USER_SER_T(tuple)       PP_EQUAL(2,PP_GET_TUPLE_COUNT(PP_GET_TUPLE(2,tuple)))
#define __FIELD_USER_SER(tuple)         PP_IF(__FIELD_USER_SER_T(tuple), <__FILED_TYPE,PP_EMPTY_V)(tuple) \
                                        PP_IF(__FIELD_USER_SER_T(tuple), >,PP_EMPTY())

#define DEF_ONE_FILED(_, i, tuple)      PP_IF(PP_EQUAL(1,PP_GET_TUPLE_COUNT(tuple)),DEF_ONE_NORMAL,DEF_ONE_REFLECT)(_, i ,tuple)
#define DEF_ONE_NORMAL(_, i, tuple)     PP_TRY_REMOVE_PARENS(PP_TRY_REMOVE_PARENS(tuple))
#define DEF_ONE_REFLECT(_, i, tuple)                                                                                                                        \
                                        __FILED_VAR(tuple)                                                                                                      \
                                        template <typename T>                                                                                                   \
                                        struct FIELD< T, PP_DEC(i) > {                                                                                          \
                                            using type = T;                                                                                                     \
                                            static constexpr auto idx = PP_DEC(i);                                                                              \
                                                                                                                                                                \
                                            type& obj;                                                                                                          \
                                            explicit FIELD(T& obj): obj(obj) {}                                                                                 \
                                            constexpr decltype(auto) value() { return obj.__FILED_NAME(tuple) ;}                                                \
                                            static constexpr auto name() { return PP_STR(__FILED_NAME(tuple));}                                                 \
                                                                                                                                                                \
                                            PP_IF_TUPLE_N(3,tuple, static constexpr auto write =                                                                \
                                                          PP_GET_N,PP_EMPTY_V)(0,PP_TRY_REMOVE_PARENS(PP_GET_TUPLE(2,tuple)))                                   \
                                            PP_IF_TUPLE_N(3,tuple,__FIELD_USER_SER,PP_EMPTY_V)(tuple);                                                          \
                                        };


#define REFL(st, ...) \
struct st{            \
    template <typename, size_t> struct FIELD;\
    static constexpr size_t _field_count_ = PP_DEC(PP_DEC(PP_GET_ARG_COUNT(__VA_ARGS__)));\
    PP_FOR_EACH(DEF_ONE_FILED, _, __VA_ARGS__)                                            \
}

#endif //TEST_REFL_H
