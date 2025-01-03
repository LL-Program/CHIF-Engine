#include "chifVersion.h"

#include <string>

namespace chif::version
{
	const int major = 0;
	const int minor = 0;
	//bug fixes, alterations, refactors, updates
	const int revision = 3;

	const std::string version_string = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);

	int GetMajor()
	{
		return major;
	}
	int GetMinor()
	{
		return minor;
	}
	int GetRevision()
	{
		return revision;
	}
	const char* GetVersionString()
	{
		return version_string.c_str();
	}

	const char* GetCreditsString()
	{
		static const char* credits = R"(
Credits
-----------
?????????
		)";

		return credits;
	}

}
