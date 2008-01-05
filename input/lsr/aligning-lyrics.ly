%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
%% Tags: text, vocal-music
\version "2.11.35"

\header { texidoc = "
You can specify different horizontal alignment for your lyrics, by
overriding the #'self-alignment-X property of the LyricText object. #-1
is left, #0 is center and #1 is right; however, you can use #LEFT,
#CENTER and #RIGHT as well.
" }
% begin verbatim
\relative c'' {
c1 c c}
\addlyrics {\once \override LyricText #'self-alignment-X = #LEFT "This is left-aligned" 
\once \override LyricText #'self-alignment-X = #CENTER "This is centered" 
\once \override LyricText #'self-alignment-X = #1 "this is right-aligned"  }
