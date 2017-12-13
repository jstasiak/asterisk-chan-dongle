# Define to const if you has iconv() const declaration of input buffer
add_definitions(-DICONV_CONST)

# Define to iconv_t if you has iconv_t in iconv.h
add_definitions(-DICONV_T="iconv_t")

# Build extention applications
add_definitions(-DBUILD_APPLICATIONS="1")

# Build Manager extentions
add_definitions(-DBUILD_MANAGER="1")

# Define to 1 if you have the <fcntl.h> header file.
add_definitions(-DHAVE_FCNTL_H="1")

# Define to 1 if you have the <iconv.h> header file.
# #undef HAVE_ICONV_H

# Define to 1 if you have the <inttypes.h> header file.
add_definitions(-DHAVE_INTTYPES_H="1")

# Define to 1 if you have the `memchr' function.
add_definitions(-DHAVE_MEMCHR="1")

# Define to 1 if you have the `memmem' function.
add_definitions(-DHAVE_MEMMEM="1")

# Define to 1 if you have the `memmove' function.
add_definitions(-DHAVE_MEMMOVE="1")

# Define to 1 if you have the <memory.h> header file.
add_definitions(-DHAVE_MEMORY_H="1")

# Define to 1 if you have the `memset' function.
add_definitions(-DHAVE_MEMSET="1")

# Define to 1 if you have the <stdint.h> header file.
add_definitions(-DHAVE_STDINT_H="1")

# Define to 1 if you have the <stdlib.h> header file.
add_definitions(-DHAVE_STDLIB_H="1")

# Define to 1 if you have the `strcasecmp' function.
add_definitions(-DHAVE_STRCASECMP="1")

# Define to 1 if you have the `strchr' function.
add_definitions(-DHAVE_STRCHR="1")

# Define to 1 if you have the `realpath' function.
add_definitions(-DHAVE_REALPATH="1")

# Define to 1 if you have the <strings.h> header file.
add_definitions(-DHAVE_STRINGS_H="1")

# Define to 1 if you have the <string.h> header file.
add_definitions(-DHAVE_STRING_H="1")

# Define to 1 if you have the `strncasecmp' function.
add_definitions(-DHAVE_STRNCASECMP="1")

# Define to 1 if you have the `strtol' function.
add_definitions(-DHAVE_STRTOL="1")

# Define to 1 if you have the <sys/stat.h> header file.
add_definitions(-DHAVE_SYS_STAT_H="1")

# Define to 1 if you have the <sys/time.h> header file.
add_definitions(-DHAVE_SYS_TIME_H="1")

# Define to 1 if you have the <sys/types.h> header file.
add_definitions(-DHAVE_SYS_TYPES_H="1")

# Define to 1 if you have the <termios.h> header file.
add_definitions(-DHAVE_TERMIOS_H="1")

# Define to 1 if you have the <unistd.h> header file.
add_definitions(-DHAVE_UNISTD_H="1")

# Define to 1 if you have the ANSI C header files.
add_definitions(-DSTDC_HEADERS=1)