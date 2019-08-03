TEMPLATE = subdirs

SUBDIRS += \
    libpebliss \
    app

win32 {
    SUBDIRS += libpatch
}

# Where to find the sub projects - give the folders
libpebliss.subdir = lib/libpebliss
app.subdir = src/app
libpatch.subdir = src/libpatch

# What subproject depends on others
app.depends = libpebliss
