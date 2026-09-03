#pragma once

#include "calendar/calendar_types.h"

#include <map>
#include <string>
#include <vector>

namespace calendar {

  // Flatten the per-account event lists into one snapshot ordered by start time.
  //
  // When dedupe is true, an occurrence that appears in more than one calendar is collapsed to a
  // single entry. This covers two calendars in one account that both hold the event as well as the
  // same calendar reached through two accounts (say a Google account and a CalDAV account).
  // Provider event ids differ between backends and between calendars, so identity is the visible
  // shape of the occurrence: title, start, end, and the all-day flag. Among duplicates the kept
  // entry is the one carrying the most presentation metadata (color, link, location).
  [[nodiscard]] std::vector<CalendarEvent>
  mergeCalendarEvents(const std::map<std::string, std::vector<CalendarEvent>>& eventsByAccount, bool dedupe);

} // namespace calendar
