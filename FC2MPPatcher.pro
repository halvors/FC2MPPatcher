TEMPLATE = subdirs

SUBDIRS += \
    libpebliss \
    app

win32 {
    SUBDIRS += \
        libargon2 \
        libpatch

    libargon2.subdir = lib/libargon2
    libpatch.subdir = src/libpatch
    libpatch.depends = libargon2
}

# Where to find the sub projects - give the folders
libpebliss.subdir = lib/libpebliss
app.subdir = src/app


# What subproject depends on others
app.depends = libpebliss
