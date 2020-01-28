#ifndef ODFAEG_WIN32_MOUSE
#define ODFAEG_WIN32_MOUSE
#ifdef _WIN32_WINDOWS
    #undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
    #undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <windows.h>
#endif // ODFAEG_WIN32_MOUSE
namespace odfaeg {
    namespace window {
        ////////////////////////////////////////////////////////////
        bool Win32Mouse::isMouseButtonPressed(Mouse::Button button)
        {
            int vkey = 0;
            switch (button)
            {
                case IMouse::Left:     vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
                case IMouse::Right:    vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
                case IMouse::Middle:   vkey = VK_MBUTTON;  break;
                case IMouse::XButton1: vkey = VK_XBUTTON1; break;
                case IMouse::XButton2: vkey = VK_XBUTTON2; break;
                default:              vkey = 0;           break;
            }

            return (GetAsyncKeyState(vkey) & 0x8000) != 0;
        }
        ////////////////////////////////////////////////////////////
        Vector2i InputImpl::getMousePosition()
        {
            POINT point;
            GetCursorPos(&point);
            return Vector2i(point.x, point.y);
        }
        ////////////////////////////////////////////////////////////
        Vector2i Win32Mouse::getMousePosition(const Win32Window& relativeTo)
        {
            WindowHandle handle = relativeTo.getSystemHandle();
            if (handle)
            {
                POINT point;
                GetCursorPos(&point);
                ScreenToClient(handle, &point);
                return Vector2i(point.x, point.y);
            }
            else
            {
                return Vector2i();
            }
        }


        ////////////////////////////////////////////////////////////
        void Win32Mouse::setMousePosition(const Vector2i& position)
        {
            SetCursorPos(position.x, position.y);
        }


        ////////////////////////////////////////////////////////////
        void Win32Mouse::setMousePosition(const Vector2i& position, const Win32Window& relativeTo)
        {
            WindowHandle handle = relativeTo.getSystemHandle();
            if (handle)
            {
                POINT point = {position.x, position.y};
                ClientToScreen(handle, &point);
                SetCursorPos(point.x, point.y);
            }
        }
    }
}
