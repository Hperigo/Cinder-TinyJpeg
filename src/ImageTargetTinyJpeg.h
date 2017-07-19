//
//  ImageTargetTinyJpeg.h
//  tinyJpeg
//
//  Created by Henrique on 7/8/17.
//
//

#ifndef ImageTargetTinyJpeg_h
#define ImageTargetTinyJpeg_h


#include "cinder/Cinder.h"
#include "cinder/ImageIo.h"



namespace cinder {

    typedef std::shared_ptr<class ImageTargetTinyJpeg> ImageTargetTinyJpegRef;

    class ImageTargetTinyJpeg : public ImageTarget {
    public:
        static ImageTargetRef		create( DataTargetRef dataTarget, ImageSourceRef imageSource, ImageTarget::Options options, const std::string &extensionData );
        
        void*	getRowPointer( int32_t row ) override;
        void	finalize() override;
        
        static void		registerSelf();
        
    protected:
        ImageTargetTinyJpeg( DataTargetRef dataTarget, ImageSourceRef imageSource, ImageTarget::Options options, const std::string &extensionData );
        
        uint8_t						mNumComponents;
        size_t						mRowBytes;
        std::string					mExtension;
        fs::path					mFilePath;
        std::unique_ptr<uint8_t[]>	mData;
        DataTargetRef				mDataTarget;
        
        int                       quality = 1; // Tiny jpeg quality is 1,2,3. 3 is best
    };
    
}
#endif /* ImageTargetTinyJpeg_h */
