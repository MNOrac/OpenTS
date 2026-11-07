---
key: POIsPerExtraPlayer
scope: housetype
label: Extra-player POI count
summary: Additional point-of-interest requests for each active house beyond the second.
when_omitted:
  kind: value
  value: "0"
---

Adds this many requests per active house instance for each active player beyond the second in a skirmish or multiplayer match. Allies count as active players. For example, with four active players and `POIsPerExtraPlayer=2`, each active house instance requests four additional POIs. [Points of interest](/systems/points-of-interest/) explains how the game calculates the final request count.
