CREATE OR REPLACE FUNCTION decode(anyelement, anyelement, text, anyelement, text, anyelement, text)
RETURNS text
AS '../lib/decode.so', 'decode'
LANGUAGE C STRICT;
