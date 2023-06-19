#include <components/submenu.hpp>
#include <core/entry.hpp>
#include <bot/application.hpp>

Tray::Submenu::Submenu(std::wstring text) : TrayEntry(std::move(text)) {}
void Tray::Submenu::update()
{
    if (parent)
    {
        parent->UpdateTray();
    }
}

std::vector<std::shared_ptr<Tray::TrayEntry>> Tray::Submenu::getEntries()
{
    return entries;
}