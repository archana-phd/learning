#include <atomic>
#include <thread>
#include <unordered_map>
#include <string>

// Class to represent an FX pair
class FXPair {
public:
    FXPair(const std::string& pair) : pair_(pair), value_(0.0) {}

    const std::string& getPair() const { return pair_; }
    double getValue() const { return value_.load(); }
    void setValue(double value) { value_.store(value); }

private:
    std::string pair_;
    std::atomic<double> value_;
};

// Manages FX pairs and provides access
class FXDataManager {
public:
    FXDataManager() = default;

    void addFXPair(const std::string& pair) {
        fxPairs_.emplace(pair, FXPair(pair));
    }

    FXPair* getFXPair(const std::string& pair) {
        auto it = fxPairs_.find(pair);
        return (it != fxPairs_.end()) ? &(it->second) : nullptr;
    }

    void updateFXPair(const std::string& pair, double value) {
        auto it = fxPairs_.find(pair);
        if (it != fxPairs_.end()) {
            it->second.setValue(value);
        }
    }

private:
    std::unordered_map<std::string, FXPair> fxPairs_;
};

// Handles client requests in separate threads
class ClientHandler {
public:
    ClientHandler(FXDataManager& fxManager) : fxManager_(fxManager) {}

    void handleRequest(const std::string& request) {
        // Parse request and interact with fxManager_
    }

private:
    FXDataManager& fxManager_;
};

// Updates FX pair values
class FXUpdater {
public:
    FXUpdater(FXDataManager& fxManager) : fxManager_(fxManager) {}

    void start() {
        updaterThread_ = std::thread(&FXUpdater::updateLoop, this);
    }

    void stop() {
        running_ = false;
        if (updaterThread_.joinable()) {
            updaterThread_.join();
        }
    }

private:
    void updateLoop() {
        while (running_) {
            // Update FX pairs with new values
            // e.g., fxManager_.updateFXPair("USD-EUR", newValue);
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Interval for updates
        }
    }

    FXDataManager& fxManager_;
    std::thread updaterThread_;
    std::atomic<bool> running_{true};
};

// Main FX Server class
class FXServer {
public:
    FXServer() : fxDataManager_(), fxUpdater_(fxDataManager_) {}

    void start() {
        fxUpdater_.start();
        // Start accepting client connections and create ClientHandler threads
    }

    void stop() {
        fxUpdater_.stop();
        // Stop accepting client connections and join ClientHandler threads
    }

private:
    FXDataManager fxDataManager_;
    FXUpdater fxUpdater_;
};
