#!/usr/bin/python3

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

from sys import argv, exit
import os

from checksum_services import *

algorithms = [
    {'name': "MD5",    'program': "md5sum",    'obsolete': True },
    {'name': "SHA1",   'program': "sha1sum",   'obsolete': False},
    {'name': "SHA224", 'program': "sha224sum", 'obsolete': False},
    {'name': "SHA256", 'program': "sha256sum", 'obsolete': False},
    {'name': "SHA384", 'program': "sha384sum", 'obsolete': False},
    {'name': "SHA512", 'program': "sha512sum", 'obsolete': False},
    {'name': "Blake2", 'program': "b2sum",     'obsolete': False},
]

if __name__ == "__main__":
    if len(argv) == 1:
        for algorithm in algorithms:
            print(algorithm['name'].lower())
        exit(0)

    assert len(argv) == 2, "This script requires one argument"
    assert os.path.isdir(argv[1]), f"The folder \"{argv[1]}\" does not exist"

    for algorithm in algorithms:
        compute = ComputeServiceEntry(algorithm['name'], algorithm['program'])
        compute['Name']['fr'] = f"Hasher avec {algorithm['name']}"
        compute['X-KDE-Submenu']['fr'] = "Hasher"

        with open(os.path.join(argv[1], compute.filename), 'wt', encoding='utf-8') as desktopFile:
            desktopFile.write(repr(compute))
            print(f"Created {desktopFile.name}")

        verify = VerifyServiceEntry(algorithm['name'], algorithm['program'])
        verify['Name']['fr'] = f"Vérifier un hash {algorithm['name']}"
        verify['X-KDE-Submenu']['fr'] = "Vérifier un hash"
        verify['Actions']['Exec']['fr'] = f"""bash -c 'CHECKSUM=$(kdialog --desktopfile checksum-verify-{algorithm['name'].lower()} --title "{algorithm['name']} checksum" --inputbox "Somme de contrôle {algorithm['name']} attendue pour %f:" ""); test -z "$CHECKSUM" && exit 0; test "$CHECKSUM" = "$({algorithm['program']} %f | cut -d " " -f 1)" && notify-send -t 0 -i "dialog-ok" "{algorithm['name']} checksum" "%f:\\nLa somme de contrôle {algorithm['name']} est valide" || notify-send -t 0 -i "dialog-error" "{algorithm['name']} checksum" "%f:\\nLa somme de contrôle {algorithm['name']} est invalide"'"""

        with open(os.path.join(argv[1], verify.filename), 'wt', encoding='utf-8') as desktopFile:
            desktopFile.write(repr(verify))
            print(f"Created {desktopFile.name}")
