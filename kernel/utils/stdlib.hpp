#pragma once

# include "utils/stdint.hpp"

inline i32	atoi(const i8 *str)
{
	if (!str)
		return (0);

	while (*str == ' ')
		++str;

	i8	sign = 1;
	i32	result = 0;

	if (*str == '-')
		sign = -1;

	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		++str;
	}

	return (result * sign);
}
