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
SDK_VERSION="8.1"
#ARCH
ARCHS="armv7 armv7s arm64"
#Output DIR
DEST=output

DEVPATH=/Applications/XCode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS${SDK_VERSION}.sdk

export CC=`xcodebuild -find clang`

for ARCH in $ARCHS; do
	
	echo "Building $ARCH ......"
	./configure \
	    --enable-cross-compile \
		--target-os=darwin \
		--arch=$ARCH \
		--sysroot=$DEVPATH \
		--as='/usr/local/bin/gas-preprocessor.pl' \
		--extra-cflags="-arch $ARCH" \
	    	--extra-ldflags="-L$DEVPATH/usr/lib/system -arch $ARCH" \
		--prefix=$DEST/$ARCH \
		--disable-ffmpeg \
		--disable-ffplay \
		--disable-ffprobe \
		--disable-ffserver \
		--disable-iconv \
		--disable-bzlib \
		--enable-avresample \
		--disable-armv6	\
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
		--disable-armv5te \
		--disable-debug


	make -j4 && make install && make clean

	echo "Installed:$DEST/$ARCH"

done