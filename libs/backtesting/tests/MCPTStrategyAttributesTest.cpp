#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../MCPTStrategyAttributes.h"
#include "number.h"
#include "TestUtils.h"

using namespace mkc_timeseries;
using namespace boost::gregorian;


TEST_CASE ("MCPTStrategyAttributes operations", "[MCPTStrategyAttributes]")
{
 auto entryLast = createTimeSeriesEntry ("20160107", "195.33", "197.44", "193.59","194.05",
				   142662900);
  auto entry0 = createTimeSeriesEntry ("20160106", "198.34", "200.06", "197.60","198.82",
				   142662900);

  auto entry1 = createTimeSeriesEntry ("20160105", "201.40", "201.90", "200.05","201.36",
				   105999900);

  auto entry2 = createTimeSeriesEntry ("20160104", "200.49", "201.03", "198.59","201.02",
				   222353400);

  auto entry3 = createTimeSeriesEntry ("20151231", "205.13", "205.89", "203.87","203.87",
				   114877900);

  auto entry4 = createTimeSeriesEntry ("20151230", "207.11", "207.21", "205.76","205.93",
				   63317700);

  auto entry5 = createTimeSeriesEntry ("20151229", "206.51", "207.79", "206.47","207.40",
				   92640700);

  auto entry6 = createTimeSeriesEntry ("20151228", "204.86", "205.26", "203.94","205.21",
				   65899900);
  auto entry7 = createTimeSeriesEntry ("20151224", "205.72", "206.33", "205.42", "205.68",
				   48542200);
  auto entry8 = createTimeSeriesEntry ("20151223", "204.69", "206.07", "204.58", "206.02",
				   48542200); 

  

  auto spySeries = std::make_shared<OHLCTimeSeries<DecimalType>>(TimeFrame::DAILY, TradingVolume::SHARES);
  spySeries->addEntry(*(entryLast));
  spySeries->addEntry(*(entry8));
  spySeries->addEntry(*(entry7));
  spySeries->addEntry(*(entry6));
  spySeries->addEntry(*(entry5));
  spySeries->addEntry(*(entry4));
  spySeries->addEntry(*(entry3));
  spySeries->addEntry(*(entry2));
  spySeries->addEntry(*(entry1));
  spySeries->addEntry(*(entry0));

  std::string equitySymbol("SPY");
  std::string equityName("SPDR S&P 500 ETF");

  auto spy = std::make_shared<EquitySecurity<DecimalType>>(equitySymbol, equityName, spySeries);

  
  
  MCPTStrategyAttributes<DecimalType> attributesLong;
  MCPTStrategyAttributes<DecimalType> attributesShort;
  REQUIRE (attributesLong.numTradingOpportunities() == 0);
  REQUIRE (attributesShort.numTradingOpportunities() == 0);

  attributesLong.addFlatPositionBar (spy, entry7->getDateValue());
  attributesLong.addLongPositionBar (spy, entry6->getDateValue());
  attributesLong.addLongPositionBar (spy, entry5->getDateValue());
  attributesLong.addLongPositionBar (spy, entry4->getDateValue());
  attributesLong.addLongPositionBar (spy, entry3->getDateValue());
  attributesLong.addLongPositionBar (spy, entry2->getDateValue());
  attributesLong.addLongPositionBar (spy, entry1->getDateValue());
  attributesLong.addLongPositionBar (spy, entry0->getDateValue());
  //REQUIRE_THROWS (attributesLong.addLongPositionBar (spy, entry0->getDateValue()));
  REQUIRE (attributesLong.numTradingOpportunities() == 8);

  attributesShort.addFlatPositionBar (spy, entry7->getDateValue());
  attributesShort.addShortPositionBar (spy, entry6->getDateValue());
  attributesShort.addShortPositionBar (spy, entry5->getDateValue());
  attributesShort.addShortPositionBar (spy, entry4->getDateValue());
  attributesShort.addShortPositionBar (spy, entry3->getDateValue());
  attributesShort.addShortPositionBar (spy, entry2->getDateValue());
  attributesShort.addShortPositionBar (spy, entry1->getDateValue());
  attributesShort.addShortPositionBar (spy, entry0->getDateValue());
  //REQUIRE_THROWS (attributesShort.addShortPositionBar (spy, entry0->getDateValue()));

  REQUIRE (attributesShort.numTradingOpportunities() == 8);

  SECTION ("MCPTStrategyAttributes");
  {
  }


  SECTION ("MCPTStrategyAttributes Long PositionDirectionIterator operations");
  {
     MCPTStrategyAttributes<DecimalType>::PositionDirectionIterator it = 
      attributesLong.beginPositionDirection();

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 24));
    REQUIRE (it->second == 0);
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 28));
    REQUIRE (it->second == 1);
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 29));
    REQUIRE (it->second == 1);

    it = attributesLong.endPositionDirection();
    it--;

    REQUIRE (it->first == TimeSeriesDate (2016, Jan, 6));
    REQUIRE (it->second == 1);

  }


SECTION ("MCPTStrategyAttributes Long PositionResultsIterator operations");
  {
     MCPTStrategyAttributes<DecimalType>::PositionReturnsIterator it = 
      attributesLong.beginPositionReturns();

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 24));
    REQUIRE (it->second == calculatePercentReturn (entry8->getCloseValue(),
						   entry7->getCloseValue()));
    //std::cout << "Return 1 " << it->second << std::endl;
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 28));
    REQUIRE (it->second == calculatePercentReturn (entry7->getCloseValue(),
						   entry6->getCloseValue()));
    //std::cout << "Return 2 " << it->second << std::endl;
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 29));
    REQUIRE (it->second == calculatePercentReturn (entry6->getCloseValue(),
						   entry5->getCloseValue()));
    //std::cout << "Return 3 " << it->second << std::endl;

    it = attributesLong.endPositionReturns();
    it--;

    REQUIRE (it->first == TimeSeriesDate (2016, Jan, 6));
    REQUIRE (it->second == calculatePercentReturn (entry1->getCloseValue(),
						   entry0->getCloseValue()));
     
    //std::cout << "Return 4 " << it->second << std::endl;
  }

  SECTION ("MCPTStrategyAttributes Long Vector position return operations");
  {
     MCPTStrategyAttributes<DecimalType>::PositionReturnsIterator it = 
      attributesLong.beginPositionReturns();

     std::vector<DecimalType> positionReturnsVector (attributesLong.getPositionReturns());
     std::vector<DecimalType>::const_iterator vectorIt = positionReturnsVector.begin();

     REQUIRE (it->first == TimeSeriesDate (2015, Dec, 24));
     REQUIRE (it->second == *vectorIt);
     //std::cout << "Return 1 " << it->second << std::endl;
     it++;
     vectorIt++;

     REQUIRE (it->first == TimeSeriesDate (2015, Dec, 28));
     REQUIRE (it->second == *vectorIt);
     //std::cout << "Return 2 " << it->second << std::endl;
     it++;
     vectorIt++;

     REQUIRE (it->first == TimeSeriesDate (2015, Dec, 29));
     REQUIRE (it->second == *vectorIt);
    //std::cout << "Return 3 " << it->second << std::endl;
  }

  SECTION ("MCPTStrategyAttributes Long PositionDirection vector operations");
  {
     MCPTStrategyAttributes<DecimalType>::PositionDirectionIterator it = 
      attributesLong.beginPositionDirection();
     std::vector<int> positionReturnsVector (attributesLong.getPositionDirection());
     std::vector<int>::const_iterator vectorIt = positionReturnsVector.begin();

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 24));
    REQUIRE (it->second == *vectorIt);
    it++; vectorIt++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 28));
    REQUIRE (it->second == *vectorIt);
    it++; vectorIt++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 29));
    REQUIRE (it->second == *vectorIt);

    it = attributesLong.endPositionDirection();
    vectorIt = positionReturnsVector.end();
    it--;
    vectorIt--;

    REQUIRE (it->first == TimeSeriesDate (2016, Jan, 6));
    REQUIRE (it->second == *vectorIt);

  }

 SECTION ("MCPTStrategyAttributes Short PositionDirectionIterator operations");
  {
     MCPTStrategyAttributes<DecimalType>::PositionDirectionIterator it = 
      attributesShort.beginPositionDirection();

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 24));
    REQUIRE (it->second == 0);
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 28));
    REQUIRE (it->second == -1);
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 29));
    REQUIRE (it->second == -1);

    it = attributesShort.endPositionDirection();
    it--;

    REQUIRE (it->first == TimeSeriesDate (2016, Jan, 6));
    REQUIRE (it->second == -1);

  }

  SECTION ("MCPTStrategyAttributes Short PositionResultsIterator operations");
  {
     MCPTStrategyAttributes<DecimalType>::PositionReturnsIterator it = 
      attributesShort.beginPositionReturns();

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 24));

    // We are flat at this point so don't negate return
    REQUIRE (it->second == calculatePercentReturn (entry8->getCloseValue(),
						   entry7->getCloseValue()));
    //std::cout << "Short Return 1 " << it->second << std::endl;
    it++;

    // We don't negate the returns because during MCPT they are multiplied by -1
    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 28));
    REQUIRE (it->second == calculatePercentReturn (entry7->getCloseValue(),
						   entry6->getCloseValue()));
    //std::cout << "Short Return 2 " << it->second << std::endl;
    it++;

    REQUIRE (it->first == TimeSeriesDate (2015, Dec, 29));
    REQUIRE (it->second == calculatePercentReturn (entry6->getCloseValue(),
						   entry5->getCloseValue()));
    //std::cout << "Short Return 3 " << it->second << std::endl;

    it = attributesShort.endPositionReturns();
    it--;

    REQUIRE (it->first == TimeSeriesDate (2016, Jan, 6));
    REQUIRE (it->second == calculatePercentReturn (entry1->getCloseValue(),
						   entry0->getCloseValue()));
     
    //std::cout << "Short Return 4 " << it->second << std::endl;
  }
}

