#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../TimeSeriesCsvReader.h"
#include "../DecimalConstants.h"
#include "TestUtils.h"

using namespace mkc_timeseries;
using namespace boost::gregorian;



TEST_CASE ("TimeSeriesCsvReader operations", "[TimeSeriesCsvReader]")
{
  //  PALFormatCsvReader<6> csvFile ("TY_RAD.csv", TimeFrame::DAILY, TradingVolume::CONTRACTS);
  // csvFile.readFile();

  TradeStationFormatCsvReader<DecimalType> gildCsvFile ("GILD.txt", TimeFrame::DAILY, TradingVolume::SHARES, DecimalConstants<DecimalType>::EquityTick);
  gildCsvFile.readFile();

  TradeStationFormatCsvReader<DecimalType> ssoCsvFile ("SSO_RAD_Hourly.txt", TimeFrame::INTRADAY, TradingVolume::SHARES, DecimalConstants<DecimalType>::EquityTick);
 
  ssoCsvFile.readFile();

  DecimalType dollarTick (dec::fromString<DecimalType> (std::string ("0.005")));
  CSIExtendedFuturesCsvReader<DecimalType> dollarIndexCsvFile ("DX20060R.txt", TimeFrame::DAILY, TradingVolume::CONTRACTS, dollarTick);
  dollarIndexCsvFile.readFile();

  std::shared_ptr<OHLCTimeSeries<DecimalType>> dollarIndexTimeSeries = dollarIndexCsvFile.getTimeSeries();

  CSIErrorCheckingExtendedFuturesCsvReader<DecimalType> dollarIndexErrorCheckedCsvFile ("DX20060R.txt", TimeFrame::DAILY, TradingVolume::CONTRACTS,
											dollarTick);

  DecimalType poundTick (dec::fromString<DecimalType> (std::string ("0.0001")));
  PinnacleErrorCheckingFormatCsvReader<DecimalType> britishPoundCsvFile ("BN_RAD.csv", 
									 TimeFrame::DAILY, 
									 TradingVolume::CONTRACTS,
									 poundTick);
  britishPoundCsvFile.readFile();
  // std::shared_ptr<OHLCTimeSeries<6>> p = csvFile.getTimeSeries();
  std::shared_ptr<OHLCTimeSeries<DecimalType>> gildTimeSeries = gildCsvFile.getTimeSeries();
  std::shared_ptr<OHLCTimeSeries<DecimalType>> ssoTimeSeries = ssoCsvFile.getTimeSeries();

  SECTION ("Timeseries time frame test", "[TimeSeriesCsvReader]")
    {
      //    REQUIRE (p->getTimeFrame() == TimeFrame::DAILY);
      REQUIRE (gildTimeSeries->getTimeFrame() == TimeFrame::DAILY);
      REQUIRE (dollarIndexTimeSeries->getTimeFrame() == TimeFrame::DAILY);
      REQUIRE (ssoTimeSeries->getTimeFrame() == TimeFrame::INTRADAY);
    }

  SECTION ("Timeseries first date test", "[TimeSeriesCsvReader]")
    {
      boost::gregorian::date refDate1 (1985, Nov, 20);
      boost::gregorian::date refDate2 (1992, Jan, 23);

      //REQUIRE (p->getFirstDate() == refDate1);
      REQUIRE (gildTimeSeries->getFirstDate() == refDate2);
      REQUIRE (dollarIndexTimeSeries->getFirstDate() == refDate1);
      time_duration barTime(13, 0, 0);

      ptime firstDateTime (refDate2, barTime);

      REQUIRE(gildTimeSeries->getFirstDateTime() == firstDateTime);

      boost::gregorian::date ssoFirstDate (2012, Apr, 02);
      time_duration ssoFirstTime(9, 0, 0);

      ptime ssoDateTime(ssoFirstDate, ssoFirstTime);
      REQUIRE(ssoTimeSeries->getFirstDateTime() == ssoDateTime);
    }

  SECTION ("Timeseries last date test", "[TimeSeriesCsvReader]")
    {
      boost::gregorian::date refDate1 (2016, Jun, 2);
      boost::gregorian::date refDate2 (2016, Apr, 6);

      //REQUIRE (p->getLastDate() == refDate1);
      REQUIRE (dollarIndexTimeSeries->getLastDate() == refDate1);
      REQUIRE (gildTimeSeries->getLastDate() == refDate2);

      time_duration barTime(13, 0, 0);
      ptime lastDateTime (refDate2, barTime);
      REQUIRE(gildTimeSeries->getLastDateTime() == lastDateTime);

      boost::gregorian::date ssoLastDate (2021, Apr, 01);
      time_duration ssoLastTime(15, 0, 0);

      ptime ssoDateTime(ssoLastDate, ssoLastTime);
      REQUIRE(ssoTimeSeries->getLastDateTime() == ssoDateTime);

    }

  SECTION ("Timeseries get OHLC tests", "[TimeSeriesCsvReader]")
    {
      OHLCTimeSeries<DecimalType>::ConstRandomAccessIterator it = dollarIndexTimeSeries->beginRandomAccess();

      DecimalType refOpen (dec::fromString<DecimalType> (std::string ("186.145")));
      REQUIRE ((*it).getOpenValue() == refOpen);

      DecimalType refHigh (dec::fromString<DecimalType> (std::string ("187.895")));
      REQUIRE ((*it).getHighValue() == refHigh);

      DecimalType refLow (dec::fromString<DecimalType> (std::string ("186.075")));
      REQUIRE ((*it).getLowValue() == refLow);

      DecimalType refClose (dec::fromString<DecimalType> (std::string ("187.615")));
      REQUIRE ((*it).getCloseValue() == refClose);

      it = it + 19;
      boost::gregorian::date refDate1 (1985, Dec, 18);
      REQUIRE ((*it).getDateValue() == refDate1);
     DecimalType refOpen0 (dec::fromString<DecimalType> (std::string ("184.370")));
      REQUIRE ((*it).getOpenValue() == refOpen0);

      DecimalType refHigh0 (dec::fromString<DecimalType> (std::string ("185.200")));
      REQUIRE ((*it).getHighValue() == refHigh0);

      DecimalType refLow0 (dec::fromString<DecimalType> (std::string ("184.325")));
      REQUIRE ((*it).getLowValue() == refLow0);

      DecimalType refClose0 (dec::fromString<DecimalType> (std::string ("185.095")));
      REQUIRE ((*it).getCloseValue() == refClose0);
    }

  /*
  SECTION ("Timeseries OHLC test", "[TimeSeriesCsvReader]")
    {
      OHLCTimeSeries<6>::RandomAccessIterator it = p->beginRandomAccess();
      decimal<6> refOpen (dec::fromString<decimal<6>> (std::string ("31.590276")));
      REQUIRE ((*it)->getOpenValue() == refOpen);

      decimal<6> refHigh (dec::fromString<decimal<6>> (std::string ("31.861192")));
      REQUIRE ((*it)->getHighValue() == refHigh);

      decimal<6> refLow (dec::fromString<decimal<6>> (std::string ("31.583147")));
      REQUIRE ((*it)->getLowValue() == refLow);

      decimal<6> refClose (dec::fromString<decimal<6>> (std::string ("31.832674")));
      REQUIRE ((*it)->getCloseValue() == refClose);

      it = it + 18;
      boost::gregorian::date refDate1 (1985, Mar, 27);
      REQUIRE ((*it)->getDateValue() == refDate1);
     decimal<6> refOpen0 (dec::fromString<decimal<6>> (std::string ("31.996650")));
      REQUIRE ((*it)->getOpenValue() == refOpen0);

      decimal<6> refHigh0 (dec::fromString<decimal<6>> (std::string ("32.060813")));
      REQUIRE ((*it)->getHighValue() == refHigh0);

      decimal<6> refLow0 (dec::fromString<decimal<6>> (std::string ("31.911098")));
      REQUIRE ((*it)->getLowValue() == refLow0);

      decimal<6> refClose0 (dec::fromString<decimal<6>> (std::string ("31.946744")));
      REQUIRE ((*it)->getCloseValue() == refClose0);

      it = p->endRandomAccess();
      it--;

      decimal<6> refOpen2 (dec::fromString<decimal<6>> (std::string ("117.794029")));
      REQUIRE ((*it)->getOpenValue() == refOpen2);

      decimal<6> refHigh2 (dec::fromString<decimal<6>> (std::string ("118.216285")));
      REQUIRE ((*it)->getHighValue() == refHigh2);

      decimal<6> refLow2 (dec::fromString<decimal<6>> (std::string ("117.639205")));
      REQUIRE ((*it)->getLowValue() == refLow2);

      decimal<6> refClose2 (dec::fromString<decimal<6>> (std::string ("118.117759")));
      REQUIRE ((*it)->getCloseValue() == refClose2);

    }
  */
}
