#ifndef ENV_H
#define ENV_H		1
/**
 * @file env.h
 * @brief Wrappers for environment variable functions.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


extern void xputenv(char *string);
extern void xsetenv(const char *name, const char *value, int overwrite);


#endif /* !ENV_H */
