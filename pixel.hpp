#ifndef Pixel_HPP
#define Pixel_HPP

#include <cmath>
#include <iostream>

struct Pixel {
  Pixel() {}

  Pixel(double rIn, double gIn, double bIn, double xIn, double yIn) :
    r(rIn), g(gIn), b(bIn), x(xIn), y(yIn) {}


  Pixel operator -() const{
    return Pixel(-r, -g, -b, -x, -y);
  }

  void operator += (const Pixel& p){
    r += p.r;
    g += p.g;
    b += p.b;
    x += p.x;
    y += p.y;
  }

  void operator -= (const Pixel& p){
    r -= p.r;
    g -= p.g;
    b -= p.b;
    x -= p.x;
    y -= p.y;
  }


	void operator *= (double s){
		r *= s;
    g *= s;
    b *= s;
    x *= s;
    y *= s;
	}

  void operator /= (double s){
	  r /= s;
    g /= s;
    b /= s;
    x /= s;
    y /= s;
	}

	void copyColor (const Pixel& p) {
    r = p.r;
    g = p.g;
    b = p.b;
	}
  double r, g, b, x, y;
};


inline std::ostream& operator << (std::ostream& os, const Pixel& a) {
  os << "Pixel(" << a.r << ", " << a.g << ", " << a.b << ", " << a.x << ", " << a.y << ")";
  return os;
}

inline Pixel operator + (const Pixel& a, const Pixel& b) {
  return Pixel(a.r + b.r, a.g + b.g, a.b + b.b, a.x + b.x, a.y + b.y);
}

inline Pixel operator - (const Pixel& a, const Pixel& b) {
  return Pixel(a.r - b.r, a.g - b.g, a.b - b.b, a.x - b.x, a.y - b.y);
}

inline Pixel operator * (double s, const Pixel& a) {
  return Pixel(s * a.r, s * a.g, s * a.b, s * a.x, s * a.y);
}

inline Pixel operator * (const Pixel& a, double s) {
  return Pixel(s * a.r, s * a.g, s * a.b, s * a.x, s * a.y);
}


inline Pixel operator / (const Pixel& a, double s){
  return Pixel(a.r / s, a.g / s, a.b / s, a.x / s, a.y / s);
}

inline double Length(const Pixel& a) {
  return std::sqrt(a.r * a.r + a.g * a.g + a.b * a.b + a.x * a.x + a.y * a.y);
}

inline double LengthSquared(const Pixel& a) {
  return a.r * a.r + a.g * a.g + a.b * a.b + a.x * a.x + a.y * a.y;
}


struct PixelInfo {
  bool mandatory = false;
  int assignment = 0;
  double transaction = 0;
};



#endif // Pixel_HPP

