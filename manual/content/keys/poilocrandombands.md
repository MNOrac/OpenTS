---
key: POILocRandomBands
summary: The minimum and maximum fractions of the distance between two starts used by `RandomBand`.
when_omitted:
  kind: value
  value: "0,1"
  note: The full distance from this house's start to the selected other start is eligible.
---

Sets the minimum and maximum distance fractions for the `RandomBand` placement rule. Enter two finite numbers in ascending order, separated by a comma; each value is clipped to the range from `0` to `1`. For example, `POILocRandomBands=0.3,1` allows candidates from 30% to 100% of the straight-line distance between this house's start and the selected other start. An invalid pair is logged and leaves the current bounds unchanged; a new house type starts with the full `0` to `1` range. [Points of interest](/systems/points-of-interest/) describes the candidate selection.
