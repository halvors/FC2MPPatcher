TEMPLATE = subdirs

SUBDIRS += \
    libpebliss \
    app

libpebliss.subdir = lib/libpebliss
app.subdir = src/app
app.depends = libpebliss

win32 {
    SUBDIRS += \
        libargon2 \
        libpatch

    libargon2.subdir = lib/libargon2
    libpatch.subdir = src/libpatch
    libpatch.depends = libargon2
}
