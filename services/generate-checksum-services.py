#!/usr/bin/python3

from sys import argv, exit
import os

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
        with open(os.path.join(argv[1], f"checksum-compute-{algorithm['name'].lower()}.desktop"), 'wt') as desktopFile:
            desktopFile.write(f"""[Desktop Entry]
Type=Service
ServiceTypes=KonqPopupMenu/Plugin
MimeType=application/octet-stream;
Actions=compute-{algorithm['name'].lower()}
TryExec={algorithm['program']}
X-KDE-Submenu=Compute Checksums
X-KDE-Submenu[fr]=Hasher

[Desktop Action compute-{algorithm['name'].lower()}]
Name={algorithm['name']}
Exec=notify-send -t 0 -i "dialog-information" "{algorithm['name']} checksum" "%f:\\n$({algorithm['program']} %F | cut -d ' ' -f 1)"
""")
            print(f"Created {desktopFile.name}")

        with open(os.path.join(argv[1], f"checksum-verify-{algorithm['name'].lower()}.desktop"), 'wt') as desktopFile:
            desktopFile.write(f"""[Desktop Entry]
Type=Service
ServiceTypes=KonqPopupMenu/Plugin
MimeType=application/octet-stream;
Actions=verify-{algorithm['name'].lower()}
TryExec={algorithm['program']}
X-KDE-Submenu=Verify Checksums
X-KDE-Submenu[fr]=Vérifier hash

[Desktop Action verify-{algorithm['name'].lower()}]
Name={algorithm['name']}
Exec=bash -c 'CHECKSUM=$(kdialog --desktopfile checksum-verify-{algorithm['name'].lower()} --title "{algorithm['name']} checksum" --inputbox "Expected {algorithm['name']} checksum:" ""); test -z "$CHECKSUM" && exit 0; test "$CHECKSUM" = "$({algorithm['program']} %f | cut -d " " -f 1)" && notify-send -t 0 -i "dialog-ok" "{algorithm['name']} checksum" "%f:\\nValid {algorithm['name']} checksum" || notify-send -t 0 -i "dialog-error" "{algorithm['name']} checksum" "%f:\\nInvalid {algorithm['name']} checksum"'
Exec[fr]=bash -c 'CHECKSUM=$(kdialog --desktopfile checksum-verify-{algorithm['name'].lower()} --title "{algorithm['name']} checksum" --inputbox "Somme de contrôle {algorithm['name']} attendue:" ""); test -z "$CHECKSUM" && exit 0; test "$CHECKSUM" = "$({algorithm['program']} %f | cut -d " " -f 1)" && notify-send -t 0 -i "dialog-ok" "{algorithm['name']} checksum" "%f:\\nLa somme de contrôle {algorithm['name']} est valide" || notify-send -t 0 -i "dialog-error" "{algorithm['name']} checksum" "%f:\\nLa somme de contrôle {algorithm['name']} est invalide"'
""")
            print(f"Created {desktopFile.name}")
