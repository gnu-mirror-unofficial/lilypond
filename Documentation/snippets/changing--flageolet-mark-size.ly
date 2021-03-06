%% DO NOT EDIT this file manually; it is automatically
%% generated from LSR http://lsr.di.unimi.it
%% Make any changes in LSR itself, or in Documentation/snippets/new/ ,
%% and then run scripts/auxiliar/makelsr.py
%%
%% This file is in the public domain.
\version "2.23.6"

\header {
  lsrtags = "expressive-marks, specific-notation, symbols-and-glyphs"

  texidoc = "
To make the @code{\\flageolet} circle smaller use the following tweak.

"
  doctitle = "Changing \\flageolet mark size"
} % begin verbatim

smallFlageolet = \tweak font-size -3 \flageolet

\layout { ragged-right = ##f }

\relative c'' {
  d4^\flageolet_\markup { default size } d_\flageolet
  c4^\smallFlageolet_\markup { smaller } c_\smallFlageolet
}
