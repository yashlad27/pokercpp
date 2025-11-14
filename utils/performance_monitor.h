#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include <chrono>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>

/**
 * Performance Monitoring and Benchmarking
 * 
 * Tracks execution time for different operations
 * Useful for optimization and showcasing performance
 */

class PerformanceMonitor {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::microseconds;
    
    struct Metric {
        long long totalMicroseconds = 0;
        int count = 0;
        long long minMicroseconds = LLONG_MAX;
        long long maxMicroseconds = 0;
    };
    
    static std::map<std::string, Metric> metrics;
    static std::map<std::string, TimePoint> activeTimers;
    
public:
    // Start timing an operation
    static void start(const std::string& operationName) {
        activeTimers[operationName] = Clock::now();
    }
    
    // Stop timing and record the duration
    static void stop(const std::string& operationName) {
        auto endTime = Clock::now();
        auto it = activeTimers.find(operationName);
        if (it == activeTimers.end()) {
            std::cerr << "Warning: No active timer for " << operationName << "\n";
            return;
        }
        
        auto duration = std::chrono::duration_cast<Duration>(endTime - it->second);
        long long micros = duration.count();
        
        Metric& m = metrics[operationName];
        m.totalMicroseconds += micros;
        m.count++;
        m.minMicroseconds = std::min(m.minMicroseconds, micros);
        m.maxMicroseconds = std::max(m.maxMicroseconds, micros);
        
        activeTimers.erase(it);
    }
    
    // Get average time in milliseconds
    static double getAverageMs(const std::string& operationName) {
        auto it = metrics.find(operationName);
        if (it == metrics.end() || it->second.count == 0) return 0.0;
        return (it->second.totalMicroseconds / it->second.count) / 1000.0;
    }
    
    // Get total count
    static int getCount(const std::string& operationName) {
        auto it = metrics.find(operationName);
        return (it == metrics.end()) ? 0 : it->second.count;
    }
    
    // Get operations per second
    static double getOpsPerSecond(const std::string& operationName) {
        auto it = metrics.find(operationName);
        if (it == metrics.end() || it->second.count == 0) return 0.0;
        
        double avgMicros = static_cast<double>(it->second.totalMicroseconds) / it->second.count;
        if (avgMicros <= 0) return 0.0;
        
        return 1000000.0 / avgMicros;  // Convert to ops/sec
    }
    
    // Print comprehensive report
    static void printReport() {
        if (metrics.empty()) {
            std::cout << "No performance metrics recorded.\n";
            return;
        }
        
        std::cout << "\n";
        std::cout << "\033[1m\033[36m";
        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              PERFORMANCE BENCHMARK REPORT                      ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\033[0m\n";
        
        // Header
        std::cout << std::left << std::setw(25) << "Operation"
                  << std::right << std::setw(10) << "Count"
                  << std::setw(12) << "Avg (ms)"
                  << std::setw(12) << "Min (ms)"
                  << std::setw(12) << "Max (ms)"
                  << std::setw(12) << "Ops/sec"
                  << "\n";
        std::cout << std::string(85, '-') << "\n";
        
        for (const auto& [name, metric] : metrics) {
            if (metric.count == 0) continue;
            
            double avgMs = metric.totalMicroseconds / metric.count / 1000.0;
            double minMs = metric.minMicroseconds / 1000.0;
            double maxMs = metric.maxMicroseconds / 1000.0;
            double opsPerSec = 1000000.0 / (static_cast<double>(metric.totalMicroseconds) / metric.count);
            
            std::cout << std::left << std::setw(25) << name
                      << std::right << std::setw(10) << metric.count
                      << std::setw(12) << std::fixed << std::setprecision(3) << avgMs
                      << std::setw(12) << std::fixed << std::setprecision(3) << minMs
                      << std::setw(12) << std::fixed << std::setprecision(3) << maxMs
                      << std::setw(12) << std::fixed << std::setprecision(0) << opsPerSec
                      << "\n";
        }
        
        std::cout << std::string(85, '-') << "\n\n";
    }
    
    // Reset all metrics
    static void reset() {
        metrics.clear();
        activeTimers.clear();
    }
};

// RAII timer for automatic start/stop
class ScopedTimer {
private:
    std::string operationName;
    
public:
    explicit ScopedTimer(const std::string& name) : operationName(name) {
        PerformanceMonitor::start(operationName);
    }
    
    ~ScopedTimer() {
        PerformanceMonitor::stop(operationName);
    }
};

// Macro for easy scoped timing
#define BENCHMARK(name) ScopedTimer _timer_##__LINE__(name)

#endif // PERFORMANCE_MONITOR_H
