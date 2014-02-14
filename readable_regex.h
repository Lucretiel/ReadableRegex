/*
 * Copyright 2014 Nathan West
 *
 * This file is part of ReadableRegex.
 *
 * ReadableRegex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ReadableRegex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ReadableRegex.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define FRONT_ANCHOR(CONTENT) "^" CONTENT
#define BACK_ANCHOR(CONTENT) CONTENT "$"
#define FULL_ANCHOR(CONTENT) FRONT_ANCHOR(BACK_ANCHOR(CONTENT))

#define SUBMATCH(CONTENT) "(" CONTENT ")"
#define GROUP(CONTENT) "(?:" CONTENT ")"
#define CLASS(CONTENT) "[" CONTENT "]"

#define OR "|"

/* Use with MANY, AT_LEAST_ONE, AT_MOST, AT_LEAST, EXACTLY, and WITHN */
#define MINIMAL(CONTENT) CONTENT "?"

/* Unsafe repitition operators. ONLY insert the relevant characters. */
#define U_AT_MOST(MAX, CONTENT) CONTENT "{," #MAX "}"
#define U_AT_LEAST(MIN, CONTENT) CONTENT "{" #MIN ",}"
#define U_EXACTLY(NUM, CONTENT) CONTENT "{" #NUM "}"
#define U_WITHIN(MIN, MAX, CONTENT) CONTENT "{" #MIN "," #MAX "}"

#define U_MANY(CONTENT) CONTENT "*"
#define U_AT_LEAST_ONE(CONTENT) CONTENT "+"
#define U_OPTIONAL(CONTENT) CONTENT "?"
#define U_EITHER(CONTENT1, CONTENT2) CONTENT1 OR CONTENT2

/* Safe repition operators. Wrap the content in a non-submatching-group. */
#define S_AT_MOST(MAX, CONTENT) U_AT_MOST(MAX, GROUP(CONTENT))
#define S_AT_LEAST(MIN, CONTENT) U_AT_LEAST(MIN, GROUP(CONTENT))
#define S_EXACTLY(NUM, CONTENT) U_EXACTLY(NUM, GROUP(CONTENT))
#define S_WITHIN(MIN, MAX, CONTENT) U_WITHIN(MIN, MAX, GROUP(CONTENT))

#define S_MANY(CONTENT) U_MANY(GROUP(CONTENT))
#define S_AT_LEAST_ONE(CONTENT) U_AT_LEAST_ONE(GROUP(CONTENT))
#define S_OPTIONAL(CONTENT) U_OPTIONAL(GROUP(CONTENT))
#define S_EITHER(CONTENT1, CONTENT2) \
	GROUP(U_EITHER(GROUP(CONTENT1), GROUP(CONTENT2)))

/*
 * #define the SAFE_REGEX macro before including to use the safe versions of the
 * repeat by default, instead of the unsafe versions
 */

#ifndef SAFE_REGEX

#define AT_MOST(...) U_AT_MOST(__VA_ARGS__)
#define AT_LEAST(...) U_AT_LEAST(__VA_ARGS__)
#define EXACTLY(...) U_EXACTLY(__VA_ARGS__)
#define WITHIN(...) U_WITHIN(__VA_ARGS__)

#define MANY(...) U_MANY(__VA_ARGS__)
#define AT_LEAST_ONE(...) U_AT_LEAST_ONE(__VA_ARGS__)
#define OPTIONAL(...) U_OPTIONAL(__VA_ARGS__)
#define EITHER(...) U_EITHER(__VA_ARGS__)

#else

#define AT_LEAST(...) S_AT_LEAST(__VA_ARGS__)
#define EXACTLY(...) S_EXACTLY(__VA_ARGS__)
#define WITHIN(...) S_WITHIN(__VA_ARGS__)

#define MANY(...) S_MANY(__VA_ARGS__)
#define AT_LEAST_ONE(...) S_AT_LEAST_ONE(__VA_ARGS__)
#define OPTIONAL(...) S_OPTIONAL(__VA_ARGS__)
#define EITHER(...) S_EITHER(__VA_ARGS__)

#endif
