/* Preference - Holds user-chosen preferences that are saved between sessions. */

#ifndef PREFERENCE_H
#define PREFERENCE_H

#include "EnumHelper.h"
#include "LuaManager.h"
#include "RageUtil.h"
<<<<<<< HEAD:itgmania/src/Preference.h

#include <cstddef>
#include <vector>


class XNode;

enum class PreferenceType
{
	// Allow reading and writing of the preference through Lua.
	// This is the default behavior.
	Mutable,

	// Mark the preference as read-only i.e. don't allow setting of the
	// preference through Lua.
	Immutable,

	// The preference is deprecated.
	// This deprecated flag will not be written to the Preferences file.
	// If this preference was deprecated in favor of a new one, see
	// TranslateDeprecatedFlags() to see the what it was deprecated in favor of.
	Deprecated,
};

=======
class XNode;

>>>>>>> origin/c++11:src/Preference.h
struct lua_State;
class IPreference
{
public:
<<<<<<< HEAD:itgmania/src/Preference.h
	IPreference( const RString& sName, PreferenceType type );
=======
	IPreference( const RString& sName );
>>>>>>> origin/c++11:src/Preference.h
	virtual ~IPreference();
	void ReadFrom( const XNode* pNode, bool bIsStatic );
	void WriteTo( XNode* pNode ) const;
	void ReadDefaultFrom( const XNode* pNode );

	virtual void LoadDefault() = 0;
	virtual void SetDefaultFromString( const RString &s ) = 0;

	virtual RString ToString() const = 0;
	virtual void FromString( const RString &s ) = 0;

	virtual void SetFromStack( lua_State *L );
	virtual void PushValue( lua_State *L ) const;

	const RString &GetName() const { return m_sName; }

	static IPreference *GetPreferenceByName( const RString &sName );
	static void LoadAllDefaults();
	static void ReadAllPrefsFromNode( const XNode* pNode, bool bIsStatic );
	static void SavePrefsToNode( XNode* pNode );
	static void ReadAllDefaultsFromNode( const XNode* pNode );

	RString GetName() { return m_sName; }
<<<<<<< HEAD:itgmania/src/Preference.h
	bool IsImmutable() { return m_bImmutable; }
private:
	RString	m_sName;
	bool m_bDoNotWrite;
	bool m_bImmutable;
=======
	void SetStatic( bool b ) { m_bIsStatic = b; }
private:
	RString	m_sName;
	bool m_bIsStatic;	// loaded from Static.ini?  If so, don't write to Preferences.ini
>>>>>>> origin/c++11:src/Preference.h
};

void BroadcastPreferenceChanged( const RString& sPreferenceName );

template <class T>
class Preference : public IPreference
{
public:
<<<<<<< HEAD:itgmania/src/Preference.h
	Preference( const RString& sName, const T& defaultValue, void (pfnValidate)(T& val) = nullptr, PreferenceType type = PreferenceType::Mutable ):
		IPreference( sName, type ),
=======
	Preference( const RString& sName, const T& defaultValue, void (pfnValidate)(T& val) = nullptr ):
		IPreference( sName ),
>>>>>>> origin/c++11:src/Preference.h
		m_currentValue( defaultValue ),
		m_defaultValue( defaultValue ),
		m_pfnValidate( pfnValidate )
	{
		LoadDefault();
	}

	RString ToString() const { return StringConversion::ToString<T>( m_currentValue ); }
	void FromString( const RString &s )
	{
		if( !StringConversion::FromString<T>(s, m_currentValue) )
			m_currentValue = m_defaultValue;
<<<<<<< HEAD:itgmania/src/Preference.h
		if( m_pfnValidate )
=======
		if( m_pfnValidate ) 
>>>>>>> origin/c++11:src/Preference.h
			m_pfnValidate( m_currentValue );
	}
	void SetFromStack( lua_State *L )
	{
		LuaHelpers::Pop<T>( L, m_currentValue );
		if( m_pfnValidate )
			m_pfnValidate( m_currentValue );
	}
	void PushValue( lua_State *L ) const
	{
		LuaHelpers::Push<T>( L, m_currentValue );
	}

	void LoadDefault()
	{
		m_currentValue = m_defaultValue;
	}
	void SetDefaultFromString( const RString &s )
	{
		T def = m_defaultValue;
		if( !StringConversion::FromString<T>(s, m_defaultValue) )
			m_defaultValue = def;
	}

	const T &Get() const
	{
		return m_currentValue;
	}
<<<<<<< HEAD:itgmania/src/Preference.h

=======
	
>>>>>>> origin/c++11:src/Preference.h
	const T &GetDefault() const
	{
		return m_defaultValue;
	}

<<<<<<< HEAD:itgmania/src/Preference.h
	operator const T &() const
	{
		return Get();
	}

=======
	operator const T () const
	{
		return Get();
	}
	
>>>>>>> origin/c++11:src/Preference.h
	void Set( const T& other )
	{
		m_currentValue = other;
		BroadcastPreferenceChanged( GetName() );
	}

	static Preference<T> *GetPreferenceByName( const RString &sName )
	{
		IPreference *pPreference = IPreference::GetPreferenceByName( sName );
		Preference<T> *pRet = dynamic_cast<Preference<T> *>(pPreference);
		return pRet;
	}

private:
	T m_currentValue;
	T m_defaultValue;
	void (*m_pfnValidate)(T& val);
};

/** @brief Utilities for working with Lua. */
namespace LuaHelpers { template<typename T> void Push( lua_State *L, const Preference<T> &Object ) { LuaHelpers::Push<T>( L, Object.Get() ); } }

template <class T>
class Preference1D
{
public:
	typedef Preference<T> PreferenceT;
<<<<<<< HEAD:itgmania/src/Preference.h
	std::vector<PreferenceT*> m_v;

	Preference1D( void pfn(size_t i, RString &sNameOut, T &defaultValueOut ), size_t N, PreferenceType type = PreferenceType::Mutable )
=======
	vector<PreferenceT*> m_v;
	
	Preference1D( void pfn(size_t i, RString &sNameOut, T &defaultValueOut ), size_t N )
>>>>>>> origin/c++11:src/Preference.h
	{
		for( size_t i=0; i<N; ++i )
		{
			RString sName;
			T defaultValue;
			pfn( i, sName, defaultValue );
<<<<<<< HEAD:itgmania/src/Preference.h
			m_v.push_back( new Preference<T>(sName, defaultValue, nullptr, type) );
=======
			m_v.push_back( new Preference<T>(sName, defaultValue) );
>>>>>>> origin/c++11:src/Preference.h
		}
	}

	~Preference1D()
	{
		for( size_t i=0; i<m_v.size(); ++i )
<<<<<<< HEAD:itgmania/src/Preference.h
			RageUtil::SafeDelete( m_v[i] );
=======
			SAFE_DELETE( m_v[i] );
>>>>>>> origin/c++11:src/Preference.h
	}
	const Preference<T>& operator[]( size_t i ) const
	{
		return *m_v[i];
	}
	Preference<T>& operator[]( size_t i )
	{
		return *m_v[i];
	}
};

#endif

/*
 * (c) 2001-2004 Chris Danford, Chris Gomez
 * All rights reserved.
<<<<<<< HEAD:itgmania/src/Preference.h
 *
=======
 * 
>>>>>>> origin/c++11:src/Preference.h
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
<<<<<<< HEAD:itgmania/src/Preference.h
 *
=======
 * 
>>>>>>> origin/c++11:src/Preference.h
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
