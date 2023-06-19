#pragma once
#include <string>


class Application;

namespace Tray
{
    class BaseTray;
    class TrayEntry
    {
      protected:
        std::wstring text;
        bool disabled = false;
        Application *parent = nullptr;

      public:
        TrayEntry(std::wstring text);
        virtual ~TrayEntry() = default;

        Application* getParent();
        void setParent(Application *);

        std::wstring getText();
        void setText(std::wstring);

        void setDisabled(bool);
        bool isDisabled() const;
    };
} // namespace Tray