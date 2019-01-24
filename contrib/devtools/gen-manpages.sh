#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

NODEHOSTD=${NODEHOSTD:-$BINDIR/nodehostd}
NODEHOSTCLI=${NODEHOSTCLI:-$BINDIR/nodehost-cli}
NODEHOSTTX=${NODEHOSTTX:-$BINDIR/nodehost-tx}
NODEHOSTQT=${NODEHOSTQT:-$BINDIR/qt/nodehost-qt}

[ ! -x $NODEHOSTD ] && echo "$NODEHOSTD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($NODEHOSTCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for nodehostd if --version-string is not set,
# but has different outcomes for bitcoin-qt and nodehost-cli.
echo "[COPYRIGHT]" > footer.h2m
$NODEHOSTD --version | sed -n '1!p' >> footer.h2m

for cmd in $NODEHOSTD $NODEHOSTCLI $NODEHOSTTX $NODEHOSTQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
