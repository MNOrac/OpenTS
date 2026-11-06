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

#pragma once

#include "abstype.h"

#include "house.hh"
#include "side.hh"
#include "typelist.h"

enum class POILocationType {
	OneQuarter,
	OneThird,
	Halfway,
	TwoThirds,
	ThreeQuarters,
	Furthest,
	RandomBand,
	RandomSafe,
	Random,
	Count,
};

/**********************************************************************
**	Each house has certain unalienable characteristics. This structure
**	elaborates these.
*/
class HouseTypeClass : public AbstractTypeClass
{
		typedef AbstractTypeClass BASECLASS;

	public:

		/*
		**	This is the house number (enum). This is a unique identification
		**	number for the house.
		*/
		HousesType HeapID;
		HousesType House;

		/*
		 * This is the side this house fights for, expressed as an index into the Sides list.
		 * The house is added to that side's roster as the rules are read, and the side then
		 * decides such things as which music it may play. If SIDE_NONE, then it belongs to
		 * no side.
		 */
		SideType Side;

		/*
		**	This points to the default remap table for this house.
		*/
		int Scheme;

		/*
		**	This controls the various general adjustments to the house owned
		**	unit and building ratings. The default value for these ratings is
		**	a fixed point number of 1.0.
		*/
		double FirepowerBias;
		double GroundspeedBias;
		double AirspeedBias;
		double ArmorBias;
		double ROFBias;
		double CostBias;
		double BuildSpeedBias;

		/*
		**	This is the filename suffix to use when creating a house specific
		**	file name. It is three characters long.
		*/
		char Suffix[4];

		/*
		**	This is a unique ASCII character used when constructing filenames. It
		**	serves a similar purpose as the "Suffix" element, but is only one
		**	character long.
		*/
		char Prefix;

		/*
		 * If this house may be picked as a country by a player setting up a multiplayer or
		 * skirmish game, then this flag will be true. It is what keeps the story only houses
		 * out of the country lists offered in the lobby.
		 */
		bool IsMultiplay;

		/*
		 * If this house takes no part in the multiplayer contest, then this flag will be
		 * true. A passive house is not treated as an opponent -- it is left out of the
		 * victory test and the score screen, and the computer players will not hunt for it.
		 */
		bool IsMultiplayPassive;

		/*
		 * If a wall may be credited to this house, then this flag will be true. A wall
		 * section belongs to whichever wall owning house has a building nearest it, so a
		 * house without this flag never inherits the walls around it.
		 */
		bool IsWallOwner;

		/// Unused
		bool IsSmartAI;


		/*
		 * POI Controls. A POI is a structure preconfigured in a House entry to be placed on
		 * the map at calculated, partially random points.
		 */
		TypeList<int> POIs;

		/*
		 * The minimum number of POIs that can be placed if a house that requires them is
		 * present in the game.
		 */
		int POIsBaseCount;

		/*
		 * The number of additional POIs to add per additional other player after the first.
		 */
		int POIsPerOpponent;

		/*
		 * The maximum number of POIs belonging to all instances of this house that are
		 * allowed to exist in the game. This prevents the number from getting out of hand
		 * if all eight players are the same house.
		 */
		int POIsMax;

		/*
		 * The POILocationType enum establishes the valid placement rules for a POI. Each
		 * POI in POIs uses the placement rule in the index it shares with the POILocations
		 * list. If either list is a different length, it will loop to make sure all POIs
		 * have a location rule and all location rules have a POI. If the POI count is higher
		 * than either, the lists will also loop to cover the shortfall.
		 */
		TypeList<POILocationType> POILocations;

		/*
		 * Controls for the RandomBand location rule. This rule picks a valid placement cell
		 * in a random band between these two values, which represent a minimum  and a maximum
		 * distance from the house's starting location.
		 */
		float POILocRandomBandMin;
		float POILocRandomBandMax;

		/*
		 * POISafeDistance prevents a POI from being placed within this distance of an occupied
		 * player start or another POI.
		 */
		int POISafeDistance;

		/*
		 * The jitter rules allow a random shift to be added to the placement of POIs that
		 * use a point-to-point placement rule. POIs whose rule places them along an axis
		 * between two player starts can use POILocJitterParallel to randomly shift placement
		 * along the axis by this number of cells. POILocJitterLateral adds jitter perpendicular
		 * to the axis.
		 */
		int POILocJitterParallel;
		int POILocJitterLateral;

		/*
		 * POIDoubleUpWithEnemies controls whether other players' POI rules can be batched
		 * together with this house. If they are, two opposing players' POIs at Halfway will be
		 * consolidated into one single POI. The same goes for one player's OneThird and an
		 * opposite player's TwoThirds, for instance.
		 */
		bool POIDoubleUpWithEnemies;


		//------------------------------------------------------------------------
		HouseTypeClass(char const * ininame = NULL);
		virtual ~HouseTypeClass() override;

		virtual ClassID Class_ID(void) const override;


		virtual void Serialize(SaveStreamClass & stream) override;

		virtual RTTIType Fetch_RTTI(void) const override;
		virtual void Compute_CRC(CRCEngine &) const override;
		virtual int Fetch_Heap_ID(void) const override;

		static HousesType From_Name(char const * name);
		static void One_Time(void);
		static HouseTypeClass * Find_Or_Make(char const * ininame);

		virtual bool Read_INI(CCINIClass const & ini) override;
};
