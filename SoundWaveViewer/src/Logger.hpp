#pragma once

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>

namespace efram { namespace logging {

enum class Level {
	DEBUG, INFO, WARNING, ERR
};

Level rootLoggerLevel = Level::INFO;

class Logger {	
	
	private: const std::string name;

	public: Logger(std::string name) : name(name) {}

	public: void Info(std::string message) {
		Log(Level::INFO, message);
	}

	public: void Error(std::string message) {
		Log(Level::ERR, message);
	}

	public: void Log(Level level, std::string message) {
		ShowMessage(level, message);
	}

	private: void ShowMessage(Level level, std::string message) {
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		struct tm *parts = std::localtime(&time);
		
		printf("%04d-%02d-%02d %02d:%02d:%02d %7s [%6d] %12s: %s", 1900 + parts->tm_year, parts->tm_mon, parts->tm_mday, parts->tm_hour, parts->tm_min, parts->tm_sec,
				LevelToString(level).c_str(), std::this_thread::get_id(), name.c_str(), message.c_str());
		std::cout << std::endl;
	}

	private: static std::string LevelToString(Level level) {
		if (level == Level::INFO) return "INFO";
		if (level == Level::DEBUG) return "DEBUG";
		if (level == Level::WARNING) return "WARNING";
		if (level == Level::ERR) return "ERROR";
		return "NONE";
	}

};

Logger rootLogger = Logger("root");

}}