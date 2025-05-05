// Copyright (C) MKC Associates, LLC - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Michael K. Collison <collison956@gmail.com>, July 2016
//

#ifndef __CSVWRITER_H
#define __CSVWRITER_H 1

#include "TimeSeries.h"
#include <boost/date_time.hpp>
#include <fstream>

namespace mkc_timeseries
{
  using boost::posix_time::ptime;
  //
  // class PalTimeSeriesCsvWriter
  //
  // Write a OHLCTimeSeries out in a format that
  // can be read by PriceActionLab

  template <class Decimal>
  class PalTimeSeriesCsvWriter
  {
  public:
    PalTimeSeriesCsvWriter (const std::string& fileName, 
			    const OHLCTimeSeries<Decimal>& series)
      : mCsvFile (fileName),
	mTimeSeries (series)
    {
    }

    PalTimeSeriesCsvWriter (const PalTimeSeriesCsvWriter& rhs)
      : mCsvFile (rhs.mCsvFile),
	mTimeSeries (rhs.mTimeSeries)
    {}

    PalTimeSeriesCsvWriter& 
    operator=(const PalTimeSeriesCsvWriter &rhs)
    {
      if (this == &rhs)
	return *this;

      mCsvFile = rhs.mCsvFile;
      mTimeSeries = rhs.mTimeSeries;

      return *this;
    }

    ~PalTimeSeriesCsvWriter()
    {}

    void writeFile()
    {
      for (auto it = mTimeSeries.beginSortedAccess();
           it != mTimeSeries.endSortedAccess();
           ++it)
      {
        // *it is an OHLCTimeSeriesEntry<Decimal>
        const auto& entry = *it;
        ptime       dateTime = entry.getDateTime();

        mCsvFile << boost::gregorian::to_iso_string(dateTime.date()) << ","
                 << entry.getOpenValue()  << ","
                 << entry.getHighValue()  << ","
                 << entry.getLowValue()   << ","
                 << entry.getCloseValue() << std::endl;
      }
    }    

  private:
    std::ofstream mCsvFile;
    OHLCTimeSeries<Decimal> mTimeSeries;
  };


  //
  
  //

  template <class Decimal>
  class PalVolumeForCloseCsvWriter
  {
  public:
    PalVolumeForCloseCsvWriter (const std::string& fileName, 
			    const OHLCTimeSeries<Decimal>& series)
      : mCsvFile (fileName),
	mTimeSeries (series)
    {
    }

    PalVolumeForCloseCsvWriter (const PalVolumeForCloseCsvWriter& rhs)
      : mCsvFile (rhs.mCsvFile),
	mTimeSeries (rhs.mTimeSeries)
    {}

    PalVolumeForCloseCsvWriter& 
    operator=(const PalVolumeForCloseCsvWriter &rhs)
    {
      if (this == &rhs)
	return *this;

      mCsvFile = rhs.mCsvFile;
      mTimeSeries = rhs.mTimeSeries;

      return *this;
    }

    ~PalVolumeForCloseCsvWriter()
    {}

    /**
     * @brief Dump the time series to CSV: Date,Open,High,Low,Volume
     */
    void writeFile()
    {
      for (auto it = mTimeSeries.beginSortedAccess();
           it != mTimeSeries.endSortedAccess();
           ++it)
      {
        // *it is an OHLCTimeSeriesEntry<Decimal>
        const auto& entry = *it;
        ptime       dateTime = entry.getDateTime();

        mCsvFile << boost::gregorian::to_iso_string(dateTime.date()) << ","
                 << entry.getOpenValue()    << ","
                 << entry.getHighValue()    << ","
                 << entry.getLowValue()     << ","
                 << entry.getVolumeValue()  << std::endl;
      }
    }    

  private:
    std::ofstream mCsvFile;
    OHLCTimeSeries<Decimal> mTimeSeries;
  };
}

#endif


