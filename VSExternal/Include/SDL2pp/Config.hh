/*
  libSDL2pp - C++11 bindings/wrapper for SDL2
  Copyright (C) 2014 Dmitry Marakasov <amdmi3@amdmi3.ru>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SDL2PP_CONFIG_HH
#define SDL2PP_CONFIG_HH

#define SDL2PP_MAJOR_VERSION 0
#define SDL2PP_MINOR_VERSION 7
#define SDL2PP_PATCH_VERSION 0

#define SDL2PP_VERSION "0.7.0"

/* #undef SDL2PP_WITH_IMAGE */
/* #undef SDL2PP_WITH_TTF */
/* #undef SDL2PP_WITH_2_0_4 */
/* #undef SDL2PP_WITH_EXPERIMENTAL_OPTIONAL */
/* #undef SDL2PP_WITH_DEPRECATED */

#if defined(SDL2PP_WITH_DEPRECATED)
#	define SDL2PP_DEPRECATED [[deprecated]]
#else
#	define SDL2PP_DEPRECATED
#endif

#endif
