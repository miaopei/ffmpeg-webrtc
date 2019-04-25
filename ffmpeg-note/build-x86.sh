#!/bin/sh

#Create by Kingxl 
#http://itjoy.org
#Make sure you have installed: Xcode/Preferences/Downloads/Components/Command Line Tools
#FFmpeg source -->git://source.ffmpeg.org/ffmpeg.git
#Build armv7 armv7s arm64  

#Download FFmpeg source
git clone git://source.ffmpeg.org/ffmpeg.git

cd ffmpeg

#Setting your SDK Version.

VERSION="2.0.2"
SDKVERSION="8.1"
MINIOSVERSION="6.0"
VERIFYGPG=true

#Output DIR
DEST=output

# by default, we won't build for debugging purposes
if [ "${DEBUG}" == "true" ]; then
    echo "Compiling for debugging ..."
    DEBUG_CFLAGS="-O0 -fno-inline -g"
    DEBUG_LDFLAGS=""
    DEBUG_CONFIG_ARGS="--enable-debug=3 --disable-optimizations --disable-stripping --disable-asm --assert-level=2"
else
    DEBUG_CFLAGS="-g"
    DEBUG_LDFLAGS=""
    DEBUG_CONFIG_ARGS="--disable-stripping"
fi

# no need to change this since xcode build will only compile in the
# necessary bits from the libraries we create
ARCHS="i386 x86_64"

DEVELOPER=`xcode-select -print-path`


set +e # don't bail out of bash script if ccache doesn't exist
CCACHE=`which ccache`
if [ $? == "0" ]; then
    echo "Building with ccache: $CCACHE"
    CCACHE="${CCACHE} "
else
    echo "Building without ccache"
    CCACHE=""
fi
set -e # back to regular "bail out on error" mode




for ARCH in $ARCHS; do
	
	echo "Building $ARCH ......"

		PLATFORM="iPhoneSimulator"
		EXTRA_CONFIG="--arch=${ARCH} --target-os=darwin --enable-cross-compile --disable-yasm"
		EXTRA_CFLAGS="-arch ${ARCH} -miphoneos-version-min=${MINIOSVERSION} ${DEBUG_CFLAGS}"
		EXTRA_LDFLAGS="-miphoneos-version-min=${MINIOSVERSION} ${DEBUG_LDFLAGS}"

	./configure \
		--cc=${CCACHE}${DEVELOPER}/usr/bin/gcc ${EXTRA_CONFIG} \
		--prefix=$DEST/$ARCH \
		--as='/usr/local/bin/gas-preprocessor.pl' \
        	--sysroot=${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk \
        	--extra-ldflags="-arch ${ARCH} -fPIE ${EXTRA_LDFLAGS} -L${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk/usr/lib/system \
			-isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk $LDFLAGS -L${OUTPUTDIR}/lib" \
        	--extra-cflags="$CFLAGS -fPIE ${EXTRA_CFLAGS} -I${OUTPUTDIR}/include -isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk" \
        	--extra-cxxflags="$CPPFLAGS -I${OUTPUTDIR}/include -isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk" \
		--disable-ffmpeg \
		--disable-ffplay \
		--disable-ffprobe \
		--disable-ffserver \
		--disable-iconv \
		--disable-bzlib \
		--enable-avresample \
		--enable-avresample  \
		--enable-pic	\
		--disable-doc	\
		--disable-decoders \
		--enable-decoder=h264 \
		--disable-encoders \
		--disable-demuxers \
		--enable-demuxer=avi \
		--disable-muxers \
		--enable-muxer=avi \
		--disable-filters
		


	make -j4 && make install && make clean

	echo "Installed:$DEST/$ARCH"

done