// DirFinder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

inline CStringW TerminateDir(const CStringW& dir)
{
	auto && result = dir[dir.GetLength() - 1] == L'\\' ? dir : dir + L'\\';
	return result;
}

inline bool IsDir(DWORD attributes)
{
	return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
}

inline bool IsDir(const wchar_t* pStr)
{
	auto attribs = ::GetFileAttributesW(pStr);
	return IsDir(attribs);
}

inline bool IsDot(const wchar_t* pStr)
{
	if (pStr)
	{
		if (*pStr == L'.' && (
			*(pStr + 1) == L'\0' ||
			*(pStr + 1) == L'.' && *(pStr + 2) == L'\0'))
		{
			return true;
		}
	}

	return false;
}

inline void MakeLongPathSyntax(CStringW& str)
{
	// FindFirstFile documentation:
	// To extend this limit to 32,767 wide characters, call the Unicode version of the function
	// and prepend "\\?\" to the path. For more information, see Naming a File.
	//
	// for more see here: https://msdn.microsoft.com/en-us/library/aa365247(v=vs.85).aspx

	auto pStr = (LPCTSTR)str;
	if (pStr && *pStr == L'\\' && *(pStr + 1) == L'\\' && *(pStr + 2) == L'?' && *(pStr + 3) == L'\\')
	{
		return;
	}

	str = L"\\\\?\\" + str;
}

inline vector<CStringW> GetDirsRecursive(const CStringW& dir)
{
	WIN32_FIND_DATAW findData;
	HANDLE hFind;

	auto baseDir = TerminateDir(dir);
	MakeLongPathSyntax(baseDir);

	auto result = vector<CStringW>();

	hFind = ::FindFirstFileW(baseDir + L'*', &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		wcerr << L"Error " << ::GetLastError() << " for '" << (LPCTSTR)baseDir << L"'" << endl;
#endif
		return result;
	}

	if (!IsDot(findData.cFileName) && IsDir(findData.dwFileAttributes))
	{
		result.push_back(baseDir + findData.cFileName);
	}

	while (::FindNextFileW(hFind, &findData))
	{
		if (!IsDot(findData.cFileName) && IsDir(findData.dwFileAttributes))
		{
			result.push_back(baseDir + findData.cFileName);
		}
	}

#ifdef _DEBUG
	auto err = ::GetLastError();
	if (err != ERROR_NO_MORE_FILES)
	{
		wcerr << L"Error " << err << L" for '" << (LPCTSTR)findData.cFileName << endl;
	}
#endif

	::FindClose(hFind);

	auto newSubdirs = vector<CStringW>();
	for (auto const& subdir : result)
	{
		auto subdirs = GetDirsRecursive(subdir);
		newSubdirs.insert(newSubdirs.end(), subdirs.begin(), subdirs.end());
	}

	result.insert(result.end(), newSubdirs.begin(), newSubdirs.end());
	return result;
}

inline vector<CStringW> GetMatchingFiles(const CStringW& dir, const wchar_t* mask)
{
	WIN32_FIND_DATAW findData;
	HANDLE hFind;

	auto baseDir = TerminateDir(dir);
	MakeLongPathSyntax(baseDir);

	auto result = vector<CStringW>();

	hFind = ::FindFirstFileW(baseDir + mask, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		wcerr << L"Error " << ::GetLastError() << " for '" << (LPCTSTR)baseDir << L"'" << endl;
#endif
		return result;
	}

	if (!IsDot(findData.cFileName) && !IsDir(findData.dwFileAttributes))
	{
		result.push_back(baseDir + findData.cFileName);
	}

	while (::FindNextFileW(hFind, &findData))
	{
		if (!IsDot(findData.cFileName) && !IsDir(findData.dwFileAttributes))
		{
			result.push_back(baseDir + findData.cFileName);
		}
	}

#ifdef _DEBUG
	auto err = ::GetLastError();
	if (err != ERROR_NO_MORE_FILES)
	{
		wcerr << L"Error " << err << L" for '" << (LPCTSTR)(baseDir + findData.cFileName) << endl;
	}
#endif

	::FindClose(hFind);

	return result;
}

inline CStringW ToLower(CStringW const& s)
{
	return s;
}

inline bool StartsWith(CStringW const& str, CStringW const& prefix)
{
	if (str.GetLength() < prefix.GetLength())
	{
		return false;
	}

	auto p = stdext::make_unchecked_array_iterator((LPCTSTR)prefix);
	return equal(p, p + prefix.GetLength(), stdext::make_unchecked_array_iterator((LPCTSTR)str));
}

inline bool IsExcluded(CStringW const& dir, vector<CStringW> const& excludes)
{
	for (auto const& e : excludes)
	{
		if (StartsWith(dir, e))
		{
			return true;
		}
	}

	return false;
}

inline map<CStringW, vector<CStringW>> GetFilesRecursive(const CStringW& dir, const wchar_t* mask, vector<CStringW> const& excludes)
{
	auto result = map<CStringW, vector<CStringW>>();
	/*auto baseDir = TerminateDir(dir);
	if (IsExcluded(dir, excludes))
	{
		return result;
	}

	auto entries = GetDirEntries(dir, mask);

	auto fileEntriesOnly = vector<CStringW>();

	for (auto const& entry : entries)
	{
		auto && fullEntry = baseDir + entry;
		if (IsDir(fullEntry))
		{
			auto subDirResult = GetFilesRecursive(fullEntry, mask, excludes);
			for (auto const& subDirItem : subDirResult)
			{
				result.insert(subDirItem);
			}
		}
		else
		{
			fileEntriesOnly.push_back(entry);
		}
	}

	if (!fileEntriesOnly.empty())
	{
		result.insert(make_pair(baseDir, fileEntriesOnly));
	}*/

	return result;
}

inline map<CStringW, vector<CStringW>> GetFilesRecursive(const CStringW& dir, const wchar_t* mask)
{
	return GetFilesRecursive(dir, mask, {});
}

int _tmain(int argc, const wchar_t* argv[])
{
	if (argc != 3)
	{
		wcout << L"Usage: " << argv[0] << L" [target_path] [mask]\n";
		return 0;
	}

	auto matches = vector<CStringW>();
	PVOID oldVal = NULL;
	auto isDisabled = ::Wow64DisableWow64FsRedirection(&oldVal);
	if (isDisabled)
	{
		auto dirs = GetDirsRecursive(argv[1]);
		//wcout << "dircount: " << dirs.size() << endl;
		for (auto const& dir : dirs)
		{
			if (!IsExcluded(dir, {}))
			{
				auto files = GetMatchingFiles(dir, argv[2]);
				matches.insert(matches.end(), files.begin(), files.end());
			}
		}

		::Wow64RevertWow64FsRedirection(oldVal);

		for (auto const& file : matches)
		{
			wcout << (LPCTSTR)file << endl;
		}
	}

	return 0;
}