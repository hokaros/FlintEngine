#include "AssetMiner.h"
#include "../FlatEngine/utility.h"

namespace files
{
    Directory::Directory(const std::filesystem::path& path)
        : DirectoryElement(path)
    {
    }

    Directory& Directory::SpecificCast(DirectoryElement& directory_elem)
    {
        return const_cast<Directory&>(SpecificCast(const_cast<const DirectoryElement&>(directory_elem)));
    }

    const Directory& Directory::SpecificCast(const DirectoryElement& directory_elem)
    {
        FE_ASSERT(directory_elem.GetType() == Type::Directory, "Invalid cast");
        return static_cast<const Directory&>(directory_elem);
    }

    DirectoryElement::Type Directory::GetType() const
    {
        return Type::Directory;
    }

    AssetFile::AssetFile(const std::filesystem::path& path)
        : DirectoryElement(path)
    {
    }

    AssetFile& AssetFile::SpecificCast(DirectoryElement& directory_elem)
    {
        return const_cast<AssetFile&>(SpecificCast(const_cast<const DirectoryElement&>(directory_elem)));
    }

    const AssetFile& AssetFile::SpecificCast(const DirectoryElement& directory_elem)
    {
        FE_ASSERT(directory_elem.GetType() == Type::AssetFile, "Invalid cast");
        return static_cast<const AssetFile&>(directory_elem);
    }

    AssetType AssetFile::GetAssetType() const
    {
        std::string ext = m_Path.extension().string();
        if (ext == s_PrefabFileExtension)
        {
            return AssetType::Prefab;
        }
        else if (ext == s_BitmapFileExtension)
        {
            return AssetType::Bitmap;
        }
        else if (ext == s_SceneFileExtension)
        {
            return AssetType::Scene;
        }
        else
        {
            return AssetType::Unknown;
        }
    }

    const char* AssetFile::GetAssetExtension(AssetType asset_type)
    {
        switch (asset_type)
        {
        case files::AssetType::Prefab:
            return s_PrefabFileExtension;

        case files::AssetType::Bitmap:
            return s_BitmapFileExtension;

        case files::AssetType::Scene:
            return s_SceneFileExtension;

        case files::AssetType::Unknown:
            return "";

        default:
            FE_ASSERT(false, "Unimplemented asset type extension");
            return "";
        }
    }

    DirectoryElement::Type AssetFile::GetType() const
    {
        return Type::AssetFile;
    }

    const std::string DirectoryElement::GetFileName() const
    {
        return m_Path.filename().string();
    }

    const std::string DirectoryElement::GetPath() const
    {
        return m_Path.string();
    }

    DirectoryElement::DirectoryElement(const std::filesystem::path& path)
        : m_Path(path)
    {
    }



    void AssetMiner::GetDirectoryContents(const Directory& dir, std::vector<std::unique_ptr<DirectoryElement>>& out_contents)
    {
        namespace fs = std::filesystem;

        for (auto& entry : fs::directory_iterator(dir.GetPath()))
        {
            std::unique_ptr<DirectoryElement> elem = nullptr;
            if (entry.is_directory())
            {
                elem = std::make_unique<Directory>(entry.path().string());
            }
            else
            {
                elem = std::make_unique<AssetFile>(entry.path().string());
            }

            out_contents.push_back(std::move(elem));
        }
    }

    void AssetMiner::GetDirectoriesInside(const Directory& src_dir, std::vector<Directory>& out_directories)
    {
        namespace fs = std::filesystem;

        for (auto& entry : fs::directory_iterator(src_dir.GetPath()))
        {
            if (!entry.is_directory())
                continue;

            out_directories.emplace_back(entry.path().string());
        }
    }
}