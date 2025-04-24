// Copyright (C) MKC Associates, LLC - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Michael K. Collison <collison956@gmail.com>, July 2016
//
#ifndef __INSTRUMENT_POSITION_H
#define __INSTRUMENT_POSITION_H 1

#include <memory>
#include <vector>
#include <cstdint>
#include "ThrowAssert.hpp"
#include "TradingPosition.h"
#include "DecimalConstants.h"

namespace mkc_timeseries
{
  template <class Decimal> class InstrumentPosition;

  /**
   * @class InstrumentPositionState
   * @brief Abstract base for polymorphic position state transitions.
   *
   * Responsibilities:
   * - Define interface for state transitions based on order executions and strategy signals.
   * - Provide a flexible extension mechanism for long/short state logic.
   *
   * Collaboration:
   * - Implemented by LongInstrumentPositionState and ShortInstrumentPositionState.
   * - Used internally by InstrumentPosition.
   */
  template <class Decimal> class InstrumentPositionState
  {
  public:
    typedef typename std::vector<std::shared_ptr<TradingPosition<Decimal>>>::const_iterator ConstInstrumentPositionIterator;

  public:
    InstrumentPositionState()
    {}

    virtual ~InstrumentPositionState()
    {}

    InstrumentPositionState (const InstrumentPositionState<Decimal>& rhs)
    {}

    InstrumentPositionState<Decimal>& 
    operator=(const InstrumentPositionState<Decimal> &rhs)
    {
      return *this;
    }

    virtual bool isLongPosition() const = 0;
    virtual bool isShortPosition() const = 0;
    virtual bool isFlatPosition() const = 0;
    virtual uint32_t getNumPositionUnits() const = 0;

    virtual ConstInstrumentPositionIterator getInstrumentPosition (uint32_t unitNumber) const = 0;
    virtual void addBar (const OHLCTimeSeriesEntry<Decimal>& entryBar) = 0;
    virtual void addPosition(InstrumentPosition<Decimal>* iPosition,
			     std::shared_ptr<TradingPosition<Decimal>> position) = 0;
    virtual ConstInstrumentPositionIterator beginInstrumentPosition() const = 0;
    virtual ConstInstrumentPositionIterator endInstrumentPosition() const = 0;
    virtual void closeUnitPosition(InstrumentPosition<Decimal>* iPosition,
				   const boost::gregorian::date exitDate,
				   const Decimal& exitPrice,
				   uint32_t unitNumber) = 0;
    virtual void closeAllPositions(InstrumentPosition<Decimal>* iPosition,
				   const boost::gregorian::date exitDate,
				   const Decimal& exitPrice) = 0;
 
  
  };

  template <class Decimal> class FlatInstrumentPositionState : public InstrumentPositionState<Decimal>
  {
  public:
    typedef typename InstrumentPositionState<Decimal>::ConstInstrumentPositionIterator ConstInstrumentPositionIterator;
  public:
    FlatInstrumentPositionState (const FlatInstrumentPositionState<Decimal>& rhs)
      : InstrumentPositionState<Decimal>(rhs)
    {}

    FlatInstrumentPositionState<Decimal>& 
    operator=(const FlatInstrumentPositionState<Decimal> &rhs) 
    {
       if (this == &rhs)
	return *this;

      InstrumentPositionState<Decimal>::operator=(rhs);
      return *this;
    }

    ~FlatInstrumentPositionState()
    {}

    static std::shared_ptr<FlatInstrumentPositionState<Decimal>> getInstance()
    {
      return mInstance;
    }

    uint32_t getNumPositionUnits() const
    {
      return 0;
    }

    bool isLongPosition() const
    {
      return false;
    }

    bool isShortPosition() const
    {
      return false;
    }

    bool isFlatPosition() const
    {
      return true;
    }

    void addBar (const OHLCTimeSeriesEntry<Decimal>& entryBar)
    {
      throw InstrumentPositionException("FlatInstrumentPositionState: addBar - no positions available in flat state");
    }


    ConstInstrumentPositionIterator getInstrumentPosition (uint32_t unitNumber) const
    {
      throw InstrumentPositionException("FlatInstrumentPositionState: getInstrumentPosition - no positions avaialble in flat state");
    }

    void addPosition(InstrumentPosition<Decimal>* iPosition,
		     std::shared_ptr<TradingPosition<Decimal>> position);

    

    ConstInstrumentPositionIterator beginInstrumentPosition() const
    {
      throw InstrumentPositionException("FlatInstrumentPositionState: beginInstrumentPosition - no positions avaialble in flat state");
    }

    ConstInstrumentPositionIterator endInstrumentPosition() const
    {
      throw InstrumentPositionException("FlatInstrumentPositionState: endInstrumentPosition - no positions avaialble in flat state");
    }

    void closeUnitPosition(InstrumentPosition<Decimal>* iPosition,
			   const boost::gregorian::date exitDate,
			   const Decimal& exitPrice,
			   uint32_t unitNumber)
    {
      throw InstrumentPositionException("FlatInstrumentPositionState: closeUnitPosition - no positions avaialble in flat state");
    }

    void closeAllPositions(InstrumentPosition<Decimal>* iPosition,
			   const boost::gregorian::date exitDate,
			   const Decimal& exitPrice)
    {
      throw InstrumentPositionException("FlatInstrumentPositionState: closeAllPositions - no positions avaialble in flat state");
    }

  private:
    FlatInstrumentPositionState()
      : InstrumentPositionState<Decimal>()
    {}

  private:
    static std::shared_ptr<FlatInstrumentPositionState<Decimal>> mInstance;
  };

  template <class Decimal> std::shared_ptr<FlatInstrumentPositionState<Decimal>> FlatInstrumentPositionState<Decimal>::mInstance(new FlatInstrumentPositionState<Decimal>());

  //
  // class InMarketPositionState
  //

  template <class Decimal> class InMarketPositionState : public InstrumentPositionState<Decimal>
  {
  public:
    typedef typename InstrumentPositionState<Decimal>::ConstInstrumentPositionIterator ConstInstrumentPositionIterator;
  protected:
    InMarketPositionState(std::shared_ptr<TradingPosition<Decimal>> position)
      : InstrumentPositionState<Decimal>(),
	mTradingPositionUnits()
    {
      this->addPositionCommon(position);
    }

  public:
    InMarketPositionState (const InMarketPositionState<Decimal>& rhs)
      : InstrumentPositionState<Decimal>(rhs),
	mTradingPositionUnits(rhs.mTradingPositionUnits)
    {}

    virtual ~InMarketPositionState()
    {}

    InMarketPositionState<Decimal>& 
    operator=(const InMarketPositionState<Decimal> &rhs) 
    {
      if (this == &rhs)
	return *this;
      
      InstrumentPositionState<Decimal>::operator=(rhs);
      mTradingPositionUnits = rhs.mTradingPositionUnits;
    }

    uint32_t getNumPositionUnits() const
    {
      return mTradingPositionUnits.size();
    }

    ConstInstrumentPositionIterator
    getInstrumentPosition (uint32_t unitNumber) const
    {
      checkUnitNumber (unitNumber);

      // We subtract one because the stl vector starts at 0, but unit numbers start at 1
      return (this->beginInstrumentPosition() + (unitNumber - 1));
      //return mTradingPositionUnits.at (unitNumber - 1);
    }

    void addPositionCommon(std::shared_ptr<TradingPosition<Decimal>> position)
    {
      if (position->isPositionClosed())
	throw InstrumentPositionException ("InstrumentPosition: cannot add a closed position");

      mTradingPositionUnits.push_back (position);
    }

    void addBar (const OHLCTimeSeriesEntry<Decimal>& entryBar)
    {
      ConstInstrumentPositionIterator it = this->beginInstrumentPosition();

      for (; it != this->endInstrumentPosition(); it++)
	{
	  // Only add a date that is after the entry date. We
	  // already added the first bar when we created the position

	  if (entryBar.getDateValue() > (*it)->getEntryDate())
	    (*it)->addBar(entryBar);
	}
    }

    ConstInstrumentPositionIterator beginInstrumentPosition() const
    {
      return  mTradingPositionUnits.begin();
    }

    ConstInstrumentPositionIterator endInstrumentPosition() const
    {
      return  mTradingPositionUnits.end();
    }

    void closeUnitPosition(InstrumentPosition<Decimal>* iPosition,
			   const boost::gregorian::date exitDate,
			   const Decimal& exitPrice,
			   uint32_t unitNumber)
    {
      ConstInstrumentPositionIterator it = getInstrumentPosition (unitNumber);
      if (it == this->endInstrumentPosition())
	throw InstrumentPositionException ("InMarketPositionState: closeUnitPosition -cannot find unit number to close position");

      if ((*it)->isPositionOpen())
	{
	  (*it)->ClosePosition (exitDate, exitPrice);
	  mTradingPositionUnits.erase (it);
	}
      else
	throw InstrumentPositionException ("InMarketPositionState: closeUnitPosition - unit already closed");

      if (getNumPositionUnits() == 0)
	iPosition->ChangeState (FlatInstrumentPositionState<Decimal>::getInstance());
 
    }

    void closeAllPositions(InstrumentPosition<Decimal>* iPosition,
			   const boost::gregorian::date exitDate,
			   const Decimal& exitPrice)
    {
      ConstInstrumentPositionIterator it = beginInstrumentPosition();
      for (; it != this->endInstrumentPosition(); it++)
	{
	  if ((*it)->isPositionOpen())
	    (*it)->ClosePosition (exitDate, exitPrice);
	}

      mTradingPositionUnits.clear();
      iPosition->ChangeState (FlatInstrumentPositionState<Decimal>::getInstance());
    }

  private:
    void checkUnitNumber (uint32_t unitNumber) const
    {
      if (unitNumber > getNumPositionUnits())
	throw InstrumentPositionException ("InstrumentPosition:getInstrumentPosition: unitNumber " + std::to_string (unitNumber) + " is out range");

      if (unitNumber == 0)
	throw InstrumentPositionException ("InstrumentPosition:getInstrumentPosition: unitNumber - unit numbers start at one");
    }

  private:
    std::vector<std::shared_ptr<TradingPosition<Decimal>>> mTradingPositionUnits;
  };

  //
  // class LongInstrumentPositionState
  //

  template <class Decimal> class LongInstrumentPositionState : public InMarketPositionState<Decimal>
  {
  public:
    LongInstrumentPositionState(std::shared_ptr<TradingPosition<Decimal>> position)
      : InMarketPositionState<Decimal>(position)
    {}

    LongInstrumentPositionState (const LongInstrumentPositionState<Decimal>& rhs)
      : InMarketPositionState<Decimal>(rhs)
    {}

    ~ LongInstrumentPositionState()
    {}

    LongInstrumentPositionState<Decimal>& 
    operator=(const LongInstrumentPositionState<Decimal> &rhs) 
    {
      if (this == &rhs)
	return *this;
      
      InMarketPositionState<Decimal>::operator=(rhs);
      return *this;
    }

    void addPosition(InstrumentPosition<Decimal>* iPosition,
		     std::shared_ptr<TradingPosition<Decimal>> position)
    {
      if (position->isLongPosition())
	this->addPositionCommon(position);
      else
	throw InstrumentPositionException ("InstrumentPosition: cannot add short position unit to long position");
    }

    bool isLongPosition() const
    {
      return true;
    }

    bool isShortPosition() const
    {
      return false;
    }

    bool isFlatPosition() const
    {
      return false;
    }
  };

  template <class Decimal> class ShortInstrumentPositionState : public InMarketPositionState<Decimal>
  {
  public:
    ShortInstrumentPositionState(std::shared_ptr<TradingPosition<Decimal>> position)
      : InMarketPositionState<Decimal>(position)
    {}

    ShortInstrumentPositionState (const ShortInstrumentPositionState<Decimal>& rhs)
      : InMarketPositionState<Decimal>(rhs)
    {}

    ShortInstrumentPositionState<Decimal>& 
    operator=(const ShortInstrumentPositionState<Decimal> &rhs) 
    {
      if (this == &rhs)
	return *this;
      
      InMarketPositionState<Decimal>::operator=(rhs);
      return *this;
    }

    void addPosition(InstrumentPosition<Decimal>* iPosition,
		     std::shared_ptr<TradingPosition<Decimal>> position)
    {
      if (position->isShortPosition())
	this->addPositionCommon(position);
      else
	throw InstrumentPositionException ("InstrumentPosition: cannot add long position unit to short position");
    }
    bool isLongPosition() const
    {
      return false;
    }

    bool isShortPosition() const
    {
      return true;
    }

    bool isFlatPosition() const
    {
      return false;
    }
  };

  /**
   * @class InstrumentPosition
   * @brief Encapsulates position state and logic for a single trading symbol.
   *
   * Responsibilities:
   * - Store and manage a list of active positions for a specific symbol.
   * - Delegate position lifecycle logic to an internal state class (e.g., LongInstrumentPositionState).
   * - Notify StrategyBroker and other observers of state changes.
   *
   * Collaboration:
   * - Owned by InstrumentPositionManager.
   * - Works with TradingPosition state objects to reflect live positions.
   *
   * An InstrumentPosition is made up of one
   * or more TradingPosition objects
   *
   * Each TradingPosition object is assigned a
   * unit number (starting at 1)
   *
   * This will allow closing a single unit at
   * a time or closing all units at the same
   * time.
   */

  template <class Decimal> class InstrumentPosition
  {
  public:
    typedef typename InstrumentPositionState<Decimal>::ConstInstrumentPositionIterator ConstInstrumentPositionIterator;

  public:
    InstrumentPosition (const std::string& instrumentSymbol)
      : mInstrumentSymbol(instrumentSymbol),
	mInstrumentPositionState(FlatInstrumentPositionState<Decimal>::getInstance())
    {}

    InstrumentPosition (const InstrumentPosition<Decimal>& rhs)
      : mInstrumentSymbol(rhs.mInstrumentSymbol),
	mInstrumentPositionState(rhs.mInstrumentPositionState)
    {}

    InstrumentPosition<Decimal>& 
    operator=(const InstrumentPosition<Decimal> &rhs)
    {
      if (this == &rhs)
	return *this;

      mInstrumentSymbol = rhs.mInstrumentSymbol;
      mInstrumentPositionState = rhs.mInstrumentPositionState;

      return *this;
    }

    ~InstrumentPosition()
    {}

    const std::string& getInstrumentSymbol() const
    {
      return mInstrumentSymbol;
    }

    bool isLongPosition() const
    {
      return mInstrumentPositionState->isLongPosition();
    }

    bool isShortPosition() const
    {
      return mInstrumentPositionState->isShortPosition();
    }

    bool isFlatPosition() const
    {
      return mInstrumentPositionState->isFlatPosition();
    }

    uint32_t getNumPositionUnits() const
    {
      return mInstrumentPositionState->getNumPositionUnits();
    }

    ConstInstrumentPositionIterator
    getInstrumentPosition (uint32_t unitNumber) const
    {
      return mInstrumentPositionState->getInstrumentPosition(unitNumber);
    }

    const Decimal& getFillPrice() const
    {
      return getFillPrice(1);
    }

    const Decimal& getFillPrice(uint32_t unitNumber) const
    {
      ConstInstrumentPositionIterator pos = getInstrumentPosition(unitNumber);
      return (*pos)->getEntryPrice();
    }

    void setRMultipleStop (const Decimal& riskStop) const
    {
      this->setRMultipleStop (riskStop, 1);
    }

    void setRMultipleStop (const Decimal& riskStop, uint32_t unitNumber) const
    {
      ConstInstrumentPositionIterator pos = getInstrumentPosition(unitNumber);
      (*pos)->setRMultipleStop (riskStop);
    }

    void addBar (const OHLCTimeSeriesEntry<Decimal>& entryBar)
    {
      mInstrumentPositionState->addBar(entryBar);
    }

    void addPosition(std::shared_ptr<TradingPosition<Decimal>> position)
    {
      if (position->isPositionClosed())
	throw InstrumentPositionException ("InstrumentPosition: cannot add a closed position");

      if (getInstrumentSymbol() != position->getTradingSymbol())
	throw InstrumentPositionException ("InstrumentPosition: cannot add position with different symbols");

      mInstrumentPositionState->addPosition (this, position);
    }

    ConstInstrumentPositionIterator beginInstrumentPosition() const
    {
      return mInstrumentPositionState->beginInstrumentPosition();
    }

    ConstInstrumentPositionIterator endInstrumentPosition() const
    {
      return  mInstrumentPositionState->endInstrumentPosition();
    }

    TradingVolume getVolumeInAllUnits() const
    {
      if (isFlatPosition() == false)
	{
	  volume_t totalVolume = 0;
	  ConstInstrumentPositionIterator it = beginInstrumentPosition();

	  for (; it != endInstrumentPosition(); it++)
	    totalVolume += (*it)->getTradingUnits().getTradingVolume();

	  if (totalVolume > 0)
	    {
	      it = beginInstrumentPosition();
	      TradingVolume::VolumeUnit volUnit = 
		(*it)->getTradingUnits().getVolumeUnits();

	      return TradingVolume(totalVolume, volUnit);
	    }
	  // Need to throw an exception here
	  else
	    throw InstrumentPositionException ("InstrumentPosition::getVolumeInAllUnits - Cannot get volume when position is flat");
	}

      throw InstrumentPositionException ("InstrumentPosition::getVolumeInAllUnits - Cannot get volume when position is flat");
    }

    void closeUnitPosition(const boost::gregorian::date exitDate,
			   const Decimal& exitPrice,
			   uint32_t unitNumber)
    {
      mInstrumentPositionState->closeUnitPosition(this, exitDate, exitPrice, unitNumber);
    }

    void closeAllPositions(const boost::gregorian::date exitDate,
			   const Decimal& exitPrice)
    {
      mInstrumentPositionState->closeAllPositions(this, exitDate, exitPrice);
    }

  private:
    void ChangeState (std::shared_ptr<InstrumentPositionState<Decimal>> newState)
    {
      mInstrumentPositionState = newState;
    }

    friend class FlatInstrumentPositionState<Decimal>;
    friend class LongInstrumentPositionState<Decimal>;
    friend class ShortInstrumentPositionState<Decimal>;
    friend class InMarketPositionState<Decimal>;

  private:
    std::string mInstrumentSymbol;
    std::shared_ptr<InstrumentPositionState<Decimal>> mInstrumentPositionState;
  };

  template <class Decimal>
  inline void FlatInstrumentPositionState<Decimal>::addPosition(InstrumentPosition<Decimal>* iPosition,
							     std::shared_ptr<TradingPosition<Decimal>> position)
    {
      if (position->isLongPosition())
	iPosition->ChangeState (std::make_shared<LongInstrumentPositionState<Decimal>>(position));
      else if (position->isShortPosition())
	iPosition->ChangeState (std::make_shared<ShortInstrumentPositionState<Decimal>>(position));
      else
	throw InstrumentPositionException ("FlatInstrumentPositionState<Decimal>::addPosition: position is neither long or short");
      
    }
}

#endif
