#include <components/imagebutton.hpp>
#include <core/traybase.hpp>
#include <utility>
#include <bot/application.hpp>

Tray::ImageButton::ImageButton(std::wstring text, Image image, std::function<void()> callback)
    : Button(std::move(text), std::move(callback)), image(image)
{
}

void Tray::ImageButton::setImage(Image newImage)
{
    image = newImage;
    if (parent)
    {
        parent->UpdateTray();
    }
}

Tray::Image Tray::ImageButton::getImage()
{
    return image;
}