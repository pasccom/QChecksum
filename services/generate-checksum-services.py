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

class LocaleString:
    def __init__(self, default, **kwArgs):
        self.__default = default
        self.__localized = kwArgs

    def __setitem__(self, item, value):
        self.__localized[item] = value

    def default(self, field):
        return f'{field}={self.__default}'

    def localized(self, field):
        yield f'{field}={self.__default}'
        for k, v in self.__localized.items():
            yield f'{field}[{k}]={v}'

class DesktopAction(dict):
    requiredKeys = ['Name']

    def __init__(self, name, entryName, **kwArgs):
        super().__init__()
        self.__name = name
        self.__setitem__('Name', entryName)
        for k, v in kwArgs.items():
            self.__setitem__(k, v)

    def __str__(self):
        return self.__name

    def __repr__(self):
        ret = [f'[Desktop Action {self.__name}]']
        for k in DesktopAction.requiredKeys + [k for k in self.keys() if k not in DesktopAction.requiredKeys]:
            if type(self.__getitem__(k)) is LocaleString:
                ret += list(self.__getitem__(k).localized(k))
            else:
                ret += [f'{k}={self.__getitem__(k)}']
        return '\n'.join(ret)

class DesktopEntry(dict):
    requiredKeys = ['Type', 'Name']

    def __init__(self, entryType, entryName, **kwArgs):
        super().__init__()
        self.__setitem__('Type', entryType)
        self.__setitem__('Name', entryName)
        for k, v in kwArgs.items():
            self.__setitem__(k, v)

    def __repr__(self):
        ret = ['[Desktop Entry]']

        for k in DesktopEntry.requiredKeys + [k for k in self.keys() if k not in DesktopEntry.requiredKeys]:
            if type(self.__getitem__(k)) is LocaleString:
                ret += list(self.__getitem__(k).localized(k))
            elif type(self.__getitem__(k)) is list:
                ret += [k + '=' + ';'.join([str(item) for item in self.__getitem__(k)]) + ';']
            else:
                ret += [f'{k}={self.__getitem__(k)}']

        for k in self.keys():
            if type(self.__getitem__(k)) is DesktopAction:
                ret += ['', repr(self.__getitem__(k))]
            elif type(self.__getitem__(k)) is list:
                for a in self.__getitem__(k):
                    if type(a) is DesktopAction:
                        ret += ['', repr(a)]

        return '\n'.join(ret) + '\n'

class ServiceEntry(DesktopEntry):
    def __init__(self, entryName, **kwArgs):
        super().__init__('Service', entryName, **kwArgs)

class ComputeServiceEntry(ServiceEntry):
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

        with open(os.path.join(argv[1], compute.filename), 'wt') as desktopFile:
            desktopFile.write(repr(compute))
            print(f"Created {desktopFile.name}")

        verify = VerifyServiceEntry(algorithm['name'], algorithm['program'])
        verify['Name']['fr'] = f"Vérifier un hash {algorithm['name']}"
        verify['X-KDE-Submenu']['fr'] = "Vérifier un hash"
        verify['Actions']['Exec']['fr'] = f"""bash -c 'CHECKSUM=$(kdialog --desktopfile checksum-verify-{algorithm['name'].lower()} --title "{algorithm['name']} checksum" --inputbox "Somme de contrôle {algorithm['name']} attendue pour %f:" ""); test -z "$CHECKSUM" && exit 0; test "$CHECKSUM" = "$({algorithm['program']} %f | cut -d " " -f 1)" && notify-send -t 0 -i "dialog-ok" "{algorithm['name']} checksum" "%f:\\nLa somme de contrôle {algorithm['name']} est valide" || notify-send -t 0 -i "dialog-error" "{algorithm['name']} checksum" "%f:\\nLa somme de contrôle {algorithm['name']} est invalide"'"""

        with open(os.path.join(argv[1], verify.filename), 'wt') as desktopFile:
            desktopFile.write(repr(verify))
            print(f"Created {desktopFile.name}")
