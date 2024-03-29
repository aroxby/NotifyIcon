#ifndef _INC_NOTIFCATIONICON_H
#define _INC_NOTIFCATIONICON_H

#include <string>
#include <stdexcept>
#include <windows.h>
#include <Shellapi.h>

// Base class to use for library errors
class NotifcationIconError : public std::runtime_error {
public:
    explicit NotifcationIconError(const std::string& what_arg) : std::runtime_error(what_arg) {
    }
};

// Shell Notify Icon class
class NotifcationIcon {
public:
    NotifcationIcon(const GUID &guid);

    // Auto cleanup Windows data structures
    ~NotifcationIcon();

    // Quick setup method to reduce boilerplate code
    void setup(HWND win, HICON ico, const char *tip, UINT msg);

    // Install icon in windows
    void add();
    // Remove icon from windows
    void remove();
    // Retrieve installation (add/remove) status
    bool installed();

    // Set the window message used to send events to the parent window
    void setMessage(UINT msg);

    // Set icon
    void setIcon(HICON ico);

    // Set parent Window
    void setWindow(HWND window);

    // Set notification icon
    void setNotificationIcon(HICON ico);

    // Set tooltip
    void setTooltip(const char *tip);

    // Hide icon in system tray
    void hide();
    // Show icon in system tray
    void show();

    // Show a notification
    void showNotification(const char *title, const char *text);

    // Retrieve raw Windows API data structure
    NOTIFYICONDATA *windows_nid();

private:
    // Convenience method for winAPI(NIM_MODIFY);
    void update();
    // Wrapper around Shell_NotifyIcon
    void winAPI(DWORD msg);
    //Raw Windows API data structure
    NOTIFYICONDATA nid;
    // Remember icon installation status
    bool icon_installed;
};

#endif//_INC_NOTIFCATIONICON_H