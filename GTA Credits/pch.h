#pragma once

#include "plugin.h"

#include <string>
#include <vector>

#include "CMessages.h"
#include "CFont.h"

static std::string ToLower(std::string data) {
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::tolower(c);
	});
	return data;
}