---
key: POISafeDistance
summary: The minimum straight-line cell distance between a point of interest and player starts or other points of interest.
when_omitted:
  kind: value
  value: "0"
---

Requires each point-of-interest placement cell to be at least this many cells from every active player's start and every previously placed point of interest. A shared point of interest must meet the distance setting of both requesting houses. Negative values act as `0`; see [Points of interest](/systems/points-of-interest/) for how distance checks interact with placement.
