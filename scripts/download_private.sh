#!/bin/bash
# Alex Bettarini - 22 Jan 2019

WD=$(pwd)
SRC_DIR=$(realpath ../)

if [ ! -z "$1" ] ; then
    SRC_DIR=$(realpath $1)
fi

BLD_DIR=$SRC_DIR/build
BIN_DIR=$BLD_DIR

# Issue #3 specifications:
DOWNLOAD_DIR=$SRC_DIR/downloads
OUTPUT_DIR=$SRC_DIR/output

mkdir -p $OUTPUT_DIR
mkdir -p $DOWNLOAD_DIR ; cd $DOWNLOAD_DIR

#echo "SRC_DIR: $SRC_DIR"
#echo "DOWNLOAD_DIR: $DOWNLOAD_DIR"
#echo "OUTPUT_DIR: $OUTPUT_DIR"

#-------------------------------------------------------------------------------
function urlencode() {
# urlencode <string>
    old_lc_collate=$LC_COLLATE
    LC_COLLATE=C
local length="${#1}"
for (( i = 0; i < length; i++ )); do
local c="${1:i:1}"
case $c in
            [a-zA-Z0-9.~_-]) printf "$c" ;;
*) printf '%%%02X' "'$c" ;;
esac
done
    LC_COLLATE=$old_lc_collate
}

#-------------------------------------------------------------------------------
# zurrose

source $WD/passwords
URL="ftp://ftp.zur-rose.ch/ywesee OutTest"
ZURROSE_DIR="${SRC_DIR}/input/zurrose"
mkdir -p $ZURROSE_DIR
pushd $ZURROSE_DIR

# Updated once per day
curl -o artikel_stamm_zurrose.csv -O "${URL}"/Artikelstamm.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"

# Updated every 4 hours
curl -o artikel_stamm_voigt.csv -O "${URL}"/Artikelstamm_Voigt.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"

# Updated every 30 minutes
curl -o artikel_vollstamm_zurrose.csv -O "${URL}"/Artikelstamm_Vollstamm.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"

curl -o direct_subst_zurrose.csv -O "${URL}"/direktsubstitution.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"
curl -o nota_zurrose.csv -O "${URL}"/Nota.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"
curl -o galenic_codes_map_zurrose.txt -O "${URL}"/Vollstamm_Galenic_Form_Mapping_by_Code.txt --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"

#curl -O "${URL}"/_log.txt --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"
curl -O "${URL}"/Autogenerika.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"

ISO_8859_1_FILE=Kunden_temp.csv
curl -o $ISO_8859_1_FILE -O "${URL}"/Kunden_alle.csv --user "${USERNAME_ZUR}:${PASSWORD_ZUR}"
iconv -f ISO-8859-1 -t UTF-8 $ISO_8859_1_FILE >Kunden_alle.csv
rm $ISO_8859_1_FILE
popd


