#pragma once

#include <thread>
#include <chrono>
#include <fcntl.h>
#include <sys/stat.h>
#include <curlpp/cURLpp.hpp>
#include "hooker.h"
#include "interfaces.h"
#include "Utils/util.h"

namespace projectspartan
{
	void SelfShutdown();
}
