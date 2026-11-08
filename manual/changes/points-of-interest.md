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

House entries can configure point-of-interest structures, placement rules, counts, ownership, and sharing for skirmish and multiplayer matches. Paired-start rules prefer unused enemy starts, then unused allies where permitted; once no unused eligible start remains, they reuse an enemy start if one exists. Reciprocal fractional requests merge before `POIsMax` limits placements for each house type.
