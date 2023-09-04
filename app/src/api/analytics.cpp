#include "api/analytics.hpp"
#include "api/http.hpp"
#include "utils/config.hpp"
#include "utils/thread.hpp"

namespace analytics {

const std::string GA_ID = "";
const std::string GA_KEY = "";
const std::string GA_URL = "https://fuck-analytics.googay.030/mp/collect";

class Property {
public:
    std::string value;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Property, value);

class Package {
public:
    std::string client_id, user_id;
    int64_t timestamp_micros;
    std::unordered_map<std::string, Property> user_properties;
    std::vector<Event> events;

    Package() {
        user_properties = {
            {"platform", {AppVersion::getPlatform()}},
            {"version", {AppVersion::getVersion()}},
            {"commit", {AppVersion::getCommit()}},
        };
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Package, client_id, user_id, user_properties, events, timestamp_micros);

Analytics::Analytics() {
}

Analytics::~Analytics() { this->ticker.stop(); }

void Analytics::report(const std::string& event, Params params) {
}

void Analytics::send() {
}

}  // namespace analytics