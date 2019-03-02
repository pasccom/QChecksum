REPOSITORY DESCRIPTION
----------------------

This repository contains the code for a small utiliy program which computes
and verifies checksums for files. It also includes services definitions to
allow users to compute checksums directly from e.g.  Dolphin or Konqueror context
menus and run the program directly from Dolphin and Konqueror context menu.

The services are implemented in such a way, that the algorithms can be disabled
individually in e.g. Dolphin or Konqueror service configuration.

FEATURES
--------

Here is a list of the current features of the program:
- Allows to compute checksums using the programs in `/bin`
- Allows to compute verify checksums using the programs in `/bin`
- Can be run directly from e.g. Dolphin or Konqueror context menus
- Lightweight version implemented using a service, which allows to
compute and verify checksums from e.g. Dolphin or Konqueror context menus

INSTALLATION
------------

Compiling the program requires Qt5 and generating the services requires Python 3.

The program can either be installed systemwide or in user HOME directory. To
select user HOME directory, set `USER_INSTALL` on `qmake` command line.
The build system also support the PREFIX option for binary and data installation.

FUTURE DEVELOPMENTS
--------------------

If you have any other feature you will be interested in, please let me know.
I will be pleased to develop it if I think it is a must have.

If you want to implement extension, also tell me please. Admittedly you
can do what you desire with the code (under the [licensing constraints](#licensing-information)), but this will avoid double work.

LICENSING INFORMATION
---------------------

QChecksum is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QChecksum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a [copy of the GNU General Public License](LICENSE)
along with QChecksum. If not, see http://www.gnu.org/licenses/
