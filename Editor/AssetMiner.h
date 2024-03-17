#pragma once
#include <vector>
#include <iostream>
#include <filesystem>

namespace files
{
	class DirectoryElement
	{
	public:
		enum class Type
		{
			Directory,
			AssetFile
		};

	public:
		const std::string GetFileName() const;
		const std::string GetPath() const;

		virtual Type GetType() = 0;

	protected:
		DirectoryElement(const std::filesystem::path& path);

	protected:
		std::filesystem::path m_Path;
	};

	class Directory
		: public DirectoryElement
	{
	public:
		Directory(const std::filesystem::path& path);

		static Directory& SpecificCast(DirectoryElement& directory_elem);

	private:
		virtual Type GetType() override;
	};

	class AssetFile
		: public DirectoryElement
	{
	public:
		AssetFile(const std::filesystem::path& path);

		static AssetFile& SpecificCast(DirectoryElement& directory_elem);

	private:
		virtual Type GetType() override;
	};

	// Responsible for extracting all assets/directories from a given directory
	class AssetMiner
	{
	public:
		static void GetDirectoryContents(const Directory& dir, std::vector<std::unique_ptr<DirectoryElement>>& out_contents);
	};
}
