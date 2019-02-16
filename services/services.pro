TEMPLATE    = aux
DESTDIR     = gen
TARGET      = .
INSTALLS    =

OTHER_FILES += generate-checksum-services.py

services.depends = generate-checksum-services.py
services.commands = (test ! -d "gen" && mkdir "$$DESTDIR");
services.commands += (python3 generate-checksum-services.py "$$DESTDIR/");
services.CONFIG += no_link

QMAKE_EXTRA_TARGETS += services
POST_TARGETDEPS += services

ALGORITMS=$$system(python3 generate-checksum-services.py)
for (algo, ALGORITMS) {
    message(Found $$algo)
    QMAKE_CLEAN += $$DESTDIR/checksum-compute-$${algo}.desktop
    QMAKE_CLEAN += $$DESTDIR/checksum-verify-$${algo}.desktop
}


