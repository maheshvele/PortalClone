//-----------------------------------------------------------------------------
// File: XbMemUnit.cpp
//
// Desc: Memory unit object wraps XGetDeviceChanges( XDEVICE_TYPE_MEMORY_UNIT )
//       and XMount/UnmountMU
//
// Hist: 01.29.01 - New for March XDK release
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "XbMemUnit.h"
#include <cassert>




//-----------------------------------------------------------------------------
// Name: CXBMemUnit()
// Desc: Creates an unmounted memory unit object
//-----------------------------------------------------------------------------
CXBMemUnit::CXBMemUnit( DWORD dwPort, DWORD dwSlot )
:
    m_dwPort( dwPort ),
    m_dwSlot( dwSlot ),
    m_chLogicalDrive( 0 )
{
    assert( dwPort == dwINVALID || dwPort >= XDEVICE_PORT0 );
    assert( dwPort == dwINVALID || dwPort <= XDEVICE_PORT3 );
    assert( dwSlot == dwINVALID || 
            dwSlot == XDEVICE_TOP_SLOT || dwSlot == XDEVICE_BOTTOM_SLOT );
}




//-----------------------------------------------------------------------------
// Name: CXBMemUnit()
// Desc: Copies the given memory unit. Note: You cannot mount a device more 
//       than once. CXBMemUnit is a lightweight wrapper that doesn't
//       implement reference counting. Therefore, the only safe copy is a 
//       copy of an unmounted MU. This function asserts if the source
//       MU is already mounted.
//-----------------------------------------------------------------------------
CXBMemUnit::CXBMemUnit( const CXBMemUnit& rhs )
:
    m_dwPort( rhs.m_dwPort ),
    m_dwSlot( rhs.m_dwSlot ),
    m_chLogicalDrive( 0 )
{
    assert( !rhs.IsMounted() );
    (VOID)rhs;
}



//-----------------------------------------------------------------------------
// Name: CXBMemUnit()
// Desc: Copies the given memory unit. Note: You cannot mount a device more 
//       than once. CXBMemUnit is a lightweight wrapper that doesn't
//       implement reference counting. Therefore, the only safe copy is a 
//       copy of an unmounted MU. This function asserts if the source
//       MU is already mounted.
//-----------------------------------------------------------------------------
CXBMemUnit& CXBMemUnit::operator=( const CXBMemUnit& rhs )
{
    assert( !rhs.IsMounted() );
    m_dwPort = rhs.m_dwPort;
    m_dwSlot = rhs.m_dwSlot;
    m_chLogicalDrive = 0;
    return *this;
}




//-----------------------------------------------------------------------------
// Name: ~CXBMemUnit()
// Desc: Unmounts the memory unit
//-----------------------------------------------------------------------------
CXBMemUnit::~CXBMemUnit()
{
    Unmount();
}




//-----------------------------------------------------------------------------
// Name: Insert()
// Desc: Sets the port and slot numbers. IsValid() will now return TRUE.
//-----------------------------------------------------------------------------
VOID CXBMemUnit::Insert( DWORD dwPort, DWORD dwSlot )
{
    assert( dwPort >= XDEVICE_PORT0 );
    assert( dwPort <= XDEVICE_PORT3 );
    assert( dwSlot == XDEVICE_TOP_SLOT || dwSlot == XDEVICE_BOTTOM_SLOT );

    if( m_dwPort == dwPort && m_dwSlot == dwSlot )
        return;

    Unmount();
    m_dwPort = dwPort;
    m_dwSlot = dwSlot;
}




//-----------------------------------------------------------------------------
// Name: Mount()
// Desc: Mounts the memory unit in preparation for saving/loading. If
//       successful, GetDrive() will contain the drive mapping for the MU.
//-----------------------------------------------------------------------------
BOOL CXBMemUnit::Mount( DWORD& dwError )
{
    assert( IsValid() );

    // If already mounted, do nothing
    if( IsMounted() )
        return TRUE;

    Unmount();
    CHAR chDrive;
    dwError = XMountMU( m_dwPort, m_dwSlot, &chDrive );
    if( dwError != ERROR_SUCCESS )
        return FALSE;

    m_chLogicalDrive = chDrive;
    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: Unmount()
// Desc: Unmounts the memory unit when it is no longer used. If successful,
//       calls to IsMounted() will return FALSE.
//-----------------------------------------------------------------------------
VOID CXBMemUnit::Unmount()
{
    if( IsMounted() )
    {
        DWORD dwSuccess = XUnmountMU( m_dwPort, m_dwSlot );
        assert( dwSuccess == ERROR_SUCCESS );
        (VOID)dwSuccess;
        m_chLogicalDrive = 0;
    }
}




//-----------------------------------------------------------------------------
// Name: Remove()
// Desc: Unmounts and invalidates the memory unit. Used when an MU is pulled
//       from its slot. If successful, calls to IsValid() will return FALSE.
//-----------------------------------------------------------------------------
VOID CXBMemUnit::Remove()
{
    Unmount();
    m_dwPort = dwINVALID;
    m_dwSlot = dwINVALID;
}




//-----------------------------------------------------------------------------
// Name: IsValid()
// Desc: Returns TRUE if the MU refers to a valid port and slot
//-----------------------------------------------------------------------------
BOOL CXBMemUnit::IsValid() const
{
    return( m_dwPort != dwINVALID && m_dwSlot != dwINVALID );
}




//-----------------------------------------------------------------------------
// Name: IsMounted()
// Desc: Returns TRUE if the MU is currently mounted
//-----------------------------------------------------------------------------
BOOL CXBMemUnit::IsMounted() const
{
    return( IsValid() && m_chLogicalDrive != 0 );
}




//-----------------------------------------------------------------------------
// Name: GetDrive()
// Desc: Returns the logical drive mapping for the MU. Returns 0 if the MU 
//       is not mounted.
//-----------------------------------------------------------------------------
CHAR CXBMemUnit::GetDrive() const
{
    assert( IsValid() );
    return m_chLogicalDrive;
}




//-----------------------------------------------------------------------------
// Name: GetPort()
// Desc: Returns the MU port (XDEVICE_PORT0-3)
//-----------------------------------------------------------------------------
DWORD CXBMemUnit::GetPort() const
{
    assert( IsValid() );
    return m_dwPort;
}




//-----------------------------------------------------------------------------
// Name: GetSlot()
// Desc: Returns the MU slot (XDEVICE_TOP_SLOT or XDEVICE_BOTTOM_SLOT)
//-----------------------------------------------------------------------------
DWORD CXBMemUnit::GetSlot() const
{
    assert( IsValid() );
    return m_dwSlot;
}




//-----------------------------------------------------------------------------
// Name: GetName()
// Desc: Get the full name of the device. Assumes the buffer holds at least
//       MAX_MUNAME + 32 characters. Does not require that the MU has been
//       mounted. Resulting name is of the form: "Xbox MU 2B (PersonalName)"
//-----------------------------------------------------------------------------
VOID CXBMemUnit::GetName( WCHAR* strPathBuffer ) const
{
    assert( strPathBuffer != NULL );
    assert( IsValid() );

    // Build the default name
    lstrcpyW( strPathBuffer, L"Xbox MU " );
    
    // Append port and slot
    DWORD dwLen = lstrlenW( strPathBuffer );
    strPathBuffer[dwLen++] = WCHAR( m_dwPort + '1' );
    strPathBuffer[dwLen++] = WCHAR( m_dwSlot + 'A' );
    strPathBuffer[dwLen] = 0;

    // TCR Memory Unit Personalization
    // If the player has named the memory unit, append the name
    WCHAR strName[ MAX_MUNAME ];
    DWORD dwSuccess;
    if( IsMounted() )
    {
        // If the MU is already mounted, use drive letter to extract name
        dwSuccess = XMUNameFromDriveLetter( m_chLogicalDrive, strName, MAX_MUNAME );
    }
    else
    {
        // If the MU is not mounted, use lightweight method.
        // This API is not currently implemented, but may appear in a
        // future release.
        // dwSuccess = XMUNameFromPortSlot( m_dwPort, m_dwSlot, strName, MAX_MUNAME );
        dwSuccess = 0;
        *strName = 0;
    }

    if( dwSuccess == ERROR_SUCCESS && *strName != 0 )
    {
        // If there was a valid personal name, append it in parens
        WCHAR strFmtName[ MAX_MUNAME + 3 ];
        wsprintfW( strFmtName, L" (%ls)", strName );
        lstrcatW( strPathBuffer, strFmtName );
    }
}




//-----------------------------------------------------------------------------
// Name: GetMemUnitSnapshot()
// Desc: Calls XGetDevices( XDEVICE_TYPE_MEMORY_UNIT ) to determine
//       what MUs are available. Returns a bitmask corresponding to the
//       installed MUs. May take on the order of 1 second.
//-----------------------------------------------------------------------------
DWORD CXBMemUnit::GetMemUnitSnapshot() // static
{
    return XGetDevices( XDEVICE_TYPE_MEMORY_UNIT );
}




//-----------------------------------------------------------------------------
// Name: GetMemUnitChanges()
// Desc: Calls XGetDeviceChanges( XDEVICE_TYPE_MEMORY_UNIT ) to determine
//       which MUs have been added or removed since the last call to
//       GetMemUnitChanges() or GetMemUnitShapshot(). Returns TRUE if there 
//       were any dynamic insertions or removals since the last call to
//       GetMemUnitChanges() or GetMemUnitSnapshot(). Parameters return
//       bitmasks indicating what devices have changed.
//-----------------------------------------------------------------------------
BOOL CXBMemUnit::GetMemUnitChanges( DWORD& dwInsertions, DWORD& dwRemovals ) // static
{
    return XGetDeviceChanges( XDEVICE_TYPE_MEMORY_UNIT, &dwInsertions, &dwRemovals );
}




//-----------------------------------------------------------------------------
// Name: GetMemUnitMask()
// Desc: Given an MU index, returns the matching bitmask used by 
//       XGetDevices( XDEVICE_TYPE_MEMORY_UNIT )
//-----------------------------------------------------------------------------
DWORD CXBMemUnit::GetMemUnitMask( DWORD i ) // static
{
    // The XGetDevices bitmask is formatted as follows:
    //
    //      0x00000001      port 0 top slot         i = 0
    //      0x00010000      port 0 bottom slot          1
    //      0x00000002      port 1 top slot             2
    //      0x00020000      port 1 bottom slot          3
    //      0x00000004      port 2 top slot             4
    //      0x00040000      port 2 bottom slot          5
    //      0x00000008      port 3 top slot             6
    //      0x00080000      port 3 bottom slot          7

    DWORD dwMask = 1 << GetMemUnitPort( i );
    if( GetMemUnitSlot( i ) == XDEVICE_BOTTOM_SLOT )
        dwMask <<= 16;

    return( dwMask );
}




//-----------------------------------------------------------------------------
// Name: GetMemUnitPort()
// Desc: Given an MU index, returns the port number for the MU
//-----------------------------------------------------------------------------
DWORD CXBMemUnit::GetMemUnitPort( DWORD i ) // static
{
    return( i / 2 );
}




//-----------------------------------------------------------------------------
// Name: GetMemUnitSlot()
// Desc: Given an MU index, returns the slot identifier for the MU
//-----------------------------------------------------------------------------
DWORD CXBMemUnit::GetMemUnitSlot( DWORD i ) // static
{
    return( ( i % 2 ) ? XDEVICE_BOTTOM_SLOT : XDEVICE_TOP_SLOT );
}
