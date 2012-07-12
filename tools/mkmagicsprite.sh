#!/bin/bash

if [ -z "$5" ]; then
	echo "Usage: $0 <texture> <sprite list> <tilew> <tileh> <output>"
	exit 127
fi


../tools/mkrawtexture "$1" "magictex.tmp"
../tools/mkexsprite "$2" $3 $4 "exsprite.tmp"

cat "magictex.tmp" "exsprite.tmp" > "$3"
rm -f "magictex.tmp"
rm -f "exsprite.tmp"

