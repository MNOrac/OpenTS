---
key: POILocJitterParallel
summary: The maximum random shift in cells along the line between two player starts.
when_omitted:
  kind: value
  value: "0"
---

Sets the maximum random shift in cells along the line between the two starts used by a paired-start placement rule. Each attempt can shift by a signed amount up to this limit; negative values act as `0`. It does not affect `RandomSafe` or `Random`. [Points of interest](/systems/points-of-interest/) explains when the game applies jitter.
