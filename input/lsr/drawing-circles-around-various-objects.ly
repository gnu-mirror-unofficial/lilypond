%% Do not edit this file; it is auto-generated from input/new
\version "2.11.35"
\layout { ragged-right= ##t }
\header {
  doctitle = "Drawing circles around various objects"
  lsrtags = "tweaks-and-overrides"
  texidoc = "
The @code{\circle} command allows you to draw circles around various objects,
for example fingering indications. However, some objects require
specific tweaks: rehearsal marks depend on the @code{Score.markFormatter}
context, bar numbers on the @code{Score.BarNumber} context, and so on.

You can tweak the printing of your circles by setting some properties
such as @code{#'thickness}, @code{#'circle-padding} or @code{#'font-size}.
" }
% begin verbatim
\relative c'{
c1
\set Score.markFormatter
  = #(lambda (mark context)
             (make-circle-markup (format-mark-numbers mark context)))
\mark \default
c2 d^\markup{\circle \finger "2"}
\override Score.BarNumber #'break-visibility = #all-visible
\override Score.BarNumber  #'stencil
  = #(make-stencil-circler 0.1 0.25 ly:text-interface::print)
}
