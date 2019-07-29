TEMPLATE = subdirs

SUBDIRS += app

# Only build patch library on win32.
win32 {
    SUBDIRS += lib
}
