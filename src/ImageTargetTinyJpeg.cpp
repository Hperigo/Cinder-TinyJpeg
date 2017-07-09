//
//  ImageTargetTinyJpeg.cpp
//  tinyJpeg
//
//  Created by Henrique on 7/8/17.
//
//

/*
 Copyright (c) 2015, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "ImageTargetTinyJpeg.h"
#include "cinder/Log.h"

#define TJE_IMPLEMENTATION

#include "TinyJPEG/tiny_jpeg.h"

namespace cinder {

    void ImageTargetTinyJpeg::registerSelf()
    {
        static bool alreadyRegistered = false;
        const int32_t PRIORITY = 3;

        if( alreadyRegistered )
            return;
        alreadyRegistered = true;

        ImageIoRegistrar::TargetCreationFunc func = ImageTargetTinyJpeg::create;
        ImageIoRegistrar::registerTargetType( "jpg", func, PRIORITY, "jpg" );
        ImageIoRegistrar::registerTargetType( "jpeg", func, PRIORITY, "jpeg" );

    }

    ImageTargetRef ImageTargetTinyJpeg::create( DataTargetRef dataTarget, ImageSourceRef imageSource, ImageTarget::Options options, const std::string &extensionData )
    {
        return ImageTargetRef( new ImageTargetTinyJpeg( dataTarget, imageSource, options, extensionData ) );
    }

    ImageTargetTinyJpeg::ImageTargetTinyJpeg( DataTargetRef dataTarget, ImageSourceRef imageSource, ImageTarget::Options options, const std::string &extensionData )
    : mDataTarget( dataTarget )
    {
        if( ! ( mDataTarget->providesFilePath() || mDataTarget->getStream() ) ) {
            throw ImageIoExceptionFailedWrite( "No file path or stream provided" );
        }

        setSize( imageSource->getWidth(), imageSource->getHeight() );
        ImageIo::ColorModel cm = options.isColorModelDefault() ? imageSource->getColorModel() : options.getColorModel();

        switch( cm ) {
            case ImageIo::ColorModel::CM_RGB:
                mNumComponents = 3;
                setColorModel( ImageIo::ColorModel::CM_RGB );
                setChannelOrder( ImageIo::ChannelOrder::RGB );
                break;
            case ImageIo::ColorModel::CM_GRAY:
                mNumComponents = 1;
                setColorModel( ImageIo::ColorModel::CM_GRAY );
                setChannelOrder( ImageIo::ChannelOrder::Y );
                break;
            default:
                throw ImageIoExceptionIllegalColorModel();
        }

        mExtension = extensionData;

        setDataType( ImageIo::DataType::UINT8 );
        mRowBytes = mNumComponents * imageSource->getWidth() * sizeof(uint8_t);

        if( mDataTarget->providesFilePath() ) {
            mFilePath = dataTarget->getFilePath();
        }

        quality = options.getQuality();

        mData = std::unique_ptr<uint8_t[]>( new uint8_t[mHeight * mRowBytes] );
    }

    void* ImageTargetTinyJpeg::getRowPointer( int32_t row )
    {
        return &mData.get()[row * mRowBytes];
    }

    void tinyJpegWriteToStream( void *context, void *data, int size )
    {
        OStream *stream = reinterpret_cast<OStream *>( context );
        stream->writeData( data, static_cast<size_t>( size ) );
    }

    void ImageTargetTinyJpeg::finalize()
    {

        if( ! mFilePath.empty() ) {

            if( mExtension == "jpeg" ) {
                if ( !tje_encode_to_file(mFilePath.string().c_str(), (int)mWidth, (int)mHeight, mNumComponents, mData.get() ) ) {
                    throw ImageIoExceptionFailedWrite();
                }
            }
        }
        else {
            OStream *stream = mDataTarget->getStream().get();
            if( mExtension == "jpeg" ) {
                if(! tje_encode_with_func( tinyJpegWriteToStream, stream, 1, (int)mWidth, (int)mHeight, mNumComponents, mData.get() ) ){
                    throw ImageIoExceptionFailedWrite();
                }

            }
        }
    }

} // namespace cinder
