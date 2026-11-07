---
key: POINeutral
scope: housetype
label: Point-of-interest owner
summary: Chooses whether this house type's point-of-interest structures belong to Neutral or the requesting house.
when_omitted:
  kind: value
  value: "yes"
---

When `yes`, this house type's point-of-interest structures belong to Neutral. When `no`, each structure belongs to the house that requested it. Player-owned reciprocal requests can share with mutual allies only when both use `POINeutral=no` and either requester sets [`POIDoubleUpWithAllies=yes`](/keys/poidoubleupwithallies/); the game randomly selects which house owns the shared structure. Player-owned requests cannot share with enemies. [Points of interest](/systems/points-of-interest/) describes ownership and placement.
