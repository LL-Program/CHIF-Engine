#ifndef CHIFENGINE_VERSION_DEFINED
#define CHIFENGINE_VERSION_DEFINED

namespace chif::version
{
	long GetVersion();
	int GetMajor();
	int GetMinor();
	int GetRevision();
	const char* GetVersionString();

	const char* GetCreditsString();
}

#endif // CHIFENGINE_VERSION_DEFINED
