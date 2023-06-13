#include <components/submenu.hpp>
#include <core/entry.hpp>

Tray::Submenu::Submenu(std::wstring text) : TrayEntry(std::move(text)) {}
void Tray::Submenu::update()
{
    if (parent)
    {
        parent->update();
    }
}

std::vector<std::shared_ptr<Tray::TrayEntry>> Tray::Submenu::getEntries()
{
    return entries;
}