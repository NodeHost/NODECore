
Debian
====================
This directory contains files used to package nodehostd/nodehost-qt
for Debian-based Linux systems. If you compile nodehostd/nodehost-qt yourself, there are some useful files here.

## nodehost: URI support ##


nodehost-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install nodehost-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your nodehostqt binary to `/usr/bin`
and the `../../share/pixmaps/nodehost128.png` to `/usr/share/pixmaps`

nodehost-qt.protocol (KDE)

