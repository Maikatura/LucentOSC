#include <core/entry.hpp>
#include <core/traybase.hpp>
#include <Bot/Application.hpp>


Tray::TrayEntry::TrayEntry(std::wstring text) : text(std::move(text)) {}

Application* Tray::TrayEntry::getParent()
{
    return parent;
}

void Tray::TrayEntry::setParent(Application *newParent)
{
    parent = newParent;
}

std::wstring Tray::TrayEntry::getText()
{
    return text;
}

void Tray::TrayEntry::setText(std::wstring newText)
{
    text = std::move(newText);
    if (parent)
    {
        parent->UpdateTray();
    }
}

bool Tray::TrayEntry::isDisabled() const
{
    return disabled;
}

void Tray::TrayEntry::setDisabled(bool state)
{
    disabled = state;
    if (parent)
    {
        parent->UpdateTray();
    }
}