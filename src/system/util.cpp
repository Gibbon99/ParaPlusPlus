#include "../../hdr/system/util.h"

typedef struct {
	char *pointer;
	int  size;
} _paraMemoryMap;

std::map<std::string, _paraMemoryMap> memoryMap;

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string sys_getString (std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
    const char *const zcFormat = format.c_str ();

    // initialize use of the variable argument array
    va_list vaArgs;
    va_start(vaArgs, format);

    // reliably acquire the size from a copy of the variable argument array
    // and a functionally reliable call to mock the formatting
    va_list vaCopy;
    va_copy(vaCopy, vaArgs);
    const int iLen = std::vsnprintf (nullptr, 0, zcFormat, vaCopy);
    va_end(vaCopy);

    // return a formatted string without risking memory mismanagement  and without assuming any compiler
    // or platform specific behavior
    std::vector<char> zc (iLen + 1);
    std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
    va_end(vaArgs);

    return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Allocate memory and return pointer - record the size as well
char *sys_malloc (int memorySize, const std::string& keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	_paraMemoryMap newMemoryMapEntry;

	newMemoryMapEntry.pointer = (char *) malloc (sizeof (char) * memorySize);
	if (newMemoryMapEntry.pointer == nullptr)
	{
		logFile.write(sys_getString ("Memory allocation error for [ %s ]", keyName.c_str ()));
	}

	newMemoryMapEntry.size = memorySize;

	memoryMap.insert (std::pair<std::string, _paraMemoryMap> (keyName, newMemoryMapEntry));

	logFile.write(sys_getString("Allocated [ %i ] for [ %s ]", memorySize, keyName.c_str()));

	return newMemoryMapEntry.pointer;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Go through memory map and free allocations
void sys_freeMemory ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (memoryMap.empty ())
	{
		return;
	}

	for (auto &memoryItr : memoryMap)
	{
		logFile.write(sys_getString ("Free memory [ %i bytes ] - [ %s ]", memoryItr.second.size, memoryItr.first.c_str ()).c_str ());

		if (memoryItr.second.pointer != nullptr)
		{
			free (memoryItr.second.pointer);
		}
	}
}