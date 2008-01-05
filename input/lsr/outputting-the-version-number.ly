%% Do not edit this file; it is auto-generated from input/new
\version "2.11.35"

\header {
  doctitle = "Outputting the version number"
  lsrtags = "text"
  texidoc = "
By putting the output of @code{lilypond-version} into a lyric or a
text markup, it is possible to print the version number of LilyPond in
a score, or in a document generated with @code{lilypond-book}.
"
}
% begin verbatim
\score { \context Lyrics  {
    \override Score.RehearsalMark  #'self-alignment-X = #LEFT
    \mark #(ly:export (string-append "Processed with LilyPond version " (lilypond-version)))
    s2
  }
}
