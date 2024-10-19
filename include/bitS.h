/* bitS.h

Bit Shifting functions based on this excellent answer on Stack Overflow: https://stackoverflow.com/a/47990
I used ChatGPT to help convert them into templated functions and to check the shift size.

*/

#ifndef bitS_h
#define bitS_h

#include <type_traits>

template <typename T>
inline T bit_set(T, T);

template <typename T>
inline T bit_unset(T, T);

template <typename T>
inline T bit_toggle(T, T);

template <typename T>
inline bool bit_check(T, T);

template <typename T>
inline T bit_set(T, T, bool);

#endif // bitS_h