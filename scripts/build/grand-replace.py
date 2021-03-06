# grand-replace.py
#
# This file is part of LilyPond, the GNU music typesetter.
#
# Copyright (C) 2009--2022 Jan Nieuwenhuizen <janneke@gnu.org>
#
# LilyPond is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LilyPond is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.

import datetime
import os
import re
import sys
#
import pytt

dry_run = False


def read_pipe(cmd, ignore_errors=False):
    pipe = os.popen(cmd)
    val = pipe.read()
    if pipe.close() and not ignore_errors:
        raise SystemFailed('Pipe failed: %(cmd)s' % locals())
    return val


def filter_out(p, lst):
    return [x for x in lst if not p(x)]


copied_files = [
    # files maintained outside of LilyPond
    'config.guess',
    'config.sub',
    'fdl.itexi',
    'gpl.itexi',
    'help2man.pl',
    'install-sh',
    'mf2pt1.mp',
    'mf2pt1.pl',
    'texinfo-ja.tex',
    'texinfo.tex',
    'txi-ca.tex',
    'txi-de.tex',
    'txi-en.tex',
    'txi-es.tex',
    'txi-fr.tex',
    'txi-hu.tex',
    'txi-it.tex',
    'txi-ja.tex',
    'txi-nl.tex',
    'txi-pt.tex',

    # files maintained by the translation team
    'ca.po',
    'cs.po',
    'da.po',
    'de.po',
    'el.po',
    'eo.po',
    'es.po',
    'fi.po',
    'fr.po',
    'hu.po',
    'it.po',
    'ja.po',
    'nl.po',
    'pt.po',
    'ru.po',
    'sv.po',
    'tr.po',
    'uk.po',
    'vi.po',
    'zh.po',
    'zh_TW.po',
]


def main():
    files = filter_out(lambda x: (os.path.basename(x) in copied_files
                                  or 'CHANGES' in x or 'ChangeLog' in x),
                       read_pipe('git ls-files').split())
    year = datetime.datetime.now().year
    last_year = year - 1
    last_year_1d = last_year % 10
    for f in files:
        pytt.pytt(r'(Copyright|\(c\)|\(C\)|@copyright\{\})\s*%(last_year)s(?=[^-]|$)' % locals(),
                  r'\1 %(last_year)s--%(year)s' % locals(),
                  f)
        pytt.pytt(r'(Copyright|\(c\)|\(C\)|@copyright\{\})\s*([^-]*)--(20[0-9][0-%(last_year_1d)s])' % locals(),
                  r'\1 \2--%(year)s' % locals(),
                  f)


if __name__ == '__main__':
    main()
