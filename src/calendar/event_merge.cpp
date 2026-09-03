#include "calendar/event_merge.h"

#include <algorithm>
#include <tuple>
#include <utility>

namespace calendar {

  namespace {
    // How much presentation metadata an event carries. Used to pick the representative when the
    // same occurrence is present in two calendars and one copy is richer than the other.
    int metadataRichness(const CalendarEvent& event) {
      return (event.colorHex.empty() ? 0 : 1) + (event.url.empty() ? 0 : 1) + (event.location.empty() ? 0 : 1);
    }

    bool sameOccurrence(const CalendarEvent& a, const CalendarEvent& b) {
      return a.allDay == b.allDay && a.start == b.start && a.end == b.end && a.title == b.title;
    }
  } // namespace

  std::vector<CalendarEvent>
  mergeCalendarEvents(const std::map<std::string, std::vector<CalendarEvent>>& eventsByAccount, bool dedupe) {
    std::vector<CalendarEvent> merged;
    for (const auto& [accountId, events] : eventsByAccount) {
      merged.insert(merged.end(), events.begin(), events.end());
    }

    if (dedupe && merged.size() > 1) {
      std::ranges::sort(merged, [](const CalendarEvent& a, const CalendarEvent& b) {
        return std::tie(a.start, a.end, a.allDay, a.title) < std::tie(b.start, b.end, b.allDay, b.title);
      });
      std::vector<CalendarEvent> deduped;
      deduped.reserve(merged.size());
      for (auto& event : merged) {
        if (!deduped.empty() && sameOccurrence(deduped.back(), event)) {
          if (metadataRichness(event) > metadataRichness(deduped.back())) {
            deduped.back() = std::move(event);
          }
          continue;
        }
        deduped.push_back(std::move(event));
      }
      merged = std::move(deduped);
    }

    std::ranges::sort(merged, {}, &CalendarEvent::start);
    return merged;
  }

} // namespace calendar
