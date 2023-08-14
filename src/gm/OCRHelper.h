//
// Created by c0518535x on 2023/8/2.
//

#ifndef GAMELUA_OCRHELPER_H
#define GAMELUA_OCRHELPER_H

#include <QDebug>
#include <QString>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <tesseract/publictypes.h>
#include <tesseract/baseapi.h>


namespace gm{

    class OCRHelper {
    private:
        tesseract::TessBaseAPI* _ocr;
        cv::Mat prepare(cv::Mat mat);
        OCRHelper();
    public:
        ~OCRHelper();

        OCRHelper(const OCRHelper &) = delete;
        OCRHelper(OCRHelper&&) = delete;

        OCRHelper operator=(const OCRHelper &) =delete;
        OCRHelper operator=(OCRHelper &&) = delete;

        static OCRHelper& instance(){
            static OCRHelper _instance;
            return _instance;
        }

        // 识别问题
        char* GetUTF8Text(cv::Mat mat);
    };
}



#endif //GAMELUA_OCRHELPER_H
