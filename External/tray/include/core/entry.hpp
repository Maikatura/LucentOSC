#pragma once
#include <string>

namespace Tray
{
    class BaseTray;
    class TrayEntry
    {
      protected:
        std::wstring text;
        bool disabled = false;
        BaseTray *parent = nullptr;

      public:
        TrayEntry(std::wstring text);
        virtual ~TrayEntry() = default;

        BaseTray *getParent();
        void setParent(BaseTray *);

        std::wstring getText();
        void setText(std::wstring);

        void setDisabled(bool);
        bool isDisabled() const;
    };
} // namespace Tray