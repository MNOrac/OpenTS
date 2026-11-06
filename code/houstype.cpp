/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/HDATA.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : HDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 22, 1994                                                 *
 *                                                                                             *
 *                  Last Update : September 4, 1996 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   HouseTypeClass::As_Reference -- Fetches a reference to the house specified.               *
 *   HouseTypeClass::From_Name -- Fetch house pointer from its name.                           *
 *   HouseTypeClass::HouseTypeClass -- Constructor for house type objects.                     *
 *   HouseTypeClass::Init_Heap -- Allocate all heap objects for the house types.               *
 *   HouseTypeClass::One_Time -- One-time initialization                                       *
 *   HouseTypeClass::Read_INI -- Fetch the house control values from ini database.             *
 *   HouseTypeClass::Remap_Table -- Fetches the remap table for this house.                    *
 *   HouseTypeClass::operator delete -- Returns a house type object back to the heap.          *
 *   HouseTypeClass::operator new -- Allocates a house type class object from special heap.    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"

#include "houstype.h"

#include "ccini.h"
#include "crc.h"
#include "dbgprint.h"
#include "findmake.h"
#include "globals.h"
#include "savestream.h"
#include "side.h"
#include "sun.h"
#include "swizzle.h"
#include "tracker.h"
#include "vector.h"

#include <cmath>
#include <cstdio>
#include <string>


namespace {

bool Parse_POI_Location(char const * name, POILocationType & location)
{
	static struct {
		char const * Name;
		POILocationType Location;
	} const locations[] = {
		{"OneQuarter", POILocationType::OneQuarter},
		{"OneThird", POILocationType::OneThird},
		{"Halfway", POILocationType::Halfway},
		{"TwoThirds", POILocationType::TwoThirds},
		{"ThreeQuarters", POILocationType::ThreeQuarters},
		{"Furthest", POILocationType::Furthest},
		{"RandomBand", POILocationType::RandomBand},
		{"RandomSafe", POILocationType::RandomSafe},
		{"Random", POILocationType::Random},
	};

	for (auto const & entry : locations) {
		if (_stricmp(name, entry.Name) == 0) {
			location = entry.Location;
			return true;
		}
	}
	return false;
}


TypeList<POILocationType> Parse_POI_Locations(char const * section, std::string const & value)
{
	TypeList<POILocationType> locations;
	std::size_t start = 0;

	while (start < value.size()) {
		std::size_t end = value.find(',', start);
		if (end == std::string::npos) {
			end = value.size();
		}

		std::size_t first = value.find_first_not_of(" \t", start);
		if (first != std::string::npos && first < end) {
			std::size_t last = value.find_last_not_of(" \t", end - 1);
			std::string name = value.substr(first, last - first + 1);
			POILocationType location;
			if (Parse_POI_Location(name.c_str(), location)) {
				locations.Add(location);
			} else {
				DebugString("[%s] POILocations contains unknown value '%s'\n", section, name.c_str());
			}
		}

		start = end + 1;
	}

	return locations;
}

}


/***********************************************************************************************
 * HouseTypeClass::HouseTypeClass -- Constructor for house type objects.                       *
 *                                                                                             *
 *    This is the constructor for house type objects. This object holds the constant data      *
 *    for the house type.                                                                      *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
HouseTypeClass::HouseTypeClass(char const * ininame) :
	BASECLASS(ininame),
	House(HOUSE_NONE),
	HeapID(HOUSE_NONE),
	Side(SIDE_NONE),
	FirepowerBias(1.0),
	GroundspeedBias(1.0),
	AirspeedBias(1.0),
	ArmorBias(1.0),
	ROFBias(1.0),
	CostBias(1.0),
	BuildSpeedBias(1.0),
	Scheme(0),
	Prefix('A'),
	IsMultiplay(false),
	IsMultiplayPassive(false),
	IsWallOwner(true),
	IsSmartAI(false),
	POIs(),
	POIsBaseCount(0),
	POIsPerOpponent(0),
	POIsMax(-1),
	POILocations(),
	POILocRandomBandMin(0.0f),
	POILocRandomBandMax(1.0f),
	POISafeDistance(0),
	POILocJitter(0),
	POIDoubleUpWithEnemies(false)
{
	Create_ID();
	Suffix[0] = '\0';
	HouseTypes.Add(this);
	House = (HousesType)HouseTypes.ID(this);
	HeapID = (HousesType)HouseTypes.ID(this);
}


/// <summary>
/// Removes this house type from the game.
/// Everything that refers to this house type is detached from it before it is dropped from
/// the house type heap.
/// </summary>
HouseTypeClass::~HouseTypeClass(void)
{
	Detach_This_From_All(this, true);
	HouseTypes.Delete(this);
}


/***********************************************************************************************
 * HouseTypeClass::From_Name -- Fetch house pointer from its name.                             *
 *                                                                                             *
 *    This routine will convert the ASCII house name specified into a                          *
 *    real house number. Typically, this is used when processing a                             *
 *    scenario INI file.                                                                       *
 *                                                                                             *
 * INPUT:   name  -- ASCII name of house to process.                                           *
 *                                                                                             *
 * OUTPUT:  Returns with actual house number represented by the ASCII                          *
 *          name specified.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/21/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
HousesType HouseTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (int house = HOUSE_FIRST; house < HouseTypes.Count(); house++) {
			HouseTypeClass *ptr = HouseTypes[house];
			if (stricmp(ptr->Full_Name(), name) == 0 || stricmp(ptr->Name(), name) == 0) {
				return(ptr->House);
			}
		}
	}
	return(HOUSE_NONE);
}


/***********************************************************************************************
 * HouseTypeClass::Read_INI -- Fetch the house control values from ini database.               *
 *                                                                                             *
 *    This routine will fetch the rules controllable values for the house type from the        *
 *    INI database specified.                                                                  *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to fetch the house control values from.     *
 *                                                                                             *
 * OUTPUT:  bool; Was the house section found and processed?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool HouseTypeClass::Read_INI(CCINIClass const & ini)
{
	if (BASECLASS::Read_INI(ini)) {
		char buffer[32];
		ini.Get_String(Name(), "Suffix", "", buffer, sizeof(Suffix));
		if (strlen(buffer) != 0) {
			strcpy(Suffix, buffer);
		}

		Scheme = ini.Get_Scheme_Index(Name(), "Color", Scheme);

		char prefix[2];
		buffer[0] = Prefix;
		buffer[1] = '\0';
		ini.Get_String(Name(), "Prefix", buffer, prefix, sizeof(prefix));
		Prefix = prefix[0];

		FirepowerBias = ini.Get_Float(Name(), "Firepower", FirepowerBias);
		GroundspeedBias = ini.Get_Float(Name(), "Groundspeed", GroundspeedBias);
		AirspeedBias = ini.Get_Float(Name(), "Airspeed", AirspeedBias);
		ArmorBias = ini.Get_Float(Name(), "Armor", ArmorBias);
		ROFBias = ini.Get_Float(Name(), "ROF", ROFBias);
		CostBias = ini.Get_Float(Name(), "Cost", CostBias);
		BuildSpeedBias = ini.Get_Float(Name(), "BuildTime", BuildSpeedBias);

		IsMultiplay = ini.Get_Bool(Name(), "Multiplay", IsMultiplay);
		IsMultiplayPassive = ini.Get_Bool(Name(), "MultiplayPassive", IsMultiplayPassive);
		IsWallOwner = ini.Get_Bool(Name(), "WallOwner", IsWallOwner);
		IsSmartAI = ini.Get_Bool(Name(), "SmartAI", IsSmartAI);

		POIs = ini.Get_BuildingType_List(ini, Name(), "POIs", POIs);
		POIsBaseCount = ini.Get_Int(Name(), "POIsBaseCount", POIsBaseCount);
		POIsPerOpponent = ini.Get_Int(Name(), "POIsPerOpponent", POIsPerOpponent);
		POIsMax = ini.Get_Int(Name(), "POIsMax", POIsMax);
		std::string POILocations = ini.Get_String(Name(), "POILocations");
		if (!POILocations.empty()) {
			this->POILocations = Parse_POI_Locations(Name(), POILocations);
		}
		std::string POIRandomBands = ini.Get_String(Name(), "POILocRandomBands");
		if (!POIRandomBands.empty()) {
			float MinDistance;
			float MaxDistance;
			char trailing;
			if (std::sscanf(POIRandomBands.c_str(), " %f , %f %c", &MinDistance, &MaxDistance, &trailing) == 2 &&
				std::isfinite(MinDistance) && std::isfinite(MaxDistance) &&
				MinDistance >= 0.0f && MinDistance <= MaxDistance) {
				POILocRandomBandMin = MinDistance;
				POILocRandomBandMax = MaxDistance;
			} else {
				DebugString("[%s] POILocRandomBands must be two nonnegative percentages in ascending order\n", Name());
			}
		}
		POISafeDistance = ini.Get_Int(Name(), "POISafeDistance", POISafeDistance);
		POILocJitter = ini.Get_Int(Name(), "POILocJitter", POILocJitter);
		POIDoubleUpWithEnemies = ini.Get_Bool(Name(), "POIDoubleUpWithEnemies", POIDoubleUpWithEnemies);

		SideType oldside = Side;
		Side = ini.Get_Side(Name(), "Side", Side);

		// The side list is the roster of record, so a country it places is not moved by its own key.
		if (Side != oldside) {
			if (oldside != SIDE_NONE && Sides[oldside]->Houses.Is_In_List((int)House)) {
				DebugString("%s: Side=%s ignored; [Sides] places it under %s.\n", Name(), Side != SIDE_NONE ? (char const *)Sides[Side]->IniName : "<none>", (char const *)Sides[oldside]->IniName);
				Side = oldside;
			} else {
				if (Side != SIDE_NONE) {
					Sides[Side]->Houses.Add((int)House);
				}
			}
		}
		return(true);
	}
	return(false);
}


/// <summary>
/// Submits this house type to the game state checksum.
/// The network sync check uses this routine to prove that every machine in the game is
/// running with identical house rules.
/// </summary>
void HouseTypeClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(House);
	crc(Side);
	crc(Scheme);
	crc(FirepowerBias);
	crc(GroundspeedBias);
	crc(AirspeedBias);
	crc(ArmorBias);
	crc(ROFBias);
	crc(CostBias);
	crc(BuildSpeedBias);
	crc(Suffix, strlen(Suffix));
	crc(Prefix);
	crc(IsMultiplay);
	crc(POIs.Count());
	for (int index = 0; index < POIs.Count(); index++) {
		crc(POIs[index]);
	}
	crc(POIsBaseCount);
	crc(POIsPerOpponent);
	crc(POIsMax);
	crc(POILocations.Count());
	for (int index = 0; index < POILocations.Count(); index++) {
		crc(static_cast<int>(POILocations[index]));
	}
	crc(POILocRandomBandMin);
	crc(POILocRandomBandMax);
	crc(POISafeDistance);
	crc(POILocJitter);
	crc(POIDoubleUpWithEnemies);
}


/// <summary>
/// Lists the members this house type carries.
/// </summary>
/// <param name="stream">The stream carrying the members.</param>
void HouseTypeClass::Serialize(SaveStreamClass & stream)
{
	BASECLASS::Serialize(stream);

	stream.Serialize(HeapID);
	stream.Serialize(House);
	stream.Serialize(Side);
	stream.Serialize(Scheme);
	stream.Serialize(FirepowerBias);
	stream.Serialize(GroundspeedBias);
	stream.Serialize(AirspeedBias);
	stream.Serialize(ArmorBias);
	stream.Serialize(ROFBias);
	stream.Serialize(CostBias);
	stream.Serialize(BuildSpeedBias);
	stream.Serialize(Suffix);
	stream.Serialize(Prefix);
	stream.Serialize(IsMultiplay);
	stream.Serialize(IsMultiplayPassive);
	stream.Serialize(IsWallOwner);
	stream.Serialize(IsSmartAI);
}


ClassID HouseTypeClass::Class_ID(void) const
{
	return(ClassID_HouseTypeClass);
}


/// <summary>
/// Fetches the house type of the name specified, creating it if need be.
/// This routine is used while processing the rules and scenario INI databases, where a house
/// can be mentioned before it has been declared.
/// </summary>
/// <param name="ininame">The internal INI name of the house type wanted.</param>
/// <returns>Returns with a pointer to the matching house type.</returns>
HouseTypeClass * HouseTypeClass::Find_Or_Make(char const * ininame)
{
	return(TFind_Or_Make<HouseTypeClass>(ininame, HouseTypes));
}


/// <summary>
/// Fetches the RTTI type of this object.
/// </summary>
/// <returns>Returns with RTTI_HOUSETYPE.</returns>
RTTIType HouseTypeClass::Fetch_RTTI(void) const
{
	return(RTTI_HOUSETYPE);
}


/// <summary>
/// Fetches the heap index of this house type.
/// </summary>
/// <returns>Returns with the position of this house type within the house type heap.</returns>
int HouseTypeClass::Fetch_Heap_ID(void) const
{
	return(HeapID);
}
