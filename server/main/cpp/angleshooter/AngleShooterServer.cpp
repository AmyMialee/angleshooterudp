#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

int main(int, char*[]) {
    try {
        Logger::info("Starting AngleShooter Server");
    } catch(std::runtime_error& e) {
        Logger::error(e.what());
    } catch(...) {
        Logger::error("An Error Occurred");
    }
    return 0;
}