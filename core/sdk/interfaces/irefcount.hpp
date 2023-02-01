#pragma once
// used: InterlockedDecrement
#include <windows.h>

class i_ref_counted
{
public:
	virtual int add_reference( ) = 0;
	virtual int release( ) = 0;
};

class c_ref_counted
{
public:
	virtual void destructor( char bDelete ) = 0;
	virtual bool on_final_release( ) = 0;

	void release( )
	{
		if (InterlockedDecrement( &vlRefCount ) == 0 && on_final_release( ))
			destructor( 1 );
	}

private:
	volatile long vlRefCount;
};

template <class T>
class c_base_auto_ptr
{
public:
	c_base_auto_ptr( ) :
		pObject( nullptr ) { }

	c_base_auto_ptr( T* pObject ) :
		pObject( pObject ) { }

	operator const void* () const
	{
		return pObject;
	}

	operator void* () const
	{
		return pObject;
	}

	operator const T* () const
	{
		return pObject;
	}

	operator const T* ()
	{
		return pObject;
	}

	operator T* ()
	{
		return pObject;
	}

	int	operator=( int i )
	{
		pObject = nullptr;
		return 0;
	}

	T* operator=( T* pSecondObject )
	{
		pObject = pSecondObject;
		return pSecondObject;
	}

	bool operator!( ) const
	{
		return (!pObject);
	}

	bool operator==( const void* pSecondObject ) const
	{
		return (pObject == pSecondObject);
	}

	bool operator!=( const void* pSecondObject ) const
	{
		return (pObject != pSecondObject);
	}

	bool operator==( T* pSecondObject ) const
	{
		return operator==( static_cast<void*>(pSecondObject) );
	}

	bool operator!=( T* pSecondObject ) const
	{
		return operator!=( static_cast<void*>(pSecondObject) );
	}

	bool operator==( const c_base_auto_ptr<T>& pSecondPtr ) const
	{
		return operator==( static_cast<const void*>(pSecondPtr) );
	}

	bool operator!=( const c_base_auto_ptr<T>& pSecondPtr ) const
	{
		return operator!=( static_cast<const void*>(pSecondPtr) );
	}

	T* operator->( )
	{
		return pObject;
	}

	T& operator*( )
	{
		return *pObject;
	}

	T** operator&( )
	{
		return &pObject;
	}

	const T* operator->( ) const
	{
		return pObject;
	}

	const T& operator*( ) const
	{
		return *pObject;
	}

	T* const* operator&( ) const
	{
		return &pObject;
	}

protected:
	c_base_auto_ptr( const c_base_auto_ptr<T>& pSecondPtr ) :
		pObject( pSecondPtr.pObject ) { }

	void operator=( const c_base_auto_ptr<T>& pSecondPtr )
	{
		pObject = pSecondPtr.pObject;
	}

	T* pObject;
};

template <class T>
class c_ref_ptr : public c_base_auto_ptr<T>
{
	typedef c_base_auto_ptr<T> c_base_class;
public:
	c_ref_ptr( ) { }

	c_ref_ptr( T* pInit )
		: c_base_class( pInit ) { }

	c_ref_ptr( const c_ref_ptr<T>& pRefPtr )
		: c_base_class( pRefPtr ) { }

	//~c_ref_ptr( )
	//{
	//	if (c_base_class::pObject != nullptr)
	//		c_base_class::pObject->release( );
	//}

	void operator=( const c_ref_ptr<T>& pSecondRefPtr )
	{
		c_base_class::operator=( pSecondRefPtr );
	}

	int operator=( int i )
	{
		return c_base_class::operator=( i );
	}

	T* operator=( T* pSecond )
	{
		return c_base_class::operator=( pSecond );
	}

	operator bool( ) const
	{
		return !c_base_class::operator!( );
	}

	operator bool( )
	{
		return !c_base_class::operator!( );
	}

	void safe_release( )
	{
		if (c_base_class::pObject != nullptr)
			c_base_class::pObject->release( );

		c_base_class::pObject = nullptr;
	}

	void assign_add_reference( T* pFrom )
	{
		if (pFrom != nullptr)
			pFrom->add_reference( );

		safe_release( );
		c_base_class::pObject = pFrom;
	}

	void add_reference_assign_to( T*& pTo )
	{
		if (c_base_class::pObject != nullptr)
			c_base_class::pObject->add_reference( );

		safe_release( pTo );
		pTo = c_base_class::pObject;
	}
};
