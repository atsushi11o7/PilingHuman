// -*- C++ -*-
/*!
 * @file  ImageProcessingTest.cpp
 * @brief 画像加工を行う
 * @date $Date$
 *
 * $Id$
 */

#include "ImageProcessingTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* imageprocessing_spec[] =
  {
    "implementation_id", "ImageProcessingTest",
    "type_name",         "ImageProcessingTest",
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
ImageProcessingTest::ImageProcessingTest(RTC::Manager* manager)
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
ImageProcessingTest::~ImageProcessingTest()
{
}



RTC::ReturnCode_t ImageProcessingTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("opImage", m_opImageIn);

  // Set OutPort buffer
  addOutPort("ipImage", m_ipImageOut);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageProcessingTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessingTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessingTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ImageProcessingTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageProcessingTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ImageProcessingTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ImageProcessingTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessingTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessingTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessingTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ImageProcessingTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void ImageProcessingTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(imageprocessing_spec);
    manager->registerFactory(profile,
                             RTC::Create<ImageProcessingTest>,
                             RTC::Delete<ImageProcessingTest>);
  }

};


