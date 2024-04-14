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

		virtual Type GetType() const = 0;

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
		static const Directory& SpecificCast(const DirectoryElement& directory_elem);

	private:
		virtual Type GetType() const override;
	};

	enum class AssetType
	{
		Unknown,
		Prefab,
		Bitmap,
		Scene
	};

	class AssetFile
		: public DirectoryElement
	{
	public:
		AssetFile(const std::filesystem::path& path);

		static AssetFile& SpecificCast(DirectoryElement& directory_elem);
		static const AssetFile& SpecificCast(const DirectoryElement& directory_elem);

		AssetType GetAssetType() const;

		static const char* GetAssetExtension(AssetType asset_type);

	public:
		static constexpr const char* s_PrefabFileExtension = ".prefab";
		static constexpr const char* s_BitmapFileExtension = ".bmp";
		static constexpr const char* s_SceneFileExtension = ".scene";

	private:
		virtual Type GetType() const override;
	};

	// Responsible for extracting all assets/directories from a given directory
	class AssetMiner
	{
	public:
		static std::unique_ptr<Directory> CreateDirectory(const std::filesystem::path& path);

		static void GetDirectoryContents(const Directory& dir, std::vector<std::unique_ptr<DirectoryElement>>& out_contents);
		static void GetDirectoriesInside(const Directory& src_dir, std::vector<Directory>& out_directories);

	private:
		static bool ShouldDirectoryEntryBeIgnored(const std::filesystem::directory_entry& entry);
	};
}
