#pragma once

using	i8 = char;
using	u8 = unsigned char;
using	i16 = short;
using	u16 = unsigned short;
using	i32 = int;
using	u32 = unsigned int;
using	i64 = long long;
using	u64 = unsigned long long;
using	f32 = float;
using	f64 = double;

constexpr i8	I8MIN = (-128);
constexpr i8	I8MAX = 127;
constexpr u8	U8MIN = 0U;
constexpr u8	U8MAX = 255U;
constexpr i16	I16MIN = (-32767 - 1);
constexpr i16	I16MAX = 32767;
constexpr u16	U16MIN = 0U;
constexpr u16	U16MAX = 65535U;
constexpr i32	I32MIN = (-2147483647 - 1);
constexpr i32	I32MAX = 2147483647;
constexpr u32	U32MIN = 0U;
constexpr u32	U32MAX = 4294967295U;
constexpr i64	I64MIN = (-9223372036854775807LL - 1);
constexpr i64	I64MAX = 9223372036854775807LL;
constexpr u64	U64MIN = 0ULL;
constexpr u64	U64MAX = 18446744073709551615ULL;
