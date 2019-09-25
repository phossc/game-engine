#ifndef JSS_STRONG_TYPEDEF_HPP
#define JSS_STRONG_TYPEDEF_HPP
#include <type_traits>
#include <utility>
#include <functional>
#include <ostream>

namespace jss {
    namespace detail {
        using small_result= char;
        struct large_result {
            small_result dummy[2];
        };

        template <typename Derived, typename ValueType, typename... Properties>
        class strong_typedef_base
            : public Properties::template mixin<Derived, ValueType>... {
        public:
            using underlying_value_type= ValueType;

            constexpr strong_typedef_base() noexcept : value() {}

            explicit constexpr strong_typedef_base(ValueType value_) noexcept(
                std::is_nothrow_move_constructible<ValueType>::value) :
                value(std::move(value_)) {}

            explicit constexpr operator ValueType const &() const noexcept {
                return value;
            }

            constexpr ValueType const &underlying_value() const noexcept {
                return value;
            }

            constexpr ValueType &underlying_value() noexcept {
                return value;
            }

        private:
            ValueType value;
        };

        template <typename T> constexpr T &&underlying_value(T &&t) {
            return std::forward<T>(t);
        }

    }

    template <typename Tag, typename ValueType, typename... Properties>
    struct strong_typedef : detail::strong_typedef_base<
                                strong_typedef<Tag, ValueType, Properties...>,
                                ValueType, Properties...> {
        using detail::strong_typedef_base<
            strong_typedef<Tag, ValueType, Properties...>, ValueType,
            Properties...>::strong_typedef_base;
    };

    namespace detail {
        template <typename Tag, typename ValueType, typename... Properties>
        constexpr ValueType &underlying_value(
            jss::strong_typedef<Tag, ValueType, Properties...> &t) {
            return t.underlying_value();
        }
        template <typename Tag, typename ValueType, typename... Properties>
        constexpr ValueType const &underlying_value(
            jss::strong_typedef<Tag, ValueType, Properties...> const &t) {
            return t.underlying_value();
        }
        template <typename Tag, typename ValueType, typename... Properties>
        constexpr ValueType &&underlying_value(
            jss::strong_typedef<Tag, ValueType, Properties...> &&t) {
            return std::move(t.underlying_value());
        }
        template <typename Tag, typename ValueType, typename... Properties>
        constexpr ValueType const &&underlying_value(
            jss::strong_typedef<Tag, ValueType, Properties...> const &&t) {
            return std::move(t.underlying_value());
        }
    }

    namespace strong_typedef_properties {
        struct equality_comparable {
            template <typename Derived, typename ValueType> struct mixin {
                friend constexpr bool
                operator==(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() ==
                        std::declval<ValueType const &>())) {
                    return lhs.underlying_value() == rhs.underlying_value();
                }
                friend constexpr bool
                operator!=(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() !=
                        std::declval<ValueType const &>())) {
                    return lhs.underlying_value() != rhs.underlying_value();
                }
            };
        };

        struct pre_incrementable {
            template <typename Derived, typename ValueType> struct mixin {
                friend Derived &operator++(Derived &self) noexcept(
                    noexcept(++std::declval<ValueType &>())) {
                    ++self.underlying_value();
                    return self;
                }
            };
        };

        struct post_incrementable {
            template <typename Derived, typename ValueType> struct mixin {
                friend Derived operator++(Derived &self, int) noexcept(
                    noexcept(std::declval<ValueType &>()++)) {
                    return Derived{self.underlying_value()++};
                }
            };
        };

        struct incrementable {
            template <typename Derived, typename ValueType>
            struct mixin : pre_incrementable::mixin<Derived, ValueType>,
                           post_incrementable::mixin<Derived, ValueType> {};
        };

        struct pre_decrementable {
            template <typename Derived, typename ValueType> struct mixin {
                friend Derived &operator--(Derived &self) noexcept(
                    noexcept(--std::declval<ValueType &>())) {
                    --self.underlying_value();
                    return self;
                }
            };
        };

        struct post_decrementable {
            template <typename Derived, typename ValueType> struct mixin {
                friend Derived operator--(Derived &self, int) noexcept(
                    noexcept(std::declval<ValueType &>()--)) {
                    return Derived{self.underlying_value()--};
                }
            };
        };

        struct decrementable {
            template <typename Derived, typename ValueType>
            struct mixin : pre_decrementable::mixin<Derived, ValueType>,
                           post_decrementable::mixin<Derived, ValueType> {};
        };

        struct generic_mixed_addable {
            template <typename Derived, typename ValueType> struct mixin {
                template <typename Rhs>
                friend typename std::enable_if<
                    !std::is_same<Rhs, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                std::declval<ValueType const &>() +
                                detail::underlying_value(
                                    std::declval<Rhs const &>())),
                            ValueType>::value,
                    Derived>::type
                operator+(Derived const &lhs, Rhs const &rhs) noexcept(noexcept(
                    std::declval<ValueType const &>() +
                    detail::underlying_value(std::declval<Rhs const &>()))) {
                    return Derived{lhs.underlying_value() +
                                   detail::underlying_value(rhs)};
                }

                template <typename Lhs>
                friend typename std::enable_if<
                    !std::is_same<Lhs, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                detail::underlying_value(
                                    std::declval<Lhs const &>()) +
                                std::declval<ValueType const &>()),
                            ValueType>::value,
                    Derived>::type
                operator+(Lhs const &lhs, Derived const &rhs) noexcept(noexcept(
                    detail::underlying_value(std::declval<Lhs const &>()) +
                    std::declval<ValueType const &>())) {
                    return Derived{detail::underlying_value(lhs) +
                                   rhs.underlying_value()};
                }
            };
        };
#define JSS_COMPOUND_ASSIGN(op_symbol) op_symbol##=

#define JSS_DEFINE_OP_MIXINS(name, op_symbol)                                  \
    template <typename Other> struct mixed_##name {                            \
        template <                                                             \
            typename Derived, typename ValueType,                              \
            bool= std::is_literal_type<ValueType>::value>                      \
        struct mixin {                                                         \
            friend constexpr Derived                                           \
            operator op_symbol(Derived const &lhs, Other const &rhs) noexcept( \
                noexcept(std::declval<ValueType const &>()                     \
                             op_symbol detail::underlying_value(               \
                                 std::declval<Other const &>()))) {            \
                return Derived{lhs.underlying_value()                          \
                                   op_symbol detail::underlying_value(rhs)};   \
            }                                                                  \
                                                                               \
            friend constexpr Derived                                           \
            operator op_symbol(Other const &lhs, Derived const &rhs) noexcept( \
                noexcept(                                                      \
                    detail::underlying_value(std::declval<Other const &>())    \
                        op_symbol std::declval<ValueType const &>())) {        \
                return Derived{detail::underlying_value(lhs)                   \
                                   op_symbol rhs.underlying_value()};          \
            }                                                                  \
                                                                               \
            friend Derived &operator JSS_COMPOUND_ASSIGN(op_symbol)(           \
                Derived &lhs,                                                  \
                Other const                                                    \
                    &rhs) noexcept(noexcept(std::declval<ValueType &>()        \
                                                JSS_COMPOUND_ASSIGN(op_symbol) \
                                                    detail::underlying_value(  \
                                                        std::declval<          \
                                                            Other const        \
                                                                &>()))) {      \
                lhs.underlying_value() JSS_COMPOUND_ASSIGN(op_symbol)          \
                    detail::underlying_value(rhs);                             \
                return lhs;                                                    \
            }                                                                  \
        };                                                                     \
    };                                                                         \
    template <typename Other>                                                  \
    template <typename Derived, typename ValueType>                            \
    struct mixed_##name<Other>::mixin<Derived, ValueType, false> {             \
        friend Derived                                                         \
        operator op_symbol(Derived const &lhs, Other const &rhs) noexcept(     \
            noexcept(std::declval<ValueType const &>()                         \
                         op_symbol detail::underlying_value(                   \
                             std::declval<Other const &>()))) {                \
            return Derived{lhs.underlying_value()                              \
                               op_symbol detail::underlying_value(rhs)};       \
        }                                                                      \
                                                                               \
        friend Derived                                                         \
        operator op_symbol(Other const &lhs, Derived const &rhs) noexcept(     \
            noexcept(detail::underlying_value(std::declval<Other const &>())   \
                         op_symbol std::declval<ValueType const &>())) {       \
            return Derived{detail::underlying_value(lhs)                       \
                               op_symbol rhs.underlying_value()};              \
        }                                                                      \
                                                                               \
        friend Derived &operator JSS_COMPOUND_ASSIGN(op_symbol)(               \
            Derived &lhs,                                                      \
            Other const                                                        \
                &rhs) noexcept(noexcept(std::declval<ValueType &>()            \
                                            JSS_COMPOUND_ASSIGN(op_symbol)     \
                                                detail::underlying_value(      \
                                                    std::declval<              \
                                                        Other const &>()))) {  \
            lhs.underlying_value() JSS_COMPOUND_ASSIGN(op_symbol)              \
                detail::underlying_value(rhs);                                 \
            return lhs;                                                        \
        }                                                                      \
    };                                                                         \
    struct self_##name {                                                       \
        template <                                                             \
            typename Derived, typename ValueType,                              \
            bool= std::is_literal_type<ValueType>::value>                      \
        struct mixin {                                                         \
            friend constexpr Derived operator op_symbol(                       \
                Derived const &lhs,                                            \
                Derived const                                                  \
                    &rhs) noexcept(noexcept(std::declval<ValueType const &>()  \
                                                op_symbol std::declval<        \
                                                    ValueType const &>())) {   \
                return Derived{lhs.underlying_value()                          \
                                   op_symbol rhs.underlying_value()};          \
            }                                                                  \
            friend Derived &operator JSS_COMPOUND_ASSIGN(op_symbol)(           \
                Derived &lhs,                                                  \
                Derived const                                                  \
                    &rhs) noexcept(noexcept(std::declval<ValueType &>()        \
                                                JSS_COMPOUND_ASSIGN(op_symbol) \
                                                    std::declval<              \
                                                        ValueType const        \
                                                            &>())) {           \
                lhs.underlying_value() JSS_COMPOUND_ASSIGN(op_symbol)          \
                    rhs.underlying_value();                                    \
                return lhs;                                                    \
            }                                                                  \
        };                                                                     \
    };                                                                         \
                                                                               \
    template <typename Derived, typename ValueType>                            \
    struct self_##name::mixin<Derived, ValueType, false> {                     \
        friend Derived                                                         \
        operator op_symbol(Derived const &lhs, Derived const &rhs) noexcept(   \
            noexcept(std::declval<ValueType const &>()                         \
                         op_symbol std::declval<ValueType const &>())) {       \
            return Derived{lhs.underlying_value()                              \
                               op_symbol rhs.underlying_value()};              \
        }                                                                      \
        friend Derived &operator JSS_COMPOUND_ASSIGN(op_symbol)(               \
            Derived &lhs,                                                      \
            Derived const                                                      \
                &rhs) noexcept(noexcept(std::declval<ValueType &>()            \
                                            JSS_COMPOUND_ASSIGN(op_symbol)     \
                                                std::declval<                  \
                                                    ValueType const &>())) {   \
            lhs.underlying_value() JSS_COMPOUND_ASSIGN(op_symbol)              \
                rhs.underlying_value();                                        \
            return lhs;                                                        \
        }                                                                      \
    };                                                                         \
    struct name {                                                              \
        template <typename Derived, typename ValueType>                        \
        struct mixin                                                           \
            : self_##name::mixin<Derived, ValueType>,                          \
              mixed_##name<ValueType>::template mixin<Derived, ValueType> {};  \
    };

        JSS_DEFINE_OP_MIXINS(addable, +)
        JSS_DEFINE_OP_MIXINS(subtractable, -)
        JSS_DEFINE_OP_MIXINS(multiplicable, *)
        JSS_DEFINE_OP_MIXINS(divisible, /)
        JSS_DEFINE_OP_MIXINS(modulus, %)
        JSS_DEFINE_OP_MIXINS(bitwise_or, |)
        JSS_DEFINE_OP_MIXINS(bitwise_and, &)
        JSS_DEFINE_OP_MIXINS(bitwise_xor, ^)

        struct generic_mixed_subtractable {
            template <typename Derived, typename ValueType> struct mixin {
                template <typename Rhs>
                friend typename std::enable_if<
                    !std::is_same<Rhs, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                std::declval<ValueType const &>() -
                                detail::underlying_value(
                                    std::declval<Rhs const &>())),
                            ValueType>::value,
                    Derived>::type
                operator-(Derived const &lhs, Rhs const &rhs) noexcept(noexcept(
                    std::declval<ValueType const &>() -
                    detail::underlying_value(std::declval<Rhs const &>()))) {
                    return Derived{lhs.underlying_value() -
                                   detail::underlying_value(rhs)};
                }

                template <typename Lhs>
                friend typename std::enable_if<
                    !std::is_same<Lhs, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                detail::underlying_value(
                                    std::declval<Lhs const &>()) -
                                std::declval<ValueType const &>()),
                            ValueType>::value,
                    Derived>::type
                operator-(Lhs const &lhs, Derived const &rhs) noexcept(noexcept(
                    detail::underlying_value(std::declval<Lhs const &>()) -
                    std::declval<ValueType const &>())) {
                    return Derived{detail::underlying_value(lhs) -
                                   rhs.underlying_value()};
                }
            };
        };

        template <typename DifferenceType> struct difference {
            template <typename Derived, typename ValueType> struct mixin {
                friend DifferenceType
                operator-(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() -
                        std::declval<ValueType const &>())) {
                    return DifferenceType{lhs.underlying_value() -
                                          rhs.underlying_value()};
                }
            };
        };

        struct ordered {
            template <typename Derived, typename ValueType> struct mixin {
                friend constexpr bool
                operator<(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() <
                        std::declval<ValueType const &>())) {
                    return lhs.underlying_value() < rhs.underlying_value();
                }
                friend constexpr bool
                operator>(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() >
                        std::declval<ValueType const &>())) {
                    return lhs.underlying_value() > rhs.underlying_value();
                }
                friend constexpr bool
                operator<=(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() <=
                        std::declval<ValueType const &>())) {
                    return lhs.underlying_value() <= rhs.underlying_value();
                }
                friend constexpr bool
                operator>=(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() >=
                        std::declval<ValueType const &>())) {
                    return lhs.underlying_value() >= rhs.underlying_value();
                }
            };
        };

        template <typename Other> struct mixed_ordered {
            template <typename Derived, typename ValueType> struct mixin {
                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                std::declval<ValueType const &>() <
                                detail::underlying_value(
                                    std::declval<Other const &>())),
                            bool>::value,
                    bool>::type
                operator<(Derived const &lhs, Other const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() <
                        detail::underlying_value(
                            std::declval<Other const &>()))) {
                    return lhs.underlying_value() <
                           detail::underlying_value(rhs);
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                detail::underlying_value(
                                    std::declval<Other const &>()) <
                                std::declval<ValueType const &>()),
                            bool>::value,
                    bool>::type
                operator<(Other const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        detail::underlying_value(
                            std::declval<Other const &>()) <
                        std::declval<ValueType const &>())) {
                    return detail::underlying_value(lhs) <
                           rhs.underlying_value();
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                std::declval<ValueType const &>() >
                                detail::underlying_value(
                                    std::declval<Other const &>())),
                            bool>::value,
                    bool>::type
                operator>(Derived const &lhs, Other const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() >
                        detail::underlying_value(
                            std::declval<Other const &>()))) {
                    return lhs.underlying_value() >
                           detail::underlying_value(rhs);
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                detail::underlying_value(
                                    std::declval<Other const &>()) >
                                std::declval<ValueType const &>()),
                            bool>::value,
                    bool>::type
                operator>(Other const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        detail::underlying_value(
                            std::declval<Other const &>()) >
                        std::declval<ValueType const &>())) {
                    return detail::underlying_value(lhs) >
                           rhs.underlying_value();
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                std::declval<ValueType const &>() >=
                                detail::underlying_value(
                                    std::declval<Other const &>())),
                            bool>::value,
                    bool>::type
                operator>=(Derived const &lhs, Other const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() >=
                        std::declval<Other const &>())) {
                    return lhs.underlying_value() >=
                           detail::underlying_value(rhs);
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                detail::underlying_value(
                                    std::declval<Other const &>()) >=
                                std::declval<ValueType const &>()),
                            bool>::value,
                    bool>::type
                operator>=(Other const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        detail::underlying_value(
                            std::declval<Other const &>()) >=
                        std::declval<ValueType const &>())) {
                    return detail::underlying_value(lhs) >=
                           rhs.underlying_value();
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                std::declval<ValueType const &>() <=
                                detail::underlying_value(
                                    std::declval<Other const &>())),
                            bool>::value,
                    bool>::type
                operator<=(Derived const &lhs, Other const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() <=
                        detail::underlying_value(
                            std::declval<Other const &>()))) {
                    return lhs.underlying_value() <=
                           detail::underlying_value(rhs);
                }

                friend typename std::enable_if<
                    !std::is_same<Other, Derived>::value &&
                        std::is_convertible<
                            decltype(
                                detail::underlying_value(
                                    std::declval<Other const &>()) <=
                                std::declval<ValueType const &>()),
                            bool>::value,
                    bool>::type
                operator<=(Other const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        detail::underlying_value(
                            std::declval<Other const &>()) <=
                        std::declval<ValueType const &>())) {
                    return detail::underlying_value(lhs) <=
                           rhs.underlying_value();
                }
            };
        };

        struct hashable {
            struct base {};
            template <typename Derived, typename ValueType>
            struct mixin : base {};
        };

        struct streamable {
            template <typename Derived, typename ValueType> struct mixin {
                friend std::ostream &
                operator<<(std::ostream &os, Derived const &st) {
                    return os << st.underlying_value();
                }
            };
        };

        struct comparable {
            template <typename Derived, typename ValueType>
            struct mixin
                : ordered::template mixin<Derived, ValueType>,
                  equality_comparable::template mixin<Derived, ValueType> {};
        };

        template <typename RatioType> struct ratio {
            template <typename Derived, typename ValueType> struct mixin {
                friend RatioType
                operator/(Derived const &lhs, Derived const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() /
                        std::declval<ValueType const &>())) {
                    return RatioType{lhs.underlying_value() /
                                     rhs.underlying_value()};
                }
            };
        };

        struct bitwise_not {
            template <
                typename Derived, typename ValueType,
                bool= std::is_literal_type<ValueType>::value>
            struct mixin {
                friend constexpr Derived operator~(Derived const &lhs) noexcept(
                    noexcept(~std::declval<ValueType const &>())) {
                    return Derived{~lhs.underlying_value()};
                }
            };
        };

        template <typename Derived, typename ValueType>
        struct bitwise_not::mixin<Derived, ValueType, false> {
            friend Derived operator~(Derived const &lhs) noexcept(
                noexcept(~std::declval<ValueType const &>())) {
                return Derived{~lhs.underlying_value()};
            }
        };

        template <typename Other> struct bitwise_left_shift {
            template <
                typename Derived, typename ValueType,
                bool= std::is_literal_type<ValueType>::value>
            struct mixin {
                friend constexpr Derived
                operator<<(Derived const &lhs, Other const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>()
                        << detail::underlying_value(
                               std::declval<Other const &>()))) {
                    return Derived{lhs.underlying_value()
                                   << detail::underlying_value(rhs)};
                }
                friend Derived &
                operator<<=(Derived &lhs, Other const &rhs) noexcept(noexcept(
                    std::declval<ValueType &>()<<=
                    detail::underlying_value(std::declval<Other const &>()))) {
                    lhs.underlying_value()<<= detail::underlying_value(rhs);
                    return lhs;
                }
            };
        };

        template <typename Other>
        template <typename Derived, typename ValueType>
        struct bitwise_left_shift<Other>::mixin<Derived, ValueType, false> {
            friend Derived
            operator<<(Derived const &lhs, Other const &rhs) noexcept(noexcept(
                std::declval<ValueType const &>()
                << detail::underlying_value(std::declval<Other const &>()))) {
                return Derived{lhs.underlying_value()
                               << detail::underlying_value(rhs)};
            }
            friend Derived &
            operator<<=(Derived &lhs, Other const &rhs) noexcept(noexcept(
                std::declval<ValueType &>()<<=
                detail::underlying_value(std::declval<Other const &>()))) {
                lhs.underlying_value()<<= detail::underlying_value(rhs);
                return lhs;
            }
        };

        template <typename Other> struct bitwise_right_shift {
            template <
                typename Derived, typename ValueType,
                bool= std::is_literal_type<ValueType>::value>
            struct mixin {
                friend constexpr Derived
                operator>>(Derived const &lhs, Other const &rhs) noexcept(
                    noexcept(
                        std::declval<ValueType const &>() >>
                        detail::underlying_value(
                            std::declval<Other const &>()))) {
                    return Derived{lhs.underlying_value() >>
                                   detail::underlying_value(rhs)};
                }
                friend Derived &
                operator>>=(Derived &lhs, Other const &rhs) noexcept(noexcept(
                    std::declval<ValueType &>()>>=
                    detail::underlying_value(std::declval<Other const &>()))) {
                    lhs.underlying_value()>>= detail::underlying_value(rhs);
                    return lhs;
                }
            };
        };

        template <typename Other>
        template <typename Derived, typename ValueType>
        struct bitwise_right_shift<Other>::mixin<Derived, ValueType, false> {
            friend Derived
            operator>>(Derived const &lhs, Other const &rhs) noexcept(noexcept(
                std::declval<ValueType const &>() >>
                detail::underlying_value(std::declval<Other const &>()))) {
                return Derived{lhs.underlying_value() >>
                               detail::underlying_value(rhs)};
            }
            friend Derived &
            operator>>=(Derived &lhs, Other const &rhs) noexcept(noexcept(
                std::declval<ValueType &>()>>=
                detail::underlying_value(std::declval<Other const &>()))) {
                lhs.underlying_value()>>= detail::underlying_value(rhs);
                return lhs;
            }
        };

    }
}

namespace std {
    template <typename Tag, typename ValueType, typename... Properties>
    struct hash<jss::strong_typedef<Tag, ValueType, Properties...>> {
        template <typename Arg>
        typename std::enable_if<
            std::is_same<
                Arg,
                jss::strong_typedef<Tag, ValueType, Properties...>>::value &&
                std::is_base_of<
                    jss::strong_typedef_properties::hashable::base, Arg>::value,
            size_t>::type
        operator()(Arg const &arg) const noexcept(noexcept(
            std::hash<ValueType>()(std::declval<ValueType const &>()))) {
            return std::hash<ValueType>()(arg.underlying_value());
        }
    };

}

#endif
