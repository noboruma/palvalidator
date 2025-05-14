#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <boost/date_time.hpp>
#include "../McptConfigurationFileReader.h"
#include "../TimeSeriesCsvReader.h"
#include "../DateRange.h"
#include "../DecimalConstants.h"
#include "../DataSourceReader.h"

typedef dec::decimal<7> DecimalType;
using namespace mkc_timeseries;

TEST_CASE ("BarchartDataSourceReaderTest operations", "[BarchartReader]")
{   
    std::string apiKey = "9bff8ed715c16109a1ce5c63341bb860";
    std::shared_ptr<BarchartReader> dataSourceReader = std::make_shared<BarchartReader>(apiKey);
    std::shared_ptr<TimeSeriesCsvReader<DecimalType>> csvReader;

    SECTION("BarchartReader daily", "[BarchartReader]")
    {
        boost::gregorian::date startDate(2019, 1, 1);
        boost::gregorian::date endDate(2019, 12, 31);
        DateRange timeSeriesDateRange(startDate, endDate);

        std::string filename = dataSourceReader->createTemporaryFile("AAPL", "Daily", timeSeriesDateRange, timeSeriesDateRange, true);
        csvReader = std::make_shared<TradeStationFormatCsvReader<DecimalType>>(filename, TimeFrame::DAILY, TradingVolume::SHARES, DecimalConstants<DecimalType>::EquityTick);
        csvReader->readFile();
        
        REQUIRE(filename == "AAPL_RAD_Daily.txt");
        REQUIRE(csvReader->getTimeSeries()->getNumEntries() == 254);    // + 2 day buffer
    }

    SECTION("BarchartReader hourly", "[BarchartReader]")
    {
        boost::gregorian::date startDate(2020, 5, 1);
        boost::gregorian::date endDate(2020, 5, 31);        // should be 21 trading days
        DateRange timeSeriesDateRange(startDate, endDate);

        std::string filename = dataSourceReader->createTemporaryFile("AAPL", "Hourly", timeSeriesDateRange, timeSeriesDateRange, true);
        io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',','\"'>> hourlyCsvFileReader(filename);
        hourlyCsvFileReader.read_header(io::ignore_extra_column, "Date", "Time", "Open", "High", "Low", "Close", "Up", "Down");

        std::string dateStamp, timeStamp;
        std::string openString, highString, lowString, closeString;
        std::string up, down;
        
        int count  = 0;
        while(hourlyCsvFileReader.read_row(dateStamp, timeStamp, openString, highString, lowString, closeString, up, down))
            count++;

        REQUIRE(filename == "AAPL_RAD_Hourly.txt");
        REQUIRE(count == 161);    // + 2 day buffer
    }
}