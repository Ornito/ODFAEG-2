#ifndef ODFAEG_X11MOUSE_HPP
#define ODFAEG_X11MOUSE_HPP
#include "../../../include/odfaeg/Window/iMouse.hpp"
#include "../../../include/odfaeg/Window/x11Window.hpp"
namespace odfaeg {
    namespace window {
        class X11Mouse {
        public:

            ////////////////////////////////////////////////////////////
            /// \brief Check if a mouse button is pressed
            ///
            /// \param button Button to check
            ///
            /// \return True if the button is pressed, false otherwise
            ///
            ////////////////////////////////////////////////////////////
            static bool isButtonPressed(IMouse::Button button);

            ////////////////////////////////////////////////////////////
            /// \brief Get the current position of the mouse in desktop coordinates
            ///
            /// This function returns the global position of the mouse
            /// cursor on the desktop.
            ///
            /// \return Current position of the mouse
            ///
            ////////////////////////////////////////////////////////////
            static sf::Vector2i getPosition();

            ////////////////////////////////////////////////////////////
            /// \brief Get the current position of the mouse in window coordinates
            ///
            /// This function returns the current position of the mouse
            /// cursor, relative to the given window.
            ///
            /// \param relativeTo Reference window
            ///
            /// \return Current position of the mouse
            ///
            ////////////////////////////////////////////////////////////
            static sf::Vector2i getPosition(const X11Window& relativeTo);
        };
    }
}
#endif
