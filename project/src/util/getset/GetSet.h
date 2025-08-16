#pragma once

template <class C>
class Getter
{
public:
	Getter(C* pValue) { m_pValue = pValue; }

	operator const C& () const { return *m_pValue; }
private:
	C* m_pValue;
};


template <class C>
class GetSet
{
};
