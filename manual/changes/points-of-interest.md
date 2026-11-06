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
  id: POIsPerOpponent
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
credit:
- Orac
---

House entries can configure point-of-interest structures, their placement rules, and their count for skirmish and multiplayer matches. OpenTS places successful requests as neutral structures after it creates the active players' starting forces; reciprocal fractional requests can share one structure when either requester enables sharing.
