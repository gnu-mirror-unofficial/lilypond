%% DO NOT EDIT this file manually; it is automatically
%% generated from LSR http://lsr.di.unimi.it
%% Make any changes in LSR itself, or in Documentation/snippets/new/ ,
%% and then run scripts/auxiliar/makelsr.py
%%
%% This file is in the public domain.
\version "2.23.6"

\header {
  lsrtags = "chords, fretted-strings, specific-notation"

  texidoc = "
Sometimes chords with a stretched fingering are required. If not
otherwise specified the context-property @code{maximumFretStretch} is
set to @code{4}, though. Resulting in a warning about \"No string for
pitch ...\" and the note is omitted. You may set
@code{maximumFretStretch} to an approppriate value or explecitely
assign string-numbers to all notes of a chord.

"
  doctitle = "Chords with stretched fingering for FretBoards and TabVoice"
} % begin verbatim

%% The code below will print two warnings, which may be omitted by uncommenting:
%#(for-each (lambda (x) (ly:expect-warning "No string for pitch")) (iota 2))

mus = {
  <c' bes'>
  <c'\2 bes'>
  \set maximumFretStretch = 5
  <c' bes'>
  <c'\2 bes'\1>
}

<<
  \new FretBoards \mus
  \new TabVoice \mus
>>
