#include "main.hpp"

void addStatEvent(std::string start, std::string end, std::string id, int value) {
    log::debug("Recieving addStatEvent: start={}", start);
    StatDisplayBaseGameLayer* layer = static_cast<StatDisplayBaseGameLayer*>(GJBaseGameLayer::get());
    otherStats++;
    g_strings.insert(std::make_pair<int, std::pair<std::string, std::string>>(static_cast<int>(Stat::OTHER) + otherStats, { start, end }));
    layer->createStatsNode(layer->m_fields->m_statsContainer, Stat::OTHER, id, value);
}

// declare dispatchEvents
$execute{
    // add a stat
    // contains start text, end text, id, and an image
    new EventListener(+[](std::string start, std::string end, std::string id, int value) {
        addStatEvent(start, end, id, value);
        return ListenerResult::Stop;
    }, ToFilter<EventAddStat>("legowiifun.stat_display/addStat"));
};