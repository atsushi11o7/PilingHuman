// -*- C++ -*-
/*!
 * @file  ImageSelection.cpp
 * @brief 画像を選別する
 * @date $Date$
 *
 * $Id$
 */

#include "ImageSelection.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imageselection_spec[] =
  {
    "implementation_id", "ImageSelection",
    "type_name",         "ImageSelection",
    "description",       "画像を選別する",
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
ImageSelection::ImageSelection(RTC::Manager* manager)
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
ImageSelection::~ImageSelection()
{
}



RTC::ReturnCode_t ImageSelection::onInitialize()
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
RTC::ReturnCode_t ImageSelection::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSelection::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSelection::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageSelection::onActivated(RTC::UniqueId ec_id)
{
    //OutPortの画面サイズの初期化
    m_opImage.width = 0;
    m_opImage.height = 0;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSelection::onDeactivated(RTC::UniqueId ec_id)
{
    //画像用メモリの解放
    image.release();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageSelection::onExecute(RTC::UniqueId ec_id)
{
    if (m_ipImageIn.isNew()) {

        //画像の読み込み
        m_ipImageIn.read();
        if (m_ipImage.width != m_opImage.width || m_ipImage.height != m_opImage.height) {
            m_opImage.width = m_ipImage.width;
            m_opImage.height = m_ipImage.height;

            //必要なメモリの確保
            image.create(cv::Size(m_ipImage.width, m_opImage.height), CV_8UC3);
        }

        //αチャンネルを追加
        cv::cvtColor(image, image, cv::COLOR_RGB2RGBA);

        //読み込んだ画像をMat型に変換
        memcpy(image.data, (void*)&(m_ipImage.pixels[0]), m_ipImage.pixels.length());

        //読み込んだ画像を表示
        cv::imshow("out", image);

        //キーボードが押されるまで待つ
        i = cv::waitKey(0);

        //0キーが押された場合
        if (i == 48) {
            i = -1;

            //画像を書き出し
            m_opImage = m_ipImage;
            m_opImageOut.write();
        }
    }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageSelection::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSelection::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSelection::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSelection::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageSelection::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ImageSelectionInit(RTC::Manager* manager)
  {
    coil::Properties profile(imageselection_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageSelection>,
                             RTC::Delete<ImageSelection>);
  }

};


