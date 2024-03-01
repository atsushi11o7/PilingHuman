// -*- C++ -*-
/*!
 * @file  ImageProcessing.cpp
 * @brief 画像加工を行う
 * @date $Date$
 *
 * $Id$
 */

#include "ImageProcessing.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imageprocessing_spec[] =
  {
    "implementation_id", "ImageProcessing",
    "type_name",         "ImageProcessing",
    "description",       "画像加工を行う",
    "version",           "1.0.0",
    "vendor",            "Otsubo",
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
ImageProcessing::ImageProcessing(RTC::Manager* manager)
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
ImageProcessing::~ImageProcessing()
{
}



RTC::ReturnCode_t ImageProcessing::onInitialize()
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
RTC::ReturnCode_t ImageProcessing::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessing::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessing::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageProcessing::onActivated(RTC::UniqueId ec_id)
{
    //OutPortの画面サイズの初期化
    m_opImage.width = 0;
    m_opImage.height = 0;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageProcessing::onDeactivated(RTC::UniqueId ec_id)
{
    //画像用メモリ解放
    a_image.release();
    co_image.release();
    ch_image.release();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageProcessing::onExecute(RTC::UniqueId ec_id)
{
    if (m_ipImageIn.isNew()) {

        //画像の読み込み
        m_ipImageIn.read();
        if (m_ipImage.width != m_opImage.width || m_ipImage.height != m_opImage.height) {
            m_opImage.width = m_ipImage.width;
            m_opImage.height = m_ipImage.height;

            //必要なメモリの確保
            a_image.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC3);
            co_image.create(cv::Size(1024, 683), CV_8UC3);
            ch_image.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC3);
        }

        //αチャンネルを追加
        cv::cvtColor(a_image, a_image, cv::COLOR_RGB2RGBA);

        //読み込んだ画像をMat型に変換
        memcpy(a_image.data, (void*)&(m_ipImage.pixels[0]), m_ipImage.pixels.length());

        //乱数生成
        srand(time(NULL));
        s = rand() % 5 + 1;

        //マスク用の画像読み込み、サイズ変更
        co_image = cv::imread("C:/mask_image/image" + std::to_string(s) + ".jpg");
        resize(co_image, ch_image, ch_image.size(), cv::INTER_CUBIC);

        mask.create(cv::Size(m_ipImage.width, m_ipImage.height), CV_8UC3);

        //αチャンネルが0でない場所に画像をコピー
        for (int y = 0; y < a_image.rows; y++) {
            for (int x = 0; x < a_image.cols; x++) {
                cv::Vec4b px = a_image.at<cv::Vec4b>(cv::Point(x, y));
                cv::Vec3b c_px = ch_image.at<cv::Vec3b>(cv::Point(x, y));
                if (px[3] != 0) {
                    px[0] = c_px[0];
                    px[1] = c_px[1];
                    px[2] = c_px[2];
                }
                a_image.at<cv::Vec4b>(cv::Point(x, y)) = px;
            }
        }

        //確認用
        //cv::imshow("out", a_image);
        //cv::waitKey(0);
        //name = "C:/Image/image" + std::to_string(count) + ".png";
        //name = "C:/Users/81806/OneDrive/デスクトップ/PilingHuman/Assets/Resources/image" + std::to_string(count) + ".png";
        //cv::imwrite(name, a_image);
        //count++;

        //画像を出力
        len = a_image.channels() * a_image.cols * a_image.rows;
        m_opImage.pixels.length(len);
        memcpy((void*)&(m_opImage.pixels[0]), a_image.data, len);
        m_opImageOut.write();

        mask.release();

    }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageProcessing::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessing::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessing::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessing::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessing::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ImageProcessingInit(RTC::Manager* manager)
  {
    coil::Properties profile(imageprocessing_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageProcessing>,
                             RTC::Delete<ImageProcessing>);
  }

};


