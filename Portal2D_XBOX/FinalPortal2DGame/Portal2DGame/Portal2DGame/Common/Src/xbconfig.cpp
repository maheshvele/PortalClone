//-----------------------------------------------------------------------------
// File: XbConfig.cpp
//
// Desc: Config object wraps XQueryValue
//
// Hist: 03.02.01 - New for April XDK release
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "XbConfig.h"
#include <cassert>




//-----------------------------------------------------------------------------
// Name: FormatDateTime()
// Desc: Formats the incoming date/time value in the proper format for
//       the current language setting.
//
//       ftZulu must be the time in UTC format, e.g. result from FindFirstFile
//       strDate is the formatted date result. NULL allowed. Minimum 11 WCHARs.
//       strTime is the formatted time result. NULL allowed. Minimum 12 WCHARs.
//-----------------------------------------------------------------------------
VOID CXBConfig::FormatDateTime( const FILETIME& ftZulu, WCHAR* strDate, 
                                WCHAR* strTime, BOOL bIncludeSeconds )
{
    // Handle empty FILETIME
    if( ftZulu.dwLowDateTime == 0 && ftZulu.dwHighDateTime == 0 )
    {
        if( strDate != NULL )
            *strDate = 0;
        if( strTime != NULL )
            *strTime = 0;
        return;
    }

    // Convert to local time
    FILETIME ftTimeLocal;
    FileTimeToLocalFileTime( &ftZulu, &ftTimeLocal );

    // Extract date/time data
    SYSTEMTIME SystemTime;
    FileTimeToSystemTime( &ftTimeLocal, &SystemTime );

    // Format the date
    if( strDate != NULL )
    {
        const WCHAR* strDateFormat = L"%d/%d/%d";
        switch( XGetLanguage() )
        {
            case XC_LANGUAGE_ENGLISH:  // mm/dd/yyyy
                wsprintfW( strDate, strDateFormat, SystemTime.wMonth,
                           SystemTime.wDay, SystemTime.wYear );
                break;

            case XC_LANGUAGE_JAPANESE: // yyyy/mm/dd
            case XC_LANGUAGE_TCHINESE: // yyyy/mm/dd
            case XC_LANGUAGE_KOREAN:   // yyyy/mm/dd
                wsprintfW( strDate, strDateFormat, SystemTime.wYear,
                           SystemTime.wMonth, SystemTime.wDay  );
                break;

            case XC_LANGUAGE_GERMAN:   // dd.mm.yyyy
            case XC_LANGUAGE_ITALIAN:  // dd.mm.yyyy
                wsprintfW( strDate, L"%d.%d.%d", SystemTime.wDay, 
                           SystemTime.wMonth, SystemTime.wYear );
                break;

            case XC_LANGUAGE_FRENCH:     // dd/mm/yyyy
            case XC_LANGUAGE_SPANISH:    // dd/mm/yyyy
            case XC_LANGUAGE_PORTUGUESE: // dd/mm/yyyy
            default:
                wsprintfW( strDate, strDateFormat, SystemTime.wDay,
                           SystemTime.wMonth, SystemTime.wYear );
                break;
        }
    }

    // Format the time
    if( strTime != NULL )
    {
        const WORD wHalfDay = 12;
        WORD wHour12 = 0;
        BOOL bUse24Hour = TRUE;
        switch( XGetLanguage() )
        {
            case XC_LANGUAGE_ENGLISH:
            case XC_LANGUAGE_KOREAN:
            case XC_LANGUAGE_TCHINESE:
                bUse24Hour = FALSE;
                break;
            default:
                bUse24Hour = TRUE;
                break;
        }
        
        if( !bUse24Hour )
        {
            // Convert base 24 hours to base 12 hours
            wHour12 = SystemTime.wHour;
            if( wHour12 >= wHalfDay )
                wHour12 -= wHalfDay;
            if( wHour12 == 0 )
                wHour12 = wHalfDay;
        }

        // hh:mm
        wsprintfW( strTime, L"%d:%2.2d", bUse24Hour ? SystemTime.wHour : wHour12,
                   SystemTime.wMinute );

        // append :ss if desired
        if( bIncludeSeconds )
        {
            WCHAR strSeconds[3];
            wsprintfW( strSeconds, L":%2.2d", SystemTime.wSecond );
            lstrcatW( strTime, strSeconds );
        }

        // append AM/PM if not 24-hour clock
        if( !bUse24Hour )
            lstrcatW( strTime, ( SystemTime.wHour < wHalfDay ) ? L" AM" : L" PM" );
    }
}




//-----------------------------------------------------------------------------
// Name: FormatInt()
// Desc: Formats the incoming integer into the proper format for the current
//       language setting, including thousands separators. Minimum size for
//       strNumber is 15 bytes.
//-----------------------------------------------------------------------------
VOID CXBConfig::FormatInt( INT nValue, WCHAR* strNumber )
{
    const INT RADIX = 10;       // Base 10

    // Format normally; no separators here
    _itow( nValue, strNumber, RADIX );

    // Determine number of separators to insert, if any
    INT nLen = wcslen( strNumber );
    INT nSeparators = ( nLen - 1 ) / 3;
    if( nSeparators <= 0 )
        return;

    // Separator character is based on language
    WCHAR cSeparator = GetThousandsSeparator();

    // Insert separators in place via backward walk
    WCHAR* pDest = strNumber + nLen + nSeparators;
    const WCHAR* pSrc = strNumber + nLen;
    for( INT i = 0; i < nLen; ++i, --pDest, --pSrc )
    {
        *pDest = *pSrc;
        if( i && ( i % 3 == 0 ) )
        {
            --pDest;
            *pDest = cSeparator;
        }
    }
}




//-----------------------------------------------------------------------------
// Name: FormatFloat()
// Desc: Formats the incoming float into the proper format for the current
//       language setting. Includes thousands separators if desired.
//       nCount indicates number of chars after the decimal point. If
//       nCount is 0, the decimal point is not included. If nCount is less
//       than 0, the decimal point is not included if fValue > 1.0.
//       strNumber minimum size depends on fValue and nCount.
//-----------------------------------------------------------------------------
VOID CXBConfig::FormatFloat( DOUBLE fValue, WCHAR* strNumber, INT nCount,
                             BOOL bThousandsSep )
{
    // Format normally; no decimal point or separators here
    INT nDecimalPtPos;
    INT nSign;
    CHAR* strDigits = _fcvt( fValue, ( (nCount<0) ? 0 : nCount ), 
                              &nDecimalPtPos, &nSign );

    WCHAR cDecimal   = GetDecimalSymbol();
    WCHAR cSeparator = GetThousandsSeparator();

    // Sign
    if( nSign != 0 )
        *strNumber++ = '-';

    // If fValue < 1.0, include the zero before the decimal point
    if( nDecimalPtPos <= 0 )
    {
        *strNumber++ = '0';
        if( nCount > 0 )
        {
            *strNumber++ = cDecimal;
            for( INT i = nDecimalPtPos; i < 0; ++i )
                *strNumber++ = '0';
        }
    }

    // Copy the floating point digits, adding separators and the decimal symbol
    INT nOffset = 2 - ( ( nDecimalPtPos + 1 ) % 3 );
    for( INT i = 0; *strDigits ; ++i, ++strDigits )
    {
        *strNumber++ = (WCHAR)*strDigits;

        // Thousands separator
        if( ( bThousandsSep ) && 
            ( nDecimalPtPos > 3 ) && 
            ( i < nDecimalPtPos - 1 ) && 
            ( ( ( i + nOffset ) % 3 ) == 0 ) )
        {
            *strNumber++ = cSeparator;
        }

        // Decimal point
        if( i + 1 == nDecimalPtPos && nCount > 0 )
            *strNumber++ = cDecimal;
    }

    // Slam in the null char
    *strNumber = 0;
}




//-----------------------------------------------------------------------------
// Name: GetDecimalSymbol()
// Desc: Returns the character for the decimal symbol based on the current
//       language.
//-----------------------------------------------------------------------------
WCHAR CXBConfig::GetDecimalSymbol()
{
    switch( XGetLanguage() )
    {
        default:
        case XC_LANGUAGE_ENGLISH:
        case XC_LANGUAGE_KOREAN:
        case XC_LANGUAGE_TCHINESE:
        case XC_LANGUAGE_JAPANESE:   return( L'.' );
        case XC_LANGUAGE_GERMAN:
        case XC_LANGUAGE_FRENCH:
        case XC_LANGUAGE_SPANISH:
        case XC_LANGUAGE_PORTUGUESE:
        case XC_LANGUAGE_ITALIAN:    return( L',' );
    }
}




//-----------------------------------------------------------------------------
// Name: GetThousandsSeparator()
// Desc: Returns the character for the separator symbol based on the current
//       language.
//-----------------------------------------------------------------------------
WCHAR CXBConfig::GetThousandsSeparator()
{
    switch( XGetLanguage() )
    {
        default:
        case XC_LANGUAGE_ENGLISH:
        case XC_LANGUAGE_KOREAN:
        case XC_LANGUAGE_TCHINESE:
        case XC_LANGUAGE_JAPANESE:   return( L',' );
        case XC_LANGUAGE_FRENCH:     return( L' ' );
        case XC_LANGUAGE_GERMAN:
        case XC_LANGUAGE_SPANISH:
        case XC_LANGUAGE_PORTUGUESE:
        case XC_LANGUAGE_ITALIAN:    return( L'.' );
    }
}
