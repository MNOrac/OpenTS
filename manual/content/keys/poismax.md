---
key: POIsMax
summary: The maximum number of point-of-interest placements attributed to this house type across its active instances.
when_omitted:
  kind: value
  value: "-1"
---

Limits the point-of-interest placements attributed to this house type across its active instances. The cap applies after reciprocal requests merge; a shared placement counts once for each distinct requesting house type. Any negative value leaves the count uncapped. [Points of interest](/systems/points-of-interest/) explains how the cap is distributed among instances.
