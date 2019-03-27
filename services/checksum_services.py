# Copyright 2019 Pascal COMBES <pascom@orange.fr>
# 
# This file is part of QChecksum.
# 
# QChecksum is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# QChecksum is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with QChecksum. If not, see <http://www.gnu.org/licenses/>

""" This module contains `Freedesktop service entries <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html>`_
specialized for QChecksum.
"""

from services import *

class ComputeServiceEntry(ServiceEntry):
    """ Python representation of a `Freedesktop service entry <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html>`_
        specialized to compute checksums.

        :param str algorithm: Name of the hash function
        :param str program: Program executing the hash function
    """

    def __init__(self, algorithm, program):
        super().__init__(
            LocaleString(f"Compute {algorithm} checksum"),
            ServiceTypes="KonqPopupMenu/Plugin",
            MimeType=["application/octet-stream"],
            Actions=DesktopAction(
                f"compute-{algorithm.lower()}",
                algorithm,
                Exec=LocaleString(f"""notify-send -t 0 -i "dialog-information" "{algorithm} checksum" "%f:\\n$({program} %f | cut -d ' ' -f 1)\"""")
            ),
            TryExec=program
        )
        self.__setitem__('X-KDE-Submenu', LocaleString("Compute Checksums"))
        self.filename = f"checksum-compute-{algorithm.lower()}.desktop"

class VerifyServiceEntry(ServiceEntry):
    """ Python representation of a `Freedesktop service entry <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html>`_
        specialized to verify checksums.

        :param str algorithm: Name of the hash function
        :param str program: Program executing the hash function
    """
    def __init__(self, algorithm, program):
        super().__init__(
            LocaleString(f"Verify {algorithm} checksum"),
            ServiceTypes="KonqPopupMenu/Plugin",
            MimeType=["application/octet-stream"],
            Actions=DesktopAction(
                f"verify-{algorithm.lower()}",
                algorithm,
                Exec=LocaleString(f"""bash -c 'CHECKSUM=$(kdialog --desktopfile checksum-verify-{algorithm.lower()} --title "{algorithm} checksum" --inputbox "Expected {algorithm} checksum for %f:" ""); test -z "$CHECKSUM" && exit 0; test "$CHECKSUM" = "$({program} %f | cut -d " " -f 1)" && notify-send -t 0 -i "dialog-ok" "{algorithm} checksum" "%f:\\nValid {algorithm} checksum" || notify-send -t 0 -i "dialog-error" "{algorithm} checksum" "%f:\\nInvalid {algorithm} checksum"'""")
            ),
            TryExec=program
        )
        self.__setitem__('X-KDE-Submenu', LocaleString("Verify Checksums"))
        self.filename = f"checksum-verify-{algorithm.lower()}.desktop"


