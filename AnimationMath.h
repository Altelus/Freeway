#ifndef _ANIMATION_MATH_H
#define _ANIMATION_MATH_H

#include "Include/SFML/Graphics.hpp"

template <typename lerpClass>
inline lerpClass LERP(const lerpClass p0, const lerpClass p1, const float t)
{
	// formula 1 (racing) overload -,+,*
	return ((p1 - p0)*t + p0);

	// formula 2 (not racing) overload +,*
	//result = (1.0f - t)*p0 + (t)*p1;
}


template <typename lerpClass>
inline lerpClass BLEND(const lerpClass p0, const lerpClass p1, const float t)
{
	result = (1.0f - t)*p0 + (t)*p1;
}

template <typename catmullromClass>
inline catmullromClass CatmullRom(const catmullromClass p_i_1, const catmullromClass p_i,
						const catmullromClass p_i1, const catmullromClass p_i2,
						const float t)
{
	float catmullromClass = 0.5f * 
		( 2*p_i 
		+ t*(-p_i_1 + p_i1) 
		+ t*t*(2*p_i_1 - 5*p_i + 4*p_i1 - p_i2)
		+ t*t*t*(-p_i_1 + 3*p_i - 3*p_i1 + p_i2));
}

template <typename bezierClass>
inline bezierClass Bezier(const bezierClass p[], 
	const unsigned int degree, //segments, # of points - 1
	const float t)
{
	if (degree == 0)
		return *p;//p[0]
	else
		return LERP(Bezier(p, degree-1, t), Bezier(p+1, degree-1, t), t);
}


#endif