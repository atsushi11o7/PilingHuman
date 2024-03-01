// -*- C++ -*-
/*!
 * @file  BackgroundSubtractionTest.cpp
 * @brief 背景差分を行う
 * @date $Date$
 *
 * $Id$
 */

#include "BackgroundSubtractionTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* backgroundsubtraction_spec[] =
  {
    "implementation_id", "BackgroundSubtractionTest",
    "type_name",         "BackgroundSubtractionTest",
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
BackgroundSubtractionTest::BackgroundSubtractionTest(RTC::Manager* manager)
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
BackgroundSubtractionTest::~BackgroundSubtractionTest()
{
}



RTC::ReturnCode_t BackgroundSubtractionTest::onInitialize()
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
RTC::ReturnCode_t BackgroundSubtractionTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t BackgroundSubtractionTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackgroundSubtractionTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BackgroundSubtractionTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BackgroundSubtractionTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void BackgroundSubtractionTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(backgroundsubtraction_spec);
    manager->registerFactory(profile,
                             RTC::Create<BackgroundSubtractionTest>,
                             RTC::Delete<BackgroundSubtractionTest>);
  }

};


