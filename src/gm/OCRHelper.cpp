//
// Created by c0518535x on 2023/8/2.
//

#include "OCRHelper.h"



gm::OCRHelper::OCRHelper() {

    _ocr = new tesseract::TessBaseAPI();
    _ocr->Init("./data", "chi_sim_vert");
    _ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}



gm::OCRHelper::~OCRHelper() noexcept {
    _ocr->End();
}

cv::Mat gm::OCRHelper::prepare(cv::Mat mat) {
    cv::Mat mg1, mg2,t;
    cv::cvtColor(mat, mg1, cv::COLOR_BGR2GRAY);  // 灰度化
    cv::GaussianBlur(mg1, mg2, cv::Size(1, 1), 0.5,0.5);  // 高斯模糊
    cv::threshold(mg2,t,200,255,cv::THRESH_BINARY_INV );  // 二值化
    cv::resize(t,t,cv::Size(),1,1,cv::INTER_LINEAR );
    cv::imwrite("prepare.bmp",t);
    return t;
}

char *gm::OCRHelper::GetUTF8Text(cv::Mat mat) {
    auto t = prepare(mat);
    _ocr->SetImage(t.data,t.size().width,t.size().height,t.channels(),t.step1());
    auto txt = _ocr->GetUTF8Text();
    qDebug() << "识别文字结果： "  << txt;
    return txt;
}