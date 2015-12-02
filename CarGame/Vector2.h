/*
 * Vector2D.h
 *
 *  Created on: 6.7.2012
 *      Author: Tom
 */

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include "Math.h"

template<class T> class Vector2;

typedef Vector2<int> ivec2;
typedef Vector2<float> fvec2;
typedef Vector2<int> Point2;
typedef Vector2<float> fPoint2;
typedef Vector2<int> Dimension2;

template<class T>
class Vector2
{
public:
	Vector2();
	Vector2(const T aX, const T aY);
	Vector2(const Vector2<T>& aOther);

	Vector2<T>& set(T aNewX, T aNewY);
	Vector2<T>& set(const Vector2<T>& aOther);

	Vector2<T>& operator +=(const Vector2<T>& aOther);
	Vector2<T>& operator -=(const Vector2<T>& aOther);
	Vector2<T>& operator *=(float aScalar);
	Vector2<T>& operator /=(float aScalar);

	Vector2<T>	operator+(const Vector2<T>& aOther) const;
	Vector2<T>	operator-(const Vector2<T>& aOther) const;
	Vector2<T>	operator-(void) const;
	Vector2<T>	operator*(float aScalar) const;
	Vector2<T>	operator/(float aScalar) const;
	//Vector2<T>	operator*(const Vector2<T>& v) const;
	//Vector2<T>	operator/(const Vector2<T>& v) const;

	Vector2<T>& add(T aAddX, T aAddY);
    Vector2<T>& add(const Vector2<T>& aOther);
    Vector2<T>& sub(T aSubX, T aSubY);
	Vector2<T>& sub(const Vector2<T>& aOther);
	Vector2<T>& mul(float aScalar);
	Vector2<T>& div(float aScalar);

	bool operator==(const Vector2<T>& aOther) const;
	bool operator!=(const Vector2<T>& aOther) const;

	T dot(const Vector2<T>& aOther) const;
	T perpdot(const Vector2<T>& aOther) const;

	void normalize();
	Vector2<T> normalized() const;

	float lengthSquared() const;
	float length() const;

	float angleDeg();
	float angleRad();

	Vector2<T>& rotate(float aAngleRad);
	Vector2<T> lerp(float t, const Vector2<T>& aOther) const;

public:
	T mX;
	T mY;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>::Vector2()
{

};

//------------------------------------------------------------------------
template<class T> Vector2<T>::Vector2(const T aX, const T aY) : mX(aX), mY(aY)
{

};

//------------------------------------------------------------------------
template<class T> Vector2<T>::Vector2(const Vector2<T>& aOther) : mX(aOther.mX), mY(aOther.mY)
{

};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::set(T aNewX, T aNewY)
{
	mX = aNewX;
	mY = aNewY;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::set(const Vector2<T>& aOther)
{
	mX = aOther.mX;
	mY = aOther.mY;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::operator +=(const Vector2<T>& aOther)
{
	mX += aOther.mX;
	mY += aOther.mY;
	return *this;
}

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::operator -=(const Vector2<T>& aOther)
{
	mX -= aOther.mX;
	mY -= aOther.mY;
	return *this;
}

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::operator *=(float aScalar)
{
	mX *= aScalar;
	mY *= aScalar;
	return *this;
}

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::operator /=(float aScalar)
{
	float m = 1.0f / aScalar;
	mX *= m;
	mY *= m;
	return *this;
}

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::operator+(const Vector2<T>& aOther) const
{
	return Vector2<T>(mX + aOther.mX, mY + aOther.mY);
}

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::operator-(const Vector2<T>& aOther) const
{
	return Vector2<T>(mX - aOther.mX, mY - aOther.mY);
}

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::operator-(void) const
{
	return Vector2<T>(-mX, -mY);
}

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::operator*(float aScalar) const
{
	return Vector2<T>(mX * aScalar, mY * aScalar);
}

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::operator/(float aScalar) const
{
	float m = 1.0 / aScalar;
	return Vector2<T>(mX * m, mY * m);
}

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::add(T aAddX, T aAddY)
{
	mX += aAddX;
	mY += aAddY;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::add(const Vector2<T>& aOther)
{
	mX += aOther.mX;
	mY += aOther.mY;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::sub(T aSubX, T aSubY)
{
	mX -= aSubX;
	mY -= aSubY;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::sub(const Vector2<T>& aOther)
{
	mX -= aOther.mX;
	mY -= aOther.mY;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::mul(float aScalar)
{
	mX *= aScalar;
	mY *= aScalar;
	return *this;
};

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::div(float aScalar)
{
	float m = 1.0f / aScalar;
	mX *= m;
	mY *= m;
	return *this;
};

//------------------------------------------------------------------------
template<class T> bool Vector2<T>::operator==(const Vector2<T>& aOther) const
{
	return (mX == aOther.mX) && (mY == aOther.mY);
};

//------------------------------------------------------------------------
template<class T> bool Vector2<T>::operator!=(const Vector2<T>& aOther) const
{
	return (mX != aOther.mX) || (mY != aOther.mY);
};

//------------------------------------------------------------------------
template<class T> T Vector2<T>::dot(const Vector2<T>& aOther) const
{
	return mX * aOther.mX + mY * aOther.mY;
};

//------------------------------------------------------------------------
template<class T>T Vector2<T>::perpdot(const Vector2<T>& aOther) const
{
	return mX * aOther.mY - mY * aOther.mX;
};

//------------------------------------------------------------------------
template<class T> void Vector2<T>::normalize()
{
	float len = length();
	if (len != 0)
	{
		float m = 1.0f / len;
		mX *= m;
		mY *= m;
	}
};

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::normalized() const
{
	Vector2<T> v(*this);
	v.normalize();
	return v;
};

//------------------------------------------------------------------------
template<class T> float Vector2<T>::lengthSquared() const
{
	return mX * mX + mY * mY;
};

//------------------------------------------------------------------------
template<class T> float Vector2<T>::length() const
{
	return Math::sqrt(lengthSquared());
};

//------------------------------------------------------------------------
template<class T> float Vector2<T>::angleDeg()
{
    float angle = Math::atan2(mY, mX) * TO_DEGREES;
    if (angle < 0)
        angle += 360;
    return angle;
}

//------------------------------------------------------------------------
template<class T> float Vector2<T>::angleRad()
{
    float angle = Math::atan2(mY, mX);
    if (angle < 0)
        angle += 2 * PI;
    return angle;
}

//------------------------------------------------------------------------
template<class T> Vector2<T>& Vector2<T>::rotate(float aAngleRad)
{
	float s = Math::sin(aAngleRad);
	float c = Math::cos(aAngleRad);

	float tmpx = mX * c - mY * s;
	float tmpy = mX * s + mY * c;

	mX = tmpx; mY = tmpy;
	return *this;
}

//------------------------------------------------------------------------
template<class T> Vector2<T> Vector2<T>::lerp(float t, const Vector2<T>& aOther) const
{
	return Vector2(mX * (1 - t) + aOther.mX * t, mY * (1 - t) + aOther.mY * t);
};

#endif /* VECTOR2D_H_ */
