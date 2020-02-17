# Instruments

Instruments have unique identifying numbers ranging
from `0` to `127`. As with channels, this
number's only purposes is to be able
to make the distinction between instruments.
An instrument having higher or lower id is meaningless.

Introduce a new type `Instrument` with support
for `==`, `!=` and `<<`.