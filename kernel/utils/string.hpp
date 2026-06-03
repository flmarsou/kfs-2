#pragma once

# include "utils/stdint.hpp"

inline i8	strcmp(const i8 *str1, const i8 *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		++str1;
		++str2;
	}

	return (*str1 - *str2);
}

inline u32	strlen(const i8 *str)
{
	u32	len = 0;

	while (*str)
	{
		++str;
		++len;
	}

	return (len);
}
