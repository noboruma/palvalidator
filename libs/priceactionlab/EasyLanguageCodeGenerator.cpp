//#include "PalAst.h"
//#include "PalCodeGenVisitor.h"
//#include <iostream>

//extern bool firstSubExpressionVisited;

/////////////////////////////////////////
///// class EasyLanguageCodeGenVisitor
////////////////////////////////////////

//EasyLanguageCodeGenVisitor::EasyLanguageCodeGenVisitor(PriceActionLabSystem *system,
//						       const std::string& outputFileName,
//						       const StopTargetDetail& dev1Detail,
//						       const StopTargetDetail& dev2Detail)

//  : PalCodeGenVisitor(),
//    mTradingSystemPatterns(system),
//    mEasyLanguageFileName(outputFileName),
//    mDev1Detail (dev1Detail),
//    mDev2Detail (dev2Detail)
//{}


//EasyLanguageCodeGenVisitor::~EasyLanguageCodeGenVisitor()
//{}

//void EasyLanguageCodeGenVisitor::genCodeForCommonVariables()
//{
//  std::ofstream *outFile = getOutputFileStream();

//  *outFile << "vars: shortStop (0), longStop (0), stopPercent (0), tradePercentChange(0.0);" << std::endl;
//  *outFile << "vars: longEntryFound (false), shortEntryFound (false), noNextDayOrders(false), StrategyMAE(TBD);" << std::endl;
//  *outFile << "vars: oscVChartLow(0.0), oscVChartHigh(0.0);" << std::endl;
//  *outFile << "vars: highestPosChange(0.0), lowestNegChange(0.0);" << std::endl;
//  *outFile << "vars: lowVolatility(false), highVolatility(false), vHighVolatility(false), breakEvenStopSet(false);" << std::endl;
//  *outFile << "vars: breakEvenThreshold(0.0), dvbValue(0.0);" << std::endl;
//  *outFile << "vars: rankedVol(0.0), MinHoldPeriod(0.0), MaxHoldPeriod(0.0);" << std::endl;
//  *outFile << "vars: profitTargetPercent(0.0), TargPrL(0.0), TargPrS(0.0), dailyChange(0.0);" << std::endl;
//  *outFile << "vars: stop1Str(\"\"), stop2Str (\"\"), target1Str(\"\"), target2Str(\"\"), target3Str(\"\");" << std::endl;
//  *outFile << "vars: buyStr(\"\"), sellStr(\"\"), stopStr(\"\"), targetStr(\"\");" << std::endl;
//}

//void EasyLanguageCodeGenVisitor::genCodeToInitVolatility(bool shortSide)
//{
//  std::ofstream *outFile = getOutputFileStream();

//  if (shortSide == false)
//    {
//      *outFile << "\t\t\tIf (lowVolatility) then" << std::endl;
//      *outFile << "\t\t\tbegin" << std::endl;
//      *outFile << "\t\t\t\tlowVolatilityEntry = true;" << std::endl;
//      *outFile << "\t\t\t\thighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tvHighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tStrategyMAE = MAE2;" << std::endl;
//      *outFile << "\t\t\t\tMinHoldPeriod = MinHold1;" << std::endl;
//      *outFile << "\t\t\t\tMaxHoldPeriod = MaxHold1;" << std::endl;
//      *outFile << "\t\t\t\tbreakEvenThreshold = lowVolBEThreshold;" << std::endl;
//      *outFile << "\t\t\tend" << std::endl;
//      *outFile << "\t\t\telse if (highVolatility) then" << std::endl;
//      *outFile << "\t\t\tbegin" << std::endl;
//      *outFile << "\t\t\t\tlowVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\thighVolatilityEntry = true;" << std::endl;
//      *outFile << "\t\t\t\tvHighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tStrategyMAE = MAE2;" << std::endl;
//      *outFile << "\t\t\t\tMinHoldPeriod = MinHold2;" << std::endl;
//      *outFile << "\t\t\t\tMaxHoldPeriod = MaxHold2;" << std::endl;
//      *outFile << "\t\t\t\tbreakEvenThreshold = highVolBEThreshold;" << std::endl;
//      *outFile << "\t\t\tend" << std::endl;
//      *outFile << "\t\t\telse" << std::endl;
//      *outFile << "\t\t\tbegin" << std::endl;
//      *outFile << "\t\t\t\tlowVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\thighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tvHighVolatilityEntry = true;" << std::endl;
//      *outFile << "\t\t\t\tMinHoldPeriod = MinHold3;" << std::endl;
//      *outFile << "\t\t\t\tMaxHoldPeriod = MaxHold3;" << std::endl;
//      *outFile << "\t\t\t\tStrategyMAE = MAE3;" << std::endl;
//      *outFile << "\t\t\t\tbreakEvenThreshold = vHighVolBEThreshold;" << std::endl;
//      *outFile << "\t\t\tend;" << std::endl;
//    }
//  else
//    {
//     *outFile << "\t\t\tIf (lowVolatility) then" << std::endl;
//      *outFile << "\t\t\tbegin" << std::endl;
//      *outFile << "\t\t\t\tlowVolatilityEntry = true;" << std::endl;
//      *outFile << "\t\t\t\thighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tvHighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tStrategyMAE = MAE3;" << std::endl;
//      *outFile << "\t\t\t\tMinHoldPeriod = MinHold1;" << std::endl;
//      *outFile << "\t\t\t\tMaxHoldPeriod = MaxHold1;" << std::endl;
//      *outFile << "\t\t\t\tbreakEvenThreshold = -lowVolBEThreshold;" << std::endl;
//      *outFile << "\t\t\tend" << std::endl;
//      *outFile << "\t\t\telse if (highVolatility) then" << std::endl;
//      *outFile << "\t\t\tbegin" << std::endl;
//      *outFile << "\t\t\t\tlowVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\thighVolatilityEntry = true;" << std::endl;
//      *outFile << "\t\t\t\tvHighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tStrategyMAE = MAE2;" << std::endl;
//      *outFile << "\t\t\t\tMinHoldPeriod = MinHold2;" << std::endl;
//      *outFile << "\t\t\t\tMaxHoldPeriod = MaxHold2;" << std::endl;
//      *outFile << "\t\t\t\tbreakEvenThreshold = -highVolBEThreshold;" << std::endl;
//      *outFile << "\t\t\tend" << std::endl;
//      *outFile << "\t\t\telse" << std::endl;
//      *outFile << "\t\t\tbegin" << std::endl;
//      *outFile << "\t\t\t\tlowVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\thighVolatilityEntry = false;" << std::endl;
//      *outFile << "\t\t\t\tvHighVolatilityEntry = true;" << std::endl;
//      *outFile << "\t\t\t\tStrategyMAE = MAE3;" << std::endl;
//      *outFile << "\t\t\t\tMinHoldPeriod = MinHold3;" << std::endl;
//      *outFile << "\t\t\t\tMaxHoldPeriod = MaxHold3;" << std::endl;
//      *outFile << "\t\t\t\tbreakEvenThreshold = -vHighVolBEThreshold;" << std::endl;
//      *outFile << "\t\t\tend;" << std::endl;
//    }
//}

//void EasyLanguageCodeGenVisitor::genCodeForCommonEntry()
//{
//  std::ofstream *outFile = getOutputFileStream();

// *outFile <<  std::endl;
// *outFile << "\t\tif (longEntryFound = true) and (shortEntryFound = false) then" << std::endl;
// *outFile << "\t\tbegin" << std::endl;
// *outFile << "\t\t\tbreakEvenStopSet = false;"  << std::endl;
// *outFile << "\t\t\tCommentary(\"Manual long stop = open of next bar - \", stopStr, NewLine);" << std::endl;;
// *outFile << "\t\t\tCommentary(\"Manual long profit target = open of next bar + \", targetStr, NewLine);" << std::endl;
// *outFile << "\t\t\thighestPosChange = 0;"  << std::endl;
// *outFile << "\t\t\tbreakEvenThreshold = (profitTargetPercent * 100.0) * 0.5;" << std::endl;

// *outFile << "\t\t\tbuy next bar at market;"  << std::endl;
// *outFile << "\t\tend;"  << std::endl;
// *outFile << "\t\tif (longEntryFound = false) and (shortEntryFound = true) then" << std::endl;
// *outFile << "\t\tbegin"  << std::endl;
// *outFile << "\t\t\tbreakEvenStopSet = false;"  << std::endl;
// *outFile << "\t\t\tCommentary(\"Manual short stop = open of next bar + \", stopStr, NewLine);" << std::endl;
// *outFile << "\t\t\tCommentary(\"Manual short profit target = open of next bar - \", targetStr, NewLine);" << std::endl;

// *outFile << "\t\t\tlowestNegChange = 0;"  << std::endl;
// *outFile << "\t\t\tbreakEvenThreshold = -((profitTargetPercent * 100.0) * 0.5);" << std::endl;
// *outFile << "\t\t\tsell short next bar at market;"  << std::endl;
// *outFile << "\t\tend;"  << std::endl;

// *outFile <<  std::endl;
// *outFile << "\tend  // end for if not LastPositionActive"  << std::endl;
//}

//void EasyLanguageCodeGenVisitor::genCommonCodeForLongExitPrologue()
//{
//  std::ofstream *outFile = getOutputFileStream();

//  *outFile << "\t\t\tnoNextDayOrders = false;" << std::endl << std::endl;
//  *outFile << "\t\t\tif dailyChange <= -StrategyMAE then" << std::endl;
//  *outFile << "\t\t\tbegin" << std::endl;
//  *outFile << "\t\t\t\tnoNextDayOrders = true;" << std::endl;
//  *outFile << "\t\t\t\tSell (\"Large neg. chng.\") next bar at Market;" << std::endl;
//  *outFile << "\t\t\tend;" << std::endl << std::endl;

//  *outFile << "\t\t\tIf (Barssinceentry > MaxHoldPeriod) and (noNextDayOrders = false) then" << std::endl;
//  *outFile << "\t\t\tbegin" << std::endl;
//  *outFile << "\t\t\t\tnoNextDayOrders = true;" << std::endl;
//  *outFile << "\t\t\t\tSell (\"L Max hold time\") next bar at Market;" << std::endl;
//  *outFile << "\t\t\tend;" << std::endl;
//}

//void EasyLanguageCodeGenVisitor::genCommonCodeForShortExitPrologue()
//{
// std::ofstream *outFile = getOutputFileStream();

//*outFile << "\t\t\tnoNextDayOrders = false;" << std::endl << std::endl;
// *outFile << "\t\t\tif dailyChange >= StrategyMAE then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tnoNextDayOrders = true;" << std::endl;
// *outFile << "\t\t\t\tBuy to Cover (\"Large +chng.\") next bar at Market;" << std::endl;
// *outFile << "\t\t\tend;" << std::endl << std::endl;


//  *outFile << "\t\t\tIf (Barssinceentry > MaxHoldPeriod) and (noNextDayOrders = false) then" << std::endl;
//  *outFile << "\t\t\tbegin" << std::endl;
//  *outFile << "\t\t\t\tnoNextDayOrders = true;" << std::endl;
//  *outFile << "\t\t\t\tBuy to Cover (\"S Max hold time\") next bar at Market;" << std::endl;
//  *outFile << "\t\t\tend;" << std::endl;
//}

//void EasyLanguageCodeGenVisitor::genCodeForCommonVariableInit()
//{
//  std::ofstream *outFile = getOutputFileStream();

//  *outFile << "\t\trankedVol = RankedSimonsHV(10, 252) * 100.0;" << std::endl;
//  *outFile << "\t\tlowVolatility = (rankedVol < 50);" << std::endl;
//  *outFile << "\t\thighVolatility = (rankedVol >= 50) and (rankedVol <= 80);" << std::endl;
//  *outFile << "\t\tvHighVolatility = (rankedVol >= 80);" << std::endl << std::endl;
//  *outFile << "\t\tdailyChange = RateOfChange (Close, 1);" << std::endl;
//  *outFile << "\t\tosc = CRSI2(3,2,100);" << std::endl;

//}

//void
//EasyLanguageCodeGenVisitor::generateCode()
//{
//  PriceActionLabSystem::ConstSortedPatternIterator it;
//  PALPatternPtr p;

//  genCodeForCommonVariables();
//  genCodeForVariablesInEntryScript();

//  genCodeForCommonVariableInit();
//  genCodeToInitializeVariables();

//  mEasyLanguageFileName << "\tif MarketPosition = 0 then" << std::endl;
//  mEasyLanguageFileName << "\tbegin" << std::endl;
//  mEasyLanguageFileName << "\t\tlongEntryFound = false;" << std::endl;
//  mEasyLanguageFileName << "\t\tshortEntryFound = false;" << std::endl << std::endl;

//  unsigned int numLongPatterns = 0;

//  for (it = mTradingSystemPatterns->patternLongsBegin(); it != mTradingSystemPatterns->patternLongsEnd(); it++)
//    {
//      p = it->second;
//      p->accept (*this);
//      numLongPatterns++;
//    }

//  unsigned int numShortPatterns = 0;
//  for (it = mTradingSystemPatterns->patternShortsBegin();
//       it != mTradingSystemPatterns->patternShortsEnd(); it++)
//    {
//      p = it->second;
//      p->accept (*this);
//      numShortPatterns++;
//     }

//  genCodeForCommonEntry();
//  genCodeForEntryExit();

//  std::cout << "Num long patterns = " << numLongPatterns << std::endl;
//  std::cout << "Num short patterns = " << numShortPatterns << std::endl;
//}
	

//std::ofstream *
//EasyLanguageCodeGenVisitor::getOutputFileStream()
//{
//  return &mEasyLanguageFileName;
//}

//void
//EasyLanguageCodeGenVisitor::visit (PriceBarOpen *bar)
//{
//  mEasyLanguageFileName << "open[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (PriceBarHigh *bar)
//{
//  mEasyLanguageFileName << "high[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (PriceBarLow *bar)
//{
//  mEasyLanguageFileName << "low[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (PriceBarClose *bar)
//{
//  mEasyLanguageFileName << "close[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (VolumeBarReference *bar)
//{
//  mEasyLanguageFileName << "volume[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (Roc1BarReference *bar)
//{
//  mEasyLanguageFileName << "RateOfChange(Close, 1)[" << bar->getBarOffset() << "]";
//}

//void EasyLanguageCodeGenVisitor::visit (IBS1BarReference *bar)
//{
//  mEasyLanguageFileName << "IBS(1)[" << bar->getBarOffset() << "]";
//}

//void EasyLanguageCodeGenVisitor::visit (IBS2BarReference *bar)
//{
//  mEasyLanguageFileName << "IBS(2)[" << bar->getBarOffset() << "]";
//}

//void EasyLanguageCodeGenVisitor::visit (IBS3BarReference *bar)
//{
//  mEasyLanguageFileName << "IBS(3)[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (MeanderBarReference *bar)
//{
//  mEasyLanguageFileName << "meanderVar[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (VChartLowBarReference *bar)
//{
//  mEasyLanguageFileName << "vchartLowVar[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (VChartHighBarReference *bar)
//{
//  mEasyLanguageFileName << "vchartHighVar[" << bar->getBarOffset() << "]";
//}

//void
//EasyLanguageCodeGenVisitor::visit (GreaterThanExpr *expr)
//{
//  if (firstSubExpressionVisited == false)
//    mEasyLanguageFileName << "\t\t\t(";
//  else
//    {
//      mEasyLanguageFileName << "(";
//      firstSubExpressionVisited = false;
//    }

//  expr->getLHS()->accept (*this);
//  mEasyLanguageFileName << " > ";
//  expr->getRHS()->accept (*this);
//  mEasyLanguageFileName << ")";
//}

//void
//EasyLanguageCodeGenVisitor::visit (AndExpr *expr)
//{
//  expr->getLHS()->accept (*this);
//  mEasyLanguageFileName << " and " << std::endl;
//  expr->getRHS()->accept (*this);
//}

//void
//EasyLanguageCodeGenVisitor::visit (PatternDescription *desc)
//{
//  mEasyLanguageFileName << "\t\t" << "//FILE:" << desc->getFileName() << "  Index: " << desc->getpatternIndex()
//			 << "  Index DATE: " << desc->getIndexDate() << "  PL: " << *(desc->getPercentLong())
//                         << "%  PS: " << *(desc->getPercentShort()) << "%  Trades: " << desc->numTrades()
//                         << "  CL: " << desc->numConsecutiveLosses() << " }" << std::endl;
    

//}

//void EasyLanguageCodeGenVisitor::visit (LongMarketEntryOnOpen *entryStatement)
//{
//  mEasyLanguageFileName << "\t\t\tlongEntryFound = true;" << std::endl;
//}

//void EasyLanguageCodeGenVisitor:: visit (ShortMarketEntryOnOpen *entryStatement)
//{
//  mEasyLanguageFileName << "\t\t\tshortEntryFound = true;" << std::endl;
//}

//bool EasyLanguageCodeGenVisitor::isHighRewardToRiskRatioPattern (PriceActionLabPattern *pattern)
//{
//  decimal7 threshold(1.25);

//  decimal7 target2 = *(pattern->getProfitTarget()->getProfitTarget());
//  decimal7 stop2 = *(pattern->getStopLoss()->getStopLoss());

//  decimal7 ratio = target2 / stop2;
//  if (ratio >= threshold)
//    return true;
//  else
//    return false;
//}

//bool EasyLanguageCodeGenVisitor::isDev1Pattern(PriceActionLabPattern *pattern)
//{
//  return ((pattern->getStopLossAsDecimal() == mDev1Detail.getStopLoss()) &&
//	  (pattern->getProfitTargetAsDecimal() == mDev1Detail.getProfitTarget()));
//}

//bool EasyLanguageCodeGenVisitor::isDev2Pattern(PriceActionLabPattern *pattern)
//{
//  return ((pattern->getStopLossAsDecimal() == mDev2Detail.getStopLoss()) &&
//	  (pattern->getProfitTargetAsDecimal() == mDev2Detail.getProfitTarget()));

//}


//void EasyLanguageCodeGenVisitor::visit (PriceActionLabPattern *pattern)
//{
//  pattern->getPatternDescription()->accept (*this);
//  mEasyLanguageFileName << std::endl;
  
//  if (pattern->isLongPattern())
//    {
//      mEasyLanguageFileName << "\t\tif (longEntryFound = false) and ";
      
		  
//    }
//  else
//    {
//      mEasyLanguageFileName << "\t\tif (shortEntryFound = false) and ";
//    }

//  if (isDev1Pattern (pattern))
//    mEasyLanguageFileName << "(tradeSys1 = true) and ";
//  else if (isDev2Pattern (pattern))
//    mEasyLanguageFileName << "(tradeSys2 = true) and ";
  
//  if (pattern->hasVolatilityAttribute())
//    {
//      if (pattern->isLowVolatilityPattern())
//	mEasyLanguageFileName << "lowVolatility and ";
//      else if (pattern->isHighVolatilityPattern())
//	mEasyLanguageFileName << "highVolatility and ";
//      else if (pattern->isVeryHighVolatilityPattern())
//	mEasyLanguageFileName << "vHighVolatility and ";
//    }

//  if (pattern->hasPortfolioAttribute())
//    {
//      if (pattern->isFilteredLongPattern())
//	mEasyLanguageFileName << "tradeLongSide and ";
//      else if (pattern->isFilteredShortPattern())
//	mEasyLanguageFileName << "tradeShortSide and ";
//    }

//  if (isHighRewardToRiskRatioPattern (pattern))
//    {
//      mEasyLanguageFileName << "(TradeHighRewardToRiskPatterns = true) and " << std::endl;
//      firstSubExpressionVisited = false;
//    }
//  else
//    firstSubExpressionVisited = true;
  
//  pattern->getPatternExpression()->accept (*this);
//  mEasyLanguageFileName << " Then" << std::endl << std::endl;
//  mEasyLanguageFileName << "\t\tbegin" << std::endl;
//  pattern->getStopLoss()->accept (*this);
//  pattern->getProfitTarget()->accept (*this);
//  pattern->getMarketEntry()->accept (*this);

//    if (isDev1Pattern (pattern))
//    {
//      mEasyLanguageFileName << "\t\t\tMinHoldPeriod = MinDev1HoldPeriod;" << std::endl;
//      mEasyLanguageFileName << "\t\t\tMaxHoldPeriod = MaxDev1HoldPeriod;" << std::endl;
//    }
//  else if (isDev2Pattern (pattern))
//    {
//      mEasyLanguageFileName << "\t\t\tMinHoldPeriod = MinDev2HoldPeriod;" << std::endl;
//      mEasyLanguageFileName << "\t\t\tMaxHoldPeriod = MaxDev2HoldPeriod;" << std::endl;

//    }

//  mEasyLanguageFileName << "\t\tend;" << std::endl;
//}

//////////////////////////////////////////////////////
///// class EasyLanguageRADCodeGenVisitor
//////////////////////////////////////////////////////

//EasyLanguageRADCodeGenVisitor::EasyLanguageRADCodeGenVisitor (PriceActionLabSystem *system,
//							      const std::string& outputFileName,
//							      const StopTargetDetail& dev1Detail,
//							      const StopTargetDetail& dev2Detail)

//  : EasyLanguageCodeGenVisitor (system, outputFileName, dev1Detail, dev2Detail)
//{}

//EasyLanguageRADCodeGenVisitor::~EasyLanguageRADCodeGenVisitor()
//{}

//void EasyLanguageRADCodeGenVisitor::genCodeToInitializeVariables()
//{


 
//}

//void EasyLanguageRADCodeGenVisitor::genCodeForEntryExit()
//{
//std::ofstream *outFile = getOutputFileStream();

// *outFile <<  std::endl;
// *outFile << "\t\tif (longEntryFound = true) and (shortEntryFound = false) then" << std::endl;
// *outFile << "\t\tbegin" << std::endl;
// *outFile << "\t\t\tbuy next bar at market;"  << std::endl;
// *outFile << "\t\tend;"  << std::endl;
// *outFile << "\t\tif (longEntryFound = false) and (shortEntryFound = true) then" << std::endl;
// *outFile << "\t\tbegin"  << std::endl;
// *outFile << "\t\t\tsell short next bar at market;"  << std::endl;
// *outFile << "\t\tend;"  << std::endl;

// *outFile <<  std::endl;
// *outFile << "\tend"  << std::endl;
// *outFile << "\telse" << std::endl;
// *outFile << "\tbegin" << std::endl;
// *outFile << "\t\tif marketposition = 1 then begin" << std::endl;
// *outFile << "\t\t\tif BarsSinceEntry = 0 then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tlongStop = Round2Fraction (EntryPrice * stopPercent);" << std::endl;
// *outFile << "\t\t\t\tTargPrL = Round2Fraction (EntryPrice * profitTargetPercent);" << std::endl;
// *outFile << "\t\t\tend;" << std::endl << std::endl;

// *outFile << "\t\t\tsell next bar at TargPrL limit;" << std::endl;
// *outFile << "\t\t\tsell next bar at longStop stop;" << std::endl;
// *outFile << "\t\tend;" << std::endl;

// *outFile << "\t\tif marketposition = -1 then begin" << std::endl;
//*outFile << "\t\t\tif BarsSinceEntry = 0 then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tshortStop = Round2Fraction (EntryPrice * stopPercent);" << std::endl;
// *outFile << "\t\t\t\tTargPrS = Round2Fraction (EntryPrice * profitTargetPercent);" << std::endl;
//*outFile << "\t\t\tend;" << std::endl << std::endl;
// *outFile << "\t\t\tbuy to cover next bar at TargPrS limit;" << std::endl;
// *outFile << "\t\t\tbuy to cover next bar at shortStop stop;" << std::endl;
// *outFile << "\t\tend;" << std::endl;

// *outFile << "\tend;" << std::endl << std::endl;
//}

//void EasyLanguageRADCodeGenVisitor::genCodeForVariablesInEntryScript()
//{
//  //  std::ofstream *outFile = getOutputFileStream();



//}

//void
//EasyLanguageRADCodeGenVisitor::visit (LongSideStopLossInPercent *stopLoss)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *stop = stopLoss->getStopLoss();

//  *outFile << "\t\t\tstopPercent = (1.0 - (" << *stop << "/100));" << std::endl;
//  *outFile << "\t\t\tlongStop = (Close * stopPercent);" << std::endl;
//  *outFile << "\t\t\tstopStr = \"" << *stop << "%\";" << std::endl;
//}

//void EasyLanguageRADCodeGenVisitor::visit (LongSideProfitTargetInPercent *profitTarget)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *target = profitTarget->getProfitTarget();

//  *outFile << "\t\t\tprofitTargetPercent = (1.0 + (" << *target << "/100));" << std::endl;
//  *outFile << "\t\t\ttargetStr = \"" << *target << "%\";" << std::endl;
//}

//void EasyLanguageRADCodeGenVisitor::visit (ShortSideProfitTargetInPercent *profitTarget)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *target = profitTarget->getProfitTarget();

//  *outFile << "\t\t\tprofitTargetPercent = (1.0 - (" << *target << "/100));" << std::endl;
//  *outFile << "\t\t\ttargetStr = \"" << *target << "%\";" << std::endl;
//}

//void
//EasyLanguageRADCodeGenVisitor::visit (ShortSideStopLossInPercent *stopLoss)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *stop = stopLoss->getStopLoss();

//  *outFile << "\t\t\tstopPercent = (1.0 + (" << *stop << "/100));" << std::endl;
//  *outFile << "\t\t\tshortStop = (Close * stopPercent);" << std::endl;
//  *outFile << "\t\t\tstopStr = \"" << *stop << "%\";" << std::endl;
//}

//////////////////////////////////////////////////////
///// class EasyLanguagePointAdjustedCodeGenVisitor
//////////////////////////////////////////////////////

//EasyLanguagePointAdjustedCodeGenVisitor
//::EasyLanguagePointAdjustedCodeGenVisitor (PriceActionLabSystem *system,
//					   const std::string& bloxOutfileFileName,
//					   const StopTargetDetail& dev1Detail,
//					   const StopTargetDetail& dev2Detail)

//  : EasyLanguageCodeGenVisitor (system, bloxOutfileFileName, dev1Detail, dev2Detail)
//{}

//EasyLanguagePointAdjustedCodeGenVisitor::~EasyLanguagePointAdjustedCodeGenVisitor()
//{}


//void EasyLanguagePointAdjustedCodeGenVisitor::genCodeForVariablesInEntryScript()
//{
//  std::ofstream *outFile = getOutputFileStream();

//  *outFile << "vars: shortStopDistance(0.0), longStopDistance(0.0), UnAdjustedClose(0.0);" << std::endl;
//  *outFile << "vars: profitTargetDistance(0.0), unAdjCloseAtEntry(0.0);" << std::endl << std::endl;
//}

//void
//EasyLanguagePointAdjustedCodeGenVisitor::visit (LongSideStopLossInPercent *stopLoss)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *stop = stopLoss->getStopLoss();

//  *outFile << "\t\t\tstopPercent = (" << *stop << "/100);" << std::endl;
//  *outFile << "\t\t\tlongStopDistance = Round2Fraction (UnAdjustedClose * stopPercent);"
//	   << std::endl;
//  *outFile << "\t\t\tlongStop = close - longStopDistance;" << std::endl;
//  *outFile << "\t\t\tstopStr = \"" << *stop << "%\";" << std::endl;
//}

//void EasyLanguagePointAdjustedCodeGenVisitor::visit (LongSideProfitTargetInPercent *profitTarget)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *target = profitTarget->getProfitTarget();

//  *outFile << "\t\t\tprofitTargetPercent = (" << *target << "/100);" << std::endl;
//    *outFile << "\t\t\ttargetStr = \"" << *target << "%\";" << std::endl;
//}

//void EasyLanguagePointAdjustedCodeGenVisitor::visit (ShortSideProfitTargetInPercent *profitTarget)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *target = profitTarget->getProfitTarget();

//  *outFile << "\t\t\tprofitTargetPercent = (" << *target << "/100);" << std::endl;
//  *outFile << "\t\t\ttargetStr = \"" << *target << "%\";" << std::endl;
//}

//void
//EasyLanguagePointAdjustedCodeGenVisitor::visit (ShortSideStopLossInPercent *stopLoss)
//{
//  std::ofstream *outFile = getOutputFileStream();
//  decimal7 *stop = stopLoss->getStopLoss();

//  *outFile << "\t\t\tstopPercent = (" << *stop << "/100);" << std::endl;
//  *outFile << "\t\t\tshortStopDistance = Round2Fraction (UnAdjustedClose * stopPercent);"
//	   << std::endl;
//  *outFile << "\t\t\tshortStop = close + shortStopDistance;" << std::endl;
//  *outFile << "\t\t\tstopStr = \"" << *stop << "%\";" << std::endl;
//}

//void EasyLanguagePointAdjustedCodeGenVisitor::genCodeToInitializeVariables()
//{
//std::ofstream *outFile = getOutputFileStream();

//  *outFile << "\t\tUnAdjustedClose = C of Data2;" << std::endl << std::endl;
//}

//void EasyLanguagePointAdjustedCodeGenVisitor::genCodeForEntryExit()
//{
//  std::ofstream *outFile = getOutputFileStream();

// *outFile << "\telse" << std::endl;
// *outFile << "\tbegin" << std::endl;
// *outFile << "\t\tif marketposition = 1 then begin" << std::endl;
 
// genCommonCodeForLongExitPrologue();

// *outFile << "\t\t\tif BarsSinceEntry = 0 then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tUnAdjustedClose = C of Data2;" << std::endl;
// *outFile << "\t\t\t\tlongStopDistance = Round2Fraction (UnAdjustedClose * stopPercent);" << std::endl;
// *outFile << "\t\t\t\tlongStop = EntryPrice - longStopDistance;" << std::endl;
// *outFile << "\t\t\t\tprofitTargetDistance = Round2Fraction (UnAdjustedClose * profitTargetPercent);" << std::endl;
// *outFile << "\t\t\t\tTargPrL = EntryPrice + profitTargetDistance;" << std::endl;
// *outFile << "\t\t\t\tunAdjCloseAtEntry = UnAdjustedClose;" << std::endl;
// *outFile << "\t\t\t\tIf Close > open then" << std::endl;
// *outFile << "\t\t\t\t\thighestPosChange = ((UnAdjustedClose/UnadjustedClose[1]) - 1) * 100.0;" << std::endl;

// *outFile << "\t\t\tend;" << std::endl << std::endl;

// *outFile << "\t\t\tIf Barssinceentry > 0 then" << std::endl;
// *outFile << "\t\t\tBegin" << std::endl;
// *outFile << "\t\t\t\tValue1 = ((UnAdjustedClose / unAdjCloseAtEntry) - 1) * 100;" << std::endl;
// *outFile << "\t\t\t\t\thighestPosChange = Maxlist (highestPosChange,value1 );" << std::endl;
// *outFile << "\t\t\tend;" << std::endl;
 
// *outFile << "\t\t\tif noNextDayOrders = False then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tsell next bar at TargPrL limit;" << std::endl;
// *outFile << "\t\t\t\tsell next bar at longStop stop;" << std::endl;
// *outFile << "\t\t\tend;" << std::endl;
// *outFile << "\t\tend;" << std::endl;

// *outFile << "\t\tif marketposition = -1 then begin" << std::endl;

// genCommonCodeForShortExitPrologue();

 
//*outFile << "\t\t\tif BarsSinceEntry = 0 then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tUnAdjustedClose = C of Data2;" << std::endl;
// *outFile << "\t\t\t\tshortStopDistance = Round2Fraction (UnAdjustedClose * stopPercent);" << std::endl;
// *outFile << "\t\t\t\tshortStop = EntryPrice + shortStopDistance;" << std::endl;
//*outFile << "\t\t\t\tprofitTargetDistance = Round2Fraction (UnAdjustedClose * profitTargetPercent);" << std::endl;
// *outFile << "\t\t\t\tTargPrS = EntryPrice - profitTargetDistance;" << std::endl;
// *outFile << "\t\t\tend;" << std::endl << std::endl;
//*outFile << "\t\t\tif noNextDayOrders = False then" << std::endl;
// *outFile << "\t\t\tbegin" << std::endl;
// *outFile << "\t\t\t\tbuy to cover next bar at TargPrS limit;" << std::endl;
// *outFile << "\t\t\t\tbuy to cover next bar at shortStop stop;" << std::endl;
// *outFile << "\t\t\tend;" << std::endl;
// *outFile << "\t\tend;" << std::endl;

// *outFile << "\tend;" << std::endl << std::endl;
// //*outFile << "end;" << std::endl << std::endl;

//}
