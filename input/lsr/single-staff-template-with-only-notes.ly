%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
%% Tags: template
\version "2.11.35"

\header { texidoc = "
This very simple template gives you a staff with notes, suitable for a
solo instrument or a melodic fragment. Cut and paste this into a file,
add notes, and you're finished! 
" }
% begin verbatim
melody = \relative c' {
            \clef treble
            \key c \major
            \time 4/4
         
            a4 b c d
         }
         
         \score {
            \new Staff \melody
            \layout { }
            \midi {}
         }

