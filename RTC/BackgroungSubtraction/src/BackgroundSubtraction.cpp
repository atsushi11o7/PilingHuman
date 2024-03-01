// -*- C++ -*-
/*!
 * @file  BackgroundSubtraction.cpp
 * @brief 背景差分を行う
 * @date $Date$
 *
 * $Id$
 */

#include "BackgroundSubtraction.h"

// Module specification
// <rtc-template block="module_spec">
static const char* backgroundsubtraction_spec[] =
  {
    "implementation_id", "BackgroundSubtraction",
    "type_name",         "BackgroundSubtraction",
    "description",       "背景差分を行う",
    "version",           "1.0.0",
    "vendor",            "Atsushi",
    "category",          "Piling Human",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
BackgroundSubtraction::BackgroundSubtraction(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ipImageIn("ipImage", m_ipImage),
    m_opImageOut("opImage", m_opImage)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
BackgroundSubtraction::~BackgroundSubtraction()
{
}



RTC::ReturnCode_t BackgroundSubtraction::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("ipImage", m_ipImageIn);

  // Set OutPort buffer
  addOutPort("opImage", m_opImageOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackgroundSubtraction::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtraction::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtraction::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t BackgroundSubtraction::onActivated(RTC::UniqueId ec_id)
{
    //OutPortの画面サイズの初期化
    m_opImage.width = 0;
    m_opImage.height = 0;
    i = 0;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackgroundSubtraction::onDeactivated(RTC::UniqueId ec_id)
{
    //画像用メモリの解放
    background.release();
    bg_gray.release();
    image.release();
    im_gray.release();
    diff.release();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackgroundSubtraction::onExecute(RTC::UniqueId ec_id)
{
    //一番最初に5秒待つ
    if (i == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    if (m_ipImageIn.isNew()) {

        //画像の読み込み
        m_ipImageIn.read();
        if (m_ipImage.width != m_opImage.width || m_ipImage.height != m_opImage.height) {
            m_opImage.width = m_ipImage.width;
            m_opImage.height = m_ipImage.height;

            //必要なメモリの確保
            background.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC3);
            bg_gray.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC1);
            image.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC3);
            im_gray.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC1);
            diff.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC1);
        }

        if (i == 0) {
            //一番最初に読み込んだ画像を背景として設定、Mat型に変換
            memcpy(background.data, (void*)&(m_ipImage.pixels[0]), m_ipImage.pixels.length());

            //背景をグレースケールに変換
            cv::cvtColor(background, bg_gray, cv::COLOR_BGR2GRAY);
            i = 1;
        }
        else {
            //読み込んだ画像をMat型に変換
            memcpy(image.data, (void*)&(m_ipImage.pixels[0]), m_ipImage.pixels.length());

            //読み込んだ画像をグレースケールに変換
            cv::cvtColor(image, im_gray, cv::COLOR_BGR2GRAY);

            //背景と読み込んだ画像の差分を取る
            cv::absdiff(im_gray, bg_gray, diff);

            //背景差分の結果を二値化する
            cv::threshold(diff, diff, 20, 255, cv::THRESH_BINARY);

            //画像の平滑化
            cv::medianBlur(diff, diff, 5);

            //輪郭抽出に必要な変数の宣言
            vector<vector<Point> > contours;
            double max_area = 0;
            int max_area_contour = -1;

            //輪郭の抽出
            cv::findContours(diff, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

            //抽出した輪郭の中で、一番内部の面積が大きいものを調べる
            for (int j = 0; j < contours.size(); j++) {
                double area = contourArea(contours.at(j), false);
                if (max_area < area) {
                    max_area = area;
                    max_area_contour = j;
                }
            }

            //抽出した輪郭を保存するためのメモリを確保
            mask.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC1);
            im_mask.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC3);

            //一番大きな輪郭の内部を塗りつぶして画像として保存
            //mask = Mat::zeros(m_ipImage.width, m_ipImage.height, CV_8UC1);
            cv::drawContours(mask, contours, max_area_contour, 255, -1);

            //白と黒に二値化する
            cv::threshold(mask, mask, 254, 255, cv::THRESH_BINARY);

            //画像にαチャンネルを追加
            cv::cvtColor(mask, im_mask, cv::COLOR_GRAY2BGRA);

            //透過処理
            for (int y = 0; y < im_mask.rows; y++) {
                for (int x = 0; x < im_mask.cols; x++) {
                    cv::Vec4b px = im_mask.at<cv::Vec4b>(cv::Point(x, y));
                    if (px[0] + px[1] + px[2] == 0) {
                        px[3] = 0;
                    }
                    else {
                        px[3] = 255;
                    }
                    im_mask.at<cv::Vec4b>(cv::Point(x, y)) = px;
                }
            }


            //確認用
            //cv::imshow("out", im_mask);
            //cv::waitKey(0);
            //name = "C:/Image/image" + std::to_string(count)+".png";
            //cv::imwrite(name, im_mask);
            //count++;


            //結果を書き出し
            len = im_mask.channels() * im_mask.cols * im_mask.rows;
            m_opImage.pixels.length(len);
            memcpy((void*)&(m_opImage.pixels[0]), im_mask.data, len);
            m_opImageOut.write();

            //画像用メモリの解放
            mask.release();
            im_mask.release();
        }
    }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackgroundSubtraction::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtraction::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtraction::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtraction::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtraction::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void BackgroundSubtractionInit(RTC::Manager* manager)
  {
    coil::Properties profile(backgroundsubtraction_spec);
    manager->registerFactory(profile,
                             RTC::Create<BackgroundSubtraction>,
                             RTC::Delete<BackgroundSubtraction>);
  }

};


