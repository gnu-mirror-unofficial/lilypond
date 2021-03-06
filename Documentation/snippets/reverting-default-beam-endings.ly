%% DO NOT EDIT this file manually; it is automatically
%% generated from LSR http://lsr.di.unimi.it
%% Make any changes in LSR itself, or in Documentation/snippets/new/ ,
%% and then run scripts/auxiliar/makelsr.py
%%
%% This file is in the public domain.
\version "2.23.6"

\header {
  lsrtags = "rhythms, version-specific"

  texidoc = "
To typeset beams grouped @code{3-4-3-2} in 12/8 it is necessary first
to override the default beam endings in 12/8, and then to set up the
new beaming endings:

"
  doctitle = "Reverting default beam endings"
} % begin verbatim

\relative c'' {
  \time 12/8

  % Default beaming
  a8 a a a a a a a a a a a

  % Set new values for beam endings
  \set Score.beatStructure = 3,4,3,2
  a8 a a a a a a a a a a a
}
