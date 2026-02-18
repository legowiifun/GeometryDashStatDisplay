#define GEODE_DEFINE_EVENT_EXPORTS
#include "hooks/StatDisplayBaseGameLayer.hpp"
#include "api.hpp"
#include "Stat.cpp"

void stat_display_api::addStatEvent(std::string start, std::string end, std::string id, int value) {
    log::debug("Recieving addStatEvent: start={}", start);
    StatDisplayBaseGameLayer* layer = static_cast<StatDisplayBaseGameLayer*>(GJBaseGameLayer::get());
    otherStats++;
    g_strings.insert(std::make_pair<int, std::pair<std::string, std::string>>(static_cast<int>(Stat::OTHER) + otherStats, { start, end }));
    layer->createStatsNode(layer->m_fields->m_statsContainer, Stat::OTHER, id, value);
    layer->m_fields->m_statsContainer->updateLayout();
}