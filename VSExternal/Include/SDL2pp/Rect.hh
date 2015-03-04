/*
  libSDL2pp - C++11 bindings/wrapper for SDL2
  Copyright (C) 2013-2015 Dmitry Marakasov <amdmi3@amdmi3.ru>

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

#ifndef SDL2PP_RECT_HH
#define SDL2PP_RECT_HH

#include <SDL/SDL_rect.h>

#include <SDL2pp/Optional.hh>

struct SDL_Rect;

namespace SDL2pp {

class Point;

////////////////////////////////////////////////////////////
/// \brief 2D rectangle
///
/// \ingroup geometry
///
/// \headerfile SDL2pp/Rect.hh
///
/// This class is public-derived from SDL_Rect structure,
/// may generally used as it if passed via pointer or
/// reference. It also supports direct access to x, y, w
/// and h members.
///
/// \see http://wiki.libsdl.org/SDL_Rect
///
////////////////////////////////////////////////////////////
class Rect : public SDL_Rect {
public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates a Rect(0, 0, 0, 0)
	///
	////////////////////////////////////////////////////////////
	Rect();

	////////////////////////////////////////////////////////////
	/// \brief Construct a rect from existing SDL_Rect
	///
	/// \param[in] rect Existing SDL_Rect
	///
	////////////////////////////////////////////////////////////
	Rect(const SDL_Rect& rect);

	////////////////////////////////////////////////////////////
	/// \brief Construct the rect from given corner coordinates, and size
	///
	/// \param[in] corner Coordinates of the top left rectangle corner
	/// \param[in] size Dimensions of the rectangle
	///
	////////////////////////////////////////////////////////////
	Rect(const Point& corner, const Point& size);

	////////////////////////////////////////////////////////////
	/// \brief Construct the rect from given corner coordinates, width and height
	///
	/// \param[in] x X coordinate of the top left rectangle corner
	/// \param[in] y Y coordinate of the top left rectangle corner
	/// \param[in] w Width of the rectangle
	/// \param[in] h Height of the rectangle
	///
	////////////////////////////////////////////////////////////
	Rect(int x, int y, int w, int h);

	////////////////////////////////////////////////////////////
	/// \brief Construct the rect from given center coordinates, width and height
	///
	/// \param[in] cx X coordinate of the rectangle center
	/// \param[in] cy Y coordinate of the rectangle center
	/// \param[in] w Width of the rectangle
	/// \param[in] h Height of the rectangle
	///
	////////////////////////////////////////////////////////////
	static Rect FromCenter(int cx, int cy, int w, int h);

	////////////////////////////////////////////////////////////
	/// \brief Construct the rect from given center coordinates and size
	///
	/// \param[in] center Coordinates of the rectangle center
	/// \param[in] size Dimensions of the rectangle
	///
	////////////////////////////////////////////////////////////
	static Rect FromCenter(const Point& center, const Point& size);

	////////////////////////////////////////////////////////////
	/// \brief Construct the rect from given corners coordinates
	///
	/// \param[in] x1 X coordinate of the top left rectangle corner
	/// \param[in] y1 Y coordinate of the top left rectangle corner
	/// \param[in] x2 X coordinate of the bottom right rectangle corner
	/// \param[in] y2 Y coordinate of the bottom right rectangle corner
	///
	////////////////////////////////////////////////////////////
	static Rect FromCorners(int x1, int y1, int x2, int y2);

	////////////////////////////////////////////////////////////
	/// \brief Construct the rect from given centers coordinates
	///
	/// \param[in] p1 Coordinates of the top left rectangle corner
	/// \param[in] p2 Coordinates of the bottom right rectangle corner
	///
	////////////////////////////////////////////////////////////
	static Rect FromCorners(const Point& p1, const Point& p2);

	////////////////////////////////////////////////////////////
	/// \brief Copy constructor
	///
	////////////////////////////////////////////////////////////
	Rect(const Rect&) noexcept = default;

	////////////////////////////////////////////////////////////
	/// \brief Move constructor
	///
	////////////////////////////////////////////////////////////
	Rect(Rect&&) noexcept = default;

	////////////////////////////////////////////////////////////
	/// \brief Assignment operator
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& operator=(const Rect&) noexcept = default;

	////////////////////////////////////////////////////////////
	/// \brief Move assignment operator
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& operator=(Rect&&) noexcept = default;

	////////////////////////////////////////////////////////////
	/// \brief Equality operator
	///
	/// \param[in] other Rect to compare to
	///
	/// \returns True if two rectangles are identical
	///
	////////////////////////////////////////////////////////////
	bool operator==(const Rect& other) const;

	////////////////////////////////////////////////////////////
	/// \brief Inequality operator
	///
	/// \param[in] other Rect to compare to
	///
	/// \returns True if two rectangles are not identical
	///
	////////////////////////////////////////////////////////////
	bool operator!=(const Rect& other) const;

	////////////////////////////////////////////////////////////
	/// \brief Get X coordinate of the rect corner
	///
	/// \returns X coordinate of the rect corner
	///
	////////////////////////////////////////////////////////////
	int GetX() const;

	////////////////////////////////////////////////////////////
	/// \brief Set X coordinate of the rect corner
	///
	/// \param[in] nx New X coordinate value
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& SetX(int nx);

	////////////////////////////////////////////////////////////
	/// \brief Get Y coordinate of the rect corner
	///
	/// \returns Y coordinate of the rect corner
	///
	////////////////////////////////////////////////////////////
	int GetY() const;

	////////////////////////////////////////////////////////////
	/// \brief Set Y coordinate of the rect corner
	///
	/// \param[in] ny New Y coordinate value
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& SetY(int ny);

	////////////////////////////////////////////////////////////
	/// \brief Get width of the rect
	///
	/// \returns Width of the rect
	///
	////////////////////////////////////////////////////////////
	int GetW() const;

	////////////////////////////////////////////////////////////
	/// \brief Set width of the rect
	///
	/// \param[in] nw New width of the rect
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& SetW(int nw);

	////////////////////////////////////////////////////////////
	/// \brief Get height of the rect
	///
	/// \returns Height of the rect
	///
	////////////////////////////////////////////////////////////
	int GetH() const;

	////////////////////////////////////////////////////////////
	/// \brief Set height of the rect
	///
	/// \param[in] nh New height of the rect
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& SetH(int nh);

	////////////////////////////////////////////////////////////
	/// \brief Get X coordinate of the rect second corner
	///
	/// \returns X coordinate of the rect second corner
	///
	////////////////////////////////////////////////////////////
	int GetX2() const;

	////////////////////////////////////////////////////////////
	/// \brief Set X coordinate of the rect second corner
	///
	/// \param[in] x2 New X coordinate value
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& SetX2(int x2);

	////////////////////////////////////////////////////////////
	/// \brief Get Y coordinate of the rect second corner
	///
	/// \returns Y coordinate of the rect second corner
	///
	/// This modifies rectangle width internally
	///
	////////////////////////////////////////////////////////////
	int GetY2() const;

	////////////////////////////////////////////////////////////
	/// \brief Set Y coordinate of the rect second corner
	///
	/// \param[in] y2 New Y coordinate value
	///
	/// This modifies rectangle height internally
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& SetY2(int y2);

	////////////////////////////////////////////////////////////
	/// \brief Check whether the rect contains given point
	///
	/// \param[in] x X coordinate of a point
	/// \param[in] y Y coordinate of a point
	///
	/// \returns True if the point is contained in the rect
	///
	////////////////////////////////////////////////////////////
	bool Contains(int x, int y) const;

	////////////////////////////////////////////////////////////
	/// \brief Check whether the rect contains given point
	///
	/// \param[in] point Point to check
	///
	/// \returns True if the point is contained in the rect
	///
	////////////////////////////////////////////////////////////
	bool Contains(const Point& point) const;

	////////////////////////////////////////////////////////////
	/// \brief Check whether the rect contains another rect
	///
	/// \param[in] rect Rect to check
	///
	/// \returns True if the checked rect is contained in this rect
	///
	////////////////////////////////////////////////////////////
	bool Contains(const Rect& rect) const;

	////////////////////////////////////////////////////////////
	/// \brief Check whether the rect intersects another rect
	///
	/// \param[in] rect Rect to check
	///
	/// \returns True if rectangles intersect
	///
	////////////////////////////////////////////////////////////
	bool Intersects(const Rect& rect) const;

	////////////////////////////////////////////////////////////
	/// \brief Calculate union with another rect
	///
	/// \param[in] rect Rect to union with
	///
	/// \returns Rect representing union of two rectangles
	///
	////////////////////////////////////////////////////////////
	Rect GetUnion(const Rect& rect) const;

	////////////////////////////////////////////////////////////
	/// \brief Union rect with another rect
	///
	/// \param[in] rect Rect to union with
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& Union(const Rect& rect);

	////////////////////////////////////////////////////////////
	/// \brief Get a rect extended by specified amount of pixels
	///
	/// \param[in] amount Number of pixels to extend by
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect GetExtension(unsigned int amount) const;

	////////////////////////////////////////////////////////////
	/// \brief Extend a rect by specified amount of pixels
	///
	/// \param[in] amount Number of pixels to extend by
	///
	/// \returns Extended rect
	///
	////////////////////////////////////////////////////////////
	Rect& Extend(unsigned int amount);

	////////////////////////////////////////////////////////////
	/// \brief Calculate intersection with another rect
	///
	/// \param[in] rect Rect to intersect with
	///
	/// \returns Rect representing intersection area or NullOpt if there was no intersection
	///
	////////////////////////////////////////////////////////////
	Optional<Rect> GetIntersection(const Rect& rect) const;

	////////////////////////////////////////////////////////////
	/// \brief Calculate the intersection of a rectangle and line segment
	///
	/// \param[in,out] x1 Starting X-coordinate of the line
	/// \param[in,out] y1 Starting Y-coordinate of the line
	/// \param[in,out] x2 Ending X-coordinate of the line
	/// \param[in,out] y2 Ending Y-coordinate of the line
	///
	/// \returns True if there is an intersection, false otherwise
	///
	/// This function is used to clip a line segment to a
	/// rectangle. A line segment contained entirely within the
	/// rectangle or that does not intersect will remain unchanged.
	/// A line segment that crosses the rectangle at either or both
	/// ends will be clipped to the boundary of the rectangle and
	/// the new coordinates saved in x1, y1, x2, and/or y2 as
	/// necessary.
	///
	////////////////////////////////////////////////////////////
	bool IntersectLine(int& x1, int& y1, int& x2, int& y2) const;

	////////////////////////////////////////////////////////////
	/// \brief Calculate the intersection of a rectangle and line segment
	///
	/// \param[in,out] p1 Starting coordinates of the line
	/// \param[in,out] p2 Ending coordinates of the line
	///
	/// \returns True if there is an intersection, false otherwise
	///
	/// This function is used to clip a line segment to a
	/// rectangle. A line segment contained entirely within the
	/// rectangle or that does not intersect will remain unchanged.
	/// A line segment that crosses the rectangle at either or both
	/// ends will be clipped to the boundary of the rectangle and
	/// the new coordinates saved in p1 and/or p2 as necessary.
	///
	////////////////////////////////////////////////////////////
	bool IntersectLine(Point& p1, Point& p2) const;

	////////////////////////////////////////////////////////////
	/// \brief Get rectangle moved by a given offset
	///
	/// \param[in] offset Point specifying an offset
	///
	/// \returns Moved rectangle
	///
	////////////////////////////////////////////////////////////
	Rect operator+(const Point& offset) const;

	////////////////////////////////////////////////////////////
	/// \brief Get rectangle moved by an opposite of given offset
	///
	/// \param[in] offset Point specifying an offset
	///
	/// \returns Moved rectangle
	///
	////////////////////////////////////////////////////////////
	Rect operator-(const Point& offset) const;

	////////////////////////////////////////////////////////////
	/// \brief Move by then given offset
	///
	/// \param[in] offset Point specifying an offset
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& operator+=(const Point& offset);

	////////////////////////////////////////////////////////////
	/// \brief Move by an opposite of the given offset
	///
	/// \param[in] offset Point specifying an offset
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Rect& operator-=(const Point& offset);
};

}

////////////////////////////////////////////////////////////
/// \brief Stream output operator overload for SDL2pp::Rect
///
/// \param[in] stream Stream to output to
/// \param[in] rect Rect to output
///
/// \returns stream
///
////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& stream, const SDL2pp::Rect& rect);

#endif
