%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
%% Tags: rhythms, expressive-marks
\version "2.11.35"

\header { texidoc = "
LilyPond syntax can involve many unusual placements for parentheses,
brackets etc., which might sometimes have to be interleaved.For
example, when entering a manual beam, the left square bracket has to be
placed after the starting note and its duration, not before. Similarly,
the right square bracket should directly follow the note which is to be
at the end of the requested beaming, even if this note happens to be
inside a tuplet section. This snippet demonstrates how to superpose
manual beaming, manual slurs, ties, and phrasing slurs, while using
tuplet sections (enclosed with curly braces).
" }
% begin verbatim
{
  r16[ g16 \times 2/3 {r16 e'8] }
  
 g16( a \times 2/3 {b d e') }
 
 g8[( a \times 2/3 {b d') e'~]}
 
 \times 4/5 {e'32\( a b d' e'} a'4.\)
}
