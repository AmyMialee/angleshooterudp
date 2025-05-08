#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

int main(int, char*[]) {
	try {
		Logger::info("Starting AngleShooter Client");
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}