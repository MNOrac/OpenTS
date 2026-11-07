---
title: Points of interest
summary: Configures structures placed near active player starts as skirmish and multiplayer matches begin.
category: maps-scenarios
keys:
  - POIs
  - POIsBaseCount
  - POIsPerExtraPlayer
  - POIsMax
  - POILocations
  - POILocRandomBands
  - POISafeDistance
  - POILocJitterParallel
  - POILocJitterLateral
  - POIDoubleUpWithEnemies
  - POIDoubleUpWithAllies
  - POINeutral
related:
  - type: system
    id: starting-forces
---

OpenTS tries to place points of interest (POIs) while a skirmish or multiplayer match starts, after it creates starting forces. [`POINeutral`](/keys/poineutral/) makes them neutral-owned by default; setting it to `no` gives each POI to the house that requested it. Each POI request pairs one building entry with one location rule. Campaign missions and debug maps do not run this placement pass.

The game counts houses with a valid spawn waypoint occupied by that house. It includes allies and excludes observers, defeated houses, and passive houses. A house without an occupied numbered start does not request POIs.

## Request counts

For each active house of a type that has both [`POIs`](/keys/pois/) and [`POILocations`](/keys/poilocations/) configured, the game calculates a per-house request count:

`max(0, POIsBaseCount + POIsPerExtraPlayer × max(0, active houses − 2))`

[`POIsBaseCount`](/keys/poisbasecount/) sets the base. [`POIsPerExtraPlayer`](/keys/poisperextraplayer/) adds requests for each active house after the second, regardless of alliance or house type. The game multiplies this count by the number of active instances of the type, then applies [`POIsMax`](/keys/poismax/) to that combined total when the setting is zero or greater. It divides the result evenly across the instances; any remainder goes to houses in ascending start-waypoint order. A negative `POIsMax` leaves the total uncapped.

Each request takes its building and location rule from the corresponding list entry, cycling through each list independently. If a location rule needs another start, the game chooses the first eligible active house in start-waypoint order that is not this house and has not already been assigned for that same rule. Allies can be selected. If no such house remains, the request is skipped.

## Location rules

`OneQuarter`, `OneThird`, `Halfway`, `TwoThirds`, and `ThreeQuarters` place the request at the named fraction of the line from this house's start to its selected other start. Reciprocal fractional requests for the same building and pair of starts can share when either requester enables the setting for their relationship. [`POIDoubleUpWithEnemies=yes`](/keys/poidoubleupwithenemies/) permits enemy requests to share only when both are neutral-owned. [`POIDoubleUpWithAllies=yes`](/keys/poidoubleupwithallies/) permits requests from mutual allies to share when both use the same ownership setting. A shared neutral POI belongs to Neutral; a shared player-owned POI is randomly assigned to one of the two requesting houses. For example, one house's `OneThird` and the other house's `TwoThirds` identify the same cell.

`Behind` searches map cells on the rear half of a one-cell-thick ring at or just beyond this house type's `POISafeDistance`, starting closest to directly behind this house's start and continuing around the ring until a placement succeeds. The game then tries random cells if no ring cell works. With `POISafeDistance=0`, the only configured candidate is this house's start cell. If the two starts occupy the same cell, the game has no direction and uses the random fallback.

`Furthest` selects a random valid cell near the perpendicular bisector halfway between the two starts. `RandomBand` selects a random cell whose straight-line distance from this house's start falls within the fractions set by [`POILocRandomBands`](/keys/poilocrandombands/). `RandomSafe` selects among cells that satisfy the safe-distance checks. `Random` picks a random map cell. `Furthest`, `RandomBand`, `RandomSafe`, and `Random` cannot share a request.

[`POILocJitterParallel`](/keys/poilocjitterparallel/) and [`POILocJitterLateral`](/keys/poilocjitterlateral/) apply to two-start rules, including `Behind`, `Furthest`, and `RandomBand`. Each placement attempt chooses an independent signed offset up to the configured maximum along the start-to-start line and perpendicular to it, then clips the result to the map. The game does not apply these settings to `RandomSafe`, `Random`, or the random fallback pass. Jittered candidates must still pass all placement checks.

## Placement checks and retries

The candidate must fit the map, pass the building's normal foundation and terrain placement rules, and have no object in its foundation. Amphibious pathfinding must reach a cell beside the foundation from every requesting house's start. For a shared request, both houses must pass the reachability and placement checks.

[`POISafeDistance`](/keys/poisafedistance/) is a minimum straight-line distance in cells from every active start and every previously placed POI. The distance is measured between start and placement cells, not foundation edges. A shared request must satisfy both requesters' settings; a new request must also satisfy the safe-distance setting of each previously placed POI's requester. The game applies [`POIsMax`](/keys/poismax/) before merging reciprocal requests, so both requests count toward the cap even when they share one structure. A candidate exactly at the required distance is accepted. Previously placed structures stay where they were; the game rejects a later candidate that is too close and tries another.

The game makes up to six attempts using the configured rule. If none succeeds, it tries up to six random cells with the same placement checks. A request that still cannot be placed is skipped.
