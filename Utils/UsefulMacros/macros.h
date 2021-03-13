#ifndef Macros_H_
#define Macros_H_

// ---------------------- 
// |      MACROS        |
// ---------------------- 
/** @def FROM_DYADIC
 * @brief Casts a dyadic function of type: `a -> b -> c` to `void* -> void* -> void*`.
 * 
 */
#define FROM_DYADIC(f) (void* (*)(void *,void*)) f
/** @def FROM_MONADIC
 * @brief Casts a monadic function of type: `a -> b` to `void* -> void*`.
 * 
 */
#define FROM_MONADIC(f) (void* (*)(void *)) f
/** @def FROM_MONADIC_VOID
 * @brief Casts a monadic function of type: `a -> ()` to `void* -> void`.
 * 
 */
#define FROM_MONADIC_VOID(f) (void (*)(void *)) f
/** @def COMPSTR
 * @brief Compares two strings, returns 1 if they are the same, and 0 otherwise.
 * 
 */
#define COMPSTR(fst, snd) strcmp(fst,snd) == 0

#endif // Macros_H_