#include <cassert>
#include <sstream>
#include "NotifcationIcon.h"
using namespace std;

// I don't know why these are missing
#define NIIF_USER 0x4
#define NIF_SHOWTIP 0x80

// strncpy won't null terminate strings if strlen(src) >= max
void copyAndTermainte(char *dst, const char *src, int max) {
    strncpy(dst, src, max);
    dst[max - 1] = 0;
}

NotifcationIcon::NotifcationIcon() {
    memset(&nid, 0, sizeof nid);
    nid.cbSize = sizeof nid;
    nid.uFlags = NIF_STATE | NIF_GUID | NIF_MESSAGE |NIF_SHOWTIP;
    nid.uCallbackMessage = callbackHash();
    nid.dwState = NIS_HIDDEN;
    nid.dwStateMask = nid.dwState;
    nid.uVersion = NOTIFYICON_VERSION_4;
    nid.dwInfoFlags = NIIF_USER;
    assert(createGUID(&nid.guidItem));

    icon_installed = false;
}

void NotifcationIcon::add() {
    winAPI(NIM_ADD);
    winAPI(NIM_SETVERSION);
    icon_installed = true;
}

void NotifcationIcon::remove() {
    winAPI(NIM_DELETE);
    icon_installed = false;
}

bool NotifcationIcon::installed() {
    return icon_installed;
}

void NotifcationIcon::setIcon(HICON ico) {
    nid.hIcon = ico;
    nid.uFlags |= NIF_ICON;
    update();
}

void NotifcationIcon::setWindow(HWND window) {
    nid.hWnd = window;
    nid.uFlags |= NIF_ICON;
    update();
}

void NotifcationIcon::setBalloonIcon(HICON ico) {
    nid.hBalloonIcon = ico;
}

void NotifcationIcon::setTooltip(const char *tip) {
    const int TIP_MAX = 128;
    copyAndTermainte(nid.szTip, tip, TIP_MAX);
    nid.uFlags |= NIF_TIP;
    update();
}

void NotifcationIcon::hide() {
    nid.dwState = NIS_HIDDEN;
    update();
}

void NotifcationIcon::show() {
    nid.dwState &= ~NIS_HIDDEN;
    update();
}

void NotifcationIcon::showBalloon(const char *title, const char *text) {
    const int TITLE_MAX = 64;
    const int TEXT_MAX = 256;

    copyAndTermainte(nid.szInfoTitle, title, TITLE_MAX);
    copyAndTermainte(nid.szInfo, text, TEXT_MAX);

    nid.uFlags |= NIF_INFO;
    update();
}

NOTIFYICONDATA *NotifcationIcon::windows_nid() {
    return &nid;
}

void NotifcationIcon::update() {
    if(installed()) {
        winAPI(NIM_MODIFY);
    }
}

void NotifcationIcon::winAPI(DWORD msg) {
    SetLastError(0);
    BOOL ret = Shell_NotifyIcon(msg, &nid);
    DWORD err = GetLastError();
    if(!ret) {
        stringstream msg;
        msg << "Shell_NotifyIcon() failed with error " << err;
        std::string smsg = msg.str();
        throw NotifcationIconError(smsg);
    }
}

unsigned short NotifcationIcon::callbackHash() {
    unsigned long addr = (unsigned long)((void*)this);
    // Use the largest 16-bit prime as modulus
    unsigned long hash = addr % 65521;
    return (unsigned short)(hash);
}

bool NotifcationIcon::createGUID(GUID *guid) {
    HRESULT ret = CoCreateGuid(guid);
    return ret == 0;
}