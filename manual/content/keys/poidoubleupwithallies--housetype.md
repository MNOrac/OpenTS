---
key: POIDoubleUpWithAllies
scope: housetype
label: Allied POI sharing
summary: Allows allied houses to share matching point-of-interest requests.
when_omitted:
  kind: value
  value: "no"
---

Allows this house's request to merge with a mutual ally's reciprocal fractional-location request for the same structure and start pair when both requests use the same [`POINeutral`](/keys/poineutral/) setting. Either house can enable sharing. A shared neutral POI belongs to Neutral; a shared player-owned POI is randomly assigned to one of the two requesting houses. Enemy requests use [`POIDoubleUpWithEnemies`](/keys/poidoubleupwithenemies/). [Points of interest](/systems/points-of-interest/) describes the matching rules and placement checks.
