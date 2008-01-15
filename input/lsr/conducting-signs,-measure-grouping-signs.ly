%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
%% Tags: rhythms
\version "2.11.35"

\header { texidoc = "
The Scheme function @code{set-time-signature}, in combination with the
Measure grouping engraver,  it will create MeasureGrouping signs.  Such
signs ease reading rhythmically complex modern music.  In the following
example, the 9/8 measure is subdivided in 2, 2, 2 and 3.  This is
passed to @code{set-time-signature} as the third argument @code{(2 2 2
3)}


" }
% begin verbatim
\score {
  \relative c'' {
    #(set-time-signature 9 8 '(2 2 2 3))
    g8[ g] d[ d] g[ g] a8[( bes g]) |
    #(set-time-signature 5 8 '(3 2))
    a4. g4
  }
  \layout {
    \context {
      \Staff
      \consists "Measure_grouping_engraver"
    }
  }
}

