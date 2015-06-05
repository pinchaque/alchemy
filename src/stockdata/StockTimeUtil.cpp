
#include "stockdata/StockTimeUtil.h"

namespace alch {


namespace StockTimeUtil {

StockTime getNextClose(const StockTime& stockTime)
{
  boost::posix_time::time_duration closeTime(13, 0, 0, 0);
  boost::posix_time::time_duration timeOfDay(stockTime.time_of_day());
  boost::gregorian::date date(stockTime.date());
  
  // if the time of day is after 13:00, then we at least have to go forward
  // to the next day
  if (timeOfDay >= closeTime)
  {
    date = date + boost::gregorian::date_duration(1);
  }

  // if we're now in a weekend, go forward to Monday
  if (date.day_of_week() == boost::gregorian::Sunday)
  {
    date = date + boost::gregorian::date_duration(1);
  }
  else if (date.day_of_week() == boost::gregorian::Saturday)
  {
    date = date + boost::gregorian::date_duration(2);
  }

  // return the day we've figured out plus the closeTime as offset
  return StockTime(date, closeTime);  
}

  
StockTime getPreviousClose(const StockTime& stockTime)
{
  boost::posix_time::time_duration closeTime(13, 0, 0, 0);
  boost::posix_time::time_duration timeOfDay(stockTime.time_of_day());
  boost::gregorian::date date(stockTime.date());
  
  // if the time of day is before 13:00, then we at least have to back
  // up to the previous day
  if (timeOfDay < closeTime)
  {
    date = date - boost::gregorian::date_duration(1);
  }

  // if we're now in a weekend, back up to previous Friday
  if (date.day_of_week() == boost::gregorian::Sunday)
  {
    date = date - boost::gregorian::date_duration(2);
  }
  else if (date.day_of_week() == boost::gregorian::Saturday)
  {
    date = date - boost::gregorian::date_duration(1);
  }

  // return the day we've figured out plus the closeTime as offset
  return StockTime(date, closeTime);
}

} // namespace StockTimeUtil

} // namespace alch
