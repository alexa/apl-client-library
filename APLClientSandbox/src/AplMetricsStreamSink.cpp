#include "../include/APLClientSandbox/AplMetricsStreamSink.h"

#include <stdint.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std::chrono;

std::string AplMetricsStreamSink::formatMetricLog(
    const std::string name,
    const std::string value
    ) {
    const std::string identifier = "APL-WebViewhostMetrics";

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = system_clock::to_time_t(now);

    std::tm bt = *std::localtime(&timer);
    std::ostringstream oss;
    oss << std::put_time(&bt, "%m-%d %H:%M:%S"); // MM-DD HH:MM:SS
    oss << "." << std::setfill('0') << std::setw(3) << ms.count() << " | " << identifier << " : ";
    oss << "name=\"" << name << "\" ";
    oss << "value=" << value;
    return oss.str();
}

void AplMetricsStreamSink::reportTimer(
        const std::map<std::string, std::string> &metadata,
        const std::string& name,
        const std::chrono::nanoseconds& value) {
    std::string v = std::to_string(value.count());
    std::cout << formatMetricLog(name, v) << "\n";
    auto message("{\"type\": \"metric\", \"payload\":"
                        "{\"kind\": \"timer\", \"name\":\"" + name + "\", \"value\": " + v + "}}");
    mAplClientBridge->sendMessage("", message);
}

void AplMetricsStreamSink::reportCounter(
        const std::map<std::string, std::string> &metadata,
        const std::string& name,
        uint64_t value) {
    std::ostringstream o;
    o << value;
    std::cout << formatMetricLog(name, o.str()) << "\n";
    auto message("{\"type\": \"metric\", \"payload\":"
                 "{\"kind\": \"counter\", \"name\":\"" + name + "\", \"value\": " + o.str() + "}}");
    mAplClientBridge->sendMessage("", message);
}

AplMetricsStreamSink::AplMetricsStreamSink(std::shared_ptr<AplClientBridge> aplClientBridge): mAplClientBridge(std::move(aplClientBridge)){
}