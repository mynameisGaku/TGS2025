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
class Setter
{
public:
	Setter(C* pValue) { m_pValue = pValue; }

	const C& operator=(const C& value) const { *m_pValue = value; return *m_pValue; }
private:
	C* m_pValue;
};

template <class C>
class GetSet
{
public:
	GetSet(C* pValue) { m_pValue = pValue; }

	operator const C& () const { return *m_pValue; }
	const C& operator=(const C& value) const { *m_pValue = value; return *m_pValue; }
private:
	C* m_pValue;
};
