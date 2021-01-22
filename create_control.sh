#!/bin/bash
# Makes a control string to append to file
cat << EOF
Package: sl
Version: $1.$2
Architecture: all
Essential: no
Priority: optional
Section: misc
Maintainer: Brian Lambrigger <lambrigger.brian@gmail.com>
Description: Punish the typo-ists with petty ASCII
EOF
