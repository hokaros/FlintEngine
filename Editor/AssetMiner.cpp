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
        FE_ASSERT(directory_elem.GetType() == Type::Directory, "Invalid cast");
        return static_cast<Directory&>(directory_elem);
    }

    DirectoryElement::Type Directory::GetType()
    {
        return Type::Directory;
    }

    AssetFile::AssetFile(const std::filesystem::path& path)
        : DirectoryElement(path)
    {
    }

    AssetFile& AssetFile::SpecificCast(DirectoryElement& directory_elem)
    {
        FE_ASSERT(directory_elem.GetType() == Type::AssetFile, "Invalid cast");
        return static_cast<AssetFile&>(directory_elem);
    }

    DirectoryElement::Type AssetFile::GetType()
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
}