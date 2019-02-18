TEMPLATE    = aux
DESTDIR     = gen
TARGET      = .
INSTALLS    =

OTHER_FILES += generate-checksum-services.py

genServices.depends = generate-checksum-services.py
genServices.commands = (test ! -d "gen" && mkdir "$$DESTDIR");
genServices.commands += (python3 generate-checksum-services.py "$$DESTDIR/");
genServices.CONFIG += no_link

QMAKE_EXTRA_TARGETS += genServices
POST_TARGETDEPS += genServices

ALGORITMS=$$system(python3 generate-checksum-services.py)
for (algo, ALGORITMS) {
    message(Found $$algo)
    QMAKE_CLEAN += $$DESTDIR/checksum-compute-$${algo}.desktop
    QMAKE_CLEAN += $$DESTDIR/checksum-verify-$${algo}.desktop
}

SERVICE_PATHS=$$system(kf5-config --path services)
SERVICE_PATHS=$$split(SERVICE_PATHS, :)
isEmpty(USER_INSTALL) {
    INSTALL_PATH = $$member(SERVICE_PATHS, 1)
    message(branch1)
} else {
    INSTALL_PATH = $$member(SERVICE_PATHS, 0)
    message(branch0)
}

message(INSTALL_PATH=$$INSTALL_PATH)
installServices.path = $$INSTALL_PATH
installServices.files = gen/*

INSTALLS += installServices
