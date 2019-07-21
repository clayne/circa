# 0.1.X

## 0.1.1

Fixed a bug with `map_get` and optimized `map_get`, `dict_get` and `seqmap_get`
to stop scanning as soon as a bucket with a probe count of 0 is hit, excluding
the initial bucket.

## 0.1.0

Initial release.
