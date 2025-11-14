#include "performance_monitor.h"

// Initialize static members
std::map<std::string, PerformanceMonitor::Metric> PerformanceMonitor::metrics;
std::map<std::string, PerformanceMonitor::TimePoint> PerformanceMonitor::activeTimers;
