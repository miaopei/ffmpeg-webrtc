#########################################################################
# File Name: build-ffmpeg.sh
# Author: Mr.Miaow
# mail: miaopei@baicells.com
# Created Time: 三  5/22 16:37:55 2019
#########################################################################
#!/bin/sh

# directories
PLATFORM="ffmpeg-sdk"
# FFmpeg脚本目录
SOURCE="ffmpeg-3.4.6"
#FAT="$PLATFORM/$SOURCE/FFmpeg-OSX"

BUILD="$PLATFORM/$SOURCE/build"
FFMPEG_SDK=`pwd`/"$PLATFORM/$SOURCE/sdk"

# CONFIGURE_FLAGS="--enable-cross-compile \
# --enable-nonfree \
# --enable-libfaac \
# --enable-libmp3lame \
# --enable-libtheora \
# --enable-libvorbis \
# --enable-libxvid \
# --enable-libass \
# --enable-libspeex \
# --enable-libx264 \
# --enable-libvpx \
# --enable-opencl \
# --disable-ffmpeg \
# --enable-gpl \
# --disable-outdev=sdl \
# --disable-ffplay \
# --disable-debug  \
# --disable-ffserver \
# --enable-static \
# --enable-shared "

#CONFIGURE_FLAGS="--enable-cross-compile \
#    --enable-nonfree \
#    --enable-opencl \
#    --disable-ffmpeg \
#    --enable-gpl \
#    --disable-outdev=sdl \
#    --disable-ffplay \
#    --disable-debug  \
#    --disable-ffserver \
#    --enable-static \
#    --enable-shared "

CONFIGURE_FLAGS="--enable-cross-compile \
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
		--disable-filters \
        --enable-static \
        --enable-shared "

CWD=`pwd`    
ARCHS="x86_64 i386"
#ARCHS="i386"

# FAAC="/usr/local/Cellar/faac/1.28"
# LAMP="/usr/local/Cellar/lame/3.99.5"
# THEORA="/usr/local/Cellar/theora/1.1.1"
# VORBIS="/usr/local/Cellar/libvorbis/1.3.5"
# XVID="/usr/local/Cellar/libxvid/1.3.4"
# ASS="/usr/local/Cellar/libass/0.13.2"
# SPEEX="/usr/local/Cellar/speex/1.2rc1"
# VPX="/usr/local/Cellar/libvpx/1.6.0"
# X264="/usr/local/Cellar/x264/r2668"

for ARCH in $ARCHS
do
    echo "building $ARCH..."
    mkdir -p "$BUILD/$ARCH"
    cd "$BUILD/$ARCH"

    CFLAGS="-arch $ARCH"

    SDK="macosx"

    XCRUN_SDK=`echo $SDK | tr '[:upper:]' '[:lower:]'`
    CC="xcrun -sdk $XCRUN_SDK clang"
    CXXFLAGS="$CFLAGS"
    LDFLAGS="$CFLAGS"

    if [ "$FAAC"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libfaac"
        CXXFLAGS="$CXXFLAGS -I$FAAC/include"
        LDFLAGS="$LDFLAGS -L$FAAC/lib"
    fi
    if [ "$LAMP"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libmp3lame"
        CXXFLAGS="$CXXFLAGS -I$LAMP/include"
        LDFLAGS="$LDFLAGS -L$LAMP/lib"
    fi
    if [ "$THEORA"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libtheora"
        CXXFLAGS="$CXXFLAGS -I$THEORA/include"
        LDFLAGS="$LDFLAGS -L$THEORA/lib"
    fi
    if [ "$VORBIS"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libvorbis"
        CXXFLAGS="$CXXFLAGS -I$VORBIS/include"
        LDFLAGS="$LDFLAGS -L$VORBIS/lib"
    fi
    if [ "$XVID"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libxvid"
        CXXFLAGS="$CXXFLAGS -I$XVID/include"
        LDFLAGS="$LDFLAGS -L$XVID/lib"
    fi
    if [ "$ASS"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libass"
        CXXFLAGS="$CXXFLAGS -I$ASS/include"
        LDFLAGS="$LDFLAGS -L$ASS/lib"
    fi
    if [ "$SPEEX"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libspeex"
        CXXFLAGS="$CXXFLAGS -I$SPEEX/include"
        LDFLAGS="$LDFLAGS -L$SPEEX/lib"
    fi
    if [ "$VPX"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libvpx"
        CXXFLAGS="$CXXFLAGS -I$VPX/include"
        LDFLAGS="$LDFLAGS -L$VPX/lib"
    fi
    if [ "$X264"  ]
    then
        CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libx264"
        CXXFLAGS="$CXXFLAGS -I$X264/include"
        LDFLAGS="$LDFLAGS -L$X264/lib"
    fi

    TMPDIR=${TMPDIR/%\/} $CWD/$SOURCE/configure \
        --target-os=darwin \
        --arch=$ARCH \
        $CONFIGURE_FLAGS \
        --extra-cflags="$CXXFLAGS" \
        --extra-ldflags="$LDFLAGS" \
        --prefix="$FFMPEG_SDK/$ARCH" \
    || exit 1
    # --cc="$CC" \

    make -j4 && make install $EXPORT && make clean || exit 1

	echo "Installed:$FFMPEG_SDK/$ARCH"
    cd $CWD
done
echo Done
