%% DO NOT EDIT this file manually; it is automatically
%% generated from LSR http://lsr.dsi.unimi.it
%% Make any changes in LSR itself, or in Documentation/snippets/new/ ,
%% and then run scripts/auxiliar/makelsr.py
%%
%% This file is in the public domain.
\version "2.14.2"

\header {
  lsrtags = "tweaks-and-overrides, expressive-marks"

%% Translation of GIT committish: 6977ddc9a3b63ea810eaecb864269c7d847ccf98
  texidoces = "
Si necesitamos alargar o acortar un símbolo de arpegio, podemos
modificar independientemente los extremos superior e inferior.

"
  doctitlees = "Posicionar símbolos de arpegio"



  texidoc = "
If you need to extend or shorten an arpeggio, you can modify the upper
and lower start positions independently.

"
  doctitle = "Positioning arpeggios"
} % begin verbatim


\relative c' {
  <c e g b>1\arpeggio
  \once \override Arpeggio #'positions = #'(-5 . 0)
  <c e g b>1\arpeggio
  \once \override Arpeggio #'positions = #'(0 . 5)
  <c e g b>1\arpeggio
  \once \override Arpeggio #'positions = #'(-5 . 5)
  <c e g b>1\arpeggio
}
