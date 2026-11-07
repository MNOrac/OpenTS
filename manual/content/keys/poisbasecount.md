---
key: POIsBaseCount
summary: The base number of point-of-interest requests for each active house of this type.
when_omitted:
  kind: value
  value: "0"
---

Sets the base number of point-of-interest requests for each active house of this type in skirmish and multiplayer games. [`POIsPerExtraPlayer`](/keys/poisperextraplayer/) adds requests as the number of active players grows, and [`POIsMax`](/keys/poismax/) caps the total for this house type. Placement can still skip requests that fail all attempts; see [Points of interest](/systems/points-of-interest/).
