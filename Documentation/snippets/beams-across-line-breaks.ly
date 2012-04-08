%% DO NOT EDIT this file manually; it is automatically
%% generated from LSR http://lsr.dsi.unimi.it
%% Make any changes in LSR itself, or in Documentation/snippets/new/ ,
%% and then run scripts/auxiliar/makelsr.py
%%
%% This file is in the public domain.
\version "2.14.2"

\header {
  lsrtags = "rhythms"

%% Translation of GIT committish: 6977ddc9a3b63ea810eaecb864269c7d847ccf98
  texidoces = "
Normalmente están prohibidos los saltos de línea si las barras
atraviesan las líneas divisorias.  Se puede cambiar este
comportamiento como se muestra aquí:

"
  doctitlees = "Barras que atraviesan saltos de línea"


%% Translation of GIT committish: 0a868be38a775ecb1ef935b079000cebbc64de40
texidocde = "
Zeilenumbrüche sind normalerweise während Balken verboten.  Das kann geändert
werden.

"
  doctitlede = "Balken über Zeilenumbrüche"



%% Translation of GIT committish: 3b125956b08d27ef39cd48bfa3a2f1e1bb2ae8b4
  texidocfr = "
Il est normalement impensable qu'un saut de ligne tombe au milieu d'une
ligature.  LilyPond permet néanmoins de l'obtenir.

"
  doctitlefr = "Ligature au moment d'un saut de ligne"

  texidoc = "
Line breaks are normally forbidden when beams cross bar lines. This
behavior can be changed as shown:

"
  doctitle = "Beams across line breaks"
} % begin verbatim


\relative c'' {
  \override Beam #'breakable = ##t
  c8 c[ c] c[ c] c[ c] c[ \break
  c8] c[ c] c[ c] c[ c] c
}


