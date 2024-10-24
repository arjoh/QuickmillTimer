#include <limits>
#include <stdexcept>
#include "bitS.h"

template <typename T>
inline void check_shift(T number, unsigned int n)
{
    // Ensure that T is an unsigned integer type
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "T must be an unsigned integer type");

    // Get the number of bits in the type T
    constexpr unsigned int bit_width = std::numeric_limits<T>::digits;

    // Check if n exceeds the bit width of T
    if (n >= bit_width)
    {
        throw std::out_of_range("Shift count exceeds the number of bits in the type.");
    }
}

template <typename T>
inline T bit_set(T number, T n)
{
    check_shift(number, n);
    return number | (static_cast<T>(1) << n);
}

template <typename T>
inline T bit_unset(T number, T n)
{
    check_shift(number, n); // Assuming this function checks if the shift is valid
    return number & ~(static_cast<T>(1) << n);
}

template <typename T>
inline T bit_toggle(T number, T n)
{
    check_shift(number, n);
    return number ^ ((T)1 << n);
}

template <typename T>
inline bool bit_check(T number, T n)
{
    check_shift(number, n);
    return (number >> n) & (T)1;
}

template <typename T>
inline T bit_set(T number, T n, bool x)
{
    check_shift(number, n);
    return (number & ~((T)1 << n)) | ((T)x << n);
}