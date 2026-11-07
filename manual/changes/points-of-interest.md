---
title: Place points of interest in skirmish and multiplayer matches
category: feature
release: 0.2.0
targets:
- type: system
  id: points-of-interest
  effect: added
- type: key
  id: POIs
  effect: added
- type: key
  id: POIsBaseCount
  effect: added
- type: key
  id: POIsPerExtraPlayer
  effect: added
- type: key
  id: POIsMax
  effect: added
- type: key
  id: POILocations
  effect: added
- type: key
  id: POILocRandomBands
  effect: added
- type: key
  id: POISafeDistance
  effect: added
- type: key
  id: POILocJitterParallel
  effect: added
- type: key
  id: POILocJitterLateral
  effect: added
- type: key
  id: POIDoubleUpWithEnemies
  effect: added
- type: key
  id: POIDoubleUpWithAllies
  effect: added
- type: key
  id: POINeutral
  effect: added
credit:
- Orac
---

House entries can configure point-of-interest structures, their placement rules, count, and ownership for skirmish and multiplayer matches. `Behind` searches the rear half of the `POISafeDistance` ring for a valid placement before trying random cells; paired-start jitter can shift its candidates, which still must pass placement checks. `POINeutral=no` gives the structure to the requesting player. Use `POIDoubleUpWithEnemies` for neutral-owned enemy requests and `POIDoubleUpWithAllies` for allied requests; a shared player-owned POI is randomly assigned to one of the two houses. `POIsPerExtraPlayer=` in each house-type section of `rules.ini` adds requests for active players beyond the second.
